#include "stdafx.h"
#include "XGlueMyCharacterFunction.h"
#include "XMyPlayer.h"
#include "XModulePlayerControl.h"
#include "XGame.h"
#include "XWorld.h"

void XGlueMyCharacterFunction::DeclGlueFunc( MWLua* pLua )
{
	MWLua::class_<XGlueMyCharacterFunction>(pLua->GetState(), "_shMycharacter")
		// ĳ���� ����
		.def("GetID", &XGlueMyCharacterFunction::GetID)	
		.def("GetSurName", &XGlueMyCharacterFunction::GetSurName)
		.def("GetLevel", &XGlueMyCharacterFunction::GetLevel)	
		.def("GetBounty", &XGlueMyCharacterFunction::GetBounty)	
		.def("GetExpPercent", &XGlueMyCharacterFunction::GetExpPercent)
		.def("GetExp", &XGlueMyCharacterFunction::GetExp)

		.def("GetItemMaxWeight", &XGlueMyCharacterFunction::GetItemMaxWeight)	
		.def("GetItemWeight", &XGlueMyCharacterFunction::GetItemWeight)	

		// �ɷ� ����
		.def("GetSTR", &XGlueMyCharacterFunction::GetSTR)	
		.def("GetDEX", &XGlueMyCharacterFunction::GetDEX)	
		.def("GetINT", &XGlueMyCharacterFunction::GetINT)
		.def("GetCON", &XGlueMyCharacterFunction::GetCON)
		.def("GetCHA", &XGlueMyCharacterFunction::GetCHA)	

		// MOD
		.def("GetModSTR", &XGlueMyCharacterFunction::GetModSTR)	
		.def("GetModDEX", &XGlueMyCharacterFunction::GetModDEX)	
		.def("GetModINT", &XGlueMyCharacterFunction::GetModINT)
		.def("GetModCON", &XGlueMyCharacterFunction::GetModCON)
		.def("GetModCHA", &XGlueMyCharacterFunction::GetModCHA)	

		// ���� ����
		.def("GetMR", &XGlueMyCharacterFunction::GetMR)	
		.def("GetFR", &XGlueMyCharacterFunction::GetFR)	
		.def("GetCR", &XGlueMyCharacterFunction::GetCR)	
		.def("GetLR", &XGlueMyCharacterFunction::GetLR)	
		.def("GetPR", &XGlueMyCharacterFunction::GetPR)	

		// ���� ����
		.def("GetHP", &XGlueMyCharacterFunction::GetHP)	
		.def("GetEN", &XGlueMyCharacterFunction::GetEN)	
		.def("GetSTA", &XGlueMyCharacterFunction::GetSTA)

		.def("GetMaxHP", &XGlueMyCharacterFunction::GetMaxHP)	
		.def("GetMaxEN", &XGlueMyCharacterFunction::GetMaxEN)	
		.def("GetMaxSTA", &XGlueMyCharacterFunction::GetMaxSTA)	

		// �κ� ����
		.def("UseItem", &XGlueMyCharacterFunction::UseItem)	
		.def("GetItemInfo", &XGlueMyCharacterFunction::GetItemInfo)
		.def("GetItemName", &XGlueMyCharacterFunction::GetItemName)

		.def("GetInventoryFirstItem", &XGlueMyCharacterFunction::GetInventoryFirstItem)	
		.def("GetInventoryNextItem", &XGlueMyCharacterFunction::GetInventoryNextItem)	

		// ������� ����
		.def("GetEquippedItem", &XGlueMyCharacterFunction::GetEquippedItem)

		// �������� ����

		// �ŷ�Ʈ ����

		// ������ ����

		// �ɸ� �ŷ�Ʈ

		// ���� ��뿡 ���� ó��

		// �� ����
		.def("GetFieldName", &XGlueMyCharacterFunction::GetFieldName)
		;
}


void XGlueMyCharacterFunction::UseItem(unsigned int nItemIDHigh, unsigned int nItemIDLow)
{
	// [10/5/2007 isnara]
	// ItemKey������ XMyAtionUseItem���� �����Ѵ�.
	MUID item_id(nItemIDHigh, nItemIDLow);

	//XPOSTCMD1(MC_ITEM_REQUEST_USE, MCommandParameterUID(item_id));
	gvar.Game.pMyPlayer->GetModulePlayerControl()->RequestUseItem(item_id);
}

void XGlueMyCharacterFunction::GetItemInfo(unsigned int nItemID_High, unsigned int nItemID_Low)
{

}

const char* XGlueMyCharacterFunction::GetItemName(unsigned int nItemID_High, unsigned int nItemID_Low)
{
	return gvar.MyInfo.Inventory.GetItem(MUID(nItemID_High, nItemID_Low))->m_pItemData->m_strName.c_str();
}


const char* XGlueMyCharacterFunction::GetInventoryFirstItem()
{
	XItem* i = gvar.MyInfo.Inventory.FirstItem();
	if (i==NULL)
	{
		return "";
	}

	return i->m_pItemData->m_strName.c_str();
}

const char* XGlueMyCharacterFunction::GetInventoryNextItem()
{
	XItem* i = gvar.MyInfo.Inventory.NextItem();
	if (i == NULL)
	{
		return "";
	}

	return i->m_pItemData->m_strName.c_str();
}

const char* XGlueMyCharacterFunction::GetEquippedItem(const char* slot)
{
	return gvar.MyInfo.EquipmentSlot.GetItemString(slot);
}

const char* XGlueMyCharacterFunction::GetFieldName()
{
	if(global.game && global.game->GetWorld())
	{
		return global.game->GetWorld()->GetInfo()->m_strFieldName.c_str();
	}

	return NULL;
}