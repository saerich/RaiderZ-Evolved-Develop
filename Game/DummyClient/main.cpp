#include "stdafx.h"
#include "MCrashDump.h"
#include "XGameApp.h"
#include "XMemPoolScopeReleaser.h"
#include "MMemoryLeak.h"
#include "XCrashReporter.h"
#include <ShellAPI.h>
#include "XNetwork.h"
#include "XNetClient.h"
#include "XReplay.h"
#include "MStatisticsProfiler.h"

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
	if (global.net && global.net->GetNetClient() && global.net->GetNetClient()->GetRecorder() && global.net->GetNetClient()->GetRecorder()->IsRecording())
	{
		global.net->GetNetClient()->GetRecorder()->Stop();
	}

	if (XCONST::AUTO_REPORT_CRASHDUMP)
	{
		XCrashReporter::bAutoReport = true;
	}

	XCrashReporter::LogInfo();
	XCrashReporter::Report(szFileName, "DummyClient");
}


bool PreAppRun(LPWSTR cmdline)
{
	USES_CONVERSION_EX;

	wchar_t szModuleFileName[_MAX_DIR] = {0,};
	GetModuleFileName(NULL, szModuleFileName, _MAX_DIR);

	// ���� SoulHunt.exe�ε� ���ڰ� �ƹ� �͵� ������ ��ó�� �����ϰ� �����Ѵ�.
	string strPureFileName = MGetRemovePathName(W2A_EX(szModuleFileName, 0));
	if (!_stricmp(strPureFileName.c_str(), "soulhunt.exe"))
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


int PASCAL wWinMain(HINSTANCE this_inst, HINSTANCE prev_inst, LPWSTR cmdline, int cmdshow)
{
	USES_CONVERSION_EX;

	XMinetInitScope MinetInitScope;
	XProfileScope ProfileScope;
	XMemPoolScopeReleaser	MemPoolScopeReleaser;

	if (PreAppRun(cmdline) == false) return 0;

	// Create file system
	global.filesys = MCreateFileSystem( MFILEACCESS_GENERIC, "EngineRes;Data");

	InitLogNConst();

	XGameApp* g_pApp = new XGameApp();
	g_pApp->Init(cmdline);

	int ret = g_pApp->Run();
	if (!ret)
		mlog_popuplasterror(W2A_EX(APPLICATION_NAME, 0));

	PostAppRun();

	delete g_pApp;

	return ret;
}



