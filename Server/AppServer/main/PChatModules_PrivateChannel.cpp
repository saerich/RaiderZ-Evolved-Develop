#include "StdAfx.h"
#include "PChatModules_PrivateChannel.h"
#include "PChatChannel.h"
#include "CCommandTable_App.h"
#include "MCommandParameter.h"
#include "PGlobal.h"
#include "PChatChannelManager.h"
#include "PCmdRouter_Chat.h"
#include "PMsgCommandFacade.h"
#include "PServer.h"
#include "SProxyPlayerManager.h"
#include "SProxyPlayer.h"


//////////////////////////////////////////////////////////////////////////

bool PChatModuleBase_PrivateChannel::CheckConditions_ChannelType_n_Owner_n_TargetNameLen(CID cidRequestPlayer, wstring& strTargetName)
{
	/// üũ : ���� ä���� �缳 ä������
	if (CCT_PRIVATE != m_pOwnerChannel->GetType())
	{
		gsys.pMsgCommandFacade->RouteSystemMsg(CR_FAIL_MSG_NOT_PRIVATE_CHANNEL, cidRequestPlayer);
		return false;
	}
	/// üũ : ��û�ڰ� ��������
	if (cidRequestPlayer != m_pOwnerChannel->GetOwnerCID())
	{
		gsys.pMsgCommandFacade->RouteSystemMsg(CR_FAIL_MSG_NOT_HOST, cidRequestPlayer);
		return false;
	}
	/// üũ : ����� �̸��� �ʹ� ���� ������
	if (strTargetName.size() > PLAYER_NAME_LEN)
	{
		gsys.pMsgCommandFacade->RouteSystemMsg(CR_FAIL_MSG_WRONG_TARGET, cidRequestPlayer);
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////

void PChatModule_Kick::OnRun(minet::MCommand* pCmd)
{
	RVALID(MPC_MSG_KICK_REQ == pCmd->GetID());

	CID cidRequestPlayer;
	MUID uidChannel;
	wstring strTargetPlayerName;
	RVALID(pCmd->GetParameter(&cidRequestPlayer,	0, MPT_INT));
	RVALID(pCmd->GetParameter(&uidChannel,			1, MPT_UID));
	RVALID(pCmd->GetParameter(strTargetPlayerName,	2, MPT_WSTR));
	
	RVALID(uidChannel == m_pOwnerChannel->GetUID());

	/// üũ : ���� ä���� �缳 ä������
	/// üũ : ��û�ڰ� ��������
	/// üũ : ����� �̸��� �ʹ� ���� ������
	if (false == CheckConditions_ChannelType_n_Owner_n_TargetNameLen(cidRequestPlayer, strTargetPlayerName))
		return;

	/// üũ : ����� ä�ο� �����ϴ���
	SProxyPlayer* pTargetPlayer = gsys.pServer->GetPlayerManager()->FindPlayer(strTargetPlayerName);
	if (NULL == pTargetPlayer ||
		false == m_pOwnerChannel->IsExistPlayer(pTargetPlayer->GetCID()))
	{
		gsys.pMsgCommandFacade->RouteSystemMsg(CR_FAIL_MSG_NOT_EXIST_PLAYER, cidRequestPlayer);
		return;
	}
	/// üũ : ����� ��û�� �ڽ�(����)�� �ƴ���
	if (pTargetPlayer->GetCID() == cidRequestPlayer)
	{
		gsys.pMsgCommandFacade->RouteSystemMsg(CR_FAIL_MSG_WRONG_TARGET, cidRequestPlayer);
		return;
	}

	/// ���� ó��
	gmgr.pChatChannelManager->LeaveChannel(m_pOwnerChannel, pTargetPlayer->GetCID());

	/// Kick Ŀ�ǵ� ����
	PCmdRouter_Chat router;
	router.PostKick(pCmd->GetSenderUID(), pTargetPlayer->GetCID(), uidChannel);
	router.RouteKickPlayerInfo(m_pOwnerChannel, strTargetPlayerName);
}

//////////////////////////////////////////////////////////////////////////

void PChatModule_Ban::OnRun(minet::MCommand* pCmd)
{
	RVALID(MPC_MSG_BAN_REQ == pCmd->GetID());

	CID cidRequestPlayer;
	MUID uidChatChannel;
	wstring strTargetPlayerName;
	RVALID(pCmd->GetParameter(&cidRequestPlayer,	0, MPT_INT));
	RVALID(pCmd->GetParameter(&uidChatChannel,		1, MPT_UID));
	RVALID(pCmd->GetParameter(strTargetPlayerName,	2, MPT_WSTR));

	RVALID(uidChatChannel == m_pOwnerChannel->GetUID());

	/// üũ : ���� ä���� �缳 ä������
	/// üũ : ��û�ڰ� ��������
	/// üũ : ����� �̸��� �ʹ� ���� ������
	if (false == CheckConditions_ChannelType_n_Owner_n_TargetNameLen(cidRequestPlayer, strTargetPlayerName))
		return;

	/// üũ : ����� ä�ο� �����ϴ���
	SProxyPlayer* pTargetPlayer = gsys.pServer->GetPlayerManager()->FindPlayer(strTargetPlayerName);
	if (NULL == pTargetPlayer ||
		false == m_pOwnerChannel->IsExistPlayer(pTargetPlayer->GetCID()))
	{
		gsys.pMsgCommandFacade->RouteSystemMsg(CR_FAIL_MSG_NOT_EXIST_PLAYER, cidRequestPlayer);
		return;
	}
	/// üũ : ����� ��û�� �ڽ�(����)�� �ƴ���
	if (pTargetPlayer->GetCID() == cidRequestPlayer)
	{
		gsys.pMsgCommandFacade->RouteSystemMsg(CR_FAIL_MSG_WRONG_TARGET, cidRequestPlayer);
		return;
	}

	/// Ban ó��
	m_pOwnerChannel->AddBanPlayer(strTargetPlayerName);
	gmgr.pChatChannelManager->LeaveChannel(m_pOwnerChannel, pTargetPlayer->GetCID());

	/// Ban Ŀ�ǵ� ����
	PCmdRouter_Chat router;
	router.PostBan(pCmd->GetSenderUID(), pTargetPlayer->GetCID(), uidChatChannel);
	router.RouteBanPlayerInfo(m_pOwnerChannel, strTargetPlayerName);
}

void PChatModule_Unban::OnRun(minet::MCommand* pCmd)
{
	RVALID(MPC_MSG_UNBAN_REQ == pCmd->GetID());

	CID cidRequestPlayer;
	MUID uidChatChannel;
	wstring strTargetPlayerName;
	RVALID(pCmd->GetParameter(&cidRequestPlayer,	0, MPT_INT));
	RVALID(pCmd->GetParameter(&uidChatChannel,		1, MPT_UID));
	RVALID(pCmd->GetParameter(strTargetPlayerName,	2, MPT_WSTR));

	RVALID(uidChatChannel == m_pOwnerChannel->GetUID());

	/// üũ : ���� ä���� �缳 ä������
	/// üũ : ��û�ڰ� ��������
	/// üũ : ����� �̸��� �ʹ� ���� ������
	if (false == CheckConditions_ChannelType_n_Owner_n_TargetNameLen(cidRequestPlayer, strTargetPlayerName))
		return;

	/// üũ : ����� �� �ߴ� �÷��̾�����
	if (false == m_pOwnerChannel->IsBanedPlayer(strTargetPlayerName))
	{
		gsys.pMsgCommandFacade->RouteSystemMsg(CR_FAIL_MSG_NOT_BANED, cidRequestPlayer);
		return;
	}
	/// üũ : ����� ��û�� �ڽ�(����)�� �ƴ���
	SProxyPlayer* pRequester = gsys.pServer->GetPlayerManager()->FindPlayer(cidRequestPlayer);
	RVALID(pRequester);
	if (strTargetPlayerName == pRequester->GetName())
	{
		gsys.pMsgCommandFacade->RouteSystemMsg(CR_FAIL_MSG_WRONG_TARGET, cidRequestPlayer);
		return;
	}

	/// Unban ó��
	m_pOwnerChannel->RemoveBanPlayer(strTargetPlayerName);

	/// Unban ���� Ŀ�ǵ� ����
	PCmdRouter_Chat router;
	router.PostUnbanRes(pCmd->GetSenderUID(), cidRequestPlayer, uidChatChannel, strTargetPlayerName);
}

void PChatModule_ChangeHost::OnRun(minet::MCommand* pCmd)
{
	RVALID(MPC_MSG_CHANGE_HOST_REQ == pCmd->GetID());

	CID cidRequestPlayer;
	MUID uidChatChannel;
	wstring strTargetPlayerName;
	RVALID(pCmd->GetParameter(&cidRequestPlayer,	0, MPT_INT));
	RVALID(pCmd->GetParameter(&uidChatChannel,		1, MPT_UID));
	RVALID(pCmd->GetParameter(strTargetPlayerName,	2, MPT_WSTR));

	Run(pCmd->GetSenderUID(), uidChatChannel, cidRequestPlayer, strTargetPlayerName);
}
void PChatModule_ChangeHost::Run(const MUID& uidGameServer, const MUID& uidChatChannel, CID cidRequestPlayer, std::wstring& strTargetPlayerName)
{
	RVALID(uidChatChannel == m_pOwnerChannel->GetUID());

	/// üũ : ���� ä���� �缳 ä������
	/// üũ : ��û�ڰ� ��������
	/// üũ : ����� �̸��� �ʹ� ���� ������
	if (false == CheckConditions_ChannelType_n_Owner_n_TargetNameLen(cidRequestPlayer, strTargetPlayerName))
		return;

	/// üũ : ����� ä�ο� �����ϴ���
	SProxyPlayer* pTargetPlayer = gsys.pServer->GetPlayerManager()->FindPlayer(strTargetPlayerName);
	if (NULL == pTargetPlayer ||
		false == m_pOwnerChannel->IsExistPlayer(pTargetPlayer->GetCID()))
	{
		gsys.pMsgCommandFacade->RouteSystemMsg(CR_FAIL_MSG_NOT_EXIST_PLAYER, cidRequestPlayer);
		return;
	}
	/// üũ : ����� ��û�� �ڽ�(����)�� �ƴ���
	if (pTargetPlayer->GetCID() == cidRequestPlayer)
	{
		gsys.pMsgCommandFacade->RouteSystemMsg(CR_FAIL_MSG_WRONG_TARGET, cidRequestPlayer);
		return;
	}
	
	m_pOwnerChannel->SetOwnerCID(pTargetPlayer->GetCID());

	PCmdRouter_Chat router;
	router.RouteChangeHostInfo(m_pOwnerChannel, strTargetPlayerName);
}

void PChatModule_Invite::OnRun(minet::MCommand* pCmd)
{
	/// TODO : �缳 ä�� ä�ο��� ����� ��� ������ �ۼ� �� - aibeast, 2010.01.11
}

void PChatModule_ChannelInfo::OnRun(minet::MCommand* pCmd)
{
	/// TODO : �缳 ä�� ä�ο��� ����� ��� ������ �ۼ� �� - aibeast, 2010.01.11
}
