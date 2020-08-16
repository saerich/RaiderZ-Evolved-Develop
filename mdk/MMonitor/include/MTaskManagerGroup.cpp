#include "stdafx.h"

#include <winsock2.h>
#include <windows.h>

#include "MTask.h"
#include "MTaskManager.h"
#include "MTaskManagerGroup.h"

#include <crtdbg.h>
#include <utility>

using std::pair;


namespace mmonitor {

MTaskManagerGroup::MTaskManagerGroup()
{
	m_nIndexCache = 0;
	m_IsUsableNameIndex = true;
}


MTaskManagerGroup::~MTaskManagerGroup()
{
	Release();
}


MTaskManager* MTaskManagerGroup::GetTaskManager( const DWORD dwIndex )
{
	if( dwIndex < m_TaskManagerList.size() )
		return m_TaskManagerList[ dwIndex ];

	return NULL;
}


MTaskManager* MTaskManagerGroup::GetTaskManager( const string& strTaskManagerName )
{
	if( 0 == strTaskManagerName.length() )
		return NULL;

	for( DWORD i = 0; i < GetTaskManagerCount(); ++i )
	{
		if( strTaskManagerName == m_TaskManagerList[i]->GetName() )
			return m_TaskManagerList[ i ];
	}

	return NULL;
}


MTaskManager* MTaskManagerGroup::GetTaskManager( const string& strType, const string& strIP, const unsigned short nPort )
{
	MTaskManager* pTaskManager;

	for( DWORD i = 0; i < GetTaskManagerCount(); ++i )
	{
		pTaskManager = m_TaskManagerList[ i ];

		if( string(TASK_NETWORK_TYPE) == strType )
		{
			if( (nPort == pTaskManager->GetPort()) &&
				(strType == pTaskManager->GetTaskType()) &&
				(strIP == pTaskManager->GetIPStr()) )
			{
				return m_TaskManagerList[ i ];
			}
		}
		else
		{
			if( strType == pTaskManager->GetTaskType() )
				return m_TaskManagerList[ i ];
		}
	}

	return NULL;
}


bool MTaskManagerGroup::AddTask( const string& strType, const string& strIP, const unsigned short nPort, MTask* pTask )
{
	// Task�� ����Ѵ�.
	// ���� ������ TaskManager�� ���ٸ� ���� ������ �Ѵ�.
	// Task�� TaskManager�� ��ϵǱ� ���� �ϸ�ũ�� �ص־� �Ѵ�.
	MTaskManager* pTaskManager = GetTaskManager( strType, strIP, nPort );
	if( NULL == pTaskManager )
	{
		pTaskManager = new MTaskManager( strType );
		if( 0 == pTaskManager )
			return false;

		pTaskManager->SetIPStr( strIP );
		pTaskManager->SetPort( nPort );

		if( string(TASK_TYPE_NETWORK) == strType )
		{
			if( !pTaskManager->MakePingTask(MakeIndex()) )
			{
				DeleteMonitorTaskManager( pTaskManager );
				return false;
			}
		}

		m_TaskManagerList.push_back( pTaskManager );
	}

	// ���ؽ� ����.
	pTask->SetIndex( MakeIndex() );

	// Task�ϸ�ũ.
	if( !AddTaskToMap(pTask) )
		return false;

	// �ϸ�ũ�� �Ϸ�� Task�� TaskManager�� ��ϵȴ�.
	if( !pTaskManager->AddTask(pTask) )
	{
		DeleteTaskFromMap( pTask );
		return false;
	}

	return true;
}


bool MTaskManagerGroup::AddTaskManager( MTaskManager* pTaskManager )
{
	if( NULL != GetTaskManager(pTaskManager->GetTaskType()
		, pTaskManager->GetIPStr()
		, pTaskManager->GetPort()) )
	{
		return false;
	}

	if( NULL == pTaskManager->GetTask(MONITOR_PING_TASK_NAME) )
	{
		if( !pTaskManager->MakePingTask(MakeIndex()) )
		{
			_ASSERT( 0 );
			return false;
		}
	}

	MTask* pTask;
	const DWORD dwTaskCount = pTaskManager->GetTaskCount();
	for( DWORD i = 0; i < dwTaskCount; ++i )
	{
		pTask = pTaskManager->GetTask( i );
		if( 0 == pTask )
			return false;

		pTask->SetIndex( MakeIndex() );

		// ��� Task�� �ϸ�ũ �Ѵ�.
		if( !AddTaskToMap(pTask) )
			return false;
	}

	// �ϸ�ũ�� �Ϸ�Ǹ� TaskManager�� ����Ѵ�.
	m_TaskManagerList.push_back( pTaskManager );

	return true;
}


bool MTaskManagerGroup::AddTaskManager( const string& strType, const string& strIP, const unsigned short nPort )
{
	// ����ִ� TaskManager�� ����.

	if( NULL != GetTaskManager(strType, strIP, nPort) )
		return false;

	MTaskManager* pTaskManager = new MTaskManager( strType );
	if( 0 == pTaskManager )
		return false;

	pTaskManager->SetIPStr( strIP );
	pTaskManager->SetPort( nPort );

	if( !pTaskManager->MakePingTask(MakeIndex()) )
	{
		DeleteMonitorTaskManager( pTaskManager );
		return false;
	}

	m_TaskManagerList.push_back( pTaskManager );

	return true;
}


void MTaskManagerGroup::Release()
{
	MTaskManager* pTaskManager;
	for( DWORD i = 0; i < GetTaskManagerCount(); ++i )
	{
		pTaskManager = GetTaskManager( i );
		if( 0 == pTaskManager )
		{
			_ASSERT( 0 );
			continue;
		}

		DeleteMonitorTaskManager( pTaskManager );
	}

	m_TaskManagerList.clear();
	m_TaskMap.clear();
	m_TaskIndexMap.clear();
}


bool MTaskManagerGroup::AddTaskToMap( MTask* pTask )
{
	// Task�� �ϸ�ũ �Ѵ�.

	if( 0 == pTask )
		return false;

	// ���� �̸��� Task�� �ִ��� �˻�.
	if( m_IsUsableNameIndex && (NULL == GetTask(pTask->GetName())) )
	{
		m_TaskMap.insert( pair<string, MTask*>(pTask->GetName(), pTask) );
	}
	else
	{
		// ���� ���� �̸��� ������, �̸����� ���ؽ��Ѱ� ����� �� ���� �Ѵ�.
		if( m_IsUsableNameIndex )
		{
			_ASSERT( 0 && "duplicated task name." );
			m_TaskMap.clear();
		}

		m_IsUsableNameIndex = false;
	}
	
	// id�� �׻� �����ϹǷ� �׳� �߰��� �Ѵ�.
	m_TaskIndexMap.insert( pair<DWORD, MTask*>(pTask->GetIndex(), pTask) );

	return true;
}


MTask* MTaskManagerGroup::GetTask( const string& strTaskName )
{
	if( !m_IsUsableNameIndex )
		return NULL;

	map< string, MTask* >::iterator itFind = m_TaskMap.find( strTaskName );
	if( m_TaskMap.end() == itFind )
		return NULL;

	return itFind->second;
}


DWORD MTaskManagerGroup::MakeIndex()
{
	if( MMONITOR_TASK_MAX_INDEX < (++m_nIndexCache) )
		m_nIndexCache = 1;

	return m_nIndexCache;
}


void MTaskManagerGroup::DeleteTaskFromMap( MTask* pTask )
{
	map< string, MTask* >::iterator itFind = m_TaskMap.find( pTask->GetName() );
	if( m_TaskMap.end() != itFind )
		m_TaskMap.erase( itFind );

	map< DWORD, MTask* >::iterator itFindIndex = m_TaskIndexMap.find( pTask->GetIndex() );
	if( m_TaskIndexMap.end() != itFindIndex )
		m_TaskIndexMap.erase( itFindIndex );
}


MTask* MTaskManagerGroup::GetTask( const DWORD dwIndex )
{
	map< DWORD, MTask* >::iterator itFind = m_TaskIndexMap.find( dwIndex );
	if( m_TaskIndexMap.end() == itFind )
		return NULL;

	return itFind->second;	
}

}