#include "stdafx.h"
#include "GTestForward.h"
#include "GBattleArenaMgr.h"
#include "GPlayerBattleArena.h"
#include "GMatchRule_SameTeamMember.h"
#include "GGameRule_DeathMatch.h"
#include "GBattleArenaDefine.h"
#include "GGlobal.h"
#include "GServer.h"
#include "GSharedField.h"
#include "GPlayerSystem.h"
#include "GFieldFactory.h"
#include "FBasePlayer.h"
#include "GUTHelper_Party.h"
#include "GRebirthSystem.h"

SUITE(BattleArena)
{
	struct FBattleArena : public FBasePlayer
	{
		FBattleArena(): m_pBlueParty(NULL), m_pRedParty(NULL)
		{
			// �ʱ� �ʵ� �����
			GFieldInfo* pSFieldInfo = new GFieldInfo;
			pSFieldInfo->m_bDynamic = false;
			pSFieldInfo->m_nFieldID = 777;
			pSFieldInfo->m_nDefaultChannelCount = 1;
			pSFieldInfo->SetFieldMaxXY_ForTest(50000.0f, 50000.0f);
			gmgr.pFieldInfoMgr->Insert(pSFieldInfo);
			gmgr.pFieldMgr->GetFactory()->CreateSharedFieldMaster(pSFieldInfo);
			m_pField = gmgr.pFieldMgr->GetFactory()->CreateSharedField(gsys.pServer->NewUID(), pSFieldInfo->m_nFieldID, 0);

			// �÷��̾� ����
			MakePlayer(m_pPlayerBlueLeader, L"BlueLeader");
			MakePlayer(m_pPlayerBlueMember1, L"BlueMember1");
			MakePlayer(m_pPlayerBlueMember2, L"BlueMember2");
			MakePlayer(m_pPlayerRedLeader, L"RedLeader");
			MakePlayer(m_pPlayerRedMember1, L"RedMember1");
			MakePlayer(m_pPlayerRedMember2, L"RedMember2");

			
			// ��Ƽ ����
			m_pBlueParty = MakeParty(m_pPlayerBlueLeader, m_pPlayerBlueMember1, m_pPlayerBlueMember2);
			m_pRedParty = MakeParty(m_pPlayerRedLeader, m_pPlayerRedMember1, m_pPlayerRedMember2);
			gsys.pServer->FrameCounterClear();

			// ������ �ʵ�׷� ����
			FIELD_GROUP_INFO* pFieldGroupInfo = new FIELD_GROUP_INFO;
			pFieldGroupInfo->nID = GConst::ARENA_FIELDGROUP_ID;
			// ������ �ʵ� ����
			GFieldInfo* pFieldInfo = new GFieldInfo;
			pFieldInfo->m_bDynamic = true;
			pFieldInfo->m_nFieldID = GConst::ARENA_FIELD_ID;
			pFieldInfo->SetFieldMaxXY_ForTest(50000.0f, 50000.0f);
			// ���� ��Ŀ ����
			MARKER_INFO markerStarting;
			markerStarting.nID = 1;
			markerStarting.nFieldID = GConst::ARENA_FIELD_ID;
			markerStarting.vPoint = vec3(1000,1000,1000);
			markerStarting.vDir = vec3(0,1,0);
			pFieldInfo->InsertMarkerInfo(markerStarting);	// 1�� ������
			markerStarting.nID = 2;
			markerStarting.nFieldID = GConst::ARENA_FIELD_ID;
			markerStarting.vPoint = vec3(2000,2000,2000);
			markerStarting.vDir = vec3(0,0,1);
			pFieldInfo->InsertMarkerInfo(markerStarting);	// 2�� ������
			pFieldGroupInfo->vecFieldInfo.push_back(pFieldInfo);
			gmgr.pFieldInfoMgr->Insert(pFieldInfo);
			gmgr.pFieldInfoMgr->InsertGroup(pFieldGroupInfo);
		}

		~FBattleArena()
		{
			test.network.ClearLinks();
			m_pField->Destroy();
		}

		void MakePlayer(GEntityPlayer*& pPlayer, std::wstring strName)
		{
			pPlayer = NewEntityPlayer(m_pField, vec3(100,100,0));
			swprintf_s(pPlayer->GetPlayerInfo()->szName, strName.c_str());
			vecAllPlayers.push_back(pPlayer);
		}

		GParty* MakeParty(GEntityPlayer* pLeader, GEntityPlayer* pMember, GEntityPlayer* pMember2)
		{
			GParty* pParty = GUTHelper_Party::CreateParty(pLeader);
			GUTHelper_Party::AddMember(pParty, pMember);

			if (pMember2 != NULL)
			{
				GUTHelper_Party::AddMember(pParty, pMember2);
			}

			return pParty;
		}

		void WaitForPlayCount()
		{
			for (int i=0; i<GConst::ARENA_GAME_COUNT; i++)
			{
				gsys.pServer->Update(1.0f);
			}
		}

		void WaitForScoreBoardCount()
		{
			for (int i=0; i<GConst::ARENA_SCOREBOARD_COUNT; i++)
			{
				gsys.pServer->Update(1.0f);
			}
		}

		void CheckFieldID(int nExpectedFieldID)
		{
			CHECK_EQUAL(nExpectedFieldID, m_pPlayerBlueLeader->GetFieldID());
			CHECK_EQUAL(nExpectedFieldID, m_pPlayerBlueMember1->GetFieldID());
			CHECK_EQUAL(nExpectedFieldID, m_pPlayerRedLeader->GetFieldID());
			CHECK_EQUAL(nExpectedFieldID, m_pPlayerRedMember1->GetFieldID());
		}

		void CheckBattleAreaUID(MUID uidExpectedBattleArena)
		{
			CHECK_EQUAL(uidExpectedBattleArena, m_pPlayerBlueLeader->GetPlayerBattleArena().GetBattleArenaUID());
			CHECK_EQUAL(uidExpectedBattleArena, m_pPlayerBlueMember1->GetPlayerBattleArena().GetBattleArenaUID());
			CHECK_EQUAL(uidExpectedBattleArena, m_pPlayerRedLeader->GetPlayerBattleArena().GetBattleArenaUID());
			CHECK_EQUAL(uidExpectedBattleArena, m_pPlayerRedMember1->GetPlayerBattleArena().GetBattleArenaUID());
		}


		GSharedField*	m_pField;
		GEntityPlayer*	m_pPlayerBlueLeader;
		GEntityPlayer*	m_pPlayerBlueMember1;
		GEntityPlayer*	m_pPlayerBlueMember2;
		GEntityPlayer*	m_pPlayerRedLeader;
		GEntityPlayer*	m_pPlayerRedMember1;
		GEntityPlayer*	m_pPlayerRedMember2;
		GParty*			m_pBlueParty;
		GParty*			m_pRedParty;
				
		vector<GEntityPlayer*> vecAllPlayers;

		DECLWRAPPER_FieldInfoMgr;
		DECLWRAPPER_FieldMgr;
		DECLWRAPPER_BuffMgr;
		GTestMgrWrapper<GBattleArenaMgr>	BattleArenaMgr;
		DECLWRAPPER_NPCMgr;
	};

	TEST_FIXTURE(FBattleArena, Register_OK)
	{
		CHECK_EQUAL(0, gmgr.pBattleArenaMgr->GetTeamQty());
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(1, gmgr.pBattleArenaMgr->GetTeamQty());

		// ��� ��Ʈ�� ���� Ȯ��
		BattleArena::Team entry = gmgr.pBattleArenaMgr->FindTeam(m_pPlayerBlueLeader->GetPartyUID());
		TVALID_EQ(false, entry.IsInvalid());
		CHECK_EQUAL(m_pPlayerBlueLeader->GetPartyUID(), entry.UID);
		CHECK_EQUAL(m_pPlayerBlueLeader->GetPartyUID(), entry.uidParty);
		CHECK_EQUAL(BattleArena::Team::WAITING_FOR_MATCH, entry.nStatus);
	}

	TEST_FIXTURE(FBattleArena, RegisterFail__WithoutParty)
	{
		GEntityPlayer*	pPlayerSolo = NewEntityPlayer(m_pField, vec3(100,100,0));
		CHECK_EQUAL(0, gmgr.pBattleArenaMgr->GetTeamQty());
		CHECK_EQUAL(false, pPlayerSolo->GetPlayerBattleArena().RegisterEntry());	// ��Ƽ�� ��� ����
		CHECK_EQUAL(0, gmgr.pBattleArenaMgr->GetTeamQty());
	}

	TEST_FIXTURE(FBattleArena, RegisterFail__Duplicated)
	{
		CHECK_EQUAL(0, gmgr.pBattleArenaMgr->GetTeamQty());
		m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry();
		CHECK_EQUAL(1, gmgr.pBattleArenaMgr->GetTeamQty());
		CHECK_EQUAL(false, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());	// �ߺ� ����̶� ����
	}

	TEST_FIXTURE(FBattleArena, Deregister_OK)
	{
		CHECK_EQUAL(0, gmgr.pBattleArenaMgr->GetTeamQty());
		m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry();
		CHECK_EQUAL(1, gmgr.pBattleArenaMgr->GetTeamQty());
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().DeregisterEntry());
		CHECK_EQUAL(0, gmgr.pBattleArenaMgr->GetTeamQty());
	}

	TEST_FIXTURE(FBattleArena, DeregisterFail__NeverRegister)
	{
		CHECK_EQUAL(false, m_pPlayerBlueLeader->GetPlayerBattleArena().DeregisterEntry());
		CHECK_EQUAL(0, gmgr.pBattleArenaMgr->GetTeamQty());
	}

	TEST_FIXTURE(FBattleArena, Match_OK)
	{
		CHECK_EQUAL(0, gmgr.pBattleArenaMgr->GetBattleArenaQty());

		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->AddGameRule(new GGameRule_DeathMatch());
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());

		// ���ο� �������� ������
		CHECK_EQUAL(1, gmgr.pBattleArenaMgr->GetBattleArenaQty());
		GBattleArena* pBattleArena1 = gmgr.pBattleArenaMgr->FindBattleArena(m_pPlayerBlueLeader->GetPlayerBattleArena().GetBattleArenaUID());
		GBattleArena* pBattleArena2 = gmgr.pBattleArenaMgr->FindBattleArena(m_pPlayerRedLeader->GetPlayerBattleArena().GetBattleArenaUID());
		TVALID_EQ(pBattleArena1, pBattleArena2);
		const BattleArena::Team* pEntry1 = pBattleArena1->FindTeam(m_pPlayerBlueLeader->GetPartyUID());
		const BattleArena::Team* pEntry2 = pBattleArena1->FindTeam(m_pPlayerRedLeader->GetPartyUID());
		TVALID(pEntry1);
		TVALID(pEntry2);
		TVALID_EQ(pEntry1->uidParty, m_pPlayerBlueLeader->GetPartyUID());
		TVALID_EQ(pEntry2->uidParty, m_pPlayerRedLeader->GetPartyUID());
	}

	TEST_FIXTURE(FBattleArena, Match__RuleIsSameEntryMember)
	{
		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->AddGameRule(new GGameRule_DeathMatch());
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());

		BattleArena::Team entry1 = gmgr.pBattleArenaMgr->FindTeam(m_pPlayerBlueLeader->GetPartyUID());
		BattleArena::Team entry2 = gmgr.pBattleArenaMgr->FindTeam(m_pPlayerRedLeader->GetPartyUID());
		TVALID_EQ(false, entry1.IsInvalid());
		TVALID_EQ(false, entry2.IsInvalid());

		// ���� �����·� �����
		CHECK_EQUAL(BattleArena::Team::WAITING_FOR_PLAY, entry1.nStatus);
		CHECK_EQUAL(BattleArena::Team::WAITING_FOR_PLAY, entry2.nStatus);
	}
	
	TEST_FIXTURE(FBattleArena, MatchFail__RuleIsSameEntryMember)
	{
		// 1�� ��Ƽ�� ��Ƽ�� �Ѹ� �߰�
		GEntityPlayer*	pExtraPlayer = NewEntityPlayer(m_pField, vec3(100,100,0));
		GUTHelper_Party::CreateParty(m_pPlayerBlueLeader, pExtraPlayer);

		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->AddGameRule(new GGameRule_DeathMatch());
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());

		BattleArena::Team entry1 = gmgr.pBattleArenaMgr->FindTeam(m_pPlayerBlueLeader->GetPartyUID());
		BattleArena::Team entry2 = gmgr.pBattleArenaMgr->FindTeam(m_pPlayerRedLeader->GetPartyUID());
		TVALID_EQ(false, entry1.IsInvalid());
		TVALID_EQ(false, entry2.IsInvalid());

		// ��Ī ����
		CHECK_EQUAL(BattleArena::Team::WAITING_FOR_MATCH, entry1.nStatus);
		CHECK_EQUAL(BattleArena::Team::WAITING_FOR_MATCH, entry2.nStatus);
	}

	// �ٸ� ���� ���࿡ �� �ٸ� ���� ��ġ�Ǳ�
	TEST_FIXTURE(FBattleArena, Match_WithOtherMatchedGame)
	{
		CHECK_EQUAL(0, gmgr.pBattleArenaMgr->GetBattleArenaQty());

		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->AddGameRule(new GGameRule_DeathMatch());
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());

		// ���ο� �������� ������
		CHECK_EQUAL(1, gmgr.pBattleArenaMgr->GetBattleArenaQty());

		// �Ǵٸ� ��Ƽ �����
		GEntityPlayer*	m_pPlayerBlackLeader;
		GEntityPlayer*	m_pPlayerBlackMember1;
		GEntityPlayer*	m_pPlayerBlackMember2;
		MakePlayer(m_pPlayerBlackLeader, L"BlackLeader");
		MakePlayer(m_pPlayerBlackMember1, L"BlackMember1");
		MakePlayer(m_pPlayerBlackMember2, L"BlackMember2");
		MakeParty(m_pPlayerBlackLeader, m_pPlayerBlackMember1, m_pPlayerBlackMember2);
		CHECK_EQUAL(true, m_pPlayerBlackLeader->GetPlayerBattleArena().RegisterEntry());

		// ���ο� �������� �������� ����
		CHECK_EQUAL(1, gmgr.pBattleArenaMgr->GetBattleArenaQty());

		// �Ǵٸ� ��Ƽ �����
		GEntityPlayer*	m_pPlayerWhiteLeader;
		GEntityPlayer*	m_pPlayerWhiteMember1;
		GEntityPlayer*	m_pPlayerWhiteMember2;
		MakePlayer(m_pPlayerWhiteLeader, L"WhiteLeader");
		MakePlayer(m_pPlayerWhiteMember1, L"WhiteMember1");
		MakePlayer(m_pPlayerWhiteMember2, L"WhiteMember2");
		MakeParty(m_pPlayerWhiteLeader, m_pPlayerWhiteMember1, m_pPlayerWhiteMember2);
		CHECK_EQUAL(true, m_pPlayerWhiteLeader->GetPlayerBattleArena().RegisterEntry());

		// ���ο� �������� ������
		CHECK_EQUAL(2, gmgr.pBattleArenaMgr->GetBattleArenaQty());
	}

	TEST_FIXTURE(FBattleArena, ProceedToPlaying)
	{
		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->AddGameRule(new GGameRule_DeathMatch());
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());

		// ���ο� �������� ������
		GBattleArena* pBattleArena1 = gmgr.pBattleArenaMgr->FindBattleArena(m_pPlayerBlueLeader->GetPlayerBattleArena().GetBattleArenaUID());
		CHECK_EQUAL(GBattleArena::COUNTING, pBattleArena1->GetStatus());
		WaitForPlayCount();
		CHECK_EQUAL(GBattleArena::PLAYING, pBattleArena1->GetStatus());
	}

	TEST_FIXTURE(FBattleArena, Finish_OK)
	{
		CHECK_EQUAL(0, gmgr.pBattleArenaMgr->GetBattleArenaQty());

		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		gmgr.pBattleArenaMgr->ClearGameRules();
		GGameRule_DeathMatch* pFinishRule = new GGameRule_DeathMatch();
		pFinishRule->SetMaxLifeCounter(1);	// ������ġ �������� 1�� ����
		gmgr.pBattleArenaMgr->AddGameRule(pFinishRule);
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());

		// ���ο� �������� ������
		GBattleArena* pBattleArena1 = gmgr.pBattleArenaMgr->FindBattleArena(m_pPlayerBlueLeader->GetPlayerBattleArena().GetBattleArenaUID());
		RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());
		CHECK_EQUAL(GBattleArena::COUNTING, pBattleArena1->GetStatus());
		WaitForPlayCount();
		CHECK_EQUAL(GBattleArena::PLAYING, pBattleArena1->GetStatus());
		CHECK_EQUAL(1, gmgr.pBattleArenaMgr->GetBattleArenaQty());
		m_pPlayerRedLeader->doDie();
		CHECK_EQUAL(1, gmgr.pBattleArenaMgr->GetBattleArenaQty());
		m_pPlayerRedMember1->doDie();
		CHECK_EQUAL(GBattleArena::SCOREBOARD, pBattleArena1->GetStatus());
		WaitForScoreBoardCount();
		CHECK_EQUAL(0, gmgr.pBattleArenaMgr->GetBattleArenaQty());
	}

	TEST_FIXTURE(FBattleArena, ChangeFIeld)
	{
		CHECK_EQUAL(0, gmgr.pBattleArenaMgr->GetBattleArenaQty());

		// �����ʵ忡 ���� 
		CheckFieldID(777);

		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		gmgr.pBattleArenaMgr->ClearGameRules();
		GGameRule_DeathMatch* pFinishRule = new GGameRule_DeathMatch();
		pFinishRule->SetMaxLifeCounter(2);	// ������ġ �������� 1�� ����
		gmgr.pBattleArenaMgr->AddGameRule(pFinishRule);
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());

		// ���ο� �������� ������
		GBattleArena* pBattleArena1 = gmgr.pBattleArenaMgr->FindBattleArena(m_pPlayerBlueLeader->GetPlayerBattleArena().GetBattleArenaUID());
		WaitForPlayCount();

		// ������ �ʵ� �̵���
		RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());
		CheckFieldID(GConst::ARENA_FIELD_ID);

		// ���� ��Ŀ �˻� (1�� ������ġ)
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPos().IsEqual(vec3(1000,1000,1000)));
		CHECK_EQUAL(true, m_pPlayerBlueMember1->GetPos().IsEqual(vec3(1000,1000,1000)));
		// ���� ��Ŀ �˻� (2�� ������ġ)
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPos().IsEqual(vec3(2000,2000,2000)));
		CHECK_EQUAL(true, m_pPlayerRedMember1->GetPos().IsEqual(vec3(2000,2000,2000)));

		// ���� �����
		m_pPlayerRedLeader->doDie();
		m_pPlayerRedMember1->doDie();
		WaitForScoreBoardCount();
		
		gmgr.pFieldMgr->Update(0.0f);	// ������ �ʵ尡 ������ ƽ

		// ���� �ʵ� �̵���
		RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());
		CheckFieldID(777);
	}

	TEST_FIXTURE(FBattleArena, EffecteeTargetSelector)
	{
		// �ǰ� �׽�Ʈ �غ�
		GTalentInfo* pDmgTalent = test.talent.NewTalentInfo();
		pDmgTalent->m_EffectInfo.m_nRelation = CSEffectInfo::RELATION_ENEMY;
		pDmgTalent->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_HITCAPSULE;
		test.talent.SetTalentDamage(pDmgTalent, 1);
		RUN_LIST(GEntityPlayer, vecAllPlayers, SetHP(100));

		// ���ӱ�Ģ �߰�
		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		gmgr.pBattleArenaMgr->ClearGameRules();
		GGameRule_DeathMatch* pFinishRule = new GGameRule_DeathMatch();
		pFinishRule->SetMaxLifeCounter(1);	// ������ġ �������� 1�� ����
		gmgr.pBattleArenaMgr->AddGameRule(pFinishRule);
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());

		// ���ο� �������� ������
		GBattleArena* pBattleArena1 = gmgr.pBattleArenaMgr->FindBattleArena(m_pPlayerBlueLeader->GetPlayerBattleArena().GetBattleArenaUID());
		RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());

		// �� ���� ��ҷ� ����
		RUN_LIST(GEntityPlayer, vecAllPlayers, Warp(vec3(100,100,0), vec3(0,1,0)));

		// ī�����߿� �ǰ� �ȵ�
		test.talent.UseTalent(m_pPlayerBlueLeader, pDmgTalent, m_pPlayerRedLeader->GetUID());
		CHECK_EQUAL(100, m_pPlayerRedLeader->GetHP());

		// ���ӽ���
		WaitForPlayCount();

		// ���� ��Ʈ�� ��������� �ǰ��� ���� ����
		test.talent.UseTalent(m_pPlayerBlueLeader, pDmgTalent, m_pPlayerBlueMember1->GetUID());
		CHECK_EQUAL(100, m_pPlayerBlueMember1->GetHP());

		// �ٸ� ��Ʈ�� ����� �ǰ��� ��
		test.talent.UseTalent(m_pPlayerBlueLeader, pDmgTalent, m_pPlayerRedLeader->GetUID());
		CHECK_EQUAL(99, m_pPlayerRedLeader->GetHP());
	}

	TEST_FIXTURE(FBattleArena, BattleArenaUID)
	{
		// ������ UID ��� �ȵ����� 
		CheckBattleAreaUID(MUID::Invalid());

		// ���ӱ�Ģ �߰�
		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		gmgr.pBattleArenaMgr->ClearGameRules();
		GGameRule_DeathMatch* pFinishRule = new GGameRule_DeathMatch();
		pFinishRule->SetMaxLifeCounter(2);	// ������ġ �������� 1�� ����
		gmgr.pBattleArenaMgr->AddGameRule(pFinishRule);
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());
		
		// ���ο� �������� ������
		GBattleArena* pBattleArena1 = gmgr.pBattleArenaMgr->FindBattleArena(m_pPlayerBlueLeader->GetPlayerBattleArena().GetBattleArenaUID());
		RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());

		// ������ UID ��� ������
		CheckBattleAreaUID(pBattleArena1->GetUID());

		// ���ӽ���
		WaitForPlayCount();
		// ���� �����
		m_pPlayerRedLeader->doDie();
		m_pPlayerRedMember1->doDie();
		WaitForScoreBoardCount();
		RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());

		// ������ UID ��� �ȵ����� 
		CheckBattleAreaUID(MUID::Invalid());
	}

	TEST_FIXTURE(FBattleArena, RepeatGameStart)
	{
		// ���ӱ�Ģ �߰�
		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		gmgr.pBattleArenaMgr->ClearGameRules();
		GGameRule_DeathMatch* pFinishRule = new GGameRule_DeathMatch();
		pFinishRule->SetMaxLifeCounter(2);	// ������ġ �������� 1�� ����
		gmgr.pBattleArenaMgr->AddGameRule(pFinishRule);

		const int TEST_REPEAT = 4;
		for (int i=0; i<TEST_REPEAT; ++i)
		{
			CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
			CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());

			// ���ο� �������� ������
			GBattleArena* pBattleArena1 = gmgr.pBattleArenaMgr->FindBattleArena(m_pPlayerBlueLeader->GetPlayerBattleArena().GetBattleArenaUID());
			RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());
			CHECK_EQUAL(GBattleArena::COUNTING, pBattleArena1->GetStatus());

			// ���ӽ���
			WaitForPlayCount();
			// ���� �����
			m_pPlayerRedLeader->doDie();
			CHECK_EQUAL(GBattleArena::PLAYING, pBattleArena1->GetStatus());
			m_pPlayerRedMember1->doDie();
			CHECK_EQUAL(GBattleArena::SCOREBOARD, pBattleArena1->GetStatus());
			WaitForScoreBoardCount();
			RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());

			gmgr.pFieldMgr->Update(0.0f); // �ʵ尡 ������ ƽ
		}
	}

	TEST_FIXTURE(FBattleArena, RebirthToCheckPoint)
	{
		// ���ӱ�Ģ �߰�
		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		gmgr.pBattleArenaMgr->ClearGameRules();
		GGameRule_DeathMatch* pFinishRule = new GGameRule_DeathMatch();
		pFinishRule->SetMaxLifeCounter(2);	// ������ġ �������� 1�� ����
		gmgr.pBattleArenaMgr->AddGameRule(pFinishRule);
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());

		// ���ο� �������� ������
		GBattleArena* pBattleArena1 = gmgr.pBattleArenaMgr->FindBattleArena(m_pPlayerBlueLeader->GetPlayerBattleArena().GetBattleArenaUID());
		RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());
		WaitForPlayCount();

		CHECK_EQUAL(false, m_pPlayerRedLeader->IsDead());
		m_pPlayerRedLeader->doDie();
		CHECK_EQUAL(true, m_pPlayerRedLeader->IsDead());
		gsys.pRebirthSystem->RebirthToCheckPoint(m_pPlayerRedLeader);
		CHECK_EQUAL(true, m_pPlayerRedLeader->IsDead());	// ȿ�� ����
	}

	TEST_FIXTURE(FBattleArena, ManualRebirthBySelf)
	{
		// ���ӱ�Ģ �߰�
		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		gmgr.pBattleArenaMgr->ClearGameRules();
		GGameRule_DeathMatch* pFinishRule = new GGameRule_DeathMatch();
		pFinishRule->SetMaxLifeCounter(2);	// ������ġ �������� 2�� ����
		gmgr.pBattleArenaMgr->AddGameRule(pFinishRule);
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());

		// ���ο� �������� ������
		GBattleArena* pBattleArena1 = gmgr.pBattleArenaMgr->FindBattleArena(m_pPlayerBlueLeader->GetPlayerBattleArena().GetBattleArenaUID());
		RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());
		WaitForPlayCount();

		CHECK_EQUAL(false, m_pPlayerRedLeader->IsDead());
		m_pPlayerRedLeader->doDie();
		CHECK_EQUAL(true, m_pPlayerRedLeader->IsDead());
		m_pPlayerRedLeader->doRebirthBySelf();
		CHECK_EQUAL(false, m_pPlayerRedLeader->IsDead());	
	}

	TEST_FIXTURE(FBattleArena, ManualRebirthByOther)
	{
		// ���ӱ�Ģ �߰�
		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		gmgr.pBattleArenaMgr->ClearGameRules();
		GGameRule_DeathMatch* pFinishRule = new GGameRule_DeathMatch();
		pFinishRule->SetMaxLifeCounter(2);	// ������ġ �������� 2�� ����
		gmgr.pBattleArenaMgr->AddGameRule(pFinishRule);
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());

		// ���ο� �������� ������
		GBattleArena* pBattleArena1 = gmgr.pBattleArenaMgr->FindBattleArena(m_pPlayerBlueLeader->GetPlayerBattleArena().GetBattleArenaUID());
		RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());
		WaitForPlayCount();

		CHECK_EQUAL(false, m_pPlayerRedLeader->IsDead());
		m_pPlayerRedLeader->doDie();
		CHECK_EQUAL(true, m_pPlayerRedLeader->IsDead());
		m_pPlayerRedLeader->doRebirthByOther();
		CHECK_EQUAL(false, m_pPlayerRedLeader->IsDead());	
	}

	TEST_FIXTURE(FBattleArena, TimeoutRebirth)
	{
		// ���ӱ�Ģ �߰�
		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		gmgr.pBattleArenaMgr->ClearGameRules();
		GGameRule_DeathMatch* pFinishRule = new GGameRule_DeathMatch();
		pFinishRule->SetMaxLifeCounter(2);	// ������ġ �������� 2�� ����
		gmgr.pBattleArenaMgr->AddGameRule(pFinishRule);
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());

		// ���ο� �������� ������
		GBattleArena* pBattleArena1 = gmgr.pBattleArenaMgr->FindBattleArena(m_pPlayerBlueLeader->GetPlayerBattleArena().GetBattleArenaUID());
		RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());
		WaitForPlayCount();

		CHECK_EQUAL(false, m_pPlayerRedLeader->IsDead());
		m_pPlayerRedLeader->doDie();
		// ��Ȱ�ð����� ���
		for (int i=0; i<GConst::ARENA_DEATHMATCH_REBIRTH_TIME; i++)
		{
			CHECK_EQUAL(true, m_pPlayerRedLeader->IsDead());
			gsys.pServer->Update(1.0f);
		}
		CHECK_EQUAL(false, m_pPlayerRedLeader->IsDead());	
	}

	TEST_FIXTURE(FBattleArena, RebirthPos)
	{
		// ���ӱ�Ģ �߰�
		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->AddGameRule(new GGameRule_DeathMatch());
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		gmgr.pBattleArenaMgr->ClearGameRules();
		GGameRule_DeathMatch* pFinishRule = new GGameRule_DeathMatch();
		pFinishRule->SetMaxLifeCounter(2);	// ������ġ �������� 2�� ����
		gmgr.pBattleArenaMgr->AddGameRule(pFinishRule);
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());

		// ���ο� �������� ������
		GBattleArena* pBattleArena1 = gmgr.pBattleArenaMgr->FindBattleArena(m_pPlayerBlueLeader->GetPlayerBattleArena().GetBattleArenaUID());
		RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());
		WaitForPlayCount();

		m_pPlayerRedLeader->SetPos(vec3(777,777,777));	// �̻��� ������ �̵�
		CHECK_EQUAL(false, m_pPlayerRedLeader->IsDead());
		m_pPlayerRedLeader->doDie();
		CHECK_EQUAL(true, m_pPlayerRedLeader->IsDead());
		gsys.pRebirthSystem->RebirthToPlaceInBattleArena(m_pPlayerRedLeader);
		CHECK_EQUAL(false, m_pPlayerRedLeader->IsDead());	
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPos().IsEqual(vec3(2000,2000,2000)));	// ���� ��Ŀ�� �̵��� 
	}

	TEST_FIXTURE(FBattleArena, RebirthWhenGameFinished)
	{
		// ���ӱ�Ģ �߰�
		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->AddGameRule(new GGameRule_DeathMatch());
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		gmgr.pBattleArenaMgr->ClearGameRules();
		GGameRule_DeathMatch* pFinishRule = new GGameRule_DeathMatch();
		pFinishRule->SetMaxLifeCounter(2);	// ������ġ �������� 1�� ����
		gmgr.pBattleArenaMgr->AddGameRule(pFinishRule);
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());

		// ���ο� �������� ������
		GBattleArena* pBattleArena1 = gmgr.pBattleArenaMgr->FindBattleArena(m_pPlayerBlueLeader->GetPlayerBattleArena().GetBattleArenaUID());
		RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());
		WaitForPlayCount();
		// ���� �����
		m_pPlayerRedLeader->doDie();
		m_pPlayerRedMember1->doDie();
		WaitForScoreBoardCount();
		RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());
		// �����ڵ� ��Ȱ��
		CHECK_EQUAL(false, m_pPlayerRedLeader->IsDead());	
		CHECK_EQUAL(false, m_pPlayerRedMember1->IsDead());
	}

	TEST_FIXTURE(FBattleArena, PartyDelete)
	{
		CHECK_EQUAL(0, gmgr.pBattleArenaMgr->GetTeamQty());
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(1, gmgr.pBattleArenaMgr->GetTeamQty());
		
		// ��Ƽ ����
		GUTHelper_Party::DeleteParty(m_pBlueParty);
		m_pBlueParty = NULL;

		CHECK_EQUAL(0, gmgr.pBattleArenaMgr->GetTeamQty());
	}

	TEST_FIXTURE(FBattleArena, PartyDeleteInPlaying)
	{
		// ���ӱ�Ģ �߰�
		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->AddGameRule(new GGameRule_DeathMatch());
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		gmgr.pBattleArenaMgr->ClearGameRules();
		GGameRule_DeathMatch* pFinishRule = new GGameRule_DeathMatch();
		pFinishRule->SetMaxLifeCounter(2);	// ������ġ �������� 1�� ����
		gmgr.pBattleArenaMgr->AddGameRule(pFinishRule);
		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());

		// ���ο� �������� ������
		GBattleArena* pBattleArena1 = gmgr.pBattleArenaMgr->FindBattleArena(m_pPlayerBlueLeader->GetPlayerBattleArena().GetBattleArenaUID());
		RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());
		WaitForPlayCount();

		CHECK_EQUAL(GBattleArena::PLAYING, pBattleArena1->GetStatus());

		// ��Ƽ ����
		GUTHelper_Party::DeleteParty(m_pBlueParty);
		m_pBlueParty = NULL;

		CHECK_EQUAL(GBattleArena::SCOREBOARD, pBattleArena1->GetStatus());
	}

	TEST_FIXTURE(FBattleArena, DestroyFieldWhenGameFinished)
	{
		// ���ӱ�Ģ �߰�
		gmgr.pBattleArenaMgr->GetMatcher().ClearRules();
		gmgr.pBattleArenaMgr->AddGameRule(new GGameRule_DeathMatch());
		gmgr.pBattleArenaMgr->GetMatcher().AddRule(new GMatchRule_SameTeamMember());
		gmgr.pBattleArenaMgr->ClearGameRules();
		GGameRule_DeathMatch* pFinishRule = new GGameRule_DeathMatch();
		pFinishRule->SetMaxLifeCounter(2);	// ������ġ �������� 1�� ����
		gmgr.pBattleArenaMgr->AddGameRule(pFinishRule);

		CHECK_EQUAL(0, gmgr.pFieldMgr->GetDynamicFieldGroupQty());

		CHECK_EQUAL(true, m_pPlayerBlueLeader->GetPlayerBattleArena().RegisterEntry());
		CHECK_EQUAL(true, m_pPlayerRedLeader->GetPlayerBattleArena().RegisterEntry());
		// ���ο� �������� ������
		GBattleArena* pBattleArena1 = gmgr.pBattleArenaMgr->FindBattleArena(m_pPlayerBlueLeader->GetPlayerBattleArena().GetBattleArenaUID());
		RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());
		
		CHECK_EQUAL(1, gmgr.pFieldMgr->GetDynamicFieldGroupQty());

		WaitForPlayCount();
		// ���� �����
		m_pPlayerRedLeader->doDie();
		m_pPlayerRedMember1->doDie();
		WaitForScoreBoardCount();
		RUN_LIST(GEntityPlayer, vecAllPlayers, GetPlayerField().GetFieldEntry().OnEnterFieldComplete());
		gmgr.pFieldMgr->Update(0.0f); // �ʵ尡 ������ ƽ

		CHECK_EQUAL(0, gmgr.pFieldMgr->GetDynamicFieldGroupQty());
	}
}
