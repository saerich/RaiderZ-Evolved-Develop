#include "stdafx.h"
#include "ZCmdHandler_Comm.h"
#include "MCommand.h"
#include "ZCommandTable.h"
#include "ZCommandCenter.h"
#include "ZGameServerObject.h"
#include "ZGameServerObjectManager.h"
#include "ZLoginServerFacade.h"
#include "ZAppServerObjectManager.h"
#include "ZServer.h"
#include "MCommandCommunicator.h"
#include "ZNetServer.h"
#include "ZGameServerAcceptor.h"
#include "ZLoginServerAcceptor.h"
#include "ZAppServerAcceptor.h"
#include "ZPlayerLogic.h"
#include "ZFieldCommandRouter.h"
#include "ZConfig.h"
#include "ZPartyLogic.h"
#include "ZAppLogic.h"
#include "ZPlayerCommandRouter.h"
#include "ZFieldLogic.h"
#include "ZAdminCommandRouter.h"
#include "ZServerInfoDumper.h"
#include "ZGMCommandRouter.h"
#include "ZDuplicationLoginLogic.h"


ZCmdHandler_Comm::ZCmdHandler_Comm(MCommandCommunicator* pCC) : MCommandHandler(pCC)
{
	SetCmdHandler(MC_LOCAL_NET_CLEAR,				OnNetClear);
	SetCmdHandler(MMC_COMM_REQUEST_LOGIN_G2M,		OnGameServerLogin);
	SetCmdHandler(MLC_COMM_REQUEST_LOGIN,			OnLoginServerLogin);
	SetCmdHandler(MAC_COMM_REQUEST_LOGIN,			OnAppServerLogin);
	SetCmdHandler(MMC_COMM_REQUEST_APP_SERVER_LIST,	OnRequestAppServerList);

	SetCmdHandler(MLC_PLAYER_ADD_PLAYER_REQ,		OnRequestAddPlayer);
	SetCmdHandler(MLC_PLAYER_DELETE_PLAYER,			OnDeletePlayer_FromLoginServer);
	SetCmdHandler(MMC_PLAYER_DELETE_PLAYER,			OnDeletePlayer_FromGameServer);

	SetCmdHandler(MLC_PLAYER_STATE_SELECT_CHAR,		OnPlayerStateSelectChar);
	SetCmdHandler(MMC_PLAYER_STATE_IN_WORLD_REQ,	OnPlayerStateInWorldReq);
	SetCmdHandler(MMC_PLAYER_IN_FIELD_NOTIFY,		OnPlayerInFieldNotify);

	// �Ǹ�
	//SetCmdHandler(MLC_PLAYER_PMANG_ADD_PLAYER_REQ,	OnPmangRequestAddPlayer);

	// Admin
	SetCmdHandler(MMC_ADMIN_CHANGE_SERVER_MODE_REQ,	OnAdminChangeServerModeReq);
	SetCmdHandler(MMC_DEBUG_DUMP,					OnDebugDump);

	// GM
	SetCmdHandler(MMC_GM_SUMMON_REQ,	OnGMSummonReq);
}

MCommandResult ZCmdHandler_Comm::OnNetClear(MCommand* pCommand, MCommandHandler* pHandler)
{
	MUID uidClient;

	if (pCommand->GetParameter(&uidClient,	0, MPT_UID)==false) return CR_ERROR;

	// GameServer Disconnect ó��
	ZGameServerObject* pGameServerObject = gmgr.pGameServerObjectManager->FindGameServer(uidClient);	
	if (pGameServerObject != NULL)
	{
		int nGameServerID = pGameServerObject->GetID();
		gmgr.pGameServerObjectManager->DeleteObject(uidClient);
		mlog("Game Server Disconnected. (id=%d)\n", nGameServerID);

		// ��Ƽ���� ����
		gsys.pPartyLogic->AllMemberOfflineInGameServer(nGameServerID);

		// �÷��̾� ���� ����
		ZPlayerLogic playerLogic;
		playerLogic.AllMemberDeleteInGameServer(nGameServerID);

		ZFieldLogic fieldLogic;
		fieldLogic.AllFieldDeleteInGameServer(nGameServerID);

		return CR_TRUE;
	}

	// LoginServer Disconnect ó��
	if (uidClient == gmgr.pLoginServerFacade->GetUID())
	{
		gmgr.pLoginServerFacade->Disconnect();
		mlog("Login Server Disconnected.\n");

		// �÷��̾� ���� ����
		ZPlayerLogic playerLogic;
		playerLogic.AllMemberDeleteInLoginServer();
		return CR_TRUE;
	}

	// AppServer Disconnect ó��
	SCommObject* pAppServerObject = gmgr.pAppServerObjectManager->GetClient(uidClient);
	if (pAppServerObject != NULL)
	{
		gmgr.pAppServerObjectManager->DeleteObject(uidClient);
		mlog("App Server Disconnected\n");
		return CR_TRUE;
	}

	return CR_TRUE;
}


MCommandResult ZCmdHandler_Comm::OnGameServerLogin(MCommand* pCommand, MCommandHandler* pHandler)
{	
	wstring strServerName;
	int nServerType = 0;
	int nWorldID = 0;
	int nServerID = 0;
	int nCommandVersion;	

	if (pCommand->GetParameter(strServerName,	0, MPT_WSTR)==false) return CR_ERROR;
	if (pCommand->GetParameter(&nServerType,	1, MPT_INT)==false) return CR_ERROR;
	if (pCommand->GetParameter(&nWorldID,		2, MPT_INT)==false) return CR_ERROR;
	if (pCommand->GetParameter(&nServerID,		3, MPT_INT)==false) return CR_ERROR;
	if (pCommand->GetParameter(&nCommandVersion,4, MPT_INT)==false) return CR_ERROR;

	MUID uidClient = pCommand->GetSenderUID();
	if (uidClient.IsInvalid()) return CR_ERROR;


	ZGameServerAcceptor acceptor(uidClient, nServerID);

	if (CheckValidLoginReqCommand(&acceptor, nCommandVersion, nWorldID) == false)
	{
		return CR_TRUE;
	}

	if (CommLoginServer(&acceptor) == false)
	{
		return CR_TRUE;
	}

	// �ʵ尡 ���� ����������Ƿ� ��ü ���� ������ �ʵ� ������ ���� �����Ѵ�.
	ZFieldCommandRouter::RouteAllFieldInfo();


	// ��Ƽ���� ����ȭ
	gsys.pPartyLogic->PartyInfoAllReq(uidClient);


	return CR_TRUE;
}

MCommandResult ZCmdHandler_Comm::OnLoginServerLogin(MCommand* pCommand, MCommandHandler* pHandler)
{
	wstring strServerName;
	int nServerType = 0;
	int nWorldID = 0;
	int nServerID = 0;
	int nCommandVersion;	

	if (pCommand->GetParameter(strServerName,	0, MPT_WSTR)==false) return CR_ERROR;
	if (pCommand->GetParameter(&nServerType,	1, MPT_INT)==false) return CR_ERROR;
	if (pCommand->GetParameter(&nWorldID,		2, MPT_INT)==false) return CR_ERROR;
	if (pCommand->GetParameter(&nServerID,		3, MPT_INT)==false) return CR_ERROR;
	if (pCommand->GetParameter(&nCommandVersion,4, MPT_INT)==false) return CR_ERROR;

	MUID uidClient = pCommand->GetSenderUID();
	if (uidClient.IsInvalid()) return CR_ERROR;


	ZLoginServerAcceptor acceptor(uidClient, nServerID);

	if (CheckValidLoginReqCommand(&acceptor, nCommandVersion, nWorldID) == false)
	{
		return CR_TRUE;
	}

	if (CommLoginServer(&acceptor) == false)
	{
		return CR_TRUE;
	}

	return CR_TRUE;
}

MCommandResult ZCmdHandler_Comm::OnAppServerLogin(MCommand* pCommand, MCommandHandler* pHandler)
{
	wstring strServerName;
	int nServerType = 0;
	int nWorldID = 0;
	int nServerID = 0;
	int nCommandVersion;	
	vector<TD_APP_SERVER_INFO> vecAppServerList;

	if (pCommand->GetParameter(strServerName,	0, MPT_WSTR)==false) return CR_ERROR;
	if (pCommand->GetParameter(&nServerType,	1, MPT_INT)==false) return CR_ERROR;
	if (pCommand->GetParameter(&nWorldID,		2, MPT_INT)==false) return CR_ERROR;
	if (pCommand->GetParameter(&nServerID,		3, MPT_INT)==false) return CR_ERROR;
	if (pCommand->GetParameter(&nCommandVersion,4, MPT_INT)==false) return CR_ERROR;
	if (pCommand->GetBlob(vecAppServerList, 5) == false)			return CR_ERROR;


	MUID uidClient = pCommand->GetSenderUID();
	if (uidClient.IsInvalid()) return CR_ERROR;


	ZAppServerAcceptor acceptor(uidClient, nServerID, vecAppServerList);

	if (CheckValidLoginReqCommand(&acceptor, nCommandVersion, nWorldID) == false)
	{
		return CR_TRUE;
	}

	if (CommLoginServer(&acceptor) == false)
	{
		return CR_TRUE;
	}


	// ���Ͻ� �÷��̾� ���� ����; �ۼ����� ����۵� ��� �������� �÷��̾���� ������ �ٽ� �����Ѵ�.
	ZPlayerCommandRouter::RouteAllProxyPlayer(uidClient);

	return CR_TRUE;
}

bool ZCmdHandler_Comm::CommLoginServer( ZServerAcceptor* pAcceptor )
{
	VALID_RET(pAcceptor != NULL, false);

	if (pAcceptor->IsAlreadyExist())
	{
		MCommand* pFailCommand = pAcceptor->MakeFailCommand(CRM_FAIL_LOGIN_ALREADY_EXIST);
		gsys.pCommandCenter->PostCommand(pFailCommand);

		return false;
	}

	if (pAcceptor->Accept() == false)
	{
		MCommand* pFailCommand = pAcceptor->MakeFailCommand(CRM_FAIL_LOGIN_ACCEPT);
		gsys.pCommandCenter->PostCommand(pFailCommand);

		return false;
	}

	// �α��� ���� �˸�
	MCommand* pResponseCommand = pAcceptor->MakeResponseCommand();
	gsys.pCommandCenter->PostCommand(pResponseCommand);

	return true;
}

MCommandResult ZCmdHandler_Comm::OnRequestAppServerList(MCommand* pCommand, MCommandHandler* pHandler)
{
	// no parameter command
	
	// ��� ���� �� ����
	MUID uidClient = pCommand->GetSenderUID();

	ZAppLogic logic;
	logic.RequestAppServerList(uidClient);

	return CR_TRUE;
}

MCommandResult ZCmdHandler_Comm::OnRequestAddPlayer(MCommand* pCommand, MCommandHandler* pHandler)
{
	MUID uidPlayer;
	int nAID;
	wstring strUserID;
	
	if (pCommand->GetParameter(&uidPlayer,		0, MPT_UID)==false) return CR_ERROR;
	if (pCommand->GetParameter(&nAID,			1, MPT_INT)==false) return CR_ERROR;
	if (pCommand->GetParameter(strUserID,		2, MPT_WSTR)==false) return CR_ERROR;
	
	
	// �α��� ���� Ȯ��
	MUID uidLoginServer = pCommand->GetSenderUID();
	if (uidLoginServer != gmgr.pLoginServerFacade->GetUID())	return CR_TRUE;
	
	// �÷��̾� �ߺ� �α��� üũ
	ZDuplicationLoginLogic duplicationLoginLogic;
	if (duplicationLoginLogic.IsDuplicated(nAID))
	{
		// ���ӿ� ������ �ִ� �÷��̾ ��������.
		duplicationLoginLogic.StartKickInWorldPlayer(uidPlayer, nAID, strUserID/*, NULL*/);
		ZPlayerCommandRouter::SendAddPlayerResponse(uidLoginServer, uidPlayer, CR_INFO_LOGIN_KICKING_DUPLICATED_PLAYER);
		return CR_TRUE;
	}

	// �ִ� ���� �� üũ
	if (gmgr.pPlayerManager->GetObjectCount() >= WORLD_PLAYER_LIMIT)
	{
		ZPlayerCommandRouter::SendAddPlayerResponse(uidLoginServer, uidPlayer, CR_FAIL_LOGIN_WORLD_IS_FULL);
		return CR_TRUE;
	}
	
	// �÷��̾� ���
	ZPlayerLogic playerLogic;
	if (!playerLogic.AddPlayer(uidPlayer, nAID, strUserID/*, NULL*/))
	{
		mlog3("Error! ZCmdHandler_Comm::OnRequestAddPlayer(), Failed! ZPlayerLogic::AddPlayer(AID=%d)\n", nAID);
		return CR_TRUE;
	}
	
	ZPlayerCommandRouter::SendAddPlayerResponse(uidLoginServer, uidPlayer, CR_SUCCESS);
	
	return CR_TRUE;
}

//MCommandResult ZCmdHandler_Comm::OnPmangRequestAddPlayer(MCommand* pCommand, MCommandHandler* pHandler)
//{
//	MUID uidPlayer;
//	int nAID;
//	wstring strUserID;
//	TD_PMANG_USER_DATA* pTdPmangUserData;
//
//	if (!pCommand->GetParameter(&uidPlayer,		0, MPT_UID))	return CR_ERROR;
//	if (!pCommand->GetParameter(&nAID,			1, MPT_INT))	return CR_ERROR;
//	if (!pCommand->GetParameter(strUserID,		2, MPT_WSTR))	return CR_ERROR;
//	if (!pCommand->GetSingleBlob(pTdPmangUserData, 3))			return CR_ERROR;
//	
//
//	PmUserData pmUserData;
//	pmUserData.Import(pTdPmangUserData);
//
//	if (!pmUserData.IsValid())	return CR_ERROR;
//
//	
//	// �α��� ���� Ȯ��	
//	MUID uidLoginServer = pCommand->GetSenderUID();
//	if (uidLoginServer != gmgr.pLoginServerFacade->GetUID())	return CR_TRUE;
//
//	// �÷��̾� �ߺ� �α��� üũ
//	ZDuplicationLoginLogic duplicationLoginLogic;
//	if (duplicationLoginLogic.IsDuplicated(nAID))
//	{
//		// ���ӿ� ������ �ִ� �÷��̾ ��������.
//		duplicationLoginLogic.StartKickInWorldPlayer(uidPlayer, nAID, strUserID, &pmUserData);
//		ZPlayerCommandRouter::SendPmangAddPlayerResponse(uidLoginServer, uidPlayer, CR_INFO_LOGIN_KICKING_DUPLICATED_PLAYER);
//		return CR_TRUE;
//	}
//
//	// �ִ� ���� �� üũ
//	if (gmgr.pPlayerManager->GetObjectCount() >= WORLD_PLAYER_LIMIT)
//	{
//		ZPlayerCommandRouter::SendPmangAddPlayerResponse(uidLoginServer, uidPlayer, CR_FAIL_LOGIN_WORLD_IS_FULL);
//		return CR_TRUE;
//	}
//
//	// �÷��̾� ���
//	ZPlayerLogic playerLogic;
//	if (!playerLogic.AddPlayer(uidPlayer, nAID, strUserID, &pmUserData))
//	{
//		mlog3("Error! ZCmdHandler_Comm::OnPmangRequestAddPlayer(), Failed! ZPlayerLogic::AddPlayer(AID=%d)\n", nAID);
//		return CR_TRUE;
//	}
//
//	ZPlayerCommandRouter::SendPmangAddPlayerResponse(uidLoginServer, uidPlayer, CR_SUCCESS);
//
//	return CR_TRUE;
//}

MCommandResult ZCmdHandler_Comm::OnDeletePlayer_FromLoginServer(MCommand* pCommand, MCommandHandler* pHandler)
{
	int nAID;
	MUID uidPlayer;

	if (pCommand->GetParameter(&nAID,			0, MPT_INT)==false) return CR_ERROR;
	if (pCommand->GetParameter(&uidPlayer,		1, MPT_UID)==false) return CR_ERROR;

	// �ߺ����� ó�� ������� �÷��̾� ó��
	ZDuplicationLoginLogic duplicationLoginLogic;
	if (duplicationLoginLogic.ExistWaitingPlayer(nAID))
	{
		duplicationLoginLogic.FlushDuplicatedLogin(nAID, uidPlayer);
	}
	else
	{
		ZPlayerLogic playerLogic;
		playerLogic.DeletePlayer(nAID);
	}

	return CR_TRUE;
}

MCommandResult ZCmdHandler_Comm::OnDeletePlayer_FromGameServer(MCommand* pCommand, MCommandHandler* pHandler)
{
	int nAID;
	MUID uidPlayer;

	if (pCommand->GetParameter(&nAID,			0, MPT_INT)==false) return CR_ERROR;
	if (pCommand->GetParameter(&uidPlayer,		1, MPT_UID)==false) return CR_ERROR;

	// �ߺ����� ó�� ������� �÷��̾� ó��
	ZDuplicationLoginLogic duplicationLoginLogic;
	if (duplicationLoginLogic.ExistWaitingPlayer(nAID))
	{
		duplicationLoginLogic.FlushDuplicatedLogin(nAID, uidPlayer);
	}
	else
	{
		ZPlayerLogic playerLogic;
		playerLogic.DeletePlayer(nAID);
	}

	return CR_TRUE;
}

MCommandResult ZCmdHandler_Comm::OnPlayerStateSelectChar(MCommand* pCommand, MCommandHandler* pHandler)
{
	int nAID;
	MUID uidPlayer;

	if (pCommand->GetParameter(&nAID,			0, MPT_INT)==false) return CR_ERROR;
	if (pCommand->GetParameter(&uidPlayer,		1, MPT_UID)==false) return CR_ERROR;

	ZPlayerLogic playerLogic;
	playerLogic.OnSelectChar(nAID, uidPlayer);

	return CR_TRUE;
}

MCommandResult ZCmdHandler_Comm::OnPlayerStateInWorldReq(MCommand* pCommand, MCommandHandler* pHandler)
{
	int nAID;
	int nCID;
	MUID uidPlayer;
	wstring strPlayerName;
	int nGID;

	if (pCommand->GetParameter(&nAID,			0, MPT_INT)==false) return CR_ERROR;
	if (pCommand->GetParameter(&nCID,			1, MPT_INT)==false) return CR_ERROR;
	if (pCommand->GetParameter(&uidPlayer,		2, MPT_UID)==false) return CR_ERROR;
	if (pCommand->GetParameter(strPlayerName,	3, MPT_WSTR)==false) return CR_ERROR;
	if (pCommand->GetParameter(&nGID,			4, MPT_INT)==false) return CR_ERROR;

	MUID uidRequestGameServer = pCommand->GetSenderUID();

	ZPlayerLogic playerLogic;
	playerLogic.OnInWorld(uidRequestGameServer, nAID, nCID, uidPlayer, strPlayerName, nGID);

	return CR_TRUE;
}

MCommandResult ZCmdHandler_Comm::OnPlayerInFieldNotify(MCommand* pCommand, MCommandHandler* pHandler)
{
	int nCID;
	MUID uidField;
	int nFieldID;

	if (pCommand->GetParameter(&nCID,			0, MPT_INT)==false) return CR_ERROR;
	if (pCommand->GetParameter(&uidField,		1, MPT_UID)==false) return CR_ERROR;
	if (pCommand->GetParameter(&nFieldID,		2, MPT_INT)==false) return CR_ERROR;

	MUID uidRequestGameServer = pCommand->GetSenderUID();

	ZPlayerLogic playerLogic;
	playerLogic.InField(uidRequestGameServer, nCID, uidField, nFieldID);

	return CR_TRUE;
}

MCommandResult ZCmdHandler_Comm::OnAdminChangeServerModeReq(MCommand* pCommand, MCommandHandler* pHandler)
{
	int nServerMode;

	if (!pCommand->GetParameter(&nServerMode, 0, MPT_INT))	return CR_ERROR;

	if (SERVER_MODE_INVALID >= nServerMode)		return CR_TRUE;
	if (SERVER_MODE_MAX <= nServerMode)			return CR_TRUE;
	if (ZConfig::m_nServerMode == nServerMode)	return CR_TRUE;

	ZConfig::m_nServerMode = nServerMode;

	ZAdminCommandRouter router;
	router.RouteChangeServerMode(ZConfig::m_nServerMode);

	return CR_TRUE;
}

MCommandResult ZCmdHandler_Comm::OnDebugDump(MCommand* pCommand, MCommandHandler* pHandler)
{
	int nAID;

	if (!pCommand->GetParameter(&nAID, 0, MPT_INT))	return CR_ERROR;

	ZPlayer* pPlayer = gmgr.pPlayerManager->Find(nAID);
	if (pPlayer == NULL) return CR_TRUE;

	ZServerInfoDumper dumper;
	dumper.Dump(pPlayer->GetPlayerName());

	return CR_TRUE;
}

MCommandResult ZCmdHandler_Comm::OnGMSummonReq(MCommand* pCommand, MCommandHandler* pHandler)
{
	wstring strTarName;
	MUID uidFieldGroup;
	int nFieldID;
	CHANNELID nChannelID;
	vec3 vPos;

	if (!pCommand->GetParameter(strTarName,			0, MPT_WSTR))	return CR_ERROR;
	if (!pCommand->GetParameter(&uidFieldGroup,		1, MPT_UID))	return CR_ERROR;
	if (!pCommand->GetParameter(&nFieldID,			2, MPT_INT))	return CR_ERROR;
	if (!pCommand->GetParameter(&nChannelID,		3, MPT_INT))	return CR_ERROR;
	if (!pCommand->GetParameter(&vPos,				4, MPT_VEC))	return CR_ERROR;

	ZPlayer* pTarPlayer = gmgr.pPlayerManager->FindByName(strTarName);
	if (pTarPlayer == NULL) return CR_TRUE;
	if (pTarPlayer->IsInWorld() == false) return CR_TRUE;

	ZGMCommandRouter::RouteSummon(pTarPlayer->GetGameServerUID(), pTarPlayer->GetUID(), uidFieldGroup, nFieldID, nChannelID, vPos);

	return CR_TRUE;
}


bool ZCmdHandler_Comm::CheckValidLoginReqCommand( ZServerAcceptor* pAcceptor, int nCommandVersion, int nWorldID )
{
	// Ŀ�ǵ� ���� Ȯ��
	if (!CCommandVersionChecker::IsValid(nCommandVersion))
	{
		MCommand* pFailCommand = pAcceptor->MakeFailCommand(CRM_FAIL_LOGIN_COMMAND_INVALID_VERSION);
		gsys.pCommandCenter->PostCommand(pFailCommand);

		return false;
	}


	// World ID Ȯ��
	if (nWorldID != ZConfig::m_nMyWorldID)
	{
		MCommand* pFailCommand = pAcceptor->MakeFailCommand(CRM_FAIL_LOGIN_INVALID_WORLD_ID);
		gsys.pCommandCenter->PostCommand(pFailCommand);

		return false;
	}

	return true;
}