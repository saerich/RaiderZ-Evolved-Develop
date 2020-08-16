#ifndef _LASYNCDBTASK_H
#define _LASYNCDBTASK_H


#include "SDBTask.h"
#include "SDBTaskID.h"


class LDBAsyncTask : public SDBAsyncTask
{
public :
	LDBAsyncTask(const SDBT_DBTYPE DBType, const SDBTASK_ID TaskID, const size_t nReserveSQLSize = 1);
	virtual ~LDBAsyncTask();

	virtual mdb::MDB_THRTASK_RESULT	OnCompleted();

	virtual void					_OnPreCheck()					{}								// ��� �˻縦 �ϱ����� ���� �Ǵ� �κ�. ����� ������� �׻� ����ȴ�.
	virtual mdb::MDB_THRTASK_RESULT	_OnCompleted()					{ return mdb::MDBTR_SUCESS; }	// ���� ó���� �̰� ������ �ؼ� ó��.
	virtual mdb::MDB_THRTASK_RESULT	_OnFailed()						{ return mdb::MDBTR_SUCESS; }	// ���� ó���� �̰� ��� �޾Ƽ� ó��.
	virtual void					_OnProfile()					{}


protected :
};


#endif