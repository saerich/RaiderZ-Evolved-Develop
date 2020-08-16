#include "stdafx.h"
#include "MCrashDump.h"

typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(
	HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
	);

MINIDUMP_TYPE MCrashDump::m_nDumpType = MiniDumpNormal;
char MCrashDump::m_szFileName[128] = "";
string MCrashDump::m_strUserData;
MCrashDump::CrashDumpHandler* MCrashDump::m_pHandlerFunc = NULL;

char* MCrashDump::GetFaultReason(_EXCEPTION_POINTERS* pExPtrs)
{
	if( NULL == pExPtrs || NULL == pExPtrs->ExceptionRecord || NULL ==  pExPtrs->ExceptionRecord->ExceptionCode )
		return "Unknown";

	if (::IsBadReadPtr(pExPtrs, sizeof(EXCEPTION_POINTERS))) 
		return "BAD EXCEPTION POINTERS";

	// ������ ���� �ڵ��� �׳� ��ȯ�� �� �ִ�.
	//http://msdn.microsoft.com/library/default.asp?url=/library/en-us/debug/base/exception_record_str.asp
	switch( pExPtrs->ExceptionRecord->ExceptionCode )
	{
	case EXCEPTION_ACCESS_VIOLATION:         return "EXCEPTION_ACCESS_VIOLATION";		// �߸��� ����
	case EXCEPTION_DATATYPE_MISALIGNMENT:    return "EXCEPTION_DATATYPE_MISALIGNMENT";  // ���ĵ������� ����Ÿ����
	case EXCEPTION_BREAKPOINT:               return "EXCEPTION_BREAKPOINT";				// �극��ũ����Ʈ�� �ɷ�����
	case EXCEPTION_SINGLE_STEP:              return "EXCEPTION_SINGLE_STEP";			// �ϳ��� ��ɾ �����Ҷ�
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";	// �迭�� �Ѿ�����
	case EXCEPTION_FLT_DENORMAL_OPERAND:     return "EXCEPTION_FLT_DENORMAL_OPERAND";	// ����ȭ�� �����ʴ� ������
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return "EXCEPTION_FLT_DIVIDE_BY_ZERO";		// float 0���γ���
	case EXCEPTION_FLT_INEXACT_RESULT:       return "EXCEPTION_FLT_INEXACT_RESULT";		// �Ҽ��� ��Ȯ�ϰ� ��Ÿ��������
	case EXCEPTION_FLT_INVALID_OPERATION:    return "EXCEPTION_FLT_INVALID_OPERATION";	// ��Ÿ����
	case EXCEPTION_FLT_OVERFLOW:             return "EXCEPTION_FLT_OVERFLOW";			// float�����÷ο�
	case EXCEPTION_FLT_STACK_CHECK:          return "EXCEPTION_FLT_STACK_CHECK";		// ���ÿ����� ��ġ�ų� �ʹ��������
	case EXCEPTION_FLT_UNDERFLOW:            return "EXCEPTION_FLT_UNDERFLOW";			// ���� ������
	case EXCEPTION_INT_DIVIDE_BY_ZERO:       return "EXCEPTION_INT_DIVIDE_BY_ZERO";		// int�� 0���� ����
	case EXCEPTION_INT_OVERFLOW:             return "EXCEPTION_INT_OVERFLOW";			// int�����÷ο�
	case EXCEPTION_PRIV_INSTRUCTION:         return "EXCEPTION_PRIV_INSTRUCTION";		// ��ɾ ���� �� �� ������
	case EXCEPTION_IN_PAGE_ERROR:            return "EXCEPTION_IN_PAGE_ERROR";			// page�� �ε� �� �� ���µ� �����Ѱ��
	case EXCEPTION_ILLEGAL_INSTRUCTION:      return "EXCEPTION_ILLEGAL_INSTRUCTION";	// �߸��� ��ɾ�
	case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";// �����Ҽ� ���µ� ��� �����ų ���
	case EXCEPTION_STACK_OVERFLOW:           return "EXCEPTION_STACK_OVERFLOW";			// ���ÿ����÷ο�
	case EXCEPTION_INVALID_DISPOSITION:      return "EXCEPTION_INVALID_DISPOSITION";	// �߸��ȱ��(?)���� C������ �߻����� ����
	case EXCEPTION_GUARD_PAGE:               return "EXCEPTION_GUARD_PAGE";				// �����������ϴ� page�� ����
	case EXCEPTION_INVALID_HANDLE:           return "EXCEPTION_INVALID_HANDLE";			// �߸��� �ڵ�
	case 0xE06D7363:                         return "Microsoft C++ Exception";			// Visual C++ ����
	default:
		break;
	}

	return "Unknown";
}

void MCrashDump::GetDefaultDumpFileName( char pszFileName[] )
{
	char szDumpFileName[MAX_PATH];
	char szFilename[MAX_PATH];
	::GetModuleFileName(NULL, szFilename, MAX_PATH);
	char* szdot = strrchr(szFilename, '.');
	::lstrcpyn(szDumpFileName, szFilename, (int)(szdot - szFilename + 1));

	SYSTEMTIME time;
	::GetLocalTime(&time);
	sprintf( pszFileName, "%d%02d%02d_%02d%02d%02d.dmp", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
}

LONG WINAPI MCrashDump::TopLevelExceptionFilter(_EXCEPTION_POINTERS *pException)
{
	char szDumpFileName[MAX_PATH];
	if (m_szFileName[0] == 0)
	{
		GetDefaultDumpFileName(szDumpFileName);
	}
	else
	{
		strcpy_s(szDumpFileName, m_szFileName);
	}

	if (!BuildCrashDump(szDumpFileName, pException))
		return EXCEPTION_CONTINUE_SEARCH;

	if ( m_pHandlerFunc )
		m_pHandlerFunc(szDumpFileName);

	return EXCEPTION_EXECUTE_HANDLER;
}


bool MCrashDump::Init(const char* szFileName, const char* szUserData, bool bFullMemoryDump, CrashDumpHandler* pFunction)
{
	::SetUnhandledExceptionFilter(TopLevelExceptionFilter);

	if (szFileName!=NULL)
	{
		strcpy_s(m_szFileName, szFileName);
	}
	if (szUserData!=NULL)
	{
		m_strUserData = szUserData;
	}

	m_nDumpType = MiniDumpNormal;
	if (bFullMemoryDump)
	{
		m_nDumpType = MiniDumpWithFullMemory;
	}

	if ( pFunction )
		m_pHandlerFunc = pFunction;

	return true;
}

bool MCrashDump::BuildCrashDump( const char* pszDumpFileName, _EXCEPTION_POINTERS* pException )
{
	char szDllHelpPath[MAX_PATH] = "";
	HMODULE hDLL = NULL;

	// �⺻ ������ ���� �̸� ����
	char szDumpFileName[MAX_PATH];
	if (NULL == pszDumpFileName)
	{
		GetDefaultDumpFileName(szDumpFileName);
		pszDumpFileName = szDumpFileName;
	}

	// DLL �ε�
	if(::GetModuleFileName(NULL, szDllHelpPath, _MAX_PATH))
	{
		char *pSlash = ::strrchr(szDllHelpPath, '\\');
		if( pSlash )
		{
			::lstrcpy(pSlash + 1, "DBGHELP.DLL");
			hDLL = ::LoadLibrary(szDllHelpPath);
		}
	}

	if( NULL == hDLL || INVALID_HANDLE_VALUE == hDLL )
	{
		MessageBox(NULL, "cannot open file DBGHELP.DLL", "ERROR", MB_OK);
		return false;
	}

	// DLL���� MiniDumpWriteDump�Լ� ������
	MINIDUMPWRITEDUMP pMiniDumpWriteDump = (MINIDUMPWRITEDUMP)::GetProcAddress(hDLL, "MiniDumpWriteDump");

	if( pMiniDumpWriteDump == NULL )
	{
		return false;
	}

	// ���� ����
	HANDLE hFile = CreateFile(
		pszDumpFileName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if( INVALID_HANDLE_VALUE == hFile )	
	{
		return false;
	}

	MINIDUMP_EXCEPTION_INFORMATION eInfo;
	eInfo.ThreadId = GetCurrentThreadId();
	eInfo.ExceptionPointers = pException;
	eInfo.ClientPointers = FALSE;

	// ���� //
	// ����� �ͼ����� �ڵ鸵�ϴ� �κ��̹Ƿ� �޸� ħ�� �ͼ��� ������쿡 
	// �̰����� �޸� �����Ҵ��� �ϸ� �ι�° �ͼ����� ���鼭 ����ɼ� �����Ƿ� 
	// �޸� �����Ҵ��� ���ؾ� �մϴ�

	const char*	szFaultReason;
	szFaultReason = pException ? GetFaultReason(pException):"UserDump";

	MINIDUMP_USER_STREAM_INFORMATION sUserInfo; 
	MINIDUMP_USER_STREAM m_vUseData[2];

	// �߰�����
	m_vUseData[0].Type = 0;
	m_vUseData[0].Buffer = (PVOID)szFaultReason;
	m_vUseData[0].BufferSize = strlen(szFaultReason);

	m_vUseData[1].Type = 1;
	m_vUseData[1].Buffer = (PVOID)m_strUserData.c_str();
	m_vUseData[1].BufferSize = m_strUserData.size();

	sUserInfo.UserStreamCount = 2; 
	sUserInfo.UserStreamArray = m_vUseData;

	bool bDumpped = false;
	if (pMiniDumpWriteDump(
		GetCurrentProcess(),
		GetCurrentProcessId(),
		hFile,
		m_nDumpType,
		pException ? &eInfo : NULL,
		&sUserInfo,
		NULL) )
	{
		bDumpped = true;
	}

	CloseHandle(hFile);
	FreeLibrary(hDLL);

	return bDumpped;
}

