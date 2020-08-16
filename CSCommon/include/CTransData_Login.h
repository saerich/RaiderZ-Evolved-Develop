#ifndef _CTRANS_DATA_LOGIN_H
#define _CTRANS_DATA_LOGIN_H

#include "MTypes.h"
#include "CSDef.h"

// ### ���� ###
//
// 1byte�� byte alignment�� �����Ǿ� �ֱ� ������
// �� �Ʒ� ���ķ� include ���� �ϸ� �ȵ˴ϴ�. 
// - birdkr(2007-06-29)
//
// ### ���� ###

#pragma pack(push, old)
#pragma pack(1)

struct TD_MasterServerInfo
{
	TCHAR				szName[SERVER_NAME_LEN + 1];
};

struct TD_MasterServerConnectionInfo
{
	uint8				nAddr[4];	// 128.0.0.1
	int					nPort;
};






#pragma pack(pop, old)


#endif