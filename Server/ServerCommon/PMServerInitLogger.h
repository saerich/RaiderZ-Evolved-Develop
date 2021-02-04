#pragma once

enum SERVER_INIT_RESULT
{
	SERVERINIT_ERROR_UNKNOWN = 0,
	SERVERINIT_FAILED_DB_CONNECT,
	SERVERINIT_FAILED_NETWORK_INIT,
	SERVERINIT_FAILED_DEPENDENCYFILE_LOAD,
	SERVERINIT_FAILED_DATAFILE_LOAD,
	SERVERINIT_FAILED_RUN_AUTOTEST,
	SERVERINIT_FAILED_INIT_GAMEGUARD,
	SERVERINIT_FAILED_INIT_FIXED_PARTY,
	SERVERINIT_OK,
};

static struct INIT_RESULT_MSG
{
	DWORD nErrLevel;
	const wchar_t* pszWarningMsg;
	const wchar_t* pszTreatMsg;
} SERVER_INIT_RESULT_MSG[] =
{
	{ 0, L"0: Unknown problem",									L"Contact the developer" },
	{ 0, L"1: Could not connect to DB",							L"Contact the developer" },
	{ 0, L"2: Network initialization failure" ,					L"Check external network connection, check port conflict" },
	{ 0, L"3: Cannot read execution-dependent file",			L"Make sure the relevant files are in the server running folder" },
	{ 0, L"4: Unable to read data file"	,						L"Make sure the relevant files are in the server running folder" },
	{ 0, L"5: Can't run automatic test",						L"Contact the developer" },
	{ 0, L"6: GameGuard (nProtect) initialization failure",		L"Contact the developer" },
	{ 0, L"7: Fixed party initialization failure",				L"Contact an Expo representative"},
	{ 0, L"8: Normal operation",								L"_"},
};

class PmServerInitLogger
{
public:
	static void SetResult(SERVER_INIT_RESULT result);
	static SERVER_INIT_RESULT GetResult();
private:
	static SERVER_INIT_RESULT m_nResult;
};

static void SetServerInitResult(SERVER_INIT_RESULT result)
{
	PmServerInitLogger::SetResult(result);
}

static SERVER_INIT_RESULT GetServerInitResult()
{
	return PmServerInitLogger::GetResult();
}

static const wchar_t* GetServerInitWarningMsg()
{
	return SERVER_INIT_RESULT_MSG[GetServerInitResult()].pszWarningMsg;
}

static const wchar_t* GetServerInitTreatMsg()
{
	return SERVER_INIT_RESULT_MSG[GetServerInitResult()].pszTreatMsg;
}

static DWORD GetServerInitResultErrLevel()
{
	return SERVER_INIT_RESULT_MSG[GetServerInitResult()].nErrLevel;
}

static bool IsServerInitFailed()
{
	return GetServerInitResult() != SERVERINIT_OK;
}