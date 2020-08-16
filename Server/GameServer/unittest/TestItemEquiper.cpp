#include "stdafx.h"
#include "FBaseItem.h"
#include "FBaseMockLink.h"
#include "GItemEquiper.h"
#include "GUTHelper_Item.h"
#include "GTestForward.h"
#include "GItemHolder.h"

SUITE(ItemEquiper)
{
	struct Fixture : public FBaseItem, public FBaseMockLink
	{
	public:
		Fixture()
		{
			m_pField = test.field.DefaultMockField();
			m_pPlayer = test.player.NewPlayer(m_pField);
		}

		~Fixture()
		{			
			m_pField->Destroy();
		}

		MockField*		m_pField;
		GEntityPlayer*	m_pPlayer;
		GItemEquiper	m_ItemEquiper;
	};

	TEST_FIXTURE(Fixture, CheckSlot)
	{
		// �κ��̳�, ���â �ܿ� �ٸ����� �ִ� �������� ����Ҽ� ����.
		GItem* pItemInMail = test.item.GainWeaponItem(m_pPlayer, INVALID_ID, WEAPON_1H_SLASH);
		pItemInMail->m_nSlotType = SLOTTYPE_MAIL;		
		CHECK_EQUAL(false, m_ItemEquiper.CheckSlot(m_pPlayer, pItemInMail, ITEMSLOT_RWEAPON));

		// ���â�� �ִ� �������� ���� ���Կ� ����Ҽ� ����.
		GItem* pEquipedItem = test.item.EquipNewItem(m_pPlayer);
		CHECK_EQUAL(false, m_ItemEquiper.CheckSlot(m_pPlayer, pEquipedItem, static_cast<SH_ITEM_SLOT>(pEquipedItem->m_nSlotID)));

		// ���а� �ƴ� ����������� �����ӿ� �������� ������� ���� ���¿���, �޼տ� �����Ҽ� ����.
		GItem* pWeapon = test.item.GainWeaponItem(m_pPlayer, INVALID_ID, WEAPON_1H_SLASH);
		CHECK_EQUAL(false, m_ItemEquiper.CheckSlot(m_pPlayer, pWeapon, ITEMSLOT_LWEAPON));
		GItem* pShield = test.item.GainShieldItem(m_pPlayer);
		CHECK_EQUAL(true, m_ItemEquiper.CheckSlot(m_pPlayer, pShield, ITEMSLOT_LWEAPON));

		// ������ ��� �׽�Ʈ�� TestItemHelper�� IsEquipableSlot���� �ϰ� �ִ�.
	}	

	TEST_FIXTURE(Fixture, CheckReservedDummySlot)
	{
		CHECK_EQUAL(true, m_ItemEquiper.CheckReservedDummySlot(m_pPlayer, ITEMSLOT_RWEAPON, ITEMSLOT_NONE));

		m_pPlayer->GetItemHolder()->ReserveDummySlot(SLOTTYPE_EQUIP, ITEMSLOT_RWEAPON);

		CHECK_EQUAL(false, m_ItemEquiper.CheckReservedDummySlot(m_pPlayer, ITEMSLOT_RWEAPON, ITEMSLOT_NONE));
	}

	TEST_FIXTURE(Fixture, CheckHasPassiveTalent_Weapon)
	{		
		GItem* pWeapon = test.item.GainWeaponItem(m_pPlayer, INVALID_ID, WEAPON_1H_SLASH);

		pWeapon->m_pItemData->m_nWeaponType = WEAPON_1H_SLASH;
		CHECK_EQUAL(false, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pWeapon));
		test.talent.LearnExtraPassiveTalent(m_pPlayer, TEPT_BASE_WEAPON_1H_SLASH);
		CHECK_EQUAL(true, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pWeapon));

		pWeapon->m_pItemData->m_nWeaponType = WEAPON_1H_BLUNT;
		CHECK_EQUAL(false, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pWeapon));
		test.talent.LearnExtraPassiveTalent(m_pPlayer, TEPT_BASE_WEAPON_1H_BLUNT);
		CHECK_EQUAL(true, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pWeapon));

		pWeapon->m_pItemData->m_nWeaponType = WEAPON_1H_PIERCE;
		CHECK_EQUAL(false, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pWeapon));
		test.talent.LearnExtraPassiveTalent(m_pPlayer, TEPT_BASE_WEAPON_1H_PIERCE);
		CHECK_EQUAL(true, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pWeapon));

		pWeapon->m_pItemData->m_nWeaponType = WEAPON_TWO_HANDED;
		CHECK_EQUAL(false, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pWeapon));
		test.talent.LearnExtraPassiveTalent(m_pPlayer, TEPT_BASE_WEAPON_TWO_HANDED);
		CHECK_EQUAL(true, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pWeapon));

		pWeapon->m_pItemData->m_nWeaponType = WEAPON_STAFF;
		CHECK_EQUAL(false, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pWeapon));
		test.talent.LearnExtraPassiveTalent(m_pPlayer, TEPT_BASE_WEAPON_STAFF);
		CHECK_EQUAL(true, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pWeapon));

		pWeapon->m_pItemData->m_nWeaponType = WEAPON_ARCHERY;
		CHECK_EQUAL(false, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pWeapon));
		test.talent.LearnExtraPassiveTalent(m_pPlayer, TEPT_BASE_WEAPON_ARCHERY);
		CHECK_EQUAL(true, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pWeapon));

		pWeapon->m_pItemData->m_nWeaponType = WEAPON_2H_BLUNT;
		CHECK_EQUAL(false, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pWeapon));
		test.talent.LearnExtraPassiveTalent(m_pPlayer, TEPT_BASE_WEAPON_2H_BLUNT);
		CHECK_EQUAL(true, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pWeapon));

		pWeapon->m_pItemData->m_nWeaponType = WEAPON_DUAL_PIERCE;
		CHECK_EQUAL(false, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pWeapon));
		test.talent.LearnExtraPassiveTalent(m_pPlayer, TEPT_BASE_WEAPON_DUAL_PIERCE);
		CHECK_EQUAL(true, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pWeapon));
	}

	TEST_FIXTURE(Fixture, CheckHasPassiveTalent_Armor)
	{
		GItem* pArmor = test.item.GiveNewItem(m_pPlayer);
		pArmor->m_pItemData->m_ItemType = ITEMTYPE_ARMOR;

		pArmor->m_pItemData->m_nArmorType = ARMOR_CLOTH;
		CHECK_EQUAL(true, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pArmor));

		pArmor->m_pItemData->m_nArmorType = ARMOR_LIGHT;
		CHECK_EQUAL(false, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pArmor));
		test.talent.LearnExtraPassiveTalent(m_pPlayer, TEPT_BASE_ARMOR_LIGHT);
		CHECK_EQUAL(true, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pArmor));

		pArmor->m_pItemData->m_nArmorType = ARMOR_MEDIUM;
		CHECK_EQUAL(false, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pArmor));
		test.talent.LearnExtraPassiveTalent(m_pPlayer, TEPT_BASE_ARMOR_MEDUIM);
		CHECK_EQUAL(true, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pArmor));

		pArmor->m_pItemData->m_nArmorType = ARMOR_HEAVY;
		CHECK_EQUAL(false, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pArmor));
		test.talent.LearnExtraPassiveTalent(m_pPlayer, TEPT_BASE_ARMOR_HEAVY);
		CHECK_EQUAL(true, m_ItemEquiper.CheckHasPassiveTalent(m_pPlayer, pArmor));
	}

	TEST_FIXTURE(Fixture, SelectUnequipItem)
	{
		GItem* p1HS = test.item.GainWeaponItem(m_pPlayer, INVALID_ID, WEAPON_1H_SLASH);
		GItem* p1HS2 = test.item.GainWeaponItem(m_pPlayer, INVALID_ID, WEAPON_1H_SLASH);
		GItem* p2HD = test.item.GainWeaponItem(m_pPlayer, INVALID_ID, WEAPON_TWO_HANDED);
		GItem* p2HD2 = test.item.GainWeaponItem(m_pPlayer, INVALID_ID, WEAPON_TWO_HANDED);
		GItem* pShield = test.item.GainShieldItem(m_pPlayer);

		GItem* pUnequipItem1 = NULL;
		GItem* pUnequipItem2 = NULL;

		// ������ ���� ������ ���ν������� �����ϰ� �ִ� ������ ã��
		test.item.EquipItem(m_pPlayer, p1HS, ITEMSLOT_RWEAPON);
		m_ItemEquiper.SelectUnequipItem(m_pPlayer, p1HS2, ITEMSLOT_RWEAPON, ITEMSLOT_NONE, pUnequipItem1, pUnequipItem2);
		CHECK_EQUAL(pUnequipItem1, p1HS);

		// ������ ���ν����� ���꽽������ ����ϰ� �ִ� ������ ã��
		test.item.EquipItem(m_pPlayer, p2HD, ITEMSLOT_RWEAPON);
		m_ItemEquiper.SelectUnequipItem(m_pPlayer, pShield, ITEMSLOT_LWEAPON, ITEMSLOT_NONE, pUnequipItem1, pUnequipItem2);
		CHECK_EQUAL(pUnequipItem1, p2HD);

		// ������ ���꽽���� ���ν������� �����ϰ� �ִ� ������ ã��
		test.item.EquipItem(m_pPlayer, pShield, ITEMSLOT_LWEAPON);
		m_ItemEquiper.SelectUnequipItem(m_pPlayer, p2HD, ITEMSLOT_RWEAPON, ITEMSLOT_NONE, pUnequipItem1, pUnequipItem2);
		CHECK_EQUAL(pUnequipItem1, pShield);

		// ������ ���꽽���� ���꽽������ �����ϰ� �ִ� ������ ã��
		test.item.EquipItem(m_pPlayer, p2HD, ITEMSLOT_RWEAPON);
		m_ItemEquiper.SelectUnequipItem(m_pPlayer, p2HD2, ITEMSLOT_RWEAPON, ITEMSLOT_NONE, pUnequipItem1, pUnequipItem2);
		CHECK_EQUAL(pUnequipItem1, p2HD);

		// ���� �����Ҽ� ���� ���� ã�� �׽�Ʈ�� TestItemHelper�� IsTogetherEquipableItemData���� �ϰ� �ִ�.
	}
}