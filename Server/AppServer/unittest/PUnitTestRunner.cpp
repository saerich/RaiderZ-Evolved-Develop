#include "stdafx.h"
#include "PUnitTestRunner.h"
#include "PUnitTestStage1.h"

bool PUnitTestRunner::BeDoTest()
{
	// publish�̸� �����׽�Ʈ ����.
#ifdef _PUBLISH
	return false;
#endif

	return MUnitTestRunner::BeDoTest();
}

bool PUnitTestRunner::RunStage1()
{
	return Run<PUnitTestStage1>();
}

