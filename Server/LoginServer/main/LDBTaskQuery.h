#ifndef _GDBTASK_UPDATE_H
#define _GDBTASK_UPDATE_H


#include "LDBTask.h"
#include "LDBAsyncTask.h"
#include "MMemPool.h"


/*
�Ʒ� ���� ��ü���� ������ ���и� �ϴ��� �׿� ���� ó���� ���� �ʰ� ��� ������ �Ѵ�.
�׷��� ������ �߰��� ������ ���� �ϴ��� ����� ���� �۾��� ���ؼ��� �Ϸ� �����
�ʿ���� �۾��� ���ؼ��� ����ؾ� �Ѵ�.
*/


class LAccountDBTaskQuery : public LDBAsyncTask, public MMemPool<LAccountDBTaskQuery>
{
public :
	LAccountDBTaskQuery(const SDBTASK_ID nID);
	virtual ~LAccountDBTaskQuery();

	void					OnExecute(mdb::MDatabase& rfDB);
	mdb::MDB_THRTASK_RESULT	_OnCompleted();	
};

class LGameDBTaskQuery : public LDBAsyncTask, public MMemPool<LGameDBTaskQuery>
{
public :
	LGameDBTaskQuery(const SDBTASK_ID nID);
	virtual ~LGameDBTaskQuery();

	void					OnExecute(mdb::MDatabase& rfDB);
	mdb::MDB_THRTASK_RESULT	_OnCompleted();	
};

class LLogDBTaskQuery : public LDBAsyncTask, public MMemPool<LLogDBTaskQuery>
{
public :
	LLogDBTaskQuery(const SDBTASK_ID nID);
	virtual ~LLogDBTaskQuery();

	void					OnExecute(mdb::MDatabase& rfDB);
	mdb::MDB_THRTASK_RESULT	_OnCompleted();	
};


#endif
