#ifndef XPLAYER_H
#define XPLAYER_H

#include "XActor.h"

class XPlayerPVP;
class XModulePlayerData;
class XDuel;

/// �÷��̾� Ŭ����
class XPlayer : public XActor
{
	MDeclareRTTI;
protected:
	XModulePlayerData*				m_pModulePlayerData;

	CHAR_STANCE						m_nStance;
	bool							m_bSwim;
	bool							m_bSitting;		// status(stance, swim, sitting)�� ���߿� �Ѱ����� ��ƾ� �ҵ�. - birdkr
	bool							m_bAFK;		// status(stance, swim, sitting)�� ���߿� �Ѱ����� ��ƾ� �ҵ�. - birdkr
	bool							m_bCutScene;

	XPlayerPVP*						m_pPlayerPVP;		///< �÷��̾�鳢�� Faction�� �ٸ��� ���� ������ �ȴ�.
	XDuel*							m_pDuel;			///< �ϴ� 1:1�� ��Ƽvs��Ƽ�� �߰�����

protected:
	virtual void					RegisterModules();
	virtual void					UnregisterModules();
	virtual bool					OnCreate();

public:
	XPlayer(MUID uid, UIID nUIID);
	virtual ~XPlayer();

	virtual XModulePlayerData*		GetModulePlayerData()			{ return m_pModulePlayerData; }

	virtual ARMOR_TYPE				GetArmorType()		{ return ARMOR_NONE;}		// MyPlayer, NetPlayer ���� �ۼ��ؾ��Ѵ�.
	virtual WEAPON_TYPE				GetCurrWeaponType() { return WEAPON_NONE;	}	// MyPlayer, NetPlayer ���� �ۼ��ؾ��Ѵ�.
	virtual WEAPON_TYPE				GetEquipWeaponType(bool bLeft){ return WEAPON_NONE;	}	// MyPlayer, NetPlayer ���� �ۼ��ؾ��Ѵ�.
	virtual bool					IsEquipShield()		{ return false;			}	// MyPlayer, NetPlayer ���� �ۼ��ؾ��Ѵ�.
	virtual void					GetWeaponParentName(wstring& strRWeapon, wstring& strLWeapon) {} // MyPlayer, NetPlayer ���� �ۼ��ؾ��Ѵ�.
	virtual void					GetCurrentWeaponSetWeaponItemData(XItemData** pItemDataR, XItemData** pItemDataL) {}

	bool							IsCurrStanceBattle();
	bool							IsSwim()							{ return m_bSwim; }
	bool							IsSitting()							{ return m_bSitting; }
	bool							IsCutScene()						{ return m_bCutScene; }
	bool							IsAFK()								{ return m_bAFK; }

	void							SetStance(CHAR_STANCE nStance)		{ m_nStance = nStance; }
	void							SetSwim(bool bSwim)					{ m_bSwim = bSwim; }
	void							SetSitting(bool bSitting)			{ m_bSitting = bSitting; }
	void							SetAFK(bool bVar)					{ m_bAFK = bVar; }
	void							SetDuel( bool bDuel, MUID OpponentUID);
	void							SetBattleArenaEnemy();
	void							SetCutScene(bool bEnterCutScene);


	CHAR_STANCE						GetStance()							{ return m_nStance; }
	float							GetMoveSpeedDefault(unsigned int nMovementFlags);	// �������� ���� �⺻ �̵��ӵ�
	float							GetMoveSpeed(unsigned int nMovementFlags);			// ������ �̵��ӵ�

	void							ChangeStance(CHAR_STANCE nStance, bool bQuick);
	void							ChangeWeapon(CHAR_STANCE nSwitching, bool bQuick);

	virtual const wchar_t*			GetName() { return L""; }
	virtual SEX						GetSex()	{ return SEX_MALE; }
	virtual RACE					GetRace()	{ return RACE_HUMAN; }
	virtual bool					IsPlayer() { return true; }

	bool							IsAlly(XPlayer* pTargetPlayer);
	bool							IsEnemy( XPlayer* pTargetPlayer );

	//bool IsAllyInPvPArea( XPlayer* pTargetPlayer );
	bool							IsCollidableTo(XPlayer* pTarPlayer);		/// �÷��̾�鳢�� �浹 üũ�� ���� ����

	bool							IsDuel();
	XPlayerPVP*						GetPlayerPVP() { return m_pPlayerPVP; }
	virtual const wchar_t*			GetRealNameInPvP() { return L""; }

	virtual int						GetVisualEnchantBuffID() { return 0; }

	virtual const wstring			GetCaptionName();
	virtual void					SetCharacterPane_Player()		{}
	virtual void					SetCharacterPane_VSNetPlayer()	{}
	//virtual void					SetCharacterPane_AllyNetPlayer(){}

	// ����
	XDuel*							GetPlayerDuel() const { return m_pDuel; }
	void							RemoveOpponentUID(MUID OpponentUID);
	void							SetOpponentUIDs(const vector<MUID>& vecUIDs);

	// �극��Ŀ�� ���� ����ȭ
	virtual void					EquipOverlapped(int nItemID);
	virtual void					UnEquipOverlapped(int nItemID);
	bool							IsUsableBreakablePartsWeapon();
	
};




#endif
