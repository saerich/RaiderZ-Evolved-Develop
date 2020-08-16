#pragma once

#include "MThread2.h"
#include "RBackgroundWorkUnit.h"
#include "RTimer.h"
#include <map>
#include <list>

namespace rs3 {

class RSceneManager;
class RBackgroundWorkUnit;
class RBackgroundWorkNotifier;
class REngine;

// Background Work�� Priority�� ������. ���ڰ� Ŭ���� ���� Priority
enum RBACKGROUNDWORKER_PRIORITY
{
	RBGWK_HIGHEST		= -50000,
	RBGWK_NORMAL		= 0,

	RBGWK_HIGH_OFFSET	= 10000,	//���ָ� high �� ���������.
	

	RBGWK_SLEEP			= 50000
};

#define DEFAULT_DISTANCE_FOR_BACKGROUND_PRIORITY	30000.0f

class RBackgroundJobSafeQueue
{
public:
	typedef std::multimap< int, RBackgroundWorkUnit::BG_TICKET* > JOB_QUEUE;
private:
	JOB_QUEUE				m_Queue;
	cml2::MCriticalSection	m_csLock;
public:
	RBackgroundJobSafeQueue() {}
	~RBackgroundJobSafeQueue() {}
	void Insert(RBackgroundWorkUnit::BG_TICKET* pTicket);
	void MoveTo(RBackgroundJobSafeQueue& tarQueue);
	void ReOrderAndMoveNotSleepTicketTo(RBackgroundJobSafeQueue& tarQueue);
	void ReOrder();
	void Clear();
	RBackgroundWorkUnit::BG_TICKET* PopTicket();

	size_t GetSize();
	bool IsEmpty();
};


struct RBackgroundWorkDebuggingInfo
{
	unsigned int nBackgroundWorkUnitTimes[50];
	unsigned int nCompleteWorkUnitTimes[50];
	unsigned int nBackgroundWorkUnitCount[50];
	unsigned int nCompleteWorkUnitCount[50];

	RBackgroundWorkDebuggingInfo()
	{
		for (int i = 0; i < 50; i++)
		{
			nBackgroundWorkUnitTimes[i] = 0;
			nCompleteWorkUnitTimes[i] = 0;
			nBackgroundWorkUnitCount[i] = 0;
			nCompleteWorkUnitCount[i] = 0;
		}
	}
};

class RS_API RBackgroundWorker
{
	friend RSceneManager;
	friend RBackgroundWorkUnit;
	friend REngine;

private:
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// �۾����� ť
	//
	RBackgroundJobSafeQueue	m_readyQueue;				// main thread������ �����
	RBackgroundJobSafeQueue	m_sleepQueue;				// main thread������ �����
	RBackgroundJobSafeQueue	m_runningQueue;
	RBackgroundJobSafeQueue	m_completeQueue;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Thread ��� ���� �̺�Ʈ
	//
	enum SEND_EVENT
	{
		SEND_ACTIVATE_THREAD		= 0,
		SEND_HALT_THREAD,
		SEND_EVENT_NUM
	};
	enum
	{
		RECEIVE_TRHEAD_ACTIVATE		= WAIT_OBJECT_0,
		RECEIVE_TRHEAD_HALT			= WAIT_OBJECT_0 + 1
	};

	bool m_bInited;

	HANDLE					m_hThreadControlEvents[SEND_EVENT_NUM];
	HANDLE					m_hThreadWillBeDieEvent;


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// �۾��� ������(Worker Thread)���� �ڵ� �� Critical Section
	//
	cml2::MCriticalSection	m_csResourceLock;
	cml2::MCriticalSection	m_csTicket;

	HANDLE					m_hWorkerThread;
	bool					m_bSleptWorkerThread;
	bool					m_bRequestedStopWorking;

	// Worker Thread Proc
	static unsigned int WINAPI	WorkerThreadProc(void* _pWorker);


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Complete Queue ������Ʈ ����
	//
	DWORD					m_dwAcceptableUpdateTime;	// Acceptable Elapsed Time
	bool					m_bSmoothUpdate;
	void					UpdateCompleteQueue();		///< ���� �����忡�� ȣ���ϴ� Update()�Լ����� �θ�

	// notifier
//	std::list<RResource*>	m_notificationQueue;		// �۾����� ����ť

	// for debugging
	RBackgroundWorkDebuggingInfo m_DebuggingInfo;

	void LogStatus();
public:
	RBackgroundWorker();
	~RBackgroundWorker(){}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� / �Ҹ�
	//
	void					Init();			// �ʱ�ȭ
	void					Destroy();		// Ŭ����� �Ҹ�

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// ���ξ������ �۾���������� ����ȭ�� ���� Lock / UnLock
	//


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// ���ξ����忡���� �Ҹ��� queue ���� �Լ�
	// 1. AddToReadyQueue			: ReadyQueue�� �۾��߰�
	// 2. WakeUpWorker				: �۾������� �����
	// 3. FlushReadyQeueueAndRun	: ReadyQueue�� �ִ� �۾��� RunningQueue�� �ű�� �۾������� �����
	// 4. ReorderQueues				: �۾����� ť�� �켱������ �������Ѵ�.
	// 5. ClearQueues				: �۾����� ť�� ����.
	void AddToReadyQueue( RBackgroundWorkUnit* pWorkable );
	void WakeUpWorker();
	void FlushReadyQueueAndRun();
	void ReOrderReadyQueue();
	void ReOrderRunningQueue();
	void ClearQueues();

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// ���ξ����忡���� Update�� ȣ���� �� ����
	// 1. m_pBackgroundWorkUnit üũ NULL�̸� �����
	//      - ���ξ����尡 ������� InvalidateWorkable �Ҽ� ����
	// 2. OnUpdateCompleteWork �ҷ��ֱ�
	// 3. m_pBackgroundWorkUnit üũ NULL�̸� �����
	//      - OnUpdateCompleteWork�ÿ� InvalidateWorkable �ϴ°�찡 ��κ�.
	//      - OnUpdateCompleteWork�ÿ� ���ο� AddJob�� ��û�� �� �ְ� �̶��� �������� �ʴ´�.
	void Update();

	// Acceptable Time. ���� 0�̸� ���� �ϳ��� �۾��� �����Ѵ�.
	void	SetUpdateAcceptableTime( DWORD dwAcceptableTime, bool bSmooth )	{ m_dwAcceptableUpdateTime = dwAcceptableTime; m_bSmoothUpdate = bSmooth; }
	DWORD	GetUpdateAcceptableTime()										{ return m_dwAcceptableUpdateTime; }

	// queue size
	int GetRemainedWorkableTicketSize();


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// ������ ��׶��� �۾� ���߱�. �� �޽�带 ���� ���°��� �ùٸ� ������ �ƴմϴ�.
	// cf. ��׶��� �۾��� Ŭ���� �ϴµ� �ùٸ� ����� ������ ���߱Ⱑ �ƴ϶�
	// ClearAllQueues�� ���� ������ �۾��� ���߰� �ϴ°� �Դϴ�.
	//
	void ForceBackgroundWorkStop();

	bool IsInited() { return m_bInited; }

	void LockTicket()	{ m_csTicket.Lock();	}
	void UnLockTicket()	{ m_csTicket.Unlock();	}
	void LockResource() { m_csResourceLock.Lock(); }
	void UnLockResource() { m_csResourceLock.Unlock(); }

// 	void RegisterNotify(RResource* pResource);
// 	void CancelNotify(RResource* pResource);
// 	void PostNotifyQueue();
};


}


// �ؾ��� ��
// ť �����ϱ� - RunningQueue, COmpleteQueue -> SafeQueue Ŭ����
// BG_TICKET �����ϱ�
// �׽�Ʈ �׽�Ʈ

