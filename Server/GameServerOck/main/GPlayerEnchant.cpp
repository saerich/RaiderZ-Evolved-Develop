#include "StdAfx.h"
#include "GPlayerEnchant.h"
#include "GEntityPlayer.h"
#include "GItemHolder.h"
#include "GGlobal.h"
#include "GItemManager.h"
#include "GEquipmentSlot.h"

GPlayerEnchant::GPlayerEnchant(GEntityPlayer* pOwner)
: m_pOwner(pOwner)
{
	m_pOwner->AttachObserver(this);
}

GPlayerEnchant::~GPlayerEnchant(void)
{
	m_pOwner->DetachObserver(this);
}

void GPlayerEnchant::OnItemEnchantSuccess( GItem* pItem, int nEnchantItemID )
{
	if (pItem->m_nSlotType != SLOTTYPE_EQUIP)
		return; // ��������� ����

	if (!IsSameWeaponSet(pItem))
		return; // ���� Ȱ��ȭ�� ���� ���� �ƴ�

	m_pOwner->GainBuff(GetLinkedBuffID(nEnchantItemID));
}

void GPlayerEnchant::OnItemEquipped( GItem* pItem )
{
	if (!IsSameWeaponSet(pItem))
		return; // ���� Ȱ��ȭ�� ���� ���� �ƴ�
			
	GainBuffFromEquip(pItem);
}

void GPlayerEnchant::OnItemUnequipped( GItem* pItem )
{
	if (!IsSameWeaponSet(pItem))
		return; // ���� Ȱ��ȭ�� ���� ���� �ƴ�

	LostBuffFromEquip(pItem);
}

void GPlayerEnchant::GainBuffFromEquip( GItem* pItem )
{
	if (!pItem)
		return;

	for (int i=0; i<ENCHANT_MAX_COUNT; ++i)
	{
		m_pOwner->GainBuff(GetLinkedBuffID(pItem->m_nEnchants[i]));
	}
}

void GPlayerEnchant::LostBuffFromEquip( GItem* pItem )
{
	if (!pItem)
		return;

	for (int i=0; i<ENCHANT_MAX_COUNT; ++i)
	{
		m_pOwner->LostBuff(GetLinkedBuffID(pItem->m_nEnchants[i]));
	}
}

bool GPlayerEnchant::IsSameWeaponSet(GItem* pItem) const
{
	if (!pItem)
		return false;

	VALID_RET(m_pOwner, false);
	VALID_RET(m_pOwner->GetItemHolder(), false);

	if (pItem->m_nSlotType == SLOTTYPE_EQUIP)
	{
		if (m_pOwner->GetItemHolder()->GetEquipment().GetWeaponSet() == ITEMSWITCH_SECONDARY &&
			(pItem->m_nSlotID == ITEMSLOT_LWEAPON || pItem->m_nSlotID == ITEMSLOT_RWEAPON))
		{
			return false; // ���� ���� �ٸ�
		}

		if (m_pOwner->GetItemHolder()->GetEquipment().GetWeaponSet() == ITEMSWITCH_PRIMARY &&
			(pItem->m_nSlotID == ITEMSLOT_LWEAPON2 || pItem->m_nSlotID == ITEMSLOT_RWEAPON2))
		{
			return false; // ���� ���� �ٸ�
		}
	}

	return true;
}

int GPlayerEnchant::GetLinkedBuffID( int nEnchantItemID ) const
{
	GItemData* pItemData = gmgr.pItemManager->GetItemData(nEnchantItemID);
	if (!pItemData)
		return INVALID_BUFF_ID;

	return pItemData->m_EnchantStone.nLinkedBuffID;
}
