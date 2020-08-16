#pragma once

class GItem;

enum ITEM_ACT_TYPE
{
	IAT_NONE = 0,

	IAT_DROP,			// ������
	IAT_SELL,			// �ȱ�
	IAT_TRADE,			// �ŷ�	
	IAT_EQUIP,			// ���
	IAT_UNEQUIP,		// ��� ����
	IAT_USE,			// ���
	IAT_CRAFT,			// ����
	IAT_MOVE,			// �̵�
	IAT_DYE,			// ����
	IAT_ENCHANT,		// ��æƮ
	IAT_REPAIR,			// ����

	IAT_MAIL_APPEND,	// ���� ÷��

	IAT_MAX
};

class GItemActableChecker : public MTestMemPool<GItemActableChecker>
{
public:
	bool IsDropable(GEntityPlayer* pPlayer, GItem* pItem);
	bool IsSellable(GEntityPlayer* pPlayer, GItem* pItem);
	bool IsTradable(GEntityPlayer* pPlayer, GItem* pItem);	
	bool IsEquipable(GEntityPlayer* pPlayer, GItem* pItem);
	bool IsUnequipable(GEntityPlayer* pPlayer, GItem* pItem);
	bool IsUsable(GEntityPlayer* pPlayer, GItem* pItem);
	bool IsCraftable(GEntityPlayer* pPlayer, GItem* pItem);

	bool IsMovable(GEntityPlayer* pPlayer, GItem* pItem);

	bool IsDyeable(GEntityPlayer* pPlayer, GItem* pItem);
	bool IsEnchantable(GEntityPlayer* pPlayer, GItem* pItem);
	bool IsRepairable(GEntityPlayer* pPlayer, GItem* pItem);	

	bool IsMailAppendable(GEntityPlayer* pPlayer, const GItem* pItem) const;

private:
	bool IsItemActable(GEntityPlayer* pPlayer, const GItem* pItem, ITEM_ACT_TYPE nAct) const;
};