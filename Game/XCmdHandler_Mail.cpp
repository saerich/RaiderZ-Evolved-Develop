#include "StdAfx.h"
#include "XCmdHandler_Mail.h"
#include "CSItemHelper.h"
#include "XMyPlayer.h"
#include "XInteractionInfo.h"
#include "XPost_Mail.h"
#include "XMail.h"
#include "CCommandResultTable.h"


XCmdHandler_Mail::XCmdHandler_Mail(MCommandCommunicator* pCC) : MCommandHandler(pCC)
{
	SetCmdHandler(MC_MAIL_MAILBOX_IS_OPENED,			OnMailboxIsOpened);
	SetCmdHandler(MC_MAIL_MAILBOX_IS_CLOSED,			OnMailboxIsClosed);
	SetCmdHandler(MC_MAIL_POST_RES,						OnPostRes);
	SetCmdHandler(MC_MAIL_READ_RES,						OnReadRes);
	SetCmdHandler(MC_MAIL_DELETE_RES,					OnDeleteRes);
	SetCmdHandler(MC_MAIL_TAKE_APPENDED_ITEM_RES,		OnTakeAppendedItemRes);
	SetCmdHandler(MC_MAIL_TAKE_APPENDED_MONEY_RES,		OnTakeAppendedMoneyRes);
	SetCmdHandler(MC_MAIL_APPEND_PUTUP_ITEM_RES,		OnAppendPutUpItemRes);
	SetCmdHandler(MC_MAIL_APPEND_PUTDOWN_ITEM_RES,		OnAppendPutDownItemRes);
	SetCmdHandler(MC_MAIL_NOTIFY_MAIL,					OnNotifyMail);
}

MCommandResult XCmdHandler_Mail::OnMailboxIsOpened(MCommand* pCommand, MCommandHandler* pHandler)
{
	vector<TD_MAIL_MAILBOX_MAIL_INFO> vecMailList;
	if (pCommand->GetBlob(vecMailList, 0)==false) return CR_ERROR;

	XMailbox& refMailbox = gvar.MyInfo.Mailbox;
	for (size_t i = 0; i < vecMailList.size(); ++i)
	{
		refMailbox.AddNewMail(	vecMailList[i].uidMail,
								(MAIL_TYPE)vecMailList[i].nType,
								vecMailList[i].strSenderName,
								vecMailList[i].strMailTitle,
								vecMailList[i].nAppendedItemID,
								vecMailList[i].nAppendedMoney,
								vecMailList[i].bIsRead,
								vecMailList[i].nExpiringTimeSec
							);
	}

	if (false == refMailbox.HadOpened())
		refMailbox.SetHasOpened();

	// Show mail box UI
	if ( global.script)		global.script->GetGlueGameEvent().OnGameEvent( "MAILBOX", "BEGIN");

	// Set player action to interact
	gvar.Game.pMyPlayer->GetModuleMyControl()->DoActionInteraction( XGetInteractionInfo().InteractionTargetUID);

	// Refresh mail box UI
	if ( global.script)		global.script->GetGlueGameEvent().OnGameEvent( "MAILBOX", "REFRESH");


	XObject* pObj = gg.omgr->FindObject(XGetInteractionInfo().InteractionTargetUID);
	if (pObj)
	{
		XModuleActorControl* pActorControl = pObj->GetModuleActorControl();
		if(pActorControl)	pActorControl->DoActionMailOpen();
	}

	return CR_TRUE;
}

MCommandResult XCmdHandler_Mail::OnPostRes(MCommand* pCommand, MCommandHandler* pHandler)
{
	int nResult;
	if ( pCommand->GetParameter( &nResult, 0, MPT_INT) == false)
		return CR_ERROR;


	// Event handler
	if ( global.script)		global.script->GetGlueGameEvent().OnGameEvent( "MAILBOX", "SEND", nResult);

	return CR_TRUE;
}

MCommandResult XCmdHandler_Mail::OnReadRes(MCommand* pCommand, MCommandHandler* pHandler)
{
	TD_MAIL_CONTENT_INFO* pInfo;
	wstring strText;
	vector<TD_ITEM> vecAppendedItem;
	
	if (!pCommand->GetSingleBlob(pInfo, 0))				return CR_ERROR;
	if (!pCommand->GetParameter(strText, 1, MPT_WSTR))	return CR_ERROR;
	if (!pCommand->GetBlob(vecAppendedItem, 2))			return CR_ERROR;

	
	XMail* pMail = gvar.MyInfo.Mailbox.GetMail( pInfo->uidMail);
	if ( pMail != NULL)
	{
		XMailContent& _content = pMail->GetContent();
		_content.strText = strText;
		_content.bFill = true;	///< ó�� ������ �޾ƿ� �� true �� �����ϰ� ���� ������ ���� ���� ������ ��û���� �ʰ� ���� �����͸� �н��ϴ�.

		_content.vAppendedItems.clear();
		for ( vector<TD_ITEM>::iterator itr = vecAppendedItem.begin();  itr != vecAppendedItem.end();  itr++)
			_content.vAppendedItems.push_back( *itr);

		if (global.script)		global.script->GetGlueGameEvent().OnGameEvent( "MAILBOX", "READ");
	}
	
	return CR_TRUE;
}

MCommandResult XCmdHandler_Mail::OnDeleteRes(MCommand* pCommand, MCommandHandler* pHandler)
{
	MUID uidMail;
	if ( pCommand->GetParameter( &uidMail, 0, MPT_UID) == false)
		return CR_ERROR;

	XMailbox& refMailbox = gvar.MyInfo.Mailbox;
	refMailbox.DeleteMail(uidMail);

	// Refresh mail box UI
	if ( global.script)		global.script->GetGlueGameEvent().OnGameEvent( "MAILBOX", "DELETE");

	return CR_TRUE;
}

MCommandResult XCmdHandler_Mail::OnTakeAppendedItemRes(MCommand* pCommand, MCommandHandler* pHandler)
{
	// Do Something
	MUID uidMail;
	uint8 nMailSlotID;
	int nNewThumbnailItemID;

	if (!pCommand->GetParameter(&uidMail, 				0, MPT_UID))	return CR_ERROR;
	if (!pCommand->GetParameter(&nMailSlotID,			1, MPT_UCHAR))	return CR_ERROR;
	if (!pCommand->GetParameter(&nNewThumbnailItemID,	2, MPT_INT))	return CR_ERROR;
	
	XMailbox& refMailbox = gvar.MyInfo.Mailbox;
	XMail* pMail = refMailbox.GetMail(uidMail);
	if (pMail == NULL) return CR_TRUE;

	pMail->GetContent().DelAppendedItem(nMailSlotID);
	pMail->SetThumbnailItemID(nNewThumbnailItemID);


	{
		/// ������ UI ���� - Thumbnail Item ������ ������ ����.
		if (global.script)		global.script->GetGlueGameEvent().OnGameEvent( "MAILBOX", "TAKE_ITEM", nMailSlotID);
	}

	return CR_TRUE;
}

MCommandResult XCmdHandler_Mail::OnTakeAppendedMoneyRes(MCommand* pCommand, MCommandHandler* pHandler)
{
	MUID uidMail;

	if (!pCommand->GetParameter(&uidMail, 0, MPT_UID))	return CR_ERROR;

	XMailbox& refMailBox = gvar.MyInfo.Mailbox;
	XMail* pMail = refMailBox.GetMail(uidMail);

	if (pMail != NULL)
	{
		XMailContent& content = pMail->GetContent();
		content.nAppendedMoney = 0;

		// TODO: ÷�� ���ӸӴϸ� �����մϴ�. [10/26/2010 praptor]
		if ( global.script)		global.script->GetGlueGameEvent().OnGameEvent( "MAILBOX", "TAKE_MONEY");
	}

	return CR_TRUE;
}

MCommandResult XCmdHandler_Mail::OnAppendPutUpItemRes(MCommand* pCommand, MCommandHandler* pHandler)
{
	// ���� Ȯ��
	TD_MAIL_RESERVE_ITEM* pReserveItem;
	
	if (!pCommand->GetSingleBlob(pReserveItem, 0))		return CR_ERROR;


	// ��� ó��
	int nSlotID = pReserveItem->nInventorySlotID;
	int nAmount = pReserveItem->nItemAmount;

	
	// TODO: ÷�� �������� �ø��ϴ�. [10/25/2010 praptor]
	if ( global.script)		global.script->GetGlueGameEvent().OnGameEvent( "MAILBOX", "ATTACH_ITEM", nSlotID, nAmount);


	return CR_TRUE;
}

MCommandResult XCmdHandler_Mail::OnAppendPutDownItemRes(MCommand* pCommand, MCommandHandler* pHandler)
{
	// ���� Ȯ��
	TD_MAIL_RESERVE_ITEM* pReserveItem;

	if (!pCommand->GetSingleBlob(pReserveItem, 0))		return CR_ERROR;


	// ��� ó��
	int nSlotID = pReserveItem->nInventorySlotID;
	int nAmount = pReserveItem->nItemAmount;

	// TODO: ÷�� �������� �����ϴ�. [10/25/2010 praptor]
	if ( global.script)		global.script->GetGlueGameEvent().OnGameEvent( "MAILBOX", "DETACH_ITEM", nSlotID, nAmount);

	return CR_TRUE;
}

MCommandResult XCmdHandler_Mail::OnNotifyMail(MCommand* pCommand, MCommandHandler* pHandler)
{
	bool bIsNew;
	vector<TD_MAIL_MAILBOX_MAIL_INFO> vecInfo;

	if (!pCommand->GetParameter(&bIsNew, 0, MPT_BOOL))	return CR_ERROR;
	if (!pCommand->GetBlob(vecInfo, 1))					return CR_ERROR;


	// ���� �߰�
	XMailbox& refMailBox = gvar.MyInfo.Mailbox;
	for (size_t i = 0; i < vecInfo.size(); ++i)
	{
		refMailBox.AddNewMail(	vecInfo[i].uidMail, 
								(MAIL_TYPE)vecInfo[i].nType,
								vecInfo[i].strSenderName,
								vecInfo[i].strMailTitle,
								vecInfo[i].nAppendedItemID,
								vecInfo[i].nAppendedMoney,
								vecInfo[i].bIsRead,
								vecInfo[i].nExpiringTimeSec
								);
	}

	
	// Event handler
	if (bIsNew)
	{
		if ( global.script)		global.script->GetGlueGameEvent().OnGameEvent( "MAILBOX", "RECIEVED");
	}

	// Refresh mail box UI
	if ( global.script)		global.script->GetGlueGameEvent().OnGameEvent( "MAILBOX", "REFRESH");


	return CR_TRUE;
}

MCommandResult XCmdHandler_Mail::OnMailboxIsClosed(MCommand* pCommand, MCommandHandler* pHandler)
{
	XObject* pObj = gg.omgr->FindObject(XGetInteractionInfo().InteractionTargetUID);
	if (pObj)
	{
		XModuleActorControl* pActorControl = pObj->GetModuleActorControl();
		if(pActorControl)	pActorControl->DoActionMailClose();
	}

	return CR_TRUE;
}