#pragma once

#include "GActorObserver.h"

class GEntityPlayer;

class GPlayerEnchant : public GActorObserver, public MTestMemPool<GPlayerEnchant>
{
public:
	GPlayerEnchant(GEntityPlayer* pOwner);
	virtual ~GPlayerEnchant(void);

private:
	// ������ ��ȭ ���� (Player ����)
	virtual void OnItemEnchantSuccess(GItem* pItem, int nEnchantItemID) override;
	// ������ ���� (Player ����)
	virtual void OnItemEquipped(GItem* pItem) override;
	// ������ �������� (Player ����)
	virtual void OnItemUnequipped(GItem* pItem) override;
	
private:
	void GainBuffFromEquip(GItem* pItem);
	void LostBuffFromEquip(GItem* pItem);
	bool IsSameWeaponSet(GItem* pItem) const;
	int GetLinkedBuffID(int nEnchantItemID) const;
private:
	GEntityPlayer*	m_pOwner;
};
