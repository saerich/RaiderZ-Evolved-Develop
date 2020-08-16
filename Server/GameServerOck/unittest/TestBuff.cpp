#include "stdafx.h"
#include "GTestForward.h"
#include "GModuleBuff.h"
#include "GModuleCombat.h"
#include "GServer.h"
#include "GFieldMgr.h"
#include "FBasePlayer.h"
#include "CSTalentInfoEnum.h"
#include "GHateTable.h"
#include "GBuffRequirement.h"
#include "GBuff.h"

SUITE(Buff)
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
			test.network.ClearLinks();
			m_pField->Destroy();
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
			pLink->AddIgnoreID(MC_BUFF_INSTANT_EFFECT_GAIN);
			pLink->AddIgnoreID(MC_ACTION_BUFF_HIT_NO_MF);
		}

		GTalentInfo* CreateTestTalent_BuffPassiveExtra(int nBuffID, float fDurationTime, BUFF_PASSIVE_EXTRA_ATTRIB_ eAttrib)
		{
			GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
			pBuff->m_nPassiveExtraAttrib = eAttrib;
			GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
			pBuff->m_fDuration = fDurationTime;

			return pBuffTalent;
		}

		MockField*		m_pField;
		GEntityPlayer*	m_pPlayer;
		MockLink*		m_pLinkTester;

		DECLWRAPPER_FieldInfoMgr;
		DECLWRAPPER_FieldMgr;
		DECLWRAPPER_BuffMgr;
		DECLWRAPPER_TalentMgr;
	};

	TEST_FIXTURE(FBuff, GainBuff)
	{
		int nBuffID = 123;
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, GainBuff_ViaBuffInfo)
	{
		int nBuffID = 123;
		float fDuration = 100.0f;
		GBuffInfo* pBuff = test.buff.NewBuffInfo(nBuffID);
		GTalentInfo* pBuffTalent = test.buff.NewBuffTalent(pBuff, fDuration); 
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		m_pField->Update(fDuration);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, LostBuffWhenDurationExpired)
	{
		int nBuffID = 123;
		float fDuration = 100.0f;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		test.buff.GainBuffSelf(m_pPlayer, pBuff, fDuration);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		m_pField->Update(fDuration);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, LostBuffWhenDurationExpiredViaTimeSlice)
	{
		int nBuffID = 123;
		float fDuration = 10.0f;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		test.buff.GainBuffSelf(m_pPlayer, pBuff, fDuration);
		for (int i=0; i<10; i++)
		{
			CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
			m_pField->Update(1.0f);
		}
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, TestBuffOverwrite)
	{
		int nBuffSlot = 777;
		// �Ŀ��� 50�� ���� ����
		GBuffInfo* pBuffPow50=test.buff.NewBuffInfo(50);
		pBuffPow50->m_nStackSlot = nBuffSlot;
		pBuffPow50->m_nStackPower = 50;
		// �Ŀ��� 100�� ���� ����
		GBuffInfo* pBuffPow100=test.buff.NewBuffInfo(100);
		pBuffPow100->m_nStackSlot = nBuffSlot;
		pBuffPow100->m_nStackPower = 100;
		// �Ŀ��� 100�� �Ǵٸ� ���� ����
		GBuffInfo* pBuffPow100_Duplicate=test.buff.NewBuffInfo(101);
		pBuffPow100_Duplicate->m_nStackSlot = nBuffSlot;
		pBuffPow100_Duplicate->m_nStackPower = 100;
		// �Ŀ��� 200�� ���� ����
		GBuffInfo* pBuffPow200=test.buff.NewBuffInfo(200);
		pBuffPow200->m_nStackSlot = nBuffSlot;
		pBuffPow200->m_nStackPower = 200;

		// ���� �����Ŀ����� ���� ������ ���õȴ�.
		test.buff.GainBuffSelf(m_pPlayer, pBuffPow100);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(100));
		test.buff.GainBuffSelf(m_pPlayer, pBuffPow50);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(100));

		// ���ų� ū ���� �Ŀ��� ������ �ڿ� �� ��� �ٲ��.
		test.buff.GainBuffSelf(m_pPlayer, pBuffPow100_Duplicate);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(101));
		test.buff.GainBuffSelf(m_pPlayer, pBuffPow200);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(200));
	}

	TEST_FIXTURE(FBuff, TestBuffUseToOtherPlayer)
	{
		int nBuffID = 123;
		GEntityPlayer* pOtherPlayer = NewEntityPlayer(m_pField);

		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		GTalentInfo* pBuffTalent= test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
		
		// ���� �ŷ�Ʈ ����
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(false, pOtherPlayer->HasBuff(nBuffID));
		test.talent.UseTalent(m_pPlayer, pBuffTalent, pOtherPlayer->GetUID());
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(true, pOtherPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, TestBuffUseToOtherNPC)
	{
		int nBuffID = 123;
		GEntityNPC* pOtherNPC = GUTHelper_NPC::SpawnNPC(m_pField);

		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;

		// ���� �ŷ�Ʈ ����
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(false, pOtherNPC->HasBuff(nBuffID));
		test.talent.UseTalent(m_pPlayer, pBuffTalent, pOtherNPC->GetUID());
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(true, pOtherNPC->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, BuffingToParty)
	{
		int nBuffID = 123;
		// ���ο� ��Ƽ ����
		GEntityPlayer* pOtherPlayer = NewEntityPlayer(m_pField);
		GParty* pParty = test.party.CreateParty(m_pPlayer, pOtherPlayer);

		// ��Ƽ ������ ���� ����
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		GTalentInfo* pBuffTalent= test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuffTalent->m_Buff1.EffectInfo.m_nRelation = CSEffectInfo::RELATION_PARTY;
		pBuffTalent->m_Buff1.EffectInfo.m_fRadius = 100.f;

		// ��Ƽ���� ���������� �̵���Ŵ
		pOtherPlayer->SetPos(m_pPlayer->GetPos()+vec3(pBuffTalent->m_Buff1.EffectInfo.m_fRadius*2, 0, 0));

		// �ڱ��ڽſ��� ��Ƽ ���� ����
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(false, pOtherPlayer->HasBuff(nBuffID));
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(false, pOtherPlayer->HasBuff(nBuffID));		// �������̶� �Ȱɸ�

		// ��Ƽ���� ���������� �̵���Ŵ
		pOtherPlayer->SetPos(m_pPlayer->GetPos()+vec3(pBuffTalent->m_Buff1.EffectInfo.m_fRadius/2, 0, 0));

		// �ڱ��ڽſ��� ��Ƽ ���� ����
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(true, pOtherPlayer->HasBuff(nBuffID));		// �������̶� �ɸ�

		test.party.DeleteParty(pParty);
	}


	TEST_FIXTURE(FBuff, TargetGainBuffFromHitTalent)
	{
		int nBuffID = 123;
		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, m_pPlayer->GetPos());
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		// Ÿ���� ������ ������ ������ �ɸ�
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		p1DmgTalent->m_Buff1.nID = pBuff->m_nID;		
		p1DmgTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
		p1DmgTalent->m_Buff1.EffectInfo.m_nRelation = CSEffectInfo::RELATION_ALL;
		p1DmgTalent->m_Buff1.nEnchantEvent = TC_HIT_TALENT;
		
		test.talent.UseTalent(m_pPlayer, p1DmgTalent, pDefender->GetUID());
		CHECK_EQUAL(true, pDefender->HasBuff(nBuffID));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, SelfGainBuffFromHitTalent)
	{
		int nBuffID = 123;
		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, m_pPlayer->GetPos());
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		// �ڽ��� ������ �ϸ� ������ �ɸ�
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		p1DmgTalent->m_Buff1.nID = pBuff->m_nID;		
		p1DmgTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		p1DmgTalent->m_Buff1.EffectInfo.m_nRelation = CSEffectInfo::RELATION_ALL;
		p1DmgTalent->m_Buff1.nEnchantEvent = TC_HIT_TALENT;

		test.talent.UseTalent(m_pPlayer, p1DmgTalent, pDefender->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(false, pDefender->HasBuff(nBuffID));
	}


	// �ڽſ��� �ѹ��� �ŷ�Ʈ �������� �ΰ��� ������ ���ÿ� �ɱ�
	TEST_FIXTURE(FBuff, GainBuffWithTwoEffect)
	{
		const float DURATION_TIME = 10.0f;

		// ���� ����
		int nBuffID1 = 123;
		int nBuffID2 = 321;
		GBuffInfo* pBuff1=test.buff.NewBuffInfo(nBuffID1);
		GBuffInfo* pBuff2=test.buff.NewBuffInfo(nBuffID2);
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		p1DmgTalent->m_Buff1.nID = pBuff1->m_nID;
		p1DmgTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		p1DmgTalent->m_Buff1.EffectInfo.m_nRelation = CSEffectInfo::RELATION_ALL;
		p1DmgTalent->m_Buff1.nEnchantEvent = TC_USE_TALENT;
		pBuff1->m_fDuration = DURATION_TIME;
		p1DmgTalent->m_Buff2.nID = pBuff2->m_nID;
		p1DmgTalent->m_Buff2.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		p1DmgTalent->m_Buff2.EffectInfo.m_nRelation = CSEffectInfo::RELATION_ALL;
		p1DmgTalent->m_Buff2.nEnchantEvent = TC_USE_TALENT;
		pBuff2->m_fDuration = DURATION_TIME/2;

		// �ڽſ��� �ѹ��� �ŷ�Ʈ �������� �ΰ��� ������ ���ÿ� �ɱ�
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID1));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID2));
		test.talent.UseTalent(m_pPlayer, p1DmgTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID1));
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID2));

		// �ð��� ������ �������� �ϳ��� Ǯ��
		m_pField->Update(DURATION_TIME/2);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID1));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID2));
		m_pField->Update(DURATION_TIME/2);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID1));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID2));

		// ��뿡�� �ǰ��ؼ� ��밡 �� ������ ���ÿ� �ɸ���
		m_pField->Update(DURATION_TIME);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID1));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID2));
		p1DmgTalent->m_Buff1.nEnchantEvent = TC_HIT_TALENT;
		p1DmgTalent->m_Buff2.nEnchantEvent = TC_HIT_TALENT;
		p1DmgTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
		p1DmgTalent->m_Buff2.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, m_pPlayer->GetPos());
		test.talent.UseTalent(m_pPlayer, p1DmgTalent, pDefender->GetUID());
		CHECK_EQUAL(true, pDefender->HasBuff(nBuffID1));
		CHECK_EQUAL(true, pDefender->HasBuff(nBuffID2));

		// ��뿡�� �ǰ��ؼ� �ڽ��� �� ������ ���ÿ� �ɸ���
		m_pField->Update(DURATION_TIME);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID1));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID2));
		p1DmgTalent->m_Buff1.nEnchantEvent = TC_HIT_TALENT;
		p1DmgTalent->m_Buff2.nEnchantEvent = TC_HIT_TALENT;
		p1DmgTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		p1DmgTalent->m_Buff2.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		test.talent.UseTalent(m_pPlayer, p1DmgTalent, pDefender->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID1));
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID2));

		// ��뿡�� �ǰ��ؼ� �ڽŰ� ���� ���� �ϳ��� �ɸ���
		m_pField->Update(DURATION_TIME);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID1));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID2));
		p1DmgTalent->m_Buff1.nEnchantEvent = TC_HIT_TALENT;
		p1DmgTalent->m_Buff2.nEnchantEvent = TC_HIT_TALENT;
		p1DmgTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
		p1DmgTalent->m_Buff2.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		test.talent.UseTalent(m_pPlayer, p1DmgTalent, pDefender->GetUID());
		CHECK_EQUAL(true, pDefender->HasBuff(nBuffID1));
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID2));
	}

	TEST_FIXTURE(FBuff, ApplyBuffMod)
	{
		const float DURATION_TIME = 10.0f;
		m_pPlayer->GetChrInfo()->nAP = 100;

		// �������� �ִ� ���� ����
		GBuffInfo* pBuff=test.buff.NewBuffInfo();
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuffTalent->m_Buff1.EffectInfo.m_nRelation = CSEffectInfo::RELATION_ALL;
		pBuff->m_fDuration = DURATION_TIME;
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_GAIN;
		pBuff->m_ActorModifier.nHPMax.nMod = 20;
		pBuff->m_ActorModifier.nHPMax.fPer = 1.5f;
		pBuff->m_ActorModifier.nENMax.nMod = 30;
		pBuff->m_ActorModifier.nENMax.fPer = 1.5f;
		pBuff->m_ActorModifier.nSTAMax.nMod = 40;
		pBuff->m_ActorModifier.nSTAMax.fPer = 1.5f;
		pBuff->m_ActorModifier.nAP.nMod = 50;
		pBuff->m_ActorModifier.nAP.fPer = 1.5f;
		CHECK_EQUAL(m_pPlayer->GetMaxHP(), 800);
		CHECK_EQUAL(m_pPlayer->GetMaxEN(), 450);
		CHECK_EQUAL(m_pPlayer->GetMaxSTA(), 100);
		CHECK_EQUAL(m_pPlayer->GetAP(), 100);

		// ���� ����
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(m_pPlayer->GetMaxHP(),	(int)((800+20)*(1.0f+1.5f)));
		CHECK_EQUAL(m_pPlayer->GetMaxEN(),	(int)((450+30)*(1.0f+1.5f)));
		CHECK_EQUAL(m_pPlayer->GetMaxSTA(),	(int)((100+40)*(1.0f+1.5f)));
		CHECK_EQUAL(m_pPlayer->GetAP(),		(int)((100+50)*(1.0f+1.5f)));

		// ���� ����
		m_pField->Update(DURATION_TIME);	
		CHECK_EQUAL(m_pPlayer->GetMaxHP(), 800);
		CHECK_EQUAL(m_pPlayer->GetMaxEN(), 450);
		CHECK_EQUAL(m_pPlayer->GetMaxSTA(), 100);
		CHECK_EQUAL(m_pPlayer->GetAP(), 100);

		// �������� �ִ� �Ǵٸ� ���� ����
		GBuffInfo* pBuff2=test.buff.NewBuffInfo();
		GTalentInfo* pBuffTalent2=test.buff.NewBuffTalent(pBuff2);
		pBuffTalent2->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuffTalent2->m_Buff1.EffectInfo.m_nRelation = CSEffectInfo::RELATION_ALL;
		pBuff2->m_fDuration = DURATION_TIME/2;
		pBuff2->m_Condition.nEffectConditionType = TC_BUFF_GAIN;
		pBuff2->m_ActorModifier.nHPMax.nMod = 50;
		pBuff2->m_ActorModifier.nHPMax.fPer = 1.5f;
		pBuff2->m_ActorModifier.nENMax.nMod = 60;
		pBuff2->m_ActorModifier.nENMax.fPer = 1.5f;
		pBuff2->m_ActorModifier.nSTAMax.nMod = 70;
		pBuff2->m_ActorModifier.nSTAMax.fPer = 1.5f;
		pBuff2->m_ActorModifier.nAP.nMod = 80;
		pBuff2->m_ActorModifier.nAP.fPer = 1.5f;
		CHECK_EQUAL(m_pPlayer->GetMaxHP(), 800);
		CHECK_EQUAL(m_pPlayer->GetMaxEN(), 450);
		CHECK_EQUAL(m_pPlayer->GetMaxSTA(), 100);
		CHECK_EQUAL(m_pPlayer->GetAP(), 100);

		// ���� ����
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		test.talent.UseTalent(m_pPlayer, pBuffTalent2, m_pPlayer->GetUID());
		CHECK_EQUAL(m_pPlayer->GetMaxHP(),	(int)((800+20+50)*(1.0f+1.5f+1.5f)));
		CHECK_EQUAL(m_pPlayer->GetMaxEN(),	(int)((450+30+60)*(1.0f+1.5f+1.5f)));
		CHECK_EQUAL(m_pPlayer->GetMaxSTA(),	(int)((100+40+70)*(1.0f+1.5f+1.5f)));
		CHECK_EQUAL(m_pPlayer->GetAP(),	(int)((100+50+80)*(1.0f+1.5f+1.5f)));

		// 2�� ���� ����
		m_pField->Update(DURATION_TIME/2);	
		CHECK_EQUAL(m_pPlayer->GetMaxHP(),	(int)((800+20)*(1.0f+1.5f)));
		CHECK_EQUAL(m_pPlayer->GetMaxEN(),	(int)((450+30)*(1.0f+1.5f)));
		CHECK_EQUAL(m_pPlayer->GetMaxSTA(),	(int)((100+40)*(1.0f+1.5f)));
		CHECK_EQUAL(m_pPlayer->GetAP(),		(int)((100+50)*(1.0f+1.5f)));

		// ��� ���� ����
		m_pField->Update(DURATION_TIME/2);	
		CHECK_EQUAL(m_pPlayer->GetMaxHP(), 800);
		CHECK_EQUAL(m_pPlayer->GetMaxEN(), 450);
		CHECK_EQUAL(m_pPlayer->GetMaxSTA(), 100);
		CHECK_EQUAL(m_pPlayer->GetAP(), 100);
	}

	TEST_FIXTURE(FBuff, GainBuffWithPeriodEffect)
	{
		const float DURATION_TIME = 10.0f;

		// �������� �ִ� ���� ����
		GBuffInfo* pBuff=test.buff.NewBuffInfo();
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_fDuration = DURATION_TIME;
		pBuff->m_fPeriod = 2.0f;
		pBuff->m_InstantModifier.nHP.nMod = 10;
		pBuff->m_InstantModifier.nEN.nMod = 10;
		pBuff->m_InstantModifier.nSTA.nMod = 10;

		// �÷��̾� ��ġ�� ����
		m_pPlayer->GetChrInfo()->nMaxHP = 100;
		m_pPlayer->GetChrInfo()->nMaxEN = 100;
		m_pPlayer->GetChrInfo()->nMaxSTA = 100;
		m_pPlayer->SetHP(50);
		m_pPlayer->SetEN(50);
		m_pPlayer->SetSTA(50);

		// ���� ����
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		m_pField->Update(0.0f);	

		// ó�� ������ �ɸ����� �ֱ��� ȿ���� ����.
		CHECK_EQUAL(m_pPlayer->GetHP(), 60);
		CHECK_EQUAL(m_pPlayer->GetEN(), 60);
		CHECK_EQUAL(m_pPlayer->GetSTA(), 60);

		// �ֱⰡ ������
		m_pField->Update(1.0f);	
		CHECK_EQUAL(m_pPlayer->GetHP(), 60);
		CHECK_EQUAL(m_pPlayer->GetEN(), 60);
		CHECK_EQUAL(m_pPlayer->GetSTA(), 60);

		// �ֱⰡ ���� ��
		m_pField->Update(1.0f);
		CHECK_EQUAL(m_pPlayer->GetHP(), 70);
		CHECK_EQUAL(m_pPlayer->GetEN(), 70);
		CHECK_EQUAL(m_pPlayer->GetSTA(), 70);

		// ���� ���ӽð� ����
		m_pField->Update(DURATION_TIME);	// ������ �ֱ��� ȿ���� �з��� ó���� (�ִ� HP��ŭ ȸ��)
		CHECK_EQUAL(m_pPlayer->GetHP(), 120);	// �ι��� ƽ�� ���ƿ����Ƿ� 2�� ȿ�� ����
		CHECK_EQUAL(m_pPlayer->GetEN(), 120);
		CHECK_EQUAL(m_pPlayer->GetSTA(), 100);
	}

	TEST_FIXTURE(FBuff, GainBuffWithPeriodEffectViaBuffInfo)
	{
		const float DURATION_TIME = 10.0f;

		// �������� �ִ� ���� ����
		GBuffInfo* pBuff=test.buff.NewBuffInfo();
		pBuff->m_fDuration = DURATION_TIME;
		pBuff->m_fPeriod = 2.0f;
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_InstantModifier.nHP.nMod = 10;
		pBuff->m_InstantModifier.nEN.nMod = 10;
		pBuff->m_InstantModifier.nSTA.nMod = 10;

		// �÷��̾� ��ġ�� ����
		m_pPlayer->GetChrInfo()->nMaxHP = 100;
		m_pPlayer->GetChrInfo()->nMaxEN = 100;
		m_pPlayer->GetChrInfo()->nMaxSTA = 100;
		m_pPlayer->SetHP(50);
		m_pPlayer->SetEN(50);
		m_pPlayer->SetSTA(50);

		// ���� ����
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		m_pField->Update(0.0f);	

		// ó�� ������ �ɸ����� �ֱ��� ȿ���� ����.
		CHECK_EQUAL(m_pPlayer->GetHP(), 60);
		CHECK_EQUAL(m_pPlayer->GetEN(), 60);
		CHECK_EQUAL(m_pPlayer->GetSTA(), 60);

		// �ֱⰡ ������
		m_pField->Update(1.0f);	
		CHECK_EQUAL(m_pPlayer->GetHP(), 60);
		CHECK_EQUAL(m_pPlayer->GetEN(), 60);
		CHECK_EQUAL(m_pPlayer->GetSTA(), 60);

		// �ֱⰡ ���� ��
		m_pField->Update(1.0f);
		CHECK_EQUAL(m_pPlayer->GetHP(), 70);
		CHECK_EQUAL(m_pPlayer->GetEN(), 70);
		CHECK_EQUAL(m_pPlayer->GetSTA(), 70);

		// ���� ���ӽð� ����
		m_pField->Update(DURATION_TIME);	// ������ �ֱ��� ȿ���� �з��� ó���� (�ִ� HP��ŭ ȸ��)
		CHECK_EQUAL(m_pPlayer->GetHP(), 120);	// �ι��� ƽ�� ���ƿ����Ƿ� 2�� ȿ�� ����
		CHECK_EQUAL(m_pPlayer->GetEN(), 120);
		CHECK_EQUAL(m_pPlayer->GetSTA(), 100);
	}


	TEST_FIXTURE(FBuff, TestGainBuffWithPeriodEffect_PeriodTimeLonggerThanDurationTime)
	{
		const float DURATION_TIME = 10.0f;

		// �������� �ִ� ���� ����
		GBuffInfo* pBuff=test.buff.NewBuffInfo();
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_fDuration = DURATION_TIME;
		pBuff->m_fPeriod = DURATION_TIME+1.0f;  // ���ӽð����� �ֱ�ð��� �� �� ��� 
															// ���ӽð��� �ֱ�ð��� �����ش�.
		pBuff->m_InstantModifier.nHP.nMod = 10;
		pBuff->m_InstantModifier.nEN.nMod = 10;
		pBuff->m_InstantModifier.nSTA.nMod = 10;

		// �÷��̾� �ʱ� ��ġ�� ����
		m_pPlayer->GetChrInfo()->nMaxHP = 100;
		m_pPlayer->GetChrInfo()->nMaxEN = 100;
		m_pPlayer->GetChrInfo()->nMaxSTA = 100;
		m_pPlayer->SetHP(50);
		m_pPlayer->SetEN(50);
		m_pPlayer->SetSTA(50);

		// ���� ����
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());

		// �ֱ�ð��� �� Ŀ�� 1ȸ ȿ���� �����Ѵ�.(�ֱ�ð��� �ȵǼ� ȿ�� ������)
		m_pField->Update(DURATION_TIME);	
		CHECK_EQUAL(m_pPlayer->GetHP(), 60);
		CHECK_EQUAL(m_pPlayer->GetEN(), 60);
		CHECK_EQUAL(m_pPlayer->GetSTA(), 60);

		// �ֱ�ð��� �Ǿ� ȿ������.  ����ð��� �ֱ�ð����� ����Ǿ���
		m_pField->Update(1.0f);	
		CHECK_EQUAL(m_pPlayer->GetHP(), 70);
		CHECK_EQUAL(m_pPlayer->GetEN(), 70);
		CHECK_EQUAL(m_pPlayer->GetSTA(), 70);
	}

	TEST_FIXTURE(FBuff, TestGainBuffWithPeriodEffect_PeriodTimeIsZero)
	{
		const float DURATION_TIME = 10.0f;

		// �������� �ִ� ���� ����
		GBuffInfo* pBuff=test.buff.NewBuffInfo();
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_PERIOD;
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_fDuration = DURATION_TIME;
		pBuff->m_fPeriod = 0.0f;  // �ֱ�ð��� 0��
		pBuff->m_InstantModifier.nHP.nMod = 10;
		pBuff->m_InstantModifier.nEN.nMod = 10;
		pBuff->m_InstantModifier.nSTA.nMod = 10;

		// �÷��̾� �ʱ� ��ġ�� ����
		m_pPlayer->GetChrInfo()->nMaxHP = 100;
		m_pPlayer->GetChrInfo()->nMaxEN = 100;
		m_pPlayer->GetChrInfo()->nMaxSTA = 100;
		m_pPlayer->SetHP(50);
		m_pPlayer->SetEN(50);
		m_pPlayer->SetSTA(50);

		// ȿ�� ����
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		m_pField->Update(DURATION_TIME);	
		CHECK_EQUAL(m_pPlayer->GetHP(), 50);
		CHECK_EQUAL(m_pPlayer->GetEN(), 50);
		CHECK_EQUAL(m_pPlayer->GetSTA(), 50);
	}

	TEST_FIXTURE(FBuff, RefCount_SelfBuffing)
	{
		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);

		CHECK_EQUAL(m_pPlayer->GetReferenceCount(), TFT_SIZE+1);
		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(m_pPlayer->GetReferenceCount(), TFT_SIZE+1);

		// ���� ������ ����
		CHECK_EQUAL(m_pField->GetPlayerQty(), 1);
		DeleteEntityPlayer(m_pPlayer);
		CHECK_EQUAL(m_pField->GetPlayerQty(), 0);	// ���۷����� ��� ������
	}

	TEST_FIXTURE(FBuff, InfinityBuff)
	{
		// ������ ���� ����
		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuff->m_fDuration = BUFF_DURATION_INFINITY;

		// ���� ���
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// �ð��� ���� ������ ��Ǯ��
		m_pField->Update(10000.0f);	
		m_pField->Update(10000.0f);	
		m_pField->Update(10000.0f);	
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, ClearBuffWhenDie)
	{
		// ������ ���� ����
		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuff->m_fDuration = BUFF_DURATION_INFINITY;

		// ���� ���
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// �׾ ������ Ǯ��
		m_pPlayer->doDie();
		m_pPlayer->GetModuleBuff()->Update(0.1f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, LostBuff)
	{
		int nBuffID = 123;
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		test.buff.LostBuff(m_pPlayer, 123);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, LostBuffStack)
	{
		// 32 ������ ���� ����
		int nBuffID1 = 1;
		GBuffInfo* pBuff1=test.buff.NewBuffInfo(nBuffID1);
		GTalentInfo* pBuffTalent1=test.buff.NewBuffTalent(pBuff1);
		pBuff1->m_nStackSlot = 32;

		// 128 ������ ���� ����
		int nBuffID3 = 3;
		GBuffInfo* pBuff3=test.buff.NewBuffInfo(nBuffID3);
		GTalentInfo* pBuffTalent3=test.buff.NewBuffTalent(pBuff3);
		pBuff3->m_nStackSlot = 128;

		// ��� ���� �ɱ�
		test.talent.UseTalent(m_pPlayer, pBuffTalent1, m_pPlayer->GetUID());
		test.talent.UseTalent(m_pPlayer, pBuffTalent3, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID1));
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID3));

		// 128 ���� ������ ���ֱ�
		test.buff.LostBuffStack(m_pPlayer, 128);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID1));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID3));

		// 32 ���� ������ ���ֱ�
		test.buff.LostBuffStack(m_pPlayer, 32);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID1));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID3));
	}

	TEST_FIXTURE(FBuff, GainPacketCheck)
	{
		int nBuffID = 123;
		float fDuration = 10.0f;
		GEntityPlayer* pOtherPlayer = NewEntityPlayer(m_pField);
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
		pBuff->m_fDuration = fDuration;

		BeginCommandRecord();
		test.talent.UseTalent(m_pPlayer, pBuffTalent, pOtherPlayer->GetUID());
		CHECK_EQUAL(true, pOtherPlayer->HasBuff(nBuffID));
				
		// ��Ŷ üũ
		const minet::MCommand& Command = m_pLinkTester->GetCommand(0);
		CHECK_EQUAL(Command.GetID(), MC_BUFF_GAIN);
		TVALID(Command.GetParameterCount() >= 3);
		UIID nPacketActorUIID;
		int nPacketBuffID;
		float fPacketDuration;
		if (!Command.GetParameter(&nPacketActorUIID,	0, MPT_USHORT))		{ CHECK(false); return; }
		if (!Command.GetParameter(&nPacketBuffID,	1, MPT_INT))		{ CHECK(false); return; }
		if (!Command.GetParameter(&fPacketDuration,	2, MPT_FLOAT))		{ CHECK(false); return; }
		CHECK_EQUAL(nPacketActorUIID, pOtherPlayer->GetUIID());
		CHECK_EQUAL(nPacketBuffID, nBuffID);
		CHECK_EQUAL(fPacketDuration, fDuration);
	}

	TEST_FIXTURE(FBuff, LostPacketCheck)
	{
		int nBuffID = 123;
		float fDuration = 10.0f;
		GEntityPlayer* pOtherPlayer = NewEntityPlayer(m_pField);
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
		pBuff->m_fDuration = fDuration;
		test.talent.UseTalent(m_pPlayer, pBuffTalent, pOtherPlayer->GetUID());
		CHECK_EQUAL(true, pOtherPlayer->HasBuff(nBuffID));

		BeginCommandRecord();
		m_pField->Update(fDuration);	// ���� ����
		CHECK_EQUAL(false, pOtherPlayer->HasBuff(nBuffID));

		// ��Ŷ üũ
		const minet::MCommand& Command = m_pLinkTester->GetCommand(0);
		CHECK_EQUAL(Command.GetID(), MC_BUFF_LOST);
		TVALID(Command.GetParameterCount() >= 2);
		UIID nPacketActorUIID;
		int nPacketBuffID;
		if (!Command.GetParameter(&nPacketActorUIID,	0, MPT_USHORT))		{ CHECK(false); return; }
		if (!Command.GetParameter(&nPacketBuffID,		1, MPT_INT))		{ CHECK(false); return; }
		CHECK_EQUAL(nPacketActorUIID, pOtherPlayer->GetUIID());
		CHECK_EQUAL(nPacketBuffID, nBuffID);
	}

	TEST_FIXTURE(FBuff, MaintainCost)
	{
		// ������ ���� ����
		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		// ���� �������
		pBuff->m_nHPCost = 10;
		pBuff->m_nENCost = 10;
		pBuff->m_nSTACost = 10;
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuff->m_fDuration = BUFF_DURATION_INFINITY;

		// ���� ���
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		m_pPlayer->SetHP(30);
		m_pPlayer->SetEN(30);
		m_pPlayer->SetSTA(30);

		m_pField->Update(1.0f);	
		CHECK_EQUAL(20, m_pPlayer->GetHP());
		CHECK_EQUAL(20, m_pPlayer->GetEN());
		CHECK_EQUAL(20, m_pPlayer->GetSTA());

		m_pField->Update(1.0f);	
		CHECK_EQUAL(10, m_pPlayer->GetHP());
		CHECK_EQUAL(10, m_pPlayer->GetEN());
		CHECK_EQUAL(10, m_pPlayer->GetSTA());

		m_pField->Update(1.0f);	
		CHECK_EQUAL(0, m_pPlayer->GetHP());
		CHECK_EQUAL(0, m_pPlayer->GetEN());
		CHECK_EQUAL(0, m_pPlayer->GetSTA());

		// ���� ����� ��� ������ Ǯ��
		m_pField->Update(1.0f);	
		CHECK_EQUAL(0, m_pPlayer->GetHP());
		CHECK_EQUAL(0, m_pPlayer->GetEN());
		CHECK_EQUAL(0, m_pPlayer->GetSTA());
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, MaintainCost__ForLack)
	{
		// ������ ���� ����
		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		// ���� �������
		pBuff->m_nHPCost = 10;
		pBuff->m_nENCost = 10;
		pBuff->m_nSTACost = 10;
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuff->m_fDuration = BUFF_DURATION_INFINITY;

		// ���� ���
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		m_pPlayer->SetHP(30);
		m_pPlayer->SetEN(30);
		m_pPlayer->SetSTA(30);

		// ���� ����� ��� ������ Ǯ��
		m_pField->Update(4.0f);	
		CHECK_EQUAL(0, m_pPlayer->GetHP());
		CHECK_EQUAL(0, m_pPlayer->GetEN());
		CHECK_EQUAL(0, m_pPlayer->GetSTA());
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, SendBuffMoveSpeedMod_WhenSectorChanged)
	{
		GEntityPlayer* pEnterer = NewEntityPlayer(	m_pField, 
																m_pPlayer->GetPos()+vec3(SECTOR_SIZE*2+1,0,0));	// ���͹ۿ��� ������
		CHECK(pEnterer->GetSectorX() != m_pPlayer->GetSectorX());

		// �̵� ���� ������ ����
		GBuffInfo* pBuff=test.buff.NewBuffInfo();
		pBuff->m_ActorModifier.fMoveSpeed = 1.7f;
		test.buff.GainBuffSelf(pEnterer, pBuff);

		// �׽��Ͱ� �ִ� ���ͷ� �̵� (������ �̵��ؿ�: 2�����̵�)
		BeginCommandRecord();
		pEnterer->SetPos(m_pPlayer->GetPos());

		// ��Ŷ üũ
		const minet::MCommand& Command = m_pLinkTester->GetCommand(0);
		CHECK_EQUAL(MC_FIELD_IN_PLAYER, Command.GetID());
		TVALID(Command.GetParameterCount() >= 1);

		MCommandParameter* pParam = Command.GetParameter(0);
		if(pParam->GetType()!=MPT_SINGLE_BLOB)									{ CHECK(false); return; }
		TD_UPDATE_CACHE_PLAYER* pPlayerInfo = (TD_UPDATE_CACHE_PLAYER*)pParam->GetPointer();

		CHECK_EQUAL(pPlayerInfo->uid, pEnterer->GetUID());
	}

	// ���� ���� �ð� �������� For DB
	TEST_FIXTURE(FBuff, GetBuffRemainTime)
	{				
		GBuffInfo* pBuffInfo = test.buff.NewBuffInfo();
		float fDurationTime = 5.0f;
		GTalentInfo* pTalentInfo = test.buff.NewBuffTalent(pBuffInfo, fDurationTime);

		test.talent.UseTalent(m_pPlayer, pTalentInfo, m_pPlayer->GetUID());

		float fElapsedTime = 1.0f;
		m_pField->Update(fElapsedTime);

		vector<REMAIN_BUFF_TIME> vecBuffRemainTime;
		m_pPlayer->GetBuffRemainTime(vecBuffRemainTime);

		for(vector<REMAIN_BUFF_TIME>::iterator it = vecBuffRemainTime.begin(); it != vecBuffRemainTime.end(); it++)
		{
			if (it->nID == pBuffInfo->m_nID)
			{
				CHECK_EQUAL(fDurationTime - fElapsedTime, it->fRemainDurationSeconds);
			}
		}
	}

	// ���� ���� �ð� ����ֱ� For DB
	TEST_FIXTURE(FBuff, InsertBuffRemainTime)
	{
		GBuffInfo* pBuffInfo = test.buff.NewBuffInfo();
		float fDurationTime = 5.0f;
		GTalentInfo* pTalentInfo = test.buff.NewBuffTalent(pBuffInfo, fDurationTime);

		float fElapsedTime = 1.0f;
		float fRemainTime = fDurationTime - fElapsedTime;
		vector<REMAIN_BUFF_TIME> vecBuffRemainTime;

		REMAIN_BUFF_TIME remainBuffTime;
		remainBuffTime.nID = pBuffInfo->m_nID;
		remainBuffTime.fRemainDurationSeconds = fRemainTime;

		vecBuffRemainTime.push_back(remainBuffTime);
		m_pPlayer->InsertBuffRemainTime(vecBuffRemainTime);
		CHECK_EQUAL(true, m_pPlayer->GetModuleBuff()->IsGained(pBuffInfo->m_nID));
		m_pField->Update(fRemainTime);
		CHECK_EQUAL(false, m_pPlayer->GetModuleBuff()->IsGained(pBuffInfo->m_nID));				
	}

	// resist Mod ���� ���� �׽�Ʈ
	TEST_FIXTURE(FBuff, GainBuff_ResistMod)
	{
		int nBuffID = 123;
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_GAIN;
		pBuff->m_ActorModifier.nResistances[TRT_FR].nMod = 50;

		// ������ �ޱ� �� FR�� üũ
		CHECK_EQUAL(PLAYER_DEFAULT_FR, m_pPlayer->GetResist(TRT_FR));

		test.buff.GainBuffSelf(m_pPlayer, pBuff);

		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// ������ ������Ƿ� 50�� ���Ѵ�.
		CHECK_EQUAL(PLAYER_DEFAULT_FR+50, m_pPlayer->GetResist(TRT_FR));
	}

	// ���� �ֱ����� ȿ���� �׾����� ó�� (ũ������ ���� �ȵ�)
	TEST_FIXTURE(FBuff, DieDuePeriodEffect)
	{
		DECLWRAPPER_SimpleCombatCalc;

		const float DURATION_TIME = 10.0f;

		// �������� �ִ� ���� ����
		GBuffInfo* pBuff=test.buff.NewBuffInfo();
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_fDuration = DURATION_TIME;	// 10�ʵ��� ȿ��
		pBuff->m_fPeriod = 1.0f;	// 1�ʴٸ��� ȿ��
		pBuff->m_nMinDamage = 100;	// 100�� ���ظ� ����
		pBuff->m_nMaxDamage = 100;	// 100�� ���ظ� ����
		pBuff->m_Resist.m_nDifficulty = 255; // ���� �Ұ�

		// �÷��̾� ��ġ�� ����
		m_pPlayer->GetChrInfo()->nMaxHP = 100;
		m_pPlayer->doRebirthBySelf();
		m_pPlayer->SetHP(1);	// �Ѵ� ������ ����

		// ���� ����
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		m_pPlayer->GetModuleBuff()->Update(0.1f);

		// ���� �°� ����
		CHECK_EQUAL(true, m_pPlayer->IsDead());
	}

	// �����϶��� DOT ȿ�� ���Ա�
	TEST_FIXTURE(FBuff, DOT_DoNotEffectionDuringInvincible)
	{
		DECLWRAPPER_SimpleCombatCalc;

		const float DURATION_TIME = 10.0f;

		// �������� �ִ� ���� ����
		GBuffInfo* pBuff=test.buff.NewBuffInfo();
		pBuff->m_nType = BUFT_DEBUFF;
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_fDuration = DURATION_TIME;	// 10�ʵ��� ȿ��
		pBuff->m_fPeriod = 1.0f;	// 1�ʴٸ��� ȿ��
		pBuff->m_nMinDamage = 100;	// 100�� ���ظ� ����
		pBuff->m_nMaxDamage = 100;	// 100�� ���ظ� ����
		pBuff->m_Resist.m_nDifficulty = 255; // ���� �Ұ�

		// �÷��̾� ��ġ�� ����
		m_pPlayer->GetChrInfo()->nMaxHP = 100;
		m_pPlayer->doRebirthBySelf();
		m_pPlayer->SetHP(1);	// �Ѵ� ������ ����

		// ���� �ɱ�
		float fDurationTime = 5.0f;
		int nBuffID = 123;
		GTalentInfo* pBuffInvincibleTalent = CreateTestTalent_BuffPassiveExtra(nBuffID, fDurationTime, BUFPEA_INVINCIBILITY);
		// ���� ���
		test.talent.UseTalent(m_pPlayer, pBuffInvincibleTalent, m_pPlayer->GetUID());
		// ���� ����
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());

		// ���� �°� ����
		CHECK_EQUAL(false, m_pPlayer->IsDead());
		CHECK_EQUAL(1, m_pPlayer->GetHP());
	}

	// ������ �ŷ�Ʈ ����ϱ�
	TEST_FIXTURE(FBuff, UseTalentViaBuff)
	{
		DECLWRAPPER_SimpleCombatCalc;

		const float DURATION_TIME = 5.0f;
		const int TALENT_ID	= 777;

		// ���� ����
		GBuffInfo* pBuff=test.buff.NewBuffInfo();
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_nTalentID = TALENT_ID;

		// ������ �ɾ��ִ� �ŷ�Ʈ ����
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_fDuration = DURATION_TIME;	// 10�ʵ��� ȿ��
		pBuff->m_fPeriod = 1.0f;	// 1�ʴٸ��� ȿ��
		pBuff->m_Resist.m_nDifficulty = 255; // ���� �Ұ�

		// ���� �߿� ���� �ŷ�Ʈ ����
		GTalentInfo* pUseTalent=test.talent.NewTalentInfo(TALENT_ID);
		pUseTalent->m_nSkillType = ST_MAGIC;
		pUseTalent->m_InstantModifier.nHP.nMod = 10;
		pUseTalent->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		
		// �÷��̾� ��ġ�� ����
		m_pPlayer->GetChrInfo()->nMaxHP = 1000;
		m_pPlayer->SetHP(500);

		// ���� ���
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());

		// �ŷ�Ʈ ȿ�� ���
		m_pPlayer->GetModuleBuff()->Update(0.1f);
		CHECK_EQUAL(510, m_pPlayer->GetHP());
		m_pPlayer->GetModuleBuff()->Update(pBuff->m_fPeriod);
		CHECK_EQUAL(520, m_pPlayer->GetHP());
		m_pPlayer->GetModuleBuff()->Update(pBuff->m_fPeriod);
		CHECK_EQUAL(530, m_pPlayer->GetHP());
		m_pPlayer->GetModuleBuff()->Update(pBuff->m_fPeriod);
		CHECK_EQUAL(540, m_pPlayer->GetHP());
		m_pPlayer->GetModuleBuff()->Update(pBuff->m_fPeriod);
		CHECK_EQUAL(550, m_pPlayer->GetHP());
		m_pPlayer->GetModuleBuff()->Update(pBuff->m_fPeriod);
		CHECK_EQUAL(560, m_pPlayer->GetHP());
		m_pPlayer->GetModuleBuff()->Update(pBuff->m_fPeriod);
		CHECK_EQUAL(560, m_pPlayer->GetHP());
	}

	TEST_FIXTURE(FBuff, EnchantResist)
	{
		int nBuffID = 123;
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		CSBuffEnchantInfo infoEnchant;
		infoEnchant.nID = nBuffID;
		infoEnchant.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		infoEnchant.EffectInfo.m_nRelation = CSEffectInfo::RELATION_PARTY;
		infoEnchant.Resist.m_nMethod = TRM_BINARY; // ������� ���� ������ ������������ ����
		infoEnchant.Resist.m_nType = TRT_CR;
		infoEnchant.Resist.m_nDifficulty = 0;		// �����ϱ� ���� ����
		m_pPlayer->GetChrInfo()->Resist.nValue[TRT_CR] = 255; // ���� �ִ�
		m_pPlayer->GetChrStatus()->ActorModifier.nResistances[TRT_CR].nMod = 10000;
		m_pPlayer->GetChrStatus()->ActorModifier.nResistances[TRT_CR].fPer = 100.0f; // 100��
		test.buff.GainBuffSelf(m_pPlayer, pBuff, &infoEnchant);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	// ���� Ǯ�� ������ �����϶��� ���׹���� ������������ ����
	TEST_FIXTURE(FBuff, Cooking_SetResistMethodToBinaryWhenReleaseConditionIsResist)
	{
		int nBuffID = 123;
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.infoRelease.nType = TC_RESIST;
		pBuff->m_Resist.m_nMethod = TRM_PARTIAL;
		pBuff->Cooking();
		CHECK_EQUAL(TRM_BINARY, pBuff->m_Resist.m_nMethod);
	}

	TEST_FIXTURE(FBuff, PeriodResist)
	{
		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuffTalent->m_Buff1.EffectInfo.m_nRelation = CSEffectInfo::RELATION_ALL;
		pBuff->m_Condition.infoRelease.nType = TC_RESIST;
		pBuff->m_Resist.m_nMethod = TRM_PARTIAL;
		pBuff->m_Resist.m_nType = TRT_CR;
		pBuff->m_Resist.m_nDifficulty = 0;		// �����ϱ� ���� ����
		pBuff->Cooking();
		m_pPlayer->GetChrInfo()->Resist.nValue[TRT_CR] = 255; // ���� �ִ�
		m_pPlayer->GetChrStatus()->ActorModifier.nResistances[TRT_CR].nMod = 10000;
		m_pPlayer->GetChrStatus()->ActorModifier.nResistances[TRT_CR].fPer = 100.0f; // 100��
		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		// 1�ʸ��� ���� üũ��
		m_pField->Update(1.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	// ����Ÿ���� ������ ���׼�ġ�� Ȯ�� üũ�ϱ�
	TEST_FIXTURE(FBuff, CheckChanceWhenResistIsNone)
	{
		int nBuffID = 123;
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		CSBuffEnchantInfo infoEnchant;
		infoEnchant.nID = nBuffID;
		infoEnchant.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		infoEnchant.EffectInfo.m_nRelation = CSEffectInfo::RELATION_PARTY;
		infoEnchant.Resist.m_nType = TRT_NONE;

		// ���� Ȯ�� 100%
		infoEnchant.Resist.m_nDifficulty = 0;		// ����Ÿ���� �������� Ȯ���� ���
		test.buff.GainBuffSelf(m_pPlayer, pBuff, &infoEnchant);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));

		// ���� Ȯ�� 0%
		infoEnchant.Resist.m_nDifficulty = 255;		// ����Ÿ���� �������� Ȯ���� ���
		test.buff.GainBuffSelf(m_pPlayer, pBuff, &infoEnchant);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, TestBuffUseToOtherNPC__InvalidUser)
	{
		int nBuffID = 123;
		GEntityNPC* pOtherNPC = GUTHelper_NPC::SpawnNPC(m_pField);

		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
		pBuff->m_fDuration = 10.0f;
		pBuff->m_fPeriod = 2.0f;
		pBuff->m_InstantModifier.nHP.nMod = 10;

		// ���� �ŷ�Ʈ ����
		test.talent.UseTalent(m_pPlayer, pBuffTalent, pOtherNPC->GetUID());
		m_pPlayer->DeleteMe();
		m_pField->Update(0.0f);
		pOtherNPC->GetModuleBuff()->Update(pBuff->m_fPeriod);
		pOtherNPC->GetModuleBuff()->Update(pBuff->m_fPeriod);
		pOtherNPC->GetModuleBuff()->Update(pBuff->m_fPeriod);
	}

	TEST_FIXTURE(FBuff, BuffEffect_HateAdj)
	{
		int nBuffID = 123;
		GEntityNPC* pOtherNPC = GUTHelper_NPC::SpawnNPC(m_pField);

		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_nHateAdj = 100;
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
		pBuff->m_fDuration = 100.0f;
		pBuff->m_fPeriod = 2.0f;
	

		// ���� �ŷ�Ʈ ����
		test.talent.UseTalent(m_pPlayer, pBuffTalent, pOtherNPC->GetUID());
		pOtherNPC->GetModuleBuff()->Update(0.1f);
		CHECK_EQUAL(100, pOtherNPC->GetHateTable().GetHate(m_pPlayer->GetUID()));
		pOtherNPC->GetModuleBuff()->Update(pBuff->m_fPeriod);
		CHECK_EQUAL(200, pOtherNPC->GetHateTable().GetHate(m_pPlayer->GetUID()));
		pOtherNPC->GetModuleBuff()->Update(pBuff->m_fPeriod);
		CHECK_EQUAL(300, pOtherNPC->GetHateTable().GetHate(m_pPlayer->GetUID()));
		pBuff->m_nHateAdj = -100;
		pOtherNPC->GetModuleBuff()->Update(pBuff->m_fPeriod);
		CHECK_EQUAL(200, pOtherNPC->GetHateTable().GetHate(m_pPlayer->GetUID()));
		pOtherNPC->GetModuleBuff()->Update(pBuff->m_fPeriod);
		CHECK_EQUAL(100, pOtherNPC->GetHateTable().GetHate(m_pPlayer->GetUID()));
		pOtherNPC->GetModuleBuff()->Update(pBuff->m_fPeriod);
		CHECK_EQUAL(1, pOtherNPC->GetHateTable().GetHate(m_pPlayer->GetUID())); // �ּҴ� 1�� ����
		m_pPlayer->DeleteMe();
		m_pField->Update(0.0f);
		CHECK_EQUAL(0, pOtherNPC->GetHateTable().GetHate(m_pPlayer->GetUID()));
	}

	TEST_FIXTURE(FBuff, BuffEffect_TriggeredBuff)
	{
		int nBuffID = 123;
		int nTriggeredBuffID = 321;
		GEntityNPC* pOtherNPC = GUTHelper_NPC::SpawnNPC(m_pField);

		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_TriggeredBuff.nBuffID = nTriggeredBuffID;
		pBuff->m_TriggeredBuff.nTiming = TC_BUFF_EXPIRED;
		pBuff->m_TriggeredBuff.infoEffect.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_TriggeredBuff.infoEffect.m_fRadius = 10*100; // 10m
		pBuff->m_TriggeredBuff.infoEffect.m_nRelation = CSEffectInfo::RELATION_ALL;
		pBuff->m_fDuration = 10.0f;
		pBuff->m_fPeriod = 2.0f;
				
		GBuffInfo* pTriggeredBuff=test.buff.NewBuffInfo(nTriggeredBuffID);
		pTriggeredBuff->m_fDuration = 20.0f;
		pTriggeredBuff->m_fPeriod = 2.0f;

		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
		
		// ���� �ŷ�Ʈ ����
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nTriggeredBuffID));
		CHECK_EQUAL(false, pOtherNPC->HasBuff(nBuffID));
		CHECK_EQUAL(false, pOtherNPC->HasBuff(nTriggeredBuffID));
		m_pPlayer->GetModuleBuff()->Update(pBuff->m_fDuration);
		m_pPlayer->GetModuleBuff()->Update(0.1f);
		pOtherNPC->GetModuleBuff()->Update(0.1f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nTriggeredBuffID));
		CHECK_EQUAL(false, pOtherNPC->HasBuff(nBuffID));
		CHECK_EQUAL(true, pOtherNPC->HasBuff(nTriggeredBuffID));
	}

	TEST_FIXTURE(FBuff, BuffEffect_TriggeredBuffRate)
	{
		int nBuffID = 123;
		int nTriggeredBuffID = 321;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_TriggeredBuff.nBuffID = nTriggeredBuffID;
		pBuff->m_TriggeredBuff.nTiming = TC_BUFF_EXPIRED;
		pBuff->m_TriggeredBuff.infoEffect.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_fDuration = 10.0f;
		pBuff->m_fPeriod = 2.0f;

		GBuffInfo* pTriggeredBuff=test.buff.NewBuffInfo(nTriggeredBuffID);
		pTriggeredBuff->m_fDuration = 20.0f;
		pTriggeredBuff->m_fPeriod = 2.0f;

		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;

		// ���� �ŷ�Ʈ ����
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nTriggeredBuffID));
		m_pPlayer->GetModuleBuff()->Update(pBuff->m_fDuration);
		m_pPlayer->GetModuleBuff()->Update(0.1f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nTriggeredBuffID));

		// Ȯ�� 0%
		pBuff->m_TriggeredBuff.fRate = 0.0f;
		m_pPlayer->GetModuleBuff()->Update(pTriggeredBuff->m_fDuration);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nTriggeredBuffID));
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nTriggeredBuffID));
		m_pPlayer->GetModuleBuff()->Update(pBuff->m_fDuration);
		m_pPlayer->GetModuleBuff()->Update(0.1f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nTriggeredBuffID));
	}

	TEST_FIXTURE(FBuff, BuffRequirement)
	{
		RequireBuff info;
		info.vecIncludeBuffID.push_back(111);
		info.vecIncludeBuffID.push_back(222);
		info.vecExcludeBuffID.push_back(333);
		info.vecExcludeBuffID.push_back(444);
		GBuffRequirement buff_requirement;
		CHECK_EQUAL(false, buff_requirement.ProcessRequirement(m_pPlayer, info));
		GBuffInfo* pBuff1=test.buff.NewBuffInfo(111);
		test.buff.GainBuffSelf(m_pPlayer, pBuff1);
		CHECK_EQUAL(false, buff_requirement.ProcessRequirement(m_pPlayer, info));
		GBuffInfo* pBuff2=test.buff.NewBuffInfo(222);
		test.buff.GainBuffSelf(m_pPlayer, pBuff2);
		CHECK_EQUAL(true, buff_requirement.ProcessRequirement(m_pPlayer, info));
		GBuffInfo* pBuff3=test.buff.NewBuffInfo(333);
		test.buff.GainBuffSelf(m_pPlayer, pBuff3);
		CHECK_EQUAL(false, buff_requirement.ProcessRequirement(m_pPlayer, info));
		GBuffInfo* pBuff4=test.buff.NewBuffInfo(444);
		test.buff.GainBuffSelf(m_pPlayer, pBuff4);
		CHECK_EQUAL(false, buff_requirement.ProcessRequirement(m_pPlayer, info));
	}

	TEST_FIXTURE(FBuff, BuffRequirement_BuffEffect)
	{
		const float DURATION_TIME = 10.0f;

		// �������� �ִ� ���� ����
		GBuffInfo* pBuff=test.buff.NewBuffInfo(123);
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_RequireBuff.vecExcludeBuffID.push_back(111);
		pBuff->m_fDuration = DURATION_TIME;
		pBuff->m_fPeriod = 2.0f;
		pBuff->m_InstantModifier.nHP.nMod = 10;

		// �÷��̾� ��ġ�� ����
		m_pPlayer->GetChrInfo()->nMaxHP = 100;
		m_pPlayer->SetHP(50);

		// ���� ����
		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(123));

		// ó�� ������ �ɸ����� �ֱ��� ȿ���� ����.
		CHECK_EQUAL(m_pPlayer->GetHP(), 60);

		// �ֱ� ȿ��
		m_pField->Update(2.0f);	
		CHECK_EQUAL(m_pPlayer->GetHP(), 70);

		// Exclude ���� ����
		GBuffInfo* pBuff1=test.buff.NewBuffInfo(111);
		pBuff1->m_fDuration = 4.0f; // 4�ʵڿ� Ǯ��
		test.buff.GainBuffSelf(m_pPlayer, pBuff1);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(111));
				
		// �ֱ� ȿ��
		m_pField->Update(2.0f);	
		CHECK_EQUAL(m_pPlayer->GetHP(), 70);

		// �ֱ� ȿ��
		m_pField->Update(2.0f);	
		CHECK_EQUAL(m_pPlayer->GetHP(), 70);

		// �ֱ� ȿ��
		m_pField->Update(2.0f);	
		CHECK_EQUAL(m_pPlayer->GetHP(), 80);

		// �ֱ� ȿ��
		m_pField->Update(2.0f);	
		CHECK_EQUAL(m_pPlayer->GetHP(), 90);
	}

	TEST_FIXTURE(FBuff, BuffRequirement_BuffEffect2)
	{
		const float DURATION_TIME = 10.0f;

		// �������� �ִ� ���� ����
		GBuffInfo* pBuff=test.buff.NewBuffInfo(123);
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_RequireBuff.vecExcludeBuffID.push_back(111);
		pBuff->m_fDuration = DURATION_TIME;
		pBuff->m_fPeriod = 2.0f;
		pBuff->m_InstantModifier.nHP.nMod = 10;

		// �÷��̾� ��ġ�� ����
		m_pPlayer->GetChrInfo()->nMaxHP = 100;
		m_pPlayer->SetHP(50);

		// Exclude ���� ����
		GBuffInfo* pBuff1=test.buff.NewBuffInfo(111);
		pBuff1->m_fDuration = 4.0f; // 4�ʵڿ� Ǯ��
		test.buff.GainBuffSelf(m_pPlayer, pBuff1);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(111));

		// ���� ����
		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(111));
		CHECK_EQUAL(false, m_pPlayer->HasBuff(123)); // �ɸ��� ����
	}

	TEST_FIXTURE(FBuff, BuffRequirement_Keep)
	{
		const float DURATION_TIME = 10.0f;

		// �������� �ִ� ���� ����
		GBuffInfo* pBuff=test.buff.NewBuffInfo(123);
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_RequireBuff.vecIncludeBuffID.push_back(111);
		pBuff->m_RequireBuff.bKeepIncludeBuff = false;
		pBuff->m_fDuration = DURATION_TIME;
		pBuff->m_fPeriod = 2.0f;
		pBuff->m_InstantModifier.nHP.nMod = 10;

		// �÷��̾� ��ġ�� ����
		m_pPlayer->GetChrInfo()->nMaxHP = 100;
		m_pPlayer->SetHP(50);

		// Exclude ���� ����
		GBuffInfo* pBuff1=test.buff.NewBuffInfo(111);
		pBuff1->m_fDuration = 4.0f; // 4�ʵڿ� Ǯ��
		test.buff.GainBuffSelf(m_pPlayer, pBuff1);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(111));

		// ���� ����
		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(111));
		CHECK_EQUAL(true, m_pPlayer->HasBuff(123)); // �ɸ�
	}

	TEST_FIXTURE(FBuff, Buff_Reposte)
	{
		const float DURATION_TIME = 10.0f;

		GEntityPlayer* pOtherPlayer = NewEntityPlayer(m_pField);
		m_pPlayer->GetChrInfo()->nMaxHP = 100;
		m_pPlayer->SetHP(100);
		pOtherPlayer->GetChrInfo()->nMaxHP = 100;
		pOtherPlayer->SetHP(100);

		// �������� �ִ� ���� ����
		GBuffInfo* pBuff=test.buff.NewBuffInfo(123);
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_RequireBuff.vecExcludeBuffID.push_back(111);
		pBuff->m_fDuration = DURATION_TIME;
		pBuff->m_Riposte.nType = TC_MELEE;
		pBuff->m_Riposte.fRate = 2.0f;

		// ���� ����
		test.buff.GainBuffSelf(pOtherPlayer, pBuff);

		// ���� ������
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(10);
		test.talent.UseTalent(m_pPlayer, p1DmgTalent, pOtherPlayer->GetUID());
		CHECK_EQUAL(90, pOtherPlayer->GetHP());
		CHECK_EQUAL(80, m_pPlayer->GetHP());
	}

	TEST_FIXTURE(FBuff, InstantEffect)
	{
		int nBuffID = 123;
		float fDuration = 0.0f;
		GEntityPlayer* pOtherPlayer = NewEntityPlayer(m_pField);
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
		pBuff->m_nMinHeal = 10;
		pBuff->m_nMaxHeal = 10;
		pBuff->m_fDuration = fDuration;

		BeginCommandRecord();
		test.talent.UseTalent(m_pPlayer, pBuffTalent, pOtherPlayer->GetUID());

		// ��Ŷ üũ
		CHECK_EQUAL(1, m_pLinkTester->GetCommandCount());
		CHECK_EQUAL(MC_BUFF_HEAL, m_pLinkTester->GetCommand(0).GetID());
	}

	TEST_FIXTURE(FBuff, Spellpower_Player)
	{
		GEntityPlayer* pPlayer = NewEntityPlayer(m_pField);
		CHECK_EQUAL(1.0f, pPlayer->GetSpellPower());
		GItem* pWeapon = test.item.EquipNewWeapon(pPlayer, WEAPON_1H_SLASH);
		pWeapon->m_pItemData->m_fSpellPower = 2.0f;
		CHECK_EQUAL(2.0f, pPlayer->GetSpellPower()); 
		GItem* pShield =test.item.EquipNewShield(pPlayer);
		pShield->m_pItemData->m_fSpellPower = 4.0f;
		CHECK_EQUAL(3.0f, pPlayer->GetSpellPower()); // ��հ�
	}

	TEST_FIXTURE(FBuff, Spellpower_NPC)
	{
		GEntityNPC* pNPC = GUTHelper_NPC::SpawnNPC(m_pField);
		CHECK_EQUAL(1.0f, pNPC->GetSpellPower());
		pNPC->GetNPCInfo()->m_fSpellPower = 3.0;
		CHECK_EQUAL(3.0f, pNPC->GetSpellPower());
	}

	TEST_FIXTURE(FBuff, CheckDefferentUserUID)
	{
		GBuffInfo* pBuffInfo=test.buff.NewBuffInfo(50);
		pBuffInfo->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
		
		CSBuffEnchantInfo infoEnchant; 
		infoEnchant.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
		infoEnchant.EffectInfo.m_nRelation = CSEffectInfo::RELATION_ALL;
		infoEnchant.nID = pBuffInfo->m_nID;
		GTalentInfo* pBuffTalent = test.buff.NewBuffTalentInfo(INVALID_ID, &infoEnchant);

		GEntityPlayer* pOtherPlayer = NewEntityPlayer(m_pField, vec3(100,100,0));

		// �ٸ� �÷��̾ ���� �ɾ��ֱ�
		{
			test.talent.UseTalent(pOtherPlayer, pBuffTalent, m_pPlayer->GetUID());
			GBuff* pBuff = m_pPlayer->GetModuleBuff()->FindBuffByID(pBuffInfo->m_nID);
			GBuff* pBuff2 = pOtherPlayer->GetModuleBuff()->FindBuffByID(pBuffInfo->m_nID);
			TVALID(pBuff);
			CHECK_EQUAL(pBuff->GetUserUID(), pOtherPlayer->GetUID());
		}
		
		// �ڽ��� ���� �ɱ�
		{
			test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
			GBuff* pBuff = m_pPlayer->GetModuleBuff()->FindBuffByID(pBuffInfo->m_nID);
			TVALID(pBuff);
			CHECK_EQUAL(pBuff->GetUserUID(), m_pPlayer->GetUID());
		}
	}
}
