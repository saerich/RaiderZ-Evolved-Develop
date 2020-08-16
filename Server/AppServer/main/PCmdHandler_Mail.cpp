#include "stdafx.h"
#include "PCmdHandler_Mail.h"
#include "CCommandTable_App.h"
#include "STypes.h"
#include "CTransData.h"
#include "PGlobal.h"
#include "SProxyPlayerManager.h"
#include "PServer.h"
#include "PPostOfficeSystem.h"

PCmdHandler_Mail::PCmdHandler_Mail(MCommandCommunicator* pCC) : MCommandHandler(pCC)
{
	SetCmdHandler(MPC_MAIL_ROUTE_NEW_MAIL_TO_RECEIVER, RouteNewMailToReceiver);
	SetCmdHandler(MPC_MAIL_RETRY_TRANS_NEW_MAIL, TransNewMailRes);
}

MCommandResult PCmdHandler_Mail::RouteNewMailToReceiver(MCommand* pCommand, MCommandHandler* pHandler)
{
	// ���� Ȯ��
	CID nCID;
	TD_MAIL_MAILBOX_MAIL_INFO* pInfo;

	if (!pCommand->GetParameter(&nCID, 0, MPT_INT))		return CR_ERROR;
	if (!pCommand->GetSingleBlob(pInfo, 1))				return CR_ERROR;

	
	// �뺸 �÷��̾ Ȯ���Ѵ�.
	SProxyPlayerManager* pPlayerManager = gsys.pServer->GetPlayerManager();
	SProxyPlayer* pPlayer = pPlayerManager->FindPlayer(nCID);

	if (pPlayer == NULL)
	{
		// �ش� �÷��̾�� �̹� ������ �����ߴ�. �뺸 �ʴ´�.
		return CR_TRUE;
	}
		

	// �뺸�Ѵ�.
	gsys.pPostOfficeSystem->RouteNewMailToReceiver(pPlayer, pInfo);
	

	return CR_TRUE;
}

MCommandResult PCmdHandler_Mail::TransNewMailRes(MCommand* pCommand, MCommandHandler* pHandler)
{
	// ���� Ȯ��
	CID nCID;
	TD_MAIL_MAILBOX_MAIL_INFO* pInfo;

	if (!pCommand->GetParameter(&nCID, 0, MPT_INT))		return CR_ERROR;
	if (!pCommand->GetSingleBlob(pInfo, 1))				return CR_ERROR;
	
	
	// �뺸 �÷��̾ Ȯ���Ѵ�.
	SProxyPlayerManager* pPlayerManager = gsys.pServer->GetPlayerManager();
	SProxyPlayer* pPlayer = pPlayerManager->FindPlayer(nCID);

	if (pPlayer == NULL)
	{
		// �ش� �÷��̾�� �̹� ������ �����ߴ�. �뺸 �ʴ´�.
		return CR_TRUE;
	}


	// �ٽ� �뺸�Ѵ�.
	gsys.pPostOfficeSystem->RetryNewMailToReceiver(pPlayer, pInfo);


	return CR_TRUE;
}
