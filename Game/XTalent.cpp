#include "stdafx.h"
#include "XModuleTalent.h"
#include "XTarget.h"
#include "XBaseApplication.h"
#include "XAnim.h"
#include "XEventID.h"
#include "XGame.h"
#include "XPlayer.h"
#include "XCutScenePlayer.h"
#include "XModuleEntity.h"
#include "XModuleMotion.h"
#include "XModuleAction.h"
#include "XModuleEffect.h"
#include "XModuleBuff.h"
#include "XTalentEvent.h"
#include "XProjectile.h"
#include "XStrings.h"
#include "XSystem.h"
#include "XCameraManager.h"
#include "XTalentEffectInfo.h"
#include "XTalentHelper.h"
#include "XMagicArea.h"
#include "XEffectInvoker.h"

XBaseTalent::XBaseTalent(XObject* pOwner) : 
m_pOwner(pOwner),
m_bReqActSpell(false),
m_nPhase(TALENT_PHASE_NONE),
m_bActive(false),
m_bAllowedActingFromServer(false),
m_pTalentInfo(NULL),
m_vReservedDirection(vec3::ZERO),
m_vProjectileTargetPos(vec3::ZERO),
m_vMagicAreaPos(vec3::ZERO),
m_fElapsedTime(0.0f), 
m_vStartPos(vec3::ZERO),
m_vActPos(vec3::ZERO),
m_fPostDelayRemainTime(0.0f), 
m_matServerStart(MMatrix::IDENTITY)
{
	m_pTalentEvent	= new XTalentEvent(this, pOwner);
}

XBaseTalent::~XBaseTalent(void)
{
	FinishTalent();

	SAFE_DELETE(m_pTalentEvent);
}

void XBaseTalent::Start( XTalentInfo* pTalentInfo )
{
	if (IsActive())
	{
		FinishTalent();
	}

	m_TalentTarget.Init();
	m_bActive = true;
	m_bAllowedActingFromServer = false;
	m_fElapsedTime = 0.0f;
	m_fActElapsedTime = 0.0f;
	m_fPostDelayRemainTime = 0.0f;
	m_vProjectileTargetPos = vec3::ZERO;

	m_vStartPos = m_pOwner->GetPosition();
	m_vActPos = vec3::ZERO;

	SetInfo(pTalentInfo);

	PreLoaderTalentEffect(pTalentInfo);
	//mlog("�ŷ�Ʈ ���� (%s)\n", pTalentInfo->m_szName.c_str());

	// �̺�Ʈ ����
	m_pTalentEvent->EventsStart( &(pTalentInfo->m_Events) );

	OnStart();

	if(IsChangePrepare(pTalentInfo))
	{
		ChangePhase(TALENT_PHASE_PREPARE);
	}
	else
	{
		ChangePhase(TALENT_PHASE_ACT);
	}
}

void XBaseTalent::OnUpdate(float fDelta)
{
	m_fElapsedTime += fDelta;

	CheckRequestSpell();
	UpdateRotationAnimation();

	if (m_bActive == false) return;

	if (m_nPhase >= TALENT_PHASE_ACT)
	{
		m_fActElapsedTime += fDelta;
	}

	if(m_pTalentEvent)
		m_pTalentEvent->EventUpdate(fDelta);

	switch (m_nPhase)
	{
	case TALENT_PHASE_PREPARE:
		{
			if (m_bAllowedActingFromServer)
			{
				ChangePhase(TALENT_PHASE_ACT);
			}
		}
		break;
	case TALENT_PHASE_POST_DELAY:
		{
			CheckPostDelayTalent(fDelta);
		}
		break;
	case TALENT_PHASE_FINISH:
	case TALENT_PHASE_CANCELED:
		{
			FinishTalent();

			XEvent evt;
			evt.m_nID = XEVTL_TALENT_CANCELED;
			m_pOwner->Event(evt);
		}
		break;
	}
}

bool XBaseTalent::Cancel()
{
	if (m_bActive)
	{
		FinishTalent();
		ChangePhase(TALENT_PHASE_CANCELED);
	}

	XEvent msg2;
	msg2.m_nID = XEVTL_TALENT_CANCELED;
	m_pOwner->Event(msg2);

	return false;
}


void XBaseTalent::CheckRequestSpell()
{
	if (m_bReqActSpell == false) return;
	if ((m_strMotionName != L"PS_101000") &&
		(m_strMotionName != MOTION_NAME_TALENT_CASTING_LOOP)) return;

	m_bReqActSpell = false;
}


void XBaseTalent::RequestActSpell()
{
	m_bReqActSpell = true;
	CheckRequestSpell();
}

void XBaseTalent::FinishTalent()
{ 
	m_pTalentEvent->EventsEnd();

	m_bActive = false;
	
	//if(m_pTalentInfo)
	//{
	//	mlog("�ŷ�Ʈ ���� (%s)\n", m_pTalentInfo->m_szName.c_str());
	//}

	ChangePhase(TALENT_PHASE_FINISH);
	ClearCastingEffect();
}


void XBaseTalent::ForceTerminate()
{
	FinishTalent();
}

void XBaseTalent::SetTarget( MUID uidTarget, int nGroupID, int nDummyID, vec3 vTargetPos )
{
	m_TalentTarget.uidTarget			= uidTarget;
	m_TalentTarget.nGroupID				= nGroupID;
	m_TalentTarget.nCapsuleID			= nDummyID;
	m_TalentTarget.vTargetPos			= vTargetPos;
}
//bool XBaseTalent::ContainThisTarget(MUID uid)
//{
//	bool ret = false;
//	if(m_pTalentTarget->CheckOverlapMUID(uid))
//	{
//		ret = true;
//	}
//
//	return ret;
//}

void XBaseTalent::ChangePhase(TALENT_PHASE nPhase)
{
	if (m_nPhase == nPhase) return;

	m_nPhase = nPhase;
	OnEnterPhase(nPhase);
}

void XBaseTalent::OnEnterPhase(TALENT_PHASE nPhase)
{
	switch (nPhase)
	{
	case TALENT_PHASE_PREPARE:
		{
			OnEnterPhasePrepare();
		}break;
	case TALENT_PHASE_ACT:	
		{
			OnEnterPhaseAct();
		}break;
	case TALENT_PHASE_POST_DELAY:
		{
			OnEnterPhasePostDelay();
		}break;
	case TALENT_PHASE_FINISH:
		{
			OnEnterPhaseFinish();
			m_pTalentEvent->EventsEnd();
		}break;
	case TALENT_PHASE_CANCELED:
		{
			OnEnterPhaseCanceled();
			m_pTalentEvent->EventsEnd();
		}break;
	case TALENT_PHASE_EXTRA:
		{
			OnEnterPhaseExtraAct();
		}break;
	case TALENT_PHASE_EXTRA2:
		{
			OnEnterPhaseExtraAct2();
		}
		break;
	case TALENT_PHASE_EXTRA3:
		{
			OnEnterPhaseExtraAct3();
		}
	};
}

void XBaseTalent::OnEnterPhasePrepare()
{
	//// �ִϸ��̼� ����
	//ChangeTalentMotion(TALENT_PHASE_PREPARE, m_pTalentInfo->m_szCastingAnimation.c_str(), m_pTalentInfo->m_szCastingLoopAnimation.c_str());
	//// ����Ʈ ����
	//ChangeTalentEffect(TALENT_PHASE_PREPARE);

}

void XBaseTalent::OnEnterPhaseAct()
{
	//// �ִϸ��̼� ����
	//ChangeTalentMotion(TALENT_PHASE_ACT, m_pTalentInfo->m_szUseAnimation.c_str());
	//// ����Ʈ ����
	//ChangeTalentEffect(TALENT_PHASE_ACT);

	//// ���� ���Ÿ� ���� Ÿ���̸� �߻�ü �߻�
	//// ���� ��� �ִϸ��̼��� �־�� �Ѵ�.
	//if(((m_pTalentInfo->m_nSkillType == ST_ARCHERY) || (m_pTalentInfo->m_nSkillType == ST_MAGIC)) &&
	//	(!m_pTalentInfo->m_szUseAnimation.empty()))
	//{
	//	// XModuleEntity���� �̺�Ʈ �� �߻� ���

	//	//mlog0("TALENT ACT %d\n", m_pTalentInfo->m_nID);

	//	//mlog0("�߻��غ� %d(%s)\n", m_pTalentInfo->m_nID, m_pTalentInfo->m_szUseAnimation.c_str());
	//}

}

void XBaseTalent::OnEnterPhasePostDelay()
{
	if(m_pTalentInfo && m_pTalentInfo->m_fPostDelay)
	{
		m_fPostDelayRemainTime = m_pTalentInfo->m_fPostDelay;
	}
	else
	{
		FinishTalent();
	}
}

void XBaseTalent::ChangeTalentMotion( TALENT_PHASE nPhase, const wchar_t* szName, const wchar_t* szLoopName /*= ""*/, MOVING_USE_TYPE nType /*= MUT_NONE*/ )
{
	XModuleMotion* pModuleMotion = m_pOwner->GetModuleMotion();
	XModuleAction* pModuleAction = m_pOwner->GetModuleAction();
	if (pModuleMotion == NULL ||
		pModuleAction == NULL) return;

	bool bNeedPushIdleMotion = false;
	float fMotionSpeedRatio = 1.0f;

	if ( m_pOwner  &&  ( m_pOwner->GetEntityType() == ETID_NPC  ||  m_pOwner->GetEntityType() == ETID_PLAYER ||  m_pOwner->GetEntityType() == ETID_CUTSCENE) &&
		 m_pTalentInfo  &&  m_pTalentInfo->m_bAffectedMotionSpd)
	{
		XBuffAttribute attrBuff = m_pOwner->GetModuleBuff()->GetBuffAttribute();
		fMotionSpeedRatio = attrBuff.GetMoveSpeedRatio();
	}


	// �ִϸ��̼� ����
	if ((pModuleMotion != NULL) && ((!CSTalentInfoHelper::IsPlayerNormalTalentID(m_pTalentInfo->m_nID))))
	{
		if (nPhase == TALENT_PHASE_PREPARE)
		{
			if(ChangePrepareTalentMotion(pModuleMotion, szName, szLoopName, nType, fMotionSpeedRatio, bNeedPushIdleMotion) == false)
				return;
		}
		else if (nPhase == TALENT_PHASE_ACT)
		{
			if(ChangeActTalentMotion(pModuleMotion, pModuleAction, szName, szLoopName, nType, fMotionSpeedRatio, bNeedPushIdleMotion) == false)
			{
				// �ִϸ��̼��� ������ ������ �� �� �ִ°� ACT���̴�.
				XModuleActorControl* pModuleActorControl = m_pOwner->GetModuleActorControl();
				if(pModuleActorControl)
					pModuleActorControl->DoActionIdle();

				return;
			}

			bNeedPushIdleMotion = true;
		}
		else if (nPhase == TALENT_PHASE_EXTRA ||
				nPhase == TALENT_PHASE_EXTRA2 ||
				nPhase == TALENT_PHASE_EXTRA3)
		{
			wstring strExtraName = MOTION_NAME_TALENT_EXTRA;
			if(nPhase == TALENT_PHASE_EXTRA2)
				strExtraName = MOTION_NAME_TALENT_EXTRA2;
			else if(nPhase == TALENT_PHASE_EXTRA3)
				strExtraName = MOTION_NAME_TALENT_EXTRA3;

			if(ChangeTalentAnimation(pModuleMotion, strExtraName.c_str(), szName, fMotionSpeedRatio, false))
				bNeedPushIdleMotion = true;
		}

		if (bNeedPushIdleMotion)
		{
			wchar_t* szMotionName = MOTION_NAME_IDLE;
			if (m_pOwner->GetEntityType() == ETID_NPC) szMotionName = MOTION_NAME_NPC_IDLE;

			pModuleMotion->PushMotion(szMotionName);
		}
	}
}

//���� Ÿ�Ժ� �ִϸ��̼� ���� ����.
wstring XBaseTalent::_GetFullPlayerTalentAniName(const wchar_t* szName)
{
	bool bWeaponAni = false;

	if (m_pOwner->GetEntityType() == ETID_PLAYER)
	{
		XPlayer* pPlayer = static_cast<XPlayer*>(m_pOwner);

		return (XTalentHelper::MakeFullPlayerTalentAniName(szName, pPlayer->GetStance(), pPlayer->GetCurrWeaponType()));
	}

	if (m_pOwner->GetEntityType() == ETID_CUTSCENE && m_pOwner->IsPlayer())
	{
		XCutScenePlayer* pCutScenePlayer = static_cast<XCutScenePlayer*>(m_pOwner);

		return (XTalentHelper::MakeFullPlayerTalentAniName(szName, pCutScenePlayer->GetStance(), pCutScenePlayer->GetCurrWeaponType()));
	}
	return wstring(szName);
}

void XBaseTalent::ChangeTalentEffect(TALENT_PHASE nPhase)
{
	//if(m_pTalentEffectDataMgr == NULL)
	//	return;

	wstring strEffectFileName;
	wstring strFXPoint;
	vec3 vPos = m_pOwner->GetPosition();
	vec3 vDir = m_pOwner->GetDirection();

	// ĳ���� ����Ʈ ����
	ClearCastingEffect();

	switch (nPhase)
	{
	case TALENT_PHASE_PREPARE:
		{
			XModuleEffect * pModuleEffect = m_pOwner->GetModuleEffect();
			if(pModuleEffect)
			{
				XTalentEffectEventData * pCastEffectData = new XTalentEffectEventData;
				pCastEffectData->Set(m_pTalentInfo, CAST_EFFECT, m_pOwner, vec3(0.0f, 0.0f, 0.0f), NULL, true);
				pModuleEffect->OnEffectEvent(XEFTEVT_EFFECT_TALENT, pCastEffectData);
			}
		}
		break;
	case TALENT_PHASE_ACT:	
		{
			// ���
			XModuleEffect * pModuleEffect = m_pOwner->GetModuleEffect();
			if(pModuleEffect)
			{
				XTalentEffectEventData * pUseEffectData = new XTalentEffectEventData;
				pUseEffectData->Set(m_pTalentInfo, USE_EFFECT, m_pOwner);
				pModuleEffect->OnEffectEvent(XEFTEVT_EFFECT_TALENT, pUseEffectData);
			}
		}
		break;
	};
}

void XBaseTalent::ClearCastingEffect()
{
	XModuleEffect * pModuleEffect = m_pOwner->GetModuleEffect();
	if(pModuleEffect)
	{
		XTalentEffectEventData * pDelEffectData = new XTalentEffectEventData;
		pModuleEffect->OnEffectEvent(XEFTEVT_EFFECT_TALENT_DEL, pDelEffectData);
	}
}

void XBaseTalent::SetInfo( XTalentInfo* pTalentInfo )
{
	m_pTalentInfo = pTalentInfo;
}

XEventResult XBaseTalent::OnEvent( XEvent& msg )
{
	switch (msg.m_nID)
	{
		// CheckRequestSpell()���� m_strMotionName�� MOTION_NAME_TALENT_CASTING_LOOP ���� �ƴ��� üũ�� 
		// �ϴ� ���� ����� loop�̸� playonce �� üũ�� ���� �ʱ� ������ changemotion �߰�
	case XEVTL_ON_CHANGE_MOTION:
		{
			//XModuleMotion* pModuleMotion = m_pOwner->GetModuleMotion();
			//if (pModuleMotion)
			//{
			//	m_strMotionName = pModuleMotion->GetCurrMotion();
			//	if ((!_stricmp(pModuleMotion->GetCurrMotion().c_str(), MOTION_NAME_IDLE)) || (!_stricmp(pModuleMotion->GetCurrMotion().c_str(), MOTION_NAME_NPC_IDLE)))
			//	{
			//		m_bReqActSpell = false;
			//		FinishTalent();

			//		// ��ǥ ����
			//		XModuleEntity* pModuleEntity = m_pOwner->GetModuleEntity();
			//		if (pModuleEntity && m_pOwner->GetUID() != XGetMyUID())
			//		{
			//			vec3 vAniDir = pModuleEntity->GetAnimationDir();
			//			if (vAniDir != vec3::ZERO)
			//			{
			//				m_pOwner->SetDirection(vAniDir);
			//			}
			//		}
			//	}
			//}

// 			if (m_vReservedDirection != vec3::ZERO)
// 			{
// 				XModuleEntity* pModuleEntity = m_pOwner->GetModuleEntity();
// 				pModuleEntity->SetDirectionDelayed(m_vReservedDirection, this);
// 				m_vReservedDirection.Set(vec3::ZERO);
// 			}
		}
		break;
	case XEVTL_MOTION_PLAYONCE:
		{
// 			XModuleMotion* pModuleMotion = m_pOwner->GetModuleMotion();
// 			if (pModuleMotion)
// 			{
// 				m_strMotionName = pModuleMotion->GetCurrMotion();
// 				if ((!_stricmp(pModuleMotion->GetCurrMotion().c_str(), MOTION_NAME_IDLE)) ||
// 					(!_stricmp(pModuleMotion->GetCurrMotion().c_str(), MOTION_NAME_NPC_IDLE)))
// 				{
// 					m_bReqActSpell = false;
// 					FinishTalent();
// 				} 
// 				else if (!_stricmp(pModuleMotion->GetCurrMotion().c_str(), MOTION_NAME_TALENT_USE))
// 				{
// 					if (GetInfo()->m_bMoving)
// 					{
// 						XModuleEntity* pModuleEntity = m_pOwner->GetModuleEntity();
// 						if (pModuleEntity && m_pOwner->GetUID() != XGetMyUID())
// 						{
// 							vec3 vAniDir = pModuleEntity->GetAnimationDir();
// 							if (vAniDir != vec3::ZERO)
// 							{
// 								m_vReservedDirection = vAniDir;
// 							}
// 						}
// 					}
// 				}
// 			}
		}
		break;
	case XEVTL_ON_CHANGED_MOTION_TO_IDLE:
		{
			PostDelayTalent();
		}
		break;
	}

	return MR_FALSE;
}

void XBaseTalent::Act()
{
	m_vActPos = m_pOwner->GetPosition();

	m_bReqActSpell = false;
	if (IsActive())
	{
		SetAllowedActingFromServer(true);
	}

	if (IsNotUseActEventOnAct() == false)
	{
		m_pTalentEvent->SetActStart(true);
	}
}

bool XBaseTalent::IsNeedTarget()
{
	return false;
}

bool XBaseTalent::IsChangePrepare( XTalentInfo* pTalentInfo )
{
	if(m_pTalentInfo->m_fCastingTime > 0.0f ||							// ĳ���� �ð��� 0���� ũ�ų�,
		m_pTalentInfo->m_nSkillType == ST_ARCHERY) 						// ��ų Ÿ���� ü���ΰ��
	{
		return true;
	}

	return false;
}

void XBaseTalent::ReCastingTalent(int nTalentID, int nStep /* = 0 */)
{
	OnReCastingTalent(nTalentID, nStep);
}

void XBaseTalent::LaunchProjectile(CSProjectileInfo& csProjectilInfo)
{
	if(m_bActive == false)
		return;

	XProjectile* pNewProjectile = NULL;

	// �߻�ü Ÿ���� ������ �ȵȴ�.
	if(csProjectilInfo.m_nType == TPT_NONE)
		return;

	// ������ Ÿ���� �ִ�.
	if(IsNeedTarget() && 
		csProjectilInfo.m_nType == TPT_MISSILE_GUIDED)
	{
		// Ÿ���� �ִ°�?
		if(m_TalentTarget.uidTarget != MUID::ZERO)
		{
			pNewProjectile = static_cast<XProjectile*>(gg.omgr->CreateObject(XOBJ_PROJECTILE, global.system->GenerateLocalUID()));
			SetupTargetProjectile(pNewProjectile, csProjectilInfo, m_TalentTarget);
			pNewProjectile->SetIndex(0, 0);

			return;
		}
	}

	// �̻��ϰ� ��Ʈĸ���� Ÿ���� ����.
	if(m_vProjectileTargetPos != vec3::ZERO)
	{
		pNewProjectile = static_cast<XProjectile*>(gg.omgr->CreateObject(XOBJ_PROJECTILE, global.system->GenerateLocalUID()));
		SetupPosProjectile(pNewProjectile, csProjectilInfo, m_vProjectileTargetPos);
		//m_vProjectileTargetPos = vec3::ZERO;
	}
	else
	{
		XTALENT_TARGET emptyTarget;
		pNewProjectile = static_cast<XProjectile*>(gg.omgr->CreateObject(XOBJ_PROJECTILE, global.system->GenerateLocalUID()));
		SetupTargetProjectile(pNewProjectile, csProjectilInfo, emptyTarget);
		pNewProjectile->SetIndex(0, 0);
	}
}


void XBaseTalent::SetupTargetProjectile( XProjectile* pProjectile, CSProjectileInfo& csProjectilInfo, const XTALENT_TARGET& TargetInfo )
{
	if(pProjectile)
	{
		pProjectile->Create();
		gg.omgr->Add(pProjectile);

		// �߻� ��ġ ã��
		vec3 vecStartPos = GetProjectileStartPos(csProjectilInfo);
		vec3 dir = GetProjectileStartDir(vecStartPos, TargetInfo.uidTarget, TargetInfo.nGroupID, TargetInfo.nCapsuleID);
		vec3 velocity = dir * csProjectilInfo.m_fSpeed;
		vec3 accel = vec3::ZERO;
		float fWeight = 0.0f;

		MUID uidPlayer = m_pOwner->GetUID();
		pProjectile->Setup(uidPlayer, m_pTalentInfo, &csProjectilInfo, vecStartPos, velocity, m_vStartPos, TargetInfo.uidTarget, TargetInfo.nGroupID, TargetInfo.nCapsuleID);
	}
}

void XBaseTalent::SetupPosProjectile( XProjectile* pProjectile, CSProjectileInfo& csProjectilInfo, vec3 vTargetPos )
{
	if(pProjectile)
	{
		pProjectile->Create();
		gg.omgr->Add(pProjectile);

		float fWeight = 0.0f;

		// �߻� ��ġ ã��
		vec3 vecStartPos = GetProjectileStartPos(csProjectilInfo);
		vec3 dir = vTargetPos - vecStartPos;
		dir.Normalize();
		vec3 velocity = dir * csProjectilInfo.m_fSpeed;

		MUID uidPlayer = m_pOwner->GetUID();
		pProjectile->Setup(uidPlayer, m_pTalentInfo, &csProjectilInfo, vecStartPos, velocity, m_vStartPos);
	}
}

vec3 XBaseTalent::GetProjectileStartPos( CSProjectileInfo& csProjectileInfo )
{
	vec3 vStartPos;

	m_pOwner->GetWorldTransform().TransformVect(csProjectileInfo.m_vLocalStartPos, vStartPos);

	return vStartPos;
}

vec3 XBaseTalent::GetProjectileStartDir( vec3& vStartPos, MUID uidTarget, int nGroupID /*= -1*/, int nDummyID /*= -1*/ )
{
	vec3 dir = vec3(0, 0, 0);

	// ���� ���� �ƴ� �÷��̾ �����ؾ� �Ѵ�. PC�� NPC���..
	if (m_pOwner->GetUID() != XGetMyUID())
	{
		dir = m_pOwner->GetDirection();
	}
	else
	{
		XBaseCameraSceneNode* pCameraNode = global.camera->GetCamera();

		if (!pCameraNode) 
			return dir;

		dir = m_pOwner->GetModuleEntity()->GetDirection();
		dir.z = pCameraNode->GetDirection().z;

		dir.Normalize();
	}

	// �κ� ���� Ÿ���̶��...
	if(nGroupID >= 0 &&
		nDummyID >= 0)
	{
		XObject* pObject = gg.omgr->Find(uidTarget);
		if(pObject)
		{
			vec3 vCapsuleCenter = pObject->GetModuleCollision()->GetHitEffectProjectileTargetPos(nGroupID, nDummyID);
			if(vCapsuleCenter != vec3::ZERO)
			{
				dir = (vCapsuleCenter - vStartPos).Normalize();
			}
		}
	}

	return dir;
}

void XBaseTalent::ExtraAct()
{
	ChangePhase(TALENT_PHASE_EXTRA);
}

void XBaseTalent::UpdateRotationAnimation()
{
	// ȸ�� �ִϸ��̼��� ���� �ŷ�Ʈ�� ���ÿ� ���� ȸ���� ó�� �Լ�
//	XModuleMotion* pModuleMotion = m_pOwner->GetModuleMotion();
//	if (pModuleMotion)
//	{
//		m_strMotionName = pModuleMotion->GetCurrMotion();
//		if (!_stricmp(pModuleMotion->GetCurrMotion().c_str(), MOTION_NAME_TALENT_USE))
//		{
//			if (GetInfo()->m_bMoving)
//			{
//				XModuleEntity* pModuleEntity = m_pOwner->GetModuleEntity();
//				if (pModuleEntity && m_pOwner->GetUID() != XGetMyUID())
//				{
//					vec3 vAniDir = pModuleEntity->GetAnimationDir();
//					if (vAniDir != vec3::ZERO)
//					{
//						m_vReservedDirection = vAniDir;
//					}
//				}
//			}
//		}
//	}
}

void XBaseTalent::OnStart()
{
	// �ŷ�Ʈ ������ �ܺο� �˸��ϴ�.
	XEvent evt;
	evt.m_nID			= XEVTL_TALENT_START; 
	evt.m_pData			= m_pTalentInfo;

	m_pOwner->Event(evt);
}

void XBaseTalent::PreLoaderTalentEffect(XTalentInfo* pTalentInfo)
{
	XModuleEffect * pModuleEffect = m_pOwner->GetModuleEffect();
	if(pModuleEffect)
	{
		XTalentEffectEventData * pPreLoaderEffectData = new XTalentEffectEventData;
		pPreLoaderEffectData->m_nParam1 = pTalentInfo->m_nID;
		pModuleEffect->OnEffectEvent(XEFTEVT_EFFECT_TALENT_EFFECT_PRE_LOADER, pPreLoaderEffectData);
	}
}

void XBaseTalent::ExtraAct2()
{
	ChangePhase(TALENT_PHASE_EXTRA2);
}

void XBaseTalent::ExtraAct3()
{
	ChangePhase(TALENT_PHASE_EXTRA3);
}

void XBaseTalent::DelayAct( wstring strParam )
{
	// ��� ������ ���� �ɰ��̹Ƿ� �׶� �׶� �����丵 ��Ź�帳�ϴ�.

	if(m_pTalentInfo == NULL)
		return;

	// ���� Ÿ���ΰ�?
	if(m_pTalentInfo->m_eTargetingType == TTT_GROUND)
	{
		// ���� ����Ʈ�� �����ش�.
		XEffectSinglyInvoker effectInvoker;
		effectInvoker.Invoke(strParam, m_TalentTarget.vTargetPos, m_pOwner->GetDirection(), vec3(vec3::AXISZ));
	}
}

bool XBaseTalent::ChangePrepareTalentMotion( XModuleMotion* pMotion, const wchar_t* szName, const wchar_t* szLoopName, MOVING_USE_TYPE nType, float fMotionSpeedRatio, bool& bNeedPushIdleMotion )
{
	bool bUpper = false;
	if(nType == MUT_CASTING)
		bUpper = true;

	if(ChangeTalentAnimation(pMotion, MOTION_NAME_TALENT_CASTING, szName, fMotionSpeedRatio, bUpper))
	{
		PushTalentAnimation(pMotion, MOTION_NAME_TALENT_CASTING_LOOP, szLoopName, fMotionSpeedRatio, bUpper);
	}
	else
	{
		ChangeTalentAnimation(pMotion, MOTION_NAME_TALENT_CASTING_LOOP, szLoopName, fMotionSpeedRatio, bUpper);
	}

	if (m_pTalentInfo->m_nSkillType == ST_ARCHERY)
	{
		bNeedPushIdleMotion = true;
	}

	return true;
}

bool XBaseTalent::ChangeActTalentMotion( XModuleMotion* pMotion, XModuleAction* pAction, const wchar_t* szName, const wchar_t* szLoopName, MOVING_USE_TYPE nType, float fMotionSpeedRatio, bool& bNeedPushIdleMotion )
{
	if(wstring(szName) == L"")
		return false;

	if (XMotionMgr::IsTalentMotionName(szName))
	{
		wstring t = szName;
		wstring motion, motionret = L"";
		int f = t.find(L";");
		if (f != wstring::npos)
		{
			motion = t.substr(0, f);
			motionret = t.substr(f+1);
		}
		else
			motion = t;

		bool bChange = false;
		if(nType == MUT_USE)
		{
			if(pMotion->ChangeUpperMotion(motion.c_str(), MT_DEFAULT, true))
				bChange = true;
		}
		else if ( pMotion->ChangeMotion(motion.c_str(), MT_DEFAULT, true))
			bChange = true;

		if(bChange)
			pMotion->SetSpeed( pMotion->GetSpeed() * fMotionSpeedRatio);
		else
		{
			return false;
		}

		if (motionret != L"")
		{
			pMotion->PushMotion(motionret.c_str(), pMotion->GetSpeed() * fMotionSpeedRatio);
		}
	}
	else
	{
		wstring strCurrAni = _GetFullPlayerTalentAniName(szName);

		bool bChange = false;
		if(nType == MUT_USE)
		{
			if(pMotion->ChangeToCustomUpperMotion(MOTION_NAME_TALENT_USE, strCurrAni.c_str(), true))
				bChange = true;
		}
		else if ( pMotion->ChangeToCustomMotion(MOTION_NAME_TALENT_USE, strCurrAni.c_str(), true))
			bChange = true;

		if(bChange)
			pMotion->SetSpeed( pMotion->GetSpeed() * fMotionSpeedRatio);
		else
		{
			return false;
		}
	}

	return true;
}

bool XBaseTalent::ChangeTalentAnimation( XModuleMotion* pMotion, const wchar_t* szMotionName, const wchar_t* szName, float fMotionSpeedRatio, bool bUpper )
{
	if(wstring(szName) == L"")
		return false;

	if (XMotionMgr::IsTalentMotionName(szName))
	{
		if(bUpper)
		{
			if(pMotion->ChangeUpperMotion(szMotionName))
			{
				pMotion->SetUpperSpeed(pMotion->GetSpeed() * fMotionSpeedRatio);
				return true;
			}
		}
		else if(pMotion->ChangeMotion(szName))
		{
			pMotion->SetSpeed(pMotion->GetSpeed() * fMotionSpeedRatio);
			return true;
		}
	}
	else
	{
		wstring strCurrAni = _GetFullPlayerTalentAniName(szName);
		if(pMotion->ChangeToCustomMotion(szMotionName, strCurrAni.c_str()))
		{
			pMotion->SetSpeed(pMotion->GetSpeed() * fMotionSpeedRatio);
			return true;
		}
	}

	return false;
}

bool XBaseTalent::PushTalentAnimation( XModuleMotion* pMotion, const wchar_t* szMotionName, const wchar_t* szName, float fMotionSpeedRatio, bool bUpper )
{
	if (XMotionMgr::IsTalentMotionName(szName))
	{
		// pushmotion������ 1.0f�� �ƴѰ�쿡�� ���ǵ尡 ������ �ȴ�.
		pMotion->PushMotion(szName, 1.0f, fMotionSpeedRatio);
		return true;
	}
	else if(wstring(szName) != L"")
	{
		wstring strCurrAni = _GetFullPlayerTalentAniName(szName);
		pMotion->PushCustomMotion(szMotionName, strCurrAni.c_str(), 1.0f, fMotionSpeedRatio);
		return true;
	}

	return false;
}

void XBaseTalent::PostDelayTalent()
{
	m_bActive = false;

	ChangePhase(TALENT_PHASE_POST_DELAY);
	ClearCastingEffect();
}

void XBaseTalent::CheckPostDelayTalent( float fDelta )
{
	if(m_fPostDelayRemainTime > 0.0f)
	{
		m_fPostDelayRemainTime -= fDelta;

		if(m_fPostDelayRemainTime <= 0.0f)
			FinishTalent();
	}
}
