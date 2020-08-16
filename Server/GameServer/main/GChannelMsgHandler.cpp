#include "stdafx.h"
#include "GChannelMsgHandler.h"
#include "GGlobal.h"
#include "GPlayerObjectManager.h"
#include "GEntityPlayer.h"
#include "GAppServerFacade.h"
#include "GCommandCenter.h"

GChannelMsgHandler::GChannelMsgHandler()
{
	// do nothing
}

GChannelMsgHandler::~GChannelMsgHandler()
{
	// do nothing
}

bool GChannelMsgHandler::OnRequest(const MCommand* pCmd)
{
	if (m_strMsg.empty())	return false;

	// ��û�� Ȯ��
	GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntity(m_uidCommandSender);
	VALID_RET(pPlayer != NULL, false);
	
	// �ۼ����� ����
	CID nCID = pPlayer->GetCID();
	MUID uidChannel = m_msgHelper.GetReceiverUID(pCmd);
	MCommand* pNewCmd = m_msgHelper.MakeNewServerCommandChannelReq(m_strMsg, nCID, uidChannel);
	
	gsys.pAppServerFacade->Route(pNewCmd);

	return true;
}

bool GChannelMsgHandler::OnResponse(const MCommand* pCmd)
{
	// Ŭ���̾�Ʈ�� ����
	wstring strSenderName = m_msgHelper.GetSenderName(pCmd);
	MUID uidChannel = m_msgHelper.GetReceiverUID(pCmd);
	MCommand* pNewCmd = m_msgHelper.MakeNewClientCommandChannelRes(m_strMsg, strSenderName, uidChannel);
		
	gsys.pCommandCenter->RouteToChatChannel(uidChannel, pNewCmd);

	return true;
}
