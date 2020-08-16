#pragma once

#include "MCommand.h"
#include "STransData_M2G.h"

class ZPlayer;
class ZPlayerManager;
struct TD_PROXY_PLAYER_INFO;

/// �÷��̾� ���� Ŀ�ǵ� ������
class ZPlayerCommandRouter
{
public:
	static void SendAddPlayerResponse(const MUID& uidLoginServer, const MUID& uidPlayer, int nResult);
	
	static void SendDuplicatedPlayerLogin_ToLoginServer(const MUID& uidLoginServer, const MUID& uidPlayer, const MUID& uidConnectionKey=MUID::ZERO);
	static void SendDuplicatedPlayerLogin_ToGameServer(const MUID& uidGameServer, const MUID& uidPlayer, const MUID& uidConnectionKey=MUID::ZERO);

	static void SendPlayerStateInWorldRes(const MUID& uidGameServer, CID nCID, int nResult);

	static void RouteProxyPlayerAdd(const ZPlayer* pPlayer);
	static void RouteProxyPlayerRemove(CID cid);
	static void RouteProxyPlayerUpdate(const ZPlayer* pPlayer);
	static void RouteAllProxyPlayer(const MUID& uidTarget);

	// �Ǹ�
	static void SendPmangAddPlayerResponse(const MUID& uidLoginServer, const MUID& uidPlayer, int nResult);

private:
	static void MakeProxyPlayerInfo(TD_PROXY_PLAYER_INFO* pInfo, const ZPlayer* pPlayer);
};
