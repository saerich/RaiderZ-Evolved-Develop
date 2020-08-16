#include "stdafx.h"
#include "GUTHelper.h"
#include "GEntityPlayer.h"
#include "MockField.h"
#include "GPlayerFactions.h"
#include "GFactionInfo.h"
#include "GFactionSystem.h"
#include "CSFactionCalculator.h"
#include "MockLink.h"
#include "CCommandTable.h"
#include "GDuel.h"
#include "GBattleArenaMgr.h"
#include "GMatchRule_SameTeamMember.h"
#include "GConst.h"
#include "GPlayerBattleArena.h"
#include "GEntityNPC.h"
#include "GTestForward.h"
#include "GHateTable.h"
#include "GConfig.h"
#include "GRelationSelector.h"
#include "GTestForward.h"
#include "GUTHelper_Duel.h"
#include "GUTHelper_Field.h"
#include "GGameRule_DeathMatch.h"


SUITE(RelationSelector)
{
	struct Fixture
	{
		Fixture()
		{
			m_pField = GUTHelper_Field::DefaultMockField();
			m_pPlayer = GUTHelper::NewEntityPlayer(m_pField);
			m_pFactionInfo =  test.faction.NewFactionInfo(CSFactionCalculator::GetNormalMaxQuantity());
			GConfig::m_strAutoTestType = L"";
		}

		virtual ~Fixture()
		{
			m_pField->Destroy();
		}

		GUTHelper			m_Helper;
		MockField*			m_pField;
		GEntityPlayer*		m_pPlayer;
		GFactionInfo*		m_pFactionInfo;
		GRelationSelector	m_RelationSelector;
	};	

	namespace ReqPlayerTarPlayer
	{
		// ����� �������� �÷��̾�
		TEST_FIXTURE(Fixture, TarIsDuelPlayer)
		{
			GEntityPlayer* pPlayer2 = m_Helper.NewEntityPlayer(m_pField);
			GUTHelper_Duel::NewMockDuel_Fighting(m_pPlayer, pPlayer2);

			CHECK_EQUAL(RT_ENEMY, m_RelationSelector.Select(m_pPlayer, pPlayer2));
		}

		// ����� ��Ʋ�Ʒ������� ���� �÷��̾�
		TEST_FIXTURE(Fixture, TarIsBattleArenaPlayer)
		{
			GEntityPlayer* pPlayer2 = m_Helper.NewEntityPlayer(m_pField);
			
			GUTHelper_Party::CreateParty(m_pPlayer);
			GUTHelper_Party::CreateParty(pPlayer2);

			gmgr.pBattleArenaMgr->AddGameRule(new GGameRule_DeathMatch());
			gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
			GFieldInfo* pBattleArenaFieldInfo = GUTHelper_Field::NewFieldInfo(GConst::ARENA_FIELD_ID);
			FIELD_GROUP_INFO* pBattleArenaFieldGroupInfo = GUTHelper_Field::NewFieldGroupInfo(pBattleArenaFieldInfo, GConst::ARENA_FIELDGROUP_ID);
			GUTHelper_Field::InsertNewMarkerInfo(pBattleArenaFieldInfo, 1);
			GUTHelper_Field::InsertNewMarkerInfo(pBattleArenaFieldInfo, 2);

			gmgr.pBattleArenaMgr->RegisterTeam(m_pPlayer);
			gmgr.pBattleArenaMgr->RegisterTeam(pPlayer2);

			GBattleArena* pBattleArena = gmgr.pBattleArenaMgr->FindBattleArena(m_pPlayer->GetPlayerBattleArena().GetBattleArenaUID());
			pBattleArena->ChangeStatus(GBattleArena::PLAYING);

			CHECK_EQUAL(RT_ENEMY, m_RelationSelector.Select(m_pPlayer, pPlayer2));
		}		

		// ����� �׾��ִ� �÷��̾�
		TEST_FIXTURE(Fixture, TarIsNormalDeadPlayer)
		{
			GEntityPlayer* pPlayer2 = m_Helper.NewEntityPlayer(m_pField);
			pPlayer2->doDie();
			CHECK_EQUAL(RT_ALLYDEAD, m_RelationSelector.Select(m_pPlayer, pPlayer2));
		}

		// ����� ��Ƽ���� �÷��̾�
		TEST_FIXTURE(Fixture, TarIsPartyPlayer)
		{
			GEntityPlayer* pPlayer2 = m_Helper.NewEntityPlayer(m_pField);			
			GUTHelper_Party::CreateParty(m_pPlayer, pPlayer2);

			CHECK_EQUAL(RT_PARTY, m_RelationSelector.Select(m_pPlayer, pPlayer2));
		}

		// ����� �Ϲ����� �÷��̾�
		TEST_FIXTURE(Fixture, TarIsNormalPlayer)
		{
			GEntityPlayer* pPlayer2 = m_Helper.NewEntityPlayer(m_pField);
			CHECK_EQUAL(RT_ALLY, m_RelationSelector.Select(m_pPlayer, pPlayer2));
		}
	}

	namespace ReqPlayerTarNPC
	{
		// ����� ����÷��� NPC
		TEST_FIXTURE(Fixture, TarIsDuelFlagNPC)
		{
			GEntityNPC* pNPC = m_Helper.NewEntityNPC(m_pField, vec3(1.0f,1.0f,1.0f), DUEL_FLAG_NPCID);

			CHECK_EQUAL(RT_NONE, m_RelationSelector.Select(m_pPlayer, pNPC));
		}

		// ����� �׻� ���� ������ �ִ� NPC
		TEST_FIXTURE(Fixture, TarIsAlwaysAttackableNPC)
		{
			GEntityNPC* pNPC = m_Helper.NewEntityNPC(m_pField);
			pNPC->ChangeAttackable(NAT_ALWAYS);

			CHECK_EQUAL(RT_ENEMY, m_RelationSelector.Select(m_pPlayer, pNPC));
		}

		// ����� �Ѽ��� ���� NPC
		TEST_FIXTURE(Fixture, TarIsGoodFactionNPC)
		{
			GEntityNPC* pNPC = m_Helper.NewEntityNPC(m_pField);
			pNPC->GetNPCInfo()->m_nFactionLossID = m_pFactionInfo->m_nID;
			pNPC->ChangeAttackable(NAT_NONE);

			uint16 nBeforeQuantity = m_pPlayer->GetPlayerFactions().GetQuantity(m_pFactionInfo->m_nID);
			m_pPlayer->GetPlayerFactions().Increase(m_pFactionInfo->m_nID, CSFactionCalculator::GetGoodMinQuantity() - nBeforeQuantity);

			CHECK_EQUAL(RT_ALLY, m_RelationSelector.Select(m_pPlayer, pNPC));
		}

		// ����� �Ѽ��� ����, �׾��ִ� NPC
		TEST_FIXTURE(Fixture, TarIsGoodFactionAndDeadNPC)
		{
			GEntityNPC* pNPC = m_Helper.NewEntityNPC(m_pField);
			pNPC->GetNPCInfo()->m_nFactionLossID = m_pFactionInfo->m_nID;
			pNPC->ChangeAttackable(NAT_NONE);
			pNPC->doDie();

			uint16 nBeforeQuantity = m_pPlayer->GetPlayerFactions().GetQuantity(m_pFactionInfo->m_nID);
			m_pPlayer->GetPlayerFactions().Increase(m_pFactionInfo->m_nID, CSFactionCalculator::GetGoodMinQuantity() - nBeforeQuantity);

			CHECK_EQUAL(RT_ALLYDEAD, m_RelationSelector.Select(m_pPlayer, pNPC));
		}

		// ����� �Ѽ��� �����϶� ���� ������ �ִ� NPC
		TEST_FIXTURE(Fixture, TarIsFactionAttackableNPC)
		{
			GEntityNPC* pNPC = m_Helper.NewEntityNPC(m_pField);
			pNPC->GetNPCInfo()->m_nFactionLossID = m_pFactionInfo->m_nID;
			pNPC->ChangeAttackable(NAT_FACTION);

			uint16 nBeforeQuantity = m_pPlayer->GetPlayerFactions().GetQuantity(m_pFactionInfo->m_nID);
			m_pPlayer->GetPlayerFactions().Decrease(m_pFactionInfo->m_nID, nBeforeQuantity - CSFactionCalculator::GetBadMaxQuantity());

			CHECK_EQUAL(RT_ENEMY, m_RelationSelector.Select(m_pPlayer, pNPC));
		}
	}

	namespace ReqNPCTarPlayer
	{
		// ����� ����Ʈ ���̺� �ִ� �÷��̾�
		TEST_FIXTURE(Fixture, TarInHateTable)
		{
			GEntityNPC* pNPC = m_Helper.NewEntityNPC(m_pField);
			pNPC->GetHateTable().AddPoint_FoundEnemy(m_pPlayer);

			CHECK_EQUAL(RT_ENEMY, m_RelationSelector.Select(pNPC, m_pPlayer));
		}

		// �׻� ���� �����ϴ� NPC
		TEST_FIXTURE(Fixture, ReqIsAlwaysAA)
		{
			GEntityNPC* pNPC = m_Helper.NewEntityNPC(m_pField);
			pNPC->GetNPCStatus().nAA = NAAT_ALWAYS;

			CHECK_EQUAL(RT_ENEMY, m_RelationSelector.Select(pNPC, m_pPlayer));
		}

		// ����� �Ѽ��� ���� �÷��̾�
		TEST_FIXTURE(Fixture, TarIsGoodFactionPlayer)
		{
			GEntityNPC* pNPC = m_Helper.NewEntityNPC(m_pField);
			pNPC->GetNPCInfo()->m_nFactionLossID = m_pFactionInfo->m_nID;

			uint16 nBeforeQuantity = m_pPlayer->GetPlayerFactions().GetQuantity(m_pFactionInfo->m_nID);
			m_pPlayer->GetPlayerFactions().Increase(m_pFactionInfo->m_nID, CSFactionCalculator::GetGoodMinQuantity() - nBeforeQuantity);

			CHECK_EQUAL(RT_ALLY, m_RelationSelector.Select(pNPC, m_pPlayer));
		}

		// ����� �Ѽ��� ����, ���� �÷��̾�
		TEST_FIXTURE(Fixture, TarIsGoodFactionAndDeadPlayer)
		{
			m_pPlayer->doDie();

			GEntityNPC* pNPC = m_Helper.NewEntityNPC(m_pField);
			pNPC->GetNPCInfo()->m_nFactionLossID = m_pFactionInfo->m_nID;

			uint16 nBeforeQuantity = m_pPlayer->GetPlayerFactions().GetQuantity(m_pFactionInfo->m_nID);
			m_pPlayer->GetPlayerFactions().Increase(m_pFactionInfo->m_nID, CSFactionCalculator::GetGoodMinQuantity() - nBeforeQuantity);

			CHECK_EQUAL(RT_ALLYDEAD, m_RelationSelector.Select(pNPC, m_pPlayer));
		}

		// �Ѽ��� �����϶� ���� �����ϴ� NPC
		TEST_FIXTURE(Fixture, ReqIsFactionAA)
		{
			GEntityNPC* pNPC = m_Helper.NewEntityNPC(m_pField);
			pNPC->GetNPCInfo()->m_nFactionLossID = m_pFactionInfo->m_nID;
			pNPC->GetNPCStatus().nAA = NAAT_FACTION;

			uint16 nBeforeQuantity = m_pPlayer->GetPlayerFactions().GetQuantity(m_pFactionInfo->m_nID);
			m_pPlayer->GetPlayerFactions().Decrease(m_pFactionInfo->m_nID, nBeforeQuantity - CSFactionCalculator::GetBadMaxQuantity());

			CHECK_EQUAL(RT_ENEMY, m_RelationSelector.Select(pNPC, m_pPlayer));
		}
	}

	namespace RetNPCTarNPC
	{
		// ����� ����÷��� NPC
		TEST_FIXTURE(Fixture, TarIsDuelFlagNPC)
		{
			GEntityNPC* pNPC = m_Helper.NewEntityNPC(m_pField);
			GEntityNPC* pNPC2 = m_Helper.NewEntityNPC(m_pField, vec3(1.0f,1.0f,1.0f), DUEL_FLAG_NPCID);

			CHECK_EQUAL(RT_NONE, m_RelationSelector.Select(pNPC, pNPC2));
		}

		// ����� ����Ʈ ���̺� �ִ� NPC
		TEST_FIXTURE(Fixture, TarInHateTable)
		{
			GEntityNPC* pNPC = m_Helper.NewEntityNPC(m_pField);
			GEntityNPC* pNPC2 = m_Helper.NewEntityNPC(m_pField);
			pNPC->GetHateTable().AddPoint_FoundEnemy(pNPC2);

			CHECK_EQUAL(RT_ENEMY, m_RelationSelector.Select(pNPC, pNPC2));
		}

		// AotoTestType�� AI�� ���
		TEST_FIXTURE(Fixture, AotoTestTypeIsAI)
		{
			GEntityNPC* pNPC = m_Helper.NewEntityNPC(m_pField);
			GEntityNPC* pNPC2 = m_Helper.NewEntityNPC(m_pField);

			GConfig::m_strAutoTestType = CONFIG_AUTO_TEST_AI;

			CHECK_EQUAL(RT_ENEMY, m_RelationSelector.Select(pNPC, pNPC2));
		}

		// ������ �Ѽ��� ���� NPC
		TEST_FIXTURE(Fixture, NPCHaveNotFaction)
		{
			GEntityNPC* pNPC = m_Helper.NewEntityNPC(m_pField);				
			GEntityNPC* pNPC2 = m_Helper.NewEntityNPC(m_pField);
			GFactionInfo* pFactionInfo2 = test.faction.NewFactionInfo(CSFactionCalculator::GetNormalMinQuantity());
			GFactionRelationInfo* pFactionRelationInfo = test.faction.NewFactionRelationInfo(m_pFactionInfo->m_nID, pFactionInfo2->m_nID, FRT_GOOD);
			pNPC->GetNPCInfo()->m_nFactionLossID = m_pFactionInfo->m_nID;

			CHECK_EQUAL(RT_ALL, m_RelationSelector.Select(pNPC, pNPC2));
		}

		// ����� �Ѽ��� ���� NPC
		TEST_FIXTURE(Fixture, TarIsGoodFactionNPC)
		{
			GEntityNPC* pNPC = m_Helper.NewEntityNPC(m_pField);				
			GEntityNPC* pNPC2 = m_Helper.NewEntityNPC(m_pField);
			GFactionInfo* pFactionInfo2 = test.faction.NewFactionInfo(CSFactionCalculator::GetNormalMinQuantity());	
			GFactionRelationInfo* pFactionRelationInfo = test.faction.NewFactionRelationInfo(m_pFactionInfo->m_nID, pFactionInfo2->m_nID, FRT_GOOD);
			pNPC->GetNPCInfo()->m_nFactionLossID = m_pFactionInfo->m_nID;
			pNPC2->GetNPCInfo()->m_nFactionLossID = pFactionInfo2->m_nID;

			CHECK_EQUAL(RT_ALLY, m_RelationSelector.Select(pNPC, pNPC2));
		}

		// ����� �Ѽ��� ����, �׾��ִ� NPC
		TEST_FIXTURE(Fixture, TarIsGoodFactionAndDeadNPC)
		{
			GEntityNPC* pNPC = m_Helper.NewEntityNPC(m_pField);				
			GEntityNPC* pNPC2 = m_Helper.NewEntityNPC(m_pField);
			GFactionInfo* pFactionInfo2 = test.faction.NewFactionInfo(CSFactionCalculator::GetNormalMinQuantity());				
			GFactionRelationInfo* pFactionRelationInfo = test.faction.NewFactionRelationInfo(m_pFactionInfo->m_nID, pFactionInfo2->m_nID, FRT_GOOD);
			pNPC->GetNPCInfo()->m_nFactionLossID = m_pFactionInfo->m_nID;
			pNPC2->GetNPCInfo()->m_nFactionLossID = pFactionInfo2->m_nID;
			pNPC2->doDie();

			CHECK_EQUAL(RT_ALLYDEAD, m_RelationSelector.Select(pNPC, pNPC2));
		}

		// ����� �Ѽ��� ���� NPC
		TEST_FIXTURE(Fixture, TarIsBadFactionNPC)
		{
			GEntityNPC* pNPC = m_Helper.NewEntityNPC(m_pField);
			GEntityNPC* pNPC2 = m_Helper.NewEntityNPC(m_pField);
			GFactionInfo* pFactionInfo2 = test.faction.NewFactionInfo(CSFactionCalculator::GetNormalMinQuantity());				
			GFactionRelationInfo* pFactionRelationInfo = test.faction.NewFactionRelationInfo(m_pFactionInfo->m_nID, pFactionInfo2->m_nID, FRT_BAD);
			pNPC->GetNPCInfo()->m_nFactionLossID = m_pFactionInfo->m_nID;
			pNPC2->GetNPCInfo()->m_nFactionLossID = pFactionInfo2->m_nID;

			CHECK_EQUAL(RT_ENEMY, m_RelationSelector.Select(pNPC, pNPC2));
		}
	}
}



