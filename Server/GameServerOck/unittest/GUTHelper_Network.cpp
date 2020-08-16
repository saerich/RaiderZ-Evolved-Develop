#include "StdAfx.h"
#include "GUTHelper_Network.h"
#include "GEntityPlayer.h"
#include "MockNetServer.h"
#include "GCommandCenter.h"
#include "MockPlayerObject.h"
#include "MockLink.h"
#include "GPlayerObjectManager.h"

MockLink* GUTHelper_Network::NewLink( GEntityPlayer* pPlayer, bool bIgnoreAllSendCommandID )
{
	MUID uid = pPlayer->GetUID();

	MockNetServer* pMockNetServer = dynamic_cast<MockNetServer*>(gsys.pCommandCenter->GetNetServer());
	if (!pMockNetServer) return NULL;

	MockLink* pNewMockLink = dynamic_cast<MockLink*>(pMockNetServer->AddMockLink(uid));
	if (!pNewMockLink) return NULL;

	if (bIgnoreAllSendCommandID)
	{
		pNewMockLink->IgnoreAll();
	}

	// ���࿡ PlayerObject�� ������ ���Ǹ� ���� PlayerObject�� �Բ� ��������.
	if (gmgr.pPlayerObjectManager->GetPlayer(uid) == NULL)
	{
		MockPlayerObject* pMockPlayerObject = new MockPlayerObject(uid);
		gmgr.pPlayerObjectManager->AddPlayer(pMockPlayerObject);
		pMockPlayerObject->Create(pPlayer);

		pNewMockLink->AttachMockPlayerObject(pMockPlayerObject);
	}

	return pNewMockLink;
}

void GUTHelper_Network::ClearLinks()
{
	MockNetServer* pMockNetServer = dynamic_cast<MockNetServer*>(gsys.pCommandCenter->GetNetServer());
	if (!pMockNetServer) return;

	pMockNetServer->ClearMockLinks();
}