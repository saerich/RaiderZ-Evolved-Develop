#include "StdAfx.h"
#include "GTestForward.h"
#include "GEntityPlayer.h"
#include "GModuleCombat.h"
#include "GServer.h"
#include "GFieldMgr.h"
#include "FBasePlayer.h"

SUITE(TalentFocus_Sneak)
{
	struct FTalentFocus : public FBasePlayer
	{
		FTalentFocus()
		{ 
			m_pField = GUTHelper_Field::DefaultMockField();
			m_pPlayer = NewEntityPlayer(m_pField, vec3(100,100,0));
			m_pLinkTester = test.network.NewLink(m_pPlayer);
			swprintf_s(m_pPlayer->GetPlayerInfo()->szName, L"Tester");

			gsys.pServer->FrameCounterClear();
			SetIgnoreCommonCommand(m_pLinkTester);
		}

		~FTalentFocus() 
		{
			m_pField->Destroy();
			test.network.ClearLinks();
		}

		void BeginCommandRecord()
		{
			m_pLinkTester->ResetCommands();
		}

		void SetIgnoreCommonCommand(MockLink* pLink)
		{
			pLink->AddIgnoreID(MC_ACTION_USE_TALENT);
			pLink->AddIgnoreID(MC_ACTION_TALENT_HIT);
			pLink->AddIgnoreID(MC_ACTION_TALENT_HIT_NO_MF);
			pLink->AddIgnoreID(MC_BUFF_GAIN);
			pLink->AddIgnoreID(MC_BUFF_LOST);
		}

		MockField*		m_pField;
		GEntityPlayer*	m_pPlayer;
		MockLink*		m_pLinkTester;

		GTestMgrWrapper<GFieldInfoMgr>				m_FieldInfoMgr;
		GTestMgrWrapper<GFieldMgr>					m_FieldMgr;
		GTestSysWrapper2<GSystem, GUnitTestUtil::FakeSystem>  m_FakeSystemWrapper;
		GTestSysWrapper2<GCombatCalculator, SimpleCombatCalculator>				m_CombatCalculator;
		GTestSysWrapper2<GAttackDamageCalculator, SimpleAttackDamageCalculator>	m_AttackDamageCalculator;
	};

	TEST_FIXTURE(FTalentFocus, Activate)
	{
		test.focus.GainItemForSneak(m_pPlayer);

		SetIgnoreCommonCommand(m_pLinkTester);
		BeginCommandRecord();

		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		test.focus.ToggleSneak(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_SNEAK));

		// ��Ŷ üũ
		const minet::MCommand& Command = m_pLinkTester->GetCommand(0);
		CHECK_EQUAL(Command.GetID(), MC_FOCUS_GAIN);
		if (Command.GetParameterCount() < 2)						 { CHECK(false); return; }
		MUID uidActor;
		TALENT_FOCUS_TYPE nFocusType;
		if (!Command.GetParameter(&uidActor,	0, MPT_UID))		{ CHECK(false); return; }
		if (!Command.GetParameter(&nFocusType,	1, MPT_INT))		{ CHECK(false); return; }
		CHECK_EQUAL(uidActor, m_pPlayer->GetUID());
		CHECK_EQUAL(nFocusType, TFT_SNEAK);
	}

	TEST_FIXTURE(FTalentFocus, Toggle)
	{
		test.focus.GainItemForSneak(m_pPlayer);

		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		test.focus.ToggleSneak(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_SNEAK));
		test.focus.ToggleSneak(m_pPlayer);
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		test.focus.ToggleSneak(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_SNEAK));
		test.focus.ToggleSneak(m_pPlayer);
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
	}

	TEST_FIXTURE(FTalentFocus, ToggleLevel)
	{
		test.focus.GainItemForSneak(m_pPlayer);

		GBuffInfo* pBuffLv1 = test.buff.NewBuffInfo(BUFF_FOCUS_ID[TFT_SNEAK]);	// ����1
		pBuffLv1->m_nStackPower = 101;
		GBuffInfo* pBuffLv2 = test.buff.NewBuffInfo(BUFF_FOCUS_ID[TFT_SNEAK]+1);	// ����2
		pBuffLv2->m_nStackPower = 102;

		GTalentInfo* pActiveTalentLv1 = test.talent.NewTalentInfo();
		test.talent.SetExtraActiveTalentInfo(pActiveTalentLv1, 1, TEAT_TOGGLE_SNEAK);
		pActiveTalentLv1->m_nExtraActiveParam1 = 1;
		GTalentInfo* pActiveTalentLv2 = test.talent.NewTalentInfo();
		test.talent.SetExtraActiveTalentInfo(pActiveTalentLv2, 2, TEAT_TOGGLE_SNEAK);
		pActiveTalentLv2->m_nExtraActiveParam1 = 2;

		// 1���� ���� ����
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		test.talent.UseTalent(m_pPlayer, pActiveTalentLv1, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_SNEAK));
		CHECK_EQUAL(true, m_pPlayer->HasBuff(BUFF_FOCUS_ID[TFT_SNEAK]));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(BUFF_FOCUS_ID[TFT_SNEAK]+1));
		test.talent.UseTalent(m_pPlayer, pActiveTalentLv1, m_pPlayer->GetUID());
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		// 2���� ���� ����
		test.talent.UseTalent(m_pPlayer, pActiveTalentLv2, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_SNEAK));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(BUFF_FOCUS_ID[TFT_SNEAK]));
		CHECK_EQUAL(true, m_pPlayer->HasBuff(BUFF_FOCUS_ID[TFT_SNEAK]+1));
		test.talent.UseTalent(m_pPlayer, pActiveTalentLv1, m_pPlayer->GetUID());
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
	}

	TEST_FIXTURE(FTalentFocus, Deactive)
	{
		test.focus.GainItemForSneak(m_pPlayer);

		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		test.focus.ToggleSneak(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_SNEAK));
		
		SetIgnoreCommonCommand(m_pLinkTester);
		BeginCommandRecord();

		test.focus.ToggleSneak(m_pPlayer);
		CHECK_EQUAL(false, m_pPlayer->HasFocus());

		// ��Ŷ üũ
		const minet::MCommand& Command = m_pLinkTester->GetCommand(0);
		CHECK_EQUAL(Command.GetID(), MC_FOCUS_LOST);
		if (Command.GetParameterCount() < 2)						 { CHECK(false); return; }
		MUID uidActor;
		TALENT_FOCUS_TYPE nFocusType;
		if (!Command.GetParameter(&uidActor,	0, MPT_UID))		{ CHECK(false); return; }
		if (!Command.GetParameter(&nFocusType,	1, MPT_INT))		{ CHECK(false); return; }
		CHECK_EQUAL(uidActor, m_pPlayer->GetUID());
		CHECK_EQUAL(nFocusType, TFT_SNEAK);
	}

	TEST_FIXTURE(FTalentFocus, UseSneakTalent)
	{
		test.focus.GainItemForSneak(m_pPlayer);

		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, vec3(100,100,0));
		GTalentInfo* pSneakTalent = test.talent.MakeDamageTalent(1);
		pSneakTalent->m_nRequireFocus = TFT_SNEAK;
		pSneakTalent->m_nNextFocus = TFT_NONE;
		GTalentInfo* pOtherTalent = test.talent.MakeDamageTalent(1);
		pOtherTalent->m_nRequireFocus = TFT_COUNTER;
		pOtherTalent->m_nNextFocus = TFT_NONE;

		test.focus.ToggleSneak(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_SNEAK));

		// �ٸ� ��Ŀ�� �ŷ�Ʈ�� �� �� ����.
		int nOldHP = pDefender->GetHP();
		test.talent.UseTalent(m_pPlayer, pOtherTalent, pDefender->GetUID());
		CHECK_EQUAL(nOldHP, pDefender->GetHP());	// �ŷ�Ʈ�� ������� ������

		// ��Ŀ���� �ʿ��� �ŷ�Ʈ�� ���
		nOldHP = pDefender->GetHP();
		test.talent.UseTalent(m_pPlayer, pSneakTalent, pDefender->GetUID());
		CHECK_EQUAL(nOldHP-1, pDefender->GetHP());	// �ŷ�Ʈ�� �������

		// NextFocus�� ��� ��Ŀ���� ����� (��ȸ��)
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
	}

	TEST_FIXTURE(FTalentFocus, WeaponCheck)
	{
		// ��Ŀ�� ���� �� ���� ���� �����ϰ� ��Ŀ�� ��� �õ�
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		test.item.EquipNewWeapon(m_pPlayer, WEAPON_1H_BLUNT);
		test.focus.ToggleSneak(m_pPlayer);
		CHECK_EQUAL(false, m_pPlayer->HasFocus(TFT_SNEAK));

		// ��Ŀ�� ���� �� �ִ� ����(Ȱ) �����ϰ� ��Ŀ�� ��� �õ�
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		test.item.EquipNewWeapon(m_pPlayer, WEAPON_ARCHERY);
		test.focus.ToggleSneak(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_SNEAK));

		// ��Ŀ�� ���� �� �ִ� ����(���) �����ϰ� ��Ŀ�� ��� �õ�
		test.focus.ClearFocus(m_pPlayer);
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		test.item.EquipNewWeapon(m_pPlayer, WEAPON_DUAL_WIELD);
		test.focus.ToggleSneak(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_SNEAK));

		// ��Ŀ�� ���� �� �ִ� ����(�Ǿ��) �����ϰ� ��Ŀ�� ��� �õ�
		test.focus.ClearFocus(m_pPlayer);
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		test.item.EquipNewWeapon(m_pPlayer, WEAPON_1H_PIERCE);
		test.focus.ToggleSneak(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_SNEAK));
	}
}