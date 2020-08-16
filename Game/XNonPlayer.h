#ifndef _X_NON_PLAYER_H
#define _X_NON_PLAYER_H

#include "XActor.h"
#include "XNPCInfo.h"
#include "CTransData.h"
#include "XAnim.h"
#include "XChangeNPCColor.h"

//////////////////////////////////////////////////////////////////////////
class XModuleNonControl;
class XModuleBParts;
class XModuleSound;
class XModuleNPCData;
//class XModuleInteraction;
class XNPCIcon;

class XNonPlayer : public XActor
{
	MDeclareRTTI;
private:
	XChangeNPCColor					m_changeTexColor;

protected:
	XModuleNPCData*					m_pModuleNPCData;

	XNPCInfo*						m_pNPCInfo;
	XModuleNonControl*				m_pModuleNonControl;
	XModuleBParts *					m_pModuleBParts;
	XModuleSound*					m_pModuleSound;
	XNPCIcon*						m_pNPCIcon;

	XIDContainer					m_ctBuffList;			///< ����ȿ������Ʈ

	int								m_nNPCID;				// �ٽ� ���ε��ϱ� ����...
	int								m_nMode;
	int								m_nLevel;
	bool							m_bLootable;			//���ð����÷���
	uint32							m_nLootableLastTime;	// ���ð����ϰ� ������ �ֱ� �ð�
	NPC_ATTACKABLE_TYPE				m_nAttackableType;
	bool							m_bCombating;			// ��׷θ� ���� NPC

	bool							m_bDeSpawn;
	bool							m_bHasCombatIdleAnimation;

	DWORD							m_dwNameColor;

	NPC_STUN_USE_ANIMATION			m_eStunAnimation;

protected:
	virtual void					RegisterModules();
	virtual void					UnregisterModules();
	virtual void					OnUpdate(float fDelta);
	virtual bool					OnCreate();

	void							UpdateMiniHpBar();
	void							UpdateTexColor(float fDelta);

	void							CheckStunAnimation();
	void							CheckCombatIdleAnimation();		///< ������ idle �ִϸ��̼��� �ֳ� ���� üũ
	void							CheckFighting();

	FACTION_RELATION_TYPE			GetFactionType();
	XNPCInfo*						GetInfo(int nMode);

	void							SetNPCIcon();

public:
	XNonPlayer(MUID uid, UIID nUIID);
	virtual ~XNonPlayer();

	virtual XObjectID				GetType() { return XOBJ_NPC; }
	virtual XModuleNonControl*		GetModuleNonControl()		{ return m_pModuleNonControl; }
	virtual XModuleActorControl*	GetModuleActorControl();
	virtual XModuleBParts *			GetModuleBParts()	{ return m_pModuleBParts; }
	virtual XModuleSound *			GetModuleSound()			{ return m_pModuleSound; }
	virtual XModuleNPCData*			GetModuleNPCData()			{ return m_pModuleNPCData; }
	virtual bool					Reload();
	virtual WEAPON_TYPE				GetCurrWeaponType();
	virtual WEAPON_TYPE				GetEquipWeaponType(bool bLeft);

	void							InitNPC(XNPCInfo* pNPCInfo, TD_UPDATE_CACHE_NPC* pNPCCacheInfo);
	void							ChangeMode(NPC_MODE nNextMode);

	XNPCInfo*						GetInfo(bool bCurrentMode=false);

	virtual bool					IsHuge();
	virtual bool					IsColliable();

	void							InNPC(XNPCInfo* pNPCInfo, TD_UPDATE_CACHE_NPC* pNPCCacheInfo, bool bAppearEffect = true, bool bLoadingAsync = true);
	int								GetMode()		{ return m_nMode; }
	bool							IsMonster();
	bool							IsInteract();
	int								GetLevel()		{ return m_nLevel; }
	virtual float					GetValidInteractionDistance();

	wstring							GetStunMotionAnimation();
	virtual wstring					GetModeAnimationNameHeader();

	void							SetLootable(bool bLootable);
	bool							IsLootable();
	bool							IsLootableRecently();

	NPC_ICON_STATE					GetIConState() const;

	bool							IsDeSpawn() const { return m_bDeSpawn; }
	void							SetDeSpawn(bool val) { m_bDeSpawn = val; }

	void							SetNPCNameColor(bool bAttackable, bool bFirstAttackable);

	NPC_ATTACKABLE_TYPE				GetAttackableType() const { return m_nAttackableType; }
	void							SetAttackableType(NPC_ATTACKABLE_TYPE val) { m_nAttackableType = val; }
	bool							IsAttackable();

	void							SetDirForwardToActor( XObject* pObject );

	bool							GetColTestObject();

	virtual const wchar_t*			GetName();
	virtual bool					IsNPC() { return true; }
	int GetNPCID() const			{ return m_nNPCID; }


	// ���� ����Ʈ
	void							PushBuff( int nBuffID)			{ m_ctBuffList.PushBackItem( nBuffID); }
	void							PopBuff( int nBuffID)			{ m_ctBuffList.PopItem( nBuffID); }
	XIDContainer&					GetBuffList()					{ return m_ctBuffList; }

//	bool							IsEnemy(NPC_ATTACKABLE_TYPE nAttackableType);
	bool							IsFisrtAttacker(NPC_AA_TYPE nAAType);

	void							SetCombating(bool bCombating)		{ m_bCombating = bCombating; }
	bool							IsCombating()						{ return m_bCombating; }

	bool							HasCombatIdleAnimation() { return m_bHasCombatIdleAnimation; }

	void							SetNPCIConTypeToIconState( NPC_ICON_TYPE NIT );
	void							SetNPCIConState( NPC_ICON_STATE NIS );
	void							SetPreviousNPCIcon();

	float							GetBallonLifeTime(float fMaintainTime, wstring &szChat);
	void							DestroyNPCIconEffect();

	void							CheckAnimation();			// Ȯ���ϰ� ���� �ִϸ��̼� �Լ��� ���⿡�� �ִ´�.

	bool							GetUseDieAni();

	bool							IsEquipShield();	
	bool							IsShowAggro();

	void							GetNPCWeaponItemData(CSItemData** pItemDataR, CSItemData** pItemDataL);

	virtual void					Despawn();
};








#endif