#ifndef _GDBTASK_UPDATE_H
#define _GDBTASK_UPDATE_H


#include "GDBAsyncTask.h"
#include "MMemPool.h"


/*
�Ʒ� ���� ��ü���� ������ ���и� �ϴ��� �׿� ���� ó���� ���� �ʰ� ��� ������ �Ѵ�.
�׷��� ������ �߰��� ������ ���� �ϴ��� ����� ���� �۾��� ���ؼ��� �Ϸ� �����
�ʿ���� �۾��� ���ؼ��� ����ؾ� �Ѵ�.
*/


class GGameDBTaskQuery : public GDBAsyncTask, public MMemPool<GGameDBTaskQuery>
{
public :
	GGameDBTaskQuery(const MUID& uidReqPlayer, const SDBTASK_ID nTaskID);
	virtual ~GGameDBTaskQuery();

	void					OnExecute(mdb::MDatabase& rfDB) override;
	mdb::MDB_THRTASK_RESULT	_OnCompleted() override;	
};

class GGameDBTaskQueryIter : public GDBAsyncTask, public MMemPool<GGameDBTaskQueryIter>
{
public :
	GGameDBTaskQueryIter(const MUID& uidReqPlayer, const SDBTASK_ID nTaskID, const size_t nReserveSQLSize);
	virtual ~GGameDBTaskQueryIter();

	void					OnExecute(mdb::MDatabase& rfDB) override;
	mdb::MDB_THRTASK_RESULT	_OnCompleted() override;	
};

class GLogDBTaskQuery : public GDBAsyncTask, public MMemPool<GLogDBTaskQuery>
{
public :
	GLogDBTaskQuery(const MUID& uidReqPlayer, const SDBTASK_ID nTaskID);
	virtual ~GLogDBTaskQuery();

	void					OnExecute(mdb::MDatabase& rfDB) override;
	mdb::MDB_THRTASK_RESULT	_OnCompleted() override;	
};

#endif