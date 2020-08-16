#include "stdafx.h"
#include "XMyActionHit.h"
#include "XModuleMyControl.h"
#include "XModuleAction.h"
#include "XModuleEntity.h"
#include "XModulePost.h"
#include "XModuleMovable.h"
#include "XObject.h"
#include "XEventID.h"
#include "XTalentInfo.h"
#include "XMyPlayer.h"
#include "XCharacter.h"
#include "XTalentHitParam.h"
#include "XMotionHelper.h"
#include "XMotionFactorHelper.h"
#include "XGameState.h"
#include "XGameTransformControllerManager.h"
#include "CSMotionFactorHelper.h"
#include "XPost_Action.h"

#define END_ALLOW_HEIGHT	50.0f

// Hitted Action State //////////////////////////////////////////////////////////
bool XMyActionHit::CheckEnterable(void* pParam)
{
	XTalentHitParam * pHitInfo = static_cast<XTalentHitParam *>(pParam);
	if(pHitInfo == NULL)
		return false;

	if (CSMotionFactorHelper::IsNoneOrFakes(pHitInfo->nMFState)) return false;

	return true;
}

void XMyActionHit::Enter(void* pParam)
{
	m_nEnterTime = XGetNowTime();
	m_bDone = false;
	m_bReservedEnd = false;

	m_nMFState = MF_NONE;
	m_nMFWeight = 0;

	m_vPrePos = GetOwnerPlayer()->GetPosition();

 	XMyActionState::Enter(pParam);

	XTalentHitParam * pHitInfo = static_cast<XTalentHitParam *>(pParam);
	if(pHitInfo == NULL)
		return;

	XModuleAction* pModuleAction = m_pOwner->GetOwner()->GetModuleAction();
	if(pModuleAction)
	{
		pModuleAction->StopMove(true);

		m_nMFState = (MF_STATE)pHitInfo->nMFState;
		if (m_nMFState == MF_NONE &&	pModuleAction->CheckUsableBeatenFake()) 
			m_bDone = true;

		m_nMFWeight = pHitInfo->nMFWeight;

		// ���� �ð����� �������� ���Ѵ� State
		CheckMFLockTime(m_nMFState);

		stMotionFactorResult rtMotionFactor;
		stServerKnockbackVictimInfo knockbackInfo(pHitInfo->vServerKnockbackDir, pHitInfo->vServerKnockbackVictimPos, pHitInfo->vServerKnockbackVictimTargetPos);
		pModuleAction->ActMF(m_nMFState, m_nMFWeight, pHitInfo->nAttackerUIID, knockbackInfo, pHitInfo->nTalentID, rtMotionFactor);

		// ���� ó��...
		if(m_pOwner->GetMyPlayerOwner()->IsDead())
		{
			m_pOwner->OnDie(pHitInfo->nMFState, rtMotionFactor.bUseMFAnimation);
		}
	}
}

void XMyActionHit::Exit()
{
	bool bDie = m_pOwner->GetMyPlayerOwner()->IsDead();
	// MF_KNOCKBACK
	// 
	// knockback �� ��� ��ġ sync�� �ٽ� �����ش�. 
	if (bDie == false &&
		(m_nMFState == MF_KNOCKBACK ||
		m_nMFState == MF_THROWUP ||
		m_nMFState == MF_DRAG))
	{
		XModulePost* pModulePost = m_pOwner->GetOwner()->GetModulePost();
		pModulePost->PostStop(m_pOwner->GetOwner()->GetPosition(),
							  m_pOwner->GetOwner()->GetDirection());
	}


	m_bReservedEnd = false;

	// �������ε�... �������...
	if(m_nMFState == MF_STUN)
	{
		XModuleAction* pModuleAction = m_pOwner->GetOwner()->GetModuleAction();
		if(pModuleAction)
		{
			// exit���... ����̺�Ʈ�� ������ �ʽ��ϴ�.
			m_nMFState = MF_NONE;
			m_bDoneLockTime = true;
			pModuleAction->EndStun(false);
		}
	}

	XMyActionState::Exit();
}


XEventResult XMyActionHit::Event(XEvent& msg)
{
	switch(msg.m_nID)
	{
	case XEVTD_END_MF_STUN:
	case XEVTD_END_MF_KNOCKDOWN:
	case XEVTL_ON_CHANGED_MOTION_TO_IDLE:		// ���ڱ� Idle�̸� ����
		{
			if(msg.m_nID == XEVTL_ON_CHANGED_MOTION_TO_IDLE &&
				m_pOwner->GetMyPlayerOwner()->IsDead())
			{
				// �׾��µ�... Idle��? �� ���� ����ó��
				m_pOwner->DoAction(ACTION_STATE_DIE);

				return MR_TRUE;
			}

			// ����, �˴ٿ�
			// ������.
			EndMFLockTime();
		}
		break;
	}

	if (XMyActionState::Event(msg) == MR_TRUE) return MR_TRUE;

	return MR_FALSE;
}


void XMyActionHit::Update(float fDelta)
{
	bool bCheckMotion = !m_DeathController.IsDeadMotionImmediately(m_nMFState);

	if (m_ActionDieTransChecker.IsReadyForHitState(fDelta, m_pOwner->GetMyPlayerOwner(), bCheckMotion)) 
	{
		if (m_pOwner->DoAction(ACTION_STATE_DIE))
		{
			return;
		}
	}

	UpdateCheckMFReleaseTime();

	if (m_bDone)
	{
		m_pOwner->DoActionIdle();
	}

	m_vPrePos = GetOwnerPlayer()->GetPosition();
}

bool XMyActionHit::OnSpecialAction( SPECIAL_ACTION_COMMAND nSpecialActionCommand )
{
	if (nSpecialActionCommand == SPECIAL_KEY_ADVANCE_ATTACK)
	{
		if (gvar.MyInfo.Talent.HasExtraPassiveTalent(TEPT_RISING_ATTACK) == false) return false;
		if (m_pOwner->GetMyPlayerOwner()->GetStance() == CS_NORMAL) return false;

		XModuleEntity* pModuleEntity = m_pOwner->GetOwner()->GetModuleEntity();
		if (pModuleEntity)
		{
			wstring t = pModuleEntity->GetAnimationController()->GetAnimationName();
			if (t.length() >= 6)
			{
				if (t.substr(t.length()-6) == L"MF_KD2")
				{
					m_pOwner->DoAction(ACTION_STATE_NORMAL_ATTACK, NA_RISING);	// ����� �ϵ��ڵ�-_-
					return true;
				}
			}
		}
	}
	else if (nSpecialActionCommand == SPECIAL_KEY_QUICK_RISE)
	{
		//if (gvar.MyInfo.Talent.HasExtraPassiveTalent(TEPT_SWORD_DRAWING) == false) return false;
		XModuleMotion* pModuleMotion = m_pOwner->GetOwner()->GetModuleMotion();
		if (!pModuleMotion) return false;

		if (pModuleMotion->IsCurrentMotion(MOTION_NAME_MF_KNOCKDOWN))
		{
			XPostStandUp();
		}
	}

	return false;
}

bool XMyActionHit::CheckExitable(int nNextStateID)
{
	if(nNextStateID == ACTION_STATE_DIE)
	{
		return true;
	}

		
	// �����ð����� lock�� ���� Exit �� �� ����.
	if(m_bDoneLockTime == false &&
		(m_nMFState == MF_STUN || m_nMFState == MF_KNOCKDOWN))
	{
		return false;
	}

	if (m_nMFState == MF_UPPERED)
	{
		return  CheckColTerrain();

	}

	return true;
}

void XMyActionHit::CheckMFLockTime( MF_STATE mfState )
{
	// ���� �ð����� �������� ���Ѵ� State
	if(m_nMFState == MF_STUN ||
		m_nMFState == MF_KNOCKDOWN)
	{
		m_bDoneLockTime = false;
	}
}

void XMyActionHit::EndMFLockTime()
{
	// ����, �˴ٿ�
	// ������.
	if(m_nMFState == MF_STUN ||
		m_nMFState == MF_KNOCKDOWN)
	{
		m_nMFState = MF_NONE;
	}

	m_bDoneLockTime = true;

	// ���� �ִϸ��̼��� idle �̶��... DoAction �ߵ�
	// �ִϸ��̼��� ���� ��쿡 ���� ó��
	XModuleAction* pModuleAction = m_pOwner->GetOwner()->GetModuleAction();
	if(pModuleAction)
	{
		if (XMotionHelper::IsIdleMotion(pModuleAction->GetCurrentMotionName()))
		{
			m_pOwner->DoActionIdle();
		}
	}
}

bool XMyActionHit::IsPostMovement()
{
	if(m_nMFState == MF_KNOCKDOWN)
		return true;

	return false;
}

void XMyActionHit::UpdateCheckMFReleaseTime()
{
	// release�� �ȵ� ��츦 ����� ������å

	float fElapsedTime = (XGetNowTime() - m_nEnterTime) / 1000.0f;

	switch (m_nMFState)
	{
	case MF_UPPERED:
		{
			if (gg.currentgamestate == NULL || gg.currentgamestate->GetWorld() == NULL) return;

			float fHeight = CalcHeightFromGroundZ();

			const float TOLER_TIME = 0.1f;
			const float TOLER_HEIGHT = 40.0f;

			float fReleaseTime = XMotionFactorHelper::CalcUpperedTime(m_nMFWeight);
			if (fElapsedTime > fReleaseTime + TOLER_TIME &&
				CheckColTerrain())
			{
				wstring strCurrMotionSequence = GetOwnerPlayer()->GetModuleMotion()->GetCurrSequence();
				if (strCurrMotionSequence != L"jump_land")
				{
					m_bDone = true;
				}
			}
		}
		break;
	}
}

float XMyActionHit::CalcHeightFromGroundZ()
{
	vec3 vOwnerPos = GetOwnerPlayer()->GetPosition();
	float fGroundZ = gg.currentgamestate->GetWorld()->GetGroundZ( vOwnerPos);
	float fHeight = vOwnerPos.z - fGroundZ;

	return fHeight;
}

bool XMyActionHit::CheckColTerrain()
{
	XModuleMovable* pModuleMovable = m_pOwner->GetOwner()->GetModuleMovable();
	if(pModuleMovable == NULL)
		return false;

	return pModuleMovable->GetMovableFactor().GENERAL_STATE.m_bColTerrain;
}