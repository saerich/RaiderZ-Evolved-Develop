#include "stdafx.h"
#include "GTestForward.h"
#include "GModuleBuff.h"
#include "GPartyManager.h"
#include "GServer.h"
#include "GFieldMgr.h"

SUITE(Buff_Target)
{
	struct FBuff
	{
		FBuff()
		{
			m_pField = test.field.DefaultMockField();
			m_pPlayerTester = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
			m_pLinkTester = test.network.NewLink(m_pPlayerTester);
			sprintf_s(m_pPlayerTester->GetPlayerInfo()->szName, "Tester");

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


		MockField*		m_pField;
		GEntityPlayer*	m_pPlayerTester;
		MockLink*		m_pLinkTester;

		DECLWRAPPER_FieldInfoMgr;
		DECLWRAPPER_FieldMgr;
		DECLWRAPPER_SimpleCombatCalc;
		DECLWRAPPER_BuffMgr;
	};

	TEST_FIXTURE(FBuff, OnSelf)
	{
		int nBuffID = 123;
		// ���� ȿ���� �޴���
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nConditionType = TC_HIT_ME;			// �ڽ��� �¾�����
		pBuff->m_nEffecteeTarget = EFFECTEE_TARGET_SELF;		// �ڱ��ڽſ���
		pBuff->m_InstantModifier.nHP.nMod = 10;					// HP�� 10ȸ��

		// ������ �����ϴ���
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.nEffecteeTarget = EFFECTEE_TARGET_SELF;

		test.talent.UseTalent(m_pPlayerTester, pBuffTalent, m_pPlayerTester->GetUID());
		CHECK_EQUAL(true, m_pPlayerTester->HasBuff(nBuffID));

		// ���� �̺�Ʈ
		GEntityPlayer* pAttacker = test.player.NewEntityPlayer(m_pField, m_pPlayerTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		m_pPlayerTester->SetHP(10);
		pAttacker->SetHP(10);
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pPlayerTester->GetUID());
		CHECK_EQUAL(m_pPlayerTester->GetHP(), 10-1+10);	// 1���� �԰� HP10ȸ��
		CHECK_EQUAL(pAttacker->GetHP(), 10);
	}

	TEST_FIXTURE(FBuff, OnSelfParty)
	{
		int nBuffID = 123;
		// ���� ȿ���� �޴���
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nConditionType = TC_HIT_ME;			// �ڽ��� �¾�����
		pBuff->m_nEffecteeTarget = EFFECTEE_TARGET_SELF_PARTY;	// �ڱ� �ڽ��� ��Ƽ���鿡��
		pBuff->m_EffectRadius = 100.0f;							// ȿ������ 100m
		pBuff->m_InstantModifier.nHP.nMod = 10;					// HP�� 10ȸ��

		// ������ �����ϴ���
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.nEffecteeTarget = EFFECTEE_TARGET_SELF;

		test.talent.UseTalent(m_pPlayerTester, pBuffTalent, m_pPlayerTester->GetUID());
		CHECK_EQUAL(true, m_pPlayerTester->HasBuff(nBuffID));

		GEntityPlayer* pPartyMember = test.player.NewEntityPlayer(m_pField, m_pPlayerTester->GetPos()+vec3(950,10,0));
		test.party.CreateParty(m_pPlayerTester, pPartyMember);
		CHECK_EQUAL(m_pPlayerTester->GetPartyUID(), pPartyMember->GetPartyUID());

		// ���� �̺�Ʈ
		GEntityPlayer* pAttacker = test.player.NewEntityPlayer(m_pField, m_pPlayerTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		m_pPlayerTester->SetHP(10);
		pAttacker->SetHP(10);
		pPartyMember->SetHP(10);
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pPlayerTester->GetUID());
		CHECK_EQUAL(m_pPlayerTester->GetHP(), 10-1+10);
		CHECK_EQUAL(pAttacker->GetHP(), 10);
		CHECK_EQUAL(pPartyMember->GetHP(), 10);			// �Ÿ��� �־ ��Ƽ���ӿ��� ȿ���� ���� ����
	}

	TEST_FIXTURE(FBuff, OnTarget)
	{
		int nBuffID = 123;
		// ���� ȿ���� �޴���
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nConditionType = TC_HIT_ME;		// �ڽ��� �¾�����
		pBuff->m_nEffecteeTarget = EFFECTEE_TARGET_TARGET;	// ����ڿ���
		pBuff->m_InstantModifier.nHP.nMod = 10;				// HP�� 10ȸ��

		// ������ �����ϴ���
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.nEffecteeTarget = EFFECTEE_TARGET_TARGET;

		test.talent.UseTalent(m_pPlayerTester, pBuffTalent, m_pPlayerTester->GetUID());
		CHECK_EQUAL(true, m_pPlayerTester->HasBuff(nBuffID));

		// ���� �̺�Ʈ
		GEntityPlayer* pAttacker = test.player.NewEntityPlayer(m_pField, m_pPlayerTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		m_pPlayerTester->SetHP(10);
		pAttacker->SetHP(10);
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pPlayerTester->GetUID());
		CHECK_EQUAL(m_pPlayerTester->GetHP(), 10-1);
		CHECK_EQUAL(pAttacker->GetHP(), 10+10);
	}

	TEST_FIXTURE(FBuff, OnTargetParty)
	{
		int nBuffID = 123;
		// ���� ȿ���� �޴���
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nConditionType = TC_HIT_ME;				// �ڽ��� �¾�����
		pBuff->m_nEffecteeTarget = EFFECTEE_TARGET_TARGET_PARTY;	// ������� ��Ƽ���鿡��
		pBuff->m_EffectRadius = 100.0f;								// ȿ������ 100m
		pBuff->m_InstantModifier.nHP.nMod = 10;						// HP�� 10ȸ��

		// ������ �����ϴ���
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.nEffecteeTarget = EFFECTEE_TARGET_TARGET;

		test.talent.UseTalent(m_pPlayerTester, pBuffTalent, m_pPlayerTester->GetUID());

		GEntityPlayer* pAttacker = test.player.NewEntityPlayer(m_pField, m_pPlayerTester->GetPos()+vec3(10,10,0));
		GEntityPlayer* pPartyMember = test.player.NewEntityPlayer(m_pField, pAttacker->GetPos()+vec3(950,10,0));
		test.party.CreateParty(pAttacker, pPartyMember);
		CHECK_EQUAL(pAttacker->GetPartyUID(), pPartyMember->GetPartyUID());

		// ���� �̺�Ʈ		
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		m_pPlayerTester->SetHP(10);
		pAttacker->SetHP(10);
		pPartyMember->SetHP(10);
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pPlayerTester->GetUID());
		CHECK_EQUAL(m_pPlayerTester->GetHP(), 10-1);
		CHECK_EQUAL(pAttacker->GetHP(), 10+10);
		CHECK_EQUAL(pPartyMember->GetHP(), 10);			// �Ÿ��� �־ ��Ƽ���ӿ��� ȿ���� ���� ����
	}

	TEST_FIXTURE(FBuff, OnAreaEnemy)
	{
		int nBuffID = 123;
		// ���� ȿ���� �޴���
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nConditionType = TC_HIT_ME;				// �ڽ��� �¾�����
		pBuff->m_nEffecteeTarget = EFFECTEE_TARGET_AREA_ENEMY;		// ������ ������ ����ڿ���
		pBuff->m_EffectRadius = 100.0f;								// ȿ������ 100m
		pBuff->m_InstantModifier.nHP.nMod = 10;						// HP�� 10ȸ��

		// ������ �����ϴ���
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.nEffecteeTarget = EFFECTEE_TARGET_SELF;

		test.talent.UseTalent(m_pPlayerTester, pBuffTalent, m_pPlayerTester->GetUID());
		CHECK_EQUAL(true, m_pPlayerTester->HasBuff(nBuffID));

		GEntityNPC* pNearNPC = test.npc.SpawnTestNPC(m_pField);
		pNearNPC->SetPos(m_pPlayerTester->GetPos());
		GEntityNPC* pFarNPC = test.npc.SpawnTestNPC(m_pField);
		pFarNPC->SetPos(m_pPlayerTester->GetPos()+vec3(1000,1000,1000));

		// ���� �̺�Ʈ
		GEntityPlayer* pAttacker = test.player.NewEntityPlayer(m_pField, m_pPlayerTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		m_pPlayerTester->SetHP(10);
		pAttacker->SetHP(10);
		pNearNPC->SetHP(10);
		pFarNPC->SetHP(10);
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pPlayerTester->GetUID());
		CHECK_EQUAL(m_pPlayerTester->GetHP(), 10-1);	// 1���� ����
		CHECK_EQUAL(pAttacker->GetHP(), 10);
		CHECK_EQUAL(pNearNPC->GetHP(), 10-1+10); // 10ȸ����
		CHECK_EQUAL(pFarNPC->GetHP(), 10); // 10ȸ����
	}

	TEST_FIXTURE(FBuff, OnAreaAllied)
	{
		int nBuffID = 123;
		// ���� ȿ���� �޴���
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nConditionType = TC_HIT_ME;	// �ڽ��� �¾�����
		pBuff->m_nEffecteeTarget = EFFECTEE_TARGET_AREA_ALLIED;// ������ ��ȣ�� ����ڿ���
		pBuff->m_EffectRadius = 100.0f;					// ȿ������ 100m
		pBuff->m_InstantModifier.nHP.nMod = 10;			// HP�� 10ȸ��

		// ������ �����ϴ���
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.nEffecteeTarget = EFFECTEE_TARGET_SELF;

		test.talent.UseTalent(m_pPlayerTester, pBuffTalent, m_pPlayerTester->GetUID());
		CHECK_EQUAL(true, m_pPlayerTester->HasBuff(nBuffID));

		GEntityPlayer* pNearPlayer = test.player.NewEntityPlayer(m_pField, m_pPlayerTester->GetPos()+vec3(50,0,0));
		GEntityPlayer* pFarPlayer = test.player.NewEntityPlayer(m_pField, m_pPlayerTester->GetPos()+vec3(1000,1000,1000));

		// ���� �̺�Ʈ
		GEntityPlayer* pAttacker = test.player.NewEntityPlayer(m_pField, m_pPlayerTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		m_pPlayerTester->SetHP(10);
		pAttacker->SetHP(10);
		pNearPlayer->SetHP(10);
		pFarPlayer->SetHP(10);
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pPlayerTester->GetUID());
		CHECK_EQUAL(m_pPlayerTester->GetHP(), 10-1+10);	// 10ȸ����(�����ڵ� ��ȣ�� �÷��̾��̹Ƿ�)
		CHECK_EQUAL(pAttacker->GetHP(), 10+10);	// 10ȸ����(�����ڵ� ��ȣ�� �÷��̾��̹Ƿ�)
		CHECK_EQUAL(pNearPlayer->GetHP(), 10-1+10); // 10ȸ����
		CHECK_EQUAL(pFarPlayer->GetHP(), 10); // 10ȸ����
	}

	TEST_FIXTURE(FBuff, OnAreaAll)
	{
		int nBuffID = 123;
		// ���� ȿ���� �޴���
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nConditionType = TC_HIT_ME;	// �ڽ��� �¾�����
		pBuff->m_nEffecteeTarget = EFFECTEE_TARGET_AREA_ALL;// ������ ��� ����ڿ���
		pBuff->m_EffectRadius = 100.0f;					// ȿ������ 100m
		pBuff->m_InstantModifier.nHP.nMod = 10;			// HP�� 10ȸ��

		// ������ �����ϴ���
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.nEffecteeTarget = EFFECTEE_TARGET_TARGET;
		
		test.talent.UseTalent(m_pPlayerTester, pBuffTalent, m_pPlayerTester->GetUID());
		CHECK_EQUAL(true, m_pPlayerTester->HasBuff(nBuffID));

		GEntityPlayer* pNearPlayer = test.player.NewEntityPlayer(m_pField, m_pPlayerTester->GetPos()+vec3(50,0,0));
		GEntityPlayer* pFarPlayer = test.player.NewEntityPlayer(m_pField, m_pPlayerTester->GetPos()+vec3(1000,1000,1000));
		GEntityNPC* pNearNPC = test.npc.SpawnTestNPC(m_pField);
		pNearNPC->SetPos(m_pPlayerTester->GetPos());
		GEntityNPC* pFarNPC = test.npc.SpawnTestNPC(m_pField);
		pFarNPC->SetPos(m_pPlayerTester->GetPos()+vec3(1000,1000,1000));

		// ���� �̺�Ʈ
		GEntityPlayer* pAttacker = test.player.NewEntityPlayer(m_pField, m_pPlayerTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		m_pPlayerTester->SetHP(10);
		pAttacker->SetHP(10);
		pNearPlayer->SetHP(10);
		pFarPlayer->SetHP(10);
		pNearNPC->SetHP(10);
		pFarNPC->SetHP(10);
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pPlayerTester->GetUID());
		CHECK_EQUAL(m_pPlayerTester->GetHP(), 10-1+10);	// 10ȸ����(�����ڵ� ��ȣ�� �÷��̾��̹Ƿ�)
		CHECK_EQUAL(pAttacker->GetHP(), 10+10);	// 10ȸ����(�����ڵ� ��ȣ�� �÷��̾��̹Ƿ�)
		CHECK_EQUAL(pNearPlayer->GetHP(), 10-1+10); // 10ȸ����
		CHECK_EQUAL(pFarPlayer->GetHP(), 10); // 10ȸ����
		CHECK_EQUAL(pNearNPC->GetHP(), 10-1+10); // 10ȸ����
		CHECK_EQUAL(pFarNPC->GetHP(), 10); // 10ȸ����
	}
}