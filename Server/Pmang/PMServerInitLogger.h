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
	{ 0, L"0: 알 수 없는 문제",					L"개발자에게 문의" },
	{ 0, L"1: DB에 연결할 수 없음",				L"개발자에게 문의" },
	{ 0, L"2: 네트워크 초기화 실패" ,			L"외부 네트워크 연결 확인, 포트 충돌 확인" },
	{ 0, L"3: 실행의존 파일을 읽어올 수 없음",	L"서버 실행 폴더에 관련 파일이 제대로 있는지 확인" },
	{ 0, L"4: 데이터 파일을 읽어올 수 없음"	,	L"서버 실행 폴더에 관련 파일이 제대로 있는지 확인" },
	{ 0, L"5: 자동테스트를 실행할 수 없음",		L"개발자에게 문의" },
	{ 0, L"6: 게임가드(nProtect) 초기화 실패",  L"개발자에게 문의" },
	{ 0, L"7: 고정파티 초기화 실패",			L"Expo 담당자에게 문의"},
	{ 0, L"8: 정상 작동됨",						L"_"},
};



class PmServerInitLogger
{
public:
	static void SetResult(SERVER_INIT_RESULT result);
	static SERVER_INIT_RESULT GetResult();
private:
	static SERVER_INIT_RESULT m_nResult;
};


static void SetServerInitResult( SERVER_INIT_RESULT result )
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