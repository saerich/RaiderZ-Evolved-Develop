#include "stdafx.h"
#include "XGlueGameRepairFunction.h"
#include "XPost_NPCShop.h"

void XGlueGameRepairFunction::ReqRepairItemList()
{
	
}

void XGlueGameRepairFunction::RepairAllItem()
{	
	XPostNPCShop_RepairAll();
}

void XGlueGameRepairFunction::RepairItem( const char* szuid )
{
	// ! GetItem(MUID) �Լ��� ���ŵǾ�, slotID�θ� ������ �� �ֽ��ϴ�. - praptor, 2010.03.03
	// ! ������� �ʴ� �Լ��� �ǴܵǾ� ���忡�� ������, ���� �ּ�ó���մϴ�. - praptor, 2010.03.03

	//if( szuid == NULL) return;
	//MUID uidItem = _atoi64(szuid);

	//XItem* pItem = gvar.MyInfo.Inventory.GetItem(uidItem);
	//if (pItem != NULL)
	//{
	//	XPostNPCShop_RepairOne(SLOTTYPE_INVENTORY, pItem->m_nSlotID);
	//	return;
	//}

	//pItem = gvar.MyInfo.EquipmentSlot.GetItemByMUID(uidItem);
	//if (pItem != NULL)
	//{
	//	XPostNPCShop_RepairOne(SLOTTYPE_EQUIP, pItem->m_nSlotID);
	//	return;
	//}
}