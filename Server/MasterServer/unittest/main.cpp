#include "stdafx.h"
#include "MBaseUnitTest.h"
#include "ZCommandTable.h"
#include "SAppHelper.h"
#include "ZUnitTestRunner.h"
#include "ZUnitTest.h"
#include "ZConfig.h"
#include "MProfiler.h"
#include "MDebug.h"
#include "ZMemory.h"
#include "SProfileScope.h"
#include "ZDef.h"
#include "MStatisticsProfiler.h"

int RunOnlyUnitTest(LPSTR cmdline)
{
	int ret = ZUnitTestRunner::CCNET_UNITTEST_SUCCESS;

	// Stage2�� ���� App ����
	ZConfig::m_bWindowVisible = false;
	ZConfig::m_nPort = 7507;	// UnitTest�� ��� ��Ʈ �浹�� ���� �ȵǹǷ�

	MInitStatisticsProfile(true);

	MUnitTestConfig unittest_config;
	unittest_config.Load();

	int nUnitTestCount = 1;

	// ���� �׽�Ʈ ȸ�� ����
	if (strlen(cmdline) > 0)
	{
		char arg_unittest_count[256]="";

		int ret = sscanf(cmdline, "%s", arg_unittest_count);
		if (ret == 1)
		{
			nUnitTestCount = atoi(arg_unittest_count);
			if (nUnitTestCount <= 0) nUnitTestCount = 1;

			unittest_config.m_nRepeat = nUnitTestCount;
		}
	}

	// stage1 --
	if (unittest_config.CheckStageFilter(UNITTEST_STAGE1_NAME))
	{
		if (unittest_config.m_nRepeat > 1)
		{
			mlog("UnitTest Repeat Count = %d\n", unittest_config.m_nRepeat);
		}

		ZUnitTestRunner	unitTestRunner;
		if (unitTestRunner.RunStage1() == false) 
		{
			ret = ZUnitTestRunner::CCNET_UNITTEST_FAILED;
		}
	}


	return ret;
}



using namespace UnitTest;


int PASCAL wWinMain(HINSTANCE this_inst, HINSTANCE prev_inst, LPWSTR cmdline, int cmdshow)
{
	ZMinetInitScope MinetInitScope;
	ZScopeMempoolReleaser MemPoolReleaser;
	SProfileScope profileScope(FILENAME_PROFILE);

	Randomize();
	SAppHelper::SetupCurrentDirectory();	// Current Directory�� ����
	SAppHelper::InitLog(FILENAME_LOG);	
	ZConfig::Init_INI();

	return RunOnlyUnitTest("");
}

