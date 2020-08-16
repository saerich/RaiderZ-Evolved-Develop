#ifndef _GDBTASK_H
#define _GDBTASK_H

#include "SDBTask.h"
#include "SDBTaskID.h"
#include "GDBTaskExecCounter.h"
#include "MMemPool.h"


#include <vector>


namespace mdb
{
	class MDatabase;
}


class GDBAsyncTask : public SDBAsyncTask
{
public :
	GDBAsyncTask(const MUID& uidReqPlayer, const SDBT_DBTYPE DBType, const SDBTASK_ID TaskID, const size_t nReserveSQLSize = 1);
	virtual ~GDBAsyncTask();

	const MUID&						GetReqPlayer();
	DWORD							GetExecuteElapsedTime();
	DWORD							GetWorkTm(const DWORD dwNowTm);
	void							SetPostTm(const DWORD dwTime);

	mdb::MDB_THRTASK_RESULT			OnCompleted()					override;

	static GDBTaskSortedList		GetExecCountDecrSortedList();

	bool							IsSync();
	void							SetSync(const bool bSync);	

public :
	virtual void					OnPrePost();
	virtual bool					OnPost();
	virtual void					OnFailPost();


protected :
	virtual void					IncreaseRefCount(const MUID& uidReqPlayer);
	virtual void					DecreaseRefCount(const MUID& uidReqPlayer);
	virtual void					AddTaskCount();


protected :
	// OnCompleted�Լ����� ȣ��Ǵ� �Լ���.
	virtual void					_OnPreCompleted();								// ��� �˻縦 �ϱ����� ���� �Ǵ� �κ�. ����� ������� �׻� ����ȴ�.
	virtual mdb::MDB_THRTASK_RESULT	_OnCompleted()	{ return mdb::MDBTR_SUCESS; }	// ���� ó���� �̰� ������ �ؼ� ó��.
	virtual mdb::MDB_THRTASK_RESULT	_OnFailed()		{ return mdb::MDBTR_SUCESS; }	// ���� ó���� �̰� ��� �޾Ƽ� ó��.
	virtual void					_OnEndCompleted();								// �������� ������ ����Ǿ�� �� ��� ������ �Ѵ�.
	

protected :
	MUID							m_uidReqPlayer;
	DWORD							m_dwPostTm;
	bool							m_bSync;
	
	static GDBTaskExecCounter		m_Conter;
};



#endif