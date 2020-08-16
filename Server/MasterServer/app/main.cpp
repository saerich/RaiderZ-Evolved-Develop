#include "stdafx.h"
#include "resource.h"
#include <shlwapi.h>
#include "ZMemory.h"
#include "ZCommandTable.h"
#include "ZApplication.h"
#include "ZConfig.h"
#include "MProfiler.h"
#include "SMinetInitScope.h"
#include "SProfileScope.h"
#include "SAppHelper.h"
#include "SLogger.h"
#include "ZDef.h"
#include "MFileSystem.h"


#define PROG_RET_SUCCESS			0
#define PROG_RET_FAILED				1

ZApplication* g_pGameServerApp = NULL;

void InitCrashDump()
{
	SAppHelper::InitCrashDump(APP_NAME, false, NULL, ZConfig::m_strDumpDestServerIP, ZConfig::m_nDumpDestServerPort);
}

void PreApp()
{
	Randomize();
	SAppHelper::SetupCurrentDirectory();	// Current Directory�� ����
	ZConfig::Init_INI();
	SLogger::Init(MLogLevel(ZConfig::m_nLogLevel), ZConfig::m_strLogPath);
	InitCrashDump();
}

int PASCAL wWinMain(HINSTANCE this_inst, HINSTANCE prev_inst, LPWSTR cmdline, int cmdshow)
{
	// File system �ʱ�ȭ
	MCreateFileSystem( MFILEACCESS_GENERIC);

	ZMinetInitScope MinetInitScope;
	ZScopeMempoolReleaser MemPoolReleaser;
	SProfileScope profileScope(FILENAME_PROFILE);

	PreApp();

	// ���� ���� ----------------------------------
	g_pGameServerApp = new ZApplication(true);
	if (g_pGameServerApp->Create(0, 
		0, 
		APPLICATION_WIDTH, 
		APPLICATION_HEIGHT, 
		APP_CLASS_NAME, 
		APP_NAME,
		IDR_SYSTEM_MENU,
		IDR_MAINFRAME))
	{
		int ret = g_pGameServerApp->Run();
		delete g_pGameServerApp;
		return ret;
	}

	delete g_pGameServerApp;

	return PROG_RET_SUCCESS;
}

///////////////////////////////////////////////////////////////////


