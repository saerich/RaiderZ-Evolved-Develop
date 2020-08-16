#include "stdafx.h"
#include "GExpCalculator.h"
#include "GCalculator.h"
#include "GMath.h"
#include "SDef.h"
#include "GGlobal.h"
#include "GFatigueSystem.h"

SLevelTable	GExpCalculator::m_LevelTable;
GNpcXPTable GExpCalculator::m_NpcXPTable;

float GExpCalculator::CalcExpPercent(int nExp, int nCurrentLevel)
{
	if (nCurrentLevel >= MAX_LEVEL) return 0;

	float fPercent = 0;

	int nRequiredXP = GetLevelUpRequiredXP(nCurrentLevel);

	float fRate;
	
	if (nRequiredXP > 0)
	{
		fRate = nExp / (float)(nRequiredXP);
	}
	else
	{
		fRate = 0.0f;
	}

	fPercent = (fRate * 100.0f);

	return fPercent;
}


int GExpCalculator::CalcXPFromNPC(int nPCLevel, int nNPCLevel, int nNPCGrade)
{
	// NPC�� �׿��� ���� ����ġ ����
	// http://iworks2.maiet.net/wiki/sh:formulas:xp

	int nLevelFactor = GCalculator::CalcLevelFactor(nPCLevel, nNPCLevel);
	float fXPFactor_Level = CalcXPFactorLevel(nLevelFactor);
	float fXPFactor_Grade = m_NpcXPTable.GetFactorGrade(nNPCGrade);

	// 0.001���ϴ� ���� float ���� ����
	int nXP = static_cast<int>((nNPCLevel * 5 + 25) * fXPFactor_Level * fXPFactor_Grade + 0.001f);

	return nXP;
}

float GExpCalculator::CalcBeneficiaryCountFactor(int BeneficiaryCount)
{
	// �ο��� ���� ���ʽ� ����ġ ����
	if (1 == BeneficiaryCount)
	{
		return 1.0f;
	}

	float fBeneficiaryFactor = static_cast<float>(1 + 0.1 * BeneficiaryCount);

	return fBeneficiaryFactor;
}

float GExpCalculator::CalcLevelNormalizeFactor(int nLevel, int nSumLevel, int nSameServerBeneficiaryCount, int nAllBeneficiaryCount)
{
	// �����ڵ��� ������ Normailze�ؼ� ����
	// http://iworks2.maiet.net/wiki/sh:party
	// �����ڰ� ���� ������ �л�Ǿ��������� ���缭���� �����ڸ����� Normalize
	
	float fFactor = (static_cast<float>(nSameServerBeneficiaryCount) / static_cast<float>(nAllBeneficiaryCount)) * (static_cast<float>(nLevel) / static_cast<float>(nSumLevel));

	return fFactor;
}

float GExpCalculator::CalcFatigueFactor(short nFatigueQuantity)
{
	FATIGUE_TYPE nFatigueType = gsys.pFatigueSystem->GetFatigueType(nFatigueQuantity);

	switch (nFatigueType)
	{
	case FT_TOO_BAD:	{ return 1.0f; }break;
	case FT_BAD: 		{ return 1.0f; }break;
	case FT_NORMAL:		{ return 1.5f; }break;
	case FT_GOOD: 		{ return 1.8f; }break;
	case FT_VERY_GOOD:	{ return 2.0f; } break;
	}

	return 0;
}

float GExpCalculator::CalcXPFactorLevel( int nLevelFactor )
{
	if (nLevelFactor >= 5)
	{
		return GMath::Round4Combat(((nLevelFactor * 0.1f) + 0.5f), 1);
	}
	else
	{
		return GMath::Round4Combat(((nLevelFactor * nLevelFactor) * 0.05f), 1);
	}

	return 1.0f;
}

int GExpCalculator::GetLevelUpRequiredXP( int nCurrentLevel )
{
	return m_LevelTable.GetLevelUpRequiredXP(nCurrentLevel);
}

int GExpCalculator::CalcNewLevel(int nCurrentLevel, int nCurrentExp, const int nMaxLevel)
{
	if (nCurrentLevel > nMaxLevel - 1)
		return nCurrentLevel; // Ŭ�� ���� �ְ���

	if (nCurrentLevel >= MAX_LEVEL) return false;

	int nNewLevel = nCurrentLevel;
	int nNewExp = nCurrentExp;

	int nRequiredXP = m_LevelTable.GetLevelUpRequiredXP(nCurrentLevel);

	while (nNewExp >= nRequiredXP && nNewLevel < MAX_LEVEL)
	{
		nNewLevel++;

		nNewExp -= nRequiredXP;
		nRequiredXP = m_LevelTable.GetLevelUpRequiredXP(nNewLevel);
	}

	return nNewLevel;
}

int GExpCalculator::CalcNewExp(int nCurrentLevel, int nCurrentExp, const int nMaxLevel)
{
	if (nCurrentLevel > nMaxLevel - 1)
		return nCurrentExp;

	if (nCurrentLevel >= MAX_LEVEL) return false;

	int nNewLevel = nCurrentLevel;
	int nNewExp = nCurrentExp;

	int nRequiredXP = m_LevelTable.GetLevelUpRequiredXP(nCurrentLevel);

	while (nNewExp >= nRequiredXP && nNewLevel < MAX_LEVEL)
	{
		nNewLevel++;

		nNewExp -= nRequiredXP;
		nRequiredXP = m_LevelTable.GetLevelUpRequiredXP(nNewLevel);
	}

	return nNewExp;
}
