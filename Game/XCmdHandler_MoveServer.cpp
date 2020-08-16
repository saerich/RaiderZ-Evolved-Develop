#include "StdAfx.h"
#include "XCmdHandler_MoveServer.h"
#include "MiNetHelper.h"
#include "XNetwork.h"
#include "XConnectionManager.h"
#include "CSNet.h"
#include "CCommandResultTable.h"
#include "XPost_Comm.h"


XCmdHandler_MoveServer::XCmdHandler_MoveServer(MCommandCommunicator* pCC) : MCommandHandler(pCC)
{
	SetCmdHandler(MC_COMM_MOVE_GAME_SERVER,		OnMoveGameServer);	///< From GS To GS
	
	SetCmdHandler(MC_COMM_START_MOVE_TO_LOGIN_SERVER_RES,	OnStartMoveToLoginServerRes);	///< LoginServer �� �̵� ����.
	SetCmdHandler(MC_COMM_RESPONSE_MOVE_LOGIN_SERVER,		OnResponseMoveLoginServer);		///< LoginServer �� �̵��ؼ� ���� ��û.
}

MCommandResult XCmdHandler_MoveServer::OnMoveGameServer(MCommand* pCommand, MCommandHandler* pHandler)
{
	MCommandParameter* pParam = pCommand->GetParameter(0);
	if(pParam->GetType()!=MPT_BLOB) return CR_ERROR;

	int nBlobCount = pParam->GetBlobCount();
	if (nBlobCount <= 0) return CR_TRUE;

	TD_LOGIN_GAME_SERVER_INFO* pLoginGameServerInfo = (TD_LOGIN_GAME_SERVER_INFO*)pParam->GetPointer();
	if (pLoginGameServerInfo == NULL) return CR_TRUE;


	wstring strIP = pLoginGameServerInfo->strHostName;
	int nPort = (int)pLoginGameServerInfo->nPort;

	global.net->GetConnectionManager()->MoveFromGameServerToGameServer(strIP, nPort, pLoginGameServerInfo->uidConnectionKey);

	return CR_TRUE;
}

MCommandResult XCmdHandler_MoveServer::OnStartMoveToLoginServerRes(MCommand* pCommand, MCommandHandler* pHandler)
{
	int nResult;
	MUID uidConnectionKey;

	if ( !pCommand->GetParameter(&nResult,			0, MPT_INT) )	return CR_ERROR;
	if ( !pCommand->GetParameter(&uidConnectionKey,	1, MPT_UID) )	return CR_ERROR;

	int nPort = DEFAULT_LOGIN_SERVER_PORT;
	if (gvar.Network.bLoginToGameServer)
		nPort = DEFAULT_GAME_SERVER_PORT;

	// ���� �̵�ó�� ����. GameServer Disconnect.
	global.net->GetConnectionManager()->MoveFromGameServerToLoginServer(gvar.Network.szIP, nPort, uidConnectionKey);

	/// TODO : ���ӿ��� ����ϴ� �����͵� �ʱ�ȭ �ʿ�. - aibeast (20110223)

	gvar.MyInfo.Clear();

	return CR_TRUE;
}

MCommandResult XCmdHandler_MoveServer::OnResponseMoveLoginServer(MCommand* pCommand, MCommandHandler* pHandler)
{
	int nResult;
	MUID uidAllocUID;

	if ( !pCommand->GetParameter(&nResult,		0, MPT_INT) )	return CR_ERROR;
	if ( !pCommand->GetParameter(&uidAllocUID,	1, MPT_UID) )	return CR_ERROR;

	if (nResult != CR_SUCCESS)
	{
		/// TODO : �����޽��� ���. ���� �����ӿ� �°� ������ �޽��� �ڽ� ��� �ʿ�. - aibeast (20110223)
		/// (XCmdHandler_GameStart.cpp) XCmdHandler_GameStart::CommonResponseLogin() �Լ��� ����ó���� �����ϰ� ó���մϴ�.
		//global.ui->ShowMessageBox(nResult);
		global.net->PostDisconnect();		///< ���⼭ �����ϸ� ���� ����.
		return CR_TRUE;
	}

	global.net->GetConnectionManager()->OnConnectedToLoginServer();
	gvar.MyInfo.MyUID = uidAllocUID;

	// ĳ���� ��� ��û.
	XPostChar_RequestAccountCharList();

	return CR_TRUE;
}
