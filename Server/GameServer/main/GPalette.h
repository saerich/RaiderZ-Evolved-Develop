#ifndef _G_PALETTE_H_
#define _G_PALETTE_H_

#include "CSPalette.h"
#include "GPaletteItem.h" // for VEC_PALETTE_ITEM

class GEntityPlayer;
class GPaletteItem;


typedef vector<GPaletteItem> VEC_PALETTE_ITEM;

class GPalette : public MTestMemPool<GPalette>
{
private:
	GEntityPlayer*		m_pOwner;
	VEC_PALETTE_ITEM	m_vecPaletteItem;
	PALETTE_NUM			m_nSelectedPaletteNum;		///< ���� ���õ� �ȷ�Ʈ
	PALETTE_NUM			m_nPrimarySetPaletteNum;	///< ����� ���ε��� �ȷ�Ʈ
	PALETTE_NUM			m_nSecondarySetPaletteNum;	///< ������� ���ε��� �ȷ�Ʈ
	bool				m_bLock;	///< ����Ʈ �� ������ ���� �Ұ�

public:
	GPalette(GEntityPlayer* pOwner);
	~GPalette();

	void Lock();
	void Unlock();
	bool IsLocked() const { return m_bLock; }

	void SelectPalette(PALETTE_NUM nNum) { m_nSelectedPaletteNum = nNum; }	///< �ȷ�Ʈ ����
	PALETTE_NUM GetSelectedPalette() { return m_nSelectedPaletteNum; }

	void SetPrimary(PALETTE_NUM nNum);	///< ����� �ȷ�Ʈ ����
	PALETTE_NUM GetPrimary() { return m_nPrimarySetPaletteNum; }

	void SetSecondary(PALETTE_NUM nNum);///< ������� �ȷ�Ʈ ����
	PALETTE_NUM GetSecondary() { return m_nSecondarySetPaletteNum; }

	void Set(PALETTE_NUM nNum, PALETTE_SLOT nSlot, PALETTE_ITEM_TYPE nType, int nItemIDorTalentID);
	bool Reset(PALETTE_NUM nNum, PALETTE_SLOT nSlot);
	void ResetTalentSlot();
	void Change(PALETTE_NUM nNum1, PALETTE_SLOT nSlot1, PALETTE_NUM nNum2, PALETTE_SLOT nSlot2);

	GPaletteItem* GetPaletteItem(PALETTE_NUM nNum, PALETTE_SLOT nSlot);
	void GetNumAndSlotByTalentID(int nTalentID, vector<pair<PALETTE_NUM, PALETTE_SLOT>>& outvecPaletteNumAndSlot);
	void GetNumAndSlotByItemID(int nItemID, vector<pair<PALETTE_NUM, PALETTE_SLOT>>& outvecPaletteNumAndSlot);
	bool IsEmpty(PALETTE_NUM nNum, PALETTE_SLOT nSlot);
	int	GetTalentCountOnPalette();

	VEC_PALETTE_ITEM&	GetContainer()		{ return m_vecPaletteItem;		}
};

#endif
