#pragma once

class CSBasicAttributeCalculator
{
public:
	// ���� ����
	static int CalcMaxHP(int nLevel, int nCON);
	static int CalcMaxEN(int nLevel, int nINT);
	static int CalcMaxSP(int nLevel, int nDEX);
};