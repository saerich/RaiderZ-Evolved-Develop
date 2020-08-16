#include "StdAfx.h"
#include "GTestForward.h"
#include "GEntityPlayer.h"
#include "GModuleCombat.h"
#include "GServer.h"
#include "GFieldMgr.h"
#include "FBasePlayer.h"

SUITE(TalentFocus_Advent)
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
			pLink->AddIgnoreID(MC_QUESTPVP_TEAMCOUNT);
		}

		MockField*		m_pField;
		GEntityPlayer*	m_pPlayer;
		MockLink*		m_pLinkTester;

		GTestMgrWrapper<GFieldInfoMgr>				m_FieldInfoMgr;
		GTestMgrWrapper<GFieldMgr>					m_FieldMgr;
		GTestSysWrapper2<GSystem, GUnitTestUtil::FakeSystem>  m_FakeSystemWrapper;
		GTestSysWrapper2<GCombatCalculator, SimpleCombatCalculator>				m_CombatCalculator;
		GTestSysWrapper2<GAttackDamageCalculator, SimpleAttackDamageCalculator>	m_AttackDamageCalculator;
		DECLWRAPPER_TalentMgr;
	};

	TEST_FIXTURE(FTalentFocus, Activate)
	{
		test.focus.GainItemForAdvent(m_pPlayer);
		
		// Ŀ�ǵ� ��� ����
		BeginCommandRecord();

		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		test.focus.ToggleAdvent(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_ADVENT));
		
		// ��Ŷ üũ
		const minet::MCommand& Command = m_pLinkTester->GetCommand(0);
		CHECK_EQUAL(Command.GetID(), MC_FOCUS_GAIN);
		if (Command.GetParameterCount() < 2)						 { CHECK(false); return; }
		MUID uidActor;
		TALENT_FOCUS_TYPE nFocusType;
		if (!Command.GetParameter(&uidActor,	0, MPT_UID))		{ CHECK(false); return; }
		if (!Command.GetParameter(&nFocusType,	1, MPT_INT))		{ CHECK(false); return; }
		CHECK_EQUAL(uidActor, m_pPlayer->GetUID());
		CHECK_EQUAL(nFocusType, TFT_ADVENT);
	}

	// ��ȹ�� ��� �ȵǰ� �����Ǿ����ϴ�.
	//TEST_FIXTURE(FTalentFocus, Toggle)
	//{
	//	test.focus.GainItemForAdvent(m_pPlayer);

	//	CHECK_EQUAL(m_pPlayer->HasFocus(), false);
	//	test.focus.ToggleAdvent(m_pPlayer);
	//	CHECK_EQUAL(m_pPlayer->HasFocus(TFT_ADVENT), true);
	//	test.focus.ToggleAdvent(m_pPlayer);
	//	CHECK_EQUAL(m_pPlayer->HasFocus(), false);
	//	test.focus.ToggleAdvent(m_pPlayer);
	//	CHECK_EQUAL(m_pPlayer->HasFocus(TFT_ADVENT), true);
	//	test.focus.ToggleAdvent(m_pPlayer);
	//	CHECK_EQUAL(m_pPlayer->HasFocus(), false);
	//}

	TEST_FIXTURE(FTalentFocus, TickPay)
	{
		int nTickCost_STA = 5;
		test.focus.GainItemForAdvent(m_pPlayer);

		m_pPlayer->GetChrInfo()->nMaxSTA = 10000;
		m_pPlayer->SetSTA(nTickCost_STA*10);	// 10�ʰ� ��ƿ �� �ִ� ���¹̳�

		// ��Ŀ�� ���� �����
		GBuffInfo* pFocusBuff = test.buff.NewBuffInfo(BUFF_FOCUS_ID[TFT_ADVENT]);
		pFocusBuff->m_nSTACost = nTickCost_STA;

		// ��Ŀ�� ���
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		test.focus.ToggleAdvent(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_ADVENT));

		BeginCommandRecord();

		// ���¹̳� ���� �޶����� ���
		for (int i=0; i<10*2; i++)		// 10�� ��ƿ ���׹̳��� ������ �ְ�, ��ƽ(.5��)���� üũ�ϹǷ� 20��
		{
			CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_ADVENT));
			m_pField->Update(0.5f);
		}

		// ��Ŀ���� �Ҿ���
		m_pField->Update(1.0f);	// ���� ���ſ� �ʿ��� ƽ
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		CHECK_EQUAL(m_pPlayer->GetSTA(), 0);

		// ��Ŷ üũ
		const minet::MCommand& Command = m_pLinkTester->GetCommand(0);
		CHECK_EQUAL(Command.GetID(), MC_FOCUS_LOST);
		if (Command.GetParameterCount() < 2)						 { CHECK(false); return; }
		MUID uidActor;
		TALENT_FOCUS_TYPE nFocusType;
		if (!Command.GetParameter(&uidActor,	0, MPT_UID))		{ CHECK(false); return; }
		if (!Command.GetParameter(&nFocusType,	1, MPT_INT))		{ CHECK(false); return; }
		CHECK_EQUAL(uidActor, m_pPlayer->GetUID());
		CHECK_EQUAL(nFocusType, TFT_ADVENT);
	}

	TEST_FIXTURE(FTalentFocus, UseAdventTalent)
	{
		test.focus.GainItemForAdvent(m_pPlayer);

		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, vec3(100,100,0));
		GTalentInfo* pAdventTalent = test.talent.MakeDamageTalent(1);
		pAdventTalent->m_nRequireFocus = TFT_ADVENT;
		pAdventTalent->m_nNextFocus = TFT_NONE;
		GTalentInfo* pOtherTalent = test.talent.MakeDamageTalent(1);
		pOtherTalent->m_nRequireFocus = TFT_COUNTER;
		pOtherTalent->m_nNextFocus = TFT_NONE;

		test.focus.ToggleAdvent(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_ADVENT));

		// �ٸ� ��Ŀ�� �ŷ�Ʈ�� �� �� ����.
		int nOldHP = pDefender->GetHP();
		test.talent.UseTalent(m_pPlayer, pOtherTalent, pDefender->GetUID());
		CHECK_EQUAL(nOldHP, pDefender->GetHP());	// �ŷ�Ʈ�� ������� ������

		// ��Ŀ���� �ʿ��� �ŷ�Ʈ�� ���
		nOldHP = pDefender->GetHP();
		test.talent.UseTalent(m_pPlayer, pAdventTalent, pDefender->GetUID());
		CHECK_EQUAL(nOldHP-1, pDefender->GetHP());	// �ŷ�Ʈ�� �������

		// NextFocus�� ��� ��Ŀ���� ������� ����
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_ADVENT));
	}

	TEST_FIXTURE(FTalentFocus, WeaponCheck)
	{
		// ��Ŀ�� ���� �� ���� ���� �����ϰ� ��Ŀ�� ��� �õ�
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		test.item.EquipNewWeapon(m_pPlayer, WEAPON_1H_PIERCE);
		test.focus.ToggleAdvent(m_pPlayer);
		CHECK_EQUAL(false, m_pPlayer->HasFocus(TFT_ADVENT));
		
		// ��Ŀ�� ���� �� �ִ� ����(��Ʈ) �����ϰ� ��Ŀ�� ��� �õ�
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		test.item.EquipNewWeapon(m_pPlayer, WEAPON_1H_BLUNT);
		test.focus.ToggleAdvent(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_ADVENT));
	}

	TEST_FIXTURE(FTalentFocus, LostFocusByExpiredDurationTime)
	{
		// ��Ŀ�� ���
		test.focus.GainItemForAdvent(m_pPlayer);
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		test.focus.ToggleAdvent(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_ADVENT));

		BeginCommandRecord();

		// ���ӽð��� ����Ǿ� ��Ŀ���� ����
		m_pField->Update(GConst::FOCUS_ADVENT_DURATION_TIME);
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
		CHECK_EQUAL(nFocusType, TFT_ADVENT);
	}
}