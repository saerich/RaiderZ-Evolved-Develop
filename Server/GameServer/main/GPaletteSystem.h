#ifndef _G_PALETTE_SYSTEM_H_
#define _G_PALETTE_SYSTEM_H_

#include "CSPalette.h"

class GEntityPlayer;
struct TD_PALETTE;

class GPaletteSystem : public MTestMemPool<GPaletteSystem>
{
public:
	GPaletteSystem();
	~GPaletteSystem();

	uint8 NumAndSlotToIndex(PALETTE_NUM nNum, PALETTE_SLOT nSlot);
	void IndexToNumAndSlot(uint8 nIndex, PALETTE_NUM& outnNum, PALETTE_SLOT& outnSlot);

	bool IsValidNumAndSlot(PALETTE_NUM nNum, PALETTE_SLOT nSlot);
	bool IsValidNum(PALETTE_NUM nNum);
	bool IsValidSlot(PALETTE_SLOT nSlot);	

	bool Select(GEntityPlayer* pPlayer, PALETTE_NUM nNum);			///< �ȷ�Ʈ ����
	bool SetPrimary(GEntityPlayer* pPlayer,  PALETTE_NUM nNum);		///< ����� �ȷ�Ʈ ���ε�
	bool SetSecondary(GEntityPlayer* pPlayer,  PALETTE_NUM nNum);	///< ������� �ȷ�Ʈ ���ε�
	
	bool PutUp(GEntityPlayer* pPlayer, PALETTE_NUM nNum, PALETTE_SLOT nSlot, PALETTE_ITEM_TYPE nType, int nItemIDorTalentID, bool bDBUpdate = true);
	bool PutDown(GEntityPlayer* pPlayer, PALETTE_NUM nNum, PALETTE_SLOT nSlot, bool bDBUpdate = true);
	bool PutDown(GEntityPlayer* pPlayer, int nItemIDorTalentID, bool bDBUpdate = true);	
	bool Change(GEntityPlayer* pPlayer, PALETTE_NUM nNum1, PALETTE_SLOT nSlot1, PALETTE_NUM nNum2, PALETTE_SLOT nSlot2);

	bool PutDownAllTalent(GEntityPlayer* pPlayer);

	void MakeTD_PALETTE(GEntityPlayer* pPlayer, TD_PALETTE& outtdPalette);
};

#endif
