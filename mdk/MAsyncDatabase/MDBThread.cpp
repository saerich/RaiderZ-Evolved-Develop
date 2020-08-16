#include "stdafx.h"
#include "MDBThread.h"
#include "MDBThreadTaskQ.h"
#include "MDBThreadTaskResultQ.h"


namespace mdb
{
	MDBThread::MDBThread()
	{
		m_bIsExit			= true;
		m_bIsExitCompleted	= false;
		m_nDBReconnectCount	= 0;
	}


	MDBThread::~MDBThread()
	{

	}


	HANDLE MDBThread::GetHandle()		
	{ 
		return m_hThread; 
	}


	DWORD MDBThread::GetThreadID()	
	{ 
		return m_dwThreadID; 
	}


	bool MDBThread::Create(const wstring& strDSN
						  , MDBThreadTaskQ* pTaskQ
						  , MDBThreadTaskResultQ* pTaskResultQ)
	{
		if (!m_DB.ConnectW(strDSN))
		{
			return false;
		}

		m_pTaskQ		= pTaskQ;
		m_pTaskResultQ	= pTaskResultQ;
		m_bIsExit		= false;			/// Create�� �����ϸ� false�� �Ѵ�.
											/// Create�������� ������ �����尡 Ȱ��ȭ ���� �ʴ´�.

		m_hThread = CreateThread(NULL, 0, _WorkThread, this, 0, &m_dwThreadID);
		if (NULL == m_hThread)
		{
			_ASSERT(NULL != m_hThread);
			m_bIsExit = true;

			return false;
		}

		return true;
	}

	void MDBThread::Release()
	{
		m_DB.Disconnect();
		m_nDBReconnectCount = 0;
	}

	void MDBThread::DecDBReconnectCount()
	{
		if (0 < m_nDBReconnectCount)
			--m_nDBReconnectCount;
	}

	void MDBThread::IncDBReconnectCount()
	{
		if (MAX_RECONNECT_COUNT > m_nDBReconnectCount)
			++m_nDBReconnectCount;
	}

	int MDBThread::GetDBReconnectCount()
	{
		return m_nDBReconnectCount;
	}

	bool MDBThread::IsDBConnected()
	{
		if (m_DB.IsConnected())
		{
			DecDBReconnectCount();	
			return true;
		}

		IncDBReconnectCount();

		if (m_DB.Reconnect())
			return true;
		
		Sleep(1000 * GetDBReconnectCount());

		if (MAX_RECONNECT_COUNT <= GetDBReconnectCount())
			return true;	// �ִ� �õ� Ƚ���� �ʰ��ϸ� �׽�ũ 1���� �����ؼ� ������ APP�� �Ѱ��ش�.
							// ������ ������ �Ѱ��� �� �ִ� ����� �ʿ��ϴ�.

		return false;		
	}

	void MDBThread::Run()
	{
		_ASSERT(NULL != m_pTaskQ);
		_ASSERT(NULL != m_pTaskResultQ);

		if (!IsDBConnected())
			return;

		MDBThreadTask* pTask = m_pTaskQ->FetchTask();
		if (NULL == pTask)
		{
			return;
		}

		pTask->OnExecute(m_DB);

		m_pTaskResultQ->Push(pTask);
	}


	bool MDBThread::IsExit()
	{
		return m_bIsExit;
	}


	void MDBThread::SetExit()
	{
		m_bIsExit = true;
	}


	bool MDBThread::IsExitCompleted()
	{
		return m_bIsExitCompleted;
	}


	void MDBThread::SetExitCompleted()
	{
		m_bIsExitCompleted = true;
	}


	DWORD WINAPI MDBThread::_WorkThread(LPVOID pContext)
	{
		MDBThread* pThread = reinterpret_cast<MDBThread*>(pContext);

		_ASSERT(false == pThread->IsExit());
		
		while (false == pThread->IsExit())
		{
			pThread->Run();
		}

		pThread->SetExitCompleted();

		return 0;
	}
}