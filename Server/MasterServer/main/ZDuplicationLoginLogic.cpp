#include "StdAfx.h"
#include "ZDuplicationLoginLogic.h"
#include "ZPlayerManager.h"
#include "ZDuplicationLoginManager.h"
#include "ZGameServerObject.h"
#include "ZGameServerObjectManager.h"
#include "ZLoginServerFacade.h"
#include "ZPlayerCommandRouter.h"
#include "ZPlayerLogic.h"


ZDuplicationLoginLogic::ZDuplicationLoginLogic(void)
{
}

ZDuplicationLoginLogic::~ZDuplicationLoginLogic(void)
{
}

bool ZDuplicationLoginLogic::IsDuplicated(AID nAID)
{
	if (NULL == gmgr.pPlayerManager->Find(nAID)) return false;
	return true;
}

void ZDuplicationLoginLogic::StartKickInWorldPlayer(MUID uidPlayer, AID nAID, const wstring& strUserID/*, PmUserData* pPmUserData*/)
{
	// ������ �ߺ����� ó�� ������� �÷��̾ �ִٸ� ���i�´�.
	if (gmgr.pDuplicationLoginManager->IsExist(nAID))
	{
		this->KickWaitingDupLoginPlayer(nAID);
	}

	ZPlayer* pExistPlayer = gmgr.pPlayerManager->Find(nAID);
	if (NULL == pExistPlayer)
	{
		mlog("Error! ZPlayerLogic::KickDuplicatedLoginPlayer(), pExistPlayer(AID: %d) is Not Exist!\n", nAID);
		return;
	}

	// �ߺ� �α��� ó�� ��� �÷��̾�� ���.
	gmgr.pDuplicationLoginManager->Add(nAID, strUserID, uidPlayer/*, pPmUserData*/);

	// ������ �������ִ� �÷��̾ �������� �뺸.
	RouteKickDuplicatedPlayer(pExistPlayer);

	// �÷��̾� ���� ����
	pExistPlayer->SetState(PS_KICK_BY_DUPLOGIN);
}

void ZDuplicationLoginLogic::KickWaitingDupLoginPlayer(AID nAID)
{
	ZDUPLICATION_LOGIN_INFO* pDuplicationInfo = gmgr.pDuplicationLoginManager->Get(nAID);
	if (pDuplicationInfo == NULL)
	{
		mlog("Error! ZDuplicationLoginLogic::KickWaitingDupLoginPlayer(), pDuplicationInfo(AID: %d) is Not Exist!\n", nAID);
		return;
	}

	MUID uidLoginServer = gmgr.pLoginServerFacade->GetUID();
	ZPlayerCommandRouter::SendDuplicatedPlayerLogin_ToLoginServer(uidLoginServer, pDuplicationInfo->uidPlayer);

	gmgr.pDuplicationLoginManager->Del(nAID);
}

void ZDuplicationLoginLogic::RouteKickDuplicatedPlayer(ZPlayer* pKickPlayer)
{
	RVALID(pKickPlayer != NULL);

	if (PS_SELECT_CHAR == pKickPlayer->GetState() ||
		PS_READY_LOGIN_GAMESERVER == pKickPlayer->GetState() ||
		PS_READY_MOVE_LOGINSERVER == pKickPlayer->GetState() ||
		PS_MOVE_LOGINSERVER == pKickPlayer->GetState())
	{
		MUID uidLoginServer = gmgr.pLoginServerFacade->GetUID();
		ZPlayerCommandRouter::SendDuplicatedPlayerLogin_ToLoginServer(uidLoginServer, pKickPlayer->GetUID(), pKickPlayer->GetReservedConnectionKey());
	}

	ZGameServerObject* pCurGameServer = gmgr.pGameServerObjectManager->FindGameServer(pKickPlayer->GetGameServerID());
	if (pCurGameServer)
		ZPlayerCommandRouter::SendDuplicatedPlayerLogin_ToGameServer(pCurGameServer->GetUID(), pKickPlayer->GetUID(), pKickPlayer->GetReservedConnectionKey());

	ZGameServerObject* pReservedGameServer = gmgr.pGameServerObjectManager->FindGameServer(pKickPlayer->GetReservedGameServerID());
	if (pReservedGameServer)
		ZPlayerCommandRouter::SendDuplicatedPlayerLogin_ToGameServer(pReservedGameServer->GetUID(), pKickPlayer->GetUID(), pKickPlayer->GetReservedConnectionKey());
}

bool ZDuplicationLoginLogic::ExistWaitingPlayer(AID nAID)
{
	return gmgr.pDuplicationLoginManager->IsExist(nAID);
}

void ZDuplicationLoginLogic::FlushDuplicatedLogin(AID nAID, const MUID& uidLeavePlayer)
{
	ZDUPLICATION_LOGIN_INFO* pDuplicationLiginInfo = gmgr.pDuplicationLoginManager->Get(nAID);
	if (NULL == pDuplicationLiginInfo) return;

	// ���� �÷��̾ �ߺ� �α��� ó���� ��ٸ��� �÷��̾�� ���.
	if (pDuplicationLiginInfo->uidPlayer == uidLeavePlayer)
	{
		gmgr.pDuplicationLoginManager->Del(nAID);
		return;
	}

	// �Ǹ� �����Ͱ� ������ ����.
	//PmUserData* pPmUserData = NULL;
	//if (pDuplicationLiginInfo->bExistPmUserData)
	//	pPmUserData = &pDuplicationLiginInfo->pmUserData;
 	
	// ���� �÷��̾� ���� / �� �÷��̾� �߰�
	ZPlayerLogic playerLogic;
	playerLogic.DeletePlayer(nAID);
	if (!playerLogic.AddPlayer(pDuplicationLiginInfo->uidPlayer, nAID, pDuplicationLiginInfo->strUserID/*, pPmUserData*/))
	{
		mlog("Error! ZPlayerLogic::FlushDuplicatedLogin(), Failed AddPlayer. UID(%u,%u) , AID(%d)\n"
			, pDuplicationLiginInfo->uidPlayer.High, pDuplicationLiginInfo->uidPlayer.Low, nAID);

		gmgr.pDuplicationLoginManager->Del(nAID);
		return;
	}
	
	MUID uidLoginServer = gmgr.pLoginServerFacade->GetUID();
	ZPlayerCommandRouter::SendAddPlayerResponse(uidLoginServer, pDuplicationLiginInfo->uidPlayer, CR_SUCCESS);
	gmgr.pDuplicationLoginManager->Del(nAID);
}