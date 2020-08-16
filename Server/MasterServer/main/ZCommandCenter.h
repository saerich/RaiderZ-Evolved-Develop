#pragma once

#include "MCommand.h"
#include "ZNetServer.h"
#include "SCommandCenter.h"

class SCommObjectManager;

/// Ŀ�ǵ� �帧�� �����ϴ� Ŭ����
class ZCommandCenter : public SCommandCenter
{
public:
	ZCommandCenter(minet::MNetServer* netserver);
	virtual ~ZCommandCenter() {}

	void AddReceiver(MCommand* pCommand, SCommObjectManager* pManager, MUID uidExceptServer=MUID::ZERO);
	void RouteToGameServer(MCommand* pCommand, MUID uidExceptServer=MUID::ZERO);
	void RouteToLoginServer(MCommand* pCommand);
	void RouteToAppServer(MCommand* pCommand);
	void RouteProxyInfo(MCommand* pCommand);
};
