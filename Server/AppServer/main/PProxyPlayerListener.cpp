#include "stdafx.h"
#include "PProxyPlayerListener.h"
#include "SProxyPlayer.h"
#include "PChattingLogic.h"
#include "STransData.h"

void PProxyPlayerListener::OnAddComplate(const SProxyPlayer* pPlayer)
{
	PChattingLogic logic;

	CID cid = pPlayer->GetCID();
	int nFieldID = pPlayer->GetFieldID();
	MUID uidParty = pPlayer->GetPartyUID();
	int nGameServerID = pPlayer->GetGameServerID();

	// �ŷ�ä�� ����
	logic.EnterAuctionChannel(cid, nGameServerID);
		
	// �ʵ�ä�� ����
	logic.EnterFieldChannel(cid, nGameServerID, nFieldID);

	// ��Ƽä�� ����
	if (uidParty.IsValid())
	{
		logic.EnterPartyChannel(cid, nGameServerID, uidParty);
	}
}

void PProxyPlayerListener::OnPreRemove(const SProxyPlayer* pPlayer)
{	
	PChattingLogic logic;
	
	// ��ä�� ����
	CID cid = pPlayer->GetCID();
	logic.LeaveAllChannel(cid);
}

void PProxyPlayerListener::OnPreUpdate(const SProxyPlayer* pPlayer, const TD_PROXY_PLAYER_INFO& info)
{
	PChattingLogic logic;

	CID cid = pPlayer->GetCID();
	int nFieldID = pPlayer->GetFieldID();
	MUID uidParty = pPlayer->GetPartyUID();
	int nGameServerID = pPlayer->GetGameServerID();

	// ���Ӽ��� ����
	if (nGameServerID != info.nGameServerID)
	{
		logic.ChangedGameServerID(cid, info.nGameServerID);
	}

	// �ʵ�ä�� ����
	if (nFieldID != info.nFieldID)
	{
		logic.LeaveFieldChannel(cid, nGameServerID, nFieldID);
		logic.EnterFieldChannel(cid, info.nGameServerID, info.nFieldID);
	}

	// ��Ƽä�� ����
	if (uidParty != info.uidParty)
	{
		if (uidParty.IsValid())
		{
			logic.LeavePartyChannel(cid, nGameServerID, uidParty);
		}

		if (info.uidParty.IsValid())
		{
			logic.EnterPartyChannel(cid, info.nGameServerID, info.uidParty);
		}
	}
}
