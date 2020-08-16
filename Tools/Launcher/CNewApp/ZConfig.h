#pragma once

class ZConfig
{
public:
	/// CONFIG
	static bool		m_bHideWebPage;
	static string	m_strWebPageURL;

	/// LAUNCH
	static bool		m_bStaticLaunch;
	static string	m_strStaticServerName;		///< ServerName
	static string	m_strStaticIPAddress;		///< IP Address
	static bool		m_bSkipSelectWorldForPWE;	///< Skip Select World For PWE

	///	PATCH
	static bool		m_bSkipUpdate;			///< ������Ʈ�� �ǳ� �ٴ��� Ȯ��
	static string	m_strPatchServAddr;		///< ��ġ ���� �ּ�
	static string	m_strPatchServDir;		///< ��ġ ���� ���� ���͸�


	static bool		Init();

};
