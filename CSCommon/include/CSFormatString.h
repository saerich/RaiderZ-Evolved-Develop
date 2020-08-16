#pragma once

#include "CSFormatStringParameter.h"

class CStringResMgr;

class CSFormatString
{
public:
	// formater�� %s�� %d ��� {0}, {1} �� ���� ������ �Է��մϴ�.
	static tstring Format(tstring formater, FSParam param = FSParam());
	
	static void SetDictionary(CStringResMgr* pDictionary);

private:
	static CStringResMgr* sm_pDictionary;
};
