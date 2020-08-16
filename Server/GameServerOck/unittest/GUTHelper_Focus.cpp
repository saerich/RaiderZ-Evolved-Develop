#include "StdAfx.h"
#include "GUTHelper_Focus.h"
#include "GTestForward.h"

void GUTHelper_Focus::ClearFocus( GEntityPlayer* pUser )
{
	VALID(pUser);
	VALID(pUser->GetField());
	pUser->doDie();
	pUser->doRebirthBySelf();
	pUser->GetField()->Update(GConst::INVINCIBLE_TIME_REVIVE);	// ��Ȱ �����ð� �ѱ�
}

void GUTHelper_Focus::GainBerserk(GEntityPlayer* pUser, bool bLearnLicense, int nHitCounter)
{
	VALID(pUser);
	VALID(pUser->GetField());

	if (bLearnLicense)
	{
		LearnLicense_Berserk(pUser);

	}

	if (nHitCounter == -1)
	{
		nHitCounter = GConst::FOCUS_BERSERK_HIT_COUNT;
	}

	// ��Ŀ�� ����
	m_HelperBuff.NewBuffInfo(BUFF_FOCUS_ID[TFT_BERSERK]);

	GEntityPlayer* pDefender = GUTHelper::NewEntityPlayer(pUser->GetField(), vec3(100,100,0));
	GTalentInfo* p1DmgTalent = m_HelperTalent.MakeDamageTalent(1);

	// �������°� �ɶ����� ����
	for (int i=0; i<nHitCounter; ++i)
	{
		m_HelperTalent.UseTalent(pUser, p1DmgTalent, pDefender->GetUID());
	}

	// ���� ���� ���� ����
	pDefender->DeleteMe();
	pUser->GetField()->Update(0.0f);
}

void GUTHelper_Focus::GainCounter( GEntityPlayer* pUser, bool bLearnLicense )
{
	VALID(pUser);
	VALID(pUser->GetField());

	if (bLearnLicense)
	{
		LearnLicense_Counter(pUser);
	}

	// ��Ŀ�� ����
	m_HelperBuff.NewBuffInfo(BUFF_FOCUS_ID[TFT_COUNTER]);
	GTalentInfo* pCounterTalent = test.talent.NewMagicTalentInfo();
	pCounterTalent->m_nNextFocus = TFT_COUNTER;
	pCounterTalent->m_nTiming = TC_USE_TALENT;

	// �ݰ� ��Ŀ���� �����Ŀ� �ݰ� ��Ŀ���� �ʿ��� �ŷ�Ʈ�� ���
	test.talent.UseTalent(pUser, pCounterTalent, pUser->GetUID());
}

void GUTHelper_Focus::ToggleAdvent(GEntityActor* pUser)
{
	VALID(pUser);
	
	// ��Ŀ�� ����
	m_HelperBuff.NewBuffInfo(BUFF_FOCUS_ID[TFT_ADVENT]);

	GTalentInfo* pActiveTalent = m_HelperTalent.NewTalentInfo();
	m_HelperTalent.SetExtraActiveTalentInfo(pActiveTalent, 1, TEAT_ADVENT);
	m_HelperTalent.UseTalent(pUser, pActiveTalent, pUser->GetUID());
}

void GUTHelper_Focus::ToggleSneak(GEntityActor* pUser)
{
	VALID(pUser);
	
	// ��Ŀ�� ����
	m_HelperBuff.NewBuffInfo(BUFF_FOCUS_ID[TFT_SNEAK]);

	GTalentInfo* pActiveTalent = m_HelperTalent.NewTalentInfo();
	m_HelperTalent.SetExtraActiveTalentInfo(pActiveTalent, 1, TEAT_TOGGLE_SNEAK);
	m_HelperTalent.UseTalent(pUser, pActiveTalent, pUser->GetUID());
}

void GUTHelper_Focus::TogglePrecision(GEntityActor* pUser)
{
	VALID(pUser);

	// ��Ŀ�� ����
	m_HelperBuff.NewBuffInfo(BUFF_FOCUS_ID[TFT_PRECISION]);

	GTalentInfo* pActiveTalent = m_HelperTalent.NewTalentInfo();
	m_HelperTalent.SetExtraActiveTalentInfo(pActiveTalent, 1, TEAT_TOGGLE_PRECISION);
	m_HelperTalent.UseTalent(pUser, pActiveTalent, pUser->GetUID());
}

void GUTHelper_Focus::GainEnlighten(GEntityPlayer* pUser, int nHitCounter)
{
	VALID(pUser);
	VALID(pUser->GetField());

	// ��Ŀ�� ����
	GBuffInfo* pBuffInfo = m_HelperBuff.NewBuffInfo(BUFF_FOCUS_ID[TFT_ENLIGHTEN]);

	test.buff.LostBuff(pUser, BUFF_FOCUS_ID[TFT_ENLIGHTEN]);
	test.buff.GainBuffSelf(pUser, pBuffInfo, BUFF_DURATION_INFINITY);
}

void GUTHelper_Focus::GainItemForEnlighten( GEntityPlayer* pUser )
{
	m_HelperItem.EquipNewWeapon(pUser, WEAPON_STAFF);
}

void GUTHelper_Focus::GainItemForAdvent( GEntityPlayer* pUser )
{
	m_HelperItem.EquipNewWeapon(pUser, WEAPON_1H_BLUNT);
}

void GUTHelper_Focus::GainItemForCounter( GEntityPlayer* pUser )
{
	m_HelperItem.EquipNewWeapon(pUser, WEAPON_1H_BLUNT);
}

void GUTHelper_Focus::GainItemForBerserk( GEntityPlayer* pUser )
{
	m_HelperItem.EquipNewWeapon(pUser, WEAPON_TWO_HANDED);
}

void GUTHelper_Focus::GainItemForPrecision( GEntityPlayer* pUser )
{
	m_HelperItem.EquipNewWeapon(pUser, WEAPON_ARCHERY);
}

void GUTHelper_Focus::GainItemForSneak( GEntityPlayer* pUser )
{
	m_HelperItem.EquipNewWeapon(pUser, WEAPON_1H_PIERCE);
}

void GUTHelper_Focus::LearnLicense_Counter( GEntityPlayer* pUser )
{
	// ���̼��� ����
	GTalentInfo* pTalent = test.talent.NewTalentInfo();
	pTalent->m_nTalentType = TT_SKILL;
	pTalent->m_nSkillType = ST_EXTRA_PASSIVE;
	pTalent->m_nExtraPassive = TEPT_FOCUS_USE_COUNTER;
	test.talent.Train(pUser, pTalent);
}

void GUTHelper_Focus::LearnLicense_Berserk( GEntityPlayer* pUser )
{
	// ���̼��� ����
	GTalentInfo* pTalent = test.talent.NewTalentInfo();
	pTalent->m_nTalentType = TT_SKILL;
	pTalent->m_nSkillType = ST_EXTRA_PASSIVE;
	pTalent->m_nExtraPassive = TEPT_FOCUS_USE_BERSERK;
	test.talent.Train(pUser, pTalent);
}