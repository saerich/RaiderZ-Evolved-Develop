#pragma once

#include "MTypes.h"
#include <string>
#include "SConfig.h"

#define CONFIG_AUTO_TEST_AI	"ai"


struct LDBConfig
{
	std::wstring strServer;
	std::wstring strDBName;
	std::wstring strUserName;
	std::wstring strPassword;
};


/// �ΰ��� ������
class LConfig : public SConfig
{
private:
	static wstring GetPathString(const wchar_t* szFileName, const wchar_t* szKeyName);

	static void InitAccountDB(const wchar_t* szFileName);
	static void InitGameDB(const wchar_t* szFileName);
	static void InitLogDB(const wchar_t* szFileName);


public:
	static void Init_INI();

	// db
	static LDBConfig		m_AccountDBConfig;
	static LDBConfig		m_GameDBConfig;
	static LDBConfig		m_LogDBConfig;

	// config
	static int				m_nMyNetworkCardID;				///< Listen Socket���� ����� ��Ʈ��ũ ī�� ID
	static int				m_nPort;						///< ���� ��Ʈ
	static int				m_nUDPPort;						///< UDP ��Ʈ
	static bool				m_bStandAlone;					///< �����ͼ��� ���� �������� ����

	// path
	static std::wstring		m_strSystemPath;				///< �ý��� ����
	static std::wstring		m_strFieldPath;					///< Field ����
	static std::wstring		m_strZonePath;					///< Zone ����

	// server info
	static std::wstring		m_strServerName;				///< ���� �̸�	
	static uint8			m_nServerType;
	static int				m_nServerID;					///< ���� ID
	static int				m_nWorldID;						///< ���� ID
	static int				m_SSN;							///< �Ǹ� ���� ID
	static wstring			m_strOdbcDriver;				///< ODBC
	static int				m_nSocketPoolSize;				///< ���� Ǯ ������
	static int				m_nSendPendingLimitCount;		///< ���� Ŀ��� �ҵ� ����

	// master server
	static int				m_nMasterServer_NetworkCardID;	///< ������ ���� ��ſ� ����� ��Ʈ��ũ ī�� ID
	static wstring			m_strMasterServerIP;			///< ������ ���� IP
	static int				m_nMasterServerPort;			///< ������ ���� Port	

	// const
	static bool				m_bWindowVisible;				///< ����â�� ������ ����
	static int				m_nMoveServerTimeout;			///< �����̵� �������� Ÿ�Ӿƿ�(ms)
	static int				m_nCommandTimeout;				///< Ŀ�ǵ� ���� Ÿ�Ӿƿ�
	static bool				m_isAllowInsertNewAccount;		///< ���ο� ������ ��� ������

	// Pmang
	static wstring			m_strPmangServerKey;				///< �Ǹ� ����Ű
	static uint32			m_nPmangToleranceSeconds;		///< �Ǹ� �α��� �ð� ������

	// PMS
	//static bool				m_bPMSEnable;

	// pmang db encrypt.
	//static bool				m_bPmangDBEncrypt;

	// etc
	static wstring			m_strDumpDestServerIP;
	static int				m_nDumpDestServerPort;
};
