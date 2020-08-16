#include "stdafx.h"
#include "GModuleAI.h"
#include "MockJobTask.h"
#include "GUTHelper.h"
#include "MockField.h"
#include "GFieldMgr.h"
#include "GEntityNPC.h"
#include "GUTHelper_Field.h"

SUITE(Job)
{
	struct FJob
	{
		FJob()
		{
			m_pField = GUTHelper_Field::DefaultMockField();
			m_pNPC = m_Helper.NewEntityNPC(m_pField);

			m_pJob = new MockJob(m_pNPC->GetModuleAI());
		}
		virtual ~FJob()
		{
			SAFE_DELETE(m_pJob);
			m_pField->Destroy();
		}

		GUTHelper		m_Helper;

		MockField*		m_pField;
		GEntityNPC*		m_pNPC;
		MockJob*		m_pJob;
		
	};

	namespace Run
	{
		// �ð��� �ʰ� �� ���
		TEST_FIXTURE(FJob, TimeOut)
		{
			MockTask* pTask = new MockTask(m_pNPC);
			m_pJob->SetRunningTask(pTask);

			m_pJob->SetDurationTime(1.0f);

			CHECK_EQUAL(JR_TIMEOUT, m_pJob->Run(1.0f));
		}

		// ���� task�� ���� ����� �Ϸ��� ���
		TEST_FIXTURE(FJob, TaskIsCompleted)
		{
			MockTask* pTask = new MockTask(m_pNPC);
			m_pJob->SetRunningTask(pTask);

			CHECK_EQUAL(JR_COMPLETED, m_pJob->Run(1.0f));
			CHECK_EQUAL(TES_TASK_COMPLETE, m_pJob->GetEndedStatus());
			CHECK(NULL == m_pJob->GetTask());
		}

		// ���� task�� ���� ����� ����� ���
		TEST_FIXTURE(FJob, TaskIsCanceled)
		{
			MockRun_CanceledTask* pTask = new MockRun_CanceledTask(m_pNPC);
			m_pJob->SetRunningTask(pTask);

			CHECK_EQUAL(JR_CANCELED, m_pJob->Run(1.0f));
			CHECK_EQUAL(TES_TASK_CANCEL, m_pJob->GetEndedStatus());
			CHECK(NULL == m_pJob->GetTask());
		}

		// ���� task�� ���� ����� ��� ������ ���
		TEST_FIXTURE(FJob, TaskIsRunning)
		{
			MockRun_RunningTask* pTask = new MockRun_RunningTask(m_pNPC);
			m_pJob->SetRunningTask(pTask);

			CHECK_EQUAL(JR_RUNNING, m_pJob->Run(1.0f));
			CHECK_EQUAL(TES_RUN, m_pJob->GetEndedStatus());
			CHECK_EQUAL(pTask, m_pJob->GetTask());
		}
	}

	// �Ϸ��� ���
	TEST_FIXTURE(FJob, Complete)
	{
		m_pJob->Complete();
		CHECK_EQUAL(TES_COMPLETE, m_pJob->GetEndedStatus());
		CHECK(NULL == m_pJob->GetTask());
	}

	// ����� ���
	TEST_FIXTURE(FJob, Cancel)
	{
		m_pJob->Cancel();
		CHECK_EQUAL(TES_CANCEL, m_pJob->GetEndedStatus());
		CHECK(NULL == m_pJob->GetTask());
	}

	// �ð� �ʰ��� ���
	TEST_FIXTURE(FJob, TimeOut)
	{
		m_pJob->TimeOut();
		CHECK_EQUAL(TES_TIMEOUT, m_pJob->GetEndedStatus());
		CHECK(NULL == m_pJob->GetTask());
	}
}