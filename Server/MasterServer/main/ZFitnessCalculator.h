#pragma once

class ZGameServerObject;

const int MAX_FITNESS_VALUE_TO_ALLOC_FIELD	= 100;	///< �ʵ带 �Ҵ��� ������ �� �� ����ϴ� ������ �ִ밪

class ZFitnessCalculator
{
public:
	static int CalcToCreateChannel(ZGameServerObject* pTargetServer, int nAdditionalValue = 0);
	static int CalcToCreateFieldGroup(ZGameServerObject* pTargetServer, int nAdditionalValue = 0);

private:
	static float CalcRate(int nCurValue, int nMaxValue);
	static int CalcFitnessValue(float fModifier, int nAdditionalValue);
};
