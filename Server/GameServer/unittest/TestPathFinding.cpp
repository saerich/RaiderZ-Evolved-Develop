#include "stdafx.h"
#include "SUnitTest.h"
#include "GPathFinder.h"
#include "MPath.h"
#include "MAgent.h"
#include "GSharedFieldMaster.h"
#include "GUTHelper.h"
#include "GNPCInfoMgr.h"
#include "GJob_MoveToEntity.h"
#include "GJobRunner.h"
#include "GModuleAI.h"
#include "GTask_Move.h"
#include "GNPCPathFinder.h"
#include "GCommandTable.h"
#include "GFieldMgr.h"
#include "MockLink.h"
#include "GEntityNPC.h"
#include "GFieldInfoMgr.h"
#include "GJobMgr.h"
#include "GBehaviorStateMachine.h"
#include "GHateTable.h"
#include "FBaseMockLink.h"
#include "GServer.h"
#include "GUTHelper_Field.h"
#include "GModuleMovable.h"

/*

######## PathFinding �׽�Ʈ �ڵ忡�� �׽�Ʈ�ؾ� �� ���� ���̽� ########

  + NPC 1������ ���� ��ġ���� ���� ��ġ���� �� ã�ư���? : TestSimplePathFinding
  + ��ã�Ⱑ ���� �� GTask_Move�� �� ������? : TestSimplePathFinding
  + Job_MoveToEntity�� ����� �����ϳ�? : TestSimplePathFinding
  + ���� �ɷ��� �� �������� �Ѿ���� �ʴ°�? : TestSimplePathFinding
  + ���� Behavior�� �Ǿ��� �� Agent ����� �ǰ� ������ ���� �� Agent�� ����� Release�Ǿ���? : TestNPCPathAgent
  + NPC �׷���(CollisionContext)�� ����� �ǳ�? : TestNPCPathCollisionContext
  + ��ã�⸦ �����ϸ� Move Ŀ�ǵ尡 ����� ���󰡰� ������ Stop Ŀ�ǵ尡 ����� ���󰡳�? : TestNPCMovingCommand

  - ���������� ��ġ���� �������� ��ġ�� ��ã�⸦ ���ϳ�?
  - ���������� ��ġ���� ���������� ��ġ�� ��ã�⸦ ���ϳ�?

*/


SUITE(PathFinding)
{
	TEST3(TestFindPathFailed)
	{
		int nZoneID = 8003;
		vec3 vStartPos = vec3(408.8f, 560.3f, 0.0f);
		vec3 vGoalPos = vec3(1.0f, 1.0f, 1.0f);
		vector<vec3> vecStubPathList;

		nZoneID = 8003;
		vStartPos = vec3(1160.3f, 1126.0, 0.0f);
		vGoalPos = vec3(1381.8f, 880.2f, 0.0f);
	}


	TEST3(TestPathFindingNewPlaceAgent)
	{
		int nZoneID;

		for(GFieldMgr::SHARED_FIELD_MASTER_MAP::iterator itor = gmgr.pFieldMgr->GetSharedFieldMap().begin(); 
			itor != gmgr.pFieldMgr->GetSharedFieldMap().end(); ++itor)
		{
			GSharedFieldMaster* pSharedField = (*itor).second;
			GField* pField = pSharedField->GetChannel(0);

			if (pField && pField->GetInfo())
			{
				MAgent path_agent;
				nZoneID = pField->GetInfo()->m_nZoneID;
				vec3 vStartPos = vec3(1000, 1000, 100);

				gsys.pPathFinder->NewPlaceAgent(path_agent, vStartPos, nZoneID, pField->GetPathMesh());

				vStartPos = vec3(426420.297f, 42030.223f, 7375.4189f);
				path_agent.MoveTo(vStartPos);

				path_agent.Release();
			}
		}
	}

	TEST3(TestFindPath)
	{
		vector<vec3>	vTarPositions;			///< ���� �������� �����ϱ� ���� ���İ��� �߰�������

		int nZoneID = 8003;

		for(GFieldMgr::SHARED_FIELD_MASTER_MAP::iterator itor = gmgr.pFieldMgr->GetSharedFieldMap().begin(); 
			itor != gmgr.pFieldMgr->GetSharedFieldMap().end(); ++itor)
		{
			GSharedFieldMaster* pSharedField = (*itor).second;
			GField* pField = pSharedField->GetChannel(0);

			if (pField && pField->GetInfo())
			{
				MPath outPath;
				MAgent path_agent;
				vec3 vStartPos = vec3(RandomNumber(100.f, 10000.f), RandomNumber(100.f, 10000.f), 0);
				vec3 vDestPos = vec3(RandomNumber(100.f, 10000.f), RandomNumber(100.f, 10000.f), 0);
				nZoneID = pField->GetInfo()->m_nZoneID;

				if (gsys.pPathFinder->NewPlaceAgent(path_agent, vStartPos + vec3(0,0,100), nZoneID, pField->GetPathMesh()))
				{
					path_agent.MoveTo(vStartPos);

					GPathFinder* pathFinder = gsys.pPathFinder;

					pathFinder->FindPath(outPath, pField->GetPathMesh(), path_agent, vDestPos);

					path_agent.Release();
					outPath.Release();
				}
			}
		}
	}

	TEST3(TestFindPath_NPCAgent)
	{
		const int FIELD_ID = 121;

		GField* pField = gmgr.pFieldMgr->GetSharedField(FIELD_ID, 0);

		if (pField && pField->GetInfo())
		{
			vec3 vStartPos = vec3(11756.128f, 10961.024f, 2830.6135f);
			vec3 vDestPos = vec3(11591.337f, 11226.968f, 2831.4187f);

			int nZoneID = pField->GetInfo()->m_nZoneID;

			GEntityNPC* pNPC = pField->SpawnNPC(11, NULL, vStartPos, vec3(0,1,0));
			CHECK(pNPC != NULL);
			if (pNPC == NULL) return;

			vector<vec3> vecPath;
			GModuleAI* ai = pNPC->GetModuleAI();
			GNPCPathFinder* npc_path_finder = pNPC->GetPathFinder();
			npc_path_finder->OnEnterToCombat();

			bool bSuccess = npc_path_finder->FindPath(vStartPos, vDestPos, vecPath);
			CHECK(bSuccess == true);

			for (size_t i = 0; i < vecPath.size(); i++)
			{
				CHECK(vecPath[i].z > 2000.0f);
			}
		}
	}

	struct PathFindingFixture : public FBaseMockLink
	{
		PathFindingFixture() : m_pField(NULL)
		{
			GFieldInfo* pFieldInfo = gmgr.pFieldInfoMgr->Find(1);
			if (pFieldInfo)
			{
				m_pField = GUTHelper_Field::NewSharedField(pFieldInfo, 1);
			}


			m_vPlayerPos = vec3(1000,1000,0.0f);
			m_pPlayer = GUTHelper::NewEntityPlayer(m_pField, m_vPlayerPos, gsys.pServer->NewUID());
			
		}
		~PathFindingFixture()
		{
			gmgr.pFieldMgr->Clear();
		}

		GField* m_pField;
		vec3	m_vPlayerPos;
		GEntityPlayer* m_pPlayer;
		GUTHelper	m_Helper;
	};

	////////////////////////////////////////////////
	TEST_FIXTURE2(PathFindingFixture, TestSimplePathFinding)
	{
		CHECK(m_pField != NULL);
		if (m_pField == NULL) return;

		vec3 vNPCFirstPos = vec3(2000.0f, 1000.0f, 0.1f);
		vec3 vNPCFirstDir = vec3(-1.0f, 0.0f, 0.0f);

		GEntityNPC* pNPC = m_pField->SpawnNPC(1, NULL, vNPCFirstPos, vNPCFirstDir);
		CHECK(pNPC != NULL);
		if (pNPC == NULL) return;

		float fNPCSpeed = pNPC->GetNPCInfo()->fSpeed;
		CHECK_CLOSE(fNPCSpeed, 100.0f, 0.01f);		// xml�� NPC ���ǵ尡 100

		GModuleAI* ai = pNPC->GetModuleAI();
		GModuleMovable* movable = pNPC->GetModuleMovable();

		const float fLeastGap = 100.0f;
		GJobRunner job_runner;
		job_runner.MoveToEntity(ai, m_pPlayer, fLeastGap, false);
		ai->Update(0.0f);
		movable->Update(0.0f);


		// Task_Move Ȯ��
		GJobMgr* pJobMgr = ai->GetJobMgr();
		CHECK(pJobMgr->GetCurrTask() != NULL);

		if (pJobMgr->GetCurrTask())
		{
			CHECK_EQUAL(pJobMgr->GetCurrTask()->GetID(), GTASK_MOVE);

			if (pJobMgr->GetCurrTask()->GetID() == GTASK_MOVE)
			{
				CHECK_EQUAL(pNPC->GetModuleMovable()->GetMoveStrategy(), MS_DEFINITE);	// ��ã�⸦ ������ �ϰ� �ִ������� Ȯ��
			}
		}

		movable->Update(1.0f);
		CHECK_CLOSE(pNPC->GetPos().DistanceTo(m_vPlayerPos), 900.0f, 0.01f);

		// 1�ʿ� fNPCSpeed��ŭ �����Ѵ�.
		movable->Update(1.0f);
		CHECK_CLOSE(pNPC->GetPos().DistanceTo(m_vPlayerPos), 900.0f - fNPCSpeed, 0.01f);

		for (int i = 0; i < 20; i++)
		{
			movable->Update(1.0f);
			float fExpectedDistance = (1000.0f - fNPCSpeed) - ((fNPCSpeed) * (i+2));
			fExpectedDistance = max(fExpectedDistance, fLeastGap);	// �ƹ��� ������ �����ص� fLeastGap������ �����Ѵ�.

			m_vPlayerPos.z = 0.0f;
			vec3 vNPCPos = pNPC->GetPos();
			vNPCPos.z = 0.0f;
			CHECK_CLOSE(vNPCPos.DistanceTo(m_vPlayerPos), fExpectedDistance, 0.01f);
		}

		// ����
		CHECK_CLOSE(pNPC->GetPos().DistanceTo(m_vPlayerPos), fLeastGap, 0.01f);

		// �̹� �÷��̾� ��ġ���� �� �ͼ� Job�� ����Ǿ���
		ai->Update(0.0f);
		CHECK(pJobMgr->GetCurrTask() == NULL ||
			  pJobMgr->GetCurrTask()->GetID() != GTASK_MOVE);
	}

	void MakeNPCToCombatBehavior(GEntityNPC* pNPC, MUID uidAttacker)
	{
		EVENT_ATTACKED_INFO info;
		info.uidAttacker=	uidAttacker;
		info.nDamage	=	1;

		GMessage msg(GMSG_AI_EVT_ATTACKED, &info);
		pNPC->Message(msg);
	}

	void MakeNPCToPeaceBehavior(GEntityNPC* pNPC)
	{
		GMessage msg(GMSG_AI_EVT_ENDCOMBAT);
		pNPC->Message(msg);
	}

	TEST_FIXTURE2(PathFindingFixture, TestNPCPathAgent)
	{
		vec3 vNPCFirstPos = vec3(2000.0f, 1000.0f, 0.1f);
		vec3 vNPCFirstDir = vec3(-1.0f, 0.0f, 0.0f);

		GEntityNPC* pNPC = m_pField->SpawnNPC(1, NULL, vNPCFirstPos, vNPCFirstDir);
		CHECK(pNPC != NULL);
		if (pNPC == NULL) return;

		GModuleAI* ai = pNPC->GetModuleAI();
		GNPCPathFinder* npc_path_finder = pNPC->GetPathFinder();
		MAgent& agent = npc_path_finder->GetAgent();

		// �������� �ƴϸ� Agent�� ��������� �ʴ´�. (��ã�� �õ��� �� ����)
		GBehaviorStateMachine* pFSM = ai->GetBehaviorFSM();
		CHECK_EQUAL(pFSM->IsNowCombat(), false);
		CHECK_EQUAL(agent.IsValid(), false); 

		MakeNPCToCombatBehavior(pNPC, m_pPlayer->GetUID());

		m_pField->Update(0.1f);

		// ������ ���۵Ǹ� agent�� ���������.
		CHECK(pNPC->IsNowCombat() == true);
		CHECK(pFSM->IsNowCombat() == true);
		CHECK_EQUAL(agent.IsValid(), true);


		MakeNPCToPeaceBehavior(pNPC);

		// ������ ������ agent�� �����ȴ�.
		CHECK_EQUAL(pNPC->IsNowCombat(), true);
		CHECK_EQUAL(pNPC->IsNowReturn(), true);
		CHECK_EQUAL(agent.IsValid(), false);

	}

	TEST_FIXTURE2(PathFindingFixture, TestNPCPathCollisionContext)
	{
		vec3 vNPCFirstPos = vec3(2000.0f, 1000.0f, 0.1f);
		vec3 vNPCFirstDir = vec3(-1.0f, 0.0f, 0.0f);

		const int NPC_COUNT = 3;

		GEntityNPC* pNPC[NPC_COUNT];
		GModuleAI* ai[NPC_COUNT];
		GNPCPathFinder* npc_path_finder[NPC_COUNT];

		for (int i = 0; i < NPC_COUNT; i++)
		{
			pNPC[i] = m_pField->SpawnNPC(1, NULL, vNPCFirstPos, vNPCFirstDir);
			CHECK(pNPC[i] != NULL);
			if (pNPC[i] == NULL) return;

			ai[i] = pNPC[i]->GetModuleAI();
			npc_path_finder[i] = pNPC[i]->GetPathFinder();
		}

		CHECK_EQUAL(m_pField->GetPathContextGroupCount(), 0);

		// ù��° NPC ���� ����
		MakeNPCToCombatBehavior(pNPC[0], m_pPlayer->GetUID());

		m_pField->Update(0.1f);
		
		CHECK_EQUAL(ai[0]->GetOwnerNPC()->GetTarget(), m_pPlayer->GetUID());


		// ������ ���۵Ǹ� agent�� ���������.
		CHECK_EQUAL(pNPC[0]->IsNowCombat(), true);
		CHECK_EQUAL(npc_path_finder[0]->GetAgent().IsValid(), true);

		// GroupAgentCount�� �׷��� �ϳ�
		CHECK(npc_path_finder[0]->GetAgent().GetContext() != NULL);
		CHECK_EQUAL(npc_path_finder[0]->GetGroupAgentCount(), 1);

		CHECK_EQUAL(m_pField->GetPathContextGroupCount(), 1);


		// �ι�° NPC ���� ����
		MakeNPCToCombatBehavior(pNPC[1], m_pPlayer->GetUID());

		m_pField->Update(0.1f);

		// GroupAgentCount�� �׷��� ���� ��
		CHECK_EQUAL(npc_path_finder[0]->GetGroupAgentCount(), 2);
		CHECK_EQUAL(m_pField->GetPathContextGroupCount(), 1);


		// �ι�° NPC ���� ��
		MakeNPCToPeaceBehavior(pNPC[1]);
		m_pField->Update(0.1f);

		// GroupAgentCount�� �׷��� ���� �ٽ� �ϳ�
		CHECK_EQUAL(npc_path_finder[0]->GetGroupAgentCount(), 1);

		// ù��° NPC ���� ��
		MakeNPCToPeaceBehavior(pNPC[0]);

		for (int i =0 ; i < 360; i++) m_pField->Update(1.0f);

		// GroupAgentCount�� �׷��� ���� 0
		CHECK_EQUAL(m_pField->GetPathContextGroupCount(), 0);
	}

	TD_NPC_MOVE GetMoveDataFromCmd(MockLink* pLink, int nCommandIndex)
	{
		TD_NPC_MOVE ret;
		memset(&ret, 0, sizeof(TD_NPC_MOVE));

		if (pLink->GetCommand(nCommandIndex).GetID() != MC_NPC_MOVE) return ret;
		MCommandParameter* pParam = pLink->GetCommand(nCommandIndex).GetParameter(0);
		if(pParam->GetType()!=MPT_SINGLE_BLOB) return ret;

		TD_NPC_MOVE* pNM = (TD_NPC_MOVE*)pParam->GetPointer();
		ret = *pNM;

		return ret;
	}

	TEST_FIXTURE2(PathFindingFixture, TestNPCMovingCommand)
	{
		MockLink* pLink = NewLink(m_pPlayer);
		pLink->AddIgnoreID(MC_FIELD_OUT_NPC);
		pLink->AddIgnoreID(MC_FIELD_IN_NPC);

		vec3 vNPCFirstPos = vec3(2000.0f, 1000.0f, 0.0f);
		vec3 vNPCFirstDir = vec3(-1.0f, 0.0f, 0.0f);

		GEntityNPC* pNPC = m_pField->SpawnNPC(1, NULL, vNPCFirstPos, vNPCFirstDir);
		CHECK(pNPC != NULL);
		if (pNPC == NULL) return;

		GModuleAI* ai = pNPC->GetModuleAI();
		GModuleMovable* movable = pNPC->GetModuleMovable();

		pLink->ResetCommands();

		const float fLeastGap = 200.0f;
		GJobRunner job_runner;
		job_runner.MoveToEntity(ai, m_pPlayer, fLeastGap, false);
		ai->Update(0.0f);
		movable->Update(1.0f);

		vec3 _tar_dir = m_pPlayer->GetPos() - pNPC->GetPos();
		_tar_dir.z = 0.0f;
		_tar_dir.Normalize();
		vec3 _tar_pos = m_pPlayer->GetPos() - _tar_dir * fLeastGap;


		CHECK_EQUAL(pLink->GetCommand(0).GetID(), MC_NPC_MOVE);
		TD_NPC_MOVE move_data = GetMoveDataFromCmd(pLink, 0);

		CHECK(move_data.nUIID == pNPC->GetUIID());
		CHECK(move_data.nSpeed == (uint16)(pNPC->GetNPCInfo()->fSpeed));
		CHECK_CLOSE(_tar_pos.x, move_data.vTarPos.x, 0.01f);
		CHECK_CLOSE(_tar_pos.y, move_data.vTarPos.y, 0.01f);
		CHECK_CLOSE(_tar_pos.z, 0.0f, 0.01f); // ��ŷ ����

		for ( int i = 0;  i < 30;  i++)
			movable->Update(1.0f);

		CHECK_CLOSE(_tar_pos.x, pNPC->GetPos().x, 0.01f);
		CHECK_CLOSE(_tar_pos.y, pNPC->GetPos().y, 0.01f);
		CHECK_CLOSE(_tar_pos.z, pNPC->GetPos().z, 2.0f);



//		[��] : MC_NPC_MOVE_STOP Ŀ�ǵ��� ���, ��Ŷ �뿪���� ���̱� ���ؼ� �� �ʿ��� ��찡 �ƴϸ� ������ �ʰԲ�
//		�����Ͽ����ϴ�.  - ��ȯ

		size_t nCmdCount = pLink->GetCommandCount();
		for ( size_t i = 0;  i < nCmdCount;  i++)
		{
			if (pLink->GetCommand(i).GetID() == MC_NPC_MOVE_STOP)
			{
				UIID nParamUIID = pLink->GetParam<unsigned short>(nCmdCount-1, 0);
				CHECK_EQUAL(nParamUIID, pNPC->GetUIID());

				vec3 vParamPos = pLink->GetParam<vec3>(nCmdCount-1, 1);

				CHECK_CLOSE(vParamPos.x, 1200.0f, 0.01f);
				CHECK_CLOSE(vParamPos.y, 1000.0f, 0.01f);
				CHECK_CLOSE(vParamPos.z, 0.0f, 0.01f);
			}
		}

	}
}
