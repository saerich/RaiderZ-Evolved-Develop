#pragma once

#include "CSItemData.h"

class GItem;

class GOverlappedEquipmentSlot : public MTestMemPool<GOverlappedEquipmentSlot>
{
public:
	GOverlappedEquipmentSlot(void);
	~GOverlappedEquipmentSlot(void);

	bool IsOverlapped() const;

	void Reset();

	GItem*	GetLeftWeapon() const;				///< ���� �޼տ� ����� ���� ��������  �ν��Ͻ�
	GItem*	GetRightWeapon() const;				///< ���� �����տ� ����� ���� ��������  �ν��Ͻ�	
	GItem*	GetItem(SH_ITEM_SLOT nSlot) const;
	void	SetItem(SH_ITEM_SLOT nSlot, GItem* pItem);

	void EquipLeftWeapon(GItem* pItem);
	void EquipRightWeapon(GItem* pItem);

private:
	vector<GItem*>	m_vecEquipItems;				// ����� ��� ������
	bool m_bOverlapped;
};
