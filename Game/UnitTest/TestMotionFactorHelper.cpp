#include "stdafx.h"
#include "XMotionFactorHelper.h"

SUITE(MotionFactorHelper)
{
	TEST(CalcKnockbackTime)
	{
		CHECK_CLOSE(1.0f, XMotionFactorHelper::CalcKnockbackTime(1000.0f), 0.001f);
		CHECK_CLOSE(2.0f, XMotionFactorHelper::CalcKnockbackTime(2000.0f), 0.001f);
		CHECK_CLOSE(0.8f, XMotionFactorHelper::CalcKnockbackTime(800.0f), 0.001f);
	}

	TEST(CalcKnockbackTime_Negative)
	{
		CHECK_CLOSE(0.0f, XMotionFactorHelper::CalcKnockbackTime(0.0f), 0.001f);
		CHECK_CLOSE(0.0f, XMotionFactorHelper::CalcKnockbackTime(-2000.0f), 0.001f);
	}

	TEST(CalcThrowupTime)
	{
		CHECK_CLOSE(1.0f, XMotionFactorHelper::CalcKnockbackTime(1000.0f), 0.001f);
		CHECK_CLOSE(2.0f, XMotionFactorHelper::CalcKnockbackTime(2000.0f), 0.001f);
		CHECK_CLOSE(0.8f, XMotionFactorHelper::CalcKnockbackTime(800.0f), 0.001f);
	}


	TEST(CalcThrowupMotionSpeed)
	{
		// ���ο���� �ʴ� 10���;� ����. : KNOCKBACK_TIME = 0.1


		// 1��¥�� �ִϸ��̼����� 10���� ��
		CHECK_CLOSE(1.0f, XMotionFactorHelper::CalcThrowupMotionSpeed(1000.0f, 0.0f, 1.0f), 0.001f);

		// 1��¥�� �ִϸ��̼����� 5���� ��
		CHECK_CLOSE(2.0f, XMotionFactorHelper::CalcThrowupMotionSpeed(500.0f, 0.0f, 1.0f), 0.001f);

		// 0.5��¥�� �ִϸ��̼����� 10���� ��
		CHECK_CLOSE(0.5f, XMotionFactorHelper::CalcThrowupMotionSpeed(1000.0f, 0.0f, 0.5f), 0.001f);

		// 1��¥�� �ִϸ��̼����� 20���� ��
		CHECK_CLOSE(0.5f, XMotionFactorHelper::CalcThrowupMotionSpeed(2000.0f, 0.0f, 1.0f), 0.001f);

		// 1��¥�� �ִϸ��̼����� 10���� ��
		CHECK_CLOSE(1.0f, XMotionFactorHelper::CalcThrowupMotionSpeed(2000.0f, 1000.0f, 1.0f), 0.001f);
	}

	TEST(CalcThrowupMotionSpeed_Negative)
	{
		CHECK(XMotionFactorHelper::CalcThrowupMotionSpeed(1000.0f, 2000.0f, 1.0f) > 0.0f);
		CHECK(XMotionFactorHelper::CalcThrowupMotionSpeed(1000.0f, 1000.0f, 1.0f) > 0.0f);
	}

}