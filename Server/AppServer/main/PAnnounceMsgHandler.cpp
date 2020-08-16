#include "stdafx.h"
#include "PAnnounceMsgHandler.h"
#include "STypes.h"
#include "PCommandCenter.h"
#include "PGlobal.h"
#include "SProxyPlayerManager.h"
#include "SProxyPlayer.h"
#include "PServer.h"

PAnnounceMsgHandler::PAnnounceMsgHandler()
{
	// do nothing
}

PAnnounceMsgHandler::~PAnnounceMsgHandler()
{
	// do nothing
}

bool PAnnounceMsgHandler::OnRequest(const minet::MCommand* pCmd)
{
	// ��û ĳ���� Ȯ��
	CID nCID = m_msgHelper.GetSenderID(pCmd);
	SProxyPlayerManager* pManager = gsys.pServer->GetPlayerManager();
	SProxyPlayer* pPlayer = pManager->FindPlayer(nCID);
	if (pPlayer == NULL)	return true;
	
	// �ȳ�
	wstring strSenderName = pPlayer->GetName();
	MCommand* pNewCmd = m_msgHelper.MakeNewServerCommandAnnounceRes(m_strMsg, strSenderName);
	gsys.pCommandCenter->RouteToAll(pNewCmd);	

	return true;
}
