#include "stdafx.h"
#include "GStateFactorCalculator.h"
#include "MMath.h"
#include "GConst.h"

SUITE(StateFactorCalculator)
{
	class Fixture
	{
	public:
		Fixture()
		{
			// Default�� ���� 1.0f
			GConst::Init(WLUA);
			GConst::STATE_FACTOR_FOR_DAMAGE_BY_BACK = 1.2f;
			GConst::STATE_FACTOR_FOR_MF_VALUE_BY_BACK = 1.2f;
		}

	public:
		GStateFactorCalculator calc;
	};

	TEST_FIXTURE(Fixture, CalcStateFactorForDamage)
	{
		for(int i = 0; i < MF_SIZE; i++)
		{
			MF_STATE mfState = static_cast<MF_STATE>(i);
			CHECK_CLOSE(1.0f, calc.CalcStateFactor(mfState, false), MMath::TOLER);
		}
	}

	TEST_FIXTURE(Fixture, CalcStateFactorForDamageByBack)
	{		
		// �ڿ��� �ǰ��� �켱�Ѵ�.
		CHECK_CLOSE(1.0f, calc.CalcStateFactor(MF_STUN, false), MMath::TOLER);		
		CHECK_CLOSE(1.2f, calc.CalcStateFactor(MF_STUN, true), MMath::TOLER);
	}

	TEST_FIXTURE(Fixture, CalcStateFactorForMfValue)
	{
		for(int i = 0; i < MF_SIZE; i++)
		{
			MF_STATE mfState = static_cast<MF_STATE>(i);
			CHECK_CLOSE(1.0f, calc.CalcMotionFactor(mfState, false), MMath::TOLER);
		}
	}

	TEST_FIXTURE(Fixture, CalcStateFactorForMfValueByBack)
	{
		// �ڿ��� �ǰ��� �켱�Ѵ�.
		CHECK_CLOSE(1.0f, calc.CalcMotionFactor(MF_STUN, false), MMath::TOLER);		
		CHECK_CLOSE(1.2f, calc.CalcMotionFactor(MF_STUN, true), MMath::TOLER);
	}
}
