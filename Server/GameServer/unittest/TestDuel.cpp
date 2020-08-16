#include "StdAfx.h"
#include "GTestWrapper.h"
#include "SUnitTestUtil.h"
#include "GUTHelper.h"
#include "MockField.h"
#include "MockLink.h"
#include "GDuelMgr.h"
#include "GDuel.h"
#include "CCommandTable.h"
#include "GEntityPlayer.h"
#include "GModuleCombat.h"
#include "GCombatCalculator.h"
#include "GConst.h"
#include "GModuleAI.h"
#include "GPartySystem.h"
#include "GFieldSystem.h"
#include "GDynamicFieldMaster.h"
#include "TestCombatCalculator.h"
#include "GTestForward.h"
#include "GServer.h"
#include "GSharedField.h"
#include "GPlayerField.h"
#include "FBaseMockLink.h"
#include "GPlayerFieldDynamic.h"
#include "GPlayerEnemyInfoSender.h"

#define KILL_DAMAGE 99999
#define RUNAWAY_DISTANCE 1000+DUEL_RADIUS
vec3 vCommonPos = vec3(1000, 1000, 0);



SUITE(Duel)
{
	struct FDuel : public FBaseMockLink
	{
		FDuel()
		{ 
			m_bOldPvPMode = GConst::PVP_MODE;
			GConst::PVP_MODE = false;

			// ������� NPCID ���
			GNPCInfo* pinfoNPC = new GNPCInfo();
			pinfoNPC->nID = DUEL_FLAG_NPCID;
			pinfoNPC->nMaxHP = 50;
			
			gmgr.pNPCInfoMgr->Insert(pinfoNPC);
		
			m_pField = GUTHelper_Field::NewMockField();
			m_pPlayerRequester	= NewPlayer(L"Requester", m_pLinkRequester);
			m_pPlayerTarget		= NewPlayer(L"Target", m_pLinkTarget);
				


			SetIgnoreCommonCommandDefaultPlayers();
			gsys.pServer->FrameCounterClear();
		}

		~FDuel() 
		{
			GConst::PVP_MODE = m_bOldPvPMode;
			m_pField->Destroy();
		}

		void MakeParty(GEntityPlayer* pLeader, GEntityPlayer* pMember)
		{
			GParty* pParty = GUTHelper_Party::CreateParty(pLeader);
			GUTHelper_Party::AddMember(pParty, pMember);
		}

		GEntityPlayer* NewPlayer(const wchar_t* pszName, MockLink*& pLink)
		{
			GEntityPlayer* pPlayer = GUTHelper::NewEntityPlayer(m_pField, vCommonPos);
			swprintf_s(pPlayer->GetPlayerInfo()->szName, pszName);
			pLink = NewLink(pPlayer);
			return pPlayer;
		}

		GTalentInfo* MakeSimpleTalent(int nDamage, int nTalentID = 1)
		{
			GTalentInfo* pMagicTalent = test.talent.NewMagicTalentInfo(nTalentID);
			test.talent.SetTalentDamage(pMagicTalent, nDamage);
			pMagicTalent->m_fDurationTime = 1.0f;
			pMagicTalent->m_fCastingTime = 0.0f;
			pMagicTalent->m_Resist.m_nDifficulty = 255;	// ���� �Ұ����ϰ� ����
			pMagicTalent->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
			pMagicTalent->m_EffectInfo.m_nRelation = CSEffectInfo::RELATION_ENEMY;
			pMagicTalent->Cooking();
			return pMagicTalent;
		}

		GField* MakeSharedField(int nFieldID)
		{
			GFieldInfo* pFieldInfo = new GFieldInfo;
			pFieldInfo->m_bDynamic = false;
			pFieldInfo->m_nFieldID = nFieldID;
			pFieldInfo->m_nDefaultChannelCount = 0;
			pFieldInfo->SetFieldMaxXY_ForTest(50000.0f, 50000.0f);
			gmgr.pFieldInfoMgr->Insert(pFieldInfo);

			GSharedField* pTestField = GUTHelper_Field::NewSharedField(pFieldInfo, 1);
			VALID_RET(pTestField, NULL);
			return pTestField;
		}

		void MakeDynamicField(int nGroupID, int nFieldID)
		{
			FIELD_GROUP_INFO* info = new FIELD_GROUP_INFO;
			info->nID = nGroupID;
			GFieldInfo* pFieldInfo = new GFieldInfo;
			pFieldInfo->m_bDynamic = true;
			pFieldInfo->m_nFieldID = nFieldID;
			pFieldInfo->SetFieldMaxXY_ForTest(50000.0f, 50000.0f);

			MARKER_INFO marker;
			marker.nID = 1;
			marker.nFieldID = nFieldID;
			marker.vPoint = vCommonPos;
			pFieldInfo->InsertMarkerInfo(marker);

			info->vecFieldInfo.push_back(pFieldInfo);
			info->pStartFieldInfo = pFieldInfo;
			info->pStartMarkerInfo = pFieldInfo->FindMarker(1);

			gmgr.pFieldInfoMgr->Insert(pFieldInfo);
			gmgr.pFieldInfoMgr->InsertGroup(info);
		}

		void SetIgnoreCommonCommand(MockLink* pLink)
		{
			pLink->AddIgnoreID(MC_ACTION_DIE);	
			pLink->AddIgnoreID(MC_NPC_NARRATION);	
			pLink->AddIgnoreID(MC_FIELD_IN_NPC);	
			pLink->AddIgnoreID(MC_FIELD_OUT_NPC);
			pLink->AddIgnoreID(MC_FIELD_IN_PLAYER);
			pLink->AddIgnoreID(MC_FIELD_OUT_PLAYER);
			pLink->AddIgnoreID(MC_FIELD_SECTOR_ENTITY_INFO);
			pLink->AddIgnoreID(MC_TRIGGER_UPDATE_USABLE_SENSOR);
			pLink->AddIgnoreID(MC_ACTION_USE_TALENT);
			pLink->AddIgnoreID(MC_ACTION_TALENT_HIT);
			pLink->AddIgnoreID(MC_CHAR_ENEMY_INFO);
			pLink->AddIgnoreID(MC_NPC_CHANGE_COMBAT_TARGET);
			pLink->AddIgnoreID(MC_ACTION_TALENT_HIT_NO_MF);
			pLink->AddIgnoreID(MC_FIELD_SET_TIME);
			pLink->AddIgnoreID(MC_CHAR_ENEMY_INFO_CLEAR);
			pLink->AddIgnoreID(MC_NPCINTERACTION_END);
			pLink->AddIgnoreID(MC_NPCINTERACTION_ICON);
			pLink->AddIgnoreID(MC_DUEL_DEFEATED);
			pLink->AddIgnoreID(MC_DUEL_READY_COUNT);	
		}

		void SetIgnoreCommonCommandDefaultPlayers()
		{
			SetIgnoreCommonCommand(m_pLinkRequester);
			SetIgnoreCommonCommand(m_pLinkTarget);
		}

		void BeginCommandRecord()
		{
			m_pLinkRequester->ResetCommands();
			m_pLinkTarget->ResetCommands();
		}

		void WaitDuelCount()
		{
			for (int i=0; i<DUEL_READY_COUNT; i++)
			{
				m_pField->Update(1.0f);
			}
		}

		void DuelQuestion()
		{
			m_pLinkRequester->OnRecv(MC_DUEL_REQUEST,		1, NEW_UID(m_pPlayerTarget->GetUID()));
		}

		void DuelRespond(bool bAnswer)
		{
			m_pLinkTarget->OnRecv(MC_DUEL_QUESTION_RESPOND, 1, NEW_BOOL(bAnswer));
		}

		void DuelStart()
		{
			DuelQuestion();
			DuelRespond(true);
			WaitDuelCount();
		}

		void DuelFinish(GEntityPlayer* pLoser)
		{
			pLoser->SetPos(vec3(RUNAWAY_DISTANCE, 0, 0));	// ����ħ
			m_pField->Update(0.1f);
		}

		void CheckDuelCancel(bool bRequesterOnly=false)
		{
			m_pField->Update(0.0f);
			CHECK_EQUAL(m_pLinkRequester->GetCommand(0).GetID(), MC_DUEL_CANCEL);
			if (!bRequesterOnly)
			{
				CHECK_EQUAL(m_pLinkTarget->GetCommand(0).GetID(), MC_DUEL_CANCEL);
			}
		}

		void CheckDuelFinished(GEntityPlayer* pWinner, GEntityPlayer* pLoser)
		{
			MockLink* pWinnerLink = pWinner==m_pPlayerRequester?m_pLinkRequester:m_pLinkTarget;
			const minet::MCommand& Command = pWinnerLink->GetCommand(0);
			CHECK_EQUAL(Command.GetID(), MC_DUEL_FINISHED);
			if (Command.GetParameterCount() < 2)						 { CHECK(false); return; }
			MUID uidWinner;
			MUID uidLoser;
			if (!Command.GetParameter(&uidWinner,	0, MPT_UID))		{ CHECK(false); return; }
			if (!Command.GetParameter(&uidLoser,	1, MPT_UID))		{ CHECK(false); return; }
			CHECK_EQUAL(uidWinner, pWinner->GetUID());
			CHECK_EQUAL(uidLoser, pLoser->GetUID());
		}

		GUTHelper		m_Helper;
		MockField*		m_pField;
		GEntityPlayer*	m_pPlayerRequester;
		GEntityPlayer*	m_pPlayerTarget;
		GEntityPlayer*	m_pPlayerRequester1;
		GEntityPlayer*	m_pPlayerTarget1;
		GEntityPlayer*	m_pPlayerRequester2;
		GEntityPlayer*	m_pPlayerTarget2;
		MockLink*		m_pLinkRequester;
		MockLink*		m_pLinkTarget;
		MockLink*		m_pLinkRequester1;
		MockLink*		m_pLinkTarget1;
		MockLink*		m_pLinkRequester2;
		MockLink*		m_pLinkTarget2;
		bool			m_bOldPvPMode;

		GTestMgrWrapper<GFieldInfoMgr>				m_FieldInfoMgr;
		GTestMgrWrapper<GFieldMgr>					m_FieldMgr;
		GTestMgrWrapper<GTalentInfoMgr>				m_TalentInfoMgr;
		GTestMgrWrapper<GNPCInfoMgr>				m_NPCInfoMgrWrapper;
		GTestSysWrapper2<GAttackDamageCalculator, SimpleAttackDamageCalculator>	m_AttackDamageCalculator;
		GTestSysWrapper2<GCombatCalculator, SimpleCombatCalculator>				m_CombatCalculator;
		GTestSysWrapper2<GSystem, GUnitTestUtil::FakeSystem>						m_FakeSystemWrapper;
		DECLWRAPPER_NPCMgr;
	};

	TEST_FIXTURE(FDuel, DuelRequest)
	{
		DuelQuestion();
	}

	TEST_FIXTURE(FDuel, IsCombatInDuel)
	{
		DuelStart();
		CHECK_EQUAL(m_pPlayerRequester->HasDuel(), true);
		CHECK_EQUAL(m_pPlayerTarget->HasDuel(), true);
		DuelFinish(m_pPlayerRequester);
		CHECK_EQUAL(m_pPlayerRequester->HasDuel(), false);
		CHECK_EQUAL(m_pPlayerTarget->HasDuel(), false);
	}

	TEST_FIXTURE(FDuel, DuelQuestionRefuse)
	{
		CHECK_EQUAL(m_pField->GetDuelMgr()->GetCount(), 0);
		DuelQuestion();
		CHECK_EQUAL(m_pField->GetDuelMgr()->GetCount(), 1);
		BeginCommandRecord();
		DuelRespond(false);
		CheckDuelCancel();
	}

	TEST_FIXTURE(FDuel, DuelQuestionAccept)
	{
		DuelQuestion();
		BeginCommandRecord();
		DuelRespond(true);

		CHECK_EQUAL(m_pLinkRequester->GetCommand(0).GetID(), MC_DUEL_READY);
		CHECK_EQUAL(m_pLinkTarget->GetCommand(0).GetID(), MC_DUEL_READY);
	}

	TEST_FIXTURE(FDuel, DuelReadyToFight)
	{
		DuelQuestion();
		DuelRespond(true);
		
		BeginCommandRecord();
		WaitDuelCount();
		CHECK_EQUAL(m_pLinkRequester->GetCommand(0).GetID(), MC_DUEL_FIGHT);
		CHECK_EQUAL(m_pLinkTarget->GetCommand(0).GetID(), MC_DUEL_FIGHT);
	}

	TEST_FIXTURE(FDuel, DuelCalcCenterPosition)
	{
		m_pPlayerRequester->SetPos(vec3(1000, 1000, 0));
		m_pPlayerTarget->SetPos(vec3(2000, 2000, 0));
		
		GDuel duel(SUnitTestUtil::NewUID(), m_pField, m_pPlayerRequester, m_pPlayerTarget);
		duel.Ready();
		CHECK(duel.GetCenterPos().IsEqual(vec3(1500, 1500, 0)));
	}

	TEST_FIXTURE(FDuel, DuelFinishedDueKilled)
	{
		GTalentInfo* pKillTalentInfo = MakeSimpleTalent(KILL_DAMAGE);

		DuelStart();
		BeginCommandRecord();
		
		// �������� ��븦 ����
		TALENT_TARGET_INFO target_info;
		target_info.uidTarget = m_pPlayerTarget->GetUID();
		m_pPlayerRequester->GetModuleCombat()->UseTalent(pKillTalentInfo, target_info, true);
		m_pField->Update(0.0f);
		gsys.pServer->Update(0.1f);	

		CheckDuelFinished(m_pPlayerRequester, m_pPlayerTarget);
	}

	TEST_FIXTURE(FDuel, DuelFinishedDuePlayerOffline)
	{
		DuelStart();

		// �÷��̾� ���� ����
		BeginCommandRecord();
		m_pPlayerTarget->DeleteMe();
		m_pField->Update(0.0f);

		CheckDuelFinished(m_pPlayerRequester, m_pPlayerTarget);
	}

	TEST_FIXTURE(FDuel, DuelFinishedDuePlayerOfflineWhenQuestionPhase)
	{
		DuelQuestion();

		// �÷��̾� ���� ����
		BeginCommandRecord();
		m_pPlayerTarget->DeleteMe();
		m_pField->Update(0.0f);

		CheckDuelCancel(true);
	}

	TEST_FIXTURE(FDuel, DuelFinishedDuePlayerOfflineWhenReadyPhase)
	{
		DuelQuestion();
		DuelRespond(true);

		// �÷��̾� ���� ����
		BeginCommandRecord();
		m_pPlayerTarget->DeleteMe();
		m_pField->Update(0.0f);

		CheckDuelCancel(true);
	}

	TEST_FIXTURE(FDuel, DuelFinishedDueRunaway)
	{
		DuelStart();
		BeginCommandRecord();

		// ����ħ
		m_pPlayerRequester->SetPos(vec3(RUNAWAY_DISTANCE, 0, 0));	
		m_pField->Update(0.1f);

		CheckDuelFinished(m_pPlayerTarget, m_pPlayerRequester);
	}

	TEST_FIXTURE(FDuel, DuelFinishedDueRunawayWhenQuestionPhase)
	{
		DuelQuestion();
		BeginCommandRecord();

		// ����ħ
		m_pPlayerRequester->SetPos(vec3(RUNAWAY_DISTANCE, 0, 0));	
		m_pField->Update(0.1f);

		CheckDuelCancel();
	}

	TEST_FIXTURE(FDuel, DuelFinishedDueRunawayWhenReadyPhase)
	{
		DuelQuestion();
		DuelRespond(true);
		BeginCommandRecord();

		// ����ħ
		m_pPlayerRequester->SetPos(vec3(RUNAWAY_DISTANCE, 0, 0));	
		m_pField->Update(0.1f);

		CheckDuelCancel();
	}

	TEST_FIXTURE(FDuel, DuelFinishedDueRunawayToOhterField)
	{
		int nTestFieldID = 7777;
		GField* pOhterField = MakeSharedField(nTestFieldID);

		DuelStart();
		BeginCommandRecord();

		m_pPlayerRequester->GetPlayerField().Gate(nTestFieldID, vec3(1000, 1000, 0), vec3(0,1,0), false);
		gmgr.pFieldMgr->Update(0.1f);
		m_pPlayerRequester->GetPlayerField().GetFieldEntry().OnEnterFieldComplete();
		CHECK_EQUAL(m_pPlayerRequester->GetField(), pOhterField);	// �ʵ��̵� ����
		m_pField->Update(0.1f);

		CheckDuelFinished(m_pPlayerTarget, m_pPlayerRequester);
	}

	TEST_FIXTURE(FDuel, SightSeerRecvieveFinishedPacket)
	{
		GEntityPlayer* pSightSeer = GUTHelper::NewEntityPlayer(m_pField, vCommonPos);
		MockLink* pLinkSeer = NewLink(pSightSeer);
		SetIgnoreCommonCommand(pLinkSeer);

		DuelStart();
		pLinkSeer->ResetCommands();
		DuelFinish(m_pPlayerRequester);

		CHECK_EQUAL(pLinkSeer->GetCommand(0).GetID(), MC_DUEL_FINISHED);
	}

	TEST_FIXTURE(FDuel, DuelHittable)
	{
		// �÷��̾ ���� �ŷ�Ʈ
		GTalentInfo* pTalentInfo = test.talent.MakeDamageTalent(10);
		pTalentInfo->m_EffectInfo.m_nRelation = CSEffectInfo::RELATION_ENEMY;
		TALENT_TARGET_INFO target_info;

		// ���� ��û
		DuelQuestion();

		// ���� ��û�߿� �ǰݵ��� �ʴ´�.
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 300);
		test.talent.UseTalent(m_pPlayerRequester, pTalentInfo, m_pPlayerTarget->GetUID());
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 300);

		// ���� ����
		DuelRespond(true);

		// �������� ���� �ǰݵ� �� ����
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 300);
		test.talent.UseTalent(m_pPlayerRequester, pTalentInfo, m_pPlayerTarget->GetUID());
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 300);

		// ī��Ʈ ���
		WaitDuelCount();

		// �����߿� ���� �ǰݵ� �� ����
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 300);
		test.talent.UseTalent(m_pPlayerRequester, pTalentInfo, m_pPlayerTarget->GetUID());
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 290);

		// ���� ����
		DuelFinish(m_pPlayerRequester);

		// ������ ������ �ٽ� �ǰݵ� �� ����
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 290);
		test.talent.UseTalent(m_pPlayerRequester, pTalentInfo, m_pPlayerTarget->GetUID());
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 290);
	}

	TEST_FIXTURE(FDuel, DuelFlag)
	{
		CHECK_EQUAL(m_pField->GetEntityQty(ETID_NPC), 0); // ����� ��
		DuelStart();
		CHECK_EQUAL(m_pField->GetEntityQty(ETID_NPC), 1); // ����� ��
		DuelFinish(m_pPlayerRequester);
		m_pField->Update(0.0f);	// ����� ������� ���� �ʿ��� ƽ
		CHECK_EQUAL(m_pField->GetEntityQty(ETID_NPC), 0); // ����� ��
	}

	TEST_FIXTURE(FDuel, KilledByNonDuelOpponent)
	{
		GNPCInfo NPCInfo;
		NPCInfo.nID = 1;
		GEntityNPC* pNonDuelOpponent = m_pField->SpawnTestNPC(&NPCInfo, vCommonPos);
		pNonDuelOpponent->GetNPCStatus().nAA = NAAT_ALWAYS;
		
		DuelStart();
		BeginCommandRecord();

		// ��3�ڰ� �������� �������� ����� ����
		GTalentInfo* pKillTalentInfo = MakeSimpleTalent(KILL_DAMAGE);
		test.talent.UseTalent(pNonDuelOpponent, pKillTalentInfo, m_pPlayerTarget->GetUID());

		CHECK_EQUAL(m_pPlayerTarget->IsDead(), true);
		CheckDuelFinished(m_pPlayerRequester, m_pPlayerTarget);
	}

	// NPC�� ���� ���ÿ� ������ 2���� ���� ���
	TEST_FIXTURE(FDuel, KilledAllDuelMemberByNonDuelOpponent)
	{
		GNPCInfo NPCInfo;
		NPCInfo.nID = 1;
		GEntityNPC* pNonDuelOpponent = m_pField->SpawnTestNPC(&NPCInfo, vCommonPos);
		pNonDuelOpponent->GetNPCStatus().nAA = NAAT_ALWAYS;

		DuelStart();
		BeginCommandRecord();

		// ��3�ڰ� �������� �������� ����� ����
		GTalentInfo* pKillTalentInfo = test.talent.MakeDamageTalent(KILL_DAMAGE);
		test.talent.UseTalent(pNonDuelOpponent, pKillTalentInfo, m_pPlayerTarget->GetUID());

		// ���� �̱��� ��Ȯ�� �� (���������� ��ȯ���� ��� �÷��̾ ���� ������ ����)
		CHECK_EQUAL(m_pPlayerRequester->IsDead(), true);
		CHECK_EQUAL(m_pPlayerTarget->IsDead(), true);
		CheckDuelFinished(m_pPlayerTarget, m_pPlayerRequester);
	}

	TEST_FIXTURE(FDuel, KilledWhenQuestionPhase)
	{
		GNPCInfo NPCInfo;
		NPCInfo.nID = 1;
		GEntityNPC* pNonDuelOpponent = m_pField->SpawnTestNPC(&NPCInfo, vCommonPos);
		pNonDuelOpponent->GetNPCStatus().nAA = NAAT_ALWAYS;

		DuelQuestion();
				
		BeginCommandRecord();

		// ��3�ڰ� �������� �������� ����� ����
		GTalentInfo* pKillTalentInfo = MakeSimpleTalent(KILL_DAMAGE);
		test.talent.UseTalent(pNonDuelOpponent, pKillTalentInfo, m_pPlayerTarget->GetUID());

		CHECK_EQUAL(m_pPlayerTarget->IsDead(), true);
		CheckDuelCancel();
	}

	TEST_FIXTURE(FDuel, KilledWhenReadyPhase)
	{
		GNPCInfo NPCInfo;
		NPCInfo.nID = 1;
		GEntityNPC* pNonDuelOpponent = m_pField->SpawnTestNPC(&NPCInfo, vCommonPos);
		pNonDuelOpponent->GetNPCStatus().nAA = NAAT_ALWAYS;

		DuelQuestion();
		DuelRespond(true);

		BeginCommandRecord();
		GTalentInfo* pKillTalentInfo = MakeSimpleTalent(KILL_DAMAGE);
		test.talent.UseTalent(pNonDuelOpponent, pKillTalentInfo, m_pPlayerTarget->GetUID());

		CHECK_EQUAL(m_pPlayerTarget->IsDead(), true);
		CheckDuelCancel();
	}

	// MagicArea ����
	//TEST_FIXTURE(FDuel, MagicAreaInDuel)
	//{
	//	GTalentInfo* pMagicAreaTalentInfo = MakeMagicAreaTalent(10, 1, 3.0f, 1.0f);

	//	TALENT_TARGET_INFO target_info;
	//	target_info.uidTarget = m_pPlayerTarget->GetUID();
	//	float fPeriodTime = pMagicAreaTalentInfo->m_fPeriodTime;
	//	float fDurationTime = pMagicAreaTalentInfo->m_fDurationTime;

	//	// ���� ������ �ȸ���
	//	m_pPlayerRequester->GetModuleCombat()->UseTalent(pMagicAreaTalentInfo, target_info, false);
	//	m_pField->Update(0.1f);
	//	CHECK_EQUAL(m_pPlayerTarget->GetHP(), 10000);
	//	gsys.pSystem->Sleep(fPeriodTime);
	//	m_pField->Update(fPeriodTime);	// ȿ���ֱⰡ ������
	//	CHECK_EQUAL(m_pPlayerTarget->GetHP(), 10000);
	//	gsys.pSystem->Sleep(fDurationTime);
	//	m_pField->Update(fDurationTime);	// �ŷ�Ʈ ���ӽð� ������

	//	DuelStart();
	//	
	//	// ���� �߿��� ����
	//	m_pPlayerRequester->GetModuleCombat()->UseTalent(pMagicAreaTalentInfo, target_info, false);
	//	m_pField->Update(0.1f);
	//	CHECK_EQUAL(m_pPlayerTarget->GetHP(), 10000);
	//	gsys.pSystem->Sleep(fPeriodTime);
	//	m_pField->Update(fPeriodTime);	// ȿ���ֱⰡ ������
	//	CHECK_EQUAL(m_pPlayerTarget->GetHP(), 9990);
	//	gsys.pSystem->Sleep(fPeriodTime);
	//	m_pField->Update(fPeriodTime);	// ȿ���ֱⰡ ������
	//	CHECK_EQUAL(m_pPlayerTarget->GetHP(), 9980);
	//	gsys.pSystem->Sleep(fPeriodTime);
	//	m_pField->Update(fPeriodTime);	// ȿ���ֱⰡ ������
	//	CHECK_EQUAL(m_pPlayerTarget->GetHP(), 9970);
	//	gsys.pSystem->Sleep(fPeriodTime);
	//	m_pField->Update(fPeriodTime);	// ȿ���ֱⰡ ������
	//	CHECK_EQUAL(m_pPlayerTarget->GetHP(), 9970);

	//	DuelFinish(m_pPlayerRequester);

	//	// ���� �Ŀ��� �ȸ���
	//	m_pPlayerRequester->GetModuleCombat()->UseTalent(pMagicAreaTalentInfo, target_info, false);
	//	m_pField->Update(0.1f);
	//	CHECK_EQUAL(m_pPlayerTarget->GetHP(), 9970);
	//	gsys.pSystem->Sleep(fPeriodTime);
	//	m_pField->Update(fPeriodTime);	// ȿ���ֱⰡ ������
	//	CHECK_EQUAL(m_pPlayerTarget->GetHP(), 9970);
	//	gsys.pSystem->Sleep(fDurationTime);
	//	m_pField->Update(fDurationTime);	// �ŷ�Ʈ ���ӽð� ������
	//}

	TEST_FIXTURE(FDuel, ProjectileInDuel)
	{
		int nTalentID = 1;
		int nDamage = 10;

		GTalentInfo* infoTalent = m_Helper.NewTalentInfo(nTalentID);
		infoTalent->m_nSkillType = ST_ARCHERY;	
		infoTalent->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
		infoTalent->m_EffectInfo.m_nRelation = CSEffectInfo::RELATION_ENEMY;
		
		// �߻�ü �̺�Ʈ �߰�
		CSTalentEventInfo talentEventInfo;
		talentEventInfo.m_ProjectileInfo.m_nType = TPT_MISSILE;	// �����̵��ϴ� �߻�ü
		talentEventInfo.m_ProjectileInfo.m_fHitRadius = 100;		// �߻�ü ũ��� 1����
		infoTalent->m_Events.m_vecActEvents.push_back(talentEventInfo);

		m_Helper.SetTalentDamage(infoTalent, nDamage);		// ������ 1�� ����
		infoTalent->Cooking();

		TALENT_TARGET_INFO target_info;
		target_info.uidTarget = m_pPlayerTarget->GetUID();

		// ���� ������ �ȸ���
		test.talent.UseTalent(m_pPlayerRequester, infoTalent, target_info);
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 300);

		DuelStart();

		// ���� �߿��� ����
		test.talent.UseTalent(m_pPlayerRequester, infoTalent, target_info);
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 290);

		DuelFinish(m_pPlayerRequester);

		// ���� �Ŀ��� �ȸ���
		m_pPlayerRequester->GetModuleCombat()->UseTalent(infoTalent, target_info, false);
		m_pField->Update(0.1f);
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 290);
	}

	TEST_FIXTURE(FDuel, MakeDuelInQuestionPhase)
	{
		CHECK_EQUAL(m_pPlayerRequester->HasDuel(), false);
		CHECK_EQUAL(m_pPlayerTarget->HasDuel(), false);
		DuelQuestion();
		CHECK_EQUAL(m_pPlayerRequester->HasDuel(), true);
		CHECK_EQUAL(m_pPlayerTarget->HasDuel(), true);
	}

	TEST_FIXTURE(FDuel, DontDuelInTrialField)
	{
		int nTestSharedFieldID = 100;
		int nTestDynamicGroupID = 10000;
		int nTestDynamicFieldID = 7777;
		GField* pOhterField = MakeSharedField(nTestSharedFieldID);
		MakeDynamicField(nTestDynamicGroupID, nTestDynamicFieldID);

		// �� �÷��̾ ��Ƽ�� ��
		GParty* pParty = GUTHelper_Party::CreateParty(m_pPlayerTarget, m_pPlayerRequester);
		CHECK(pParty != NULL);

		// Ż�� ��Ŀ ����
		MARKER_INFO markerStart;
		markerStart.nFieldID = nTestSharedFieldID;
		m_pPlayerRequester->GetPlayerField().GetFieldDynamic().SetEnterPos(markerStart);
		m_pPlayerTarget->GetPlayerField().GetFieldDynamic().SetEnterPos(markerStart);

		// �����ʵ� ���� (���� ��Ƽ�� ���� �����ʵ�� ���Եȴ�.)
		if (!m_pPlayerRequester->GetPlayerField().GateToTrial(nTestDynamicGroupID))
		{
			CHECK(FALSE && "���� �ʵ� ���� ����"); 
			return;
		}
		if (!m_pPlayerTarget->GetPlayerField().GateToTrial(nTestDynamicGroupID))
		{
			CHECK(FALSE && "���� �ʵ� ���� ����"); 
			return;
		}

		GDynamicFieldMaster* pFieldGroup1 = gmgr.pFieldMgr->GetDynamicFieldMaster(m_pPlayerRequester);
		GDynamicFieldMaster* pFieldGroup2 = gmgr.pFieldMgr->GetDynamicFieldMaster(m_pPlayerTarget);
		m_pPlayerRequester->GetPlayerField().Gate(nTestDynamicFieldID, vec3(1000, 1000, 1000), vec3(0,1,0), false);
		m_pPlayerTarget->GetPlayerField().Gate(nTestDynamicFieldID, vec3(1000, 1000, 1000), vec3(0,1,0), false);
		gmgr.pFieldMgr->Update(0.1f);
		m_pPlayerRequester->GetPlayerField().GetFieldEntry().OnEnterFieldComplete();
		m_pPlayerTarget->GetPlayerField().GetFieldEntry().OnEnterFieldComplete();

		// ���� ���� �����ʵ忡 �����߳�?
		CHECK_EQUAL(pFieldGroup1->GetUID(), pFieldGroup2->GetUID());
		CHECK_EQUAL(m_pPlayerRequester->GetFieldUID(), m_pPlayerTarget->GetFieldUID());
		CHECK_EQUAL(m_pPlayerRequester->GetFieldID(), nTestDynamicFieldID);
		CHECK_EQUAL(m_pPlayerTarget->GetFieldID(), nTestDynamicFieldID);

		// �����ʵ忡�� ������ �����Ѱ�?
		CHECK_EQUAL(m_pPlayerRequester->HasDuel(), false);
		CHECK_EQUAL(m_pPlayerTarget->HasDuel(), false);
		DuelQuestion();
		CHECK_EQUAL(m_pPlayerRequester->HasDuel(), false);
		CHECK_EQUAL(m_pPlayerTarget->HasDuel(), false);
	}

	TEST_FIXTURE(FDuel, IsNowCombatForPVP)
	{
		CHECK_EQUAL(false, m_pPlayerRequester->IsNowCombat());
		CHECK_EQUAL(false, m_pPlayerTarget->IsNowCombat());
		// ����
		GTalentInfo* pTalentInfo = MakeSimpleTalent(10);
		pTalentInfo->m_EffectInfo.m_nRelation = CSEffectInfo::RELATION_ALL;
		test.talent.UseTalent(m_pPlayerRequester, pTalentInfo, m_pPlayerTarget->GetUID());
		CHECK_EQUAL(true, m_pPlayerRequester->IsNowCombat());
		CHECK_EQUAL(true, m_pPlayerTarget->IsNowCombat());
		// ���� �ð��� ���� PVP���� Ǯ��
		m_pField->Update(GConst::PVP_DURATIONTIME);
		CHECK_EQUAL(false, m_pPlayerRequester->IsNowCombat());
		CHECK_EQUAL(false, m_pPlayerTarget->IsNowCombat());
	}

	TEST_FIXTURE(FDuel, Cleanup)
	{
		DuelStart();
		int nBuffID = 123;
		int nDebuffID = 321;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_nType = BUFT_BUFF;
		GBuffInfo* pDebuff=test.buff.NewBuffInfo(nDebuffID);
		pDebuff->m_nType = BUFT_DEBUFF;

		test.buff.GainBuffSelf(m_pPlayerRequester, pBuff);
		test.buff.GainBuffSelf(m_pPlayerTarget, pBuff);
		test.buff.GainBuffSelf(m_pPlayerRequester, pDebuff);
		test.buff.GainBuffSelf(m_pPlayerTarget, pDebuff);

		// ������ ����� ������� ���ŵ�
		CHECK_EQUAL(true, m_pPlayerRequester->HasBuff(nBuffID));
		CHECK_EQUAL(true, m_pPlayerTarget->HasBuff(nBuffID));
		CHECK_EQUAL(true, m_pPlayerRequester->HasBuff(nDebuffID));
		CHECK_EQUAL(true, m_pPlayerTarget->HasBuff(nDebuffID));
		DuelFinish(m_pPlayerRequester);
		CHECK_EQUAL(true, m_pPlayerRequester->HasBuff(nBuffID));
		CHECK_EQUAL(true, m_pPlayerTarget->HasBuff(nBuffID));
		CHECK_EQUAL(false, m_pPlayerRequester->HasBuff(nDebuffID));
		CHECK_EQUAL(false, m_pPlayerTarget->HasBuff(nDebuffID));
	}

	TEST_FIXTURE(FDuel, DontHealingByObserver_AlliedCase)
	{
		// �÷��̾ ���� �ŷ�Ʈ
		GTalentInfo* pTalentInfo = new GTalentInfo();
		m_Helper.SetMagicTargetTalentInfo(pTalentInfo, 1);
		pTalentInfo->m_EffectInfo.m_nRelation = CSEffectInfo::RELATION_ALLIED;
		pTalentInfo->m_nMaxHeal = pTalentInfo->m_nMinHeal = 10;		// hp�� 10 �÷���
		gmgr.pTalentInfoMgr->Insert(pTalentInfo);

		GEntityPlayer* pObserver = GUTHelper::NewEntityPlayer(m_pField, vCommonPos);

		// �� ����
		m_pPlayerTarget->SetHP(100);
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 100);
		test.talent.UseTalent(pObserver, pTalentInfo, m_pPlayerTarget->GetUID());
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 110);

		// ���� ����
		DuelStart();

		// �� �Ұ���
		m_pPlayerTarget->SetHP(100);
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 100);
		test.talent.UseTalent(pObserver, pTalentInfo, m_pPlayerTarget->GetUID());
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 100);

		// ���� ����
		DuelFinish(m_pPlayerRequester);

		// ������ ������ �ٽ� �ǰݵ� �� ����
		m_pPlayerTarget->SetHP(100);
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 100);
		test.talent.UseTalent(pObserver, pTalentInfo, m_pPlayerTarget->GetUID());
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 110);
	}

	TEST_FIXTURE(FDuel, DontHealingByObserver_PartyCase)
	{
		// �÷��̾ ���� �ŷ�Ʈ
		GTalentInfo* pTalentInfo = new GTalentInfo();
		m_Helper.SetMagicTargetTalentInfo(pTalentInfo, 1);
		pTalentInfo->m_EffectInfo.m_nRelation = CSEffectInfo::RELATION_PARTY;
		pTalentInfo->m_nMaxHeal = pTalentInfo->m_nMinHeal = 10;		// hp�� 10 �÷���
		gmgr.pTalentInfoMgr->Insert(pTalentInfo);

		GEntityPlayer* pObserver = GUTHelper::NewEntityPlayer(m_pField, vCommonPos);
		MakeParty(pObserver, m_pPlayerTarget);

		// �� ����
		m_pPlayerTarget->SetHP(100);
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 100);
		test.talent.UseTalent(pObserver, pTalentInfo, m_pPlayerTarget->GetUID());
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 110);

		// ���� ����
		DuelStart();

		// �� �Ұ���
		m_pPlayerTarget->SetHP(100);
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 100);
		test.talent.UseTalent(pObserver, pTalentInfo, m_pPlayerTarget->GetUID());
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 100);

		// ���� ����
		DuelFinish(m_pPlayerRequester);

		// ������ ������ �ٽ� �ǰݵ� �� ����
		m_pPlayerTarget->SetHP(100);
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 100);
		test.talent.UseTalent(pObserver, pTalentInfo, m_pPlayerTarget->GetUID());
		CHECK_EQUAL(m_pPlayerTarget->GetHP(), 110);
	}

	TEST_FIXTURE(FDuel, DuelLimitObserveCount)
	{
		// ���� ����
		DuelStart();

		GDuel* pDuel = m_pPlayerRequester->GetDuel();
		TVALID(pDuel);
		
		GEntityPlayer* pObserver = GUTHelper::NewEntityPlayer(m_pField, vCommonPos);
		CHECK_EQUAL(true, pObserver->GetEnemyInfoSender().AttachObserveDuel(pDuel->GetUID()));
		CHECK_EQUAL(true, pObserver->GetEnemyInfoSender().DetachObserveDuel());

		for (int i=0; i<GConst::DUEL_LIMIT_OBSERVE_COUNT; ++i)
			pDuel->IncreaseObserveCount();

		CHECK_EQUAL(false, pObserver->GetEnemyInfoSender().AttachObserveDuel(pDuel->GetUID()));
	}

	TEST_FIXTURE(FDuel, LifeCycle)
	{
		CHECK(NULL == m_pPlayerRequester->GetDuel());
		CHECK(NULL == m_pPlayerTarget->GetDuel());
		DuelStart();
		CHECK(NULL != m_pPlayerRequester->GetDuel());
		CHECK(NULL != m_pPlayerTarget->GetDuel());
		DuelFinish(m_pPlayerRequester);
		CHECK(NULL == m_pPlayerRequester->GetDuel());
		CHECK(NULL == m_pPlayerTarget->GetDuel());
	}

	TEST_FIXTURE(FDuel, ResetWhenChangeField)
	{
		int nTestSharedFieldID = 100;
		GField* pOhterField = MakeSharedField(nTestSharedFieldID);
		DuelStart();
		m_pPlayerRequester->GetPlayerField().Gate(nTestSharedFieldID, vec3(1000, 1000, 1000), vec3(0,1,0), false);
		m_pField->Update(0.0f); // ���� ���� ƽ
		// �ʵ� �̵�
		gmgr.pFieldMgr->Update(0.0f); 
		m_pPlayerRequester->GetPlayerField().GetFieldEntry().OnEnterFieldComplete();
		CHECK(NULL == m_pPlayerRequester->GetDuel());
		CHECK(NULL == m_pPlayerTarget->GetDuel());
	}
}
