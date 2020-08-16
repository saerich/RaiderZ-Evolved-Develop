#include "stdafx.h"
#include "XModulePost.h"

SUITE(ModulePost)
{
	TEST(TestMovementPostChecker_Rotate)
	{
		vec3 vPos = vec3(1000.0f, 1000.0f, 0.0f);
		vec3 vDir = vec3(0.0f, 1.0f, 0.0f);

		XMovementPostChecker checker;
		checker.SetLastPostedPosition(vPos);

		XMovementPostChecker::RESULT nResult;
		XMovementPostChecker::CheckArgs args(vPos, vDir, false, false, false, false, false, true);

		// ������ �ٲ�����Ƿ� PostRotate
		nResult = checker.Check(POST_MOVE_TICK, args);
		CHECK_EQUAL(XMovementPostChecker::RESULT_PostRotate, nResult);

		// ������ �ٲ��� �ʾҴ�
		nResult = checker.Check(POST_MOVE_TICK, args);
		CHECK_EQUAL(XMovementPostChecker::RESULT_NONE, nResult);

		// ������ �ٲ����.
		args.Dir = vec3(1.0f, 0.0f, 0.0f);
		nResult = checker.Check(POST_MOVE_TICK, args);
		CHECK_EQUAL(XMovementPostChecker::RESULT_PostRotate, nResult);

		// ������ �ٲ������ ƽ �ð��� ���� �ȵǾ���
		args.Dir = vec3(0.0f, 1.0f, 0.0f);
		nResult = checker.Check(0.001f, args);
		CHECK_EQUAL(XMovementPostChecker::RESULT_NONE, nResult);
	}

	TEST(TestMovementPostChecker_Move)
	{
		vec3 vPos = vec3(1000.0f, 1000.0f, 0.0f);
		vec3 vDir = vec3(0.0f, 1.0f, 0.0f);
		const vec3 vDeltaPos = vec3(0.0f, 50.0f, 0.0f);

		XMovementPostChecker checker;
		checker.SetLastPostedPosition(vPos);

		XMovementPostChecker::RESULT nResult;
		XMovementPostChecker::CheckArgs args(vPos, vDir, true, false, true, false, false, true);

		// �̵� ����
		args.Pos += vDeltaPos;
		nResult = checker.Check(POST_MOVE_TICK, args);
		CHECK_EQUAL(XMovementPostChecker::RESULT_PostMove, nResult);

		// ƽ�� �������� ũ�� ���������� �ʾҴ�.
		args.Pos += vec3(0.0f, 0.01f, 0.0f);
		nResult = checker.Check(0.01f, args);
		CHECK_EQUAL(XMovementPostChecker::RESULT_NONE, nResult);

		// ũ�� �������� �ʾ����� ƽ�� ������.
		args.Pos += vec3(0.0f, 0.01f, 0.0f);
		nResult = checker.Check(POST_MOVE_TICK, args);
		CHECK_EQUAL(XMovementPostChecker::RESULT_PostMove, nResult);

		// �̵��� ������
		args.bCurrentGoing = false;
		args.Pos += vec3(0.0f, 0.01f, 0.0f);
		nResult = checker.Check(POST_MOVE_TICK, args);
		CHECK_EQUAL(XMovementPostChecker::RESULT_PostMoveStop, nResult);

	}

	TEST(TestMovementPostChecker_Warp)
	{
		vec3 vPos = vec3(1000.0f, 1000.0f, 0.0f);
		vec3 vDir = vec3(0.0f, 1.0f, 0.0f);
		const vec3 vDeltaPos = vec3(0.0f, 50.0f, 0.0f);

		XMovementPostChecker checker;
		checker.SetLastPostedPosition(vPos);

		XMovementPostChecker::RESULT nResult;
		XMovementPostChecker::CheckArgs args(vPos, vDir, false, false, false, false, false, true);

		// ���ڱ� ��ġ�� �ٲ����.
		args.Pos += vec3(0.0f, 1000.0f, 0.0f);
		nResult = checker.Check(0.01f, args);
		CHECK_EQUAL(XMovementPostChecker::RESULT_PostStop, nResult);


	}

}
