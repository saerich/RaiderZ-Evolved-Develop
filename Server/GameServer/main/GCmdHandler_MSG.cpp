#include "StdAfx.h"
#include "GCmdHandler_MSG.h"
#include "GGlobal.h"
#include "GMsgCommandFacade.h"
#include "GAppServerFacade.h"
#include "GPlayerObjectManager.h"
#include "GEntityPlayer.h"
#include "GServerChatChannelRouter.h"
#include "CCommandTable.h"

GCmdHandler_MSG::GCmdHandler_MSG(MCommandCommunicator* pCC) : MCommandHandler(pCC)
{
	SetCmdHandler(MC_MSG_REQ,					OnMsgReq);
	SetCmdHandler(MC_MSG_CREATE_CHANNEL_REQ,	OnMsgCreateChannel);
	SetCmdHandler(MC_MSG_ENTER_CHANNEL_REQ,		OnMsgEnterChannelReq);
	SetCmdHandler(MC_MSG_LEAVE_CHANNEL_REQ,		OnMsgLeaveChannelReq);
	SetCmdHandler(MC_MSG_KICK_PLAYER_REQ,		OnMsgKickPlayerReq);
	SetCmdHandler(MC_MSG_BAN_PLAYER_REQ,		OnMsgBanPlayerReq);
	SetCmdHandler(MC_MSG_UNBAN_PLAYER_REQ,		OnMsgUnbanPlayerReq);
	SetCmdHandler(MC_MSG_CHANGE_HOST_REQ,		OnMsgChangeHostReq);
}

MCommandResult GCmdHandler_MSG::OnMsgReq(MCommand* pCmd, MCommandHandler* pHandler)
{
	// Ŀ�ǵ� �Ľ� �� ó���� ��� ���
	if (!gsys.pMsgCommandFacade->RequestMessage(pCmd))	return CR_ERROR;

	return CR_TRUE;
}

MCommandResult GCmdHandler_MSG::OnMsgCreateChannel(MCommand* pCmd, MCommandHandler* pHandler)
{
	// Ŀ�ǵ� Ȯ��
	wstring strChannelName;
	if (!pCmd->GetParameter(strChannelName, 0, MPT_WSTR))	return CR_ERROR;
	if (strChannelName.empty())	return CR_ERROR;

	// ���ڿ� �Ķ���� üũ
	if (strChannelName.size() > MAX_CHATTING_CHANNEL_NAME_LEN) return CR_ERROR;

	// ��û�� Ȯ��
	MUID uidPlayer = pCmd->GetSenderUID();
	GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntityInWorld(uidPlayer);
	if (pPlayer == NULL)	return CR_TRUE;

	// �ۼ��� ����
	CID cidPlayer = pPlayer->GetCID();	
	GServerChatChannelRouter* router = gsys.pAppServerFacade->GetChatChannelRouter();
	router->CreateChannelReq(cidPlayer, strChannelName);
	
	return CR_TRUE;
}

MCommandResult GCmdHandler_MSG::OnMsgEnterChannelReq(MCommand* pCmd, MCommandHandler* pHandler)
{
	// Ŀ�ǵ� Ȯ��
	wstring strChannelName;
	if (!pCmd->GetParameter(strChannelName, 0, MPT_WSTR))	return CR_ERROR;
	if (strChannelName.empty())	return CR_ERROR;

	// ���ڿ� �Ķ���� üũ
	if (strChannelName.size() > MAX_CHATTING_CHANNEL_NAME_LEN) return CR_ERROR;

	// ��û�� Ȯ��
	MUID uidPlayer = pCmd->GetSenderUID();
	GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntityInWorld(uidPlayer);
	if (pPlayer == NULL)	return CR_TRUE;

	// �ۼ��� ����
	CID cidPlayer = pPlayer->GetCID();
	GServerChatChannelRouter* router = gsys.pAppServerFacade->GetChatChannelRouter();
	router->EnterChannelReq(cidPlayer, strChannelName);

	return CR_TRUE;
}

MCommandResult GCmdHandler_MSG::OnMsgLeaveChannelReq(MCommand* pCmd, MCommandHandler* pHandler)
{
	// Ŀ�ǵ� Ȯ��
	MUID uidChannel;
	if (!pCmd->GetParameter(&uidChannel, 0, MPT_UID))	return CR_ERROR;
	if (uidChannel.IsInvalid())	return CR_ERROR;

	// ��û�� Ȯ��
	MUID uidPlayer = pCmd->GetSenderUID();
	GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntityInWorld(uidPlayer);
	if (pPlayer == NULL)	return CR_TRUE;

	// �ۼ��� ����
	CID cidPlayer = pPlayer->GetCID();
	GServerChatChannelRouter* router = gsys.pAppServerFacade->GetChatChannelRouter();
	router->LeaveChannelReq(cidPlayer, uidChannel);

	return CR_TRUE;
}

MCommandResult GCmdHandler_MSG::OnMsgKickPlayerReq(MCommand* pCmd, MCommandHandler* pHandler)
{
	// Ŀ�ǵ� Ȯ��
	MUID uidChannel;
	wstring strTargetName;
	if (!pCmd->GetParameter(&uidChannel, 0, MPT_UID))	return CR_ERROR;
	if (!pCmd->GetParameter(strTargetName, 1, MPT_WSTR))	return CR_ERROR;
	if (uidChannel.IsInvalid())	return CR_ERROR;
	if (strTargetName.empty())	return CR_ERROR;

	// ���ڿ� �Ķ���� üũ
	if (strTargetName.size() > PLAYER_NAME_LEN) return CR_ERROR;

	// ��û�� Ȯ��
	MUID uidPlayer = pCmd->GetSenderUID();
	GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntityInWorld(uidPlayer);
	if (pPlayer == NULL)	return CR_TRUE;

	// �ۼ��� ����
	CID cidPlayer = pPlayer->GetCID();
	GServerChatChannelRouter* router = gsys.pAppServerFacade->GetChatChannelRouter();
	router->KickPlayerReq(cidPlayer, uidChannel, strTargetName);

	return CR_TRUE;
}

MCommandResult GCmdHandler_MSG::OnMsgBanPlayerReq(MCommand* pCmd, MCommandHandler* pHandler)
{
	// Ŀ�ǵ� Ȯ��
	MUID uidChannel;
	wstring strTargetName;
	if (!pCmd->GetParameter(&uidChannel, 0, MPT_UID))	return CR_ERROR;
	if (!pCmd->GetParameter(strTargetName, 1, MPT_WSTR))	return CR_ERROR;
	if (uidChannel.IsInvalid())	return CR_ERROR;
	if (strTargetName.empty())	return CR_ERROR;

	// ���ڿ� �Ķ���� üũ
	if (strTargetName.size() > PLAYER_NAME_LEN) return CR_ERROR;

	// ��û�� Ȯ��
	MUID uidPlayer = pCmd->GetSenderUID();
	GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntityInWorld(uidPlayer);
	if (pPlayer == NULL)	return CR_TRUE;

	// �ۼ��� ����
	CID cidPlayer = pPlayer->GetCID();
	GServerChatChannelRouter* router = gsys.pAppServerFacade->GetChatChannelRouter();
	router->BanPlayerReq(cidPlayer, uidChannel, strTargetName);

	return CR_TRUE;
}

MCommandResult GCmdHandler_MSG::OnMsgUnbanPlayerReq(MCommand* pCmd, MCommandHandler* pHandler)
{
	// Ŀ��� Ȯ��
	MUID uidChannel;
	wstring strTargetName;
	if (!pCmd->GetParameter(&uidChannel, 0, MPT_UID))	return CR_ERROR;
	if (!pCmd->GetParameter(strTargetName, 1, MPT_WSTR))	return CR_ERROR;
	if (uidChannel.IsInvalid())	return CR_ERROR;
	if (strTargetName.empty())	return CR_ERROR;

	// ���ڿ� �Ķ���� üũ
	if (strTargetName.size() > PLAYER_NAME_LEN) return CR_ERROR;

	// ��û�� Ȯ��
	MUID uidPlayer = pCmd->GetSenderUID();
	GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntityInWorld(uidPlayer);
	if (pPlayer == NULL)	return CR_TRUE;

	// �ۼ��� ����
	CID cidPlayer = pPlayer->GetCID();
	GServerChatChannelRouter* router = gsys.pAppServerFacade->GetChatChannelRouter();
	router->UnbanPlayerReq(cidPlayer, uidChannel, strTargetName);

	return CR_TRUE;
}

MCommandResult GCmdHandler_MSG::OnMsgChangeHostReq(MCommand* pCmd, MCommandHandler* pHandler)
{
	// Ŀ��� Ȯ��
	MUID uidChannel;
	wstring strTargetName;
	if (!pCmd->GetParameter(&uidChannel, 0, MPT_UID))	return CR_ERROR;
	if (!pCmd->GetParameter(strTargetName, 1, MPT_WSTR))	return CR_ERROR;
	if (uidChannel.IsInvalid())	return CR_ERROR;
	if (strTargetName.empty())	return CR_ERROR;

	// ���ڿ� �Ķ���� üũ
	if (strTargetName.size() > PLAYER_NAME_LEN) return CR_ERROR;

	// ��û�� Ȯ��
	MUID uidPlayer = pCmd->GetSenderUID();
	GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntityInWorld(uidPlayer);
	if (pPlayer == NULL)	return CR_TRUE;

	// �ۼ��� ����
	CID cidPlayer = pPlayer->GetCID();
	GServerChatChannelRouter* router = gsys.pAppServerFacade->GetChatChannelRouter();
	router->ChangeHostReq(cidPlayer, uidChannel, strTargetName);

	return CR_TRUE;
}
