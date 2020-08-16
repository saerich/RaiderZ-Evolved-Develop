#include "stdafx.h"
#include "RBackgroundWorker.h"
#include "REngine.h"
#include "RDevice.h"
#include "mmsystem.h"
#include "RResourceID.h"

namespace rs3 {

unsigned int WINAPI RBackgroundWorker::WorkerThreadProc(void* _pWorker)
{
	RBackgroundWorker* pWorker = (RBackgroundWorker*)_pWorker;

#ifdef _DEBUG
	cml2::SetThisThreadName("RBackgroundWorker");
#endif

	bool bShutdown(false);
	while(!bShutdown)
	{
		pWorker->m_bSleptWorkerThread = true;

		DWORD dwEvent = WaitForMultipleObjects(SEND_EVENT_NUM, pWorker->m_hThreadControlEvents, FALSE, INFINITE);

		switch(dwEvent)
		{
			case RECEIVE_TRHEAD_ACTIVATE:
			{
				pWorker->m_bSleptWorkerThread = false;

				RBackgroundWorkUnit::BG_TICKET* pTicket = pWorker->m_runningQueue.PopTicket();
				while(pTicket)
				{
					// ������ �۾�
					{
						RTimeChecker tc;
						tc.Start();
						int nWorkUnitID = -1;

						pWorker->m_csTicket.Lock();
						if (pTicket->GetWorkUnit())
						{
							nWorkUnitID = pTicket->GetWorkUnit()->GetDebuggingID();
							pTicket->GetWorkUnit()->BackgroundWork();
						}
						pWorker->m_csTicket.Unlock();

						if (nWorkUnitID >= 0 && nWorkUnitID <= 50)
						{
							pWorker->m_DebuggingInfo.nBackgroundWorkUnitCount[nWorkUnitID]++;
							pWorker->m_DebuggingInfo.nBackgroundWorkUnitTimes[nWorkUnitID] += tc.Check();
						}
					}

					// �۾��Ϸ�
					pWorker->m_completeQueue.Insert(pTicket);

					// �����۾�
					pTicket = pWorker->m_runningQueue.PopTicket();
				}
			}
			break;
			case RECEIVE_TRHEAD_HALT:
			{
				bShutdown = true;
			}
			break;
		}
	}

	SetEvent( pWorker->m_hThreadWillBeDieEvent );

	int m_nReturnValue = 0;
	_endthreadex(m_nReturnValue);

	return (m_nReturnValue);
}

void RBackgroundWorker::Init()
{
	unsigned int dwThreadIdDumy(0);

	// init Thread Event
	// send to worker
	m_hThreadControlEvents[SEND_ACTIVATE_THREAD] = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hThreadControlEvents[SEND_HALT_THREAD]     = CreateEvent(NULL, FALSE, FALSE, NULL);

	// receive from worker
	m_hThreadWillBeDieEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_hWorkerThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, WorkerThreadProc, (void*)this, 0, &dwThreadIdDumy));

	_ASSERT(m_hWorkerThread);

	m_bInited = true;
}

void RBackgroundWorker::Destroy()
{
	if (!m_bInited) return;

	ClearQueues();

	// ���� ����������� 5�� ���� ���ڱ⸦ ��ٸ���.
	int nCountSleep(0);
	while(m_bSleptWorkerThread == false && nCountSleep < 500)
	{
		Sleep(10);
		++nCountSleep;
	}

	// worker thread ����
	SetEvent( m_hThreadControlEvents[SEND_HALT_THREAD]);
	if (WaitForSingleObject( m_hThreadWillBeDieEvent, 10000 ) == WAIT_TIMEOUT )
	{
		if( TerminateThread(m_hWorkerThread, 0) == FALSE )
			mlog("Thread Termination Error Code : %d\n", GetLastError() );
	}
	CloseHandle(m_hWorkerThread);

	// close event
	CloseHandle(m_hThreadControlEvents[SEND_ACTIVATE_THREAD]);
	CloseHandle(m_hThreadControlEvents[SEND_HALT_THREAD]);
	CloseHandle(m_hThreadWillBeDieEvent);

	m_bInited = false;
}

void RBackgroundWorker::AddToReadyQueue( RBackgroundWorkUnit* pWorkUnit )
{
	_ASSERT(IsInited());
	if (pWorkUnit == NULL) return;

	// complete update�ÿ� invalidate ������ ���ϰ� �ι�° �۾��� ��û�� �� �ִ�.
	if (pWorkUnit->m_pTicket == NULL)
	{
		pWorkUnit->m_pBackgroundWorker = this;
		pWorkUnit->m_pTicket = new RBackgroundWorkUnit::BG_TICKET(pWorkUnit);
	}
	else
	{
		_ASSERT(pWorkUnit->m_pTicket->GetWorkUnit() == pWorkUnit);
	}

	RBackgroundWorkUnit::BG_TICKET* pTicket = pWorkUnit->m_pTicket;

	// ticket �Ӽ�
	pTicket->m_nPriority			= pWorkUnit->CalcPriority();


	// ����
	if (pTicket->m_nPriority == RBGWK_SLEEP)
	{
		pWorkUnit->SetState(RBackgroundWorkUnit::WS_SLEEPING);
		m_sleepQueue.Insert(pTicket);
	}
	else
	{
		pWorkUnit->SetState(RBackgroundWorkUnit::WS_RUNNING);
		m_readyQueue.Insert(pTicket);
	}
}

void RBackgroundWorker::WakeUpWorker()
{
	// �����尡 ���� ������ �����
	if( m_bSleptWorkerThread == true )
		SetEvent( m_hThreadControlEvents[SEND_ACTIVATE_THREAD] );
}

void RBackgroundWorker::FlushReadyQueueAndRun()
{
	if (!m_bInited) return;

	if (m_sleepQueue.IsEmpty() == false)
	{
		m_sleepQueue.ReOrderAndMoveNotSleepTicketTo(m_readyQueue);
	}

	if (m_readyQueue.IsEmpty() == false)
	{
		// copy to running queue
		m_readyQueue.MoveTo(m_runningQueue);
	}

	if (m_runningQueue.IsEmpty() == false)
	{
		// �۾������� �����
		WakeUpWorker();
	}
}

void RBackgroundWorker::ReOrderReadyQueue()
{
	if (!m_bInited) return;

	m_readyQueue.ReOrder();
}

void RBackgroundWorker::ReOrderRunningQueue()
{
	if (!m_bInited) return;

	m_runningQueue.ReOrder();
	m_completeQueue.ReOrder();
}

void RBackgroundWorker::ClearQueues()
{
	if (!m_bInited) return;

	m_sleepQueue.Clear();
	m_readyQueue.Clear();
	m_runningQueue.Clear();
	m_completeQueue.Clear();
}

void RBackgroundWorker::Update()
{
	if (!m_bInited) return;

	UpdateCompleteQueue();

	FlushReadyQueueAndRun();

//	PostNotifyQueue();

	static uint32 nLastTime = timeGetTime();
	uint32 nNowTime = timeGetTime();
	if (nNowTime - nLastTime > 1000)
	{
		LogStatus();
		nLastTime = nNowTime;
	}
	
}

int RBackgroundWorker::GetRemainedWorkableTicketSize()
{
	return (int)m_readyQueue.GetSize() + (int)m_runningQueue.GetSize() + (int)m_completeQueue.GetSize();
}


void RBackgroundWorker::ForceBackgroundWorkStop()
{
	_ASSERT(m_bRequestedStopWorking == false);
	m_bRequestedStopWorking = true;
	while (m_bSleptWorkerThread)
	{
		Sleep(10);
	}
	m_bRequestedStopWorking = false;
}

void RBackgroundWorker::LogStatus()
{
	return;

	int nTotalBackgroundWorkTime = 0;
	int nTotalCompleteWorkTime = 0;

	for (int i = 0; i < 8; i++)
	{
		nTotalBackgroundWorkTime += m_DebuggingInfo.nBackgroundWorkUnitTimes[i];
		nTotalCompleteWorkTime += m_DebuggingInfo.nCompleteWorkUnitTimes[i];
	}

	mlog("sleep=%d , ready=%d , running=%d , complete=%d , bg_time=%u , fg_time=%u\n", 
		m_sleepQueue.GetSize(), m_readyQueue.GetSize(), m_runningQueue.GetSize(), m_completeQueue.GetSize(),
		nTotalBackgroundWorkTime, nTotalCompleteWorkTime);

	// 0 - terrain
	// 1 - tree
	// 2 - water
	// 3 - actor
	// 4 - material
	// 5 - actorshader
	// 6 - xcharacter
	// 7 - xcharacter_parts

	string strWorkUnitNames[8] = {"terrain", 
		                          "tree", 
								  "water",
								  "actor",
								  "material",
								  "actorshader",
								  "xcharacter",
								  "xcharacter_parts"};

	for (int i = RRESOURCETYPE::RR_UNKNOWN; i < RRESOURCETYPE::RR_MAX; i++)
	{
		mlog_filter("birdkr", "%s=(%u,%u:%u,%u) , ",
			RRESOURCETYPE::GetResoruceIDString(i),
			m_DebuggingInfo.nBackgroundWorkUnitCount[i], 
			m_DebuggingInfo.nBackgroundWorkUnitTimes[i],
			m_DebuggingInfo.nCompleteWorkUnitCount[i],
			m_DebuggingInfo.nCompleteWorkUnitTimes[i]);
	}
	mlog_filter("birdkr", "\n");
}

RBackgroundWorker::RBackgroundWorker() : m_bRequestedStopWorking(false)
, m_bSleptWorkerThread(true)
, m_dwAcceptableUpdateTime(10)
, m_bInited(false)
, m_bSmoothUpdate(true)
{

}

void RBackgroundWorker::UpdateCompleteQueue()
{
	static int nSkipCompleteQueue = 0;
	if (m_bSmoothUpdate && nSkipCompleteQueue > 0)
	{
		--nSkipCompleteQueue;
		return;
	}

	RBackgroundWorkUnit::BG_TICKET* pTicket = m_completeQueue.PopTicket();
	DWORD dwTimeStamp = REngine::GetTimer().GetAbsoluteTime();

	//#define _BW_REPORT
	#ifdef _BW_REPORT
	struct DebugReporter
	{
		std::string name;
		void SetName(RBackgroundWorkUnit::BG_TICKET* pTicket)
		{
			name = pTicket->GetWorkUnit()->GetName();

			REngine::m_strDebugFlags.clear();
			REngine::m_strDebugInfo.clear();
			REngine::m_uintDebugInfo.clear();
			REngine::m_strDebugFlags += "/check_actor_createtime";
		}
		void Report(unsigned int _time)
		{
			if (_time > 300)
			{
				mlog("UpdateCompleteQueue overtime = %d, %s\n",_time, name.c_str());
				size_t cnt = REngine::m_strDebugInfo.size();
				for (size_t i = 0; i < cnt; ++i)
				{
					mlog("%s, %d\n", REngine::m_strDebugInfo[i].c_str(), REngine::m_uintDebugInfo[i]);
				}
			}
		}
	} reporter;
	#endif

	while (pTicket)
	{
		if ( pTicket->IsExpired() )
		{
			// 1
			SAFE_DELETE(pTicket);
		}
		else
		{
			// 2
			if (pTicket->GetWorkUnit())
			{
				RTimeChecker tc;
				tc.Start();

				int nWorkUnitID = pTicket->GetWorkUnit()->GetDebuggingID();

				#ifdef _BW_REPORT
				reporter.SetName(pTicket);
				#endif

				pTicket->GetWorkUnit()->UpdateCompleteWork();
				//mlog("loading: fg (%s) , time=(%u)\n", work_name, tc.Check());
				// �� ������ �ǹ̴� ������ ?
				if (nWorkUnitID >= 0 && nWorkUnitID <= 50)
				{
					m_DebuggingInfo.nCompleteWorkUnitCount[nWorkUnitID]++;
					m_DebuggingInfo.nCompleteWorkUnitTimes[nWorkUnitID] += tc.Check();
				}
			}

			// 3
			// NULL�� �ƴ� ���� �ٽ� BackgroundWork�� �� ����̴�.
			if ( pTicket->IsExpired() )
			{
				SAFE_DELETE(pTicket);
			}
		}


		// Acceptable�� �ð����� �۾��� �ߴٸ� ���� �۾�. �ƴϸ� ���߱�
		// RBackgroundWorkable�� OnUpdateCompleteWork�� ���� �����ϹǷ�
		// m_dwAcceptableUpdateTime�� 0�̴��� �� �۾��� �����Ѵ�.
		unsigned int nElapsedTime = REngine::GetTimer().GetAbsoluteTime() - dwTimeStamp;
		if ( nElapsedTime > m_dwAcceptableUpdateTime )
		{
			// ����°��� ���̱� ���� ������ ������Ʈ�� complete queue�� ��ŵ���� ����
			float fFps = REngine::GetDevice().GetFrameRate();
			if (fFps > 15.f)
				nSkipCompleteQueue = int(nElapsedTime / 5);
			else if (fFps > 30.f)
				nSkipCompleteQueue = int(nElapsedTime / 2);
			else if (fFps > 40.f)
				nSkipCompleteQueue = (int)nElapsedTime;

			#ifdef _BW_REPORT
			reporter.Report(nElapsedTime);
			#endif
			break;
		}

		pTicket = m_completeQueue.PopTicket();
	}
}

// void RBackgroundWorker::RegisterNotify(RResource* pResource)
// {
// 	m_notificationQueue.push_back( pResource );
// }
// 
// void RBackgroundWorker::CancelNotify(RResource* pResource)
// {
// 	m_notificationQueue.remove( pResource );
// }
// 
// //#pragma optimize( "", off )
// 
// void RBackgroundWorker::PostNotifyQueue()
// {
//  	TIMEPROFILE("RBackgroundWorker::PostNotifyQueue");
// 
// 	while( !m_notificationQueue.empty() )
// 	{
// 		RResource* pNotifier = m_notificationQueue.front();
// 		m_notificationQueue.pop_front();
// 
// 		// TODO: ����, ���°��� �����ϱ� ���� �ӽ� ����
// 		char buffer[256];
// 
// 		pNotifier->SendNotifyToListeners();
// 
// 		strcpy_s( buffer, pNotifier->GetName().c_str() );
// 	}
// }
// 
// //#pragma optimize( "", on )

////////////////////////////////////////////////////////////////////////
// RBackgroundJobSafeQueue
void RBackgroundJobSafeQueue::Insert( RBackgroundWorkUnit::BG_TICKET* pTicket )
{
	_ASSERT(pTicket != NULL);

	cml2::MCriticalSection::Guard guard(m_csLock);

	m_Queue.insert( JOB_QUEUE::value_type(pTicket->m_nPriority, pTicket) );
}

void RBackgroundJobSafeQueue::MoveTo( RBackgroundJobSafeQueue& tarQueue )
{
	cml2::MCriticalSection::Guard guard1(m_csLock);
	cml2::MCriticalSection::Guard guard2(tarQueue.m_csLock);

	tarQueue.m_Queue.insert( m_Queue.begin(), m_Queue.end() );
	m_Queue.clear();
}

void RBackgroundJobSafeQueue::ReOrderAndMoveNotSleepTicketTo( RBackgroundJobSafeQueue& tarQueue )
{
	cml2::MCriticalSection::Guard guard1(m_csLock);
	cml2::MCriticalSection::Guard guard2(tarQueue.m_csLock);

	for (JOB_QUEUE::iterator itr = m_Queue.begin(); itr != m_Queue.end(); )
	{
		RBackgroundWorkUnit::BG_TICKET* pTicket = itr->second;
		RBackgroundWorkUnit* pWorkUnit = pTicket->GetWorkUnit();
		if (pWorkUnit)
		{
			int WorkUnitPriority = pWorkUnit->CalcPriority();
			if( WorkUnitPriority < pTicket->m_nPriority ) pTicket->m_nPriority = WorkUnitPriority;

			if (pTicket->m_nPriority != RBGWK_SLEEP)
			{
				pWorkUnit->SetState(RBackgroundWorkUnit::WS_RUNNING);
				tarQueue.m_Queue.insert( JOB_QUEUE::value_type(pTicket->m_nPriority, pTicket) );
				itr = m_Queue.erase(itr);
			}
			else
			{
				++itr;
			}
		}
		else
		{
			SAFE_DELETE(pTicket);
			itr = m_Queue.erase(itr);
		}
	}
}


RBackgroundWorkUnit::BG_TICKET* RBackgroundJobSafeQueue::PopTicket()
{
	cml2::MCriticalSection::Guard guard(m_csLock);

	if (m_Queue.empty())
	{
		return NULL;
	}

	RBackgroundWorkUnit::BG_TICKET* pTicket = NULL;
	JOB_QUEUE::iterator itr = m_Queue.begin();

	pTicket = itr->second;

	m_Queue.erase(itr);

	return pTicket;
}

void RBackgroundJobSafeQueue::ReOrder()
{
	cml2::MCriticalSection::Guard guard(m_csLock);
	if (m_Queue.empty())
		return;

	JOB_QUEUE tempQueue(m_Queue);
	m_Queue.clear();
	for (JOB_QUEUE::iterator itr = tempQueue.begin(); itr != tempQueue.end(); ++itr )
	{
		RBackgroundWorkUnit::BG_TICKET* pTicket = itr->second;
		RBackgroundWorkUnit* pWorkUnit = pTicket->GetWorkUnit();
		if (pWorkUnit)
		{
			pTicket->m_nPriority = pWorkUnit->CalcPriority();
			m_Queue.insert( JOB_QUEUE::value_type(pTicket->m_nPriority, pTicket) );
		}
	}
}

void RBackgroundJobSafeQueue::Clear()
{
	cml2::MCriticalSection::Guard guard(m_csLock);

	for (JOB_QUEUE::iterator itr = m_Queue.begin(); itr != m_Queue.end(); ++itr )
	{
		RBackgroundWorkUnit::BG_TICKET* pTicket = itr->second;
		if (pTicket->GetWorkUnit())
		{
			pTicket->GetWorkUnit()->m_pTicket = NULL;	// �����ִ°� ��ȿȭ
		}
		SAFE_DELETE(pTicket);
	}
	m_Queue.clear();
}

size_t RBackgroundJobSafeQueue::GetSize()
{
	cml2::MCriticalSection::Guard guard(m_csLock);
	return m_Queue.size();
}

bool RBackgroundJobSafeQueue::IsEmpty()
{
	cml2::MCriticalSection::Guard guard(m_csLock);
	return m_Queue.empty();
}

} // namespace rs3
