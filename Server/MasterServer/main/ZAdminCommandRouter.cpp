#include "StdAfx.h"
#include "ZAdminCommandRouter.h"
#include "ZCommandCenter.h"
#include "CCommandTable_Master.h"


void ZAdminCommandRouter::RouteChangeServerMode(int nServerMode)
{
	/// GameServer �� �����
	minet::MCommand* pCmdToGameServer = gsys.pCommandCenter->MakeNewCommand(MMC_ADMIN_CHANGE_SERVER_MODE
		, 1
		, NEW_INT(nServerMode));

	gsys.pCommandCenter->RouteToGameServer(pCmdToGameServer);

	/// LoginServer �� �����
	minet::MCommand* pCmdToLoginServer = gsys.pCommandCenter->MakeNewCommand(MLC_ADMIN_CHANGE_SERVER_MODE
		, 1
		, NEW_INT(nServerMode));

	gsys.pCommandCenter->RouteToLoginServer(pCmdToLoginServer);
}
