#include "stdafx.h"
#include "MockField.h"
#include "GItem.h"
#include "GUTHelper.h"
#include "GItemUnequiper.h"
#include "FBaseMockLink.h"
#include "FBaseItem.h"
#include "GItemEquiper.h"
#include "GUTHelper_Item.h"
#include "GUTHelper_Talent.h"
#include "GUTHelper_Field.h"
#include "GItemHolder.h"

SUITE(ItemUnequiper)
{
	struct FItemUnequiper : public FBaseItem, public FBaseMockLink
	{
		FItemUnequiper()
		{
			 m_pField = GUTHelper_Field::DefaultMockField();
			 m_pPlayer = GUTHelper::NewEntityPlayer(m_pField);

			 GUTHelper_Talent utHelper_Talent;
			 utHelper_Talent.TrainEquipUsableAllTalent(m_pPlayer);

			 m_p1HSData = GUTHelper_Item::NewItemData();
			 m_p1HSData->m_nItemSlot = ITEMSLOT_RWEAPON;
			 m_p1HSData->m_ItemType = ITEMTYPE_WEAPON;
			 m_p1HSData->m_nWeaponType = WEAPON_1H_SLASH;
			 m_p1HSData->m_bLeftWeaponEquipable = true;

			 m_p2HSData = GUTHelper_Item::NewItemData();
			 m_p2HSData->m_nItemSlot = ITEMSLOT_RWEAPON;
			 m_p2HSData->m_nSubSlot = ITEMSLOT_LWEAPON;
			 m_p2HSData->m_ItemType = ITEMTYPE_WEAPON;
			 m_p2HSData->m_nWeaponType = WEAPON_TWO_HANDED;

			 m_pShieldData = GUTHelper_Item::NewItemData();
			 m_pShieldData->m_nItemSlot = ITEMSLOT_LWEAPON;
			 m_pShieldData->m_ItemType = ITEMTYPE_ARMOR;
			 m_pShieldData->m_nArmorType = ARMOR_SHIELD;

			 m_pItemHolder = m_pPlayer->GetItemHolder();
		}

		~FItemUnequiper()
		{
			m_pField->Destroy();
		}

		MockField*		m_pField;
		GEntityPlayer*	m_pPlayer;
		GItemData*		m_p1HSData;
		GItemData*		m_p2HSData;
		GItemData*		m_pShieldData;
		GItemHolder*	m_pItemHolder;

		GItemUnequiper	m_ItemUnequiper;
	};

	// ���� �Ѱ�¥�� ������ ����
	TEST_FIXTURE(FItemUnequiper, Unequip_OneSlotItem)
	{
		GItem* pOneSlotItem = GUTHelper_Item::GiveNewItem(m_pPlayer, m_p1HSData->m_nID);

		gsys.pItemSystem->GetEquiper().Equip(m_pPlayer, pOneSlotItem, ITEMSLOT_RWEAPON);
		CHECK_EQUAL(true, m_ItemUnequiper.Unequip(m_pPlayer, pOneSlotItem));		
		CHECK_EQUAL(m_pItemHolder->GetItem(SLOTTYPE_EQUIP, ITEMSLOT_RWEAPON), (GItem*)NULL);
	}

	// ���� �ΰ�¥�� ������ ����
	TEST_FIXTURE(FItemUnequiper, Unequip_DoubleSlotItem)
	{
		GItem* pTwoSlotItem = GUTHelper_Item::GiveNewItem(m_pPlayer, m_p2HSData->m_nID);

		gsys.pItemSystem->GetEquiper().Equip(m_pPlayer, pTwoSlotItem, ITEMSLOT_RWEAPON);
		CHECK_EQUAL(true, m_ItemUnequiper.Unequip(m_pPlayer, pTwoSlotItem));
		CHECK_EQUAL(m_pItemHolder->GetItem(SLOTTYPE_EQUIP, ITEMSLOT_LWEAPON), (GItem*)NULL);
		CHECK_EQUAL(m_pItemHolder->GetItem(SLOTTYPE_EQUIP, ITEMSLOT_RWEAPON), (GItem*)NULL);
	}

	// ����� ���и� ������ ���¿��� ���� ����
	TEST_FIXTURE(FItemUnequiper, Unequip_WeaponWithShield)
	{
		GItem* pWeapon = GUTHelper_Item::GiveNewItem(m_pPlayer, m_p1HSData->m_nID);
		GItem* pShield = GUTHelper_Item::GiveNewItem(m_pPlayer, m_pShieldData->m_nID);

		// ���� : ����, ����
		gsys.pItemSystem->GetEquiper().Equip(m_pPlayer, pWeapon, ITEMSLOT_RWEAPON);
		gsys.pItemSystem->GetEquiper().Equip(m_pPlayer, pShield, ITEMSLOT_LWEAPON);

		CHECK(m_pItemHolder->GetItem(SLOTTYPE_EQUIP, ITEMSLOT_LWEAPON) != NULL);
		CHECK(m_pItemHolder->GetItem(SLOTTYPE_EQUIP, ITEMSLOT_RWEAPON) != NULL);

		// �Ѽհ� ����
		gsys.pItemSystem->GetUnequiper().Unequip(m_pPlayer, pWeapon);

		// ���д� �����־�� �Ѵ�.
		CHECK(m_pItemHolder->GetItem(SLOTTYPE_EQUIP, ITEMSLOT_LWEAPON) != NULL);
		CHECK(m_pItemHolder->GetItem(SLOTTYPE_EQUIP, ITEMSLOT_RWEAPON) == NULL);
	}

	// �����հ� �޼տ� ���⸦ ������ ���¿��� ������ ���� ����
	TEST_FIXTURE(FItemUnequiper, Unequip_RWeaponWithLWeapon)
	{
		GItem* pRWeapon = GUTHelper_Item::GiveNewItem(m_pPlayer, m_p1HSData->m_nID);
		GItem* pLWeapon = GUTHelper_Item::GiveNewItem(m_pPlayer, m_p1HSData->m_nID);

		// ���� : ����, ����
		gsys.pItemSystem->GetEquiper().Equip(m_pPlayer, pRWeapon, ITEMSLOT_RWEAPON);
		gsys.pItemSystem->GetEquiper().Equip(m_pPlayer, pLWeapon, ITEMSLOT_LWEAPON);

		CHECK(m_pItemHolder->GetItem(SLOTTYPE_EQUIP, ITEMSLOT_LWEAPON) != NULL);
		CHECK(m_pItemHolder->GetItem(SLOTTYPE_EQUIP, ITEMSLOT_RWEAPON) != NULL);

		// ������ ���� ����
		gsys.pItemSystem->GetUnequiper().Unequip(m_pPlayer, pRWeapon);

		// ������ ���� ���� �޼ո� ���⸦ ��� ����.
		CHECK(m_pItemHolder->GetItem(SLOTTYPE_EQUIP, ITEMSLOT_LWEAPON) == NULL);
		CHECK(m_pItemHolder->GetItem(SLOTTYPE_EQUIP, ITEMSLOT_RWEAPON) == NULL);
	}
}