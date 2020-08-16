#pragma once

#include "CSItemData.h"
#include "GDef.h"

class GEntityPlayer;
class GItemData;
class GItem;

class GUTHelper_Item
{
public:
	// ���ο� ������������ �����ϰ� ItemManager�� �߰��Ѵ�.  nID�� INVALID_ID�� ���� ���, ������ ID�� �����Ѵ�.
	static GItemData* NewItemData(int nID=INVALID_ID, SH_ITEM_SLOT nFirstSlot=ITEMSLOT_NONE, SH_ITEM_SLOT nSecondSlot=ITEMSLOT_NONE);
	// ���ο� ���� ������������ �����ϰ� ItemManager�� �߰�.  ���������� ITEMSLOT_RWEAPON�� ����.
	static GItemData* NewWeaponItemData(int nID, WEAPON_TYPE nWeaponType);
	// ���ο� �� ������������ �����ϰ� ItemManager�� �߰�.  ���������� ITEMSLOT_HEAD�� ����.
	static GItemData* NewArmorItemData(int nID, ARMOR_TYPE nArmorType);
	// ���ο� �� ������ �����ϰ� ItemManager�� �߰�. ���������� ITEMSLOT_LWEAPON�� ����.
	static GItemData* NewShieldItemData(int nID=INVALID_ID);

	// ���ο� �������� �����
	static GItem* NewItem(int nItemID=INVALID_ID, int nAmount=1);
	// ���ο� �������� ����� �����Ѵ�.
	static GItem* GiveNewItem(GEntityPlayer* pPlayer, int nItemID=INVALID_ID, int nItemQuantity=1, SH_ITEM_SLOT_TYPE nSlotType = SLOTTYPE_INVENTORY);
	// ���ο� �������� ����� �����Կ� �����Ѵ�.
	static GItem* GiveNewItemToStorage(GEntityPlayer* pPlayer, int nItemID=INVALID_ID, int nItemQuantity=1);
	// ���ο� �������� ����� �����Կ� �����Ѵ�.
	static GItem* GiveNewItemToGuildStorage(GEntityPlayer* pPlayer, int nItemID=INVALID_ID, int nItemQuantity=1);
	// ���ο� �������� ����� ��������, ���� ��Ų��.
	static GItem* EquipNewItem(GEntityPlayer* pPlayer, int nItemID=INVALID_ID, SH_ITEM_SLOT nItemSlot = ITEMSLOT_NONE);
	// ���� ������������ �����ϰ�, pOwner�� �κ��丮�� �߰��Ѵ�.
	static GItem* GainWeaponItem(GEntityPlayer* pOwner, int nID=INVALID_ID, WEAPON_TYPE nWeaponType = WEAPON_1H_SLASH);
	// �� ������������ �����ϰ�, pOwner�� �κ��丮�� �߰��Ѵ�.
	static GItem* GainArmorItem(GEntityPlayer* pOwner, ARMOR_TYPE nArmorType = ARMOR_CLOTH);
	// ���� ������������ �����ϰ�, pOwner�� �κ��丮�� �߰��Ѵ�.
	static GItem* GainShieldItem(GEntityPlayer* pOwner);

	// ���� ������������ �����ϰ�, pOwner�� �κ��丮�� �߰��Ѵ�.
	static GItem* GainDyeItem( GEntityPlayer* pOwner, int nColor);
	static GItem* GainDyeItem( GEntityPlayer* pOwner, vector<int> vecColor);


	// ���⸦ pOwner�� �κ��丮�� �߰��ϰ� �����Ѵ�.
	static GItem* EquipNewWeapon(GEntityPlayer* pOwner, WEAPON_TYPE nWeaponType, SH_ITEM_SLOT nSlot=ITEMSLOT_RWEAPON);
	// ���� pOwner�� �κ��丮�� �߰��ϰ� �����Ѵ�.
	static GItem* EquipNewShield(GEntityPlayer* pOwner);
	// �ܼ��� ������ ���
	static bool EquipItem(GEntityPlayer* pPlayer, GItem* pItem, SH_ITEM_SLOT nSlot=ITEMSLOT_RWEAPON);
	// �ܼ��� ������ ����
	static bool UnequipItem(GEntityPlayer* pPlayer, GItem* pItem);

	// �κ��丮�� ���������� ä��
	static bool FullItemToInven(GEntityPlayer* pPlayer, int nEmptySize=0);

	// ��ȭ�� �����
	GItem* NewEnchantStoneItem(GEntityPlayer* pPlayer, int nLinkedBuffID=INVALID_BUFF_ID);
};