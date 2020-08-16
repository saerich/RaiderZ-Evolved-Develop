#include "stdafx.h"
#include "MDBThreadTaskResultQ.h"


namespace mdb
{
	MDBThreadTaskResultQ::MDBThreadTaskResultQ()
	{
		m_bIsFatching		= false;
		m_pWorkingTaskList	= &m_TaskList[0];
		m_pFetchingTaskList = &m_TaskList[1];
	}


	MDBThreadTaskResultQ::~MDBThreadTaskResultQ()
	{

	}


	bool MDBThreadTaskResultQ::Init()
	{
		return true;
	}


	/*
	�� �Լ��� �ϳ��� �����忡���� ȣ���� �Ǿ�� �Ѵ�.
	��Ƽ �����忡�� ����ϸ� �������� �������� ����.
	*/
	MDB_THRTASK_RESULT_LIST* MDBThreadTaskResultQ::GetResultList()
	{
		_ASSERT(false == m_bIsFatching);
		if (m_bIsFatching)
		{
			// �ߺ� ȣ���� �Ǿ���... �̰� ��Ƽ �����忡�� ����ϸ� ������.
			return NULL;
		}

		m_bIsFatching = true;
		
		{
			MDBGuard Guard(&m_CS);

			MDB_THRTASK_RESULT_LIST* pTmp = m_pWorkingTaskList;
			m_pWorkingTaskList = m_pFetchingTaskList;
			m_pFetchingTaskList = pTmp;
		}

		m_bIsFatching = false;
		
		return m_pFetchingTaskList;
	}


	void MDBThreadTaskResultQ::Push(MDBThreadTask* pTask)
	{
		MDBGuard Guard(&m_CS);
		m_pWorkingTaskList->push_back(pTask);
	}


	void MDBThreadTaskResultQ::Release()
	{
		MDBGuard Guard(&m_CS);

		for (MDB_THRTASK_RESULT_LIST::iterator it = m_pWorkingTaskList->begin()
			; it != m_pWorkingTaskList->end()
			; ++it)
		{
			MDBThreadTask* pTask = (*it);
			delete pTask;
		}
		m_pWorkingTaskList->clear();


		for (MDB_THRTASK_RESULT_LIST::iterator it = m_pFetchingTaskList->begin()
			; it != m_pFetchingTaskList->end()
			; ++it)
		{
			MDBThreadTask* pTask = (*it);
			delete pTask;
		}
		m_pFetchingTaskList->clear();
	}


	size_t MDBThreadTaskResultQ::GetWoringQSize()
	{
		MDBGuard Guard(&m_CS);

		size_t nSize = m_pWorkingTaskList->size();

		return nSize;
	}


	size_t MDBThreadTaskResultQ::GetFetchingQSize()
	{
		MDBGuard Guard(&m_CS);

		size_t nSize = m_pFetchingTaskList->size();

		return nSize;
	}
}