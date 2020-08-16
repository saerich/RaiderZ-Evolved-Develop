#include "StdAfx.h"
#include "PChatModule_LeavePartyChannel.h"
#include "SDef.h"
#include "PGlobal.h"
#include "CCommandTable_App.h"
#include "PCmdRouter_Chat.h"
#include "PChatChannelManager.h"
#include "PChatChannel.h"
#include "PChatModule_LeaveChannel.h"


PChatModule_LeavePartyChannel::PChatModule_LeavePartyChannel(PChatChannel* pOwnerChannel)
: PChatModule_LeaveChannel(pOwnerChannel)
{
}

PChatModule_LeavePartyChannel::~PChatModule_LeavePartyChannel(void)
{
}

void PChatModule_LeavePartyChannel::OnRun(minet::MCommand* pCmd)
{
	// Ŀ��� Ȯ��
	VALID(MPC_MSG_LEAVE_CHANNEL_REQ == pCmd->GetID());

	CID cidRequestPlayer;
	MUID uidChannel;
	VALID(pCmd->GetParameter(&cidRequestPlayer,		0, MPT_INT));
	VALID(pCmd->GetParameter(&uidChannel,			1, MPT_UID));

	MUID uidGameServer = pCmd->GetSenderUID();
	Run(uidChannel, cidRequestPlayer, uidGameServer);
}

void PChatModule_LeavePartyChannel::Run(MUID uidChannel, CID cidPlayer, MUID uidGameServer)
{
	VALID(uidChannel == m_pOwnerChannel->GetUID());
	VALID(0 < cidPlayer);

	// ����
	if (false == PChatModule_LeaveChannel::Run(uidChannel, cidPlayer, uidGameServer)) return;

	// ä�ο� �÷��̾ �������� ���� ���, ���� ����
	if (m_pOwnerChannel->IsEmptyPlayer())
	{
		gmgr.pChatChannelManager->LazilyDeleteChannel(m_pOwnerChannel);
		return;
	}
}
