#pragma once


#include <list>
using std::list;


#include "MDBCriticalSection.h"


namespace mdb
{
	class MDBThreadTask;


	static const DWORD	MDB_TASK_Q_WAIT_IFINITE = INFINITE;
	static const DWORD	MDB_TASK_Q_DEFAULT_WAIT = 1000; // 1��.


	class MDBThreadTaskQ
	{
	public :
		MDBThreadTaskQ();
		~MDBThreadTaskQ();

		bool				Init(const DWORD dwWaitEventTime = MDB_TASK_Q_DEFAULT_WAIT 	// ť�� Task�� ������ ���� Task�� ť�� ��ϵɶ����� 
																						// ��ٸ��� �ð�.
																						// 0�̸� ��ٸ��� �ʴ´�.
																						// MDB_TASK_Q_WAIT_IFINITE�̸� �Էµɶ����� ��� ��ٸ���.
							);

		void				Push(MDBThreadTask* pTask);
		MDBThreadTask*		FetchTask();
		void				SetPushEvent();

		
		size_t				GetSize();


		void				Release();


	private :
		MDBThreadTask*		_FetchTaskUnsafe();


	private :
		typedef list<MDBThreadTask*> MDB_THRTASK_LIST;


		MDBCriticalSecton	m_CS;
		HANDLE				m_hPushEvent;
		MDB_THRTASK_LIST	m_TaskList;
		DWORD				m_dwWaitEventTime;	
	};
}