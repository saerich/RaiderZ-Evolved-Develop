#pragma once

#include "CSCommonLib.h"


// Ŀ�ǵ� ��ȣ 20000 ~ 21000  ���� ���
enum CWorldLocatorCommandTable
{					
	MWC_COMM_WORLD_INFO_REQ = 20001,		///< (C->S)	���� ���� ��û
	MWC_COMM_WORLD_INFO_RES = 20002,		///< (S->C) ���� ���� ����
};

CSCOMMON_API void AddWorldLocatorCommandTable();