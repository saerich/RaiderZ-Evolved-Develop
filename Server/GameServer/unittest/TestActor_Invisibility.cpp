#include "stdafx.h"
#include "GTestForward.h"
#include "GUTHelper.h"
#include "MockField.h"
#include "GEntityPlayer.h"
#include "GEntityNPC.h"

#include "FBaseMockLink.h"
#include "GActorInvisibility.h"
#include "GPartySystem.h"
#include "GPartyManager.h"
#include "GItemManager.h"
#include "GModuleAI.h"
#include "GFieldGridEntityInfoMaker.h"

SUITE(Invisibility)
{
	struct FInvisibility : public FBaseMockLink
	{
		FInvisibility()
		{
			m_pField = GUTHelper_Field::DefaultMockField();

			m_pNPC = GUTHelper_NPC::SpawnNPC(m_pField);
			m_pPlayer = test.player.NewPlayer(m_pField);
			m_nInvisibleBuffID = 123;
			m_pInvisibleBuff = test.buff.NewBuffInfo(m_nInvisibleBuffID);
			m_pInvisibleBuff->m_nPassiveExtraAttrib = BUFPEA_INVISIBILITY;

			m_pNPC->GetNPCInfo()->m_bEnableAILod = false;

		}
		virtual ~FInvisibility()
		{
			m_pNPC->DeleteMe();
			m_pPlayer->DeleteMe();
			m_pField->Update(0.0f);
		}

		void doInvisible(GEntityActor* pTarget)
		{
			VALID(pTarget);
			pTarget->GetActorInvisibility().SetDice(new SuccessOnlyDice());
			test.buff.GainBuffSelf(pTarget, m_pInvisibleBuff);
		}

		void MakeCombat(GEntityActor* pAttacker, GEntityActor* pVictim)
		{
			GTalentInfo* pAttackTalent = test.talent.MakeDamageTalent(10);
			test.talent.UseTalent(pAttacker, pAttackTalent, pVictim->GetUID());
		}

		MockField*		m_pField;
		GEntityNPC*		m_pNPC;
		GEntityPlayer*	m_pPlayer;
		int				m_nInvisibleBuffID;
		GBuffInfo*		m_pInvisibleBuff;
		

		DECLWRAPPER_FieldInfoMgr;
		DECLWRAPPER_BuffMgr;
		DECLWRAPPER_TalentMgr;
		DECLWRAPPER_ItemMgr;
		GTestSysWrapper<GPartySystem>		m_PartySystemWrapper;
	};

	TEST_FIXTURE(FInvisibility, CheckSectorTD)
	{
		vector<TD_UPDATE_CACHE_PLAYER> vecPlayerInfoNode;
		vector<TD_PLAYER_FEATURE_TATTOO> vecPlayerFeatureTattooNode;
		vector<TD_UPDATE_CACHE_NPC> vecNPCInfoNode;
		vector<TD_UPDATE_CACHE_PBART> vecBParts;
		vector<TD_TRIGGER_SENSOR_INFO> vecMarkers;
		vector<TD_UPDATE_CACHE_BUFFENTITY> vecMagicArearInfoNode;
		
		vecPlayerInfoNode.clear();
		vecPlayerFeatureTattooNode.clear();
		vecNPCInfoNode.clear();
		vector<GFieldGrid::Sector*> vecCells;
		class SectorCollector : public GFieldGrid::SectorSelector::IVisitor
		{
		public:
			SectorCollector(vector<GFieldGrid::Sector*>& sectors) : m_sectors(sectors) {}

		private:
			void OnVisit(GFieldGrid::Sector& sector)
			{
				m_sectors.push_back(&sector);
			}

		private:
			vector<GFieldGrid::Sector*>& m_sectors;
		};
		m_pField->GetGrid().GetSectorSelector()->VisitNeighborsByPos(SectorCollector(vecCells), m_pPlayer->GetPos(), 1);

		GFieldGridEntityInfoMaker gridInfoMaker;
		gridInfoMaker.MakeSensorInfos(m_pPlayer, vecMarkers);
		gridInfoMaker.MakeBuffInfos(m_pPlayer, vecCells, vecMagicArearInfoNode);
		gridInfoMaker.MakeNPCInfos(m_pPlayer, vecCells, vecNPCInfoNode, vecBParts);
		gridInfoMaker.MakePlayerInfos(m_pPlayer, vecCells, vecPlayerInfoNode, vecPlayerFeatureTattooNode);

		CHECK_EQUAL(0, vecPlayerInfoNode.size()); // �ڱ��ڽ��� ������ ������ ����
		CHECK_EQUAL(0, vecPlayerFeatureTattooNode.size()); // �ڱ��ڽ��� ������ ������ ����
		CHECK_EQUAL(1, vecNPCInfoNode.size());

		// ����ȭ �Ŀ��� ������������ ã�� �� ����
		doInvisible(m_pPlayer);
		doInvisible(m_pNPC);

		vecPlayerInfoNode.clear();
		vecNPCInfoNode.clear();

		gridInfoMaker.MakeSensorInfos(m_pPlayer, vecMarkers);
		gridInfoMaker.MakeBuffInfos(m_pPlayer, vecCells, vecMagicArearInfoNode);
		gridInfoMaker.MakeNPCInfos(m_pPlayer, vecCells, vecNPCInfoNode, vecBParts);
		gridInfoMaker.MakePlayerInfos(m_pPlayer, vecCells, vecPlayerInfoNode, vecPlayerFeatureTattooNode);
		
		CHECK_EQUAL(0, vecPlayerInfoNode.size());
		CHECK_EQUAL(0, vecPlayerFeatureTattooNode.size());
		CHECK_EQUAL(0, vecNPCInfoNode.size());
	}

	TEST_FIXTURE(FInvisibility, SendHideShowPacket_ForPlayer)
	{
		GEntityPlayer* pObserver = test.player.NewPlayer(m_pField);
		pObserver->SetPos(vec3(1000,1000,0)); // Ž���Ÿ� �ۿ� ����
		MockLink* pLink = NewLink(pObserver);
		pLink->IgnoreAll();
		pLink->AddAllowedID(MC_ACTION_HIDE);
		pLink->AddAllowedID(MC_FIELD_IN_PLAYER);
		pLink->AddAllowedID(MC_FIELD_IN_NPC);
		doInvisible(m_pPlayer);
		CHECK_EQUAL(1, pLink->GetCommandCount());
		CHECK_EQUAL(MC_ACTION_HIDE, pLink->GetCommand(0).GetID());

		pLink->ResetCommands();
		pObserver->SetPos(vec3(0,0,0)); // Ž���Ÿ� ������ ����
		m_pPlayer->Update(GConst::INVISIBILITY_DETECT_TICK);	// Ž�� ƽ ���
		CHECK_EQUAL(1, pLink->GetCommandCount());
		CHECK_EQUAL(MC_FIELD_IN_PLAYER, pLink->GetCommand(0).GetID());
	}

	TEST_FIXTURE(FInvisibility, SendHideShowPacket_ForNPC)
	{
		GEntityPlayer* pObserver = test.player.NewPlayer(m_pField);
		pObserver->SetPos(vec3(1000,1000,0)); // Ž���Ÿ� �ۿ� ����
		MockLink* pLink = NewLink(pObserver);
		pLink->IgnoreAll();
		pLink->AddAllowedID(MC_ACTION_HIDE);
		pLink->AddAllowedID(MC_FIELD_IN_PLAYER);
		pLink->AddAllowedID(MC_FIELD_IN_NPC);
		doInvisible(m_pNPC);
		CHECK_EQUAL(1, pLink->GetCommandCount());
		CHECK_EQUAL(MC_ACTION_HIDE, pLink->GetCommand(0).GetID());

		pLink->ResetCommands();
		pObserver->SetPos(vec3(0,0,0)); // Ž���Ÿ� ������ ����
		m_pNPC->Update(GConst::INVISIBILITY_DETECT_TICK);	// Ž�� ƽ ���
		CHECK_EQUAL(1, pLink->GetCommandCount());
		CHECK_EQUAL(MC_FIELD_IN_NPC, pLink->GetCommand(0).GetID());
	}

	TEST_FIXTURE(FInvisibility, SendShowPacket_WhenUncloak)
	{
		GEntityPlayer* pObserver = test.player.NewPlayer(m_pField);
		pObserver->SetPos(vec3(1000,1000,0)); // Ž���Ÿ� �ۿ� ����
		MockLink* pLink = NewLink(pObserver);
		pLink->IgnoreAll();
		pLink->AddAllowedID(MC_ACTION_HIDE);
		pLink->AddAllowedID(MC_FIELD_IN_PLAYER);
		pLink->AddAllowedID(MC_FIELD_IN_NPC);
		doInvisible(m_pPlayer);
		CHECK_EQUAL(1, pLink->GetCommandCount());
		CHECK_EQUAL(MC_ACTION_HIDE, pLink->GetCommand(0).GetID());

		// ����ȭ ���� ������ ��� ���̱�
		pLink->ResetCommands();
		test.buff.LostBuff(m_pPlayer, m_nInvisibleBuffID);
		CHECK_EQUAL(1, pLink->GetCommandCount());
		CHECK_EQUAL(MC_FIELD_IN_PLAYER, pLink->GetCommand(0).GetID());
	}

	TEST_FIXTURE(FInvisibility, SendShowPacket_SectorRoute)
	{
		GEntityPlayer* pObserver = test.player.NewPlayer(m_pField);
		pObserver->SetPos(vec3(1000,1000,0)); // Ž���Ÿ� �ۿ� ����
		MockLink* pObserverLink = NewLink(pObserver);
		pObserverLink->IgnoreAll();
		pObserverLink->AddAllowedID(MC_ACTION_MOVE);
		MockLink* pPlayerLink = NewLink(m_pPlayer);

		doInvisible(m_pPlayer);

		TD_PC_MOVE stMoveInfo;
		stMoveInfo.nFlags = MOVEMENTFLAG_FORWARD;
		stMoveInfo.vTarPos = m_pPlayer->GetPos();
		stMoveInfo.svDir.SetX(m_pPlayer->GetDir().x);
		stMoveInfo.svDir.SetY(m_pPlayer->GetDir().y);

		// �Ⱥ���
		pPlayerLink->OnRecv(MC_ACTION_MOVE_REQ,		1, NEW_SINGLE_BLOB(&stMoveInfo, sizeof(TD_PC_MOVE)));
		CHECK_EQUAL(0, pObserverLink->GetCommandCount());

		// ����
		pObserver->SetPos(vec3(0,0,0)); // Ž���Ÿ� ������ ����
		m_pPlayer->Update(GConst::INVISIBILITY_DETECT_TICK);	// Ž�� ƽ ���
		pPlayerLink->OnRecv(MC_ACTION_MOVE_REQ,		1, NEW_SINGLE_BLOB(&stMoveInfo, sizeof(TD_PC_MOVE)));
		CHECK_EQUAL(1, pObserverLink->GetCommandCount());
		CHECK_EQUAL(MC_ACTION_MOVE, pObserverLink->GetCommand(0).GetID());

		// �ٽ� �Ⱥ���
		pObserver->SetPos(vec3(1000,1000,0)); // Ž���Ÿ� �ۿ� ����
		pObserverLink->ResetCommands();
		m_pPlayer->Update(GConst::INVISIBILITY_DETECT_TICK);	// Ž�� ƽ ���
		pPlayerLink->OnRecv(MC_ACTION_MOVE_REQ,		1, NEW_SINGLE_BLOB(&stMoveInfo, sizeof(TD_PC_MOVE)));
		CHECK_EQUAL(0, pObserverLink->GetCommandCount());
	}

	TEST_FIXTURE(FInvisibility, SendHideShowPacketOnlyOnce)
	{
		GEntityPlayer* pObserver = test.player.NewPlayer(m_pField);
		pObserver->SetPos(vec3(1000,1000,0)); // Ž���Ÿ� �ۿ� ����
		MockLink* pLink = NewLink(pObserver);
		pLink->IgnoreAll();
		pLink->AddAllowedID(MC_ACTION_HIDE);
		pLink->AddAllowedID(MC_FIELD_IN_PLAYER);
		pLink->AddAllowedID(MC_FIELD_IN_NPC);
		doInvisible(m_pPlayer);
		CHECK_EQUAL(1, pLink->GetCommandCount());
		CHECK_EQUAL(MC_ACTION_HIDE, pLink->GetCommand(0).GetID());

		// �ð��� ������ ��Ŷ ������ ����
		pLink->ResetCommands();
		m_pPlayer->Update(GConst::INVISIBILITY_DETECT_TICK);	// Ž�� ƽ ���
		CHECK_EQUAL(0, pLink->GetCommandCount());

		pLink->ResetCommands();
		pObserver->SetPos(vec3(0,0,0)); // Ž���Ÿ� ������ ����
		m_pPlayer->Update(GConst::INVISIBILITY_DETECT_TICK);	// Ž�� ƽ ���
		CHECK_EQUAL(1, pLink->GetCommandCount());
		CHECK_EQUAL(MC_FIELD_IN_PLAYER, pLink->GetCommand(0).GetID());

		// �ð��� ������ ��Ŷ ������ ����
		pLink->ResetCommands();
		m_pPlayer->Update(GConst::INVISIBILITY_DETECT_TICK);	// Ž�� ƽ ���
		CHECK_EQUAL(0, pLink->GetCommandCount());
	}

	TEST_FIXTURE(FInvisibility, DetectLevelPercent)
	{
		struct Case
		{
			const int nLevelFactor;
			const float fPercent;
		} cases[] = {
			{0, 9}
			,{1, 5}
			,{2, 2}
			,{3, 0}
			,{4, 0}
			,{5, 0}
			,{6, -1}
			,{7, -1}
			,{8, -3}
			,{9, -6}
			,{10, -10}
			,{11, -10}
			,{12, -10}
		};

		m_pPlayer->SetLevel(10);

		for (int i=0; i<ArraySize(cases); ++i) 
		{
			MakeLevelFactor(cases[i].nLevelFactor, m_pNPC, m_pPlayer);
			CHECK_EQUAL(cases[i].fPercent, m_pNPC->GetActorInvisibility().CalcDetectLevelPercent(m_pPlayer));
		}
	}

	TEST_FIXTURE(FInvisibility, DetectDistancePercent)
	{
		struct Case
		{
			const float fDistance;
			const float fExpected;
		} cases[] = {
			{0, -5}
			,{100, -5}
			,{200, -2.5}
			,{400, 0}
			,{800, 5}
			,{1200, 10}
			,{1400, 10}
		};

		for (int i=0; i<ArraySize(cases); ++i) 
		{
			m_pPlayer->SetPos(vec3(0,0,0));
			m_pNPC->SetPos(vec3(cases[i].fDistance, 0, 0));
			CHECK_EQUAL(cases[i].fExpected, m_pNPC->GetActorInvisibility().CalcDetectDistancePercent(m_pPlayer));
		}
	}

	TEST_FIXTURE(FInvisibility, DetectArmorPercent)
	{
		// �ƹ� ���ʵ� ���԰� ����
		CHECK_EQUAL(0.0f, m_pPlayer->GetActorInvisibility().CalcDetectArmorPercent());

		struct Case
		{
			const ARMOR_TYPE nEquipmentType;
			const float fExpected;
		} cases[] = {
			{ARMOR_NONE, 0}
			,{ARMOR_CLOTH, 0}
			,{ARMOR_LIGHT, 0}
			,{ARMOR_MEDIUM, 15}
			,{ARMOR_HEAVY, 30}
		};

		for (int i=0; i<ArraySize(cases); ++i) 
		{
			GItemData* pItemData = test.item.NewItemData(i, ITEMSLOT_BODY);
			pItemData->m_nArmorType = cases[i].nEquipmentType;
			test.item.EquipNewItem(m_pPlayer, i, ITEMSLOT_BODY);
			CHECK_EQUAL(cases[i].fExpected, m_pPlayer->GetActorInvisibility().CalcDetectArmorPercent());
		}
	}

	TEST_FIXTURE(FInvisibility, SneakExpert)
	{
		MakeLevelFactor(5, m_pNPC, m_pPlayer);	// ���� ���� ����
		m_pNPC->SetPos(vec3(1000,1000,0));			// �ִ�Ÿ��� ������

		CHECK_EQUAL(10, m_pPlayer->GetActorInvisibility().CalcDetectChancePercent(m_pNPC));
		GTalentInfo* pPassive = test.talent.NewExtraPassiveTalentInfo(TEPT_BASE_SNEAK);
		pPassive->m_nExtraPassiveParam = 10;	// �߰� Ȯ���� 10% ������
		test.talent.Train(m_pPlayer, pPassive);
		CHECK_EQUAL(0, m_pPlayer->GetActorInvisibility().CalcDetectChancePercent(m_pNPC));
	}

	TEST_FIXTURE(FInvisibility, AlwaysShowForPartyMember)
	{
		GEntityPlayer* pObserver = test.player.NewPlayer(m_pField);
		pObserver->SetPos(vec3(1000,1000,0)); // Ž���Ÿ� �ۿ� ����
		MockLink* pLink = NewLink(pObserver);
		pLink->IgnoreAll();
		pLink->AddAllowedID(MC_ACTION_HIDE);
		pLink->AddAllowedID(MC_FIELD_IN_PLAYER);
		pLink->AddAllowedID(MC_FIELD_IN_NPC);
		doInvisible(m_pPlayer);
		CHECK_EQUAL(1, pLink->GetCommandCount());
		CHECK_EQUAL(MC_ACTION_HIDE, pLink->GetCommand(0).GetID());

		// �������� ��Ƽ�ϱ�
		pLink->ResetCommands();

		GParty* pParty = GUTHelper_Party::CreateParty(pObserver, m_pPlayer);
		
		m_pPlayer->Update(GConst::INVISIBILITY_DETECT_TICK);	// Ž�� ƽ ���
		CHECK_EQUAL(1, pLink->GetCommandCount());
		CHECK_EQUAL(MC_FIELD_IN_PLAYER, pLink->GetCommand(0).GetID());

		pLink->ResetCommands();
		pObserver->SetPos(vec3(0,0,0)); // Ž���Ÿ� ������ ����
		m_pPlayer->Update(GConst::INVISIBILITY_DETECT_TICK);	// Ž�� ƽ ���
		CHECK_EQUAL(0, pLink->GetCommandCount());

		// �������� ��Ƽ����
		pObserver->SetPos(vec3(1000,1000,0)); // Ž���Ÿ� �ۿ� ����
		pLink->ResetCommands();

		GUTHelper_Party::LeaveMember(pParty, pObserver);

		m_pPlayer->Update(GConst::INVISIBILITY_DETECT_TICK);	// Ž�� ƽ ���
		CHECK_EQUAL(1, pLink->GetCommandCount());
		CHECK_EQUAL(MC_ACTION_HIDE, pLink->GetCommand(0).GetID());
	}

	TEST_FIXTURE(FInvisibility, MakePeaceByInvisibe)
	{
		MakeCombat(m_pPlayer, m_pNPC);
		CHECK_EQUAL(true, m_pPlayer->IsNowCombat());
		CHECK_EQUAL(true, m_pNPC->IsNowCombat());
		doInvisible(m_pPlayer);
		m_pNPC->GetModuleAI()->Update(0.0f); // FSM ���濡 �ʿ��� ƽ
		m_pNPC->GetModuleAI()->Update(GConst::VICTORY_DEFAULT_DELAY); // �¸����� ��⿡ �ʿ��� ƽ
		m_pNPC->GetModuleAI()->Update(0.0f); // FSM ���濡 �ʿ��� ƽ
		CHECK_EQUAL(false, m_pPlayer->IsNowCombat());
		CHECK_EQUAL(true, m_pNPC->IsNowCombat());
	}

	TEST(InvisibilityBuffValue)
	{
		GBuffInfo info;
		info.Cooking();

		// �ƹ� �Ӽ��� ���� ���� �⺻�� ��ȯ
		CHECK_EQUAL(100.0f, info.GetInvisibilityPower());

		// 1�� �Ӽ��� ����ȭ
		info.m_nPassiveExtraAttrib = BUFPEA_INVISIBILITY;
		info.m_fPassiveExtraAttribValue = 34.0f;
		info.Cooking();
		CHECK_EQUAL(info.m_fPassiveExtraAttribValue, info.GetInvisibilityPower());

		// 2�� �Ӽ��� ����ȭ
		info.m_nPassiveExtraAttrib = BUFPEA_NONE;
		info.m_fPassiveExtraAttribValue = 0.0f;
		info.m_nPassiveExtraAttrib2 = BUFPEA_INVISIBILITY;
		info.m_fPassiveExtraAttribValue2 = 23.0f;
		info.Cooking();
		CHECK_EQUAL(info.m_fPassiveExtraAttribValue2, info.GetInvisibilityPower());

		// ����ȭ �Ӽ��� ���� ������ ���
		info.m_nPassiveExtraAttrib = BUFPEA_INVISIBILITY;
		info.m_fPassiveExtraAttribValue = 0.0f;
		info.m_nPassiveExtraAttrib2 = BUFPEA_NONE;
		info.m_fPassiveExtraAttribValue2 = 34.0f;
		info.Cooking();
		CHECK_EQUAL(0.0f, info.GetInvisibilityPower());
	}
}