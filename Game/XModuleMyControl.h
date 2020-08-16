#ifndef _XMODULE_MY_CONTROL_H
#define _XMODULE_MY_CONTROL_H

#include "XVirtualKeyDef.h"
#include "XMyActionState.h"
#include "CSMotionFactorInfo.h"
#include "XModuleActorControl.h"

#include "XMyHitController.h"
#include "XPvPAreaChecker.h"
#include "XMyCharacterAlphaMgr.h"
#include "XMapSignAreaChecker.h"
#include "XRestrictedAreaChecker.h"

class XModuleEntity;
class XMyActionState;
class XMyPlayer;
class XSpecialActionTransChecker;
class MockController;
class XModulePost;
class XSensorController;
class XMouseMove;
class XTargetIndicator;
class XChallengerQuestExitChecker;

class XReserveAttack
{
public:
	int m_nTalentID;
	TALENT_NORMAL_ATTACK_TYPE	m_nNextAttackType;
	uint32 m_nReservedTime;
	XReserveAttack() : m_nNextAttackType(NA_NA) {}
	void Reserve(TALENT_NORMAL_ATTACK_TYPE	nNextAttackType)
	{
		m_nNextAttackType = nNextAttackType;
		m_nReservedTime = XGetNowTime();
	}
	void ReserveUseTalent(int nTalentID)
	{
		m_nTalentID = nTalentID;
		m_nNextAttackType = NA_USE_TALENT__CLIENT_ONLY;
		m_nReservedTime = XGetNowTime();
	}

	void Release()
	{
		m_nTalentID = 0;
		m_nNextAttackType = NA_NA;
	}
};



class _ChangeWeaponInfo
{
public:
	SH_ITEM_SLOT	m_nSlot;
	MUID			m_uidWeaponID;

	_ChangeWeaponInfo() : m_nSlot( ITEMSLOT_NONE), m_uidWeaponID( MUID::ZERO)		{}
	_ChangeWeaponInfo( SH_ITEM_SLOT	nSlot, MUID uidWeaponID) : m_nSlot( nSlot), m_uidWeaponID( uidWeaponID)		{}

	void Set( SH_ITEM_SLOT	nSlot, MUID uidWeaponID)
	{
		m_nSlot = nSlot;
		m_uidWeaponID = uidWeaponID;
	}

	bool IsEmpty()
	{
		return ( (m_nSlot != ITEMSLOT_RWEAPON  &&  m_nSlot != ITEMSLOT_RWEAPON2)  ||  m_uidWeaponID == MUID::ZERO);
	}

	void Clear()
	{
		m_nSlot = ITEMSLOT_NONE;
		m_uidWeaponID = MUID::ZERO;
	}
};




/// @brief �� �÷��̾��� ������ ����ϴ� ���
/// @author �����
/// @see
///  - XMyPlayer�� �� ����� ������ �ִ�.
class XModuleMyControl : public XModuleActorControl
{
private:

	friend class XMyActionState;
	friend class XMyActionIdle;
	friend class XMyActionNormalAttack;
	friend class XMyActionDodgeAttack;
	friend class XMyActionBow;
	friend class XMyActionShootGun;
	friend class XMyActionGuard;
	friend class XMyActionHit;
	friend class XMyActionUseItem;
	friend class XMyActionHold;
	friend class XMyActionSwitchingWeapon;
	friend class XMyActionChangeState;
	friend class XMyActionLootingItem;
	friend class XMyActionRebirth;
	friend class XMyActionUseTalent;
	friend class XMyActionJump;
	friend class XMyActionFalling;
	friend class XMyActionGrappled;
	friend class XMyActionChangeChannel;
	friend class XMyActionQuestInteraction;
	friend class XMyActionDeactivate;
	friend class XPlayFrame;
	friend class XPlayFrame_Debugger;
	friend class MockController;
	friend class XMyActionSwallowed;
	friend class XMyActionSit;
	friend class XMyActionDeactivative;
	friend class XMyActionGesture;
	friend class XMyActionNPCInteraction;
	friend class XMyActionEnchant;

	XModulePost*			m_pModulePost;

	//////////////////////////////////////////////////////////////////////////
	// �̵�����
	//
	bool					m_bPaused;					// �̵�/ȸ��/�ִϸ��̼� ��� ��������
	float					m_fSpeed;
	bool					m_bAutoRun;
	bool					m_bAutoRunKeyRelease;
	bool					m_bRestoreCamera;			// ī�޶� ����ġ ������ ����
	unsigned int			m_nMovementFlags;
	vec3					m_vCharacterFrontMoveDir;	// ĳ���� �̵� ����

	bool					m_bFallingStart;
	bool					m_bCurrentFreeLook;

	// ���콺 �̵�
	vec3					m_vMouseAutoMove;			// ���콺 �̵� ���� ��ġ
	bool					m_bMouseAutoMove;


	//////////////////////////////////////////////////////////////////////////
	// �׼ǰ���
	//
	XSpecialActionTransChecker*		m_pAttackChecker;
	unsigned int			m_nLastCancelActionTime;
	bool					m_bCancelAction;
	int						m_nCancelParam;					// 0 : ���ĵ��  1 : uppercombo�� ����
	void					_UpdateReleaseCancelAction();

	bool					m_bOnTargetObject;				//���� Ÿ�� ������Ʈ�� ��������.

	_ChangeWeaponInfo		m_ChangeWeaponInfo;				// ���� ���Ľ��� ������� �������� �ٲܶ� ����


	//////////////////////////////////////////////////////////////////////////
	// Ʈ���� üũ ����
	//
	XSensorController*		m_pSensorController;
	XRegulator				m_TouchSensorCheckRegulator;

	//////////////////////////////////////////////////////////////////////////
	// ��ǥ���� ���� üũ ����
	//
	XMapSignAreaChecker		m_MapSignAreaChecker;
	XRestrictedAreaChecker  m_RestrictedAreaChecker;

	//////////////////////////////////////////////////////////////////////////
	// ���콺 �̵� ����
	//
	XMouseMove*				m_pMouseMove;

	//////////////////////////////////////////////////////////////////////////
	// ��Ÿ
	//
	XMyHitController		m_HitController;
	XMyCharacterAlphaMgr	m_CharacterAlphaMgr;
	XTargetIndicator*		m_pTargetIndicator;

	//////////////////////////////////////////////////////////////////////////
	// ��Ÿ ������
	// 
	bool					m_bDamageRepeat;
	_DamageInfo *			m_pDamageInfo;

	//////////////////////////////////////////////////////////////////////////
	// Module �ʱ�ȭ
	//
	virtual void			OnInitialized();

	//////////////////////////////////////////////////////////////////////////
	// ����Ʈ(�ӽ� ����)
	//MUID					m_uidCharacterLightEffect;

	//////////////////////////////////////////////////////////////////////////
	// OnEvent �ݹ� : Action and Hit
	//
	virtual void			OnSubscribeEvent();
	virtual XEventResult	OnEvent(XEvent& msg);

	// Action
	XMyActionFSM*			m_pActionFSM;
	void					InitStates();

	bool					DoAction(ACTION_STATE nState, void* pParam=NULL);			///< �׼� ����
	bool					DoAction(ACTION_STATE nState, int nParam);

	bool					IsCanCancelAllMotion();
	bool					IsCanCancelUpper();	

	bool					IsCanStopMouseToggle();

	// Hit
	virtual void			HitByTalent(XTalentHitParam * pInfo);


	//////////////////////////////////////////////////////////////////////////
	// OnUpdate : if pass CheckUpdatable,
	// OnUpdateState -> OnUpdateCommand -> OnUpdateSpecail
	//
	virtual void			OnUpdate(float fDelta);
	virtual bool			OnUpdateCheckUpdatable();
	virtual void			OnUpdateState(float fDelta);
	virtual void			OnUpdateCommand(float fDelta);
	virtual void			OnUpdateSpecial(float fDelta);
	virtual void			OnPostUpdate(float fDelta);

	void					_UpdateMyCharacterAlpha();
	void					_UpdateRestrictedArea(float fDelta);

	// OnUpdateCommand, �̵�����
	void					_UpdateMyDir( float fDelta, const vec3& dir );
	void					_UpdateFrontDirAndSetCamera( float fDelta, vec3& dir);
	bool					_UpdateMyMovement(float fDelta, XModuleEntity* pModuleEntity, vec3& vDir);
	//void					_UpdateMovementPost(float fDelta, vec3& pos, vec3& dir, bool bKeyAction=false);
	//void					_UpdateRotationPost(float fDelta, XModuleEntity* pModuleEntity);

	// OnUpdateSpecail, ��������
	void					_UpdateTouchSensorTrigger(float fDelta);
	void					_UpdateTouchAreaTrigger();
	void					_UpdateTouchAreaTrigger( float fDelta );

	// ���콺 ���ͷ��� ����
	void					MouseMoveClick(MPoint& pt);
	void					SetMouseMove(vec3& vMovePos);
	void					InitMouseMove()		{ m_bMouseAutoMove = false; m_vMouseAutoMove = vec3::ZERO; }

	//////////////////////////////////////////////////////////////////////////
	// Current üũ�Լ�
	//
	XMotionTypes			GetCurrMotionType();				// ���� ���(������ �޸���, �ڷ� �޸��� ���)
	bool					CheckSpecialActionKey( SPECIAL_ACTION_COMMAND& eCmd );			// ���� Ű �Է� ������
	bool					CheckUsableSpecialActionKey(XEvent& msg);	
	bool					CheckActionEventAcceptable(XEvent* pEvent = NULL);		// ���� �̺�Ʈ ������ �������� ����
	void					CheckMouseMoveState();
	bool					CheckActionEventAcceptableByMouseVisible(XEvent* pEvent = NULL);
	bool					CheckAttackReturnAnimation();
	bool					CheckMouseMovable();
	bool					CheckGuardKnockback();

	//////////////////////////////////////////////////////////////////////////
	// ���� ������Ʈ
	void					UpdateStateFalling();

	//////////////////////////////////////////////////////////////////////////
	// ����Ʈ(�ӽ� ����)
	//bool					CreateCharacterLightEffect();
	//void					_UpdateLightEffectPos(vec3 vCameraDir);

protected:
	virtual void			CheckSwim(bool bIn);				// ���� ��������

	void					DoActionDie();

	bool					StartGrappled(_DamageRepeatInfo* pInfo);
	bool					StartSwallowed(_DamageRepeatInfo* pInfo);

	XChallengerQuestExitChecker*	m_pChallengerQuestExitChecker;

public:

	//////////////////////////////////////////////////////////////////////////
	// ���� / �Ҹ�
	//
	XModuleMyControl(XObject* pOwner=NULL);
	virtual ~XModuleMyControl(void);


	//////////////////////////////////////////////////////////////////////////
	// ��Ʈ������ �Ѿ�� Ŀ�ǵ�� ����
	//
	virtual void 			OnHit(XTalentHitParam* pHitInfo);
	virtual void 			OnDie(MF_STATE nMFState = MF_NONE, bool bSetAni = true);
	virtual void			OnRebirth();
	virtual void			OnSwitchingWeaponBegin();
	virtual void			OnSwitchingWeapon(int8 nWeaponSet);
	virtual void			OnLootEndReady();
	virtual void			OnHitLooting();
	virtual void			OnHeal(int nAmount, UIID nUserUIID = UIID_INVALID);

	virtual void			OnWarp(const vec3& vPos, const vec3& vDir);

	virtual void			DoActionLootingItem(MUID& uidNPC, float fLootTime=0.f);
	virtual void			OnLootEnd();
	virtual void			OnLootItemStart();

	virtual void			OnGuardKnockBack(vec3& vDir);
	virtual void			OnGuarded();
	virtual void			OnGuardFailed();

	virtual void			DoActionSitDown();
	virtual void			DoActionSitRise();

	virtual void			OnHitByBuff(TD_BUFF_HIT* pBuffHitInfo);

	bool					DoActionDye(void* pParam)		{ return DoAction(ACTION_STATE_DYE, pParam);	}
	void					DoActionHold()					{ DoAction(ACTION_STATE_HOLD);	}
	void					DoActionUseItem(int nSlotID);

	void					DoActionUseTalent(int nTalentID);
	void					DoActionEquipItem( SH_ITEM_SLOT nSlot, MUID uidItemID);
	void					DoActionChangeChannel();
	virtual void			DoActionIdle();
	void					DoActionIdleWithSpecialState();
	virtual void			DoActionQuestInteraction();
	virtual void			DoActionDeactivate( const int nBuffID, const wchar_t* szUseAni);
	virtual void			DoActionGesture(const wchar_t* szMotionName);
	void					DoActionInteraction(MUID uidTarget);
	bool					DoActionEnchantUseItem(int nSlotID);
	void					DoActionMapObjInteraction(MUID uidSearch);

	void					ReleaseCancelAction();
	void					SetCancelAction(int nParam);

	XMyPlayer*				GetMyPlayerOwner();
	inline ACTION_STATE		GetCurrentActionStateID();
	XMyActionState*			GetCurrentActionState();

	bool					IsMoving();
	bool					IsJumping();		// tripleJ 110420

	void					SetAutoRun(bool bAutoRun)	{ m_bAutoRun = bAutoRun; }
	bool					IsAutoRun()					{ return m_bAutoRun;	}

	void					Init_ForChangeField();
	void					Final_ForChangeField();

	void					InitStand();
	void					InitAutoMove();
	void					InitSensorController(XGameState* pState);

	void					OnRebirthSoulBinding();
	void					Rebirth();

	void					StopRun();

	void					SetPause( bool bPause);
	bool					IsPaused()					{ return m_bPaused;		}

	bool					ChangeWeapon();

	virtual void			DoInnBeginSleep();
	virtual void			DoInnEndSleep();

	void					DoGrappledEnd();

	XReserveAttack			m_ReserveAttack;

	bool					IsCurrentGuardEnabled();

	virtual void			OnTalentInstantEffectGain(XTalentInfo* pTalentInfo);
	void					FallingDamage();

	virtual bool			IsCurrentGuard();
	virtual bool			IsCurrentGrappled();

	XMyHitController*		GetMyHitController() { return &m_HitController; }

	bool					IsPostPosSync();
	bool					IsSendablePositionPost();

	void					InitFallingStartZ();
	void					UseRefillPotion();

	void					SyncPlayerDirToCameraDir();

	bool					IsMouseMove()		{ return m_bMouseAutoMove; }
	bool					IsHelpMessage();

	virtual void 			OnUseTalent(int nTalentID, vec3& vPos, vec3& vDir);

	// Target NPC Info
	void					ChangeIndicatorEffectPos();
	void					SetTargetIndicatorToggle(bool bShow);

	void					RequestCancel_PreInteraction();

	void					StartChallengerQuestExitTimer();
	void					StopChallengerQuestExitTimer();

	virtual void			HitByBuff(XTalentHitParam * pHitInfo);

	void					DoCameraLock(MUID& uidTarget, wstring& strBoneName, float& fDurationTime);
	void					DoForceCameraLock();

	virtual void			SetAFK(bool bAFK);

	// LockOnTarget Ǯ��	_by tripleJ 110520
	void					ClearLockOnTarget( const UIID& uidObj );
};

//----------------------------------------------------------------------------------------------------
inline ACTION_STATE XModuleMyControl::GetCurrentActionStateID()
{
	if(m_pActionFSM == NULL)
		return ACTION_STATE_NONE;
	else
		return (ACTION_STATE)m_pActionFSM->GetCurrentStateID();
}

inline XMyActionState* XModuleMyControl::GetCurrentActionState()
{
	if(m_pActionFSM == NULL)
		return NULL;
	else
		return m_pActionFSM->GetCurrentState();
}
//----------------------------------------------------------------------------------------------------

#endif // #ifndef _XMODULE_MY_CONTROL_H