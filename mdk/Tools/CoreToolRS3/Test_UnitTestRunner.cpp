#include "StdAfx.h"
#include "RUnitTestRunner.h"
#include "RUnitTestReporter.h"

#include "Test_UnitTestRunner.h"
#include "UnitTest++.h"

int RunUnitTest()
{
	rs3::RUnitTestRunner::GetInstance().InitMockEngine();

	mlog("\n=============== UnitTest ===============\n");

	const char* TEST_XML_FILENAME = "Result_UnitTest.xml";

	std::ofstream f(TEST_XML_FILENAME);
	RXmlAndStdTestReporter reporter(f);  
	int failure_count = UnitTest::RunAllTests();

	rs3::RUnitTestRunner::GetInstance().HaltMockEngine();

	return failure_count;
}