#pragma once

#include "CSMotionFactorHelper.h"
#include "CSChrInfo.h"

#define FORCED_RELEASE_TIME_DELAY		0.8f				// ���� ��� ���� �ð� �߰� ��

class GMotionFactorHelper : public CSMotionFactorHelper
{
public:
	static float CalcReleaseTime( MF_STATE nMF, int nWeight, float fStandupTime, ENTITY_TYPEID nEntityTypeID );
};