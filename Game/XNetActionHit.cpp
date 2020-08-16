#include "stdafx.h"
#include "XNetActionHit.h"
#include "XModuleNetControl.h"
#include "XModuleAction.h"
#include "XTalentHitParam.h"

// Hitted Action State //////////////////////////////////////////////////////////
void XNetActionHit::Enter(void* pParam)
{
	m_nMFState = MF_NONE;
	m_nMFWeight = 0;

 	XNetActionState::Enter(pParam);

	XTalentHitParam * pHitInfo = static_cast<XTalentHitParam *>(pParam);
	if(pHitInfo == NULL)
		return;

	stMotionFactorResult rtMotionFactor;
	XModuleAction* pModuleAction = m_pOwner->GetOwner()->GetModuleAction();
	if(pModuleAction)
	{
		m_nMFState = (MF_STATE)pHitInfo->nMFState;
		if (m_nMFState == MF_NONE &&	pModuleAction->CheckUsableBeatenFake()) 
			m_bDone = true;

		m_nMFWeight = pHitInfo->nMFWeight;

		stServerKnockbackVictimInfo knockbackInfo(pHitInfo->vServerKnockbackDir, pHitInfo->vServerKnockbackVictimPos, pHitInfo->vServerKnockbackVictimTargetPos);
		pModuleAction->ActMF(m_nMFState, m_nMFWeight, pHitInfo->nAttackerUIID, knockbackInfo, pHitInfo->nTalentID, rtMotionFactor);
	}

	// ���� ó��...
	if(m_pOwner->GetOwner()->IsDead())
	{
		m_pOwner->OnDie(pHitInfo->nMFState, rtMotionFactor.bUseMFAnimation);
	}
}

XEventResult XNetActionHit::Event(XEvent& msg)
{
	switch(msg.m_nID)
	{
	case XEVTD_END_MF_STUN:
	case XEVTD_END_MF_KNOCKDOWN:
	case XEVTL_ON_CHANGED_MOTION_TO_IDLE:		// ���ڱ� Idle�̸� ����
		{
			// ����, �˴ٿ�
			// ������.
			EndMFLockTime();
		}
		break;
	}

	if (XNetActionState::Event(msg) == MR_TRUE) return MR_TRUE;

	return MR_FALSE;
}

bool XNetActionHit::CheckExitable(int nNextStateID)
{
	if(nNextStateID == ACTION_STATE_DIE)
	{
		return true;
	}
		
	// �����ð����� lock�� ���� Exit �� �� ����.
	if(m_nMFState == MF_STUN || m_nMFState == MF_KNOCKDOWN)
	{
		return false;
	}

	return true;
}

void XNetActionHit::EndMFLockTime()
{
	// ����, �˴ٿ�
	// ������.
	if(m_nMFState == MF_STUN ||
		m_nMFState == MF_KNOCKDOWN)
	{
		m_nMFState = MF_NONE;
	}

	// ���� �ִϸ��̼��� idle �̶��... DoAction �ߵ�
	// �ִϸ��̼��� ���� ��쿡 ���� ó��
	XModuleAction* pModuleAction = m_pOwner->GetOwner()->GetModuleAction();
	if(pModuleAction)
	{
		m_pOwner->DoAction( NET_ACTION_STATE_NA);
	}
}

void XNetActionHit::Update( float fDelta )
{
	if(m_bDone)
	{
		XModuleAction* pModuleAction = m_pOwner->GetOwner()->GetModuleAction();
		if(pModuleAction)
		{
			m_pOwner->DoAction( NET_ACTION_STATE_NA);
		}
	}
}