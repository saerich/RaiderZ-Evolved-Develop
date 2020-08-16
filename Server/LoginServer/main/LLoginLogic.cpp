#include "stdafx.h"
#include "LLoginLogic.h"
#include "LPlayerObject.h"
#include "LPlayerObjectManager.h"
#include "SDef.h"
#include "LConfig.h"
#include "LGlobal.h"
#include "LDBManager.h"
#include "LCommandCenter.h"
#include "CCommandTable_Login.h"
#include "CTransData.h"
//#include "PmLoginFacade.h"
//#include "PmDef.h"
#include "LDBTaskDataAccount.h"
#include "MLocale.h"


LLoginLogic::LLoginLogic()
{
}

bool LLoginLogic::Login(MUID uidPlayer, wstring strUserID, wstring strPassword)
{
	AddPlayerObject(uidPlayer);

	LDBT_ACC_LOGIN data(uidPlayer, strUserID, L"", strUserID, strPassword, L"testing");

	bool bRet = gsys.pDBManager->LoginGetInfo(data);

	if (!bRet)
	{
		DelPlayerObject(uidPlayer);
	}

	return bRet;
}

//bool LLoginLogic::PmangLogin(MUID uidPlayer, wstring strLoginStream)
//{
//	// 피망 로그인 검사
//	PmLoginFacade loginFacade(MLocale::ConvUTF16ToTCHAR(LConfig::m_strPmangServerKey.c_str()).c_str(), LConfig::m_nPmangToleranceSeconds);
//
//	PmUserData userData;
//	if (!loginFacade.Login(W2CT_EX(strLoginStream.c_str(), 0), &userData))
//	{
//		return false;
//	}
//
//	AddPlayerObject(uidPlayer);
//
//	// 로그인 요청
//	int nWorldID = LConfig::m_nWorldID;
//	wstring strUserID = T2CW_EX(userData.strUserID.c_str(), 0);
//
//	bool bRet =  gsys.pDBManager->PmangLoginGetInfo(uidPlayer, strUserID, userData);
//
//	if (!bRet)
//	{
//		DelPlayerObject(uidPlayer);
//	}
//
//	return bRet;
//}

void LLoginLogic::AddPlayerObject( MUID uidPlayer )
{
	LPlayerObject* pNewPlayer = (LPlayerObject*)gmgr.pPlayerObjectManager->NewObject(uidPlayer);
	gmgr.pPlayerObjectManager->AddObject(pNewPlayer);
}

void LLoginLogic::DelPlayerObject( MUID uidPlayer )
{
	gmgr.pPlayerObjectManager->DeleteObject(uidPlayer);
}