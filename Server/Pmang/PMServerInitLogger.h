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
	{ 0, L"0: �� �� ���� ����",					L"�����ڿ��� ����" },
	{ 0, L"1: DB�� ������ �� ����",				L"�����ڿ��� ����" },
	{ 0, L"2: ��Ʈ��ũ �ʱ�ȭ ����" ,			L"�ܺ� ��Ʈ��ũ ���� Ȯ��, ��Ʈ �浹 Ȯ��" },
	{ 0, L"3: �������� ������ �о�� �� ����",	L"���� ���� ������ ���� ������ ����� �ִ��� Ȯ��" },
	{ 0, L"4: ������ ������ �о�� �� ����"	,	L"���� ���� ������ ���� ������ ����� �ִ��� Ȯ��" },
	{ 0, L"5: �ڵ��׽�Ʈ�� ������ �� ����",		L"�����ڿ��� ����" },
	{ 0, L"6: ���Ӱ���(nProtect) �ʱ�ȭ ����",  L"�����ڿ��� ����" },
	{ 0, L"7: ������Ƽ �ʱ�ȭ ����",			L"Expo ����ڿ��� ����"},
	{ 0, L"8: ���� �۵���",						L"_"},
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