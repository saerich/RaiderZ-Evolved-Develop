#include "stdafx.h"
#include "MUnitTestReporter.h"
#include "TestDetails.h"
#include "MDebug.h"

MUnitTestReporter::MUnitTestReporter( std::ostream& ostream, bool bBreakOnFailure/*=false*/ ) 
						: UnitTest::XmlTestReporter(ostream), m_bBreakOnFailure(bBreakOnFailure)
{

}

BOOL MUnitTestReporter::_IsDebuggerPresent()
{
	// IsDebuggerPresent() �Լ��� ������ 
	// ���μ����� PEB����ü�� BeingDebuged�� ���� �������� �ζ��� ����� ���� ����. - birdkr

	BOOL Retval = 0;
	__asm
	{
		push eax
		mov eax,dword ptr fs:[0x18]
		mov eax,dword ptr ds:[eax+0x30]
		movzx eax,byte ptr ds:[eax+0x2]
		mov Retval,eax
		pop eax
	}
	return Retval;
}

void MUnitTestReporter::ReportFailure(UnitTest::TestDetails const& details, char const* failure)
{
	if (m_bBreakOnFailure && _IsDebuggerPresent() == TRUE)
	{
		// ���н� �극��ũ �ɵ��� �����Ǿ� ������ ��������� ���� �����.
		*static_cast<int*>(NULL) = 1;
	}

	UnitTest::XmlTestReporter::ReportFailure(details, failure);

	char const* const errorFormat = "%s(%d): error: Failure in %s: %s\n";

	mlog("\n\n >>> UnitTest Failure -----\n");
	mlog(errorFormat, details.filename, details.lineNumber, details.testName, failure);
	mlog("\n\n");
}

void MUnitTestReporter::ReportTestStart(UnitTest::TestDetails const& details)
{
	XmlTestReporter::ReportTestStart(details);
}

void MUnitTestReporter::ReportTestFinish(UnitTest::TestDetails const& details, float secondsElapsed)
{
	XmlTestReporter::ReportTestFinish(details, secondsElapsed);
}

void MUnitTestReporter::ReportSummary(int const totalTestCount, int const failedTestCount,
	int const failureCount, float secondsElapsed)
{
	UnitTest::XmlTestReporter::ReportSummary(totalTestCount, failedTestCount, failureCount, secondsElapsed);

	if (failureCount > 0)
		mlog("FAILURE: %d out of %d tests failed (%d failures).\n", failedTestCount, totalTestCount, failureCount);
	else
	{
		mlog("\n�ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ�\n\n");
		mlog("Success: %d tests passed.\n", totalTestCount);
		mlog("\n�ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ�\n\n");
	}
	mlog("Test time: %.2f seconds.\n\n", secondsElapsed);
}

