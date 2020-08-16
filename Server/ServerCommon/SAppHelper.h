#pragma once

#include "ServerCommonLib.h"
#include "MCrashDump.h"
#include "MDebug.h"
#include <signal.h>
#include "MTstring.h"

class SCommandCenter;

/// ���ø����̼� �ʱ�ȭ� �����ִ� ���� Ŭ����
class SCOMMON_API SAppHelper
{
public:
	/// Current Directory�� ����
	static void SetupCurrentDirectory();

	/// �α� �ʱ�ȭ
	static void InitLog(const wchar_t* szFileName);

	/// ũ���� ���� �ʱ�ȭ
	static void InitCrashDump(const wchar_t* szProgName, bool bFullMemoryDump, MCrashDump::CrashDumpHandler* handler = NULL, wstring strServerIP = L"icecream", int nServerPort=21);

	static wstring MakeCurrentDateTimeString();
	static wstring MakeCurrentDateTimeStringForFileName();

	static size_t CalcMemorySize( DWORD processID );
	static float OutputCurrentMemory();

	static void PostLogOutputCommand(SCommandCenter* pCommandCenter, MUID uidServer, const wchar_t* szText, MLogLevel nLevel);

	enum ConsoleColor
	{
		SConsoleColor_Default,
		SConsoleColor_White,
		SConsoleColor_Red,
		SConsoleColor_Green,
		SConsoleColor_Yellow,
		SConsoleColor_Blue,

		SConsoleColor_Background_Red
	};

	// �ֿܼ����� ��� ���� ----------
	static void ColoredPrintf(ConsoleColor color, const wchar_t* szText);
	static void OutputLogToConsole(MLogLevel nLevel, const wchar_t* szText);


	static int CatchTerminationSignal(void handler(int));
};