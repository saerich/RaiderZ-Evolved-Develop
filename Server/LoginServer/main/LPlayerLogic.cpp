#include "StdAfx.h"
#include "LPlayerLogic.h"
#include "LPlayerObject.h"
#include "LPlayerObjectManager.h"
#include "LGlobal.h"
#include "LCommandCenter.h"
#include "SCharacterInfoChecker.h"
#include "CSNameStringChecker.h"
#include "LCmdRouter_Player.h"
#include "SCmdRouter_Login.h"
#include "LDBManager.h"
#include "LMasterClient.h"
#include "LServer.h"
//#include "PmDef.h"
//#include "PmRegionStat.h"
//#include "LPMSSystem.h"
#include "LDBParamData.h"
#include "LPlayerLoginGameServerProcManager.h"
#include "CSBasicAttributeCalculator.h"
#include "LPlayerAcceptManager.h"
#include "LPlayerAcceptor_FromGameServer.h"
#include "LTimeoutManager.h"
#include "CSDef_Fatigue.h"


bool LPlayerLogic::RequestAccountCharList(MUID uidPlayer)
{
	LPlayerObject* pPlayer = gmgr.pPlayerObjectManager->GetPlayer(uidPlayer);
	if (pPlayer == NULL) return false;
	if (pPlayer->GetState() != PlayerObjectState::POS_SELECT_CHAR) return false;

	ACCOUNT_INFO& accountInfo = pPlayer->GetAccountInfo();
	int64 nAID = accountInfo.nAID;

	if (!gsys.pDBManager->GetAccountCharList(uidPlayer, nAID))
		return false;

	return true;
}

bool LPlayerLogic::InsertCharacter(MUID uidPlayer, TD_INSERT_CHARACTER_INFO* pInsertCharInfo)
{
	// ��û�� Ȯ��
	LPlayerObject* pPlayer = gmgr.pPlayerObjectManager->GetPlayer(uidPlayer);
	if (pPlayer == NULL) return false;
	if (pPlayer->GetState() != PlayerObjectState::POS_SELECT_CHAR) return false;
	
	
	// ���ڰ� ��ȿ�� �˻�
	if (!CheckInsertCharacter_Account(pPlayer))	return false;
	if (!CheckInsertCharacter_Parameter(uidPlayer, pInsertCharInfo))	return false;
	
	
	// ĳ���� �߰�
	ACCOUNT_INFO& accountInfo = pPlayer->GetAccountInfo();
	int64 nAID = accountInfo.nAID;
	
	if (!ApplyInsertCharacter(uidPlayer, nAID, pInsertCharInfo))		return false;
	
	
	return true;
}

bool LPlayerLogic::DeleteCharacter(MUID uidPlayer, int8 nCharIndex)
{
	LPlayerObject* pPlayer = gmgr.pPlayerObjectManager->GetPlayer(uidPlayer);
	if (pPlayer == NULL) return false;
	if (pPlayer->GetState() != PlayerObjectState::POS_SELECT_CHAR) return false;

	SAccountCharFieldInfo* pCharFieldInfo = pPlayer->GetCharFieldInfo(nCharIndex);
	if (pCharFieldInfo == NULL)	return false;

	return gsys.pDBManager->DeleteCharacter(uidPlayer, pPlayer->GetAccountInfo().nAID, pCharFieldInfo->nCID, nCharIndex);
}

bool LPlayerLogic::SelMyChar(MUID uidPlayer, int8 nCharIndex)
{
	// ��û �÷��̾� ���� Ȯ��
	LPlayerObject* pPlayer = gmgr.pPlayerObjectManager->GetPlayer(uidPlayer);
	if (pPlayer == NULL) return false;
	if (PlayerObjectState::POS_SELECT_CHAR != pPlayer->GetState() &&
		PlayerObjectState::POS_LOGIN_GAME_SERVER != pPlayer->GetState())	///< �ڵ� ��õ��� �� ����.
		return false;

	SCmdRouter_Login cmdRouter(gsys.pCommandCenter);

	// ĳ���� �ε����� ��ȿ���� Ȯ��
	SAccountCharFieldInfo* pCharFieldInfo = pPlayer->GetCharFieldInfo(nCharIndex);
	if (NULL == pCharFieldInfo)
	{
		cmdRouter.ResponseSelMyChar(pPlayer->GetUID(), CR_FAIL_LOGIN_INVALID_CHAR_INFO);
		return false;
	}

	// ���� ĳ���� ���Ӽ��� �α��� ��û Queuing.
	pPlayer->OnLoginGameServer(nCharIndex);
	gsys.pServer->GetPlayerLoginGameServerProcManager()->Add(uidPlayer, nCharIndex);

	return true;
}

void LPlayerLogic::DuplicatedPlayerLogin(MUID uidPlayer, MUID uidConnectionKey)
{
	LPlayerObject* pPlayer = gmgr.pPlayerObjectManager->GetPlayer(uidPlayer);
	if (pPlayer != NULL)
	{
		SCmdRouter_Login router(gsys.pCommandCenter);
		router.DuplicatedPlayerLogin(uidPlayer);

		/// Ŭ���̾�Ʈ���� ���� ����.
		gsys.pServer->HardDisconnectPlayer(uidPlayer);

		return;
	}

	// �����̵� ���� ��� Acceptor �� ���� �� �ִ�.
	LPlayerAcceptor_FromGameServer* pAcceptor = gmgr.pPlayerAcceptManager->FindAcceptor(uidConnectionKey);
	if (pAcceptor != NULL)
	{
		LCmdRouter_Player::PostDeletePlayer(pAcceptor->GetAID(), pAcceptor->GetUID());

		gmgr.pPlayerAcceptManager->Del(uidConnectionKey);

		return;
	}
}

void LPlayerLogic::OnDisconnectPlayer(MUID uidPlayer)
{
	LPlayerObject* pPlayer = gmgr.pPlayerObjectManager->GetPlayer(uidPlayer);
	if (pPlayer == NULL) return;

	if (PlayerObjectState::POS_MOVE_SERVER == pPlayer->GetState())
	{
		/// ���� �̵� Timeout ����
		gmgr.pTimeoutManager->StopWatchingMoveServerTimeout(uidPlayer);
	}
	else
	{
		/// ���� �̵����� ���� ���� ���ᰡ �ƴ� ���, �÷��̾� ���� ����.
		LCmdRouter_Player::PostDeletePlayer(pPlayer->GetAccountInfo().nAID, pPlayer->GetUID());
	}
	
	/// Object ����
	//if (pPlayer->IsExistStatIndex())
	//{
	//	PmRegionStat* pRegionStat = gsys.pPMS->GetRegionStat();
	//	PmStatIndex* pStatIndex = pPlayer->GetStatIndex();
	//
	//	PmRegionCode regionCode = pStatIndex->nRegionCode;
	//	PmAgeCode ageCode = pStatIndex->nAgeCode;
	//	PmGenderCode genderCode = pStatIndex->nGenderCode;
	//	int nPCCafeID = pPlayer->GetPCCafeID();
	//
	//	pRegionStat->RemoveUser(regionCode, ageCode, genderCode, nPCCafeID);
	//}

	gmgr.pPlayerObjectManager->DeleteObject(uidPlayer);
}

bool LPlayerLogic::CheckInsertCharacter_Account(LPlayerObject* pPlayerObject)
{
	int nCharCount = pPlayerObject->GetCharFieldInfoCount();

	SCharacterInfoChecker checker;	

	if (!checker.IsRemainCharSlot(nCharCount))
	{		
		MUID uidPlayer = pPlayerObject->GetUID();

		SCmdRouter_Login cmdRouter(gsys.pCommandCenter);
		cmdRouter.ResponseInsertChar(uidPlayer, CR_FAIL_CHAR_LIMIT_CHAR_SLOT_COUNT);
		
		return false;
	}

	return true;
}

bool LPlayerLogic::CheckInsertCharacter_Parameter(MUID uidPlayer, TD_INSERT_CHARACTER_INFO* pInsertCharInfo)
{
	/// ���� üũ
	SCharacterInfoChecker checker;
	SCmdRouter_Login cmdRouter(gsys.pCommandCenter);

	// ���� �̸��� ��ȿ���� Ȯ��
	CSNameStringChecker illegalword_checker(pInsertCharInfo->szName);
	CSNameStringChecker::ERROR_CODE nErrorCode = illegalword_checker.Check();
	if (CSNameStringChecker::OK != nErrorCode)
	{
		cmdRouter.ResponseInsertChar(uidPlayer, nErrorCode);
		return false;
	}

	// �Ķ���� Ȯ��
	SEX sex = (pInsertCharInfo->nSex == SEX_FEMALE ? SEX_FEMALE : SEX_MALE);
	
	if (false == checker.CheckAllParam_InsertCharacter(*pInsertCharInfo))
	{
		mlog3("Invalid Param - InsertCharacter(Name:%s, Sex:%d, Hair:%d, Face:%d, Voice:%d, HairColor:%d, SkinColor:%d, EyeColor:%d, TalentStyle:%d, MakeUp:%d, TattoType:%d, TattoScale:%d)\n"
			, pInsertCharInfo->szName, (int)pInsertCharInfo->nSex, (int)pInsertCharInfo->nHair, (int)pInsertCharInfo->nFace, (int)pInsertCharInfo->nVoice
			, (int)pInsertCharInfo->nHairColor, (int)pInsertCharInfo->nSkinColor, pInsertCharInfo->nEyeColor
			, pInsertCharInfo->nMakeUp, pInsertCharInfo->nTattooType, pInsertCharInfo->nTattooPosX, pInsertCharInfo->nTattooPosY, pInsertCharInfo->nTattooScale);

		cmdRouter.ResponseInsertChar(uidPlayer, CR_FAIL_CHAR_INVALID_PARAMETER);
		return false;
	}

	return true;
}

bool LPlayerLogic::ApplyInsertCharacter(MUID uidPlayer, AID nAID, TD_INSERT_CHARACTER_INFO* pInsertCharInfo)
{
	DBP_CHAR_INSERT param;
	param.uidPlayer = uidPlayer;
	param.nAID = nAID;
	param.Assign(pInsertCharInfo);
	
	// Level, XP, Silver �⺻��
	param.nLevel = 1;
	param.nXP = 0;
	param.nSilver = 0;

	// HP, EN, STA, TP �⺻��
	param.nTotalTP = PLAYER_DEFAULT_TP;
	param.nHP = CSBasicAttributeCalculator::CalcMaxHP(param.nLevel, PLAYER_DEFAULT_CON);
	param.nEN = CSBasicAttributeCalculator::CalcMaxEN(param.nLevel, PLAYER_DEFAULT_INT);
	param.nSTA = CSBasicAttributeCalculator::CalcMaxSP(param.nLevel, PLAYER_DEFAULT_DEX);


	param.nFatigue = NORMAL_FATIGUE;
	param.nSoulBindingID = PLAYER_DEFAULT_SOULBINDINGID;
	param.nInnRoomID = PLAYER_DEFAULT_INNROOMID;
	param.nPrimaryPalettedID = PLAYER_DEFAULT_PRIPALETTE;
	param.nSecondaryPalettedID = PLAYER_DEFAULT_SECPALETTE;

	if (!gsys.pDBManager->InsertCharacter(param))
	{
		SCmdRouter_Login cmdRouter(gsys.pCommandCenter);
		cmdRouter.ResponseInsertChar(uidPlayer, CR_FAIL_CHAR_POST_INSERT_DB);

		return false;
	}

	return true;
}