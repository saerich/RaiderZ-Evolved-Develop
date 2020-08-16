#include "stdafx.h"
#include "GEntityNPC.h"
#include "GUnitTestUtil.h"
#include "GDropList.h"
#include "GNPCLoot.h"
#include "GUTHelper.h"
#include "GLootInfo.h"
#include "MockLink.h"
#include "CCommandTable.h"
#include "GNPCRewarder.h"
#include "GTestForward.h"

SUITE(EntityNPC)
{
	class GTestEntityNPC : public GEntityNPC
	{
	protected:
		GActorMeshInfo		m_ActorMeshInfo;
	public:
		GTestEntityNPC() : GEntityNPC() {}
		virtual ~GTestEntityNPC() {}

		virtual GActorMeshInfo* GetMeshInfo() override 
		{
			return &m_ActorMeshInfo;
		}
	
		void SetColRadius_ForTest(float fColRadius)
		{
			m_ActorMeshInfo.SetRadius(fColRadius, fColRadius);
		}
		GActorMeshInfo& GetActorMeshInfo() { return m_ActorMeshInfo; }
	};

	class FEntityNPC
	{
	public:
		FEntityNPC()
		{
			m_NPCInfo.nID = 1;

			m_pEntityNPC = new GTestEntityNPC();
			m_pEntityNPC->SetNPCInfo(&m_NPCInfo);
		}
		~FEntityNPC()
		{
			delete m_pEntityNPC;
		}

		GTestEntityNPC*		m_pEntityNPC;
		GNPCInfo			m_NPCInfo;
		MUID				m_uidNPC;

		GUTHelper			m_helper;
	};

	TEST_FIXTURE(FEntityNPC, TestEntityNPC_SetColRadius_ForTest)
	{
		m_pEntityNPC->SetColRadius_ForTest(200.0f);
		CHECK_EQUAL(200.0f, m_pEntityNPC->GetColRadius());

		m_pEntityNPC->SetColRadius_ForTest(1000.0f);
		CHECK_EQUAL(1000.0f, m_pEntityNPC->GetColRadius());

		m_pEntityNPC->SetColRadius_ForTest(0.0f);
		CHECK_EQUAL(0.0f, m_pEntityNPC->GetColRadius());
	}

	TEST_FIXTURE(FEntityNPC, TestEntityNPC_GetValidInteractionDistance)
	{
		// 1��
		float fColRadius = 100.0f;
		m_pEntityNPC->SetColRadius_ForTest(fColRadius);
		float fValidDist = m_pEntityNPC->GetValidInteractionDistance();
		CHECK_EQUAL((fColRadius * GAME_INTERACTION_DISTANCE_XY_MULTIPLICATION) + GAME_INTERACTION_DISTANCE_XY_ADDITION , fValidDist);

		// 2��
		fColRadius = 50.0f;
		m_pEntityNPC->SetColRadius_ForTest(fColRadius);
		fValidDist = m_pEntityNPC->GetValidInteractionDistance();
		CHECK_EQUAL((fColRadius * GAME_INTERACTION_DISTANCE_XY_MULTIPLICATION) + GAME_INTERACTION_DISTANCE_XY_ADDITION , fValidDist);

		// 3��
		fColRadius = 500.0f;
		m_pEntityNPC->SetColRadius_ForTest(fColRadius);
		fValidDist = m_pEntityNPC->GetValidInteractionDistance();
		CHECK_EQUAL((fColRadius * GAME_INTERACTION_DISTANCE_XY_MULTIPLICATION) + GAME_INTERACTION_DISTANCE_XY_ADDITION , fValidDist);
	}

	TEST_FIXTURE(FEntityNPC, TestEntityNPC_GetValidInteractionDistance_NPCInfoInteractionDistance)
	{
		// 1��
		m_NPCInfo.nInteractionDistance = 1;
		float fColRadius = 100.0f;
		m_pEntityNPC->SetColRadius_ForTest(fColRadius);
		float fValidDist = m_pEntityNPC->GetValidInteractionDistance();
		CHECK_EQUAL(1.0f, fValidDist);

		// 2��
		m_NPCInfo.nInteractionDistance = 1000;
		fValidDist = m_pEntityNPC->GetValidInteractionDistance();
		CHECK_EQUAL(1000.0f, fValidDist);

		// 3��
		m_NPCInfo.nInteractionDistance = 10;
		fValidDist = m_pEntityNPC->GetValidInteractionDistance();
		CHECK_EQUAL(10.0f, fValidDist);
	}

	TEST_FIXTURE(FEntityNPC, DoRewardByGather)
	{
		m_pEntityNPC->GetNPCInfo()->m_bEnableAILod = false;

		GEntityPlayer* pGatherer = m_helper.NewEntityPlayer();
		MockLink* pLink = m_helper.NewLink(pGatherer);
		int nCID = pGatherer->GetCID();

		// 100% Ȯ�� ��� ������ ����
		GLootInfo* pLootInfo = test.loot.NewLootInfo();
		LOOT_ITEM_INFO* pLootItemInfo = test.loot.NewLootItemInfo(pLootInfo, 100.0f, 1, 10);		
		int nLootID = pLootInfo->m_nID;


		// ä��
		m_pEntityNPC->GetNPCRewarder().RewardByGather(pGatherer, nLootID, m_pEntityNPC);

		
		// ���ñ��� Ȯ��
		GNPCLoot& npcLoot = m_pEntityNPC->GetNPCLoot();
		GDropList& dropList = npcLoot.GetDropList();
		
		CHECK(dropList.IsDropByGather());
		CHECK(dropList.IsAuthorizedCID(nCID));
		CHECK(dropList.IsViewableCID(nCID));

		set<int> setVisibleCID;
		dropList.GetViewableCID(setVisibleCID);

		CHECK_EQUAL(1, setVisibleCID.size());
		CHECK(setVisibleCID.end() != setVisibleCID.find(nCID));
	

		// �������� ���� Ȯ��
		CHECK_EQUAL(MC_LOOT_INSERT_LOOTABLE_NPC, pLink->GetCommandID(0));
		CHECK_EQUAL(m_pEntityNPC->GetUID(), pLink->GetParam<MUID>(0, 0));
	}
}
