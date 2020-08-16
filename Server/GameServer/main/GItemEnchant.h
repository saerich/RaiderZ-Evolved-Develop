#pragma once

class GEntityPlayer;
class GItem;

class GItemEnchant : public MTestMemPool<GItemEnchant>
{
public:
	GItemEnchant(void);
	~GItemEnchant(void);

	// ��ȭ������ �����ۿ� ��ȭ
	bool Enchant( GEntityPlayer* pPlayer, int nEnchantSlotID, SH_ITEM_SLOT_TYPE nTargetSlotType, int nTargetSlotID, int nAgentSlotID = INVALID_ID);
	bool Enchant( GEntityPlayer* pPlayer, GItem* pEStone, SH_ITEM_SLOT_TYPE nTargetSlotType, int nTargetSlotID, GItem* pEAgent );
	int EnchantCheck( GEntityPlayer* pPlayer, GItem* pEStone, SH_ITEM_SLOT_TYPE nTargetSlotType, int nTargetSlotID, GItem* pEAgent );
	void EnchantPrepare( GEntityPlayer* pPlayer );
	// �����ۿ� ���� ������ ����, ��ȯ���� ��ȭ�� ������ ���� ��ȣ (���н� -1)
	int EnchantBuff( GEntityPlayer* pPlayer, GItem* pEStone, GItem* pTargetItem, ENCHANT_COLOR nEnchantColor=ENCHANT_NONE );
	// ��ȭ ���� Ȯ��
	float CalcEnchantChanceRate( GItem* pEStone, GItem* pTargetItem ) const;
	// ��ȭ ���� üũ
	bool CheckChance(GItem* pEStone, GItem* pTargetItem) const;
	// ��ȭ ���
	void EnchantCancel( GEntityPlayer* pPlayer );

	// ��ȭ�� ������ ���̵�
	int GetAgentItemID(int nTier);

	// ��ȭ �˻� ���� ��Ŷ �۽�
	void RouteEnchantCheckResult(GEntityPlayer* pPlayer, int nErrorCode);
private:
	void EnchantBroken(GEntityPlayer* pPlayer, GItem* pEStone, GItem* pEAgent, GItem* pTargetItem);
	bool CheckLimitLevel(GEntityPlayer* pPlayer, GItem* pEStone);
	bool CheckLimitEquipments(GItem* pTargetItem, GItem* pEStone);
	bool CheckLimitWeapon(GItem* pTargetItem, GItem* pEStone);
	bool CheckAgent(GItem* pEStone, GItem* pEAgent);
	bool EnchantToDB( GEntityPlayer* pPlayer, GItem* pEStone, GItem* pEAgent, GItem* pTargetItem, bool bBroken );
	int GetEmptyEnchantSlotIdx(GItem* pEStone, GItem* pTargetItem, GItem* pEAgent);
};
