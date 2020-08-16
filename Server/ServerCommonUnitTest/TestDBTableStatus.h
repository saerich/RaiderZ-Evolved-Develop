#pragma once

#include "ServerCommonUnitTestLib.h"
#include "TestDBTable.h"

class SCOMMON_UNITTEST_API TestDBTableStatus
{
private:
	void Release();
public :
	TestDBTableStatus();
	~TestDBTableStatus();


	// xml���� �׽�Ʈ �����͸� �о�ͼ� �����Ѵ�.
	bool Load(const char* szFileName);

	TDBTableDef* GetTable(const string& strName);


	DBTableVec m_DBTableVec;
};


