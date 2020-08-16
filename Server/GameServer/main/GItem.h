#ifndef _GITEM_H
#define _GITEM_H

#include "MMemPool.h"
#include "GItemData.h"
#include "CSChrInfo.h"
#include "CTransData.h"
#include "GItemPeriod.h"

class GItemManager;
class TestItemManager;
struct TD_ITEM;
struct TD_PLAYER_GAME_DATA_ITEM_INSTANCE;
class GBuffInfo;

TEST_FORWARD_FT(MailItemTaker, Fixture, TestMailItemTaker_Take_Failure_OverMaxStackAmount);
//TEST_FORWARD_FT(Enchant, FEnchant, Enchant_ActiveBuff);
//TEST_FORWARD_FT(Enchant, FEnchant, EnchantBuff);

/// ������ �ν��Ͻ�
class GItem : public MMemPool<GItem>
{
TEST_FRIEND_FT(MailItemTaker, Fixture, TestMailItemTaker_Take_Failure_OverMaxStackAmount);
//	TEST_FRIEND_FT(Enchant, FEnchant, Enchant_ActiveBuff);
//	TEST_FRIEND_FT(Enchant, FEnchant, EnchantBuff);
private:
	friend class TestItemManager;
	friend class GItemManager;
	friend class GItemContainer;
	friend class GGuildSystemForMasterServer;	
	
public:
	~GItem();

	MUID			GetUID() const				{ return m_uidID; }
	int				GetID();

	TD_ITEM								ToTD_ITEM();	
	TD_PLAYER_GAME_DATA_ITEM_INSTANCE	ToTD_PLAYER_GAME_DATA_ITEM_INSTANCE();
	TD_UPDATE_DURABILITY				ToTD_UPDATE_DURABILITY();
	TD_ITEM_INSERT						ToTD_ITEM_INSERT();

	bool			IsEquiped() const			{ return m_bEquiped; }
	void			SetEquiped(bool val)		{ m_bEquiped = val; }

	int				GetLostedDurability();
	void			GetExpiDtForDB(wstring& strExpiDt);

	bool			IsInteractableType();
	bool			IsExpired(const int nCharPlayTimeMin);
	bool			IsFineDuration() const;
	bool			IsPeriod() const;
	
	// ��ȭ ���� ����
	bool			IsEnchantable(GBuffInfo* pBuffInfo) const;
	// ��ȭ
	int8 Enchant(GBuffInfo* pBuffInfo, int nItemID, ENCHANT_COLOR nEnchantColor);
	// ��ȭ ����
	int8 EnchantBreak(GBuffInfo* pBuffInfo);
	// ��Ƽ�� ��ȭ���� ID ���
	int				GetActiveEnchantBuffID() const;
	// ��Ƽ�� ��ȭ������ �ִ��� ����
	bool			HasActiveEnchantBuff() const;
	// ��ȭ Ƚ��
	int				GetEnchantCount() const;
	
	bool			operator== (const GItem* pItem);
	bool			operator!= (const GItem* pItem);

	int8			GetEmptyEnchantSlotIdx(bool bIsActiveSlot, ENCHANT_COLOR nColor=ENCHANT_NONE);
	int				GetAmount() const;

private:
	GItem();
	void			Set(const MUID& id, GItemData* dat);

	// ��Ƽ�� ��ȭ ���� ����
	bool			IsEnchantableActiveBuff( GBuffInfo* pBuffInfo) const;
	int8 AddEnchantSlot(bool bIsActiveSlot, int nBuffID, ENCHANT_COLOR nEnchantColor=ENCHANT_NONE);


public:
	int64			m_nIUID;					// IUID - DB�� ID
	MUID			m_uidID;					// ������ �ν��Ͻ��� ���� ID
	GItemData*		m_pItemData;				// ������ �Ķ���͵�

	// ������� �� ������ �ν��Ͻ����� �޶��� �� �ִ� ������
	bool			m_bEquiped;		
	int				m_nDurability;				// ����

	int				m_nDyedColor;				// ������ ����

	bool			m_bBind;					// �ͼ� �Ǿ�����?

	/// ������ ��� �ð�.
	GItemPeriod		m_UsagePeriod;				// ��� �ð�. ������ ������������ ���ҵȴ�.
	GItemPeriod		m_ExpirationPeriod;			// ��� ���� �ð�. ������ ������ ���� �ʾƵ� ���ҵȴ�. 
	wstring			m_strExpiDt;				// ���� �Ͻ� ��Ʈ�� ����.
	
	uint8			m_nSoulCount;				// ��ȥ ��� Ƚ��

	int				m_nEnchants[ENCHANT_MAX_COUNT];		// ��æƮ ��� (ù��°�� Ȱ��ȭ ��æƮ)
	
	SH_ITEM_SLOT_TYPE	m_nSlotType;			// ���� ����
	int16				m_nSlotID;				// ���� ��ġ

private:
	int				m_nAmount;					// ���� (��ġ�� ������ ���)
};


#endif // _GITEM_H