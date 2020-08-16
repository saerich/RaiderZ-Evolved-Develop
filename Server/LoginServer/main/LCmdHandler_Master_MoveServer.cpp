#include "StdAfx.h"
#include "LCmdHandler_Master_MoveServer.h"
#include "CCommandTable_Master.h"
#include "CCommandResultTable.h"
#include "LCmdRouter_MoveServer.h"
#include "LPlayerAcceptor_FromGameServer.h"
#include "LGlobal.h"
#include "LPlayerAcceptManager.h"

#include "CTransData.h"
#include "LTimeoutManager.h"
#include "LPlayerObject.h"
#include "LPlayerObjectManager.h"
#include "LPlayerLoginGameServerRetryer.h"
#include "SCmdRouter_Login.h"
#include "LCommandCenter.h"


LCmdHandler_Master_MoveServer::LCmdHandler_Master_MoveServer(MCommandCommunicator* pCC)
: MCommandHandler(pCC)
{
	SetCmdHandler(MLC_COMM_RESPONSE_LOGIN_GAME_SERVER,	OnResponseLoginGameServer);
	SetCmdHandler(MLC_COMM_READY_ENTER_LOGINSERVER_REQ,	OnReadyEnterLoginServerReq);
	SetCmdHandler(MLC_COMM_CANCEL_MOVE_LOGINSERVER,		OnCancelMoveLoginServer);
}


MCommandResult LCmdHandler_Master_MoveServer::OnResponseLoginGameServer(MCommand* pCommand, MCommandHandler* pHandler)
{
	// Ŀ�ǵ� ���� Ȯ��
	MUID uidPlayer;
	int nResult = 0;
	TD_LOGIN_GAME_SERVER_INFO* pLoginGameServerInfo = NULL;
	int nCount = 0;

	if (!pCommand->GetParameter(&uidPlayer,	0, MPT_UID))		return CR_ERROR;
	if (!pCommand->GetParameter(&nResult,	1, MPT_INT))		return CR_ERROR;
	if (!pCommand->GetBlob(pLoginGameServerInfo, nCount, 2))	return CR_ERROR;

	// Timeout üũ ����
	gmgr.pTimeoutManager->StopWatchingSelMyCharTimeout(uidPlayer);

	// ��û �÷��̾� Ȯ��
	LPlayerObject* pPlayer = gmgr.pPlayerObjectManager->GetPlayer(uidPlayer);
	if (pPlayer == NULL) return CR_TRUE;
	if (pPlayer->GetState() != PlayerObjectState::POS_LOGIN_GAME_SERVER) return CR_TRUE;

	if (CR_SUCCESS_LOGIN_GAME_SERVER_RETRY == nResult)
	{
		// ��õ� ������ ��� Retryer �� ���
		gsys.pPlayerLoginGameServerRetryer->AddRetryer(uidPlayer, pPlayer->GetSelectedCharIndex());

		return CR_TRUE;
	}

	if (CR_SUCCESS == nResult)
	{
		// ����� ������ ��� MOVER_SERVER ���·� ����.
		pPlayer->OnMoveServer();

		// ���� �̵� Timeout ����
		gmgr.pTimeoutManager->StartWatchingMoveServerTimeout(uidPlayer);
	}
	else
	{
		// ����� ������ ���, SelectChar ���·� �ǵ���.
		pPlayer->OnSelectChar();
	}


	// ��� ����
	SCmdRouter_Login cmdRouter(gsys.pCommandCenter);
	cmdRouter.ResponseSelMyChar(pPlayer->GetUID(), nResult);

	if (nResult == CR_SUCCESS && pLoginGameServerInfo != NULL)
	{
		cmdRouter.MoveToGameServer(pPlayer->GetUID(), pLoginGameServerInfo);
	}

	return CR_TRUE;
}

MCommandResult LCmdHandler_Master_MoveServer::OnReadyEnterLoginServerReq(MCommand* pCommand, MCommandHandler* pHandler)
{
	MUID	uidRequestGameServer;
	MUID	uidRequester;
	MUID	uidConnectionKey;
	AID		nAID;
	wstring	strUserID;

	if (!pCommand->GetParameter(&uidRequestGameServer,	0, MPT_UID))	return CR_ERROR;
	if (!pCommand->GetParameter(&uidRequester,			1, MPT_UID))	return CR_ERROR;
	if (!pCommand->GetParameter(&uidConnectionKey,		2, MPT_UID))	return CR_ERROR;
	if (!pCommand->GetParameter(&nAID,					3, MPT_INT))	return CR_ERROR;
	if (!pCommand->GetParameter(strUserID,				4, MPT_WSTR))	return CR_ERROR;
	
	if (gmgr.pPlayerAcceptManager->FindAcceptor(uidConnectionKey))
	{
		LCmdRouter_MoveServer::RouteReadyEnterLoginServerRes(uidRequestGameServer, uidRequester, CR_FAIL, uidConnectionKey);
		mlog("Failed! LCmdHandler_Master_MoveServer::OnReadyEnterLoginServerReq(), Already Exist Acceptor! (AID : %d)\n", nAID);
		return CR_TRUE;
	}

	LPlayerAcceptor_FromGameServer* pAcceptor = new LPlayerAcceptor_FromGameServer();
	pAcceptor->ReserveAccept(uidConnectionKey, nAID, strUserID, uidRequester);

	if (!gmgr.pPlayerAcceptManager->Add(pAcceptor))
	{
		SAFE_DELETE(pAcceptor);
		LCmdRouter_MoveServer::RouteReadyEnterLoginServerRes(uidRequestGameServer, uidRequester, CR_FAIL, uidConnectionKey);
		return CR_TRUE;
	}

	LCmdRouter_MoveServer::RouteReadyEnterLoginServerRes(uidRequestGameServer, uidRequester, CR_SUCCESS, uidConnectionKey);

	return CR_TRUE;
}

MCommandResult LCmdHandler_Master_MoveServer::OnCancelMoveLoginServer(MCommand* pCommand, MCommandHandler* pHandler)
{
	MUID uidConnectionKey;

	if (!pCommand->GetParameter(&uidConnectionKey, 0, MPT_UID))		return CR_ERROR;
	
	gmgr.pPlayerAcceptManager->Del(uidConnectionKey);
	
	return CR_TRUE;
}
