/************************************************************************/
/* 1. ĳ���������� ���ϴ�.                                              */
/************************************************************************/

#ifndef _XITEM_H
#define _XITEM_H

#include "XItemData.h"
#include "CSChrInfo.h"
#include "CTransData.h"

class XItem
{
public:
	XItem();
	virtual ~XItem();

	int				m_nSlotID;					// ������ ���� ID
	XItemData*		m_pItemData;				// ������ �Ķ���͵�

	// ������� �� ������ �ν��Ͻ����� �޶��� �� �ִ� ������
	int				m_nAmount;					// ���� (��ġ�� ������ ���)
	int				m_nDurability;				// ����
	int				m_nDyedColor;				// ������ ��
	bool			m_bClaimed;					// �ͼ� �Ǿ��°�?
	uint8			m_nSoulCount;				// �ҿ� ��� Ƚ��
	bool			m_bNewItem;					// �ֱٿ� ������ ���������� ����
	int				m_nEnchants[ENCHANT_MAX_COUNT];		// ��æ ��� (ù��°�� Ȱ��ȭ ��æƮ)

	void			Set(const MUID& id, XItemData* dat);
	void			ApplyTDItem(const TD_ITEM& tdItem);

	int				GetTalentID();				///< �����ۿ� ���ε� �� Ż��Ʈ ���̵�
	bool			IsUsableItem();
	bool			IsQuestRelated();
	int				GetID();
};
#endif // _XITEM_H
