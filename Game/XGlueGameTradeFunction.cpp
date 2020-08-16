#include "stdafx.h"
#include "XGlueGameTradeFunction.h"
#include "XCommonFunction.h"
#include "XPost_Trade.h"
#include "XInteractionInfo.h"

void XGlueGameTradeFunction::TradeStartReq()
{
	if (XGetInteractionInfo().InteractionTargetUID ==  gvar.MyInfo.MyUID)
	{
		global.ui->OnSystemMsg(L"�ڱ� �ڽſ��� �ŷ��� ��û�� �� �����ϴ�.");
	}
	else
	{
		XPostTrade_Start(XGetInteractionInfo().InteractionTargetUID);

		wstring strPlayerName = XCommonFunction::GetPlayerName(XGetInteractionInfo().GetInteractionSearchUID());
		global.ui->OnSystemMsg(L"{0}�Բ� �ŷ��� ��û �Ͽ����ϴ�.", FSParam(strPlayerName));
	}
}

void XGlueGameTradeFunction::TradeRequiredAnswer( bool bAccept )
{
	//XPostTrade_StartAccept(bAccept);

	////char text[1024]="";
	////sprintf_s(text, "�ŷ��� �������� �ʾҽ��ϴ�.");

	//// TODO : �ŷ� ��û�� �ް� �ٷ� ������ �ϰ� �Ǹ� �ʿ������ ����.
	//if (false == bAccept)
	//{		
	//	gvar.MyInfo.TradeContents.Reset();		
	//}
}

void XGlueGameTradeFunction::TradeItemListUp( const char* szITEMUID )
{
	// ! GetItem(MUID) �Լ��� ���ŵǾ�, slotID�θ� ������ �� �ֽ��ϴ�. - praptor, 2010.03.03
	// ! ������� �ʴ� �Լ��� �ǴܵǾ� ���忡�� ������, ���� �ּ�ó���մϴ�. - praptor, 2010.03.03


	//if( szITEMUID == NULL) return;

	//MUID uidItem = _atoi64(szITEMUID);

	////�ϴ� ������ ��� ��,��;;
	//int nPutUpItemQuantity = 1;// = static_cast<int>(atoi(argv[2]));

	//XItem* pInvenItem = gvar.MyInfo.Inventory.GetItem(uidItem);
	//if (NULL == pInvenItem) return;

	//nPutUpItemQuantity = pInvenItem->m_nAmount;

	//if (MAX_TRADE_ITEM_SPACE <= gvar.MyInfo.TradeContents.m_MyTradeContainer.GetSize())
	//{
	//	char text[1024];
	//	sprintf_s(text, 1024, "�ŷ�â�� �ø��� �ִ� �ִ� �������� ������ %d�� �Դϴ�.", MAX_TRADE_ITEM_SPACE);
	//	global.ui->OnSystemMsg(text);
	//	return;
	//}

	//int nTradeItemQuantity = 0;
	//XItem* pTradeItem = gvar.MyInfo.TradeContents.m_MyTradeContainer.GetItem(pInvenItem->m_nSlotID);
	//if (NULL != pTradeItem)
	//{
	//	nTradeItemQuantity = pTradeItem->m_nAmount;
	//}

	//if (nPutUpItemQuantity > pInvenItem->m_nAmount - nTradeItemQuantity) return;

	//TD_ITEM tdItem;
	//tdItem.m_nDurability = pInvenItem->m_nDurability;
	//tdItem.m_nQuantity = nPutUpItemQuantity;
	//tdItem.m_nItemID = pInvenItem->m_pItemData->m_nID;
	//tdItem.m_nSlotID = pInvenItem->m_nSlotID;

	//XPostTrade_PutUpItem(&tdItem);
}

void XGlueGameTradeFunction::TradeItemListDel( const char* szITEMUID )
{
	// ! GetItem(MUID) �Լ��� ���ŵǾ�, slotID�θ� ������ �� �ֽ��ϴ�. - praptor, 2010.03.03
	// ! ������� �ʴ� �Լ��� �ǴܵǾ� ���忡�� ������, ���� �ּ�ó���մϴ�. - praptor, 2010.03.03


	//if( szITEMUID == NULL) return;

	//MUID uidItem = _atoi64(szITEMUID);
	//int nPutDownItemQuantity = 1;//static_cast<int>(atoi(argv[2]));

	//int nTradingItemQuantity = 0;
	//XItem* pTradeItem = gvar.MyInfo.TradeContents.m_MyTradeContainer.GetItem(uidItem);
	//if( pTradeItem == NULL) return;

	//if (NULL != pTradeItem)
	//{
	//	nTradingItemQuantity = pTradeItem->m_nAmount;
	//}

	////������ ���
	//nPutDownItemQuantity = pTradeItem->m_nAmount;

	//if (nPutDownItemQuantity > nTradingItemQuantity) return;

	//TD_ITEM tdItem;
	//tdItem.m_nDurability = pTradeItem->m_nDurability;
	//tdItem.m_nQuantity = nPutDownItemQuantity;
	//tdItem.m_nItemID = pTradeItem->m_pItemData->m_nID;
	//tdItem.m_nSlotID = pTradeItem->m_nSlotID;

	//XPostTrade_PutDownItem(&tdItem);
};

void XGlueGameTradeFunction::TradePutUpSilver(int nPutUpSilver)
{
	XPostTrade_PutUpSilver(nPutUpSilver);
}

void XGlueGameTradeFunction::TradeConfirmRequire()
{
	XPostTrade_Confirm();
}

void XGlueGameTradeFunction::TradeCancel()
{
	XPostTrade_Cancel();
}
