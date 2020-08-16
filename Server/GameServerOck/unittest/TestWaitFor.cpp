#include "stdafx.h"
#include "GTestForward.h"
#include "GNPCWaitFor.h"
#include "GJobRunner.h"
#include "GModuleAI.h"
#include "GJobMgr.h"

SUITE(WaitFor)
{
	struct FWaitFor
	{
		FWaitFor()
		{
			pField = GUTHelper_Field::DefaultMockField();
			pNPC1 = GUTHelper_NPC::SpawnNPC(pField);
			pNPC2 = GUTHelper_NPC::SpawnNPC(pField);
			pNPC3 = GUTHelper_NPC::SpawnNPC(pField);
			pNPCWaitFor = &pNPC1->GetNPCWaitFor();
		}
		virtual ~FWaitFor()
		{
		}

		MockField* pField;
		GEntityNPC* pNPC1;
		GEntityNPC* pNPC2;
		GEntityNPC* pNPC3;
		GNPCWaitFor* pNPCWaitFor;

		DECLWRAPPER_Field;
	};

	TEST_FIXTURE(FWaitFor, CheckWaiting)
	{
		CHECK_EQUAL(false, pNPCWaitFor->IsWaiting());
		pNPCWaitFor->AddWaiting(pNPC2->GetUID());
		CHECK_EQUAL(true, pNPCWaitFor->IsWaiting());
		pNPCWaitFor->OnNextAction(pNPC2->GetUID());
		CHECK_EQUAL(false, pNPCWaitFor->IsWaiting());
	}

	TEST_FIXTURE(FWaitFor, WaitingSelf)
	{
		CHECK_EQUAL(false, pNPCWaitFor->IsWaiting());
		pNPCWaitFor->AddWaiting(pNPC1->GetUID());
		CHECK_EQUAL(false, pNPCWaitFor->IsWaiting());
	}

	TEST_FIXTURE(FWaitFor, ActualWaiting)
	{
		CHECK_EQUAL(false, pNPC1->GetNPCWaitFor().IsWaiting());
		CHECK_EQUAL(false, pNPC3->GetNPCWaitFor().IsWaiting());
		// 1��NPC�� 2��NPC ���
		pNPC1->GetNPCWaitFor().AddWaiting(pNPC2->GetUID());
		pNPC2->GetNPCWaitFor().AddWaiter(pNPC1->GetUID());
		// 3��NPC�� 2��NPC ���
		pNPC3->GetNPCWaitFor().AddWaiting(pNPC2->GetUID());
		pNPC2->GetNPCWaitFor().AddWaiter(pNPC3->GetUID());
		CHECK_EQUAL(true, pNPC1->GetNPCWaitFor().IsWaiting());
		CHECK_EQUAL(true, pNPC3->GetNPCWaitFor().IsWaiting());
		pNPC2->GetNPCWaitFor().RunNextAction();
		CHECK_EQUAL(false, pNPC1->GetNPCWaitFor().IsWaiting());
		CHECK_EQUAL(false, pNPC3->GetNPCWaitFor().IsWaiting());
	}

	TEST_FIXTURE(FWaitFor, InterWaiting)
	{
		CHECK_EQUAL(false, pNPC1->GetNPCWaitFor().IsWaiting());
		CHECK_EQUAL(false, pNPC2->GetNPCWaitFor().IsWaiting());
		// 1��NPC�� 2��NPC ���
		pNPC1->GetNPCWaitFor().AddWaiting(pNPC2->GetUID());
		pNPC2->GetNPCWaitFor().AddWaiter(pNPC1->GetUID());
		CHECK_EQUAL(true, pNPC1->GetNPCWaitFor().IsWaiting());
		CHECK_EQUAL(false, pNPC2->GetNPCWaitFor().IsWaiting());
		// 2��NPC�� 1��NPC ���
		pNPC2->GetNPCWaitFor().AddWaiting(pNPC1->GetUID());
		pNPC1->GetNPCWaitFor().AddWaiter(pNPC2->GetUID());
		CHECK_EQUAL(true, pNPC1->GetNPCWaitFor().IsWaiting());
		CHECK_EQUAL(true, pNPC2->GetNPCWaitFor().IsWaiting());
		// 1��NPC �������
		pNPC1->GetNPCWaitFor().RunNextAction();
		CHECK_EQUAL(true, pNPC1->GetNPCWaitFor().IsWaiting());
		CHECK_EQUAL(false, pNPC2->GetNPCWaitFor().IsWaiting());
		// 2��NPC �������
		pNPC2->GetNPCWaitFor().RunNextAction();
		CHECK_EQUAL(false, pNPC1->GetNPCWaitFor().IsWaiting());
		CHECK_EQUAL(false, pNPC2->GetNPCWaitFor().IsWaiting());
	}

	TEST_FIXTURE(FWaitFor, UsingWaitforJob)
	{
		GJobRunner job_runner;
		// 1��NPC�� 2��NPC ���
		CHECK_EQUAL(GJOB_NA, pNPC1->GetModuleAI()->GetJobMgr()->GetCurrJobID());
		CHECK_EQUAL(false, pNPC1->GetNPCWaitFor().IsWaiting());
		job_runner.WaitFor(pNPC1->GetModuleAI(), pNPC2);
		pNPC1->GetModuleAI()->Update(0.0f);
		CHECK_EQUAL(GJOB_WAITFOR, pNPC1->GetModuleAI()->GetJobMgr()->GetCurrJobID());
		CHECK_EQUAL(true, pNPC1->GetNPCWaitFor().IsWaiting());
		// �ƹ��� ��ٷ��� ��Ǯ��
		pNPC1->GetModuleAI()->Update(100.0f);
		pNPC1->GetModuleAI()->Update(100.0f);	
		CHECK_EQUAL(GJOB_WAITFOR, pNPC1->GetModuleAI()->GetJobMgr()->GetCurrJobID());
		CHECK_EQUAL(true, pNPC1->GetNPCWaitFor().IsWaiting());
		// 2��NPC �������
		job_runner.NextAction(pNPC2->GetModuleAI());
		pNPC2->GetModuleAI()->Update(0.0f);
		pNPC1->GetModuleAI()->Update(1.0f);	// ���üũ�� �ð�
		CHECK_EQUAL(GJOB_NA, pNPC1->GetModuleAI()->GetJobMgr()->GetCurrJobID());
		CHECK_EQUAL(false, pNPC1->GetNPCWaitFor().IsWaiting());
	}
}