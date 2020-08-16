#include "stdafx.h"
#include "RGrassSystem.h"

using namespace rs3;

class TestWindStrengthAmplifier : public RGrassSystem_WindStrengthAmplifier
{
protected:
	float m_fRandAmplifier;
	float m_fRandUpdateTime;

	virtual float GetNewAmplifier() override
	{
		return m_fRandAmplifier;
	}
	virtual float GetNewUpdateTime() override
	{
		return m_fRandUpdateTime;
	}
public:
	TestWindStrengthAmplifier()
	{
		m_fRandAmplifier = 1.0f;
		m_fRandUpdateTime = 1.0f;
	}
	void SetRandAmplifierForTest(float fAmplifier) 
	{
		m_fRandAmplifier = fAmplifier;
	}
	void SetRandUpdateTimeForTest(float fUpdateTime) 
	{
		m_fRandUpdateTime = fUpdateTime;
	}
	float GetStrengthCoefficient()
	{
		return m_fStrengthCoefficient;
	}
};

TEST( TestGrassSystem, TestGrassSystemWindTickUpdater )
{
	TestWindStrengthAmplifier amp;
	const float fWindStrength = 1.0f;

	const float fStrengthCoefficient = amp.GetStrengthCoefficient();

	float fTick = 0.1f;
	EXPECT_NEAR((fTick * fWindStrength * fStrengthCoefficient * 1.0f), amp.Strength(fTick, fWindStrength), 0.01f);

	amp.SetRandAmplifierForTest(2.0f);
	amp.SetRandUpdateTimeForTest(3.0f);
	
	// ���� ������Ʈ ƽ�� ���ƿ��� �ʾ����Ƿ� amp�� ��� 1.0�̴�
	EXPECT_NEAR((fTick * fWindStrength * fStrengthCoefficient * 1.0f), amp.Strength(fTick, fWindStrength), 0.01f);

	// �ð��� �������Ƿ� amp�� �ٲ����
	fTick = 3.0f;
	EXPECT_NEAR((fTick * fWindStrength * fStrengthCoefficient * 2.0f), amp.Strength(fTick, fWindStrength), 0.01f);
}