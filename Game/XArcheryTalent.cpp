#include "stdafx.h"
#include "XArcheryTalent.h"
#include "XTarget.h"
#include "XEventID.h"
#include "XAnim.h"
#include "XProjectile.h"

void XArcheryTalent::OnEnterPhasePrepare()
{
	// �ִϸ��̼� ����
	ChangeTalentMotion(TALENT_PHASE_PREPARE, m_pTalentInfo->m_szCastingAnimation.c_str(), m_pTalentInfo->m_szCastingLoopAnimation.c_str());
	// ����Ʈ ����
	ChangeTalentEffect(TALENT_PHASE_PREPARE);
}

void XArcheryTalent::OnEnterPhaseAct()
{
	XEvent evt;
	evt.m_nID = XEVTL_ON_MOTION_EVENT; 
	evt.m_pData = MOTION_EVENT_DOSHOOT;
	m_pOwner->Event(evt);
}

void XArcheryTalent::OnStart()
{
}
