#include "stdafx.h"
#include "ZCommandCenter.h"
#include "ZGlobal.h"
#include "ZGameServerObjectManager.h"
#include "ZLoginServerFacade.h"
#include "ZAppServerObjectManager.h"

ZCommandCenter::ZCommandCenter( minet::MNetServer* netserver )
: SCommandCenter(netserver)
{
	
}

void ZCommandCenter::AddReceiver(MCommand* pCommand, SCommObjectManager* pManager, MUID uidExceptServer)
{
	SCommObjectManager::SCommObjectMap& mapObject = pManager->GetObjects();

	for(SCommObjectManager::SCommObjectMap::iterator it = mapObject.begin(); it != mapObject.end(); it++)
	{
		if (uidExceptServer == it->first)
			continue;

		pCommand->AddReceiver(it->first);
	}
}

void ZCommandCenter::RouteToGameServer(MCommand* pCommand, MUID uidExceptServer)
{
	AddReceiver(pCommand, gmgr.pGameServerObjectManager, uidExceptServer);
	PostCommand(pCommand);
}

void ZCommandCenter::RouteToLoginServer(MCommand* pCommand)
{
	pCommand->SetReceiverUID(gmgr.pLoginServerFacade->GetUID());
	PostCommand(pCommand);
}

void ZCommandCenter::RouteToAppServer(MCommand* pCommand)
{
	AddReceiver(pCommand, gmgr.pAppServerObjectManager);
	PostCommand(pCommand);
}

void ZCommandCenter::RouteProxyInfo(MCommand* pCommand)
{
	// Proxy ������ �����ϴ� �������� UID ����
	AddReceiver(pCommand, gmgr.pGameServerObjectManager);
	AddReceiver(pCommand, gmgr.pAppServerObjectManager);

	// ����
	PostCommand(pCommand);
}