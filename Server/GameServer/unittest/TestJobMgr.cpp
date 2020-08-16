#include "stdafx.h"
#include "GModuleAI.h"
#include "MockJobMgr.h"
#include "MockJobTask.h"
#include "GTestWrapper.h"
#include "MockField.h"
#include "GUTHelper.h"
#include "GJobT_Script.h"
#include "GFieldMgr.h"
	
SUITE(JobMgr)
{
	struct FJobMgr
	{
		FJobMgr() {}
		virtual ~FJobMgr() {}

		GJobMgr		m_JobMgr;
		GModuleAI	m_ModuleAI;
	};

	namespace Push
	{
		// �Էµ� job�� NULL�� ���
		TEST_FIXTURE(FJobMgr, PushNull)
		{			
			m_JobMgr.Push(NULL);
			m_JobMgr.Run(0.0f);

			CHECK_EQUAL(true, m_JobMgr.IsEmpty());
		}

		// �̹� job�� ������ �ִ� ���
		TEST_FIXTURE(FJobMgr, AlreadyJobExist)
		{
			MockJob* pExistJob = new MockJob(&m_ModuleAI);
			MockJob* pNewJob = new MockJob(&m_ModuleAI);
			m_JobMgr.Push(pExistJob);
			m_JobMgr.Push(pNewJob);
			m_JobMgr.Run(0.0f);

			CHECK_EQUAL(false, m_JobMgr.IsEmpty());
			CHECK_EQUAL(1, m_JobMgr.GetCount());
			CHECK_EQUAL(m_JobMgr.GetCurrJob(), pExistJob);
			CHECK_EQUAL(TES_RUN, pExistJob->GetEndedStatus());
			CHECK_EQUAL(TES_NONE, pNewJob->GetEndedStatus());
		}

		TEST_FIXTURE(FJobMgr, InstantJob)
		{
			GJobT_Script* pInstantJob1 = new GJobT_Script(&m_ModuleAI, NULL, L"", L"");
			GJobT_Script* pInstantJob2 = new GJobT_Script(&m_ModuleAI, NULL, L"", L"");
			MockJob*	 pJob = new MockJob(&m_ModuleAI);
			
			CHECK_EQUAL(pInstantJob1->IsInstant(), true);

			m_JobMgr.Push(pInstantJob1);
			m_JobMgr.Push(pInstantJob2);
			m_JobMgr.Push(pJob);
			m_JobMgr.Run(0.0f);

			CHECK_EQUAL(m_JobMgr.GetCurrJob(), pJob);
		}

		namespace JobStart
		{
			// �Էµ� job�� ������ ������ ���
			TEST_FIXTURE(FJobMgr, Fail)
			{
				MockStartFailJob* pJob = new MockStartFailJob(&m_ModuleAI);
				m_JobMgr.Push(pJob);
				m_JobMgr.Run(0.0f);

				CHECK_EQUAL(true, m_JobMgr.IsEmpty());
			}

			// �Էµ� job�� ������ ������ ���
			TEST_FIXTURE(FJobMgr, Success)
			{
				MockJob* pJob = new MockJob(&m_ModuleAI);
				m_JobMgr.Push(pJob);
				m_JobMgr.Run(0.0f);

				CHECK_EQUAL(false, m_JobMgr.IsEmpty());
				CHECK_EQUAL(0, m_JobMgr.GetCount());
				CHECK_EQUAL(TES_RUN, pJob->GetEndedStatus());
				CHECK_EQUAL(m_JobMgr.GetCurrJob(), pJob);
			}
		}		
	}

	namespace PushFront
	{
		// �Էµ� job�� NULL�� ���
		TEST_FIXTURE(FJobMgr, PushNull)
		{			
			m_JobMgr.PushFront(NULL);
			m_JobMgr.Run(0.0f);

			CHECK_EQUAL(true, m_JobMgr.IsEmpty());
		}

		// �̹� job�� ������ �ִ� ���
		TEST_FIXTURE(FJobMgr, AlreadyJobExist)
		{
			MockJob* pExistJob = new MockJob(&m_ModuleAI);
			MockJob* pNewJob = new MockJob(&m_ModuleAI);
			m_JobMgr.Push(pExistJob);
			m_JobMgr.Run(0.0f);
			m_JobMgr.PushFront(pNewJob);
			m_JobMgr.Run(0.0f);

			CHECK_EQUAL(false, m_JobMgr.IsEmpty());
			CHECK_EQUAL(TES_CANCEL, pExistJob->GetEndedStatus());
			CHECK_EQUAL(TES_RUN, pNewJob->GetEndedStatus());
			CHECK_EQUAL(m_JobMgr.GetCurrJob(), pNewJob);
		}

		namespace JobStart
		{
			// �Էµ� job�� ������ ������ ���
			TEST_FIXTURE(FJobMgr, Fail)
			{
				MockJob* pExistJob = new MockJob(&m_ModuleAI);
				MockStartFailJob* pNewJob = new MockStartFailJob(&m_ModuleAI);

				m_JobMgr.Push(pExistJob);
				m_JobMgr.Run(0.0f);
				m_JobMgr.PushFront(pNewJob);
				m_JobMgr.Run(0.0f);

				CHECK_EQUAL(0, m_JobMgr.GetCount());
				CHECK_EQUAL(TES_RUN, pExistJob->GetEndedStatus());
				CHECK_EQUAL(m_JobMgr.GetCurrJob(), pExistJob);
			}

			// �Էµ� job�� ������ ������ ���
			TEST_FIXTURE(FJobMgr, Success)
			{
				MockJob* pExistJob = new MockJob(&m_ModuleAI);
				MockJob* pNewJob = new MockJob(&m_ModuleAI);

				m_JobMgr.Push(pExistJob);
				m_JobMgr.Run(0.0f);
				m_JobMgr.PushFront(pNewJob);
				m_JobMgr.Run(0.0f);

				CHECK_EQUAL(1, m_JobMgr.GetCount());
				CHECK_EQUAL(TES_CANCEL, pExistJob->GetEndedStatus());
				CHECK_EQUAL(TES_RUN, pNewJob->GetEndedStatus());
				CHECK_EQUAL(m_JobMgr.GetCurrJob(), pNewJob);
			}
		}		
	}

	// job�� ����� ���
	TEST_FIXTURE(FJobMgr, Cancel)
	{
		MockJob* pJob1 = new MockJob(&m_ModuleAI);
		MockJob* pJob2 = new MockJob(&m_ModuleAI);
		MockJob* pJob3 = new MockJob(&m_ModuleAI);
		m_JobMgr.Push(pJob1);
		m_JobMgr.Push(pJob2);
		m_JobMgr.Push(pJob3);
		m_JobMgr.Run(0.0f);

		m_JobMgr.Cancel();

		CHECK_EQUAL(2, m_JobMgr.GetCount());
		CHECK(NULL == m_JobMgr.GetCurrJob());
	}

	// jobmgr�� ������ ���
	TEST_FIXTURE(FJobMgr, Suspend)
	{
		MockJob* pJob = new MockJob(&m_ModuleAI);
		m_JobMgr.Push(pJob);
		m_JobMgr.Run(0.0f);

		m_JobMgr.Suspend();

		CHECK_EQUAL(TES_SUSPEND, pJob->GetEndedStatus());
		CHECK_EQUAL(m_JobMgr.GetCurrJob(), pJob);
	}

	// jobmgr�� ������ �ٽ� ������ ���
	TEST_FIXTURE(FJobMgr, Resume)
	{
		MockJob* pJob = new MockJob(&m_ModuleAI);
		m_JobMgr.Push(pJob);
		m_JobMgr.Run(0.0f);

		m_JobMgr.Suspend();
		m_JobMgr.Resume();

		CHECK_EQUAL(TES_RESUME, pJob->GetEndedStatus());
		CHECK_EQUAL(m_JobMgr.GetCurrJob(), pJob);
	}

	namespace Clear
	{
		// ���� job�� ����ϸ鼭 Clear�� ���
		TEST_FIXTURE(FJobMgr, CancelCurJob)
		{
			MockJob* pJob1 = new MockJob(&m_ModuleAI);
			MockJob* pJob2 = new MockJob(&m_ModuleAI);
			MockJob* pJob3 = new MockJob(&m_ModuleAI);
			m_JobMgr.Push(pJob1);
			m_JobMgr.Push(pJob2);
			m_JobMgr.Push(pJob3);
			m_JobMgr.Run(0.0f);

			m_JobMgr.Clear(true);
			m_JobMgr.Run(0.0f);

			CHECK_EQUAL(true, m_JobMgr.IsEmpty());
		}

		// ���� job�� ������� �ʰ� Clear�� ���
		TEST_FIXTURE(FJobMgr, NoCancelCurJob)
		{
			MockJob* pJob1 = new MockJob(&m_ModuleAI);
			MockJob* pJob2 = new MockJob(&m_ModuleAI);
			MockJob* pJob3 = new MockJob(&m_ModuleAI);
			m_JobMgr.Push(pJob1);
			m_JobMgr.Push(pJob2);
			m_JobMgr.Push(pJob3);
			m_JobMgr.Run(0.0f);

			m_JobMgr.Clear(false);
			m_JobMgr.Run(0.0f);

			CHECK_EQUAL(0, m_JobMgr.GetCount());			
			CHECK_EQUAL(TES_RUN, pJob1->GetEndedStatus());
			CHECK_EQUAL(m_JobMgr.GetCurrJob(), pJob1);
		}
	}

	namespace Run
	{
		// ������ ���
		TEST_FIXTURE(FJobMgr, Suspended)
		{
			MockJob* pJob = new MockJob(&m_ModuleAI);
			m_JobMgr.Push(pJob);
			m_JobMgr.Run(0.0f);

			m_JobMgr.Suspend();
			m_JobMgr.Run(1);

			CHECK_EQUAL(TES_SUSPEND, pJob->GetEndedStatus());
			CHECK_EQUAL(m_JobMgr.GetCurrJob(), pJob);
		}

		// ���� job�� NULL�� ���
		TEST_FIXTURE(FJobMgr, CurrJobIsNULL)
		{
			MockJob* pJob1 = new MockJob(&m_ModuleAI);
			MockJob* pJob2 = new MockJob(&m_ModuleAI);
			m_JobMgr.Push(pJob1);
			m_JobMgr.Push(pJob2);
			m_JobMgr.Run(0.0f);

			m_JobMgr.Cancel();
			m_JobMgr.Run(1);

			CHECK_EQUAL(0, m_JobMgr.GetCount());
			CHECK_EQUAL(TES_RUN, pJob2->GetEndedStatus());
			CHECK_EQUAL(m_JobMgr.GetCurrJob(), pJob2);
		}

		// ���� job�� ���� ����� �Ϸ��� ���
		TEST_FIXTURE(FJobMgr, CurrJobIsCompleted)
		{
			MockRun_CompletedJob* pJob = new MockRun_CompletedJob(&m_ModuleAI);
			pJob->Attr.bAutoDeletable = false;
			m_JobMgr.Push(pJob);
			m_JobMgr.Run(0.0f);

			m_JobMgr.Run(1);

			CHECK_EQUAL(true, m_JobMgr.IsEmpty());
			CHECK_EQUAL(TES_COMPLETE, pJob->GetEndedStatus());

			SAFE_DELETE(pJob);
		}

		// ���� job�� ���� ����� ����� ���
		TEST_FIXTURE(FJobMgr, CurrJobIsCancled)
		{
			MockRun_CanceledJob* pJob = new MockRun_CanceledJob(&m_ModuleAI);
			pJob->Attr.bAutoDeletable = false;
			m_JobMgr.Push(pJob);
			m_JobMgr.Run(0.0f);

			m_JobMgr.Run(1);

			CHECK_EQUAL(true, m_JobMgr.IsEmpty());
			CHECK_EQUAL(TES_CANCEL, pJob->GetEndedStatus());

			SAFE_DELETE(pJob);
		}

		// ���� ��ҵɶ� Clear�ɼ��� ���� �ִٸ� �� �����췯�� �ʱ�ȭ�Ѵ�.
		//TEST_FIXTURE(FJobMgr, CurrJobIsCanceledWithJobClearOption)
		//{
		//	MockRun_CanceledJob* pJob = new MockRun_CanceledJob(&m_ModuleAI);
		//	pJob->Attr.bClearJobListWhenCanceled = true;
		//	pJob->Attr.bAutoDeletable = false;
		//	MockJob* pJob2 = new MockJob(&m_ModuleAI);
		//	MockJob* pJob3 = new MockJob(&m_ModuleAI);
		//	
		//	m_JobMgr.Push(pJob);
		//	m_JobMgr.Push(pJob2);
		//	m_JobMgr.Push(pJob3);
		//	m_JobMgr.Run(0.0f);

		//	CHECK_EQUAL(2, m_JobMgr.GetCount());

		//	m_JobMgr.Run(1);
		//
		//	CHECK_EQUAL(TES_CANCEL, pJob->GetEndedStatus());
		//	CHECK_EQUAL(0, m_JobMgr.GetCount());
		//	CHECK_EQUAL(true, m_JobMgr.IsEmpty());

		//	SAFE_DELETE(pJob);
		//}

		// ���� job�� ���� ����� �ð� �ʰ��� ���
		TEST_FIXTURE(FJobMgr, CurrJobIsTimeOut)
		{
			MockRun_TimeOutJob* pJob = new MockRun_TimeOutJob(&m_ModuleAI);
			pJob->Attr.bAutoDeletable = false;
			m_JobMgr.Push(pJob);
			m_JobMgr.Run(0.0f);

			m_JobMgr.Run(1);

			CHECK_EQUAL(true, m_JobMgr.IsEmpty());
			CHECK_EQUAL(TES_TIMEOUT, pJob->GetEndedStatus());

			SAFE_DELETE(pJob);			
		}
	}	
}