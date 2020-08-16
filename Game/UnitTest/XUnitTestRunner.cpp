#include "stdafx.h"
#include "XUnitTestRunner.h"
#include "XUnitTestStage1.h"
#include "XUnitTestStage2.h"
#include "XUnitTestStage3.h"
#include <fstream>
using namespace std;
using std::ofstream;


bool XUnitTestRunner::BeDoTest()
{
	// publish�̸� �����׽�Ʈ ����.
#ifdef _PUBLISH
	return false;
#endif

	// unittest�̸� �����׽�Ʈ ������ ��
#ifdef _UNITTEST
	return true;
#endif

	return MUnitTestRunner::BeDoTest();
}

bool XUnitTestRunner::RunStage1()
{
#ifdef _PUBLISH
	return true;
#endif

	PFC_THISFUNC;
	return Run<XUnitTestStage1>();
}

bool XUnitTestRunner::RunStage2()
{
	// Cookie���� ũ���ù����� �����ϴ�.
	return true;

#ifdef _PUBLISH
	return true;
#endif

	PFC_THISFUNC;
	return Run<XUnitTestStage2>();
}

bool XUnitTestRunner::RunStage3()
{
#ifdef _PUBLISH
	return true;
#endif

	PFC_THISFUNC;
	return Run<XUnitTestStage3>();
}

XUnitTestRunner::XUnitTestRunner() : MUnitTestRunner()
{
	InitCodeCoverage();
}

XUnitTestRunner::~XUnitTestRunner()
{
	CC_End();
}

void XUnitTestRunner::InitCodeCoverage()
{
	if (m_Config.m_bCheckCodeCoverage == false) return;

	char* CodeCoverageItems[] = {

#define CC_Add(e) #e,

#include "CodeCoverages.inc"

	};

#undef CC_Add

	CC_Begin(CodeCoverageItems, m_Config.m_strCodeCoverageFilter.c_str());
}