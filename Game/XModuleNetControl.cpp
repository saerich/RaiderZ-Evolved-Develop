#include "stdafx.h"
#include "XModuleNetControl.h"
#include "XModuleEntity.h"
#include "XModuleAction.h"
#include "XModuleActorControl.h"
#include "XModuleMovable.h"
#include "XModuleTalent.h"
#include "XModuleBuff.h"
#include "XModuleEffect.h"
#include "XEventID.h"
#include "XObject.h"
#include "XController.h"
#include "XGame.h"
#include "XWorld.h"
#include "XGlueGameEvent.h"
#include "XEffectManager.h"
#include "XNonPlayer.h"
#include "XCharacter.h"
#include "XControlCmd.h"
#include "XNetActionState.h"
#include "XNetActionNA.h"
#include "XNetActionGrappled.h"
#include "XNetActionSwallowed.h"
#include "XNetActionSit.h"
#include "XNetActionGuard.h"
#include "XNetActionGesture.h"
#include "XNetActionHit.h"
#include "XNetActionTalent.h"
#include "XTalentHitParam.h"
#include "XGameTransformControllerManager.h"
#include "CSMotionFactorHelper.h"
#include "XTalentHelper.h"

#include "RDebugPassRenderer.h"

XNetPlayer* XModuleNetControl::GetOwner()
{ 
	return m_pOwner->AsNetPlayer();
}

XModuleNetControl::XModuleNetControl(XObject* pOwner) : XModuleActorControl(pOwner)
{
	m_pActionFSM = new XNetActionFSM(this);
	m_bSwitchingWeapon		= false;
	m_nSwitchingWeaponSet	= 0;
	m_bLooting				= false;
	m_fCurrAppearTime		= 0.0f;
	m_nMovementFlag			= 0;
	m_nTempSwallowedSynch	= 0;
	m_nMFState				= MF_NONE;

	m_pOverlappedEquipmentController = new XNetBPartWeaponController;
}

XModuleNetControl::~XModuleNetControl(void)
{
	m_pActionFSM->DeleteAll();
	SAFE_DELETE(m_pActionFSM);
}

void XModuleNetControl::OnInitialized()
{
	XModuleActorControl::OnInitialized();
	InitStates();
}

void XModuleNetControl::OnSubscribeEvent()
{
	XModuleActorControl::OnSubscribeEvent();

	Subscribe(XEVTL_MOTION_PLAYONCE);
	Subscribe(XEVTL_UPPER_MOTION_PLAYONCE);

	Subscribe(XEVTD_TALENT_HIT);
	Subscribe(XEVTD_DISAPPEAR_ME);

	Subscribe(XEVTL_ON_CHANGE_MOTION);
	Subscribe(XEVTL_ON_CHANGE_ANIMATION);
	Subscribe(XEVTL_ON_CHANGED_MOTION_TO_IDLE);

	Subscribe(XEVTL_THROW);
	Subscribe(XEVTL_GRAPPLED_TERRAIN_COL);

	Subscribe(XEVTL_SWALLOWED_DISAPPEAR);

	Subscribe(XEVTL_SIT_RISE);

	Subscribe(XEVTL_GUARD_KNOCKBACK);
	Subscribe(XEVTL_GUARDED);

	Subscribe(XEVTD_END_MF_STUN);
	Subscribe(XEVTD_END_MF_KNOCKDOWN);

	Subscribe(XEVTL_TALENT_START);
	Subscribe(XEVTL_TALENT_CANCELED);
	Subscribe(XEVTL_TALENT_ACT_SPELL);
	Subscribe(XEVTL_TALENT_RECASTING);

	Subscribe(XEVTL_NET_MOVE);
}


XEventResult XModuleNetControl::OnEvent(XEvent& msg)
{
	if (XModuleActorControl::OnEvent(msg) == MR_TRUE) return MR_TRUE;

	if (m_pActionFSM->Event(msg) == MR_TRUE) return MR_TRUE;

	switch (msg.m_nID)
	{
	case XEVTD_TALENT_HIT:
		{
			HitByTalent(static_cast<XTalentHitParam*>(msg.m_pData));
			SAFE_DELETE(msg.m_pData);
		}
		return MR_TRUE;
	case XEVTL_ON_CHANGE_MOTION:
	case XEVTL_ON_CHANGE_ANIMATION:
		{
		}
		return MR_TRUE;
	case XEVTL_ON_CHANGED_MOTION_TO_IDLE:
		{
			m_nMFState = MF_NONE;
		}
		return MR_FALSE;
	case XEVTL_MOTION_PLAYONCE:
		{
			if(m_bLooting)
			{
				//���ó����� �ִϰ� �� �÷��� �Ǹ� idle��.
				//�� �̰� �� �� �� �Ҽ� ����.
				m_pModuleAction->Idle();

				m_bLooting = false;
			}
		}
		return MR_TRUE;
	case XEVTD_DISAPPEAR_ME:
		{
			StartDisappear();
		}
		return MR_TRUE;
	}

	return MR_FALSE;
}

void XModuleNetControl::OnUpdate(float fDelta)
{
	PFC_THISFUNC;

	PFI_BLOCK_THISFUNC(305);
	XModuleActorControl::OnUpdate(fDelta);
}

void XModuleNetControl::OnUpdateState( float fDelta )
{
	m_pActionFSM->Update(fDelta);


	// Ż��Ʈ ���� ������Ʈ
	if ( m_pModuleTalent->IsActive())		
	{
		int nID = -1;
		if(m_pModuleTalent->GetInfo())
		{
			nID = m_pModuleTalent->GetInfo()->m_nID;
		}

		m_pModuleMovable->TriggerTalentActive(nID);
	}
	else									m_pModuleMovable->TriggerTalentDeActive();

	m_pModuleAction->UpdateNetMovementAction(fDelta);
	
	XModuleActorControl::OnUpdateState(fDelta);
}

void XModuleNetControl::OnUpdateCommand( float fDelta )
{
}

void XModuleNetControl::OnUpdateSpecial( float fDelta )
{
	_UpdateAimingShoot(fDelta);
}

// TODO: �̷��� ��ƾ�� ���� �־ �ǳ�...
// TODO: ���̹����϶� ��� ����� �ϱ�.
void XModuleNetControl::_UpdateAimingShoot(float fDelta)
{
	if( m_pAimingObj.empty() )
		return;

	XObject* pObject = m_pAimingObj.front();
	if (pObject == NULL) 
	{
		_ASSERT(0);	// pObject�� �ΰ��� ���� ���� �ֳ׿�. ��� ���Ͻ��׽��ϴ�. - birdkr(2007-05-04)
		return;
	}

	vec3 ownerPos = m_pModuleEntity->GetPosition();
	vec3 ownerDir = m_pModuleEntity->GetDirection();
	vec3 objPos	  = pObject->GetPosition();
	vec3 dir = objPos - ownerPos;
	dir.Normalize();

	vec3 townerPos	= ownerPos; townerPos.z = 0.f;
	vec3 tobjPos	= objPos;	tobjPos.z = 0.f;
	vec3 tDir		= tobjPos - townerPos;
	tDir.Normalize();

	float angle = ownerDir.DotProduct(tDir);
	if(angle > 0.999f)
	{
		m_pOwner->SetDirection(tDir);
		m_pModuleAction->Shoot();

		XModuleEntity* pObjModuleEntity = pObject->GetModuleEntity();
		XCharacter* pActor = pObjModuleEntity->GetActor();
		if(pActor)
			global.emgr->AddXEffect(L"dmg_melee", pActor, NULL);

		m_pAimingObj.pop_front();

		if( !m_pAimingObj.empty() )
		{
			m_pModuleAction->ActionAiming();
		}
	}
	else
	{
		// TODO: ���߿� Ƣ�ų� �̻��� ������ ���ʹϾ�����...
		// 1.0f õõ��, 0.3f ���� ���� ȸ��.
		vec3 dd = ownerDir.GetInterpolated(dir, (0.6f - fDelta));
		m_pOwner->SetDirection(dd);
	}
}

void XModuleNetControl::OnUseTalent(int nTalentID, vec3& vPos, vec3& vDir)
{
	if ( GetOwner()->IsDead())
		return;

	// ���� ����
	m_pOwner->SetFacingDir( vDir);
	XMath::NormalizeZ0( vDir);
	m_pOwner->SetDirection( vDir);

	// [2007/8/1 isnara] ���尡 Ǯ�� �޼����� �ȿ��� ���� �޼����� ���� ��쿡 Ǭ��.
	XNetPlayer * pNetPlayer = GetOwner();
	pNetPlayer->SetGuard( false);

	// �������̸� ������ ����.
	m_pModuleAction->EndStun();

	if(XCONST::DEBUG_SHOW_INTERPOLATION_TARGET_INFO_NETPLAYER)
	{
		vec3 bottom = vPos + vec3(0, 0, 10);
		vec3 top	= bottom + vec3(0, 0, 80);
		// ���ӷ��� ��
		REngine::GetSceneManager().GetPassRenderer< RDebugPassRenderer >()->AddDebugCapsule(MCapsule(bottom, top, 10), 5, D3DCOLOR_XRGB(0, 255, 255));
	}

	XMovableGoParam* pGoParam = new XMovableGoParam(XMovableGoParam::E_GO_INTERPOLATION);
	pGoParam->Set( vPos, vDir, GetOwner()->GetMoveSpeedDefault( m_nMovementFlag), 400.0f, BOOST_NONE, m_nMovementFlag & 0x000F, true, 1.0f);
	m_pModuleMovable->TriggerGo( pGoParam);
	delete pGoParam;

	DoAction(NET_ACTION_STATE_TALENT);

	XModuleActorControl::OnUseTalent(nTalentID, vPos, vDir);
}

void XModuleNetControl::OnJump(vec3& vPos, vec3& vDir)
{
	if ( GetOwner()->IsDead())
		return;

	// �������̸� ������ ����.
	m_pModuleAction->EndStun();

	// �׼ǿ� ���������� �˷��ֱ�, ���� �Ӽ��� ����
	// �׼ǿ��� ���� �ִϸ��̼� ����
	m_nMovementFlag = MOVEMENTFLAG_JUMPING;
	m_pModuleAction->Jump(MT_DEFAULT,true);


	XMovableFactor::SInterpolation param = m_pModuleMovable->GetMovableFactor().INTERPOLATION;
	
	const float _speed = /*450.0f*/ XCONST::MOVE_SPEED_NONE * param.m_fJumpSpeedRatio;
	vec3 _pos = vPos + vDir * _speed;
	
	XMovableGoParam* pGoParam = new XMovableGoParam(XMovableGoParam::E_GO_INTERPOLATION);
	pGoParam->Set( _pos, param.m_vTargetDir, _speed, 200.0f, BOOST_NONE, m_nMovementFlag & 0x000F, true);
	m_pModuleMovable->TriggerGo( pGoParam);
	delete pGoParam;
}

void XModuleNetControl::OnFalling(vec3& pos, vec3& dir)
{
	if ( GetOwner()->IsDead())
		return;


	m_nMovementFlag = MOVEMENTFLAG_FALLING;		// ���� �÷��׸� �Է��� �ش�.


	XMovableGoParam* pGoParam = new XMovableGoParam(XMovableGoParam::E_GO_INTERPOLATION);
	pGoParam->Set( pos, dir, GetOwner()->GetMoveSpeedDefault( m_nMovementFlag), 200.0f, BOOST_NONE, m_nMovementFlag & 0x000F, true, 1.0f);
	m_pModuleMovable->TriggerGo( pGoParam);
	delete pGoParam;

	m_pModuleAction->ActionFalling();
}

void XModuleNetControl::OnMove(TD_PC_MOVE* pMI)
{
	if ( GetOwner()->IsDead())
		return;


	// �̵� ���� ����
	m_nMovementFlag	= pMI->nFlags;

	vec3 _diff = pMI->vTarPos - GetOwner()->GetPosition();
	vec3 _dir = Normalize( _diff);
	float _dist = _diff.Length();
	float _speed = _dist / 0.3f;															// �ӵ��� ��ǥ �������� 0.3�� �̳��� ������ �� �ֵ��� �����Ѵ�
	vec3 _target = pMI->vTarPos + _dir * min(_speed * 0.1f, VIRTUAL_POSITION_VELOCITY);		// ���� Ÿ���� ��ǥ �������� ���� �ӵ��� 0.1�� �� �̵��� ������ ��´�


	vec3 vTransTarDir;
	vTransTarDir.x = pMI->svDir.X();
	vTransTarDir.y = pMI->svDir.Y();
	vTransTarDir.z = 0.0f;

	// �Ÿ����� �� ���� �� ��쿣 ������Ų��.
	if ( _dist > PROCESS_MOVE_EXCEPTION_DISTANCE)
	{
#ifdef _DEBUG
		mlog( "�̵� ���� ���� �ʰ�. �����Ǿ����ϴ�. (time=%u)\n", timeGetTime());
#endif
		_target = pMI->vTarPos;
		_speed = 100.0f;
		GetOwner()->SetPosition( pMI->vTarPos);


		GetOwner()->SetDirection( vTransTarDir);
	}

	if(XCONST::DEBUG_SHOW_INTERPOLATION_TARGET_INFO_NETPLAYER)
	{
		vec3 bottom = _target + vec3(0, 0, 10);
		vec3 top	= bottom + vec3(0, 0, 80);
		// ��ȫ��
		REngine::GetSceneManager().GetPassRenderer< RDebugPassRenderer >()->AddDebugCapsule(MCapsule(bottom, top, 10), 5, D3DCOLOR_XRGB(0, 0, 255));
	}

	XEvent msg(XEVTL_NET_MOVE);
	m_pOwner->Event(msg);

	// �̵� ����
	XMovableGoParam* pGoParam = new XMovableGoParam(XMovableGoParam::E_GO_INTERPOLATION);
	pGoParam->Set( _target, vTransTarDir, _speed, CONST_MOVABLE_INTERPOLATION_DEFAULT_ROTATION_SPEED, BOOST_NONE, m_nMovementFlag & 0x000F, true, 1.0f);
	m_pModuleMovable->TriggerGo( pGoParam);
	delete pGoParam;


	// �������̸� ������ ����.
	m_pModuleAction->EndStun();
}

void XModuleNetControl::OnStop(vec3* pPos, vec3* pDir)
{
	// ó������ �ʴ� ��Ȳ
	if ( GetOwner()->IsDead() &&
		m_pModuleMovable->GetMovableFactor().IsGrappled() == false)
		return;

	if(XCONST::DEBUG_SHOW_INTERPOLATION_TARGET_INFO_NETPLAYER)
	{
		vec3 bottom = *pPos + vec3(0, 0, 10);
		vec3 top	= bottom + vec3(0, 0, 80);
		// �Ķ���
		REngine::GetSceneManager().GetPassRenderer< RDebugPassRenderer >()->AddDebugCapsule(MCapsule(bottom, top, 10), 5, D3DCOLOR_XRGB(255, 0, 255));
	}

	// ������ ���� ����
	unsigned int nOldFlags = m_nMovementFlag;
	vec3 curr_pos( m_pOwner->GetPosition());
	vec3 minus_diff(*pPos - curr_pos);
	float fDist = minus_diff.LengthSq();
	if ( fDist > FLT_EPSILON)
	{
		XMovableGoParam* pGoParam = new XMovableGoParam(XMovableGoParam::E_GO_INTERPOLATION);
		pGoParam->Set( *pPos, *pDir, GetOwner()->GetMoveSpeedDefault( m_nMovementFlag), 200.0f, BOOST_NONE, MOVEMENTFLAG_FORWARD, true, 1.0f);
		m_pModuleMovable->TriggerGo( pGoParam);
		delete pGoParam;
	}
	else
	{
		m_nMovementFlag = 0;
	}
}

void XModuleNetControl::OnMoveStop(vec3* pPos)
{
	// ó������ �ʴ� ��Ȳ
	if ( GetOwner()->IsDead())
		return;


	if(XCONST::DEBUG_SHOW_INTERPOLATION_TARGET_INFO_NETPLAYER)
	{
		vec3 bottom = *pPos + vec3(0, 0, 10);
		vec3 top	= bottom + vec3(0, 0, 80);
		// �Ķ���
		REngine::GetSceneManager().GetPassRenderer< RDebugPassRenderer >()->AddDebugCapsule(MCapsule(bottom, top, 10), 5, D3DCOLOR_XRGB(0, 0, 125));
	}

	XMovableGoParam* pGoParam = new XMovableGoParam(XMovableGoParam::E_GO_INTERPOLATION);
	XMovableFactor::SInterpolation param = m_pModuleMovable->GetMovableFactor().INTERPOLATION;
	pGoParam->Set( *pPos, param.m_vTargetDir, GetOwner()->GetMoveSpeedDefault( m_nMovementFlag), 200.0f, BOOST_NONE, m_nMovementFlag & 0x000F, true, 1.0f);
	m_pModuleMovable->TriggerGo( pGoParam);
	delete pGoParam;
}

void XModuleNetControl::OnRotate(vec3& dir)
{
	if ( GetOwner()->IsDead())
		return;

	XMovableGoParam* pGoParam = new XMovableGoParam(XMovableGoParam::E_GO_INTERPOLATION);
	XMovableFactor::SInterpolation param = m_pModuleMovable->GetMovableFactor().INTERPOLATION;
	pGoParam->SetRotate( dir, 200.0f);
	m_pModuleMovable->TriggerRotate( pGoParam);
	delete pGoParam;

	if(XCONST::DEBUG_SHOW_INTERPOLATION_TARGET_INFO_NETPLAYER)
	{
		vec3 bottom = param.m_vTargetPos + vec3(0, 0, 10);
		vec3 top	= bottom + vec3(0, 0, 80);
		// �Ķ���
		REngine::GetSceneManager().GetPassRenderer< RDebugPassRenderer >()->AddDebugCapsule(MCapsule(bottom, top, 10), 5, D3DCOLOR_XRGB(255, 255, 255));
		REngine::GetSceneManager().GetPassRenderer< RDebugPassRenderer >()->AddDebugLine(top, top + (dir * 30.0f), 5, D3DCOLOR_XRGB(255, 255, 255));

		MMatrix mat;
		mat.MakeIdentity();
		mat.SetTranslation(top + (dir * 33.0f));
		REngine::GetSceneManager().GetPassRenderer< RDebugPassRenderer >()->AddDebugSphere(mat, 3.0f, 5, D3DCOLOR_XRGB(255, 255, 255));
	}

}

void XModuleNetControl::OnDie( MF_STATE nMFState /*= MF_NONE*/, bool bSetAni /*= true*/ )
{
	GetOwner()->OnDie();

	m_pModuleTalent->Cancel();

	if(m_bReservedHitByTalent == false)
		m_pModuleAction->Die();
}

void XModuleNetControl::OnRebirth()
{
	GetOwner()->OnRebirth();

	m_pModuleAction->ActionRebirth();
}

// TODO: pHitInfoNodes ó���ؾߵȴ�.
void XModuleNetControl::OnAttackedGun(vec3& vPos, vec3& vDir, MUID& uidTarget, bool& bHitted)
{
	m_pOwner->SetPosition(vPos);
	m_pOwner->SetDirection( vDir);

	XObject* pObject = gg.omgr->Find(uidTarget);
	if (pObject == NULL)
		return;

	m_pAimingObj.push_back(pObject);
}

void XModuleNetControl::OnChangeStance(CHAR_STANCE nStance)
{
	//m_pModuleAction->Stop();
}

void XModuleNetControl::OnActTalent( int nTalentID, vec3& vDir )
{
	m_pOwner->SetFacingDir( vDir);
	vDir.z = 0;
	vDir.Normalize();

	m_pOwner->SetDirection( vDir);

	XModuleActorControl::OnActTalent(nTalentID, vDir);
}

void XModuleNetControl::OnCancelTalent()
{
	XModuleActorControl::OnCancelTalent();

	// ���� ���� ��Ʈ�Ӽ��̳� �����Ӽ��� ������ �ɷ��ִٸ�... Stop �ִϸ��̼��� ���� �ʴ´�.
	// �׾��ٸ�...
	if(m_pModuleBuff->GetBuffAttribute().IsMesmerizable() ||
		GetOwner()->IsDead())
		return;

	m_pModuleAction->Stop();
}

void XModuleNetControl::OnActSpellProjectile(int nTalentID, vec3& vEntityDir, vec3& vTargetPos, MUID& uidTarget, int nGroupID, int nCapsuleID)
{
	m_pModuleTalent->SetProjectileTargetPos(vTargetPos);

	XModuleActorControl::OnActSpellProjectile(nTalentID, vEntityDir, vTargetPos, uidTarget, nGroupID, nCapsuleID);
}

void XModuleNetControl::OnActArchery( int nTalentID, vec3& vDir, MUID& uidTarget )
{
	m_pOwner->SetFacingDir( vDir);
	vDir.z = 0;
	vDir.Normalize();

	m_pOwner->SetDirection( vDir);

	if (uidTarget != MUID::Invalid())
	{
		m_pModuleTalent->SetTarget(uidTarget);
	}

	XModuleActorControl::OnActArchery(nTalentID, vDir, uidTarget);
}

void XModuleNetControl::OnActArcheryDetail( int nTalentID, vec3& vDir, MUID& uidTarget, int nGroupID, int nCapsuleID )
{
	m_pOwner->SetFacingDir( vDir);
	vDir.z = 0;
	vDir.Normalize();

	m_pOwner->SetDirection( vDir);

	if (uidTarget != MUID::Invalid())
	{
		m_pModuleTalent->SetTarget(uidTarget, nGroupID, nCapsuleID);
	}

	XModuleActorControl::OnActArcheryDetail(nTalentID, vDir, uidTarget, nGroupID, nCapsuleID);
}

void XModuleNetControl::OnGuard(vec3& pos, vec3& dir)
{
	//m_pModuleAction->Stop();
	//m_pModuleAction->Guard();

	//XNetPlayer * pNetPlayer = GetOwner();
	//if(pNetPlayer) 
	//	pNetPlayer->SetGuard(true);
	DoAction(NET_ACTION_STATE_GUARD);
}

void XModuleNetControl::OnGuardReleased()
{
	//mlog("net guard released\n");
	//// ���� �˹���¶�� ���帱���� �ִϸ��̼��� ���� �ʴ´�.
	//if(IsCurrentMotionGuardKnockback() == false)
	//{
	//	m_pModuleAction->ActionGuardRelease();
	//}

	//XNetPlayer * pNetPlayer = GetOwner();
	//if(pNetPlayer)
	//	pNetPlayer->SetGuard(false);

	DoAction( NET_ACTION_STATE_NA);
}

void XModuleNetControl::OnHit(XTalentHitParam* pHitInfo)
{
	// OnHit�϶� ������ Stop�� �ϸ� ��Ÿ�϶� ù��°�� ���� ��������� ���� �ʴ´�.
	//m_pModuleAction->Stop();
	XModuleActorControl::OnHit(pHitInfo);
}

void XModuleNetControl::HitByTalent(XTalentHitParam * pHitInfo)
{
	CheckReservedHitByTalent();

	XObject* pAttackObject = gg.omgr->FindActor_UIID(pHitInfo->nAttackerUIID);
	if(m_pModuleTalent == NULL)
		return;

	m_bSwitchingWeapon = false;

	if (pAttackObject && pAttackObject->GetModuleTalent())
	{
		pAttackObject->GetModuleTalent()->OnHitTarget(pHitInfo->nTalentID);
	}

	bool bDieCheck				= false;

	// �׾��ٸ�.. �ǰ��� �������� �ʴ´�.
	//if(m_DeathController.IsDeadActionImmediatelyOnHit(GetOwner(), pHitInfo->nMFState))
	//{
	//	OnDie();
	//}
	//else
	{
		// �׾��°�? �����Ŷ�� �ٷ� ���� �� ���� MF �ϰ��̴�.
		//if(GetOwner()->IsDead())
		//{
		//	m_pModuleAction->MFDie();
		//}

		// ��Ʈ�� �Ҽ� �ִ����� ���� ������� Ż��Ʈ�� ���Ѵ�.
		bool bHitable = true;
		if (m_pModuleTalent->IsActive())
		{
			// MF_NONE�϶� �ŷ�Ʈ ������̶��... ������͸� ���Ѵ�. 
			// ����(��ȿ)�� ��Ұ� �ȵȴ�.
			if ( (pHitInfo->IsFlagImmune() == false && 
				(m_pModuleTalent->GetInfo()->m_bCancelable && 
				XTalentHelper::IsTalentCancelableMotionFactor(pHitInfo->nMFState) )) ||
				m_pModuleTalent->GetInfo()->m_nSkillType == ST_SOCIAL)						// �Ҽ� �ŷ�Ʈ ������̸� ��Ұ� �ǰ�
			{
				// ������̴� Ż��Ʈ �������
				m_pModuleTalent->Cancel();
			}
			else
			{
				bHitable = false;
			}
		}

		if(pHitInfo->nMFState != MF_GRAPPLED &&
			pHitInfo->nMFState != MF_SWALLOWED)
		{
			// �����Ϳ� ���� ��Ʈ �������
			if (bHitable)
			{
				DoAction(NET_ACTION_STATE_HIT, (void*)(pHitInfo));
				bDieCheck = true;
			}
		}
		else
		{
			if(pHitInfo->nMFState == MF_GRAPPLED)
			{
				// ���
				_DamageRepeatInfo * pDamageInfo = InitDamageRepeat(pHitInfo);
				if(StartGrappled(pDamageInfo) == false)
					SAFE_DELETE(pDamageInfo);
			}
			else if(pHitInfo->nMFState == MF_SWALLOWED)
			{
				// �Ա�
				_DamageRepeatInfo * pDamageInfo = InitDamageRepeat(pHitInfo);
				if(StartSwallowed(pDamageInfo) == false)
					SAFE_DELETE(pDamageInfo);
			}
		}

		// ���� ó��...
		if(bDieCheck == false && GetOwner()->IsDead())
		{
			OnDie(pHitInfo->nMFState);
		}
	}

	m_nMFState						= pHitInfo->nMFState;

	// �ǰݹ� ������ �����ֱ�
	_DamageInfo damageInfo;
	damageInfo.nDamage				= pHitInfo->nDamage;
	damageInfo.nFlags				= pHitInfo->nFlags;
	damageInfo.nMFState				= pHitInfo->nMFState;
	damageInfo.nAttackedTalentID	= pHitInfo->nTalentID;
	damageInfo.nAttackerUIID		= pHitInfo->nAttackerUIID;
	damageInfo.bSecondaryEffect		= pHitInfo->bSecondaryEffect;

	ShowHitAndDamage(&damageInfo, pAttackObject);
	UpdateHitDamage(pHitInfo->nDamage);			// ������ ���

	FakeBeatenAndSetMultiplyColor(pHitInfo);

	BirdTest_ShowDelay(pHitInfo);
}


void XModuleNetControl::OnSwitchingWeapon( int8 nWeaponSet )
{
	//mlog("�� ����Ī %d\n", nWeaponSet);
	m_nSwitchingWeaponSet	= nWeaponSet;

	// ���� ����
	DoSwitchingWeapon();
}

void XModuleNetControl::OnSwitchingWeaponBegin()
{
	//mlog("�� ü��¡ ����\n");
	if (GetOwner()->IsCurrStanceBattle())
	{
		// �������¸� Ǭ��.
		bool bQuick = false;
		if(gvar.MyInfo.Talent.HasExtraPassiveTalent(TEPT_QUICK_DRAW)) 
		{
			bQuick = true;
		}

		GetOwner()->ChangeWeapon(CS_NORMAL, false);
		m_bSwitchingWeapon		= true;
	}
}

void XModuleNetControl::DoSwitchingWeapon()
{
	//mlog("�� ���� %d\n", m_nSwitchingWeaponSet);
	GetOwner()->SwitchingWeaponSet(m_nSwitchingWeaponSet);

	if (GetOwner()->IsCurrStanceBattle() || m_bSwitchingWeapon)
	{
		GetOwner()->ChangeWeapon(CS_BATTLE, false);
	}

	m_bSwitchingWeapon		= false;
	m_nSwitchingWeaponSet	= 0;
}

void XModuleNetControl::OnHitLooting()
{
	m_bLooting = false;

	OnLootEnd();
}

void XModuleNetControl::DoActionLootingItem( MUID& uidNPC, float fLootTime/*=0.f*/ )
{
	XNonPlayer* pTargetNPC = gg.omgr->FindNPC(uidNPC);
	if(NULL == pTargetNPC ) return;

	m_pOwner->GetModuleAction()->ChangeLootingMotion(pTargetNPC->GetInfo()->strInteractionLootAni.c_str());
	m_bLooting = true;
}

void XModuleNetControl::OnLootEnd()
{
	DoActionIdle();
}

bool XModuleNetControl::StartGrappled(_DamageRepeatInfo* pInfo)
{
	if ( m_pActionFSM->GetCurrentStateID() == NET_ACTION_STATE_GRAPPLED)
	{
		XEvent evt( XEVTL_GRAPPLED_NEW_START, (void*)( pInfo));
		m_pActionFSM->Event( evt);
	}
	else
	{
		DoAction(NET_ACTION_STATE_GRAPPLED, (void*)(pInfo));

		// Ȯ��
		if(m_pActionFSM->GetCurrentStateID() != NET_ACTION_STATE_GRAPPLED)
		{
			return false;
		}
	}

	return true;
}
bool XModuleNetControl::StartSwallowed(_DamageRepeatInfo* pInfo)
{
	if ( m_pActionFSM->GetCurrentStateID() == NET_ACTION_STATE_SWALLOWED)
	{
		XEvent evt( XEVTL_SWALLOWED_NEW_START, (void*)( pInfo));
		m_pActionFSM->Event( evt);
	}
	else
	{
		DoAction(NET_ACTION_STATE_SWALLOWED, (void*)(pInfo));
	
		// Ȯ��
		if(m_pActionFSM->GetCurrentStateID() != NET_ACTION_STATE_SWALLOWED)
		{
			return false;
		}
	}

	return true;
}

XModuleNetControl::E_DIR_TYPE XModuleNetControl::CalcMovementDirFromFlag(const vec3& vDir, int nFlag, vec3& _rOutPut)
{
	E_DIR_TYPE eDirType = E_DIR_NORMAL;
	bool bMove = false;

	if (nFlag & MOVEMENTFLAG_FORWARD) 
	{
		_rOutPut	= vDir;
		bMove		= true;
		eDirType	= E_DIR_FORWARD;
	}
	else if (nFlag & MOVEMENTFLAG_BACKWARD)
	{
		_rOutPut	= -vDir;
		bMove		= true;
		eDirType	= E_DIR_BACKWARD;
	}

	if (nFlag & MOVEMENTFLAG_LEFT)
	{
		vec3 left = -CrossProduct(vDir, UP_VECTOR);
		left.z = 0.0f;
		left.Normalize();

		if( bMove)
		{
			_rOutPut += left;
			_rOutPut.Normalize();
		}
		else
			_rOutPut = left;

		if (eDirType == E_DIR_NORMAL)
		{
			eDirType = E_DIR_SIDE;
		}
	}
	else if (nFlag & MOVEMENTFLAG_RIGHT)
	{
		vec3 right = CrossProduct(vDir, UP_VECTOR);
		right.z = 0.0f;
		right.Normalize();

		if( bMove)
		{
			_rOutPut += right;
			_rOutPut.Normalize();
		}
		else
			_rOutPut = right;

		if (eDirType == E_DIR_NORMAL)
		{
			eDirType = E_DIR_SIDE;
		}
	}

	return eDirType;
}

bool XModuleNetControl::IsCanChangeAnimation(CHANGE_ANI_STATE nState, unsigned int nFlage /* = MT_DEFAULT */)
{
	XNetPlayer * pNetPlayer = GetOwner();

	// ������
	if(pNetPlayer &&
		nState == CHANGEANI_STOP)
	{
		if((nFlage & MOVEMENTFLAG_GUARD) ||						// ����
			pNetPlayer->GetGuard() ||							// ����			
			m_bLooting == true)									// ����
		{
			return false;
		}
		else if (m_pActionFSM->CheckFlag(NACT_CANNOT_CHANGE_ANIMATION))
		{
			return false;
		}
	}

	return true;
}

void XModuleNetControl::InitStates()
{
	m_pActionFSM->AddState(new XNetActionNA(this));
	m_pActionFSM->AddState(new XNetActionGrappled(this));
	m_pActionFSM->AddState(new XNetActionSwallowed(this));
	m_pActionFSM->AddState(new XNetActionSit(this));
	m_pActionFSM->AddState(new XNetActionGuard(this));
	m_pActionFSM->AddState(new XNetActionGesture(this));
	m_pActionFSM->AddState(new XNetActionHit(this));
	m_pActionFSM->AddState(new XNetActionTalent(this));

	m_pActionFSM->ChangeState(NET_ACTION_STATE_NA);
}

void XModuleNetControl::DoAction( NET_ACTION_STATE nState, void* pParam/*=NULL*/ )
{
	if(m_pActionFSM->GetCurrentStateID() == NET_ACTION_STATE_HIT && nState == NET_ACTION_STATE_HIT)
	{
		m_pActionFSM->GetCurrentState()->Exit();
		if (m_pActionFSM->GetCurrentState()->CheckEnterable(pParam))
			m_pActionFSM->GetCurrentState()->Enter(pParam);
		else
			return DoAction(NET_ACTION_STATE_NA, pParam);
	}
	else if (m_pActionFSM->GetCurrentStateID() != nState)
	{
		m_pActionFSM->ChangeState(nState, pParam);
	}
}

void XModuleNetControl::DoActionQuestInteraction()
{
	m_pOwner->GetModuleAction()->ChangeMotionQuestInteraction();
}

void XModuleNetControl::DoActionIdle()
{
	XEvent evt;
	evt.m_nID = XEVTL_ON_MOTION_EVENT;
	evt.m_pData = MOTION_EVENT_NEXT_MOTION;
	m_pOwner->Event(evt);
}

bool XModuleNetControl::IsCurrentGuard()
{
	return GetOwner()->GetGuard();
}

void XModuleNetControl::OnSwallowedSynch( int nTalentID, MUID& uidAttacker )
{
	_DamageRepeatInfo* pNewDamageInfo	= new _DamageRepeatInfo;

	pNewDamageInfo->nTalentID	= nTalentID;
	pNewDamageInfo->nMFState			= MF_SWALLOWED;
	pNewDamageInfo->nMFWeight			= m_nTempSwallowedSynch;
	pNewDamageInfo->nAttackerUIID		= UIID_INVALID;

	XActor* pAttacker = gg.omgr->FindActor(uidAttacker);
	if (pAttacker)
	{
		pNewDamageInfo->nAttackerUIID		= pAttacker->GetUIID();
	}

	if(StartSwallowed(pNewDamageInfo) == false)
		SAFE_DELETE(pNewDamageInfo);

	m_nTempSwallowedSynch				= 0;
}

void XModuleNetControl::WaitSwallowedSynch(int nMFWeight)
{
	m_nTempSwallowedSynch = nMFWeight;
}

void XModuleNetControl::DoActionSitDown()
{
	DoAction(NET_ACTION_STATE_SIT);
}

void XModuleNetControl::DoActionSitRise()
{
	XEvent msg(XEVTL_SIT_RISE);
	m_pOwner->Event(msg);
}

void XModuleNetControl::OnReInPlayer(TD_UPDATE_CACHE_PLAYER* pPlayerInfo)
{
	vec3 vDir;
	vDir = pPlayerInfo->svDir;

	OnReInField(pPlayerInfo->vPos, vDir);

	GetOwner()->SetBuff(pPlayerInfo);
}

void XModuleNetControl::OnGuardKnockBack(vec3& vDir)
{
	XEvent msg(XEVTL_GUARD_KNOCKBACK);
	m_pActionFSM->Event(msg);

	XModuleActorControl::OnGuardKnockBack(vDir);
}

void XModuleNetControl::OnGuarded()
{
	// ���� ���� �׼� ���¶��... ���忡���� ������ �˷��� ó������
	XEvent msg(XEVTL_GUARDED);
	m_pActionFSM->Event(msg);

	XModuleActorControl::OnGuarded();
}

void XModuleNetControl::DoActionGesture( const wchar_t* szMotionName )
{
	DoAction(NET_ACTION_STATE_GESTURE, (void*)szMotionName);

}

void XModuleNetControl::OnPostUpdate( float fDelta )
{
	m_pActionFSM->PostUpdate(fDelta);

	XModuleActorControl::OnPostUpdate(fDelta);
}

void XModuleNetControl::DoActionDeactivate( const int nBuffID, const wchar_t* szUseAni )
{
	m_bSwitchingWeapon = false;
}

void XModuleNetControl::HitByBuff( XTalentHitParam * pHitInfo )
{
	if(pHitInfo)
		HitByTalent(pHitInfo);
}

void XModuleNetControl::SetAFK( bool bAFK )
{
	XNetPlayer * pNetPlayer = GetOwner();
	if (pNetPlayer->IsDead()) return;

	pNetPlayer->SetAFK(bAFK);

	XPlayer* pMyPlayer = gg.omgr->FindPlayer(XGetMyUID());
	if (pMyPlayer == NULL) return;
	
	if (bAFK)
	{
		DoActionSitDown();

		if(pNetPlayer->IsDuel() || !pNetPlayer->IsAlly(pMyPlayer))
			pNetPlayer->SetCharacterPane_VSNetPlayer();
		else
			pNetPlayer->SetCharacterPane_Player();
	}
	else
	{
		if(pNetPlayer->IsDuel() || !pNetPlayer->IsAlly(pMyPlayer))
			pNetPlayer->SetCharacterPane_VSNetPlayer();
		else
			pNetPlayer->SetCharacterPane_Player();
	}
}
