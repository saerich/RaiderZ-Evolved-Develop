#ifndef _GCONFIG_H
#define _GCONFIG_H

#include "GFrameworkLib.h"
#include "MTypes.h"
#include "SConfig.h"
#include "GConfig_Arg.h"
#include "GConfig_Lua.h"

#define CONFIG_AUTO_TEST_AI			L"ai"
#define CONFIG_AUTO_TEST_QPVPEVENT	L"qpvpevent"
#define CONFIG_AUTO_TEST_AI_PROFILE	L"ai_profile"


/// �ΰ��� ������
class GFRAMEWORK_API GConfig : public SConfig
{
private:
	static void InitLogDB(const wchar_t* szFileName);
public:
	static void Init_INI();
	static void Init_Arg(const GConfig_Arg& arg);
	static void Init_Lua(const GConfig_Lua& lua);

	static bool IsStandAloneMode(void)		{ return m_bStandAlone || m_bAutoTestActive;	}

	// server info
	static int			m_nMaxUser;					///< �ִ� ������
	static std::wstring	m_strServerName;
	static uint8		m_nServerType;
	static int			m_SSN;						///< �Ǹ� ���� ID
	static int			m_nMyWorldID;				///< ���� ID
	static int			m_nMyServerID;				///< ���� ID
	static wstring		m_strOdbcDriver;			///< ODBC
	static wstring		m_strPublicIP;				///< �ܺ� ���� IP
	static int			m_nSocketPoolSize;			///< ���� Ǯ ������
	static int			m_nSendPendingLimitCount;	///< ���� Ŀ��� �ҵ� ����

	// db
	static std::wstring		m_strGameDB_Server;			///< DB Server
	static std::wstring		m_strGameDB_DatabaseName;	///< DB Name
	static std::wstring		m_strGameDB_UserName;		///< DB Username
	static std::wstring		m_strGameDB_Password;		///< DB Password

	// LogDB
	static std::wstring		m_strLogDB_Server;			///< LogDB Server.
	static std::wstring		m_strLogDB_DatabaseName;	///< LogDB Name.
	static std::wstring		m_strLogDB_UserName;		///< LogDB Username.
	static std::wstring		m_strLogDB_Password;		///< LogDB Password.

	// config
	static bool		m_bWindowVisible;				///< ����â�� ������ ����
	static int		m_nMyNetworkCardID;				///< Listen Socket���� ����� ��Ʈ��ũ ī�� ID
	static int		m_nPort;						///< ���� ��Ʈ
	static bool		m_bStandAlone;					///< ������ �������� �������� ����

	// heart beat
	static bool		m_bEnableHeartBeat;				///< HeartBeat üũ�� �� ������ ����
	static int		m_nHeartBeatTickTime;			///< HeartBeat üũ �ֱ�
	static int		m_nHeartBeatTimeout;			///< HeartBeat Timeout


	// network
	static int		m_nPrivateNetworkCardID;	///< ������ ���� ��ſ� ����� ��Ʈ��ũ ī�� ID
	static wstring	m_strMasterServerIP;
	static int		m_nMasterServerPort;

	// debug
	static wchar_t		m_szTestSuite[4][128];			///< �׽�Ʈ�� Suite, �ƹ��͵� ������ ��� �׽�Ʈ�Ѵ�.
	static DWORD	m_dwPacketDelayTime;			///< ��Ŷ�� ������ �޴µ� ���Ǵ� �ð� (�׽�Ʈ��)
	static bool		m_bDBTaskCount;
	static bool		m_bNoDB;
	static bool		m_bLogPoolCount;				///< �޸� Ǯ ���� �α׷� ������ ����

	static bool				m_bExportUpdateDB;
	static bool				m_bExportWriteFile;
	static wstring			m_strExportLocale;

	// autotest
	static bool				m_bAutoTestActive;
	static wstring			m_strAutoTestType;
	static GAITestType		m_nAITestType;
	static GCommandRecordingType m_nCommandRecordingType;
	static int				m_nRepeat;
	static DWORD			m_dwTime;	
	static int				m_nSpeed;
	static float			m_fLimitFrame;
	static int				m_nNPCCount;
	static int				m_nFieldID;
	static int				m_nQPVPEventID;

	// ���Ӽ��� �������Ϸ�
	static wstring			m_strAutoTestFieldlistFileName;

	// ���� ����ð�
	static bool				m_bLimitRunTime;
	static DWORD			m_dwRunTime;

	// PMS
	static bool				m_bPMSEnable;

	// GameGuard (���� ����: CS_Auth26.pdf)
	static bool				m_bGGEnable;		
	static bool				m_bGGShowDebugLog; // (InitGameGuardAuth ���)
	static int				m_nGGInitNumActive; // CS���� ����� ������ �ּ� �����ڼ� (InitGameGuardAuth ���)
	static int				m_nGGTimeLimit;		// ������Ʈ ���� �� ��赥���͸� �����ϴ� �ð� (SetUpdateCondition ���)
	static int				m_nGGCondition;		// ������Ʈ ���� �� ���� �뵥���� �� ������ ���� ���� (SetUpdateCondition ���)
	static int				m_nGGAuthInterval;	// ���� �ֱ�ð� (����: ��) 


	// pmang db encrypt.
	static bool				m_bPmangDBEncrypt;

	// etc
	static bool				m_bFullMemoryCrashDump;
	static bool				m_bDumpEnable;
	static wstring			m_strDumpDestServerIP;
	static int				m_nDumpDestServerPort;

	// command profiler
	static tstring	m_strCommandProfiler_LogName_ForMasterServer;	// Ŀ�ǵ� �������Ϸ� �α� ���� �̸� - �����ͼ���
	static tstring	m_strCommandProfiler_LogName_ForAppServer;		// Ŀ�ǵ� �������Ϸ� �α� ���� �̸� - �ۼ���

	// ����
	static bool				m_moveSectorWithoutCopySectorVector;
};





#endif