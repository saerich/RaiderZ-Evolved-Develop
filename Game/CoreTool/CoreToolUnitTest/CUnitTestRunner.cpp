#include "stdafx.h"
#include "CUnitTestRunner.h"
#include "CUnitTestStage1.h"
//#include <fstream>
using namespace std;
//using std::ofstream;

bool CUnitTestRunner::BeDoTest()
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

bool CUnitTestRunner::RunStage1()
{
#ifdef _PUBLISH
	return true;
#endif

	return Run<CUnitTestStage1>();
}


CUnitTestRunner::CUnitTestRunner() : MUnitTestRunner()
{
	
}

CUnitTestRunner::~CUnitTestRunner()
{

}

