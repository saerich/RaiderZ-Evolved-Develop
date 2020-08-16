#ifndef _SLEVEL_TABLE_H
#define _SLEVEL_TABLE_H

#include "CSDef.h"

/// ���� ���̺�
class SLevelTable
{
protected:
	int m_RequiredXP[MAX_LEVEL];

	int CalcLevelUpRequiredXP(int nLevel);
	int GetIndex(int nLevel);
public:
	SLevelTable();
	int GetLevelUpRequiredXP(int nCurrentLevel);
};


#endif