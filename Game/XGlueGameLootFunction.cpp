#include "stdafx.h"
#include "XGlueGameLootFunction.h"
#include "XItemManager.h"
#include "XEventID.h"
#include "XMyPlayer.h"

void XGlueGameLootFunction::LootItem( const char* szItemID, const char* szItemCount, bool bPass /*= false*/ )
{
	XItemData* pItemData = info.item->GetItemData(atoi(szItemID));
	if (pItemData == NULL) return;

	if ((bPass == false) &&
		(pItemData->m_bQuestRelated == false) &&	//����Ʈ �������� �ƴϰ�
		(pItemData->m_bTradable == false))		// ȹ��
	{
		global.script->GetGlueGameEvent().OnLootItemBind(MLocale::ConvUTF16ToAnsi(pItemData->GetName()).c_str(), szItemID, szItemCount);		
		return;
	}

	MUID nDropItemUID;
	// TODO : DropItemUID	

	XEvent evt;
	evt.m_nID = XEVTL_LOOT_GETITEM;
	evt.m_pData = &nDropItemUID;
	gg.omgr->PostEvent(gvar.MyInfo.MyUID, evt);
}

void XGlueGameLootFunction::LootEndRequest()
{
	// ���� ���� �޼��� ������
	gvar.Game.pMyPlayer->GetModuleMyControl()->OnLootEndReady();
}
