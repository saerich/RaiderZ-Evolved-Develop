#include "StdAfx.h"
#include "GTestForward.h"
#include "GServer.h"
#include "GFieldMgr.h"
#include "FBasePlayer.h"
#include "GModuleBuff.h"

SUITE(Talent_Passive)
{
	struct FTalent : public FBasePlayer
	{
		FTalent()
		{
			m_pField = GUTHelper_Field::DefaultMockField();
			m_pPlayer = NewEntityPlayer(m_pField, vec3(100,100,0));
			m_pLinkTester = test.network.NewLink(m_pPlayer);
			swprintf_s(m_pPlayer->GetPlayerInfo()->szName, L"Tester");

			gsys.pServer->FrameCounterClear();
			SetIgnoreCommonCommand(m_pLinkTester);
		}

		~FTalent()
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

		DECLWRAPPER_FieldInfoMgr;
		DECLWRAPPER_FieldMgr;
		DECLWRAPPER_TalentMgr;
		DECLWRAPPER_SimpleCombatCalc;
	};

	TEST_FIXTURE(FTalent, ModEffect)
	{
		GTalentInfo* pPassiveTalent = test.talent.NewPassiveTalentInfo(1);
		pPassiveTalent->m_ActorModifier.nHPMax.nMod = 100;	
		pPassiveTalent->m_ActorModifier.nENMax.nMod = 100;	
		pPassiveTalent->m_ActorModifier.nENMax.fPer = 0.5f;	
		pPassiveTalent->m_ActorModifier.nSTAMax.nMod = 100;	

		pPassiveTalent->m_ActorModifier.nAP.nMod = 100;
		pPassiveTalent->m_ActorModifier.nAP.fPer = 0.5f;
		
		m_pPlayer->GetPlayerInfo()->nAP = 100;

		CHECK_EQUAL(m_pPlayer->GetMaxHP(), 800);
		CHECK_EQUAL(m_pPlayer->GetMaxEN(), 450);
		CHECK_EQUAL(m_pPlayer->GetMaxSTA(), 100);
		CHECK_EQUAL(m_pPlayer->GetAP(), 100);

		test.talent.Train(m_pPlayer, pPassiveTalent);
		CHECK_EQUAL(m_pPlayer->GetMaxHP(), 900);
		CHECK_EQUAL(m_pPlayer->GetMaxEN(), 825);
		CHECK_EQUAL(m_pPlayer->GetMaxSTA(), 200);
		CHECK_EQUAL(m_pPlayer->GetAP(), 300);

		test.talent.UntrainAll(m_pPlayer);
		CHECK_EQUAL(m_pPlayer->GetMaxHP(), 800);
		CHECK_EQUAL(m_pPlayer->GetMaxEN(), 450);
		CHECK_EQUAL(m_pPlayer->GetMaxSTA(), 100);
		CHECK_EQUAL(m_pPlayer->GetAP(), 100);
	}


	TEST_FIXTURE(FTalent, NoApplyInstantModByPassiveTalent)
	{
		GTalentInfo* pPassiveTalent = test.talent.NewPassiveTalentInfo(1);
		pPassiveTalent->m_InstantModifier.nHP.nMod = 100;	
		pPassiveTalent->m_InstantModifier.nEN.nMod = 100;	
		pPassiveTalent->m_InstantModifier.nEN.fPer = 0.5f;	
		pPassiveTalent->m_InstantModifier.nSTA.nMod = 100;	

		SetStatus(m_pPlayer, 100, 100, 100);

		CHECK_EQUAL(m_pPlayer->GetHP(), 100);
		CHECK_EQUAL(m_pPlayer->GetEN(), 100);
		CHECK_EQUAL(m_pPlayer->GetSTA(), 100);
		test.talent.Train(m_pPlayer, pPassiveTalent);
		CHECK_EQUAL(m_pPlayer->GetHP(), 100);
		CHECK_EQUAL(m_pPlayer->GetEN(), 100);
		CHECK_EQUAL(m_pPlayer->GetSTA(), 100);
		test.talent.UntrainAll(m_pPlayer);
		CHECK_EQUAL(m_pPlayer->GetHP(), 100);
		CHECK_EQUAL(m_pPlayer->GetEN(), 100);
		CHECK_EQUAL(m_pPlayer->GetSTA(), 100);
	}

	TEST_FIXTURE(FTalent, ExtraPassive)
	{
		CHECK_EQUAL(m_pPlayer->GetPassiveValue(TEPT_GUARD), 0);

		{
			GTalentInfo* pPassiveTalent = test.talent.NewExtraPassiveTalentInfo(TEPT_GUARD);
			pPassiveTalent->m_nTalentLine = 777;
			pPassiveTalent->m_nRank = 1;
			pPassiveTalent->m_nExtraPassiveParam = 1;
			test.talent.Train(m_pPlayer, pPassiveTalent);
		}
		
		CHECK_EQUAL(m_pPlayer->GetPassiveValue(TEPT_GUARD), 1);

		{
			GTalentInfo* pPassiveTalent = test.talent.NewExtraPassiveTalentInfo(TEPT_GUARD);
			pPassiveTalent->m_nTalentLine = 778;
			pPassiveTalent->m_nRank = 1;
			pPassiveTalent->m_nExtraPassiveParam = 1;
			test.talent.Train(m_pPlayer, pPassiveTalent);
		}

		CHECK_EQUAL(m_pPlayer->GetPassiveValue(TEPT_GUARD), 2);
	}

	//TEST_FIXTURE(FTalent, WeaponMastery)
	//{
	//	// ���� �ŷ�Ʈ�迭�� �ٸ� ��ũ�� ����
	//	GTalentInfo* pSlashMasteryRank1 = test.talent.NewExtraPassiveTalentInfo(TEPT_MASTERY_SLASH);
	//	pSlashMasteryRank1->m_nTalentLine = 777;
	//	pSlashMasteryRank1->m_nRank = 1;
	//	pSlashMasteryRank1->m_nExtraPassiveValue = 1;
	//	test.talent.LearnTalent(m_pPlayer, pSlashMasteryRank1);
	//	CHECK_EQUAL(m_pPlayer->GetWeaponMasteryRank(WEAPON_1H_SLASH), 1);

	//	GTalentInfo* pSlashMasteryRank3 = test.talent.NewExtraPassiveTalentInfo(TEPT_MASTERY_SLASH);
	//	pSlashMasteryRank3->m_nTalentLine = 777;
	//	pSlashMasteryRank3->m_nRank = 3;
	//	pSlashMasteryRank3->m_nExtraPassiveValue = 3;
	//	test.talent.LearnTalent(m_pPlayer, pSlashMasteryRank3);
	//	CHECK_EQUAL(m_pPlayer->GetWeaponMasteryRank(WEAPON_1H_SLASH), 3);	// �� ���� ȿ���� �ٲ�

	//	GTalentInfo* pSlashMasteryRank2 = test.talent.NewExtraPassiveTalentInfo(TEPT_MASTERY_SLASH);
	//	pSlashMasteryRank2->m_nTalentLine = 777;
	//	pSlashMasteryRank2->m_nRank = 2;
	//	pSlashMasteryRank2->m_nExtraPassiveValue = 2;
	//	test.talent.LearnTalent(m_pPlayer, pSlashMasteryRank2);
	//	CHECK_EQUAL(m_pPlayer->GetWeaponMasteryRank(WEAPON_1H_SLASH), 3);	// �������� ���� ȿ���� ����

	//	// �ٸ� �ŷ�Ʈ�迭�� ��ũ�� ����
	//	GTalentInfo* pSlashMasteryRankB = test.talent.NewExtraPassiveTalentInfo(TEPT_MASTERY_SLASH);
	//	pSlashMasteryRankB->m_nTalentLine = 123;
	//	pSlashMasteryRankB->m_nRank = 1;
	//	pSlashMasteryRankB->m_nExtraPassiveValue = 10;
	//	test.talent.LearnTalent(m_pPlayer, pSlashMasteryRankB);
	//	CHECK_EQUAL(m_pPlayer->GetWeaponMasteryRank(WEAPON_1H_SLASH), 3+10);	// ȿ�� ��ø
	//} 

	TEST_FIXTURE(FTalent, FocusBerserkDecreaseMaxBubble)
	{
		TVALID(GConst::FOCUS_BERSERK_HIT_COUNT >= 3);
		test.focus.GainItemForBerserk(m_pPlayer);
		test.focus.GainBerserk(m_pPlayer, true, GConst::FOCUS_BERSERK_HIT_COUNT-2);	// �ʿ� ������ 2�� ��ħ
		CHECK_EQUAL(false, m_pPlayer->HasFocus(TFT_BERSERK));
		CHECK_EQUAL(m_pPlayer->GetBerserkBubble(), GConst::FOCUS_BERSERK_HIT_COUNT-2);
		m_pField->Update(10000.0f);	// ���� ����
		CHECK_EQUAL(m_pPlayer->GetBerserkBubble(), 0);

		// �нú� �ŷ�Ʈ�� ���� ��õ�
		GTalentInfo* pPassive = test.talent.NewExtraPassiveTalentInfo(TEPT_FOCUS_BERSERK_DECREASE_MAXBUBBLE);
		pPassive->m_nExtraPassiveParam = 2;	// �ʿ� ���� 2�� �ٿ���
		test.talent.Train(m_pPlayer, pPassive);
		test.focus.GainBerserk(m_pPlayer, true, GConst::FOCUS_BERSERK_HIT_COUNT-2);	// �ʿ� ������ 2�� ��ħ
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_BERSERK));
	}

	TEST_FIXTURE(FTalent, FocusBerserkExtendTime)
	{
		test.focus.GainItemForBerserk(m_pPlayer);
		test.focus.GainBerserk(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_BERSERK));
		m_pField->Update(GConst::FOCUS_ENLIGHTEN_DURATION_TIME);	// ��Ŀ�� �ð� ����
		CHECK_EQUAL(false, m_pPlayer->HasFocus(TFT_BERSERK));

		// �нú� �ŷ�Ʈ�� ����Ŀ��� ���ӽð��� �þ��.
		GTalentInfo* pPassive = test.talent.NewExtraPassiveTalentInfo(TEPT_FOCUS_BERSERK_EXTENDTIME);
		pPassive->m_nExtraPassiveParam = 12;	// ���ӽð��� 12�ʸ�ŭ �þ
		test.talent.Train(m_pPlayer, pPassive);
		test.focus.GainBerserk(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_BERSERK));
		m_pField->Update(GConst::FOCUS_BERSERK_DURATION_TIME);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_BERSERK));
		m_pField->Update(11.0f);	// �������� 11�� �� ����
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_BERSERK));
		m_pField->Update(1.0f);	// 12���Ŀ� �Ҹ�
		CHECK_EQUAL(false, m_pPlayer->HasFocus(TFT_BERSERK));
	}

	TEST_FIXTURE(FTalent, FocusCounterExtendTime)
	{
		test.focus.GainItemForCounter(m_pPlayer);
		test.focus.GainCounter(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_COUNTER));
		m_pField->Update(GConst::FOCUS_COUNTER_DURATION_TIME);	// ��Ŀ�� �ð� ����
		CHECK_EQUAL(false, m_pPlayer->HasFocus(TFT_COUNTER));

		// �нú� �ŷ�Ʈ�� ����Ŀ��� ���ӽð��� �þ��.
		GTalentInfo* pPassive = test.talent.NewExtraPassiveTalentInfo(TEPT_FOCUS_COUNTER_EXTENDTIME);
		pPassive->m_nExtraPassiveParam = 12;	// ���ӽð��� 12�ʸ�ŭ �þ
		test.talent.Train(m_pPlayer, pPassive);
		test.focus.GainCounter(m_pPlayer);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_COUNTER));
		m_pField->Update(GConst::FOCUS_COUNTER_DURATION_TIME);
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_COUNTER));
		m_pField->Update(11.0f);	// �������� 11�� �� ����
		CHECK_EQUAL(true, m_pPlayer->HasFocus(TFT_COUNTER));
		m_pField->Update(1.0f);	// 12���Ŀ� �Ҹ�
		CHECK_EQUAL(false, m_pPlayer->HasFocus(TFT_COUNTER));
	}

	TEST_FIXTURE(FTalent, TalentShoutRadius)
	{
		int nBuffID = 123;
		const float DURATION_TIME = 10.0f;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		GTalentInfo* pBuffTalent= test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuffTalent->m_Buff1.EffectInfo.m_nRelation = CSEffectInfo::RELATION_ALL;
		pBuffTalent->m_Buff1.EffectInfo.m_fRadius  = 0.0f;
		pBuff->m_fDuration = DURATION_TIME;
		pBuffTalent->m_strSpecialization = TALENT_PROPERTY_SHOUT;	// ��ħ �Ӽ� �ŷ�Ʈ

		// ȿ�� ���� ���͵�
		GEntityPlayer* pPlayer1 = NewEntityPlayer(m_pField);
		pPlayer1->SetPos(vec3(300, 0, 0));
		GEntityPlayer* pPlayer2 = NewEntityPlayer(m_pField);
		pPlayer2->SetPos(vec3(0, 300, 0));
		
		// ���� ��ȿ�Ÿ��� 0�̶� �����ڸ� ȿ���� ����.
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(false, pPlayer1->HasBuff(nBuffID));
		CHECK_EQUAL(false, pPlayer2->HasBuff(nBuffID));
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(false, pPlayer1->HasBuff(nBuffID));
		CHECK_EQUAL(false, pPlayer2->HasBuff(nBuffID));

		// ���� ����
		m_pField->Update(DURATION_TIME);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(false, pPlayer1->HasBuff(nBuffID));
		CHECK_EQUAL(false, pPlayer2->HasBuff(nBuffID));

		// �Լ� ������ ���� ȿ�������� �þ��.
		GTalentInfo* pPassive = test.talent.NewExtraPassiveTalentInfo(TEPT_TALENT_SHOUT_RADIUS);
		pPassive->m_nExtraPassiveParam = 10;	// ��ġ�� ������ 10���� �þ
		test.talent.Train(m_pPlayer, pPassive);
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(true, pPlayer1->HasBuff(nBuffID));
		CHECK_EQUAL(true, pPlayer2->HasBuff(nBuffID));

	}

	TEST_FIXTURE(FTalent, TestPlayerPassiveRank)
	{
		GPlayerPassiveTalent	playerPassiveTalent;

		const int MAX_TALENTS = 5;

		GTalentInfo* pPassiveTalent[MAX_TALENTS];
		for (int i = 0; i < MAX_TALENTS; i++)
		{
			pPassiveTalent[i] = test.talent.NewExtraPassiveTalentInfo(TEPT_SPECIALIZATION_SLASH, i+1);
			pPassiveTalent[i]->m_nTalentLine = 1;
			pPassiveTalent[i]->m_nRank = i+1;
			pPassiveTalent[i]->m_nExtraPassiveParam = i+1;
		}

		CHECK_EQUAL(playerPassiveTalent.GetRank(TEPT_SPECIALIZATION_SLASH), 0);

		playerPassiveTalent.GainEffect(pPassiveTalent[0]);
		CHECK_EQUAL(playerPassiveTalent.GetRank(TEPT_SPECIALIZATION_SLASH), 1);

		// �׳� �ѹ� �� ���� �ŷ�Ʈ�� Gain�غ���.
		playerPassiveTalent.GainEffect(pPassiveTalent[0]);
		CHECK_EQUAL(playerPassiveTalent.GetRank(TEPT_SPECIALIZATION_SLASH), 1);

		playerPassiveTalent.GainEffect(pPassiveTalent[1]);
		CHECK_EQUAL(playerPassiveTalent.GetRank(TEPT_SPECIALIZATION_SLASH), 2);
	}

	TEST_FIXTURE(FTalent, GuardMastery)
	{
		test.item.EquipNewShield(m_pPlayer);

		bool bEquippedShield = true;
		int nGuardLevel = m_pPlayer->GetPassiveValue(TEPT_GUARD);
		CHECK_EQUAL(0, nGuardLevel);
		CHECK_EQUAL(101050, CSTalentInfoHelper::NormalGuardTalentID(WEAPON_1H_SLASH, nGuardLevel, bEquippedShield));

		int nTalentLine = SUnitTestUtil::NewID();

		// ��� ������ ���
		GTalentInfo* pPassive = test.talent.NewExtraPassiveTalentInfo(TEPT_GUARD, SUnitTestUtil::NewID(), nTalentLine);
		pPassive->m_nExtraPassiveParam = 3;	// ����ġ�� 3��ŭ �þ
		test.talent.Train(m_pPlayer, pPassive);

		nGuardLevel = m_pPlayer->GetPassiveValue(TEPT_GUARD);
		CHECK_EQUAL(3, nGuardLevel);
		CHECK_EQUAL(101053, CSTalentInfoHelper::NormalGuardTalentID(WEAPON_1H_SLASH, nGuardLevel, bEquippedShield));

		// ���а� ���� ��쿡�� �ŷ�Ʈ ���̵� 10 ���Ѵ�.
		bEquippedShield = false;
		CHECK_EQUAL(101063, CSTalentInfoHelper::NormalGuardTalentID(WEAPON_1H_SLASH, nGuardLevel, bEquippedShield));

		// ��� ������ ���
		pPassive = test.talent.NewExtraPassiveTalentInfo(TEPT_GUARD, SUnitTestUtil::NewID(), nTalentLine);
		pPassive->m_nExtraPassiveParam = 5;	// ����ġ�� 3��ŭ �þ
		test.talent.Train(m_pPlayer, pPassive);

		nGuardLevel = m_pPlayer->GetPassiveValue(TEPT_GUARD);
		CHECK_EQUAL(5, nGuardLevel);
		CHECK_EQUAL(101065, CSTalentInfoHelper::NormalGuardTalentID(WEAPON_1H_SLASH, nGuardLevel, bEquippedShield));
	} 

	TEST_FIXTURE(FTalent, SazahuSpecialization)
	{
		GEntityPlayer* pOtherPlayer = NewEntityPlayer(m_pField);
		int nBuffID = 1123;		// ���� ��å�������� �Ϲ� ����ID�� 1000�� �̻��̾���մϴ�.
		CHECK_EQUAL(false, pOtherPlayer->HasBuff(nBuffID));
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_nType = BUFT_BUFF;
		test.buff.GainBuffSelf(pOtherPlayer, pBuff);
		CHECK_EQUAL(true, pOtherPlayer->HasBuff(nBuffID));

		GTalentInfo* pSazahuTalent=NULL;
		pSazahuTalent = test.talent.NewTalentInfo();
		pSazahuTalent->m_strSpecialization = TALENT_PROPERTY_SAZAHU;	// ������ �Ӽ� �ŷ�Ʈ
		pSazahuTalent->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
		pSazahuTalent->m_EffectInfo.m_nRelation = CSEffectInfo::RELATION_ALL;

		// ������ �ŷ�Ʈ �Ẹ��
		test.talent.UseTalent(m_pPlayer, pSazahuTalent, pOtherPlayer->GetUID());
		pOtherPlayer->GetModuleBuff()->Update(0.1f); // ���� ���� ó�� ƽ
		CHECK_EQUAL(true, pOtherPlayer->HasBuff(nBuffID));	// ������ ������� ����

		// ������ Ưȭ ���
		GTalentInfo* pPassive = test.talent.NewExtraPassiveTalentInfo(TEPT_TALENT_SAZAHU_SPECIALIZATION);
		pPassive->m_nExtraPassiveParam = pPassive->m_nID;	// ������ Ưȭ �ŷ�Ʈ ID (�ڱ��ڽ�)
		pPassive->m_nExtraActive = TEAT_DISPEL_BUFF;	// ���� ����
		pPassive->m_nExtraActiveParam1 = 10;	// �����Ŀ� 10���� ����		
		test.talent.Train(m_pPlayer, pPassive);

		// ������ �ŷ�Ʈ �Ẹ��
		test.talent.UseTalent(m_pPlayer, pSazahuTalent, pOtherPlayer->GetUID());
		pOtherPlayer->GetModuleBuff()->Update(0.1f); // ���� ���� ó�� ƽ
		CHECK_EQUAL(false, pOtherPlayer->HasBuff(nBuffID));	// ������ �����
	}
}

