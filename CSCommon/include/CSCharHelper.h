#pragma once

#include "CSCommonLib.h"
#include "CSChrInfo.h"

/// ĳ���� ���� ���� Ŭ����
class CSCOMMON_API CSCharHelper
{
public:
	static bool IsDeveloperGrade(GPlayerGrade nPlayerGrade)
	{
		if (nPlayerGrade == PLAYER_GRADE_TESTER ||
			nPlayerGrade == PLAYER_GRADE_DEVELOPER)
		{
			return true;
		}
		return false;
	}

	static bool IsGMGrade(GPlayerGrade nPlayerGrade)
	{
		if (PLAYER_GRADE_NORMAL < nPlayerGrade)
		{
			return true;
		}

		return false;
	}

	/// ���ͷ��� ������ �Ÿ� ���. nInteractionDistance�� 0�̸� Radius�� ��꿡 ���� ��������, 0���� ũ�� nInteractionDistance���� ������ �Ÿ�
	static float CalcValidInteractionDistance(float fTarColRadius, int nInteractionDistance=0);
};