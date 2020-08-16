#include "stdafx.h"
#include "GNarrationMsgHandler.h"
#include "GGlobal.h"
#include "GPlayerObjectManager.h"
#include "GEntityPlayer.h"

GNarrationMsgHandler::GNarrationMsgHandler()
{
	// do nothing
}

GNarrationMsgHandler::~GNarrationMsgHandler()
{
	// do nothing
}

bool GNarrationMsgHandler::OnRequest(const MCommand* pCmd)
{
	if (m_strMsg.empty())	return false;

	// ��û�� Ȯ��
	GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntity(m_uidCommandSender);
	VALID_RET(pPlayer != NULL, false);
	
	
	// ����ó��	
	if (!pPlayer->IsGM())	return false;


	// �޽��� ����
	MCommand* pNewCmd = m_msgHelper.MakeNewClientCommandNarrationRes(m_strMsg);
	pPlayer->RouteToThisCell(pNewCmd);

	return true;
}

bool GNarrationMsgHandler::OnResponse(const MCommand* pCmd)
{
	// do nothing
	return true;
}
