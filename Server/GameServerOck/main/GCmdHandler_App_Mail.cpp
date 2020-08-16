#include "stdafx.h"
#include "GCmdHandler_App_Mail.h"
#include "CCommandTable_App.h"
#include "CTransData.h"
#include "GPlayerObject.h"
#include "GGlobal.h"
#include "GPlayerObjectManager.h"
#include "GMailSystem.h"

GCmdHandler_App_Mail::GCmdHandler_App_Mail(MCommandCommunicator* pCC) : MCommandHandler(pCC)
{
	SetCmdHandler(MPC_MAIL_TRANS_NEW_MAIL,	OnTranNewMailReq);
}

MCommandResult GCmdHandler_App_Mail::OnTranNewMailReq(MCommand* pCommand, MCommandHandler* pHandler)
{
	// ���� Ȯ��
	CID nCID;
	TD_MAIL_MAILBOX_MAIL_INFO* pInfo;

	if (!pCommand->GetParameter(&nCID, 0, MPT_INT))	return CR_ERROR;
	if (!pCommand->GetSingleBlob(pInfo, 1))			return CR_ERROR;

	
	// ������ Ȯ��
	GPlayerObject* pPlayerObject = gmgr.pPlayerObjectManager->GetPlayer(nCID);

	if (pPlayerObject == NULL)
	{
		// AppServer�� �������� ��û�Ѵ�.
		gsys.pMailSystem->RetryTransNewMail(nCID, pInfo);
		return CR_TRUE;
	}

	
	// ���� ���°� �ƴ϶��(�����̵� ��), AppServer�� �������� �䱸�Ѵ�.
	if (!pPlayerObject->IsInWorld())
	{	
		gsys.pMailSystem->RetryTransNewMail(nCID, pInfo);
		return CR_TRUE;
	}		


	// ���� ���̸�, �뺸�Ѵ�.
	GEntityPlayer* pPlayer = pPlayerObject->GetEntity();
	gsys.pMailSystem->UpdateMailBox(pPlayer, pInfo);


	return CR_TRUE;
}
