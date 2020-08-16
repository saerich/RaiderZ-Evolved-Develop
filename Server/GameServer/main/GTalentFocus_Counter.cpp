#include "StdAfx.h"
#include "GTalentFocus_Counter.h"
#include "GConst.h"
#include "GEntityPlayer.h"

GTalentFocus_Counter::GTalentFocus_Counter(GEntityPlayer* pOnwer)
: GTalentFocus(pOnwer)
, m_nGuardCount(0)
{
	AddEnableWeapon(WEAPON_1H_SLASH);
	AddEnableWeapon(WEAPON_1H_BLUNT);
}

GTalentFocus_Counter::~GTalentFocus_Counter(void)
{
}

bool GTalentFocus_Counter::CheckActivate()
{
	if (!CheckLicense())
		return false; // ���̼��� ������ ���� ����

	if (m_nGuardCount > 0)
	{
		m_nGuardCount = 0;
		return true;
	}

	return false;
}

float GTalentFocus_Counter::GetDurationTime()
{
	return GConst::FOCUS_COUNTER_DURATION_TIME + (float)m_pOwner->GetPassiveValue(TEPT_FOCUS_COUNTER_EXTENDTIME);
}

void GTalentFocus_Counter::OnGuard(GEntityActor* pAttacker)
{
	VALID(pAttacker);

	// SEAL(pyo): ���÷�Ʈ�߿� ��Ŀ�� �Ⱦ�� ����
	// TEST_FIXTURE(FTalentFocus, GainCounterFocusOnOpponentDeflect)
	//
	//if (pAttacker->GetCurrMF() != MF_DEFLECT)
	//	return;		// �����ڰ� ���÷�Ʈ �ɸ����� ��Ŀ���� ����

	//m_nGuardCount++;
}

void GTalentFocus_Counter::OnAbsoluteGuard(GEntityActor* pAttacker)
{
	VALID(pAttacker);

	if (!CheckLicense())
		return; // ���̼��� ������ ���� ����

	m_nGuardCount++;
}

bool GTalentFocus_Counter::CheckLicense() const 
{
	return (m_pOwner->HasPassiveTalent(TEPT_FOCUS_USE_COUNTER));
}