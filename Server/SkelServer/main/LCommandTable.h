#pragma once

#include "MCommandTable.h"
#include "CCommandTable.h"
#include "CCommandTable_Master.h"
#include "SMinetInitScope.h"

// Ŀ�ǵ� ���� ////////////////////////////////////////////////////////////////
// Ŀ�ǵ� ��ȣ 20001 ~ 30000 ���� ���

// ���� ///////////////////////////////////////////////////////////////////////

// ���μ����� ���
enum LCommandTable
{

};

void LAddCommandTable();


class LMinetInitScope : public SMinetInitScope
{
public:
	LMinetInitScope() : SMinetInitScope()
	{
		LAddCommandTable();
	}
};
