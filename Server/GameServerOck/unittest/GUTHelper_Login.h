#pragma once

#include "MTypes.h"

class GEntityPlayer;

/// �α��� ���� ���� Ŭ����
class GUTHelper_Login
{
public:
	static MCommand* NewCommand_MMC_COMM_REQUEST_READY_ENTER_MOVE_PLAYER( GEntityPlayer* pEntityPlayer, MUID uidConnectionKey, MUID uidGameServer, MUID uidNetClient );
};