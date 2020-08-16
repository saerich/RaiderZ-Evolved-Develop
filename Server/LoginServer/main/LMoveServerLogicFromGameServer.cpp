#include "StdAfx.h"
#include "LMoveServerLogicFromGameServer.h"
#include "LPlayerObject.h"
#include "LPlayerObjectManager.h"
#include "LGlobal.h"
#include "LPlayerAcceptor_FromGameServer.h"
#include "LPlayerAcceptManager.h"
#include "MLocale.h"
#include "LCmdRouter_MoveServer.h"
//#include "PmStatIndexParser.h"
//#include "PmRegionStat.h"
//#include "LPMSSystem.h"
#include "LCmdRouter_Player.h"


LMoveServerLogicFromGameServer::LMoveServerLogicFromGameServer(void)
{
}

LMoveServerLogicFromGameServer::~LMoveServerLogicFromGameServer(void)
{
}

bool LMoveServerLogicFromGameServer::MoveLoginServer(MUID uidPlayer, MUID uidConnectionKey, /*bool bOnPmang/*=false,*/ wstring strPmangStatIndex/*=L""*/, int nPCCafeID/* = 0*/)
{
	if (!Check(uidPlayer, uidConnectionKey)) return false;
	Apply(uidPlayer, uidConnectionKey);

	//if (bOnPmang)
	//	AddPmangStatIndex(uidPlayer, strPmangStatIndex, nPCCafeID);

	return true;
}

bool LMoveServerLogicFromGameServer::Check(MUID uidPlayer, MUID uidConnectionKey)
{
	LPlayerAcceptor_FromGameServer* pAcceptor = gmgr.pPlayerAcceptManager->FindAcceptor(uidConnectionKey);
	if (pAcceptor == NULL)
	{
		LCmdRouter_MoveServer::RouteResponseMoveLoginServer(uidPlayer, CR_FAIL_LOGIN, MUID::ZERO);
		return false;
	}

	return true;
}
void LMoveServerLogicFromGameServer::Apply(MUID uidPlayer, MUID uidConnectionKey)
{
	LPlayerAcceptor_FromGameServer* pAcceptor = gmgr.pPlayerAcceptManager->FindAcceptor(uidConnectionKey);
	if (pAcceptor == NULL) return;

	/// PlayerObject �߰�
	LPlayerObject* pNewPlayer = (LPlayerObject*)gmgr.pPlayerObjectManager->NewObject(uidPlayer);
	gmgr.pPlayerObjectManager->AddObject(pNewPlayer);

	/// ���� ���� ����
	ACCOUNT_INFO AccInfo;
	AccInfo.nAID	= pAcceptor->GetAID();
	AccInfo.bNewAcc = false;

	tstring tszName = MLocale::ConvUTF16ToTCHAR(pAcceptor->GetName().c_str());
	_tcsncpy_s(AccInfo.szUserID, tszName.c_str(), tszName.length());
	AccInfo.szUserID[tszName.length()] = 0;

	pNewPlayer->InitAccountInfo(AccInfo);

	/// ���� Acceptor ����
	gmgr.pPlayerAcceptManager->Del(uidConnectionKey);

	/// MasterServer �� ���� ���� �˸�
	LCmdRouter_Player::PostPlayerState_SELECT_CHAR(AccInfo.nAID, uidPlayer);

	/// Ŭ���̾�Ʈ�� ����
	LCmdRouter_MoveServer::RouteResponseMoveLoginServer(uidPlayer, CR_SUCCESS, uidPlayer);
}

//void LMoveServerLogicFromGameServer::AddPmangStatIndex(MUID uidPlayer, wstring strStatIndex, int nPCCafeID)
//{
//	LPlayerObject* pPlayerObject = gmgr.pPlayerObjectManager->GetPlayer(uidPlayer);
//	if (pPlayerObject == NULL)	return;
//
//	PmStatIndexParser parser(strStatIndex);
//
//	if (!parser.Parser())
//	{
//		return;
//	}
//
//	PmStatIndex statIndex = parser.GetStatIndex();
//	pPlayerObject->AddStatIndex(&statIndex, nPCCafeID);
//
//	PmRegionStat* pRegionStat = gsys.pPMS->GetRegionStat();
//
//	PmRegionCode regionCode = statIndex.nRegionCode;
//	PmAgeCode ageCode = statIndex.nAgeCode;
//	PmGenderCode genderCode = statIndex.nGenderCode;	
//
//	pRegionStat->AddUser(regionCode, ageCode, genderCode, nPCCafeID);
//}
