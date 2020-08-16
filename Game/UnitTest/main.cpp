#include "stdafx.h"
#include "MCrashDump.h"
#include "XUnitTestRunner.h"
#include "XGameApp.h"
#include "XMemPoolScopeReleaser.h"
#include "MMemoryLeak.h"
#include "XUnitTestRunner.h"
#include "XUnitTestStage1.h"
#include "XUnitTestStage2.h"
#include "XCrashReporter.h"
#include <ShellAPI.h>
#include "XNetwork.h"
#include "XNetClient.h"
#include "XReplay.h"
#include "CSGoogleMockListener.h"

//#define VLD_CHECK_LEAK	// stdafx.h �� ������ ���������� ���� rebuild �ϰ� �Ǿ cpp ������ �����
#ifdef _DEBUG
#ifdef VLD_CHECK_LEAK

#define VLD_MAX_DATA_DUMP		0xFF		// vld���� ������ ������ FF Byte�� �ض�.

#include "../../../sdk/vld/vld.h"
#pragma comment(lib,"vldmt.lib")
#else
#endif
#endif // _DEBUG
// Leak üũ Define ----------------------------------------------


#pragma comment(lib, "../../../sdk/zlib/lib/zlib.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")


/// ũ���� �ڵ鷯
void XCrashDumpHandler(const char* szFileName)
{
	XCrashReporter::bAutoReport = true;
	XCrashReporter::Report(szFileName, "GameUnitTest");
}


bool PreAppRun(LPWSTR cmdline)
{
	USES_CONVERSION_EX;

	wchar_t szModuleFileName[_MAX_DIR] = {0,};
	GetModuleFileName(NULL, szModuleFileName, _MAX_DIR);

	// ���� Raiderz.exe�ε� ���ڰ� �ƹ� �͵� ������ ��ó�� �����ϰ� �����Ѵ�.
	string strPureFileName = MGetRemovePathName(W2A_EX(szModuleFileName, 0));
	if (!_stricmp(strPureFileName.c_str(), "raiderz.exe"))
	{
		if (wcslen(cmdline) <= 0) 
		{
			ShellExecute(0, NULL, FILENAME_LAUNCHER, L"", NULL, SW_NORMAL);
			return false;
		}
	}

	// Current Directory�� ����
	PathRemoveFileSpec(szModuleFileName);
	SetCurrentDirectory(szModuleFileName);


	return true;
}

void PostAppRun()
{
#ifdef M_CHECK_LEAK
	_ASSERT(MMemoryLeak::CheckMemory());
#endif
}

bool InitLogNConst()
{
	MLogLevel level = MLOG_LEVEL_INFORMATION;
#ifndef _PUBLISH
	level = MLOG_LEVEL_DEBUG;
#endif
	// �α� �ʱ�ȭ
	MInitLog(level, 
		MLOG_ALL, 
		NULL, 
		"mlog.txt", 
		"mlog.xml", 
		"http://maiet.net/~zelinde/mlog.xsl");

	//// ���ӿ��� ����ϴ� ������� �д´�.
	XCONST::Init();

#ifndef _DEBUG
	MCrashDump::Init(NULL, NULL, XCONST::FULL_DUMP, &XCrashDumpHandler);
#endif

	return true;
}

class XMinetInitScope
{
public:
	XMinetInitScope() 
	{
		minet::Init(SH_COMMAND_VERSION);
		XAddCommandTable();
	}
	~XMinetInitScope() 
	{
		minet::Fini();
	}
};

class XProfileScope
{
public:
	XProfileScope() 
	{
	}
	~XProfileScope() 
	{
		MSaveStatisticsProfile("profile_game.txt", true);
	}
};


int RunOnlyUnitTest(LPWSTR cmdline)
{
	int ret = XUnitTestRunner::CCNET_UNITTEST_SUCCESS;

	MUnitTestConfig unittest_config;
	unittest_config.Load();

	if (unittest_config.m_nRepeat > 1)
	{
		mlog("UnitTest Repeat Count = %d\n", unittest_config.m_nRepeat);
	}

	// stage1 --
	if (unittest_config.CheckStageFilter(UNITTEST_STAGE1_NAME))
	{
		XUnitTestRunner	unitTestRunner;
		if (unitTestRunner.RunStage1() == false) 
		{
			ret = XUnitTestRunner::CCNET_UNITTEST_FAILED;
		}
	}

	// stage2 --
	if (unittest_config.CheckStageFilter(UNITTEST_STAGE2_NAME))
	{
		XUnitTestRunner	unitTestRunner;
		if (unitTestRunner.RunStage2() == false) 
		{
			ret = XUnitTestRunner::CCNET_UNITTEST_FAILED;
		}
	}

	return ret;
}

void InitGoogleMock()
{
	int argc = 0;
	char** argv = NULL;
	::testing::GTEST_FLAG(throw_on_failure) = false;
	::testing::InitGoogleMock(&argc, argv);

	// GoogleMock ���� �޽���, UnitTest++ ����� ���
	CSGoogleMockListener::InitListener();
}

int PASCAL wWinMain(HINSTANCE this_inst, HINSTANCE prev_inst, LPWSTR cmdline, int cmdshow)
{
	InitGoogleMock();

	XMinetInitScope MinetInitScope;
	XProfileScope ProfileScope;
	XMemPoolScopeReleaser	MemPoolScopeReleaser;


	// Create file system
	if (PreAppRun(cmdline) == false) return 0;


	// Create file system
	global.filesys = MCreateFileSystem( MFILEACCESS_GENERIC, "Data");


	// Init LOG and constance defines
	InitLogNConst();

	return RunOnlyUnitTest(cmdline);
}
