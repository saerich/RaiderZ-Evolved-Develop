#include "stdafx.h"
#include "LUnitTestRunner.h"
#include "LUnitTestStage1.h"

bool LUnitTestRunner::BeDoTest()
{
	// publish�̸� �����׽�Ʈ ����.
#ifdef _PUBLISH
	return false;
#endif

	return MUnitTestRunner::BeDoTest();
}

bool LUnitTestRunner::RunStage1()
{
	return Run<LUnitTestStage1>();
}

