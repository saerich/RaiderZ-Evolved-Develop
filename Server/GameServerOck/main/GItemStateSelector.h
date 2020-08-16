#pragma once

class GItem;

enum ITEM_STATE_TYPE
{
	IST_NONE = 0,

	IST_INVENTORY,		// �κ��丮�� �ִ� ������
	IST_STORAGE,		// â�� �ִ� ������
	IST_POST,			// �����Կ� �ִ� ������
	IST_EQUIPED,		// �������� ��� ������
	IST_ARRANGED,		// ��ġ�� ���� ������
	IST_TRADING,		// �ŷ����� ������
	IST_ENCNAHTING,		// ��ȭ���� ������
	IST_DYEING,			// �������� ������
	IST_MAX
};


class GItemStateSelector
{
public:
	ITEM_STATE_TYPE Select(GEntityPlayer* pPlayer, const GItem* pItem);

private:
	bool IsInventoryItem(GEntityPlayer* pPlayer, const GItem* pItem);
	bool IsStorageItem(GEntityPlayer* pPlayer, const GItem* pItem);
	bool IsPostItem(GEntityPlayer* pPlayer, const GItem* pItem);
	bool IsEquipedItem(GEntityPlayer* pPlayer, const GItem* pItem);
	bool IsArrangedItem(GEntityPlayer* pPlayer, const GItem* pItem);
	bool IsTradingItem(GEntityPlayer* pPlayer, const GItem* pItem);	
	bool IsEnchant(GEntityPlayer* pPlayer);
	bool IsDyeingItem(GEntityPlayer* pPlayer);
};