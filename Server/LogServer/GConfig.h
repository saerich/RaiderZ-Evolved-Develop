#ifndef _GCONFIG_H
#define _GCONFIG_H

#include "MTypes.h"

#define CONFIG_AUTO_TEST_AI	"ai"



/// �ΰ��� ������
class GConfig
{
private:
	static bool GetPrivateProfileBool(const char* szAppName, const char* szKeyName, bool bDefault, const char* szFileName);
public:
	static void Init_INI();

	// server info
	static int		m_nMaxUser;					///< �ִ� ������

	// db
	static char		m_szDB_DNS[128];				///< DB DNS
	static char		m_szDB_UserName[128];		///< DB Username
	static char		m_szDB_Password[128];		///< DB Password

	// config
	static bool		m_bWindowVisible;					///< ����â�� ������ ����
	static int		m_nPort;							///< ���� ��Ʈ
};





#endif