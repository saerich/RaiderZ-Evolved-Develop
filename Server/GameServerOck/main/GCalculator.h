#ifndef _GCALCULATOR_H
#define _GCALCULATOR_H

#include "GDef.h"
#include "GMath.h"
#include "GModGradeTable.h"
#include "GLevelFactorTable.h"
#include "GDamageCalculator.h"


class GEntityPlayer;
class GNPCInfo;

/// ���� ����
class GCalculator
{
private:
	static GLevelFactorTable			m_LevelFactorTable;
	static GModGradeTable				m_ModGradeTable;
	static GDamageCalculator			m_DamageCalculator;

public:
	static int CalcMaxTP(int nLevel);

	// ����
	static int CalcLevelFactor(int nActorLevel, int nTargetLevel);
	static float CalcModGrade(int nNPCGrade);
	static float CalcModGroup(int nGroupMembers);

	// ���� ����
	static int CalcFallingDamage(bool bIsFallenWater, float fFallingStartHeight, int nMaxHP);
	static int CalcFallingDebuffID(float fFallingHeight);

	// �����ʵ� ä�� ����
	static CHANNEL_STATUS CalcChannelStatus(int nCurPlayerCount, int nMaxPlayerCount);

private:
	static float CalcFallingDamageFactor(bool bIsFallenWater, float fFallingHeight);	
};


#endif