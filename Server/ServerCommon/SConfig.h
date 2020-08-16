#pragma once

#include "ServerCommonLib.h"
#include "MTstring.h"

#define CONFIG_TOKEN_APP_COMMAND_PROFILER	L"COMMAND_PROFILER"

// �� ������ ConfigŬ������ �θ� Ŭ����
class SCOMMON_API SConfig
{
protected:
	static void Init_INI_Common(const wchar_t* szFileName);
	static void InitNetworkCard(const wchar_t* szFileName);
	static bool GetPrivateProfileBool(const wchar_t* szAppName, const wchar_t* szKeyName, bool bDefault, const wchar_t* szFileName);	

public:
	static bool				IsExpoMode();
	static string			GetServerModeANSIString();

	// server info
	static int				m_nServerMode;			///< ���� ���. 0 - normal, 1 - Expo

	// log
	static std::wstring		m_strLogPath;			///< �α׸� ���� ���
	static int				m_nLogLevel;			///< �α� ���

	// command profiler
	static wstring	m_strCommandProfiler_LogFolder;			// Ŀ�ǵ� �������Ϸ� �αװ� ���� ����
	static wstring	m_strCommandProfiler_LogName;			// Ŀ�ǵ� �������Ϸ� �α� ���� �̸�
	static bool		m_bCommandProfiler_AutoStart;			// Ŀ�ǵ� �������Ϸ��� �ڵ����� ������ ������ ����
	static bool		m_bCommandProfiler_AutoSave;			// Ŀ�ǵ� �������Ϸ� �α� �ڵ����� ������ ����
	static int		m_nCommandProfiler_AutoSaveTickMinute;	// Ŀ�ǵ� �������Ϸ� �α� �ڵ����� ���� �� ƽ ��
	
	// debug
	static bool		m_isCrashDumpAutoAgree;			// ũ���� ���� ����Ʈ �ڵ�������
	static bool		m_iocpAppService;
};