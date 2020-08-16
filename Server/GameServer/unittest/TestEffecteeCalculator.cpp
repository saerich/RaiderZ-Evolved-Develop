#include "StdAfx.h"
#include "GTestForward.h"
#include "GEntityPlayer.h"
#include "GModuleCombat.h"
#include "GFieldMgr.h"

SUITE(EffecteeCalculator)
{
	struct FEffecteeCalculator
	{
		FEffecteeCalculator()
		{ 
			m_pField = test.field.DefaultMockField();
			m_pTester = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
			sprintf_s(m_pTester->GetPlayerInfo()->szName, "Tester");
		}

		~FEffecteeCalculator() 
		{
			m_pField->Destroy();
			test.network.ClearLinks();
		}

		MockField*		m_pField;
		GEntityPlayer*	m_pTester;
		GEffecteeTargetSelector m_EffecteeTargetSelector;

		DECLWRAPPER_Field;
	};

	TEST_FIXTURE(FEffecteeCalculator, OnSelf)
	{
		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, m_pTester, EFFECTEE_TARGET_SELF, NULL, vec3::ZERO, 0.0f);

		TVALID(!vecResults.empty());
		CHECK_EQUAL(vecResults[0]->GetUID(), m_pTester->GetUID());
	}

	TEST_FIXTURE(FEffecteeCalculator, OnVictim)
	{
		GEntityPlayer* pVictim = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, m_pTester, EFFECTEE_TARGET_TARGET, pVictim, vec3::ZERO, 0.0f);

		TVALID(!vecResults.empty());
		CHECK_EQUAL(vecResults[0]->GetUID(), pVictim->GetUID());
	}

	TEST_FIXTURE(FEffecteeCalculator, OnVictim_TargetOnSelf)
	{
		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, m_pTester, EFFECTEE_TARGET_TARGET, m_pTester, vec3::ZERO, 0.0f);

		TVALID(!vecResults.empty());
		CHECK_EQUAL(vecResults[0]->GetUID(), m_pTester->GetUID());
	}

	TEST_FIXTURE(FEffecteeCalculator, OnParty_TargetOnSelf)
	{
		GEntityPlayer* pMember1 = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		GEntityPlayer* pMember2 = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		GEntityPlayer* pMember3 = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		test.party.CreateParty(m_pTester, pMember1);
		test.party.CreateParty(m_pTester, pMember2);
		test.party.CreateParty(m_pTester, pMember3);
		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, m_pTester, EFFECTEE_TARGET_SELF_PARTY, m_pTester, vec3::ZERO, 100.0f);

		TVALID_EQ(vecResults.size(), 4);	// tester + member1,2,3
		CHECK_EQUAL(vecResults[0]->GetUID(), m_pTester->GetUID());
		CHECK_EQUAL(vecResults[1]->GetUID(), pMember1->GetUID());
		CHECK_EQUAL(vecResults[2]->GetUID(), pMember2->GetUID());
		CHECK_EQUAL(vecResults[3]->GetUID(), pMember3->GetUID());
	}

	TEST_FIXTURE(FEffecteeCalculator, OnParty_TargetOnMember)
	{
		GEntityPlayer* pMember1 = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		GEntityPlayer* pMember2 = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		GEntityPlayer* pMember3 = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		test.party.CreateParty(m_pTester, pMember1);
		test.party.CreateParty(m_pTester, pMember2);
		test.party.CreateParty(m_pTester, pMember3);
		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, m_pTester, EFFECTEE_TARGET_SELF_PARTY, pMember1, vec3::ZERO, 100.0f);

		TVALID_EQ(vecResults.size(), 4);	// tester + member1,2,3
		CHECK_EQUAL(vecResults[0]->GetUID(), m_pTester->GetUID());
		CHECK_EQUAL(vecResults[1]->GetUID(), pMember1->GetUID());
		CHECK_EQUAL(vecResults[2]->GetUID(), pMember2->GetUID());
		CHECK_EQUAL(vecResults[3]->GetUID(), pMember3->GetUID());
	}

	TEST_FIXTURE(FEffecteeCalculator, OnParty_TargetOtherPartyLeader)
	{
		GEntityPlayer* pLeader = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		GEntityPlayer* pMember1 = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		GEntityPlayer* pMember2 = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		test.party.CreateParty(pLeader, pMember1);
		test.party.CreateParty(pLeader, pMember2);

		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, m_pTester, EFFECTEE_TARGET_TARGET_PARTY, pLeader, vec3::ZERO, 100.0f);

		// �ٸ� ��Ƽ�� ������ �� ����, �ڱ��ڽŸ� �ɸ�
		TVALID_EQ(vecResults.size(), 3);
		CHECK_EQUAL(vecResults[0]->GetUID(), pLeader->GetUID());
		CHECK_EQUAL(vecResults[1]->GetUID(), pMember1->GetUID());
		CHECK_EQUAL(vecResults[2]->GetUID(), pMember2->GetUID());
	}

	TEST_FIXTURE(FEffecteeCalculator, OnParty_TargetOtherPartyMember)
	{
		GEntityPlayer* pLeader = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		GEntityPlayer* pMember1 = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		GEntityPlayer* pMember2 = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		test.party.CreateParty(pLeader, pMember1);
		test.party.CreateParty(pLeader, pMember2);

		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, m_pTester, EFFECTEE_TARGET_TARGET_PARTY, pMember1, vec3::ZERO, 100.0f);

		// �ٸ� ��Ƽ�� ������ �� ����, �ڱ��ڽŸ� �ɸ�
		TVALID_EQ(vecResults.size(), 3);
		CHECK_EQUAL(vecResults[0]->GetUID(), pLeader->GetUID());
		CHECK_EQUAL(vecResults[1]->GetUID(), pMember1->GetUID());
		CHECK_EQUAL(vecResults[2]->GetUID(), pMember2->GetUID());
	}

	TEST_FIXTURE(FEffecteeCalculator, OnParty_WithoutParty)
	{
		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, m_pTester, EFFECTEE_TARGET_SELF_PARTY, m_pTester, vec3::ZERO, 100.0f);

		// ��Ƽ�� ���ٸ�, �ڱ��ڽŸ� �ɸ�
		TVALID_EQ(vecResults.size(), 1);
		CHECK_EQUAL(vecResults[0]->GetUID(), m_pTester->GetUID());
	}

	TEST_FIXTURE(FEffecteeCalculator, OnAreaAll)
	{
		GEntityPlayer* pMember = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		test.party.CreateParty(m_pTester, pMember);
		GEntityPlayer* pMan = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		GEntityNPC* pMob = test.npc.SpawnTestNPC(m_pField);
		pMob->SetPos(vec3(100,100,0));

		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, m_pTester, EFFECTEE_TARGET_AREA_ALL, NULL, vec3(10,10,10), 1000.0f);

		// ��ΰ� �ɸ�
		TVALID_EQ(vecResults.size(), 4);	// pMember + pMan + pMob + m_pTester
	}

	TEST_FIXTURE(FEffecteeCalculator, OnAreaAll__NoPoint)	// ���� ���Ϳ� ���� ��ǥ ���� ã�� ���� ����� �߽����� ã��
	{
		m_pTester->SetPos(vec3(10000,100,0));
		GEntityPlayer* pMember = test.player.NewEntityPlayer(m_pField, vec3(10000,100,0));
		test.party.CreateParty(m_pTester, pMember);
		GEntityPlayer* pMan = test.player.NewEntityPlayer(m_pField, vec3(10000,100,0));
		GEntityNPC* pMob = test.npc.SpawnTestNPC(m_pField);
		pMob->SetPos(vec3(10000,100,0));

		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, m_pTester, EFFECTEE_TARGET_AREA_ALL, NULL, vec3::ZERO, 1000.0f);

		// ��ΰ� �ɸ�
		TVALID_EQ(vecResults.size(), 4);	// pMember + pMan + pMob + m_pTester
	}

	TEST_FIXTURE(FEffecteeCalculator, OnEnemy_Player)
	{
		GEntityPlayer* pPlayer = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		GEntityNPC* pNPC = test.npc.SpawnTestNPC(m_pField);
		pNPC->SetPos(vec3(100,100,0));

		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, m_pTester, EFFECTEE_TARGET_AREA_ENEMY, NULL, vec3(10,10,10), 1000.0f);

		// �÷��̾��� �������� NPC�� �ɸ�
		TVALID_EQ(vecResults.size(), 1);
		CHECK_EQUAL(vecResults[0]->GetUID(), pNPC->GetUID());
	}

	TEST_FIXTURE(FEffecteeCalculator, OnEnemy_NPC)
	{
		GEntityNPC* pNPC = test.npc.SpawnTestNPC(m_pField);
		pNPC->SetPos(vec3(100,100,0));

		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, pNPC, EFFECTEE_TARGET_AREA_ENEMY, NULL, vec3(10,10,10), 1000.0f);

		// NPC�� �������� �÷��̾ �ɸ�
		TVALID_EQ(vecResults.size(), 1);
		CHECK_EQUAL(vecResults[0]->GetUID(), m_pTester->GetUID());
	}

	TEST_FIXTURE(FEffecteeCalculator, OnAllied_Player)
	{
		GEntityNPC* pNPC = test.npc.SpawnTestNPC(m_pField);
		pNPC->SetPos(vec3(100,100,0));
		GEntityPlayer* pMember = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		test.party.CreateParty(m_pTester, pMember);
		GEntityPlayer* pMan = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));

		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, m_pTester, EFFECTEE_TARGET_AREA_ALLIED, NULL, vec3(10,10,10), 1000.0f);

		// �α��� ��� �÷��̾ �ɸ�
		TVALID_EQ(vecResults.size(), 3);
		CHECK_EQUAL(vecResults[0]->GetUID(), m_pTester->GetUID());
		CHECK_EQUAL(vecResults[1]->GetUID(), pMember->GetUID());
		CHECK_EQUAL(vecResults[2]->GetUID(), pMan->GetUID());
	}

	TEST_FIXTURE(FEffecteeCalculator, OnAllied_NPC)
	{
		GEntityNPC* pNPC = test.npc.SpawnTestNPC(m_pField);
		pNPC->SetPos(vec3(100,100,0));

		GEntityNPC* pOtherNPC = test.npc.SpawnTestNPC(m_pField);
		pOtherNPC->SetPos(vec3(100,100,0));

		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, pNPC, EFFECTEE_TARGET_AREA_ALLIED, NULL, vec3(10,10,10), 1000.0f);

		// ��� NPC�� NPC�� ��ȣ���� 
		TVALID_EQ(vecResults.size(), 2); //pNPC+pOtherNPC
		CHECK_EQUAL(vecResults[0]->GetUID(), pNPC->GetUID());
		CHECK_EQUAL(vecResults[1]->GetUID(), pOtherNPC->GetUID());
	}


	TEST_FIXTURE(FEffecteeCalculator, AreaAll_RadiusZero)
	{
		GEntityPlayer* pMember = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		test.party.CreateParty(m_pTester, pMember);
		GEntityPlayer* pMan = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		GEntityNPC* pMob = test.npc.SpawnTestNPC(m_pField);
		pMob->SetPos(vec3(100,100,0));

		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, m_pTester, EFFECTEE_TARGET_AREA_ALL, NULL, vec3(10,10,10), 0.0f);

		// ��ȿ������ 0�ϰ��� �ڱ��ڽŸ� ���õȴ�.
		TVALID_EQ(vecResults.size(), 1);
		CHECK_EQUAL(vecResults[0]->GetUID(), m_pTester->GetUID());
	}

	TEST_FIXTURE(FEffecteeCalculator, AreaAllied_RadiusZero)
	{
		GEntityPlayer* pMember = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		test.party.CreateParty(m_pTester, pMember);
		GEntityPlayer* pMan = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		GEntityNPC* pMob = test.npc.SpawnTestNPC(m_pField);
		pMob->SetPos(vec3(100,100,0));

		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, m_pTester, EFFECTEE_TARGET_AREA_ALLIED, NULL, vec3(10,10,10), 0.0f);

		// ��ȿ������ 0�ϰ��� �ڱ��ڽŸ� ���õȴ�.
		TVALID_EQ(vecResults.size(), 1);
		CHECK_EQUAL(vecResults[0]->GetUID(), m_pTester->GetUID());
	}

	TEST_FIXTURE(FEffecteeCalculator, AreaEnemy_RadiusZero)
	{
		GEntityPlayer* pMember = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		test.party.CreateParty(m_pTester, pMember);
		GEntityPlayer* pMan = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		GEntityNPC* pMob = test.npc.SpawnTestNPC(m_pField);
		pMob->SetPos(vec3(100,100,0));

		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, m_pTester, EFFECTEE_TARGET_AREA_ENEMY, NULL, vec3(10,10,10), 0.0f);

		// ��ȿ������ 0�ϰ��� �ڱ��ڽŸ� ���õȴ�.
		TVALID_EQ(vecResults.size(), 1);
		CHECK_EQUAL(vecResults[0]->GetUID(), m_pTester->GetUID());
	}

	TEST_FIXTURE(FEffecteeCalculator, Party_RadiusZero)
	{
		GEntityPlayer* pMember1 = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		GEntityPlayer* pMember2 = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		GEntityPlayer* pMember3 = test.player.NewEntityPlayer(m_pField, vec3(100,100,0));
		test.party.CreateParty(m_pTester, pMember1);
		test.party.CreateParty(m_pTester, pMember2);
		test.party.CreateParty(m_pTester, pMember3);
		vector<GEntityActor*>  vecResults =
			m_EffecteeTargetSelector.Select(m_pField, m_pTester, EFFECTEE_TARGET_SELF_PARTY, m_pTester, vec3::ZERO, 0.0f);

		// ��ȿ������ 0�ϰ��� �ڱ��ڽŸ� ���õȴ�.
		TVALID_EQ(vecResults.size(), 1);
		CHECK_EQUAL(vecResults[0]->GetUID(), m_pTester->GetUID());
	}

}