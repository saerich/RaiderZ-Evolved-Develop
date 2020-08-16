// UnitTest++ 용 xml 과 mlog 동시에 남기는 리포터, 테스트 어플리케이션에서 include 해서 쓰세요
#pragma once

#include "TestDetails.h"
#include "XmlTestReporter.h"
#include <fstream>

class RUnitTestReporter : public UnitTest::XmlTestReporter
{
private:

public:
	explicit RUnitTestReporter(std::ostream& ostream) : XmlTestReporter(ostream) {}
	virtual ~RUnitTestReporter()
	{
	}

	virtual void ReportTestStart(UnitTest::TestDetails const& test)
	{
		XmlTestReporter::ReportTestStart(test);
		printf("%s , %s \n{\n", test.suiteName, test.testName);
		mlog("%s , %s \n{\n", test.suiteName, test.testName);
	}

	virtual void ReportFailure(UnitTest::TestDetails const& test, char const* failure)
	{
		XmlTestReporter::ReportFailure(test, failure);
		printf("\t %s(%d): error %s in %s\n", test.filename, test.lineNumber, failure, test.testName);
		mlog("\t %s(%d): error %s in %s\n", test.filename, test.lineNumber, failure, test.testName);
	}
	virtual void ReportTestFinish(UnitTest::TestDetails const& test, float secondsElapsed)
	{
		XmlTestReporter::ReportTestFinish(test, secondsElapsed);
		printf("\t %s Test time : %5.2f sec\n", test.testName, secondsElapsed);
		printf("}\n");

		mlog("\t %s Test time : %5.2f sec\n", test.testName, secondsElapsed);
		mlog("}\n");
	}
	virtual void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed)
	{
		XmlTestReporter::ReportSummary(totalTestCount, failedTestCount, failureCount, secondsElapsed);
		printf("Suit Total count : %d, ", totalTestCount);
		printf("Suit Failed Test count : %d, ", failedTestCount);
		printf("Suit Failure count : %d\n", failureCount);
		printf("Suit Test time : %5.2f sec\n", secondsElapsed);

		mlog("Suit Total count : %d, ", totalTestCount);
		mlog("Suit Failed Test count : %d, ", failedTestCount);
		mlog("Suit Failure count : %d\n", failureCount);
		mlog("Suit Test time : %5.2f sec\n", secondsElapsed);
	}
};

class RXmlAndStdTestReporter : public UnitTest::XmlTestReporter
{
public:
	explicit RXmlAndStdTestReporter(std::ostream& ostream) : XmlTestReporter(ostream) {}
private:
	virtual void ReportFailure(UnitTest::TestDetails const& details, char const* failure)
	{
		XmlTestReporter::ReportFailure(details, failure);

		char const* const errorFormat = "%s(%d): error: Failure in %s: %s\n";

		printf(errorFormat, details.filename, details.lineNumber, details.testName, failure);
	}

	virtual void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed)
	{
		XmlTestReporter::ReportSummary(totalTestCount, failedTestCount, failureCount, secondsElapsed);

		if (failureCount > 0)
			printf("FAILURE: %d out of %d tests failed (%d failures).\n", failedTestCount, totalTestCount, failureCount);
		else
			printf("Success: %d tests passed.\n", totalTestCount);
		printf("Test time: %.2f seconds.\n\n", secondsElapsed);
	}
};

