#pragma once

#include "MCommandTable.h"
#include "CCommandTable.h"
#include "CCommandTable_Master.h"
#include "SMinetInitScope.h"

// Ŀ�ǵ� ���� ////////////////////////////////////////////////////////////////
// Ŀ�ǵ� ��ȣ 20001 ~ 30000 ���� ���

// ���� ///////////////////////////////////////////////////////////////////////

// ���μ����� ���
enum ZCommandTable
{

};

void ZAddCommandTable();


class ZMinetInitScope : public SMinetInitScope
{
public:
	ZMinetInitScope() : SMinetInitScope()
	{
		ZAddCommandTable();
	}
};
