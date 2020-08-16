#include "stdafx.h"
#include "XGrappledController.h"
#include "XModuleAction.h"
#include "XModuleEntity.h"
#include "XModuleMovable.h"
#include "XModuleTalent.h"
#include "XEffectInvoker.h"
#include "XTalentHitParam.h"
#include "MPhysX4R3/MPxRagDollTC.h"
#include "XGameState.h"
#include "XTalentInfoMgr.h"
#include "XGameTransformControllerManager.h"

#ifdef _DEBUG
//#include "RDebugPassRenderer.h"
//#define GRAPLLED_DEBUG
#endif

XGrappledController::XGrappledController()
{
	m_pPlayer = NULL;
	m_pHitInfo = NULL;
	m_bAttach = false;
	m_bBeThrown = false;
	m_bCollision = false;
	m_bFinished = true;
	m_bStart = false;

	m_fRemainStartTime = 0.0f;

	m_strGrappledPlayerAnimation = MOTION_NAME_MF_GRAPPLED;
}


XGrappledController::~XGrappledController()
{
	m_pPlayer = NULL;
	SAFE_DELETE( m_pHitInfo);
}


void XGrappledController::SetOwnerPlayer( XPlayer* pPlayer)
{
	if ( pPlayer == NULL)
	{
		assert( 0);
		return;
	}


	m_pPlayer = pPlayer;
}


bool XGrappledController::OnStart( _DamageRepeatInfo* pHitInfo)
{
	// ���� ����
	m_bStart = true;
	m_bBeThrown = false;
	m_bCollision = false;
	m_bFinished = false;

	m_fRemainStartTime = 0.0f;

	if ( m_pPlayer == NULL)
	{
		assert( 0);
		return false;
	}

	// ������ ����
	if ( m_pHitInfo)
		SAFE_DELETE( m_pHitInfo);

	m_pHitInfo = pHitInfo;
	if ( m_pHitInfo == NULL)
	{
		assert( 0);
		return false;
	}

	XObject* pAttacker = gg.omgr->FindActor_UIID( m_pHitInfo->nAttackerUIID);
	if ( pAttacker == NULL)
	{
		assert( 0);
		return false;
	}

	pAttacker->SetExtraTarget( m_pPlayer->GetUID());

	// ���� ���� ��������(��ġ, �ð�)
	vec3 vHitPos = m_pPlayer->GetPosition();
	m_fRemainStartTime = GetGrappledHitTime(pAttacker);

	// �ǰ����� ����
	XModuleMovable* pMovable = m_pPlayer->GetModuleMovable();
	if ( pMovable)
	{
		MUID uidAttacker = pAttacker->GetUID();

		pMovable->ForceStop();
		pMovable->TriggerGrappledStart( uidAttacker, (float)m_pHitInfo->nMFWeight, vHitPos, m_fRemainStartTime);
	}

	// �ǰ����� �ִϸ��̼� ����
	XTalentInfo* pTalentInfo = info.talent->Get(pHitInfo->nTalentID);
	if(pTalentInfo)
	{
		if(pTalentInfo->m_strGrappledAni.empty() == false)
			m_strGrappledPlayerAnimation = pTalentInfo->m_strGrappledAni;
		if(pTalentInfo->m_strGrappledBone.empty() == false)
			m_strGrappledPlayerBoneName = pTalentInfo->m_strGrappledBone;
	}

	return true;
}


void XGrappledController::OnFinish()
{
	SAFE_DELETE( m_pHitInfo);

	m_bFinished = true;


	if ( m_pPlayer == NULL)
		return;


	XModuleEntity* pEntity = m_pPlayer->GetModuleEntity();
	if ( pEntity)
		pEntity->SetRenderPosDir( vec3::ZERO, vec3::ZERO);


	XModuleMovable* pMovable = m_pPlayer->GetModuleMovable();
	if ( pMovable)
		pMovable->TriggerGrappledEnd();
}

bool XGrappledController::IsValidAttacker(XPlayer* pAttacker)
{
	if(pAttacker && pAttacker->GetActor()->IsLoadingCompleted() == false)
	{
		//mlog("invalid attacker\n");
		return false;
	}
	return true;
}

void XGrappledController::OnUpdate( float fDelta)
{
	if ( m_pPlayer == NULL  ||  m_bFinished == true)
		return;

	m_fRemainStartTime -= fDelta;
	if(m_fRemainStartTime > 0.0f)
	{
		m_bStart = true;
		return;
	}
	
	if(m_bStart)
	{
		SetGrappledAnimation();
		m_bStart = false;
	}

	// ������ ǥ�� ������Ʈ
	_UpdateDamage(fDelta);


	// ������Ʈ
	XModuleMovable* pMovable = m_pPlayer->GetModuleMovable();
	XModuleEntity* pEntity = m_pPlayer->GetModuleEntity();
	XModuleMotion* pMotion = m_pPlayer->GetModuleMotion();

	const XMovableFactor::SState& GENERAL_STATE = pMovable->GetMovableFactor().GENERAL_STATE;
	const XMovableFactor::SGrapple& GRAPPLE = pMovable->GetMovableFactor().GRAPPLE;

	XObject* pObject = gg.omgr->Find( GRAPPLE.m_uidGrappledAttacker);
	XPlayer* pAttacker = NULL;
	if ( pObject != NULL)
		pAttacker = static_cast<XPlayer*>( pObject);
	
	if ( GRAPPLE.m_bGrappled == false || IsValidAttacker(pAttacker) == false )
	{
		OnFinish();
		return;
	}

	m_bAttach = GRAPPLE.m_bGrappledAttach;

	// �������� ���� �������� ���� �˻�
	if ( m_bBeThrown == false)
	{
		// ���� �ȴ����µ� �����ڰ� �������ų� �׾����� �˻�
		if ( pObject == NULL  ||  pAttacker->IsDead())
		{
			OnFinish();
			return;
		}

		// �����ڰ� ���� ������ Ż��Ʈ(extra talent)�� ������� �ʰ� ���� �� ������ �����⸦ �����Ѵ�
		XModuleTalent* pModuleTalent = pObject->GetModuleTalent();
		if ( pModuleTalent->IsActive() == false  ||  pModuleTalent->GetTalent()->GetInfo()->m_nSkillType != ST_EXTRA_ACTIVE)
		{
			OnBeThrown();
			pMovable->TriggerGrappledBeThrown();
		}
	}



	// �����ڰ� ���������� ���� - ���� ���Ұ�� �̺�Ʈ�� ���� ȣ��Ǵ� ��찡 �߻��ϹǷ� ���⼭ ó����
	// �����ڰ� �׾ �̺�Ʈ�� �߻��ϹǷ� ����.
	if ( m_bBeThrown == true  &&  GRAPPLE.m_bGrappledBeThrown == false)
	{
		pMovable->TriggerGrappledBeThrown();
	}


	// �������̸� �浹������ ������
	if ( GENERAL_STATE.m_bSwim == true)
	{
		OnFinish();
		return;
	}


	// ���� �浹
	if ( GRAPPLE.m_bGrappledBeThrown == true  &&  m_bCollision == false  &&  GENERAL_STATE.m_bColTerrain == true)
	{
		m_bCollision = true;

		// ����Ʈ
		_ShowEffect( GRAPPLED_COLLISION_BASE_EFFECT_NAME, GRAPPLED_COLLISION_BASE_EFFECT_BONENAME);


		// ���� ������ ǥ��
		int nDamage = m_pHitInfo->nDamage;
		for ( vector<_DamageRepeatEvent>::iterator itr = m_pHitInfo->vecDamageRepeatEvents.begin();  itr != m_pHitInfo->vecDamageRepeatEvents.end();  itr++)
			nDamage -= (*itr).nDamage;

		_ShowDamage( nDamage);


		// ���� �غ� ���
		pEntity->SetRenderPosDir( vec3::ZERO, vec3::ZERO);
		pMotion->ChangeMotion( GRAPPLED_COLLISION_PLAYER_ANIMATION, MT_DEFAULT, true);
		pMotion->PushMotion( GRAPPLED_DOWN_PLAYER_ANIMATION);
	}

	
	// ���� üũ
	if ( GRAPPLE.m_vGrappledVelocity == vec3::ZERO)
	{
		// ���� �浹 ��� ���̸� �о�
		if ( pMotion->GetCurrMotion() == GRAPPLED_COLLISION_PLAYER_ANIMATION)
		{
			// �׾����� ���¸� �ٷ� �ٲ��ش�
			if ( m_pPlayer->IsDead() == true)
			{
				pMotion->HoldSequence( true);
				OnFinish();

				// ���� �˻�
				vec3 vCurrDir = m_pPlayer->GetDirection();
				if(vCurrDir.x == 0.0f &&
					vCurrDir.y == 0.0f)
					m_pPlayer->SetDirection(GRAPPLE.m_vGrappledEndDir);

				// ���� �븻�� �׾�����.
				Die();

				// by pok. '���� �浹 - ���� �غ� ���'���� ������ �ڵ�. �׾�� ��������� ���Ѵ�.
				// Ticket #1929 ���ù���
				pEntity->SetRenderPosDir( vec3::ZERO, vec3::ZERO);
				pMotion->ChangeMotion( GRAPPLED_COLLISION_PLAYER_ANIMATION , MT_DEFAULT, true);
				pMotion->PushMotion( GRAPPLED_DOWN_PLAYER_ANIMATION);
			}
		}

		// ����� ������ ������ ������ �����
		else if ( pMotion->GetCurrMotion() == GRAPPLED_DOWN_PLAYER_ANIMATION)
		{
			pMotion->ChangeMotion( GRAPPLED_UP_PLAYER_ANIMATION, MT_DEFAULT, true);
			pMotion->PushMotion( MOTION_NAME_IDLE);

			return;
		}

		// �Ͼ���� �����Ѵ�
		else if ( pMotion->GetCurrMotion() == MOTION_NAME_IDLE)
		{
			OnFinish();
			return;
		}
	}
}


void XGrappledController::OnBeThrown()
{
	m_bBeThrown = true;
	m_bCollision = false;
}


bool XGrappledController::IsFinish()
{
	if ( m_pPlayer == NULL)
		return true;

	return m_bFinished;
}


vec3 XGrappledController::_GetSpineDummyPos(wstring strBoneName)
{
	vec3 v( 0.0f, 0.0f, 0.0f);
	if ( m_pPlayer == NULL)
		return v;

	XCharacter* pCharacter = m_pPlayer->GetActor();
	if ( pCharacter == NULL)
		return v;

	if (pCharacter->GetActorNodePos(strBoneName.c_str(), v, E_TS_LOCAL_RESULT))
		v.z = -v.z;

	return v;
}

void XGrappledController::_UpdateDamage( float fDelta)
{
	if ( m_pPlayer == NULL  ||  m_pHitInfo == NULL)
		return;


	for ( vector<_DamageRepeatEvent>::iterator itr = m_pHitInfo->vecDamageRepeatEvents.begin();  itr != m_pHitInfo->vecDamageRepeatEvents.end();  itr++)
	{
		if ( (*itr).fTime > 1000.0f)
			continue;


		(*itr).fTime -= fDelta;


		if ( (*itr).fTime <= 0.0f)
		{
			_ShowDamage( (*itr).nDamage);
			_ShowEffect( (*itr).strEffectName, (*itr).strBone);

			(*itr).fTime = 9999.0f;
		}
	}
}


void XGrappledController::_ShowDamage( int nDamage)
{
	if ( m_pPlayer == NULL  ||  m_pHitInfo == NULL)
		return;

	if ( nDamage <= 0)
		return;


	XModuleActorControl * pModuleActorControl = m_pPlayer->GetModuleActorControl();
	if ( pModuleActorControl)
	{
		_DamageInfo damageInfo;
		damageInfo.nAttackedTalentID	= m_pHitInfo->nTalentID;
		damageInfo.nDamage				= nDamage;
		damageInfo.nFlags				= m_pHitInfo->nFlags;
		damageInfo.nMFState				= m_pHitInfo->nMFState;
		damageInfo.nAttackerUIID		= m_pHitInfo->nAttackerUIID;

		pModuleActorControl->ShowHitAndDamage( &damageInfo, NULL, vec3( 0, 0, -120.0f));
	}
}


void XGrappledController::_ShowEffect( wstring strEffectName, wstring strBoneName)
{
	if ( m_pPlayer == NULL  ||  m_pHitInfo == NULL)
		return;

	if ( strEffectName.empty())
		return;


	// ����Ʈ�� �Ⱥ��� ���̴�. �װ�... ���������̴�.
	if(m_pPlayer->GetModuleEntity())
	{
		if(m_pPlayer->GetModuleEntity()->GetTransformController<physx::MPxRagDollTC>()->IsEnabled())
		{
			XEffectSinglyInvoker effectInvoker;
			effectInvoker.Invoke(strEffectName, m_pPlayer->GetPosition(), (vec3)vec3::AXISY, (vec3)vec3::AXISZ);
		}
		else
		{
			XEffectInvoker effectInvoker;
			effectInvoker.Invoke( m_pPlayer, strEffectName, wstring(), strBoneName);
		}
	}
}

void XGrappledController::Die()
{
	XModuleAction* pModuleAction = m_pPlayer->GetModuleAction();
	if(pModuleAction)
	{
		pModuleAction->DoneDIe();
	}
}

void XGrappledController::OnPostUpdate( float fDelta )
{
	if(m_bStart)
		return;

	XModuleEntity* pEntity = m_pPlayer->GetModuleEntity();
	XModuleMovable * pMovable = m_pPlayer->GetModuleMovable();

	// Rag doll�� spine ��ġ�� �����
	if ( (m_bBeThrown == false && m_bAttach == true ) ||  (m_bBeThrown == true  &&  m_bCollision == false))
	{
		wstring strBoneName = GRAPPLED_PLAYER_BONENAME;
		if(m_strGrappledPlayerBoneName.empty() == false)
			pEntity->SetRenderPosDir( _GetSpineDummyPos(m_strGrappledPlayerBoneName), vec3::ZERO );
		else
			pEntity->SetRenderPosDir( _GetSpineDummyPos(strBoneName), vec3::ZERO );
	}
}

float XGrappledController::GetGrappledHitTime( XObject* pAttacker )
{
	if ( pAttacker == NULL ||
		pAttacker->GetActor() == NULL)
		return 0.0f;

	XExtraActiveTalent_Grapple* pGrappleTalent = static_cast<XExtraActiveTalent_Grapple*>(pAttacker->GetModuleTalent()->GetTalent());
	if(pGrappleTalent == NULL)
		return 0.0f;

	for ( vector<_DamageRepeatEvent>::iterator itr = m_pHitInfo->vecDamageRepeatEvents.begin();  itr != m_pHitInfo->vecDamageRepeatEvents.end();  itr++)
		itr->fTime -= pGrappleTalent->m_fGrappledRemainTime;

	// Throw ��� �������ΰ�?
	if(pAttacker->GetModuleEntity()->GetAnimationController()->GetPreAnimationName().empty() == false)
	{
		int nPreAniFrame = pAttacker->GetModuleEntity()->GetAnimationController()->GetPreAniFrame();

		// ���� Throw ����� �ƴ��ε�... ��Ʈ �ð��� �����ٸ�... �پ��...
		if(nPreAniFrame >= pGrappleTalent->m_nGrappledHitFrame)
		{

			#ifdef GRAPLLED_DEBUG
				vec3 vCurPoint;
				pAttacker->GetActor()->GetActorNodePos(GRAPPLED_NPC_GRIP_BONE_NAME, vCurPoint, E_TS_WORLD, true);
				RMatrix mat;
				mat.MakeIdentity();
				mat.SetTranslation( vCurPoint);
				REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBox( mat, 5, 20, D3DCOLOR_XRGB(255, 0, 0));
			#endif

			return 0.0f;
		}


#ifdef GRAPLLED_DEBUG
		vec3 vCurPoint;
		pAttacker->GetActor()->GetActorNodePos(GRAPPLED_NPC_GRIP_BONE_NAME, vCurPoint, E_TS_WORLD, true);
		RMatrix mat;
		mat.MakeIdentity();
		mat.SetTranslation( vCurPoint);
		REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBox( mat, 5, 20, D3DCOLOR_XRGB(0, 255, 0));
#endif

		int nRemainFrame = pGrappleTalent->m_nGrappledHitFrame - nPreAniFrame;
		return pAttacker->GetModuleEntity()->GetAnimationController()->GetPreAniFrameToSec(nRemainFrame);
	}

	return 0.0f;
}

void XGrappledController::SetGrappledAnimation()
{
	// �ǰ����� ��� ����
	XModuleMotion* pMotion = m_pPlayer->GetModuleMotion();
	if ( pMotion)
	{
		if(info.motion->GetMotion(m_strGrappledPlayerAnimation.c_str()) == NULL)
		{
			// �ִϸ��̼��̴�.
			pMotion->ChangeToCustomMotion(MOTION_NAME_CUSTOM, m_strGrappledPlayerAnimation.c_str(), true);
		}
		else
		{
			pMotion->ChangeMotion( m_strGrappledPlayerAnimation.c_str(), MT_DEFAULT, true);
		}
	}
}