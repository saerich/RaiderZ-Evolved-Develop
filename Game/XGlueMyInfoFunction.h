#pragma once

#include "XGlueFunction.h"

/// lua���� ����ϴ� ��ĳ������ API(read ����)
class XGlueMyInfoFunction : public XGlueFunction
{
public:
	virtual void DeclGlueFunc(MWLua* pLua);

	// ĳ���� ���� ----
	__int64					GetUID()			{ return gvar.MyInfo.MyUID.Value; }
	/// �̸�(ID)�� �ѱ��.
	const char*				GetID();
	/// First Name�� �ش��ϴ� �̸��� �ѱ��.
	const char*				GetSurName();
	/// ���������� �ѱ��.
	int						GetLevel()			{ return gvar.MyInfo.nLevel; }
	/// �ǹ�(��)
	int						GetSilver()			{ return gvar.MyInfo.ChrInfo.GetMoney(); }
	/// ����ġ �ۼ�Ʈ�� �ѱ��.
	int						GetExpPercent()		{ return gvar.MyInfo.GetExpPercent(); }
	/// ����ġ
	int						GetExp()			{ return gvar.MyInfo.ChrInfo.nXP; }
	/// ���� ��ġ�� �� �̸��� �ѱ��.
	const char*				GetFieldName();
	int						GetFieldID();
	int						GetChannelID();
	bool					IsDynamicField();
	/// �÷��̾� ���
	int						GetPlayerGrade()	{ return (int)gvar.MyInfo.ChrInfo.nPlayerGrade; }

	// �ɷ� ���� ----
	/// �Ϸ°��� �ѱ��.
	int						GetSTR()	{ return gvar.MyInfo.ChrInfo.nSTR; }
	/// ȸ�ǰ��� �ѱ��.
	int						GetDEX()	{ return gvar.MyInfo.ChrInfo.nDEX; }
	/// ���°��� �ѱ��.
	int						GetINT()	{ return gvar.MyInfo.ChrInfo.nINT; }
	/// Con���� �ѱ��.
	int						GetCON()	{ return gvar.MyInfo.ChrInfo.nCON; }
	/// ī���������� �ѱ��.
	int						GetCHA()	{ return gvar.MyInfo.ChrInfo.nCHA; }

	// MOD ----
	/// �Ϸ��� �߰����� �ѱ��.
	int						GetModSTR() { return gvar.MyInfo.PlayerModifier.nSTR.nMod; }
	/// ȸ���� �߰����� �ѱ��.
	int						GetModDEX() { return gvar.MyInfo.PlayerModifier.nDEX.nMod; }
	/// ������ �߰����� �ѱ��.
	int						GetModINT() { return gvar.MyInfo.PlayerModifier.nINT.nMod; }
	// Con�� �߰����� �ѱ��.
	int						GetModCON() { return gvar.MyInfo.PlayerModifier.nCON.nMod; }
	/// ī�������� �߰����� �ѱ��.
	int						GetModCHA() { return gvar.MyInfo.PlayerModifier.nCHA.nMod; }

	// ���� ���� ----
	/// ȭ�Ӽ� ���׷��� ���� �ѱ��.
	int						GetFR()		{ return gvar.MyInfo.ChrInfo.Resist.nValue[TRT_FR]; }
	/// ���Ӽ� ���׷��� ���� �ѱ��.
	int						GetCR()		{ return gvar.MyInfo.ChrInfo.Resist.nValue[TRT_CR]; }
	/// ǳ�Ӽ� ���׷��� ���� �ѱ��.
	int						GetLR()		{ return gvar.MyInfo.ChrInfo.Resist.nValue[TRT_LR]; }
	/// ���Ӽ� ���׷��� ���� �ѱ��.
	int						GetPR()		{ return gvar.MyInfo.ChrInfo.Resist.nValue[TRT_PR]; }
	/// Holy ���׷��� ���� �ѱ��.
	int						GetHR()		{ return gvar.MyInfo.ChrInfo.Resist.nValue[TRT_HR]; }
	/// Unholy ���׷��� ���� �ѱ��.
	int						GetUR()		{ return gvar.MyInfo.ChrInfo.Resist.nValue[TRT_UR]; }

	// ���� ���� Mod��
	int						GetModFR()		{ return gvar.MyInfo.ChrStatus.ActorModifier.nResistances[TRT_FR].nMod; }
	int						GetModCR()		{ return gvar.MyInfo.ChrStatus.ActorModifier.nResistances[TRT_CR].nMod; }
	int						GetModLR()		{ return gvar.MyInfo.ChrStatus.ActorModifier.nResistances[TRT_LR].nMod; }
	int						GetModPR()		{ return gvar.MyInfo.ChrStatus.ActorModifier.nResistances[TRT_PR].nMod; }
	int						GetModHR()		{ return gvar.MyInfo.ChrStatus.ActorModifier.nResistances[TRT_HR].nMod; }
	int						GetModUR()		{ return gvar.MyInfo.ChrStatus.ActorModifier.nResistances[TRT_UR].nMod; }

	// ���� ���� ----
	/// ������� ���� �ѱ��.
	int						GetHP()		{ return gvar.MyInfo.ChrStatus.nHP; }
	/// ���ŷ��� ���� �ѱ��.
	int						GetEN()		{ return gvar.MyInfo.ChrStatus.nEN; }
	/// ����� ���� �ѱ��.
	int						GetSTA()	{ return gvar.MyInfo.ChrStatus.nSTA; }
	/// �ִ� ������� ���� �ѱ��.
	int						GetMaxHP()	{ return gvar.MyInfo.GetMaxHP(); }
	/// �ִ� ���ŷ��� ���� �ѱ��.
	int						GetMaxEN()	{ return gvar.MyInfo.GetMaxEN(); }
	/// �ִ� ����� ���� �ѱ��.
	int						GetMaxSTA() { return gvar.MyInfo.GetMaxSTA(); }

	/// ��Ŀ�� �ۼ�Ʈ�� �ѱ��.
	const char*				GetFocusType();
	int						GetFocusPercent()	{ return gvar.MyInfo.nBubblePercent; }

	// �κ� ���� ----
	/// ������ ������ �ѱ��.
	//void					GetItemInfo(unsigned int nItemID_High, unsigned int nItemID_Low);
	/// �������� �̸��� �ѱ��.
	const char*				GetItemName(unsigned int nItemID_High, unsigned int nItemID_Low);
	const char*				GetInventoryItemName( const char* szuid );
	int						GetInventoryItemCount();
	int						GetInventoryItemIDCount(int nItemID);

	int						GetInventoryUIDItemCount(const char* szuid);
	const char*				GetInventoryItemType( const char* szuid);
	int						GetInventoryItemID( const char* szuid );

	// ������� ���� ----
	/// slot�� ������ �������� �ѱ��.
	const char*				GetEquippedItemName( const char* szuid );
	int						GetEquippedItemID( const char* szSlot );
	__int64					GetEquippedItemUID(const char* szSlot);
	const char*				GetEquippedItem(const char* szSlot);
	void					SetEquipItem( const char* szSlot, const char* szuid, bool bPass = false );
	void					SetUnEquipItem(  const char* szSlot);

	// �������� ����
	int				GetWeaponType()	{ return gvar.MyInfo.EquipmentSlot.GetCurrWeaponType();}

	// �ŷ�Ʈ ����
	int						GetTP()		{ return gvar.MyInfo.Talent.GetTP();}
	const char*				GetLearnedTalentList();
	bool					IsLearnedTalent( int nTalentID);
	int						GetTalentCooldownRemainPercent(int nTalentID);
	int						GetStyleUsedTalent(int index);

	// ������ ����
	bool					IsQuestItem(const char* szuid);

	// �ɸ� �ŷ�Ʈ

	// ���� ��뿡 ���� ó��

	// �ŷ�
	int						GetMyTradingContainerSize();
	__int64					GetMyTradingContainerItemUID(int index );
	int						GetMyTradingSilver();

	int						GetOtherTradingContainerSize();
	__int64					GetOtherTradingContainerItemUID(int index );
	const char*				GetOtherTradingContainerItemName( const char* szuid);
	int						GetOtherTradingSilver();

	//Buff
	//const char*				GetBuffList();

	// EnemyInfo
	MWLua::table			GetEnemyInfoTable();

	//��Ƽ����
	//const char*				GetPartyMemberBuffIDList(const char* szuid);

	//Quest
	const char*				GetQuestIDList();
	int						GetQuestProgress(int nQuestID, int nObjID );
	unsigned int			GetQuestRemainTime( int nQuestID );
	bool					IsQuestFail( int nQuestID );
	bool					IsQuestTypeTimeAttack( int nQuestID );

	float					GetFieldMyPosX();
	float					GetFieldMyPosY();

	int						GetInvenEquipItemDurability( const char* szuid);
	int						GetInvenEquipItemID( const char* szuid);
	int						GetRepairableItemCount();
	int						GetTotalRepairPrice();

	//���
	const char*				GetGuildMemberList();
	int						GetGuildMemberGrade( const char* szName );
	const char*				GetGuildMemberFieldName( const char* szName );
	int						GetGuildMemberChannelID( const char* szName );
	bool					IsGuildMemberOnline( const char* szName );
	const char*				GetGuildName();
	bool					IsGuildMaster();

	//�ȷ�Ʈ
	void					AddPaletteTalent( int index, const char* szID );
	void					AddPaletteItem(int index, const char* szuid);
	//const char*				GetPaletteList();
	void					UsePaletteIndex(int index);
	void					TakeAwayPalette(int nSlot);	//�ȷ�Ʈ���� ����

	void					ChangePalette(int nNum1, int nSlot1, int nSlot2);
	void					ChangePaletteList(int nIndex);
	int						GetPaletteListIndex();			//<<< ���� �ȷ�Ʈ����Ʈ�� �ε���.
	void					WeaponBinding(int index);
	bool					PrimaryBinding();
	bool					SecondaryBinding();

	//faction

	MWLua::table			GetFactionInfoTable();
	// ��Ÿ
};
