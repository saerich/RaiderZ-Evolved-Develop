#pragma once

#include "ServerCommonLib.h"

// ServerCommon : 18001 ~ 17000 (SCommandTable)
enum SCommandTable
{	
	MSC_PROXY_PLAYER_ADD		= 18001,		///< �÷��̾� ���� �߰�
	MSC_PROXY_PLAYER_REMOVE		= 18002,		///< �÷��̾� ���� ����
	MSC_PROXY_PLAYER_UPDATE		= 18003,		///< �÷��̾� ���� ����
	MSC_PROXY_ALL_PLAYER_INFO	= 18004,		///< ��� �÷��̾� ����

	MSC_ADMIN_IS_SERVABLE		= 18101,		///< ���� ������ �������� ���θ� ������ ������ �˸�
};

SCOMMON_API void AddServerCommonCommandTable();
