#pragma once

#include "ServerCommonUnitTestLib.h"

class UTestDatabase;
class SUnitTestStageGameDB;
class SUnitTestDBBuilder;

/*
UTestDatabaseContainerŬ������ �����׽�Ʈ���� Database��ü�� ����� �� �ְ� �̱������� ������ �ֱ� ���� Ŭ����.
Database��ü ������ ���� �ܿ� �ٸ� ����� �߰��ϸ� �ȵ�. - sunge.
*/
class SCOMMON_UNITTEST_API UTestDatabaseContainer
{
public :
	static UTestDatabaseContainer& GetUTestDatabaseContainer() 
	{ 
		return *(UTestDatabaseContainer::m_pTDBMgrContainer);
	}

	UTestDatabase& GetUTestDB();

	void Create();
	void Release();

private :
	friend SUnitTestDBBuilder;

	UTestDatabaseContainer();
	~UTestDatabaseContainer();	
	
private :
	static UTestDatabaseContainer*	m_pTDBMgrContainer;
	static int						m_nCrateCnt;

	UTestDatabase*					m_pUTestDB;
};


#define UTestDB UTestDatabaseContainer::GetUTestDatabaseContainer().GetUTestDB()