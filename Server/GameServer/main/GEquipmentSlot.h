#pragma once

#include "GItemContainer.h"
#include "CSItemData.h"
#include "GItem.h"
#include "GModEffector.h"

class GOverlappedEquipmentSlot;

/// ��� ����
class GEquipmentSlot: public GItemContainer
{
public:
	GEquipmentSlot(GEntityPlayer* pOwner);
	virtual ~GEquipmentSlot();

	virtual void			CollectAllItem(vector<GItem*>& outvecItem) override;	///< ���� ����� ��� �������� �ν��Ͻ�	
	virtual bool			PushItem(int nSlotID, GItem* pItem) override;		///< ���� ������ �������� ����.;	
	virtual GItem*			PopItem(int nSlotID) override;						///< ���� ���� ������ �������� ����.;		

	void					AddModifierByTrain(TALENT_EXTRA_PASSIVE_TYPE nTrainTEPT);
	void					EraseModifierByUntrainAll();
	void					AddModifierByEquip(GItem* pItem);
	void					EraseModifierByUnequip(GItem* pItem);

	void					AddModifier(GItem* pItem);
	void					EraseModifier(GItem* pItem );

	GItem*					GetLeftWeapon() const;				///< ���� �޼տ� ����� ���� ��������  �ν��Ͻ�
	GItem*					GetRightWeapon() const;				///< ���� �����տ� ����� ���� ��������  �ν��Ͻ�	
	GItem*					GetArmor() const;					///< �������� ���� ��ȯ
	GItem*					GetShield() const;					///< ���� ����� ���� ��������  �ν��Ͻ�
	GItem*					GetItemFromSubSlot(SH_ITEM_SLOT nSubSlot)  const;	

	void					CollectArmorSlotItem(vector<GItem*>& outvecEquipmentItem);	///< ���� ����� ���и� ������ ��� ���� �ν��Ͻ�	

	WEAPON_TYPE				GetWeaponType() const;
	WEAPON_TYPE				GetLeftWeaponType() const;			///< ���� �޼տ� ����� ���� �������� Ÿ��
	WEAPON_TYPE				GetRightWeaponType() const;			///< ���� �����տ� ����� ���� �������� Ÿ��
	ARMOR_TYPE				GetArmorType() const;				///< ���� �������� �� �������� Ÿ��
	ARMOR_TYPE				GetSlotMaterial(SH_ITEM_SLOT nSlot);

	int						GetTotalAP();						// ��ü ����� AP. �̰� ����ϰ� ȣ��Ǵ� ���߿� ���� ������ �־���� �Ѵ�. - birdkr
	SH_ITEM_SLOT			GetEquipedSlot(const GItem* pItem) const;
	SH_ITEM_SLOT			GetSubSlot(SH_ITEM_SLOT nSlot);
	
	void					SwitchingWeaponSet(SH_ITEM_SWITCH_WEAPON val);
	SH_ITEM_SWITCH_WEAPON	GetWeaponSet() const;	
	
	bool					IsEquipItem(SH_ITEM_SLOT nSlot);
	bool					IsEquipItem(int nItemID);	
	bool					IsEquipItem(GItem* pItem);
	bool					IsEquipShield() const;
	bool				 	IsUnarmed() const;
	bool				 	IsWeaponSwitchable(SH_ITEM_SWITCH_WEAPON nWeaponSet) const;

	GOverlappedEquipmentSlot& GetOverlapped();

private:
	void					SetWeaponSet(SH_ITEM_SWITCH_WEAPON val);
	bool				 	IsUnarmedByWeaponSet(SH_ITEM_SWITCH_WEAPON nWeaponSet) const;
	WEAPON_TYPE				GetLeftWeaponTypeByWeaponSet(SH_ITEM_SWITCH_WEAPON nWeaponSet) const;
	WEAPON_TYPE				GetRightWeaponTypeByWeaponSet(SH_ITEM_SWITCH_WEAPON nWeaponSet) const;	
	GItem*					GetLeftWeaponByWeaponSet(SH_ITEM_SWITCH_WEAPON nWeaponSet) const;
	GItem*					GetRightWeaponByWeaponSet(SH_ITEM_SWITCH_WEAPON nWeaponSet) const;


private:		
	SH_ITEM_SWITCH_WEAPON	m_nWeaponSet;			// ���� ���� ��ȣ (�ֹ���/��������)
	GModEffector			m_ModEffector;
	GOverlappedEquipmentSlot*	m_pOverlapped;
};
