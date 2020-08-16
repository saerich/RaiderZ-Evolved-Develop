#include "stdafx.h"
#include "PGuildMsgHandler.h"
#include "PGlobal.h"
#include "PServer.h"
#include "PCommandCenter.h"
#include "SProxyPlayerManager.h"
#include "SProxyPlayer.h"

PGuildMsgHandler::PGuildMsgHandler()
{
	// do nothing
}

PGuildMsgHandler::~PGuildMsgHandler()
{
	// do nothing
}

bool PGuildMsgHandler::OnRequest(const minet::MCommand* pCmd)
{
	// ��û�� Ȯ��
	CID nCID = m_msgHelper.GetSenderID(pCmd);
	SProxyPlayerManager* pManager = gsys.pServer->GetPlayerManager();
	SProxyPlayer* pPlayer = pManager->FindPlayer(nCID);
	if (pPlayer == NULL)	return true;

	// ä�� Ȯ��
	int nGuildID = m_msgHelper.GetReceiverID(pCmd);
	// TODO: ���ä�� �߰��� �� - praptor, 2010.01.11

	// �޽��� �뺸
	wstring strSenderName = pPlayer->GetName();
	MCommand* pNewCmd = m_msgHelper.MakeNewServerCommandGuildRes(m_strMsg, strSenderName, nGuildID);
	gsys.pCommandCenter->RouteToAll(pNewCmd);

	return true;
}
