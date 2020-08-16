#pragma once

#include "MTypes.h"
#include <string>

#include "SConfig.h"

/// �ΰ��� ������
class PConfig : public SConfig
{
private:
	static wstring GetPathString( const wchar_t* szFileName, const wchar_t* szKeyName );

public:
	static void Init_INI();

	// config
	static bool		m_bWindowVisible;			///< ����â�� ������ ����
	static int		m_nMyNetworkCardID;			///< Listen Socket���� ����� ��Ʈ��ũ ī�� ID
	static int		m_nPort;					///< ���� ��Ʈ

	// ServerInfo
	static wstring	m_strServerName;			///< �����̸�
	static uint8	m_nServerType;				///< ����Ÿ��
	static int		m_nMyServerID;				///< ����ID
	static int		m_nMyWorldID;				///< ����ID
	static int		m_SSN;						///< �Ǹ� ���� ID
	//static bool		m_bPmangDBEncrypt;
	static wstring	m_strOdbcDriver;			///< ODBC ����̹� �̸�
	static int		m_nSocketPoolSize;			///< ���� Ǯ ������
	static int		m_nSendPendingLimitCount;		///< ���� Ŀ��� �ҵ� ����

	// MasterServer
	static int		m_nMasterServer_NetworkCardID;	///< ������ ���� ��ſ� ����� ��Ʈ��ũ ī�� ID
	static wstring	m_strMasterServerIP;		///< ���� IP
	static int		m_nMasterServerPort;		///< ���� ��Ʈ

	// db
	static wstring	m_strGameDB_Server;				///< DB Server
	static wstring	m_strGameDB_DatabaseName;		///< DB Name
	static wstring	m_strGameDB_UserName;			///< DB Username
	static wstring	m_strGameDB_Password;			///< DB Password

	static wstring	m_strLogDB_Server;				///< DB Server
	static wstring	m_strLogDB_DatabaseName;		///< DB Name
	static wstring	m_strLogDB_UserName;			///< DB Username
	static wstring	m_strLogDB_Password;			///< DB Password

	// path
	static wstring	m_strSystemPath;			///< �ý��� ����

	// PMS
	//static bool		m_bPMSEnable;

	// etc
	static wstring			m_strDumpDestServerIP;
	static int				m_nDumpDestServerPort;


};
