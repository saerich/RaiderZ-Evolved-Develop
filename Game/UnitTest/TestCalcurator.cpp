#include "stdafx.h"
#include "XCalculator.h"

SUITE(Calcurator)
{
	TEST(TestPlayerMoveSpeed)
	{
		XCalculator calc;

		// �����׽�Ʈ���� XCONST ���� �ǵ�� �ȵǰ� �Ǿ��ִ�. - birdkr
		CHECK_CLOSE(XCONST::MOVE_SPEED_NONE, calc.CalcPlayerMoveSpeed(false, CS_NORMAL, WEAPON_NONE), 0.001f);

		CHECK_CLOSE(XCONST::MOVE_SPEED_NONE, calc.CalcPlayerMoveSpeed(false, CS_NORMAL, WEAPON_1H_SLASH), 0.001f);
		CHECK_CLOSE(XCONST::MOVE_SPEED_1HS, calc.CalcPlayerMoveSpeed(false, CS_BATTLE, WEAPON_1H_SLASH), 0.001f);

		CHECK_CLOSE(XCONST::MOVE_SPEED_NONE, calc.CalcPlayerMoveSpeed(false, CS_NORMAL, WEAPON_STAFF), 0.001f);
		CHECK_CLOSE(XCONST::MOVE_SPEED_STF, calc.CalcPlayerMoveSpeed(false, CS_BATTLE, WEAPON_STAFF), 0.001f);

		CHECK_CLOSE(XCONST::MOVE_BACK_SPEED_NONE, calc.CalcPlayerMoveSpeed(true, CS_NORMAL, WEAPON_NONE), 0.001f);

		CHECK_CLOSE(XCONST::MOVE_BACK_SPEED_NONE, calc.CalcPlayerMoveSpeed(true, CS_NORMAL, WEAPON_1H_SLASH), 0.001f);
		CHECK_CLOSE(XCONST::MOVE_BACK_SPEED_1HS, calc.CalcPlayerMoveSpeed(true, CS_BATTLE, WEAPON_1H_SLASH), 0.001f);

	}
}
