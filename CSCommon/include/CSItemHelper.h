#pragma once

#include "CSItemData.h"
#include "CSCommonLib.h"

/// ������ ���� ���� Ŭ����
class CSCOMMON_API CSItemHelper
{
public:
	/// ����� ���������� ����Ÿ�� �˾Ƴ���
	static WEAPON_TYPE GetCurrentWeaponType(CSItemData* pRightItemData, CSItemData* pLeftItemData);
	static WEAPON_TYPE GetCurrentWeaponType(WEAPON_TYPE nRight, WEAPON_TYPE nLeft, bool bEquipShield=false);		// ���д� �б�� ����ϱ� ������ ���и� ���� �ִ��� Ȯ���Ѵ�.

	/// �ش� ����Ÿ���� ������� ��ƾ� �ϴ� ����Ÿ������ ����
	static bool IsDualWieldWeaponType(WEAPON_TYPE nWeaponType);

	/// �������� ����
	static bool IsShieldItemType(CSItemData* pItemData);

	/// ���п� �Բ� ������ �������� ����
	static bool IsEquipableWithShield(WEAPON_TYPE nWeaponType);

	/// ��� �׼��� ������ ����Ÿ���ΰ�?
	static bool IsGuardableWeaponType( WEAPON_TYPE nWeaponType );

	/// ITEMSLOT_LWEAPON �Ǵ� ITEMSLOT_LWEAPON2 ��������
	static bool IsLeftWeaponSlot(SH_ITEM_SLOT nSlot);
	static bool IsRightWeaponSlot(SH_ITEM_SLOT nSlot);
	static bool IsWeaponSlot(SH_ITEM_SLOT nSlot);
	static bool IsPrimaryWeaponSlot(SH_ITEM_SLOT nSlot);
	static bool IsSecondaryWeaponSlot(SH_ITEM_SLOT nSlot);
	static SH_ITEM_SLOT GetPrimaryWeaponSlot(SH_ITEM_SLOT nSlot);
	static SH_ITEM_SLOT GetSecondaryWeaponSlot(SH_ITEM_SLOT nSlot);
	static SH_ITEM_SLOT GetOppositeWeaponSlot(SH_ITEM_SLOT nSlot);


	// ���ν������� ���꽽���� ã�Ƴ���
	static SH_ITEM_SLOT GetSubSlot(SH_ITEM_SLOT nSlot, CSItemData* pItemData);
	// ���밡���� �������� ����
	static bool IsEquipableSlot(SH_ITEM_SLOT nSlot, CSItemData* pItemData);
	// �Բ� ���� ������ ���������� ����
	static bool IsTogetherEquipableItemData(CSItemData* pItemData1, CSItemData* pItemData2);

	static SH_ITEM_SLOT GetProperWeaponSlot(SH_ITEM_SLOT nWishSlot, CSItemData* pItemData, CSItemData* pRightEquipedItemData, CSItemData* pLeftEquipedItemData);

	// Item Type
	static bool IsReusableItem(CSItemData* pItemData);
	static bool IsQuestTriggerItem(CSItemData* pItemData);
	static bool IsTalentTrainItem(CSItemData* pItemData);

	// pallete
	static int GetPalleteIndex(int nNum, int nSlot);

	static bool IsMustCheckPassiveType(TALENT_EXTRA_PASSIVE_TYPE nPassiveType);

	static TALENT_EXTRA_PASSIVE_TYPE GetTEPTForEquip(CSItemData* pItemData);
};