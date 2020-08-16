#pragma once

#include "CSItemData.h" // for ITEMSLOT_NONE
#include "CSChrInfo.h"
#include "CSWorldInfo.h"
#include "CSBasicAttributeCalculator.h"
#include "XMyInventory.h"
#include "XEquipmentSlot.h"
#include "XMyTalent.h"
#include "XTradeContents.h"
#include "XParty.h"
#include "XPaletteContainer.h"
#include "XPlayerInfo.h"
#include "XPlayerQuest.h"
#include "XRepairItemContainer.h"
#include "XGuild.h"
#include "XBattleArenaInfo.h"
#include "XSystem.h"
#include "XPlayerFaction.h"
#include "XMyLootingItemContainer.h"
#include "XCraftableRecipeInfo.h"
#include "XMyRecipeInfo.h"
#include "XMailbox.h"
#include "XMyStorage.h"
#include "XMyGuildStorage.h"
#include "XBuffContainer.h"

/// ���������� ����ϴ� �� ����
class GlobalVar_MyInfo
{
private:
	UIID			m_nUIID;			///< �� Indexed ID
	float			m_fExpPercent;	///< ����ġ �ۼ�Ʈ��

public:
	MUID			MyUID;			///< �� UID
	wstring			strLoginID;		///< �� ���� ID
	PLAYER_INFO		ChrInfo;		///< �� ĳ���� ����
	CHR_STATUS		ChrStatus;		///< �� ĳ���� ����
	CSPlayerModifier PlayerModifier;	///< �� ĳ���� ������̾�
	
	CSActorModifier	 ActorModifier_ExceptTalent;
	CSPlayerModifier PlayerModifier_ExceptTalent;

	int				nLevel;			///< ����

	// ī��Ʈ ����
	int				nRecievedQuest;
	int				nCompletedQuest;
	int				nRecievedRecipe;
	int				nCompletedRecipe;
	int				nKilledEnemy;
	int				nKilledEpicEnemy;

	// ����Ʈ
	XPlayerQuests				Quests;				///< ����Ʈ
	XMyRecipeInfo				RecipeInfo;			///< ������

	// Party
	XParty			Party;

	// ������ ����
	XMyEquipmentSlot	EquipmentSlot;		///< ������ ��� ����
	XMyInventory		Inventory;			///< ������ �κ��丮
	XMyStorage			Storage;			///< ������
	XMyGuildStorage		GuildStorage;		///< ��� ������
	XTradeContents		TradeContents;		///< �ŷ�����

	XMyLootingItemContainer		LootingItem;			///< ���þ����۸���Ʈ
	XMyLootingItemContainer		AssignLootingItem;		///< ��� ���� ���þ����۸���Ʈ

	// ��ų ����
	XMyTalent				Talent;
	XMyBuffContainer		BuffList;			///< ����ȿ������Ʈ

	XCraftableRecipeInfo	CraftableRecipeInfo;	///< ���۰����� ������ ����Ʈ

	//�ȷ�Ʈ
	XPaletteManager		PaletteList;

	// ������
	XMailbox				Mailbox;

	XPlayerFactions			FactionInfo;

	bool					bGod;			///< ��������� ����

	FATIGUE_TYPE	nFatigueType;

	XGuild			Guild;	///< ���

	XBattleArenaInfo	BattleArenaInfo;	///< ������

	//map<TALENT_FOCUS_TYPE, int> BubblePercent;	///< ����
	TALENT_FOCUS_TYPE	nFocusType;
	int					nBubblePercent;

	unsigned int		nDieTime;					///< �����ð�
	float				fAutoRebirthRemainCoolTime; ///< ��Ȱ���� ���� �ð�

	int					nSoulbindingFieldID;
	int					nCheckPointFieldID;

	GlobalVar_MyInfo();
	~GlobalVar_MyInfo();

	void	Clear();

	int		GetHP()	const		{ return ChrStatus.nHP; }
	int		GetEN()	const		{ return ChrStatus.nEN; }
	int		GetSTA() const		{ return ChrStatus.nSTA; }
	int		GetMaxHP() const	{ return max(1, ChrStatus.ActorModifier.nHPMax.Calc(CSBasicAttributeCalculator::CalcMaxHP(nLevel, GetCON()))); }
	int		GetMaxEN() const	{ return max(1, ChrStatus.ActorModifier.nENMax.Calc(CSBasicAttributeCalculator::CalcMaxEN(nLevel, GetINT()))); }
	int		GetMaxSTA() const	{ return max(1, ChrStatus.ActorModifier.nSTAMax.Calc(CSBasicAttributeCalculator::CalcMaxSP(nLevel, GetDEX()))); }
	int		GetSTR() const		{ return PlayerModifier.nSTR.Calc(ChrInfo.nSTR); }
	int		GetDEX() const		{ return PlayerModifier.nDEX.Calc(ChrInfo.nDEX); }
	int		GetINT() const		{ return PlayerModifier.nINT.Calc(ChrInfo.nINT); }
	int		GetCHA() const		{ return PlayerModifier.nCHA.Calc(ChrInfo.nCHA); }
	int		GetCON() const		{ return PlayerModifier.nCON.Calc(ChrInfo.nCON); }

	int		GetModAP()
	{
		int ap = EquipmentSlot.GetAP();
		return ActorModifier_ExceptTalent.nAP.Calc(ap) - ap;
	}

	float	GetExpPercent() const { return m_fExpPercent; }
	void	SetExpPercent(float fPercent)		{ m_fExpPercent = fPercent; }

	void SetHP(int nHP)
	{
#ifndef _PUBLISH
		if (this->bGod)
		{
			ChrStatus.nHP = ChrInfo.nMaxHP;
			return;
		}
#endif
		ChrStatus.nHP = max(0, min(nHP, GetMaxHP()));
	}
	void SetEN(int nEN)
	{
#ifndef _PUBLISH
		if (this->bGod)
		{
			ChrStatus.nEN = ChrInfo.nMaxEN;
			return;
		}
#endif
		ChrStatus.nEN = max(0, min(nEN, GetMaxEN()));
	}
	void SetSTA(int nSTA)
	{
#ifndef _PUBLISH
		if (this->bGod)
		{
			ChrStatus.nSTA = ChrInfo.nMaxSTA;
			return;
		}
#endif
		ChrStatus.nSTA = max(0, min(nSTA, GetMaxSTA()));

	}

	void SubChrStatus(XTalentInfo* pTalentInfo);

	void Assign(const TD_MYINFO& td_myinfo);
	void ResetByMoveGameServer();

	TD_MYINFO AsTransDataMyInfo();

	void SetFrom_TD_MYINFO(
		const TD_MYINFO& TDMyInfo, 
		const vector<TD_ITEM>& vecTDInventory, 
		const vector<TD_ITEM>& vecTDEquipment,
		const vector<int>& vecTalent, 
		const vector<TD_PLAYERQUEST>& vecTDPlayerQuest,
		const TD_PALETTE& TDPalette, 
		const vector<TD_FACTION>& vecTDFaction, 
		const vector<int>& vecRecipeID);

	bool BuffExist(int nID)
	{
		return BuffList.FindID(nID);
	}

	void SetUIID(UIID nUIID) { m_nUIID = nUIID; }
	UIID GetUIID() { return m_nUIID; }
};
