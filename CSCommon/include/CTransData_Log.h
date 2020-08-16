#ifndef _CTRANS_DATA_LOG_H
#define _CTRANS_DATA_LOG_H

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

struct TDL_DATE
{
	tm date;
};

struct TDL_LOG_ITEM
{
	int64	nIUID;
	int		nQuantity;
};


#pragma pack(pop, old)


#endif