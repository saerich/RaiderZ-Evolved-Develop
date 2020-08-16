#include "StdAfx.h"
#include "LCmdHandler_NotServable.h"
#include "LCommandTable.h"
#include "SCmdRouter_Login.h"
#include "LGlobal.h"
#include "LCommandCenter.h"


LCmdHandler_NotServable::LCmdHandler_NotServable(MCommandCommunicator* pCC) : MCommandHandler(pCC)
{
	SetCmdHandler(MC_COMM_REQUEST_LOGIN,				OnLogin);
	//SetCmdHandler(MC_COMM_REQUEST_LOGIN_ON_PMANG,		OnPmangLogin);

}

MCommandResult LCmdHandler_NotServable::OnLogin(MCommand* pCommand, MCommandHandler* pHandler)
{
	int nCommandVersion;
	if (!pCommand->GetParameter(&nCommandVersion,	2,	MPT_INT))	return CR_ERROR;

	MUID uidPlayer = pCommand->GetSenderUID();

	if (!_Login(uidPlayer, nCommandVersion))
		return CR_ERROR;


	return CR_TRUE;
}

//MCommandResult LCmdHandler_NotServable::OnPmangLogin(MCommand* pCommand, MCommandHandler* pHandler)
//{
//	// Ŀ�ǵ� Ȯ��
//	int nCommandVersion;
//	if (!pCommand->GetParameter(&nCommandVersion,	0,	MPT_INT))	return CR_ERROR;
//
//	MUID uidPlayer = pCommand->GetSenderUID();
//
//	if (!_Login(uidPlayer, nCommandVersion))
//		return CR_ERROR;
//
//	return CR_TRUE;
//}

bool LCmdHandler_NotServable::_Login(MUID uidPlayer, int nCommandVersion)
{
	// ��û�� Ȯ��
	if (uidPlayer.IsInvalid()) return false;

	SCmdRouter_Login cmdRouter(gsys.pCommandCenter);

	// Ŀ�ǵ� ���� Ȯ��
	if (!CCommandVersionChecker::IsValid(nCommandVersion))
	{
		//cmdRouter.ResponsePmangLogin(uidPlayer, CR_FAIL_LOGIN_COMMAND_INVALID_VERSION);		
		return true;
	}

	// ��� ���� Ŀ�ǵ� ����
	cmdRouter.ResponseLogin(uidPlayer, CR_FAIL_LOGIN_SERVERS_ARE_STARTING);
	return true;
}