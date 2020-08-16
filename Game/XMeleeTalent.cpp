#include "stdafx.h"
#include "XMeleeTalent.h"
#include "XModuleEffect.h"
#include "XModuleMotion.h"
#include "XEventID.h"

void XMeleeTalent::OnEnterPhasePrepare()
{
	// �ִϸ��̼� ����
	ChangeTalentMotion(TALENT_PHASE_PREPARE, m_pTalentInfo->m_szCastingAnimation.c_str(), m_pTalentInfo->m_szCastingLoopAnimation.c_str(), m_pTalentInfo->m_nMovableType);
	// ����Ʈ ����
	ChangeTalentEffect(TALENT_PHASE_PREPARE);
	// �˱�
	SwordTarilStart();
}

void XMeleeTalent::OnEnterPhaseAct()
{
	// �ִϸ��̼� ����
	ChangeTalentMotion(TALENT_PHASE_ACT, m_pTalentInfo->m_szUseAnimation.c_str(), L"", m_pTalentInfo->m_nMovableType);
	// ����Ʈ ����
	ChangeTalentEffect(TALENT_PHASE_ACT);
	// �˱�
	SwordTarilStart();
}

void XMeleeTalent::OnEnterPhaseFinish()
{
}

void XMeleeTalent::SwordTarilStart()
{
	XModuleEffect * pModuleEffect = m_pOwner->GetModuleEffect();
	if(pModuleEffect)
	{
		pModuleEffect->OnEffectEvent(XEFTEVT_EFFECT_SWORDTRAIL);
	}
}

void XMeleeTalent::SwordTarilEnd()
{
	XModuleEffect * pModuleEffect = m_pOwner->GetModuleEffect();
	if(pModuleEffect)
	{
		pModuleEffect->OnEffectEvent(XEFTEVT_EFFECT_END_SWORDTRAIL);
	}
}

XEventResult XMeleeTalent::OnEvent( XEvent& msg )
{
	switch (msg.m_nID)
	{
	case XEVTL_ON_CHANGE_MOTION:
		{
			SwordTarilEnd();
		}
		break;
	case XEVTL_ON_CHANGED_MOTION_TO_IDLE:
		{
			if(m_pTalentInfo->m_nMovableType == MUT_NONE &&
				m_fActElapsedTime > 0.0f)
			{
				PostDelayTalent();
			}
		}
		break;
	}

	// XBaseTalent::OnEvent ó���� ���� ����. ������ �����. 
	// ���� ���� ������ �ϸ� m_bActive�� false�� �Ǿ move_dummy�� �۵��� �ȵȴ�.
	// Module ���� ������... �׷� ������ �����. �� XBaseTalent::OnEvent�� �ִ� �޼����� ���� ������ ���⼭ ���� �ۼ�����.
	return MR_FALSE;
}
