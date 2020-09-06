#include "stdafx.h"
#include "CCommandTable.h"
#include "XCommandTable.h"
#include "MCrashDump.h"
#include "RDeviceD3D.h"
#include "MFileSystem.h"
#include "XGameApp.h"
#include "XGlobal.h"
#include "XMemPoolScopeReleaser.h"
#include "MMemoryLeak.h"
#include "XCrashReporter.h"
#include <ShellAPI.h>
#include "XNetwork.h"
#include "XNetClient.h"
#include "XReplay.h"
#include "XSystemInfoUtil.h"
#include "XAntiHack.h"
#include "XPath.h"
#include "XResourceLoader.h"
#include "MLocale.h"
#include "XAppErrorCollector.h"
#include "REngine.h"


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
#ifndef _PUBLISH
	if (global.net && global.net->GetNetClient() && global.net->GetNetClient()->GetRecorder() && global.net->GetNetClient()->GetRecorder()->IsRecording())
	{
		global.net->GetNetClient()->GetRecorder()->Stop();
	}
#endif

	if (XCONST::AUTO_REPORT_CRASHDUMP)
	{
		XCrashReporter::bAutoReport = true;
	}

	XCrashReporter::LogInfo();
	XCrashReporter::Report(szFileName, "game");
}


bool PreAppRun(LPWSTR cmdline)
{
	wchar_t szModuleFileName[_MAX_DIR] = {0,};
	GetModuleFileName(NULL, szModuleFileName, _MAX_DIR);

	// ���� Raiderz.exe�ε� ���ڰ� �ƹ� �͵� ������ ��ó�� �����ϰ� �����Ѵ�.
	wstring strPureFileName = MLocale::ConvAnsiToUTF16(MGetRemovePathName(MLocale::ConvUTF16ToAnsi(szModuleFileName)).c_str());

	if (!_wcsicmp(strPureFileName.c_str(), L"raiderz.exe"))
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
	//// ���ӿ��� ����ϴ� ������� �д´�.
	XCONST::Init();

	MLogLevel nLogLevel = MLogLevel::MLOG_LEVEL_DEBUG;
	MLogOutput nLogOutput = MLOG_FILED;

#ifdef _PUBLISH
	nLogLevel = MLogLevel::MLOG_LEVEL_INFORMATION;
	nLogOutput = MLOG_FILE;
#else

#ifndef _DEBUG
	if (XCONST::DEBUG_LOG == false)
	{
		nLogLevel = MLogLevel::MLOG_LEVEL_INFORMATION;
	}
#endif


#endif

	// �α� �ʱ�ȭ
	MInitLog(nLogLevel, 
		nLogOutput, 
		NULL, 
		"mlog.txt",
		"mlog.xml",
		MLocale::ConvUTF16ToAnsi(XPath::GetMyDocumentsRaiderzSubPath(PATH_MYDOC_SAVE).c_str()).c_str());

#ifndef _DEBUG
	MCrashDump::Init(NULL, NULL, XCONST::FULL_DUMP, &XCrashDumpHandler);
#endif

	return true;
}

bool InitLogForFileSystemError()
{
	MLogLevel nLogLevel = MLogLevel::MLOG_LEVEL_DEBUG;
	MLogOutput nLogOutput = MLOG_FILED;

#ifdef _PUBLISH
	nLogLevel = MLogLevel::MLOG_LEVEL_INFORMATION;
	nLogOutput = MLOG_FILE;
#endif

	// �α� �ʱ�ȭ
	MInitLog(nLogLevel, 
		nLogOutput, 
		NULL, 
		"mlog.txt",
		"mlog.xml",
		MLocale::ConvUTF16ToAnsi(XPath::GetMyDocumentsRaiderzSubPath(PATH_MYDOC_SAVE).c_str()).c_str());

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
#ifdef _PUBLISH
		return;
#endif

		MSaveStatisticsProfile("profile_game.txt", true);
	}
};


void CallbackDetectedInvalidFile()
{
	XGetAppErrorCollector()->SetLastError( ERRORTYPE_SYSTEM, L"An invalid file detected. Please check for anti-virus program.");
	XGetAppErrorCollector()->ExitApplication( 300000);
}


bool InitFileSystem() 
{
#ifdef _PUBLISH

	global.filesys = MCreateFileSystem( MFILEACCESS_ENCRYPTED);
	global.filesys->SetDetectInvalidFileCallback( CallbackDetectedInvalidFile);
	global.filesys->ReadFileIndex( "fileindex.msf");

#else

	// ���� ���� READ_MRS.sys ������ ������ MRS ���� �б� ���� ����
	FILE* pFile;
	fopen_s( &pFile, "READ_MRS.sys", "r");
	if ( pFile != NULL)
	{
		fclose( pFile);

		global.filesys = MCreateFileSystem( MFILEACCESS_ENCRYPTED);
		global.filesys->SetDetectInvalidFileCallback( CallbackDetectedInvalidFile);
		global.filesys->ReadFileIndex( "fileindex.msf");
	}
	else
	{
		if( rs3::REngine::IsDevelopVersion() )
			global.filesys = MCreateFileSystem( MFILEACCESS_GENERIC, "EngineResDev;Data");
		else
			global.filesys = MCreateFileSystem( MFILEACCESS_GENERIC, "EngineRes;Data");
	}

#endif

	return true;
}

int PASCAL wWinMain(HINSTANCE this_inst, HINSTANCE prev_inst, LPWSTR cmdline, int cmdshow)
{
	// Create anti-hack module
	if ( XGetAntiHack()->CreateModule() == false)
		return 0;


#ifdef _PUBLISH
	// ������ XP ���� ������ ���� �ȵ�.
	OSVERSIONINFOEX osvi;
	ZeroMemory( &osvi, sizeof( OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX);
	if ( GetVersionEx( (OSVERSIONINFO*)&osvi) == FALSE)
	{
		osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO);
		if ( GetVersionEx( (OSVERSIONINFO*)&osvi) == FALSE)
		{
			XGetAppErrorCollector()->SetLastError( ERRORTYPE_SYSTEM, L"���� ���� ���� ������ ������ Ȯ���� �� ���� ������ ���α׷��� ������ �� �����ϴ�. ������ ���ĵ�� �˼��մϴ�.");
			return 0;
		}
	}

	if ( osvi.dwPlatformId != VER_PLATFORM_WIN32_NT  ||  osvi.dwMajorVersion < 5  ||  ( osvi.dwMajorVersion == 5  &&  osvi.dwMinorVersion == 0))
	{
		XGetAppErrorCollector()->SetLastError( ERRORTYPE_SYSTEM, L"�� ���α׷��� Windows XP �̻󿡼��� ���డ���մϴ�. ������ ���ĵ�� �˼��մϴ�.");
		return 0;
	}
#endif


#ifdef _PUBLISH
	// Publish ��忡�� �Ķ���Ͱ� �������� �ʴ´ٸ� �����Ѵ�
	if ( cmdline == NULL  ||  cmdline[ 0] == 0)
	{
		XGetAppErrorCollector()->SetLastError( ERRORTYPE_APPLICATION, L"No command line parameters.");
		XGetAntiHack()->RemoveModule();
		return 0;
	}
#endif


	XMinetInitScope MinetInitScope;
	XProfileScope ProfileScope;
	XMemPoolScopeReleaser	MemPoolScopeReleaser;


	// Pre-run application 
	if ( PreAppRun( cmdline) == false)
	{
		XGetAntiHack()->RemoveModule();
		return 0;
	}


	// Create file system
	if ( InitFileSystem() == false)
	{
		InitLogForFileSystemError();
		mlog("InitFileSystem Failed\n");

		XGetAppErrorCollector()->SetLastError( ERRORTYPE_APPLICATION, L"Can't create file system.\n\nMay not enough system resource.");
		XGetAntiHack()->RemoveModule();
		return 0;
	}


	// Init LOG and constance defines
	InitLogNConst();


	// System Info Log
	XSystemInfoUtil systemInfoUtil;
	systemInfoUtil.SetLogSystemInfo();

	// Create application
	XGameApp* g_pApp = new XGameApp();
	g_pApp->Init(cmdline);

	int ret = g_pApp->Run();
	if (!ret)
		mlog_popuplasterror(MLocale::ConvUTF16ToAnsi(APPLICATION_NAME).c_str());

	PostAppRun();

	delete g_pApp;


	// Remove anti-hack module
	XGetAntiHack()->RemoveModule();

	return ret;
}



