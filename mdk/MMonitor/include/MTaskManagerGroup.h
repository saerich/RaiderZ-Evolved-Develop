#ifndef _MMONITORTASKMANAGERGROUP
#define _MMONITORTASKMANAGERGROUP


#include <map>
#include <string>
#include <vector>


using std::map;
using std::string;
using std::vector;


namespace mmonitor {

#define MMONITOR_TASK_MAX_INDEX 4200000000


class MTask;
class MTaskManager;


class MTaskManagerGroup
{
protected :
	string					m_strName;
	vector< MTaskManager* >	m_TaskManagerList;	// TaskManager�� Type, IP, Port�� ������ �Ǿ� ����ȴ�.
												// ��� Task�� xml�� ���ǵ� Type, IP, Port�� ��ġ�ϴ� TaskManager�� ����� �ȴ�.

	map< string, MTask* >	m_TaskMap;			// Task�� ���� �˻��� ���ؼ� �̸����� �ϸ�ũ�� �س��´�.
												// ��� Task�� TaskManager�� ��ϵɶ� �ݵ�� �ϸ�ũ�Ǿ�� �Ѵ�.
	bool					m_IsUsableNameIndex;

	map< DWORD, MTask* >	m_TaskIndexMap;		// Index�� �̿��� �˻�.

	DWORD					m_nIndexCache;		// Task�� ���鶧 ���� Index�� �����Ҷ� ���ȴ�. Index�˻��� ���ؼ� ���.
												// Index�� 1���� ����. 0�� ������� ����.

private :
	bool			AddTaskManager( MTaskManager* pTaskManager );
	bool			AddTaskManager( const string& strType, const string& strIP, const unsigned short nPort );

protected :
	bool			AddTaskToMap( MTask* pTask );
	void			DeleteTaskFromMap( MTask* pTask );
	DWORD			MakeIndex();

public :
	MTaskManagerGroup();
	~MTaskManagerGroup();

	const string&	GetName() const								{ return m_strName; }
	const DWORD		GetTaskManagerCount() const 				{ return static_cast<DWORD>(m_TaskManagerList.size()); }
	const DWORD		GetTaskCount() const						{ return static_cast<DWORD>(m_TaskIndexMap.size()); }
	MTaskManager*	GetTaskManager( const DWORD dwIndex );
	MTaskManager*	GetTaskManager( const string& strTaskManagerName );
	MTaskManager*	GetTaskManager( const string& strType, const string& strIP, const unsigned short nPort );
	MTask*			GetTask( const string& strTaskName );
	MTask*			GetTask( const DWORD dwIndex );

	bool			AddTask( const string& strType, const string& strIP, const unsigned short nPort, MTask* pTask );
	
	void			Release();
};


}

#endif