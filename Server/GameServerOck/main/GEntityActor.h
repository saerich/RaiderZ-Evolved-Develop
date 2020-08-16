#ifndef _GENTITY_ACTOR_H
#define _GENTITY_ACTOR_H

#include "GEntitySync.h"
#include "CSDef.h"
#include "CSChrInfo.h"
#include "GTalentInfo.h" // for L'TALENT_TYPE'
#include "GFieldInfo.h" // for L'MARKER_INFO'
#include "MTime.h"
#include "GItem.h"
#include "GMotionFactor.h"
#include "GRegenUpdater.h"
#include "MAgent.h"
#include "GItemSystem.h"
#include "GTalentEffector.h"
#include "GRiposte.h"
#include "GImmuneHandler.h"
#include "GIndexedIDMap.h"
#include "GActorObserverMgr.h"

class GModuleCombat;
class GModuleBuff;

class GActorMeshInfo;
class GField;
class GBuffInfo;
class GActorObserver;
class GGlueActor;
class GGlueNPC;
class GNullGlueActor;
class GEnemyNPCRef;
class GActorInvisibility;
class GActorCounter;
class GActorCooltimeChecker;
class GActorHateTracker;
class GTalent;
class GActorBuffEntity;
class GActorTalent;


enum ACTION_STATE
{
	AS_NORMAL = 0,		// ����
	AS_MF,				// MF ������
	AS_USING_TALENT,	// ��ų �����
	AS_CHANGING_STANCE,	// ���Ľ� ��ȯ��(PC��)
	AS_SWIMMING,		// ������
};

struct GEntityActorGuardInfo
{
	bool					bGuarding;			///< ��������� ����
	int						nGuardingTalentID;	///< ������� �ŷ�Ʈ ID
	uint32					nGuardStartTime;		///< ��� ���� �ð�
	TALENT_GUARD_DIRECTION	nDirection;			///< ����ϰ� �ִ� ����
	GEntityActorGuardInfo() : bGuarding(false), nGuardStartTime(0), nGuardingTalentID(0), nDirection(GUARD_DIRECTION_FRONT) {}
};

struct GHitInfo
{
	bool					bHitProcessed;
	int32					nDamageType;
	int32					nDamage;
	int32					nHealAmount;
	MOTION_FACTOR_GROUP*	pMotionFactorGroup;			// script������ ���
	uint16					nCheckTime;
	int						nCapsuleGroup;
	int						nCapsuleIndex;
	uint32 					nCombatResultFlags;
	bool					bTryGuard;
	bool					bCriticalHit;
	bool					bBackHit;
	GTalentInfo*			pTalentInfo;
	GEntityActor*			pAttacker;
	GEntityActor*			pVictim;
	GGlueActor*	pAttackerForLua;
	GGlueNPC*		pVictimForLua;	// ��ũ��Ʈ���� �ڵ鸵�Ҷ� �����ڴ� �׻� NPC�̴� (�κ������� NPC�� ����)	
	bool					bGuarded;
	int						nTalentID;

	GHitInfo(): bHitProcessed(true), nDamageType(DA_NONE), nDamage(0), nHealAmount(0), nCheckTime(0), pMotionFactorGroup(NULL), nCapsuleGroup(INVALID_ID), nCapsuleIndex(INVALID_ID), nCombatResultFlags(0L), bTryGuard(false), pTalentInfo(NULL), pAttacker(NULL), pVictim(NULL), pAttackerForLua(NULL), pVictimForLua(NULL), bCriticalHit(false), bGuarded(false), bBackHit(false), nTalentID(INVALID_ID)
	{}

	bool IsValidInfo() const
	{
		return !bHitProcessed;
	}
};

struct GMFApplyArgs
{
	MOTION_FACTOR_GROUP* pMotionFactorGroup;	
	GEntityActor* pOpponent;
	MF_STATE nNotApplyMF;
	MF_STATE nForceMF;
	int nForceMFWeight;
	bool bKnockbackDirSyncToOpponentDir;	
	float fStateMotionFactor;
	GMFApplyArgs() : nNotApplyMF(MF_NONE), nForceMF(MF_NONE), nForceMFWeight(0), pMotionFactorGroup(NULL), pOpponent(NULL), bKnockbackDirSyncToOpponentDir(false), fStateMotionFactor(1.0f) {}
	GMFApplyArgs(MOTION_FACTOR_GROUP* mfg, GEntityActor* opponent) : nNotApplyMF(MF_NONE), nForceMF(MF_NONE), nForceMFWeight(0), pMotionFactorGroup(mfg), pOpponent(opponent), bKnockbackDirSyncToOpponentDir(false), fStateMotionFactor(1.0f) {}
};

struct GMFApplyReturnValue
{
	MF_STATE nMotionFactor;
	int nWeight;
	GMFApplyReturnValue(MF_STATE mf, int weight) : nMotionFactor(mf), nWeight(weight) {}
};

/// �ൿ�� �� �ִ� ��ü - PC, NPC
class GEntityActor : public GEntitySync, public GActorObserverMgr
{
	friend class GRegenUpdater;
	friend class GActorDamageCalculator;
protected:
	UIID					m_nUIID;			///< m_UID�� �ε��� ID

	GRegenUpdater			m_RegenUpdater;
	GModuleCombat*			m_pModuleCombat;
	GModuleBuff*			m_pModuleBuff;
	CHR_INFO*				m_pChrInfo;
	CHR_STATUS				m_ChrStatus;
	ACTION_STATE			m_nActionState;			///< �׼� ����
	GMotionFactor			m_MotionFactor;
	
	MUID					m_uidKiller;			///< �� ���� ������ UID

	MRegulator				m_MFReleaser;

	bool					m_bScheduleMove;
	vec3					m_vScheduleMovePos;
	list<uint32>			m_listEnteredSensorID;
	
	GEntityActorGuardInfo	m_GuardInfo;

	bool					m_isMoving;
	bool					m_isJumping;

	bool					m_bCancelAction;
	
	GTalentEffector			m_TalentEffector;

	uint32					m_nTalentStartTime;

	// �������� �ǰݴ��������� ����
	GHitInfo				m_LastHitinfo;
	
	// �ǹ޾�ġ�� ���� ��ü
	GRiposte				m_Riposte;
	MUID					m_uidLastHitOrHittedEnemy;	///< ���� �ֱٿ� ���� ������ �߰ų�, ������ ���� ��
	// �鿪ó����
	GImmuneHandler			m_ImmuneHandler;

	// �ൿ�Ұ����� �������� ����
	bool					m_bNowUnableAction;

	// ����ȭ ó����
	GActorInvisibility*		m_pInvisibility;
	// ī���� ó����
	GActorCounter*			m_pCounter;
	// ��Ÿ�� üĿ
	GActorCooltimeChecker*	m_pCoolTimeChecker;
	// ����Ʈ ������
	GActorHateTracker*		m_pHateTracker;
	// ���� ��ƼƼ ������
	GActorBuffEntity*		m_pBuffEntity;
	// �ŷ�Ʈ ������
	GActorTalent*			m_pActorTalent;


	GGlueActor*				m_pDelegator;
	
	bool					m_isAllowCalcVictimDodge;		///< ����� ȸ���� ���뿩��
	bool					m_bRegen;


	virtual void OnRebirth();
	virtual void OnRebirthBySelf();
	virtual void OnRebirthByOther();
protected:
	virtual void RegisterModules();
	virtual void UnregisterModlues();
	virtual void OnUpdate(float fDelta);

	virtual void OnUpdatedStatus() {}

	void _UpdateMotionFactor(float fDelta);
	virtual void OnChangeActionState(ACTION_STATE nNextState);
	
	// ���� ����
	virtual bool	_HandleCheckpointTrigger(const SENSOR_INFO& infoSensor) { return false; }
	virtual bool	_HandleAutoWarpTrigger(const SENSOR_INFO& infoSensor) { return false; }
	virtual bool	_HandleInnroomWarpTrigger(const SENSOR_INFO& infoSensor, GATE_TYPE eGateType) { return false; }
	virtual void	_HandleQuestTrigger(const SENSOR_INFO& infoSensor) {}

	void _FillRegenVar();

	virtual GGlueActor*	NewDelegator();

	// ����ü�϶��� �ڽ��� �߰��� �÷��̾�鿡�� �޼��� ����
	void RouteToInvisibleDetectors(MCommand* pCommand, MUID uidExcept) const;
public:
	typedef GNullGlueActor NullType;

	GEntityActor();
	virtual ~GEntityActor();
	GModuleCombat*			GetModuleCombat() const	{ return m_pModuleCombat; }
	GModuleBuff*			GetModuleBuff()	 const	{ return m_pModuleBuff; }

	GGlueActor* GetDelegator();

	virtual void UpdateCombat(float fDelta);
	virtual void UpdateBuff(float fDelta);

	virtual bool Create(MUID& uid);
	virtual void Destroy();
	void CreateDelegator();
	
	virtual bool SetPos(const vec3& pos) override;

	float	DistanceTo(GEntityActor* pTarget);
	
	virtual float GetSpellPower() const { return 1.0f; }

	uint32 GetTalentStartTime()		{ return m_nTalentStartTime;	}

	// ������ �ٶ󺸰� �ϱ�
	void SetDirForwardToActor(GEntityActor* pTarget, bool bRoute=false);
	// ������ �ٶ󺸰� �ϱ�
	void SetDirForwardToActor(MUID uidTarget, bool bRoute=false);

	// �������� �ǰݴ��������� ���� ��ȯ
	GHitInfo			GetLastHitInfo()	{ return m_LastHitinfo; }
	void				ClearLastHitInfo();

	// �������� �ൿ�� �������� ����
	virtual bool IsBeginUnableAction();
			bool IsEndUnableAction();
	virtual void OnBeginUnableAction();
	virtual void OnEndUnableAction();
			bool IsNowUnableAction();

	virtual void OnDestroy() override;

	bool		IsDie();
	virtual void doDie();

	void RouteDie();
	virtual bool doDamage(MUID uidAttacker, DAMAGE_ATTRIB nDamageAttrib, uint32 nDamage, EFFECT_SOURCE_TYPE nEffectSourceType = EST_NONE, int nEffectSourceID = INVALID_ID);	// return : ���ظ� �԰� �׾��°�
	void doHeal(MUID uidAttacker, uint32 nHealAmount);
	virtual void doAttack(MUID uidTarget, int nDamage);
	virtual void doTryDamage(MUID& uidAttacker)	{}
	virtual void doTryAttack(MUID& uidTarget)		{}
	virtual bool doUseTalent(int nTalentID, TALENT_TARGET_INFO Target = TALENT_TARGET_INFO::Invalid(), bool bCheckEnabled=true, bool bGainStress=true);
	virtual bool doUseTalent(GTalentInfo* pTalentInfo, TALENT_TARGET_INFO Target = TALENT_TARGET_INFO::Invalid(), bool bCheckEnabled=true, bool bGainStress=true);
	// �ŷ�Ʈ�� ��� (�ŷ�Ʈ������ cancelable�Ӽ��� ���� ��쿡�� �ش�)
	virtual void doCancelTalent(bool bPostCommand=true);
	// �ŷ�Ʈ�� ������ ���
	void doCancelTalentForce(bool bPostCommand=true);

	void doRebirthBySelf();
	void doRebirthByOther();

	virtual void doTryHit(GEntityActor* pAttacker, GTalentInfo* pTalentInfo, int nDamage, uint16 nCheckTime, int nCapsuleGroup, int nCapsuleIndex);
	
	// �鿪������ ��ȯ
	GImmuneHandler& GetImmuneHandler()		{ return m_ImmuneHandler; }
	// �鿪 ó��, ��ȯ���� �鿪ó�� �ƴ��� ����
	bool HandleImmune( GEntityActor* pAttacker, GTalentInfo* pTalentInfo );
	bool HandleImmune( GEntityActor* pAttacker, GBuffInfo* pBuffInfo );

	void OnImmuned( GTalentInfo* pTalentInfo, GEntityActor* pAttacker );
	void OnAvoid( GTalentInfo* pTalentInfo, GEntityActor* pAttacker );

	void		 SetKiller(MUID uidKiller)			{ m_uidKiller = uidKiller; }
	MUID		 GetKiller() const					{ return m_uidKiller; }

	// ������������ ����
	virtual  bool IsNowInvincibility();
	// ����ȭ�������� ����
	virtual bool IsNowInvisibility() const;
	// NPC���� ����������� ����
	virtual  bool IsNowInvisibilityToNPC();
	// ȸ�ǻ������� ����
	bool IsNowAvoidTime();

	// ���������� �ŷ�Ʈ�� �ٰ��Ͽ� ����� ��ȯ, �ŷ�Ʈ������ ���ٸ� ���ʹ����� ���
	virtual DAMAGE_ATTRIB GetDamageType(const GTalentInfo* pTalentInfo=NULL);

	// �ŷ�Ʈ�� ��� �Ұ����� �������� ����
	bool IsDisableTalent(GTalentInfo* pTalentInfo);

	void GetBuffList(vector<int>& vBuffList) const;
	void GetBuffList(int* Buffs);
	bool CheckTalentRange(GEntityActor* pTargetActor, int nTalentID);

	virtual float GetStandupTime() { assert(false); return -1.0f; }

	// �ǰ� �̺�Ʈ
	virtual void OnHit(GHitInfo infoHit, bool bRouteHit);
	// ��� ��Ʈĸ�� �ǰ�
	virtual void OnHitAll(GTalentInfo* pTalentInfo);
	// ��� ��Ʈĸ�� �ǰ�
	virtual void OnHitSuccess(GTalentInfo* pTalentInfo);
	// ���� ���������� �̺�Ʈ
	virtual void OnHitEnemy(uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pTalentInfo);
	// ġ��Ÿ �ǰ� �̺�Ʈ
	virtual void OnCriHit( GEntityActor* pAttacker, GTalentInfo* pTalentInfo );
	// ���� ġ��Ÿ�� ���������� �̺�Ʈ
	virtual void OnCriHitEnemy( GEntityActor* pTarget, GTalentInfo* pTalentInfo );
	// ������ �̺�Ʈ
	virtual void OnMiss(uint32 nCombatResultFalg, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo);
	// ���� ������ ȸ�������� �̺�Ʈ
	virtual void OnMissEnemy(uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pAttackTalentInfo);
	// ��� �̺�Ʈ
	virtual void OnGuard( GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo );
	// ���� ��� �̺�Ʈ
	virtual void OnPerfectGuard(GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo);
	// ���� ��� �̺�Ʈ
	virtual void OnAbsoluteGuard( GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo );
	// ���� ������ �������� �̺�Ʈ
	virtual void OnGuardEnemy(GEntityActor* pTarget, GTalentInfo* pAttackTalentInfo);
	// �ŷ�Ʈ ����� �Ϸ������ ȣ��Ǵ� �̺�Ʈ
	virtual void OnFinishTalent(GTalentInfo* pTalentInfo);
	// �ŷ�Ʈ ����� ȿ���� �ߵ��ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnActTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo ) override;


	// �ǹ޾�ġ�� ������ �߰�, Rate�� �ǹ޾�ĥ ����ġ, ���� ������ �ǹ޾�ġ��� ��ø��
	void	AddRiposte(CSRiposte infoRiposte);
	// �ǹ޾�ġ�� ������ ����, Rate�� �ǹ޾�ĥ ����ġ
	void	EraseRiposte(CSRiposte infoRiposte);

	virtual bool IsEquipShield() const			{ return false; }


	GMFApplyReturnValue ApplyMotionFactor(const GMFApplyArgs args);
	virtual void DoMotionFactor(MF_STATE nMF, int nWeight, GEntityActor* pOpponent, bool bKnockbackDirSyncToOpponentDir);
	void ReleaseMotionFactor(bool bStateToNormal=true);

	void ChangeActionState(ACTION_STATE nState);
	virtual bool IsNowRunningCancelableTalent()		{ return true; }
	virtual void OnUseTalentFailed(int nTalentID, CCommandResultTable nFailCause) {}
	
	void SetHP(int nHP);
	void SetEN(int nEN);
	void SetSTA(int nSTA);

	// ���� ����
	bool			IsValidSensor(const unsigned long nSensorID, vec3 vEventPos);
	void			EnterSensor(uint32 nSensorID);
	void			LeaveSensor(uint32 nSensorID);
	void			ClearEnteredSensorID(void);
	uint32			GetForcusedSensorID();
	void			SensorUseTalent(const int nTalentID);
	bool			IsEnteredSensorID(int nSensorID);
	void			AddEnteredSensorID(int nSensorID);
	void			RemoveEnteredSensorID(int nSensorID);
	
	vec3 GetScheduleMovePos() const;
	void SetScheduleMove(vec3 pos);
	virtual bool DoScheduleMove();
	virtual bool doGuard(int nGuardTalentID);
	virtual bool doGuardReleased();

	void doJump()	{ m_isJumping = true;	}
	void doLand()	{ m_isJumping = false;	}
	void doMove()	{ m_isMoving = true;	}
	void doStop()	{ m_isMoving = false;	}

	int	 GetGuardTalentID();
	bool IsGuarding()		{ return m_GuardInfo.bGuarding; }	// ��� ������ ����
	bool IsMoving()			{ return m_isMoving;			}	// �̵� ������ ����
	bool IsJumping()		{ return m_isJumping;			}	// ���� ������ ����
	bool IsKnockdowned()	{ return GetCurrentMotionFactor() == MF_KNOCKDOWN; }// �˴ٿ����� ����
	bool IsTalentCasting();
	virtual bool IsHittable(GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo);		///< ���� �������� ����
	virtual bool IsHittable(GBuffInfo* pBuffInfo );
	bool IsGuardableDirection(GEntityActor* pAttacker);
	// �̵��� �� �ִ� ���� (�ɵ����� �̵�)
	virtual bool IsMovable();
	// �������� �� �ִ��� ���� (��� �̵�)
	bool IsSticky();
	
	bool IsAllowCalcVictimDodge();		// ����� ȸ���� ���뿩�� Ȯ��	

	void RouteActionCancelNextAttack(UIID nEntityUIID, CANCEL_NEXT_ATTACK_PARAM nParam);


	// ������� ����
	MF_STATE	 GetCurrMF()				{ return m_MotionFactor.GetCurrMF(); }
	int			 GetCurrWeight()			{ return m_MotionFactor.GetCurrWeight(); }
	GMotionFactor& GetMotionfactorForTest() { return m_MotionFactor; }
			bool SetMFModValue( int nType, float fValue );
			bool SetMFModWeight( int nType, float fWeight );
	virtual void OnBeginMotionFactor()		{}
	virtual void OnEndMotionFactor()		{}


	// �޼��� ����
	virtual void RouteToThisCell(MCommand* pCommand, MUID uidExcept=0) const override;
	virtual void RouteToMe(MCommand* pCommand) const;


	void RouteRebirth();
	void RouteTalentDrain(const GEntityActor* pAttacker, int nTalentID, TALENT_DRAIN_TYPE nDrainType, int nDrainAmount);
	void RouteSimpleStatus(MUID uidListener);
	virtual void RouteFaceTo( vec3 vDir ) {}
	void RouteCancelTalent( int nTalentID );	// Ŭ�� ��Ÿ�� ��������� ����
	void RouteUseTalentRequestFailed( int nTalentID );
	void RouteImmune( GEntityActor* pAttacker, GTalentInfo* pTalentInfo );
	void RouteAvoid( GEntityActor* pAttacker, GTalentInfo* pTalentInfo );
	void RouteMiss( GEntityActor* pAttacker, GTalentInfo* pTalentInfo );

	virtual MCommand* MakeCmd_Whois(GEntitySync* pReqSync) { return NULL;}

	virtual CHR_INFO*	GetChrInfo() const					{ return m_pChrInfo; }
			void		SetChrInfo(CHR_INFO* infoChar)		{ m_pChrInfo = infoChar; }
			CHR_STATUS*	GetChrStatus()						{ return &m_ChrStatus; }


	virtual void doMakePeace();	
	virtual bool IsNowCombat() const { return false; };	///< ���� �������ΰ�?

	// Ư�� ������ �ɷȴ��� ����
	bool HasBuff(int nBuffID);
	// Ư�� �̷ο� ������ �ɷȴ��� ����
	int GetBuffQty();
	// Ư�� ������� �ɷȴ��� ����
	int GetDebuffQty();
	// Ư�� ������ �����ϱ�
	void LostBuff(int nBuffID);
	// Ư�� ���� ���� ���θ� ����
	void LostBuffStack(int nBuffIDSlot);

	// Ư�� ��Ŀ���� �ɷ��ִ��� ����
	virtual bool HasFocus(TALENT_FOCUS_TYPE nFocus)			{ return false; }
	// ��Ŀ���� �ϳ��� �ɷ��ִ��� ����
	virtual bool HasFocus()									{ return false; }

	// Attribute Modifier
	virtual int GetMaxHPProto() const { return m_pChrInfo->nMaxHP; }
	virtual int GetMaxENProto() const { return m_pChrInfo->nMaxEN; }
	virtual int GetMaxSTAProto() const { return m_pChrInfo->nMaxSTA; }

	void TrimStats();
	int GetMaxHP() const;
	int GetMaxEN() const;
	int GetMaxSTA() const;
	virtual int GetAP() const { return m_ChrStatus.ActorModifier.nAP.Calc(m_pChrInfo->nAP); }
	int GetABS() const { return m_ChrStatus.ActorModifier.nABS.Calc(m_pChrInfo->nABS); }
	float GetWalkSpeed() const { return DefaultWalkSpeed() * (1 + m_ChrStatus.ActorModifier.fMoveSpeed); }
	virtual float DefaultWalkSpeed() const;
	int GetHPRegen() const { return m_ChrStatus.ActorModifier.nHPRegen.Calc(m_pChrInfo->nHPRegen); }
	uint8 GetHPPercent() { if (GetMaxHP() <= 0) return 0;	return (GetHP() * 100) / GetMaxHP(); }
	int GetENRegen() const { return m_ChrStatus.ActorModifier.nENRegen.Calc(m_pChrInfo->nENRegen); }
	int GetSTARegen() const { return m_ChrStatus.ActorModifier.nSTARegen.Calc(m_pChrInfo->nSTARegen); }
	int GetResist(TALENT_RESIST_TYPE nResistType) const;

	void RegUIID(UIID nUIID)			{ m_nUIID = nUIID; }
	const UIID	GetUIID(void) const	{ return m_nUIID; }


	int GetHP()		{ return m_ChrStatus.nHP; }
	int GetEN()		{ return m_ChrStatus.nEN; }
	int GetSTA()	{ return m_ChrStatus.nSTA; }
	virtual int GetCHA() const	{ return 0; }
	
	virtual const wchar_t* GetName() const { return L"actor"; }
	
	ACTION_STATE	GetActionState() const 			{ return m_nActionState; }
	void			SetCurrentMotionFactor(MF_STATE nState, int nWeight) { m_MotionFactor.Force(nState, nWeight); }
	MF_STATE		GetCurrentMotionFactor() const	{ return m_MotionFactor.GetCurrMF(); }
	bool			HasMotionFactor() const;
	float			GetColRadius();
	float			GetHitRadius();
	virtual float	GetValidInteractionDistance();		///< ���ͷ��� ���� �Ÿ�
	virtual GActorMeshInfo*	GetMeshInfo()				{ return NULL; }
	virtual int GetLevel() const					{ return m_pChrInfo->nLevel; }
	virtual void SetLevel(int nLevel)				{ m_pChrInfo->nLevel = nLevel; }

	virtual void ForceStandUp();	// ������ �����Ѽ����.
	bool SetPosToGroundVertical(vec3* pvTarPos=NULL);	// ��ġ�� ���鿡 �°� z��ǥ�� �������ش�.(pvTarPos�� NULL�̸� ���� ��ġ�� ���鿡 ����.)

	virtual int GetStepTalentID(int nTalentGroupID, int nStepIndex) { return 0; }

	void Warp(vec3 vPos);
	virtual void Warp(vec3 vecPos, vec3 vecDir, bool bRouteToMe)	{ }

	bool RemoveBuff(int nBuffID);
	virtual bool GainBuff( int nBuffID, GTalentInfo* pTalentInfo=NULL, GEntityActor* pUser=NULL );
	bool GainBuffDetail( int nBuffID, float fDuration, float fPeriod, GTalentInfo* pTalentInfo=NULL, GEntityActor* pUser=NULL );
	void SetRegenActive(bool bActive) { return m_RegenUpdater.SetActive(bActive); }
	bool IsRegenActive() { return m_RegenUpdater.IsActive(); }
	void SetMFRegenActive(bool bActive) { return m_MotionFactor.SetActive(bActive); }
	bool IsMFRegenActive() { return m_MotionFactor.IsActive(); }
	virtual int8 GetGrade() const { return PLAYER_GRADE; }
	virtual ARMOR_ATTRIB GetArmorAttrib() const 	{ return ARMOR_ATT_NEUTRAL; }
	virtual DAMAGE_ATTRIB GetEnchantAttrib() const;
	GMotionFactor& GetMotionFactorForTest() { return m_MotionFactor; }
	GEntityActor* FindActor(const MUID& uidActor) const;	

	MUID GetLastHitOrHittedEnemy() { return m_uidLastHitOrHittedEnemy; }
	GTalent* GetCurrentTalent();
	
	virtual void Narration(const wchar_t* szText)		{ return; }
	virtual void NarrationNow(const wchar_t* szText)	{ return;	}

	GActorInvisibility& 	GetActorInvisibility()		{ _ASSERT(m_pInvisibility); return (*m_pInvisibility);  }
	GActorCounter& 			GetActorCounter()			{ _ASSERT(m_pCounter); return (*m_pCounter);  }
	GActorCooltimeChecker&	GetActorCooltimeChecker()	{ _ASSERT(m_pCoolTimeChecker); return *m_pCoolTimeChecker; }
	GActorHateTracker&		GetActorHateTracker()		{ _ASSERT(m_pHateTracker); return *m_pHateTracker; }
	GActorBuffEntity&		GetActorBuffEntity()		{ _ASSERT(m_pBuffEntity); return *m_pBuffEntity; }
	GActorTalent&			GetActorTalent() const		{ _ASSERT(m_pActorTalent); return *m_pActorTalent; }

	void DisableRegen()		{ m_bRegen = false; }
	void EnableRegen()		{ m_bRegen = true; }
	bool IsDisableRegen()	{ return !m_bRegen; }


	bool HasBuff_ForTest( int nBuffID );
	

};

GEntityActor* ToEntityActor( GEntity* pEntity );


#endif