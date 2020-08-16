#include "stdafx.h"
#include "GFieldInfoMgr.h"
#include "GTestForward.h"
#include "GModuleBuff.h"
#include "GServer.h"
#include "GFieldMgr.h"
#include "FBasePlayer.h"
#include "GModuleCombat.h"

SUITE(Buff_Effect)
{
	struct FBuff : public FBasePlayer
	{
		FBuff()
		{
			m_pField = GUTHelper_Field::DefaultMockField();
			m_pPlayer = NewEntityPlayer(m_pField, vec3(100,100,0));
			m_pLinkTester = test.network.NewLink(m_pPlayer);
			swprintf_s(m_pPlayer->GetPlayerInfo()->szName, L"Tester");

			gsys.pServer->FrameCounterClear();
			SetIgnoreCommonCommand(m_pLinkTester);
		}

		~FBuff()
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

		void GradualyLostBuff(int nBuffID)
		{
			for (int i=0; i<5; i++)
			{
				// �ֱ��� ȿ��
				CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
				m_pField->Update(2.0f);
			}
		}

		GTalentInfo* CreateTestTalent_BuffPassiveExtra(int nBuffID, float fDurationTime, BUFF_PASSIVE_EXTRA_ATTRIB_ eAttrib)
		{
			GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
			pBuff->m_nPassiveExtraAttrib = eAttrib;
			pBuff->m_fDuration = fDurationTime;
			GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
			return pBuffTalent;
		}



		MockField*		m_pField;
		GEntityPlayer*	m_pPlayer;
		MockLink*		m_pLinkTester;

		DECLWRAPPER_FieldInfoMgr;
		DECLWRAPPER_FieldMgr;
		DECLWRAPPER_SimpleCombatCalc;
		DECLWRAPPER_BuffMgr;
	};

	TEST_FIXTURE(FBuff, OnStatus)
	{
		DECLWRAPPER_MissCombatCalc;

		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nEffectConditionType = TC_MISS_ME;			// �ڽ��� �¾�����
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;	// �ڽſ���
		// ����� �Ӽ�
		pBuff->m_InstantModifier.nHP.nMod = 10;	
		pBuff->m_InstantModifier.nHP.fPer = 2.0f;	
		pBuff->m_InstantModifier.nEN.nMod = 20;	
		pBuff->m_InstantModifier.nEN.fPer = 3.0f;	
		pBuff->m_InstantModifier.nSTA.nMod = 10;
		pBuff->m_InstantModifier.nSTA.fPer = 2.0f;

		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// ���� �̺�Ʈ
		m_pPlayer->SetHP(10);
		m_pPlayer->SetEN(10);
		m_pPlayer->SetSTA(10);
		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, m_pPlayer->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(m_pPlayer->GetHP(), (10+10)*(1.0f+2.0f));
		CHECK_EQUAL(m_pPlayer->GetEN(), (10+20)*(1.0f+3.0f));
		CHECK_EQUAL(m_pPlayer->GetSTA(), (10+10)*(1.0f+2.0f));
	}

	TEST_FIXTURE(FBuff, OnHeal)
	{
		DECLWRAPPER_MissCombatCalc;

		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nEffectConditionType = TC_MISS_ME;			// �ڽ��� �¾�����
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;	// �ڽſ���
		// ����� �Ӽ�
		pBuff->m_nMinHeal = 10;
		pBuff->m_nMaxHeal = 10;

		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// ���� �̺�Ʈ
		m_pPlayer->SetHP(10);
		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, m_pPlayer->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(m_pPlayer->GetHP(), 10+10);
	}

	TEST_FIXTURE(FBuff, OnHealPecent)
	{
		DECLWRAPPER_MissCombatCalc;

		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nEffectConditionType = TC_MISS_ME;			// �ڽ��� �¾�����
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;	// �ڽſ���

		// ����� �Ӽ�
		pBuff->m_fPercentageHeal = 0.5f;

		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// ���� �̺�Ʈ
		m_pPlayer->GetChrInfo()->nMaxHP = 800;
		m_pPlayer->SetHP(10);
		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, m_pPlayer->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(m_pPlayer->GetHP(), 10+400);
	}

	TEST_FIXTURE(FBuff, OnStatus_WhenGain)
	{
		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_GAIN;	// ������ ������
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;	// �ڽſ���
		// ����� �Ӽ�
		pBuff->m_InstantModifier.nHP.nMod = 10;	
		pBuff->m_InstantModifier.nHP.fPer = 2.0f;	
		pBuff->m_InstantModifier.nEN.nMod = 20;	
		pBuff->m_InstantModifier.nEN.fPer = 3.0f;	
		pBuff->m_InstantModifier.nSTA.nMod = 10;
		pBuff->m_InstantModifier.nSTA.fPer = 2.0f;

		m_pPlayer->SetHP(10);
		m_pPlayer->SetEN(10);
		m_pPlayer->SetSTA(10);

		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetHP(), (10+10)*(1.0f+2.0f));
		CHECK_EQUAL(m_pPlayer->GetEN(), (10+20)*(1.0f+3.0f));
		CHECK_EQUAL(m_pPlayer->GetSTA(), (10+10)*(1.0f+2.0f));
	}

	TEST_FIXTURE(FBuff, OnStatus_WhenLost)
	{
		int nBuffID = 123;
		float fDuration = 10.0f;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_GONE;	// ������ ������
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;	// �ڽſ���
		// ����� �Ӽ�
		pBuff->m_InstantModifier.nHP.nMod = 10;	
		pBuff->m_InstantModifier.nHP.fPer = 2.0f;	
		pBuff->m_InstantModifier.nEN.nMod = 20;	
		pBuff->m_InstantModifier.nEN.fPer = 3.0f;	
		pBuff->m_InstantModifier.nSTA.nMod = 10;
		pBuff->m_InstantModifier.nSTA.fPer = 2.0f;

		m_pPlayer->SetHP(10);
		m_pPlayer->SetEN(10);
		m_pPlayer->SetSTA(10);

		test.buff.GainBuffSelf(m_pPlayer, pBuff, fDuration);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetHP(), 10);
		CHECK_EQUAL(m_pPlayer->GetEN(), 10);
		CHECK_EQUAL(m_pPlayer->GetSTA(), 10);
		m_pField->Update(fDuration);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetHP(), (10+10)*(1.0f+2.0f));
		CHECK_EQUAL(m_pPlayer->GetEN(), (10+20)*(1.0f+3.0f));
		CHECK_EQUAL(m_pPlayer->GetSTA(), (10+10)*(1.0f+2.0f));
	}

	TEST_FIXTURE(FBuff, OnMaxStatus)
	{
		int nBuffID = 123;
		float fDuration = 10.0f;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_fPeriod = 2.0f;
		pBuff->m_fDuration = fDuration;
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_GAIN;
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;	// �ڽſ���
		// ����� �Ӽ�
		pBuff->m_ActorModifier.nHPMax.nMod = 10;	
		pBuff->m_ActorModifier.nHPMax.fPer = 2.0f;	
		pBuff->m_ActorModifier.nENMax.nMod = 20;	
		pBuff->m_ActorModifier.nENMax.fPer = 3.0f;	
		pBuff->m_ActorModifier.nSTAMax.nMod = 30;
		pBuff->m_ActorModifier.nSTAMax.fPer = 4.0f;
		
		GTalentInfo* pBuffTalent= test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
				
		int nOldMaxHP = m_pPlayer->GetMaxHP();
		int nOldMaxEN = m_pPlayer->GetMaxEN();
		int nOldMaxSTA = m_pPlayer->GetMaxSTA();

		// ���� ����
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// ���� ����
		GradualyLostBuff(nBuffID);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetMaxHP(), nOldMaxHP);
		CHECK_EQUAL(m_pPlayer->GetMaxEN(), nOldMaxEN);
		CHECK_EQUAL(m_pPlayer->GetMaxSTA(), nOldMaxSTA);
	}

	TEST_FIXTURE(FBuff, OnAbiliity)
	{
		int nBuffID = 123;
		float fDuration = 10.0f;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_fPeriod = 2.0f;
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_GAIN;
		// ����� �Ӽ�
		pBuff->m_PlayerModifier.nSTR.nMod = 10;	
		pBuff->m_PlayerModifier.nSTR.fPer = 2.0f;	
		pBuff->m_PlayerModifier.nDEX.nMod = 20;	
		pBuff->m_PlayerModifier.nDEX.fPer = 3.0f;	
		pBuff->m_PlayerModifier.nINT.nMod = 30;
		pBuff->m_PlayerModifier.nINT.fPer = 4.0f;
		pBuff->m_PlayerModifier.nCHA.nMod = 10;
		pBuff->m_PlayerModifier.nCHA.fPer = 2.0f;
		pBuff->m_PlayerModifier.nCON.nMod = 20;
		pBuff->m_PlayerModifier.nCON.fPer = 3.0f;
		GTalentInfo* pBuffTalent= test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_fDuration = fDuration;

		// ���� ����
		m_pPlayer->GetPlayerInfo()->nLevel = 1;
		m_pPlayer->GetPlayerInfo()->nSTR = 10;
		m_pPlayer->GetPlayerInfo()->nDEX = 10;
		m_pPlayer->GetPlayerInfo()->nINT = 10;
		m_pPlayer->GetPlayerInfo()->nCHA = 10;
		m_pPlayer->GetPlayerInfo()->nCON = 10;
		// ���� ����
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetSTR(), (10+10)*(1.0f+2.0f));
		CHECK_EQUAL(m_pPlayer->GetDEX(), (10+20)*(1.0f+3.0f));
		CHECK_EQUAL(m_pPlayer->GetINT(), (10+30)*(1.0f+4.0f));
		CHECK_EQUAL(m_pPlayer->GetCHA(), (10+10)*(1.0f+2.0f));
		CHECK_EQUAL(m_pPlayer->GetCON(), (10+20)*(1.0f+3.0f));

		// ���� ����
		GradualyLostBuff(nBuffID);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetSTR(), 10);
		CHECK_EQUAL(m_pPlayer->GetDEX(), 10);
		CHECK_EQUAL(m_pPlayer->GetINT(), 10);
		CHECK_EQUAL(m_pPlayer->GetCHA(), 10);
		CHECK_EQUAL(m_pPlayer->GetCON(), 10);
	}

	TEST_FIXTURE(FBuff, OnMoveSpeed)
	{
		int nBuffID = 123;
		float fDuration = 10.0f;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		// ����� �Ӽ�
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_GAIN;
		pBuff->m_ActorModifier.fMoveSpeed = 2.0f;	
		GTalentInfo* pBuffTalent= test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_fDuration = fDuration;
		pBuff->m_fPeriod = 2.0f;

		// ���� ����
		float fSpeed = (float)MoveSpeedByWeapon[WEAPON_NONE];
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(fSpeed*(1.0f+2.0f), m_pPlayer->GetWalkSpeed());

		// ���� ����
		GradualyLostBuff(nBuffID);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(fSpeed, m_pPlayer->GetWalkSpeed());
	}

	TEST_FIXTURE(FBuff, OnAP)
	{
		int nBuffID = 123;
		float fDuration = 10.0f;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		// ����� �Ӽ�
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_GAIN;
		pBuff->m_ActorModifier.nAP.nMod = 10;
		pBuff->m_ActorModifier.nAP.fPer = 2.0f;
		GTalentInfo* pBuffTalent= test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_fDuration = fDuration;
		pBuff->m_fPeriod = 2.0f;

		// ���� ����
		m_pPlayer->GetChrInfo()->nAP = 10;
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetAP(), (10+10)*(1.0f+2.0f));

		// ���� ����
		GradualyLostBuff(nBuffID);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetAP(), 10);
	}

	TEST_FIXTURE(FBuff, OnABS)
	{
		int nBuffID = 123;
		float fDuration = 10.0f;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		// ����� �Ӽ�
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_GAIN;
		pBuff->m_ActorModifier.nABS.nMod = 10;
		pBuff->m_ActorModifier.nABS.fPer = 2.0f;
		GTalentInfo* pBuffTalent= test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_fDuration = fDuration;
		pBuff->m_fPeriod = 2.0f;

		// ���� ����
		m_pPlayer->GetChrInfo()->nABS = 10;
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetABS(), (10+10)*(1.0f+2.0f));

		// ���� ����
		GradualyLostBuff(nBuffID);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetABS(), 10);
	}

	TEST_FIXTURE(FBuff, CounterAttack)
	{
		int nBuffID = 123;
		float fDuration = 10.0f;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		// ����� �Ӽ�
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_GAIN;
		pBuff->m_ActorModifier.nABS.nMod = 10;
		GTalentInfo* pBuffTalent= test.talent.MakeDamageTalent(10);
		test.talent.SetMeleeTalentInfo(pBuffTalent, pBuffTalent->m_nID);
		pBuffTalent->m_Buff1.nID = nBuffID;
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuffTalent->m_Buff1.EffectInfo.m_nRelation = CSEffectInfo::RELATION_ALL;
		pBuffTalent->m_Buff1.nEnchantEvent = TC_COUNTER;
		pBuff->m_fDuration = fDuration;
		pBuffTalent->m_HitInfo.m_vSegments.clear();	// ���� �ǰ����� ����
		test.talent.AddSimpleTalentInfoHitInfo(pBuffTalent, 10);	// 10���Ŀ� �ǰ�
		test.talent.AddSimpleTalentInfoHitInfo(pBuffTalent, 11);	// 11���Ŀ� �ǰ� (�ŷ�Ʈ�� 1�� �� ������Ű�� ���� ���, �ŷ�Ʈ�� ����� �Ŀ� �ǰ��� �ǰ� ����)
		pBuffTalent->m_bUseInvincibleTime = true;	// ���� ��� ����
		pBuffTalent->m_fInvincibleTimeStart = 0.0f;	// ���� ���� �ð�
		pBuffTalent->m_fInvincibleTimeEnd = 10.0f;	// ���� ���� �ð�
		pBuffTalent->Cooking();

		// ���� ����
		m_pPlayer->GetChrInfo()->nABS = 10;
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));	// �ݰ����� ���� ���� �ɸ��� ����
		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, m_pPlayer->GetPos()+vec3(10,10,0));
		test.duel.NewMockDuel_Fighting(m_pPlayer, pAttacker); // ������� ����
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		test.talent.UseTalent(m_pPlayer, pBuffTalent, pAttacker->GetUID());
		CHECK_EQUAL(true, m_pPlayer->IsNowInvincibility());
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pPlayer->GetUID());
		m_pPlayer->GetModuleCombat()->Update(10.0f); // ������� �ŷ�Ʈ�� �ǰݵɸ��� �ð��� ����
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));	// �ݰ����� ���� ���� �ɸ�
		CHECK_EQUAL(m_pPlayer->GetABS(), 20);
	}

	TEST_FIXTURE(FBuff, RestorePeriodActorModifierEffect)
	{
		int nBuffID = 123;
		float fDuration = 10.0f;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_fPeriod = 2.0f;
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_GAIN_N_PERIOD;
		// ����� �Ӽ�
		pBuff->m_PlayerModifier.nSTR.nMod = 10;	
		pBuff->m_PlayerModifier.nSTR.fPer = 2.0f;	
		GTalentInfo* pBuffTalent= test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_fDuration = fDuration;

		// ���� ����
		m_pPlayer->GetPlayerInfo()->nLevel = 1;
		m_pPlayer->GetPlayerInfo()->nSTR = 10;
		// ���� ����
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetSTR(), 60);

		// �ֱ� ȿ��
		m_pField->Update(2.0f);	
		CHECK_EQUAL(m_pPlayer->GetSTR(), 150);

		// �ֱ� ȿ��
		m_pField->Update(2.0f);	
		CHECK_EQUAL(m_pPlayer->GetSTR(), 280);

		// ���� ����
		m_pField->Update(fDuration);	
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetSTR(), 10);
	}

	TEST_FIXTURE(FBuff, RestorePeriodActorModifierEffectTwice)
	{
		int nBuffID = 123;
		float fDuration = 10.0f;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_fPeriod = 2.0f;
		pBuff->m_fDuration = fDuration;
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_GAIN_N_PERIOD;
		pBuff->m_PlayerModifier.nSTR.nMod = 10;	
		pBuff->m_PlayerModifier.nSTR.fPer = 2.0f;	
		GTalentInfo* pBuffTalent= test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;

		int nBuffID2 = 234;
		GBuffInfo* pBuff2=test.buff.NewBuffInfo(nBuffID2);
		pBuff2->m_fPeriod = 2.0f;
		pBuff2->m_fDuration = fDuration;
		pBuff2->m_Condition.nEffectConditionType = TC_BUFF_GAIN_N_PERIOD;
		pBuff2->m_PlayerModifier.nSTR.nMod = 10;	
		pBuff2->m_PlayerModifier.nSTR.fPer = 2.0f;	
		GTalentInfo* pBuffTalent2= test.buff.NewBuffTalent(pBuff2);
		pBuffTalent2->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;

		// ���� ����
		m_pPlayer->GetPlayerInfo()->nLevel = 1;
		m_pPlayer->GetPlayerInfo()->nSTR = 10;
		// ���� ����
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		test.talent.UseTalent(m_pPlayer, pBuffTalent2, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID2));

		// ���� ����
		m_pField->Update(fDuration);	
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID2));
		CHECK_EQUAL(m_pPlayer->GetSTR(), 10);
	}

	TEST_FIXTURE(FBuff, RestoreDuplicateBuffPeriodActorModifierEffect)
	{
		int nBuffID = 123;
		float fDuration = 10.0f;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		// ����� �Ӽ�
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_GAIN_N_PERIOD;
		pBuff->m_ActorModifier.fMoveSpeed = 2.0f;	
		GTalentInfo* pBuffTalent= test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_fDuration = fDuration;
		pBuff->m_fPeriod = 2.0f;

		// ���� ����
		float fSpeed = (float)MoveSpeedByWeapon[WEAPON_NONE];
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetWalkSpeed(), (fSpeed)*(1.0f+2.0f));
		m_pPlayer->GetModuleBuff()->Update(pBuff->m_fPeriod);
		CHECK_EQUAL(m_pPlayer->GetWalkSpeed(), (fSpeed)*(1.0f+4.0f));
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(m_pPlayer->GetWalkSpeed(), (fSpeed)*(1.0f+2.0f));

		// ���� ����
		GradualyLostBuff(nBuffID);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetWalkSpeed(), fSpeed);
	}
}
