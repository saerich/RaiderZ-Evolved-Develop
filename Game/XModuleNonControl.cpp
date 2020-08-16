#include "StdAfx.h"
#include "XNonPlayer.h"
#include "XModuleNonControl.h"
#include "XModuleEntity.h"
#include "XModuleTalent.h"
#include "XModuleAction.h"
#include "XModuleCollision.h"
#include "XModuleBuff.h"
#include "XNPCActionState.h"
#include "XEventID.h"
#include "XGame.h"
#include "XWorld.h"
#include "XTalentInfo.h"
#include "XModuleMovable.h"
#include "XModuleEffect.h"
#include "XControlCmd.h"
#include "XTalentInfoMgr.h"
#include "XTalentHelper.h"
#include "XTalentHitParam.h"
#include "XGameTransformControllerManager.h"
#include "XNPCActionNA.h"
#include "XNPCActionMove.h"
#include "XNPCActionTalent.h"
#include "XNPCActionDie.h"
#include "XNPCActionHit.h"
#include "XNPCAligner.h"
#include "XCameraManager.h"

#include "RDebugPassRenderer.h"
#include "RCameraSceneNode.h"

//#define _DEBUG_LOG_REV_TIME

#define DELETE_ME_TIME			DISAPPEAR_TIME + 500
#define DISAPPEAR_START_TIME	0

XNonPlayer* XModuleNonControl::GetOwner()
{ 
	return AsNPC(m_pOwner); 
}

XModuleNonControl::XModuleNonControl(XObject* pOwner) 
: XModuleActorControl(pOwner)
{
	m_pActionFSM = new XNPCActionFSM(this);
	m_pAligner = new XNPCAligner();
}

XModuleNonControl::~XModuleNonControl(void)
{
	m_pActionFSM->DeleteAll();
	SAFE_DELETE(m_pActionFSM);

	SAFE_DELETE(m_pAligner);
}

void XModuleNonControl::OnInitialized()
{
	XModuleActorControl::OnInitialized();
	InitStates();
}

void XModuleNonControl::OnSubscribeEvent()
{
	XModuleActorControl::OnSubscribeEvent();

	Subscribe(XEVTL_MOTION_PLAYONCE);
	Subscribe(XEVTL_ON_CHANGED_MOTION_TO_IDLE);

	Subscribe(XEVTD_TALENT_HIT);
	Subscribe(XEVTD_DISAPPEAR_ME);

	Subscribe(XEVTD_NPC_SAY_END);	

	Subscribe(XEVTL_INTERPOLATION_MOVE_ARRIVED);
}

XEventResult XModuleNonControl::OnEvent(XEvent& msg)
{
	if (XModuleActorControl::OnEvent(msg) == MR_TRUE) return MR_TRUE;

	if (m_pActionFSM->Event(msg) == MR_TRUE) return MR_TRUE;

	switch(msg.m_nID)
	{
	case XEVTD_TALENT_HIT:
		{
			HitByTalent(static_cast<XTalentHitParam*>(msg.m_pData));
			SAFE_DELETE(msg.m_pData);
		}
		return MR_TRUE;
	case XEVTD_DISAPPEAR_ME:
		{
			StartDisappear();
			ReleaseObjSound();
		}
		return MR_TRUE;
	case XEVTD_NPC_SAY_END:
		{
			XBuffAttribute attrBuff = m_pOwner->GetModuleBuff()->GetBuffAttribute();
			// ModuleAction()�� ���� �ɵ�
			// �ŷ�Ʈ ���� Stop ���Ѵ�.
			if(m_pModuleTalent->IsActive() == false &&
				attrBuff.IsMesmerizable() == false)
				DoNPCStop();
		}
		return MR_TRUE;
	case XEVTD_MESH_LOADING_COMPLETE:
		{
			LoadObjSound();

			GetOwner()->CheckAnimation();
		}
		break;
	case XEVTL_INTERPOLATION_MOVE_ARRIVED:
		{
			// �̵����� üũ
			CheckMoveState();
		}
		break;
	}

	return MR_FALSE;
}

void XModuleNonControl::LoadObjSound()
{
	const char* SET_SOUND		= "set_sound";
	const char* SET_SOUND_NAME	= "set_sound_name";
	const char* SET_SOUND_MIN	= "set_sound_minradius";
	const char* SET_SOUND_MAX	= "set_sound_maxradius";
	
	RActor* pActor = static_cast<RActor*>(m_pModuleEntity->GetActor());
	int count = pActor->GetActorNodeCount();
	for ( int nActorNodeIndex = 0;  nActorNodeIndex < count;  nActorNodeIndex++)
	{
		RActorNode *pActorNode = pActor->GetActorNodes()[ nActorNodeIndex];
		int nPropertyCount = pActorNode->m_pMeshNode->GetUserPropertyCount();
		if( nPropertyCount == 0)		continue;

		string strToken;
		const string& strProperty = pActorNode->m_pMeshNode->GetUserPropertyString( 0);
		stringstream sstream(strProperty);
		sstream >> strToken;

		if ( strcmp( strToken.c_str(),SET_SOUND) != 0)		continue;
		{
			sstream >> strToken;
			sstream >> strToken;

			if ( strcmp( strToken.c_str(), "true") !=0)		continue;


			FIELDSOUND_INFO info;
			info.vPos = pActorNode->GetWorldPosition();

			for ( int i = 1;  i < nPropertyCount;  i++)
			{
				const string& strProperty = pActorNode->m_pMeshNode->GetUserPropertyString( i);
				stringstream sstream( strProperty);
				sstream >> strToken;

				if ( strcmp( strToken.c_str(), SET_SOUND_NAME) == 0)
				{
					sstream >> strToken;
					sstream >> strToken;
					info.strName = MLocale::ConvAnsiToUTF16(strToken.c_str());
				}
				else if ( strcmp( strToken.c_str(), SET_SOUND_MIN) == 0)
				{
					sstream >> strToken;
					sstream >> strToken;
					info.fMinDist = atoi( strToken.c_str());
				}
				else if ( strncmp( strProperty.c_str(), SET_SOUND_MAX,strlen( SET_SOUND_MAX)) == 0)
				{
					sstream >> strToken;
					sstream >> strToken;
					info.fMaxDist = atoi( strToken.c_str());
				}
			}

			if (gg.currentgamestate && gg.currentgamestate->GetWorld() )
				gg.currentgamestate->GetWorld()->GetInfo()->m_vecSound.push_back(info);
		}
	}
}

void XModuleNonControl::ReleaseObjSound()
{
	const char* SET_SOUND		= "set_sound";
	const char* SET_SOUND_NAME	= "set_sound_name";
	const char* SET_SOUND_MIN	= "set_sound_minradius";
	const char* SET_SOUND_MAX	= "set_sound_maxradius";

	RActor* pActor = static_cast<RActor*>(m_pModuleEntity->GetActor());
	int count = pActor->GetActorNodeCount();
	for ( int nActorNodeIndex = 0;  nActorNodeIndex < count;  nActorNodeIndex++)
	{
		RActorNode *pActorNode = pActor->GetActorNodes()[ nActorNodeIndex];
		int nPropertyCount = pActorNode->m_pMeshNode->GetUserPropertyCount();
		if( nPropertyCount == 0)		continue;

		string strToken;
		const string& strProperty = pActorNode->m_pMeshNode->GetUserPropertyString( 0);
		stringstream sstream(strProperty);
		sstream >> strToken;

		if ( strcmp( strToken.c_str(),SET_SOUND) != 0)		continue;
		{
			sstream >> strToken;
			sstream >> strToken;

			if ( strcmp( strToken.c_str(), "true") !=0)		continue;


			FIELDSOUND_INFO info;
			info.vPos = pActorNode->GetWorldPosition();

			for ( int i = 1;  i < nPropertyCount;  i++)
			{
				const string& strProperty = pActorNode->m_pMeshNode->GetUserPropertyString( i);
				stringstream sstream( strProperty);
				sstream >> strToken;

				if ( strcmp( strToken.c_str(), SET_SOUND_NAME) == 0)
				{
					sstream >> strToken;
					sstream >> strToken;
					info.strName = MLocale::ConvAnsiToUTF16(strToken.c_str());

					if (gg.currentgamestate && gg.currentgamestate->GetWorld() )
					{
						int i = 0;
						vector< FIELDSOUND_INFO>& vecSound = gg.currentgamestate->GetWorld()->GetInfo()->m_vecSound;
						for (vector< FIELDSOUND_INFO>::iterator itor = vecSound.begin(); itor != vecSound.end(); ++itor, ++i)
						{
							FIELDSOUND_INFO& rinfo = *itor;
							if (rinfo.strName == info.strName && rinfo.vPos == info.vPos)
							{
								char szID[ 256];
								sprintf_s( szID, "fieldsound_%u", i);
								if ( global.sound)	global.sound->Stop( szID);

								vecSound.erase(itor);
								break;
							}
						}
					}
				}
			}
		}
	}
}

void XModuleNonControl::OnUpdate(float fDelta)
{
	PFC_THISFUNC;

	PFI_BLOCK_THISFUNC(308);

	UpdateAlign(fDelta);

	XModuleActorControl::OnUpdate(fDelta);
}

void XModuleNonControl::OnUpdateState( float fDelta )
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

	m_pModuleAction->UpdateNonMovementAction(fDelta);

	XModuleActorControl::OnUpdateState(fDelta);
}

void XModuleNonControl::OnUpdateCommand( float fDelta )
{
}

void XModuleNonControl::OnUpdateSpecial( float fDelta )
{

}

void XModuleNonControl::OnActTalent( int nTalentID, vec3& vDir )
{
	// [2007/8/27 isnara] � ��Ȳ������ ó���ϸ� �ȵȴ�.
	// �׾�����...
	if(GetOwner()->IsDead())
		return;

	DoAction(NPC_ACTION_STATE_TALENT);

	// �̵�/ȸ�� �ӵ��� ����
 	XMovableGoParam* pGoParam = new XMovableGoParam(XMovableGoParam::E_GO_INTERPOLATION);
 	XMovableFactor::SInterpolation param = m_pModuleMovable->GetMovableFactor().INTERPOLATION;
 	pGoParam->Set( param.m_vTargetPos, vDir, param.m_fMoveSpeed, param.m_fRotateSpeed, BOOST_NONE, param.m_nMovementFlags, param.m_bRunMotion);
 	pGoParam->SetGoInterpolationForce( true);
	m_pModuleMovable->TriggerGo( pGoParam);
	delete pGoParam;


	XModuleActorControl::OnActTalent(nTalentID, vDir);
}

void XModuleNonControl::OnUseTalent(int nTalentID, vec3& vPos, vec3& vDir)
{
	#ifdef _DEBUG_LOG_REV_TIME
	SYSTEMTIME stCurTime;
	GetLocalTime(&stCurTime);
	char szTime[256] = "";
	sprintf_s(szTime, "[%02d:%02d:%02d.%03d]:"
		, stCurTime.wHour
		, stCurTime.wMinute
		, stCurTime.wSecond
		, stCurTime.wMilliseconds);
	mlog("[%s] XModuleNonControl::OnUseTalent\n", szTime);
	#endif

	if(gvar.Debug.bShowNpcMoveInfo)
	{
		vec3 bottom = vPos + vec3(0, 0, 10);
		vec3 top	= bottom + vec3(0, 0, 80);
		// ���ӷ��� ��
		REngine::GetSceneManager().GetPassRenderer< RDebugPassRenderer >()->AddDebugCapsule(MCapsule(bottom, top, 10), 5, D3DCOLOR_XRGB(0, 255, 255));
	}

	DoAction(NPC_ACTION_STATE_TALENT);


	// �Ÿ����� �ʹ� ���� �� ��쿣 ������Ų��.
	vec3 pos_diff = vPos.DistanceTo(GetOwner()->GetPosition());

	if ( pos_diff > PROCESS_MOVE_EXCEPTION_DISTANCE)
	{
		GetOwner()->SetPosition( vPos);
	}


	// �̵�/ȸ�� �ӵ��� ����
	XMovableFactor::SInterpolation param = m_pModuleMovable->GetMovableFactor().INTERPOLATION;
	float fMovSpeed		= param.m_fMoveSpeed;
	float fRotSpeed		= GetOwner()->GetInfo(true)->fRotateSpeed;	 // param.m_fRotateSpeed;
	unsigned int nBoost	= BOOST_MOVE;

	XNonPlayer* pNPC = GetOwner();
	XNPCInfo* pNPCInfo = pNPC->GetInfo(true);
	if(pNPCInfo && pNPCInfo->IsRotationTalent(nTalentID))
	{
		// ȸ�� �ŷ�Ʈ�� ��� ȸ�� �ӵ��� �ٽ� �����
		XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(nTalentID);

		float angle = m_pOwner->GetDirection().AngleToXY( vDir);
		fRotSpeed = abs( RadianToDegree( angle) / pTalentInfo->m_ActAnimationTime.GetTime());
	}
	else
		nBoost |= BOOST_ROTATE;

	fMovSpeed = max( fMovSpeed, 50.0f);			// �ּ� �̵� �ӵ��� ����
	fRotSpeed = max( fRotSpeed, 20.0f);			// �ּ� ȸ�� �ӵ��� ����


	XMovableGoParam* pGoParam = new XMovableGoParam(XMovableGoParam::E_GO_INTERPOLATION);
	pGoParam->Set( vPos, vDir, fMovSpeed, fRotSpeed, nBoost, MOVEMENTFLAG_FORWARD | MOVEMENTFLAG_USING_TALENT, true);
	pGoParam->SetGoInterpolationForce( true);
	m_pModuleMovable->TriggerGo( pGoParam);
	delete pGoParam;

	XModuleActorControl::OnUseTalent(nTalentID, vPos, vDir);
}

void XModuleNonControl::OnActSpellProjectile(int nTalentID, vec3& vEntityDir, vec3& vTargetPos, MUID& uidTarget, int nGroupID, int nCapsuleID)
{
	DoAction(NPC_ACTION_STATE_TALENT);

	m_pOwner->SetFacingDir(vEntityDir);

	vEntityDir.z = 0;
	vEntityDir.Normalize();


	m_pOwner->SetDirectionSmoothly(vEntityDir, 0.5f);

	if (m_pModuleTalent)
	{
		m_pModuleTalent->SetProjectileTargetPos(vTargetPos);
	}

	XModuleActorControl::OnActSpellProjectile(nTalentID, vEntityDir, vTargetPos, uidTarget, nGroupID, nCapsuleID);
}

void XModuleNonControl::OnActArchery( int nTalentID, vec3& vDir, MUID& uidTarget )
{
	// [2007/8/27 isnara] � ��Ȳ������ ó���ϸ� �ȵȴ�.
	// �׾�����...
	if(GetOwner()->IsDead())
	{
		return;
	}

	DoAction(NPC_ACTION_STATE_TALENT);

	m_pOwner->SetFacingDir(vDir);
	vDir.z = 0;
	vDir.Normalize();

	m_pOwner->SetDirectionSmoothly(vDir, 0.5f);

	if (m_pModuleTalent)
	{
		if (uidTarget != MUID::Invalid())
		{
			m_pModuleTalent->SetTarget(uidTarget);
		}
	}

	XModuleActorControl::OnActArchery(nTalentID, vDir, uidTarget);
}

void XModuleNonControl::OnActArcheryDetail( int nTalentID, vec3& vDir, MUID& uidTarget, int nGroupID, int nCapsuleID )
{
	// [2007/8/27 isnara] � ��Ȳ������ ó���ϸ� �ȵȴ�.
	// �׾�����...
	if(GetOwner()->IsDead())
	{
		return;
	}

	DoAction(NPC_ACTION_STATE_TALENT);

	m_pOwner->SetFacingDir(vDir);
	vDir.z = 0;
	vDir.Normalize();

	m_pOwner->SetDirection(vDir);

	if (m_pModuleTalent)
	{
		if (uidTarget != MUID::Invalid())
		{
			m_pModuleTalent->SetTarget(uidTarget, nGroupID, nCapsuleID);
		}
	}

	XModuleActorControl::OnActArcheryDetail(nTalentID, vDir, uidTarget, nGroupID, nCapsuleID);
}

void XModuleNonControl::OnCancelTalent()
{
	//mlog("cancel!!\n");

	DoAction(NPC_ACTION_STATE_NA);

	XModuleActorControl::OnCancelTalent();

	// ���� ���� ��Ʈ�Ӽ��̳� �����Ӽ��� ������ �ɷ��ִٸ�... Stop �ִϸ��̼��� ���� �ʴ´�.
	// �׾��ٸ�...
	if(m_pModuleBuff->GetBuffAttribute().IsMesmerizable() ||
		GetOwner()->IsDead())
		return;

	DoNPCStop();
}

void XModuleNonControl::DoNPCStop()
{
	// [2007/8/27 isnara] � ��Ȳ������ ó���ϸ� �ȵȴ�.
	// �׾�����...
	if(GetOwner()->IsDead())
	{
		return;
	}

	DoAction(NPC_ACTION_STATE_NA);

	XModuleAction* pModuleAction = m_pOwner->GetModuleAction();
	if (pModuleAction) pModuleAction->Stop();
}

void XModuleNonControl::OnHit(XTalentHitParam* pHitInfo)
{
	DoAction(NPC_ACTION_STATE_HIT);

	XModuleActorControl::OnHit(pHitInfo);
}

void XModuleNonControl::OnGuard(vec3& pos, vec3& dir)
{
	DoNPCStop();

	XModuleAction* pModuleAction = m_pOwner->GetModuleAction();
	if (pModuleAction) pModuleAction->Guard();

	XMovableGoParam* pGoParam = new XMovableGoParam(XMovableGoParam::E_GO_INTERPOLATION);
	XMovableFactor::SInterpolation param = m_pModuleMovable->GetMovableFactor().INTERPOLATION;
	pGoParam->Set( pos, dir, param.m_fMoveSpeed, GetOwner()->GetInfo(true)->fRotateSpeed, BOOST_NONE, MOVEMENTFLAG_FORWARD | MOVEMENTFLAG_GUARD, true);
	pGoParam->SetGoInterpolationForce(true);
	m_pModuleMovable->TriggerGo( pGoParam);
	delete pGoParam;
}

void XModuleNonControl::OnGuardReleased()
{
	//mlog("net guard released\n");
	// [2007/8/27 isnara] � ��Ȳ������ ó���ϸ� �ȵȴ�.
	// �׾�����...
	if(GetOwner()->IsDead())
	{
		return;
	}

	XModuleAction* pModuleAction = m_pOwner->GetModuleAction();
	if (pModuleAction) pModuleAction->ActionGuardRelease();
}

void XModuleNonControl::OnDie( MF_STATE nMFState /*= MF_NONE*/, bool bSetAni /*= true*/ )
{
	DoAction(NPC_ACTION_STATE_DIE);

	GetOwner()->OnDie();

	m_pModuleTalent->Cancel();

	if(m_bReservedHitByTalent == false)
	{
		m_pModuleAction->Die();
	}

	XNonPlayer* pOwnerNPC = GetOwner();
	if (pOwnerNPC->GetInfo() && pOwnerNPC->GetInfo()->SoundInfo.DyingSound.empty() == false)
	{
		if ( global.sound)
			global.sound->PlayDynamic3D( MLocale::ConvUTF16ToAnsi(pOwnerNPC->GetInfo()->SoundInfo.DyingSound.c_str()).c_str(), NULL, 0, pOwnerNPC->GetUID());
	}
}

void XModuleNonControl::OnDespawn()
{
	XEvent msgDisAppear( XEVTD_DISAPPEAR_ME);
	m_pOwner->PostDelayEvent( msgDisAppear, 0);

	gg.omgr->ReserveDelete(m_pOwner->GetUID(), 5.0f);
}

void XModuleNonControl::HitByTalent(XTalentHitParam * pHitInfo)
{
	CheckReservedHitByTalent();

	stMotionFactorResult rtMotionFactor;

	// ��, �� �Ÿ� Ȯ��
	// ���Ÿ��� �ǰ� �ִϸ��̼��� ����ŷ�Ʈ���� �߻�ü �ǰݽ� ȣ���Ѵ�.
	XObject* pAttackObject = gg.omgr->FindActor_UIID(pHitInfo->nAttackerUIID);
	if (pAttackObject && pAttackObject->GetModuleTalent())
	{
		pAttackObject->GetModuleTalent()->OnHitTarget(pHitInfo->nTalentID);
	}

	// �׾��ٸ�.. �ǰ��� �������� �ʴ´�.
	//if(m_DeathController.IsDeadActionImmediatelyOnHit(GetOwner(), pHitInfo->nMFState))
	//{
	//	OnDie();
	//}
	//else 
	{
		//// �׾��°�? �����Ŷ�� �ٷ� ���� �� ���� MF �ϰ��̴�.
		//if(GetOwner()->IsDead())
		//{
		//	m_pModuleAction->MFDie();
		//}

		XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(pHitInfo->nTalentID, pAttackObject);

		bool bCancel = true;
		if (m_pModuleTalent->IsActive())
		{
			// NPC�� �̵��ϴ� �ŷ�Ʈ�� ����� �� ����.
			// ����(��ȿ)�� ��Ұ� �ȵȴ�.
			if (pHitInfo->IsFlagImmune() == false &&
				(m_pModuleTalent->GetInfo()->m_bCancelable && 
				XTalentHelper::IsTalentCancelableMotionFactor(pHitInfo->nMFState)))
			{
				m_pModuleTalent->Cancel();
			}
		}

		// MF_NONE�϶� �ŷ�Ʈ ������̶��... ������͸� ���Ѵ�. 
		bool bActMf = true;
		if(pHitInfo->nMFState == MF_NONE &&
			m_pModuleTalent->IsActive())
		{
			bActMf = false;
		}

		XModuleAction* pModuleAction = m_pOwner->GetModuleAction();
		if(pModuleAction && bActMf)
		{
			stServerKnockbackVictimInfo knockbackInfo(pHitInfo->vServerKnockbackDir, pHitInfo->vServerKnockbackVictimPos, pHitInfo->vServerKnockbackVictimTargetPos);
			pModuleAction->ActMF(pHitInfo->nMFState, pHitInfo->nMFWeight, pHitInfo->nAttackerUIID, knockbackInfo, pHitInfo->nTalentID, rtMotionFactor);
		}

	/*	if (pHitInfo->IsFlagDead())
		{
			if (pHitInfo->nMFState == MF_KNOCKBACK || 
				pHitInfo->nMFState == MF_THROWUP ||
				pHitInfo->nMFState == MF_DRAG)
			{
				if (pTalentInfo && pTalentInfo->m_FinishEffect.m_bBreakableParts)
				{
					for (size_t i = 0; i < MAX_BREAKABLE_PARTS; i++)
					{
						MUID uid = MUID::ZERO;
						if(pAttackObject)
							uid = pAttackObject->GetUID();
						OnBreakParts(uid, i);
					}
				}
			}
		}*/
	}

	// ���� ó��...
	if(GetOwner()->IsDead())
	{
		OnDie(pHitInfo->nMFState, rtMotionFactor.bUseMFAnimation);
	}

	// �ǰݹ� ������ �����ֱ�
	global.emgr->StartHitEffectSelect(pHitInfo);

	_DamageInfo damageInfo;
	damageInfo.nDamage				= pHitInfo->nDamage;
	damageInfo.nFlags				= pHitInfo->nFlags;
	damageInfo.nMFState				= pHitInfo->nMFState;
	damageInfo.nAttackedTalentID	= pHitInfo->nTalentID;
	damageInfo.nAttackerUIID		= pHitInfo->nAttackerUIID;
	damageInfo.bSecondaryEffect		= pHitInfo->bSecondaryEffect;

	ShowHitAndDamage(&damageInfo, pAttackObject);
	UpdateHitDamage(pHitInfo->nDamage);				// ������ ���
	BirdTest_ShowDelay(pHitInfo);


	bool bThisIsMyPlayerAttack = false;
	if (pHitInfo->nAttackerUIID == XGetMyUIID()) bThisIsMyPlayerAttack = true;

	FakeBeatenAndSetMultiplyColor(pHitInfo, !rtMotionFactor.bUseMFAnimation);

	XNonPlayer* pOwnerNPC = GetOwner(); 

	// �극��Ŀ�� ����
	//if (pOwnerNPC->GetInfo() && pOwnerNPC->GetInfo()->nGrade <= DEFAULT_NPC_GRADE)
	//{
	//	if (pHitInfo->nMFState == MF_KNOCKBACK || 
	//		pHitInfo->nMFState == MF_THROWUP ||
	//		pHitInfo->nMFState == MF_UPPERED)
	//	{
	//		int pn = XMath::RandomNumber(0, 1);

	//		MUID uid = m_pOwner->GetUID();
	//		OnBreakParts(uid, pn);
	//	}
	//}
	
	if (bThisIsMyPlayerAttack)
	{
		// �ǰݽ� ����
		{
			if (pOwnerNPC->GetInfo() &&
				(pHitInfo->nMFState == MF_NONE || pHitInfo->nMFState == MF_FAKE_BEATEN))
			{
				int n = XMath::RandomNumber(0, 4);
				if (n == 0)
				{
					if (pOwnerNPC->GetInfo()->SoundInfo.HitSound1.empty() == false)
					{
						if ( global.sound)
							global.sound->PlayDynamic3D( MLocale::ConvUTF16ToAnsi(pOwnerNPC->GetInfo()->SoundInfo.HitSound1.c_str()).c_str(), NULL, 0, pOwnerNPC->GetUID());
					}
				}
				else if (n == 1)
				{
					if (pOwnerNPC->GetInfo()->SoundInfo.HitSound2.empty() == false)
					{
						if ( global.sound)
							global.sound->PlayDynamic3D( MLocale::ConvUTF16ToAnsi(pOwnerNPC->GetInfo()->SoundInfo.HitSound2.c_str()).c_str(), NULL, 0, pOwnerNPC->GetUID());
					}
				}

			}
		}
	}
}

void XModuleNonControl::OnNPCMove(TD_NPC_MOVE* pNM, bool bRunning)
{
	#ifdef _DEBUG_LOG_REV_TIME
	SYSTEMTIME stCurTime;
	GetLocalTime(&stCurTime);
	char szTime[256] = "";
	sprintf_s(szTime, "[%02d:%02d:%02d.%03d]:"
		, stCurTime.wHour
		, stCurTime.wMinute
		, stCurTime.wSecond
		, stCurTime.wMilliseconds);
	mlog("[%s] XModuleNonControl::OnNPCMove\n", szTime);
	#endif

	vec3 pos = pNM->vTarPos;
	vec3 dir; dir = pNM->svDir;		//posTarget - m_pModuleEntity->GetPosition();

	if(gvar.Debug.bShowNpcMoveInfo)
	{
		vec3 bottom = pos + vec3(0, 0, 10);
		vec3 top	= bottom + vec3(0, 0, 80);
		// ��ȫ��
		REngine::GetSceneManager().GetPassRenderer< RDebugPassRenderer >()->AddDebugCapsule(MCapsule(bottom, top, 10), 5, D3DCOLOR_XRGB(255, 0, 255));
		bottom = pos + vec3(0, 0, 10);
		top	= bottom + vec3(0, 0, 80);
		// �ʷϻ�
		//	REngine::GetSceneManager().GetPassRenderer< RDebugPassRenderer >()->AddDebugCapsule(MCapsule(bottom, top, 10), 5, D3DCOLOR_XRGB(0, 255, 0));
	}

	// Action State ��ȯ
	XNPCActionMoveParam npcActionMoveParam;
	npcActionMoveParam.vTarPos = pNM->vTarPos;
	npcActionMoveParam.fSpeed = (float)pNM->nSpeed;

	DoAction(NPC_ACTION_STATE_MOVE, &npcActionMoveParam);


	// �ŷ�Ʈ ������̸�... �ŷ�Ʈ ���
	if(m_pModuleTalent->IsActive())
	{
		m_pModuleTalent->Cancel();
		m_pModuleAction->Stop();
	}

	XMovableGoParam* pGoParam = new XMovableGoParam(XMovableGoParam::E_GO_INTERPOLATION);
	pGoParam->Set( pos, dir, (float)pNM->nSpeed, GetOwner()->GetInfo(true)->fRotateSpeed, BOOST_NONE, MOVEMENTFLAG_FORWARD, bRunning);
	pGoParam->SetGoInterpolationForce(true);

	// �������� 7���� �̻��̸� ������...
	float fHeightGap = pos.z - m_pOwner->GetPosition().z;
	float fMoveLen = (pos - m_pOwner->GetPosition()).Length();			
	if(fHeightGap >= MOVE_DIRECT_CHECK_HEIGHT && fMoveLen <= MOVE_LEVITATION_LEN)	// 10 ���� �̳����� ���ߺξ��Ѵ�.
		pGoParam->SetMoveCheckType(MCT2_DIRECT);
	else if(fHeightGap > MOVE_TARGET_HEIGHT_CHECK_HEIGHT)
		pGoParam->SetMoveCheckType(MCT2_TARGET_HEGITH);

	m_pModuleMovable->TriggerGo( pGoParam);

	delete pGoParam;
}

void XModuleNonControl::OnNPCStop(vec3 vPos)
{
	#ifdef _DEBUG_LOG_REV_TIME
	SYSTEMTIME stCurTime;
	GetLocalTime(&stCurTime);
	char szTime[256] = "";
	sprintf_s(szTime, "[%02d:%02d:%02d.%03d]:"
		, stCurTime.wHour
		, stCurTime.wMinute
		, stCurTime.wSecond
		, stCurTime.wMilliseconds);
	mlog("[%s] XModuleNonControl::OnNPCStop\n", szTime);
	#endif
	
	if(gvar.Debug.bShowNpcMoveInfo)
	{
		vec3 bottom = vPos + vec3(0, 0, 10);
		vec3 top	= bottom + vec3(0, 0, 80);
		// �Ķ���
		REngine::GetSceneManager().GetPassRenderer< RDebugPassRenderer >()->AddDebugCapsule(MCapsule(bottom, top, 10), 5, D3DCOLOR_XRGB(0, 0, 255));
	}

	DoAction(NPC_ACTION_STATE_NA);

	// �ŷ�Ʈ ������̸�... �ŷ�Ʈ ���
	if(m_pModuleTalent->IsActive())
	{
		m_pModuleTalent->Cancel();
		m_pModuleAction->Stop();
	}


	XMovableGoParam* pGoParam = new XMovableGoParam(XMovableGoParam::E_GO_INTERPOLATION);
	XMovableFactor::SInterpolation param = m_pModuleMovable->GetMovableFactor().INTERPOLATION;
	pGoParam->Set( vPos, param.m_vTargetDir, param.m_fMoveSpeed, GetOwner()->GetInfo(true)->fRotateSpeed, BOOST_NONE, MOVEMENTFLAG_FORWARD, param.m_bRunMotion);
	pGoParam->SetGoInterpolationForce( true);
	m_pModuleMovable->TriggerGo( pGoParam);
	delete pGoParam;
}

void XModuleNonControl::OnNPCFaceTo( vec3 vPos, vec3 vDir )
{
	XNonPlayer* pNPC = GetOwner();

	StopMove(false);

	DoAction(NPC_ACTION_STATE_NA);

	pNPC->SetDirectionSmoothly(vDir, 0.5f);
	pNPC->SetFacingDir(vDir);
}

void XModuleNonControl::OnRebirth()
{
	XActor * pActor = static_cast<XActor*>(m_pOwner);
	pActor->OnRebirth();

	m_pModuleAction->ActionRebirth();
}

void XModuleNonControl::OnSetLootable( bool bLootable )
{
	XNonPlayer * pNPC = GetOwner();
	pNPC->SetLootable(bLootable);
}

void XModuleNonControl::OnSetDeSpawn()
{
	XNonPlayer * pNPC = GetOwner();
	pNPC->SetDeSpawn(true);
}

void XModuleNonControl::DoActionIdle()
{
	DoAction(NPC_ACTION_STATE_NA);

	m_pModuleAction->Idle();
}

void XModuleNonControl::OnSpew()
{
	// ��� �ִϸ��̼����� ����
	// ��� �ִϸ��̼��� ExtarAni2�� �ִ�.
	m_pModuleTalent->ExtraAct2();
}

void XModuleNonControl::OnSpewInterrupt()
{
	// �߰��� ��� �ִϸ��̼����� ����
	// �߰��� ��� �ִϸ��̼��� ExtarAni3�� �ִ�.
	m_pModuleTalent->ExtraAct3();
}

void XModuleNonControl::OnSwallowedSynch( int nTalentID, MUID& uidTarget )
{
	m_pModuleTalent->Start(nTalentID);
	m_pModuleTalent->ExtraAct();

	XNetPlayer* pNetPlayer = gg.omgr->FindNetPlayer(uidTarget);
	if(pNetPlayer)
	{
		XModuleActorControl* pModuleNetPlayer = pNetPlayer->GetModuleActorControl();
		if(pModuleNetPlayer)
		{
			pModuleNetPlayer->OnSwallowedSynch(nTalentID, (MUID)m_pOwner->GetUID());
		}
	}
}


void XModuleNonControl::UpdateAlign( float fDelta )
{
	m_pAligner->Update(GetOwner(), fDelta);
}

void XModuleNonControl::InitStates()
{
	m_pActionFSM->AddState(new XNPCActionNA(this));
	m_pActionFSM->AddState(new XNPCActionMove(this));
	m_pActionFSM->AddState(new XNPCActionTalent(this));
	m_pActionFSM->AddState(new XNPCActionHit(this));
	m_pActionFSM->AddState(new XNPCActionDie(this));

	m_pActionFSM->ChangeState(NPC_ACTION_STATE_NA);
}

void XModuleNonControl::DoAction( NPC_ACTION_STATE nState, void* pParam/*=NULL*/ )
{
	if (m_pActionFSM->GetCurrentStateID() != nState)
	{
		m_pActionFSM->ChangeState(nState, pParam);
	}
}

const wchar_t* XModuleNonControl::GetActionStateName()
{
	return ((m_pActionFSM->GetCurrentState()) ? m_pActionFSM->GetCurrentState()->GetDebugName() : L"");
}

void XModuleNonControl::OnReInNonPlayer( TD_UPDATE_CACHE_NPC* pMonsterInfo )
{
	vec3 vDir;
	vDir = pMonsterInfo->svDir;

	OnReInField(pMonsterInfo->vPos, vDir);
}

void XModuleNonControl::OnDieAniEnd()
{
	XModuleActorControl::OnDieAniEnd();

	// �̺�Ʈ ����
	XEvent msg(XEVTL_MOTION_PLAYONCE_NPC_DIE);
	m_pOwner->Event(msg);
}

void XModuleNonControl::OnBallonText( wstring szID, wstring szChat, float fMaintainTime )
{
	XModuleActorControl::OnBallonText(szID, szChat, fMaintainTime);

	XNonPlayer* pOwnerNPC = GetOwner(); 
	float fLifeTime = pOwnerNPC->GetBallonLifeTime(fMaintainTime, szChat);

	XEvent msg(XEVTD_NPC_SAY_END);
	m_pOwner->PostDelayEvent(msg, fLifeTime * 1000.f);
}

void XModuleNonControl::CheckMoveState()
{
	// �̵� ���� �˻�
	XMovableFactor::SInterpolation param = m_pModuleMovable->GetMovableFactor().INTERPOLATION;
	XMovableFactor::SMovingDummy moving = m_pModuleMovable->GetMovableFactor().MOVING_DUMMY;

	// ������ ���� �̵��� �����̵��� �ƴϹǷ� ����
	if(moving.m_bHaveDummy)
		return;

	// ���� ������ �ߴ�. 
	// �׷���... ��ǥġ���� �������� �ִٸ�...
	if(XMath::Equals(param.m_vTargetPos.z, m_pOwner->GetPosition().z, 1.0f) == false)
	{
		// �����ض�.
		m_pOwner->SetPosition(param.m_vTargetPos);
	}
}

void XModuleNonControl::DoActionMailOpen()
{
	XModuleMotion* pModuleMotion = m_pOwner->GetModuleMotion();
	if (pModuleMotion)
	{
		pModuleMotion->ChangeMotion(L"mail_open", MT_DEFAULT, true);
	}
}

void XModuleNonControl::DoActionMailClose()
{
	XModuleMotion* pModuleMotion = m_pOwner->GetModuleMotion();
	if (pModuleMotion)
	{
		pModuleMotion->ChangeMotion(L"mail_close", MT_DEFAULT, true);
	}
}

void XModuleNonControl::OnLootable( bool bLootable )
{
	// ���� ����Ʈ�� �ؾ� �ϴ���... üũ
	if(bLootable)
	{
		XModuleAction* pModuleAction = m_pOwner->GetModuleAction();
		if(pModuleAction)
		{
			if(pModuleAction->IsDieAniEnd())
			{
				// �̺�Ʈ ����
				XEvent msg(XEVTL_MOTION_PLAYONCE_NPC_DIE);
				m_pOwner->Event(msg);
			}
		}
	}
}

void XModuleNonControl::HitByBuff( XTalentHitParam * pHitInfo )
{
	if(pHitInfo)
	{
		HitByTalent(pHitInfo);
	}
}
