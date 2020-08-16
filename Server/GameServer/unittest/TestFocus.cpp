#include "StdAfx.h"
#include "GTestForward.h"
#include "GEntityPlayer.h"
#include "GModuleCombat.h"
#include "GServer.h"
#include "GFieldMgr.h"
#include "FBasePlayer.h"
#include "GModuleBuff.h"

SUITE(TalentFocus)
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
		}

		MockField*		m_pField;
		GEntityPlayer*	m_pPlayer;
		MockLink*		m_pLinkTester;

		DECLWRAPPER_Field;
		DECLWRAPPER_TalentMgr;
		DECLWRAPPER_SimpleCombatCalc;
		DECLWRAPPER_FakeSystem;
	};

	TEST_FIXTURE(FTalentFocus, NoFocusInStart)
	{
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
	}

	TEST_FIXTURE(FTalentFocus, DoNotGainNPC)
	{	
		DECLWRAPPER_NPCInfoMgr;
		GEntityNPC* pTestNPC = GUTHelper_NPC::SpawnNPC(m_pField);
		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, vec3(100,100,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		// ���
		int nGuardTalentID = 100;
		test.talent.NewGuardTalentInfo(nGuardTalentID);
		pTestNPC->doGuard(nGuardTalentID);

		// ����
		test.talent.UseTalent(pAttacker, p1DmgTalent, pTestNPC->GetUID());

		// �ݰ� ��Ŀ���� ���� ����
		GModuleCombat* pmCombat =  pTestNPC->GetModuleCombat();
		CHECK_EQUAL(false, pmCombat->HasFocus());
	}

	TEST_FIXTURE(FTalentFocus, UseTalentRequriedFocus)
	{
		// �ݰ� ��Ŀ���� Ȱ��ȭ�� �� �ִ� ���⸦ ����
		test.item.EquipNewWeapon(m_pPlayer, WEAPON_1H_BLUNT);

		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, vec3(100,100,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		// �ݰ� ��Ŀ���� ����
		test.focus.GainCounter(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_COUNTER));

		// �ݰ� ��Ŀ���� �ʿ��� �ŷ�Ʈ�� ���
		GTalentInfo* pTalentRequriedCounterFocus = test.talent.MakeDamageTalent(10);
		pTalentRequriedCounterFocus->m_nRequireFocus = TFT_COUNTER;	// �ݰ� ��Ŀ���� �ʿ��ϴ�
		CHECK_EQUAL(pAttacker->GetHP(), 300);
		test.talent.UseTalent(m_pPlayer, pTalentRequriedCounterFocus, pAttacker->GetUID());
		CHECK_EQUAL(pAttacker->GetHP(), 290);	// ����10�� �Ծ���.

		// ���� ��Ŀ���� �ʿ��� �ŷ�Ʈ�� ���
		GTalentInfo* pTalentRequriedBerserkFocus = test.talent.MakeDamageTalent(10);
		pTalentRequriedBerserkFocus->m_nRequireFocus = TFT_BERSERK;	// ���� ��Ŀ���� �ʿ��ϴ�
		CHECK_EQUAL(pAttacker->GetHP(), 290);
		test.talent.UseTalent(m_pPlayer, pTalentRequriedBerserkFocus, pAttacker->GetUID());
		CHECK_EQUAL(pAttacker->GetHP(), 290);	// ���ظ� ���� �ʾҴ�.
	}

	TEST_FIXTURE(FTalentFocus, GainFocusAfterUseTalent)
	{
		test.focus.LearnLicense_Counter(m_pPlayer);
		test.buff.NewBuffInfo(BUFF_FOCUS_ID[TFT_COUNTER]);

		// �ݰ� ��Ŀ���� Ȱ��ȭ�� �� �ִ� ���⸦ ����
		test.item.EquipNewWeapon(m_pPlayer, WEAPON_1H_BLUNT);

		GModuleCombat* pmCombat =  m_pPlayer->GetModuleCombat();
		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, vec3(100,100,0));

		//  ��Ŀ���� ��� �ŷ�Ʈ�� ���
		GTalentInfo* pTalentGainCounterFocus = test.talent.MakeDamageTalent(10);
		pTalentGainCounterFocus->m_nTiming = TC_FINISH_TALENT;
		pTalentGainCounterFocus->m_nNextFocus = TFT_COUNTER;	// �ݰ� ��Ŀ���� ��´�.

		// �ݰ� ��Ŀ���� ����
		CHECK_EQUAL(false, pmCombat->HasFocus());
		m_pPlayer->GetModuleCombat()->UseTalent(pTalentGainCounterFocus, pDefender->GetUID(), true);
		m_pField->Update(0.0f);	// �ŷ�Ʈ�� ���� ���� ƽ
		CHECK_EQUAL(true, pmCombat->HasFocus(TFT_COUNTER));
	}

	TEST_FIXTURE(FTalentFocus, AnytimeUseTalentWithNoRequrieFocus)
	{
		// �ݰ� ��Ŀ���� Ȱ��ȭ�� �� �ִ� ���⸦ ����
		test.item.EquipNewWeapon(m_pPlayer, WEAPON_1H_BLUNT);

		GModuleCombat* pmCombat =  m_pPlayer->GetModuleCombat();
		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, vec3(100,100,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		// ��Ŀ���� ������ ���� ����
		CHECK_EQUAL(false, pmCombat->HasFocus());
		CHECK_EQUAL(pDefender->GetHP(), 300);
		test.talent.UseTalent(m_pPlayer, p1DmgTalent, pDefender->GetUID());
		CHECK_EQUAL(pDefender->GetHP(), 299);

		// �ݰ� ��Ŀ���� ����
		test.focus.GainCounter(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_COUNTER));

		// �ݰ� ��Ŀ���� �־ ��Ŀ���� �ʿ���� �ŷ�Ʈ�� ��� ����
		CHECK_EQUAL(true, pmCombat->HasFocus(TFT_COUNTER));
		CHECK_EQUAL(pDefender->GetHP(), 299);
		test.talent.UseTalent(m_pPlayer, p1DmgTalent, pDefender->GetUID());
		CHECK_EQUAL(pDefender->GetHP(), 298);
	}

	TEST_FIXTURE(FTalentFocus, ResetFocusWhenDie)
	{
		// �ݰ� ��Ŀ���� Ȱ��ȭ�� �� �ִ� ���⸦ ����
		test.item.EquipNewWeapon(m_pPlayer, WEAPON_1H_BLUNT);

		// �ݰ� ��Ŀ���� ����
		test.focus.GainCounter(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_COUNTER));

		// ������ ��Ŀ���� �Ҵ´�.
		m_pPlayer->doDie();
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
	}

	TEST_FIXTURE(FTalentFocus, NotLostFocusAfterUseNoFocusTalent)
	{
		// �ݰ� ��Ŀ���� Ȱ��ȭ�� �� �ִ� ���⸦ ����
		test.item.EquipNewWeapon(m_pPlayer, WEAPON_1H_BLUNT);

		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, vec3(100,100,0));
		GTalentInfo* pCounterTalent = test.talent.MakeDamageTalent(1);

		// �ݰ� ��Ŀ���� ����
		test.focus.GainCounter(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_COUNTER));

		// �ݰ� ��Ŀ���� �����Ŀ� ������Ŀ���� ���� �ŷ�Ʈ�� �����
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_COUNTER));
		test.talent.UseTalent(m_pPlayer, pCounterTalent, pDefender->GetUID());

		// ������Ŀ�� �Ӽ��� ���� �ŷ�Ʈ�� �ᵵ ��Ŀ���� ���� �ʴ´�.
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_COUNTER));
	}

	TEST_FIXTURE(FTalentFocus, GainMultiFocus)
	{
		static const float TICK_TIME = 0.1f;

		// ��Ŀ���� Ȱ��ȭ�� �� �ִ� ���⸦ ����
		test.item.EquipNewWeapon(m_pPlayer, WEAPON_ARCHERY);

		// ��Ŀ�� �ƹ��͵� ����
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		CHECK_EQUAL(false, m_pPlayer->HasFocus(TFT_SNEAK));
		CHECK_EQUAL(false, m_pPlayer->HasFocus(TFT_PRECISION));

		// �ΰ��� ��Ŀ���� ����.
		test.focus.ToggleSneak(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_SNEAK));
		CHECK_EQUAL(false, m_pPlayer->HasFocus(TFT_PRECISION));
		test.focus.TogglePrecision(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_SNEAK));
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_PRECISION));

		// �ϳ��� ������ ����.
		test.focus.ToggleSneak(m_pPlayer);
		m_pPlayer->GetModuleBuff()->Update(TICK_TIME);
		CHECK_EQUAL(false, m_pPlayer->HasFocus(TFT_SNEAK));
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_PRECISION));
		test.focus.TogglePrecision(m_pPlayer);
		m_pPlayer->GetModuleBuff()->Update(TICK_TIME);
		CHECK_EQUAL(false, m_pPlayer->HasFocus(TFT_SNEAK));
		CHECK_EQUAL(false, m_pPlayer->HasFocus(TFT_PRECISION));

		// �ٽ� �ɾ��.
		test.focus.ToggleSneak(m_pPlayer);
		m_pPlayer->GetModuleBuff()->Update(TICK_TIME);		
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_SNEAK));
		CHECK_EQUAL(false, m_pPlayer->HasFocus(TFT_PRECISION));
		test.focus.TogglePrecision(m_pPlayer);
		m_pPlayer->GetModuleBuff()->Update(TICK_TIME);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_SNEAK));
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_PRECISION));
	}

	TEST_FIXTURE(FTalentFocus, PlayerRefCount)
	{
		CHECK_EQUAL(m_pField->GetPlayerQty(), 1);
		CHECK_EQUAL(TFT_SIZE+1, m_pPlayer->GetReferenceCount());

		// ĳ���� ����
		DeleteEntityPlayer(m_pPlayer);
		CHECK_EQUAL(0, m_pPlayer->GetReferenceCount());	// EntityPlayer ��ü�� ī���͸� ����
		CHECK_EQUAL(m_pField->GetPlayerQty(), 0);
	}

	TEST_FIXTURE(FTalentFocus, GainFocusOnUseTalent)
	{
		test.buff.NewBuffInfo(BUFF_FOCUS_ID[TFT_ADVENT]);
		test.focus.GainItemForAdvent(m_pPlayer);

		GTalentInfo* pAdventTalent = test.talent.MakeDamageTalent(1);
		pAdventTalent->m_nTiming = TC_USE_TALENT;	// �ŷ�Ʈ ���� ��Ŀ���� �ɸ��� �ϱ� ����
		pAdventTalent->m_nRequireFocus = TFT_NONE;
		pAdventTalent->m_nNextFocus = TFT_ADVENT;		

		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, vec3(100,100,0));
		test.talent.UseTalent(m_pPlayer, pAdventTalent, pDefender->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_ADVENT));
	}

	TEST_FIXTURE(FTalentFocus, GainFocusOnHitTalent)
	{
		test.buff.NewBuffInfo(BUFF_FOCUS_ID[TFT_ADVENT]);
		test.focus.GainItemForAdvent(m_pPlayer);

		GTalentInfo* pAdventTalent = test.talent.MakeDamageTalent(1);
		pAdventTalent->m_nTiming = TC_HIT_TALENT;	// ������ �����Ҷ� ��Ŀ���� �ɸ��� �ϱ� ����
		pAdventTalent->m_nRequireFocus = TFT_NONE;
		pAdventTalent->m_nNextFocus = TFT_ADVENT;		

		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, vec3(100,100,0));
		test.talent.UseTalent(m_pPlayer, pAdventTalent, pDefender->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_ADVENT));
	}

	TEST_FIXTURE(FTalentFocus, ResetDruationTimeWhenUseFocusTalent)
	{
		test.focus.LearnLicense_Berserk(m_pPlayer);
		test.focus.GainItemForBerserk(m_pPlayer);
		CHECK_EQUAL(false, m_pPlayer->HasFocus());
		test.focus.GainBerserk(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_BERSERK));

		GTalentInfo* pFocusTalent = test.talent.MakeDamageTalent(1);
		pFocusTalent->m_nRequireFocus = TFT_BERSERK;
		pFocusTalent->m_nNextFocus = TFT_BERSERK;

		m_pField->Update(GConst::FOCUS_BERSERK_DURATION_TIME/2);

		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, vec3(100,100,0));
		test.talent.UseTalent(m_pPlayer, pFocusTalent, pDefender->GetUID());

		// ������Ŀ���� �̾������Ƿ� ��Ŀ�� �����ð��� �ʱ�ȭ��
		m_pField->Update(GConst::FOCUS_BERSERK_DURATION_TIME/2);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_BERSERK));

		// ���ӽð��� ����Ǿ� ���� ��Ŀ���� ����
		m_pField->Update(GConst::FOCUS_BERSERK_DURATION_TIME/2);
		CHECK_EQUAL(false, m_pPlayer->HasFocus(TFT_BERSERK));
	}

	TEST_FIXTURE(FTalentFocus, LostFocusWhenChangeEquipIncorrectItem)
	{
		test.focus.LearnLicense_Counter(m_pPlayer);
		test.buff.NewBuffInfo(BUFF_FOCUS_ID[TFT_COUNTER]);

		// �ݰ� ��Ŀ���� Ȱ��ȭ�� �� �ִ� ���⸦ ����
		test.item.EquipNewWeapon(m_pPlayer, WEAPON_1H_BLUNT);

		GModuleCombat* pmCombat =  m_pPlayer->GetModuleCombat();
		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, vec3(100,100,0));

		//  ��Ŀ���� ��� �ŷ�Ʈ�� ���
		GTalentInfo* pTalentGainCounterFocus = test.talent.MakeDamageTalent(10);
		pTalentGainCounterFocus->m_nTiming = TC_USE_TALENT;
		pTalentGainCounterFocus->m_nNextFocus = TFT_COUNTER;	// �ݰ� ��Ŀ���� ��´�.

		// �ݰ� ��Ŀ���� ����
		CHECK_EQUAL(false, pmCombat->HasFocus());
		m_pPlayer->GetModuleCombat()->UseTalent(pTalentGainCounterFocus, pDefender->GetUID(), true);
		m_pField->Update(0.0f);	// �ŷ�Ʈ�� ���� ���� ƽ
		CHECK_EQUAL(true, pmCombat->HasFocus(TFT_COUNTER));

		// �ݰ� ��Ŀ���� Ȱ��ȭ�� �� ���� ���� ����
		test.item.EquipNewWeapon(m_pPlayer, WEAPON_STAFF);
		CHECK_EQUAL(false, pmCombat->HasFocus(TFT_COUNTER));
	}
}
