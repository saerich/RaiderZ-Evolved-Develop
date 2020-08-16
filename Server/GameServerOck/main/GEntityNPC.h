#ifndef _GENTITY_NPC_H
#define _GENTITY_NPC_H

#include "GEntitySync.h"
#include "GEntityActor.h"
#include "CTransData.h"
#include "GNPCInfo.h"
#include "GTaskID.h"
#include "MMemPool.h"
#include "GEntityNPCUpdater.h"
#include "GDef.h"

// GNPCSignal
enum ACTION_TYPE
{
	CHANGE_BEHAVIOR_STATE,
	CHANGE_COMBAT_STATE,
	ATTACK,
	FLEE,
	TARGETTING,
	PATROL
};

// ���ϴ� NPC�� ����
struct NPC_STATUS
{
public:
	int						nLevel;
	bool					bRooted;
	NPC_ATTACKABLE_TYPE		nAttackable;
	NPC_AA_TYPE				nAA;

	NPC_STATUS()
	{
		nLevel = 1;
		bRooted = false;
		nAttackable = NAT_NONE;
		nAA = NAAT_ALWAYS;
	}
};

struct NPC_TICK_VARS
{
	bool bProceedReactionColt;
	struct LAZY_ATTACK
	{
		MUID uidTarget;
		int nDamage;

		LAZY_ATTACK()
		{
			uidTarget.SetInvalid();
			nDamage = 0;
		}
	} LazyAttack;
	

	NPC_TICK_VARS(): bProceedReactionColt(false) {}
};


class GJob;
class GTask;
class GModuleAI;
class GColt;
class GLootSystem;
struct SPAWN_INFO;
struct LOOT_ITEM_INFO;
class GNPCDebugger;
class GNPCPathFinder;
class GJobRunner;
class GColtMgr;
class GNullGlueNPC;
class GNullGlueNPC;
class GHateTable;
class GBehaviorStateMachine;
class GJobMgr;
class GNPCDecay;
class GNPCWaitFor;
class GNPCNetLogger;
class GNPCTarget;
class GNPCInteraction;
class GNPCLoot;
class GNPCSwallow;
class GNPCQuest;
class GNPCSensorChecker;
class GNPCMinion;
class GNPCHitcapsuleGroup;
class GNPCRage;
class GNPCStress;
class GNPCPathContext;
class GNPCBParts;
class GNPCMove;
class GNPCAssist;
class GNPCFirstStrike;
class GNPCMode;
class GNPCRewardOwner;
class GNPCRewarder;
class GNPCSetterForTest;
class GNPCLuaCallback;
class GScriptTimer;
class GModuleMovable;
class GNPCCombatAnticheat;
class GQPNPCPosSender;
class GLootInfo;
class GFieldNPCSession;

/// NPC
class GEntityNPC : public GEntityActor
{
	friend class GNPCDebugger;
	TEST_FRIEND_NPCSETTER
private:	
	friend class GColtRunner;
	friend class GColtMgr;
	friend class GModuleAI;
	friend class GEntityNPCUpdater;

	SPAWN_ID					m_nSpawnID;
	GJobRunner*					m_pJobRunner;
	GNPCDebugger*				m_pDebugger;

	NPC_STATUS					m_NPCStatus;
	NPC_STATUS					m_NPCStatusForced;
	SimpleMarker				m_HomePoint;
	int							m_nStaticID;				///< �ʵ��� ������ ��ü ID. �� ���� 0�̸� ������� �ʴ´�.

	GModuleAI*					m_pModuleAI;
	GModuleMovable*				m_pModuleMovable;
	int							m_nGroupID;

	GDamageInfo					m_LastDamagedInfo;
	GNPCSwallow*				m_pSwallow;						///< �Ա� ó��

	NPC_TICK_VARS				m_TickVar;

	// ��ã�� ����
	GNPCPathFinder*				m_pPathFinder;


	GEntityNPCUpdater			m_Updater;
	GHateTable*					m_pHateTable;
	GNPCDecay*					m_pNPCDecay;					
	GNPCInteraction*			m_pNPCInteraction;
	GNPCLoot*					m_pNPCLoot;
	GNPCQuest*					m_pNPCQuest;
	GNPCSensorChecker*			m_pNPCSensorChecker;
	GNPCWaitFor*				m_pNPCWaitFor;
	GNPCTarget*					m_pNPCTarget;
	GNPCMinion*					m_pNPCMinion;
	GNPCHitcapsuleGroup*		m_pNPCHitcapsuleGroup;
	GNPCRage*					m_pNPCRage;
	GNPCStress*					m_pNPCStress;
	GNPCPathContext*			m_pNPCPathContext;
	GNPCMove*					m_pNPCMove;
	GNPCFirstStrike*			m_pNPCFirstStrike;
	GNPCMode*					m_pNPCMode;
	GNPCRewardOwner*			m_pNPCRewardOwner;
	GNPCRewarder*				m_pNPCRewarder;
	GNPCSetterForTest*			m_pNPCSetterForTest;
	GNPCLuaCallback*			m_pNPCLuaCallback;
	GNPCCombatAnticheat*		m_pNPCCombatAnticheat;
	GQPNPCPosSender*			m_pQPNPCPosSender;

	// �������� ������
	GNPCAssist*					m_pNPCAssist;
	GNPCBParts*					m_pNPCBParts;	

protected:	
	GNPCInfo*					m_pNPCInfo;
	const SPAWN_INFO*			m_pSpawnInfo;	

	vector<int>					m_vecUserData;
	static map<int,int>			m_mapUserDataSize;		///< map<NPC_ID, nSize>

	GScriptTimer*				m_pScriptTimer;

	bool						m_isThinkable;
	bool						m_EXPRewardable;
	bool						m_QuestRewardable;
	
	

private:
	void ResetTickVars();
	virtual void RegisterModules();
	virtual void UnregisterModlues();

	void UpdateScriptTimer( float fDelta );

	virtual void OnEnter() override;
	virtual void OnLeave() override;

public:
	virtual MCommand* CreateFieldInCmd() override;
	virtual MCommand* CreateFieldOutCmd() override;
protected:
	virtual void OnVisitedNewSectorPC(const MUID& newSightedPC,MCommand* const fieldInCmd) override;

private:
	virtual MCommand* MakeCmd_Whois(GEntitySync* pReqSync) override;
	virtual void RouteInEntity(const vector<MUID>& vecPlayers) override;
	virtual void RouteOutEntity( const vector<MUID>& vecPlayers ) override;

	NPC_TICK_VARS& GetTickVars()				{ return m_TickVar; }
protected:
	virtual void OnHandle_NoNPCInfo();
	virtual GGlueActor*	NewDelegator() override;
	virtual void OnUpdate(float fDelta);
public:
	typedef GNullGlueNPC NullType;

	GEntityNPC();
 	virtual ~GEntityNPC();

	bool InitFromNPCInfo(GNPCInfo* pNPCInfo, const SPAWN_INFO* pSpawnInfo, int nStaticID=0);
	bool SetNPCInfo(GNPCInfo* pNPCInfo);

	int GetID() const;
	GGlueNPC* GetDelegator();

	virtual bool IsHuge() const override;

	virtual void UpdateBuff(float fDelta);
	virtual void UpdateAI(float fDelta);
	void UpdateMovable( float fDelta );

	virtual void		Destroy() override;
	void Reset();
		// === ���� ���� ===
	bool				IsNowRunningCancelableTalent();
	// ���� ��Ʈĸ���׷� ���̵� ��ȯ
	int8				GetCapsuleGroupIndex();

	GNPCPathFinder* GetPathFinder()				{ return m_pPathFinder; }

	virtual bool SetPos(const vec3& pos) override;
	vec3  GetSpawnPos();
	float GetCombatRadius();
	
	virtual void RouteFaceTo( vec3 vDir );
	void SetDirForwardToEnemy(bool bRoute=false);
	
	void ReturnToHomePoint();
	void ReturnToSpawnPoint();
	
	// �ڽ��� �׾����� ȣ��Ǵ� �̺�Ʈ
	virtual void OnDie() override;
	// �������� �¸����� �� ȣ��
	void OnVictory();
	// ������ ������� �� ȣ��
	void OnEndCombat();

	virtual void OnUseTalentFailed(int nTalentID, CCommandResultTable nFailCause) override;


	virtual bool IsMovable() override;


	// TransData ����ü�� ����� ��ȯ, ���ϰ��� ����ü�� �������� ���θ� �Ǵ�
	void MakeTDCacheInfo(TD_UPDATE_CACHE_NPC& out);
	void MakeTDCacheInfo(GEntityPlayer* pReqPlayer, TD_UPDATE_CACHE_NPC& out);

	// �޼��� ����
	virtual void RouteMove(vec3& tarPos);
	virtual void RouteMoveStop();
	void RouteChangeCombatTarget(MUID uidTarget);
	virtual void CollectEssentialReceiver(vector<MUID>& vecReceiverUID) override;

	//// �׷� ����
	//bool				JoinGroup(GEntityNPCGroup* pGroup);
	//bool				JoinGroup(GEntityNPC* pNPC);
	//void				LeaveGroup();
	//GEntityNPCGroup*	GetGroup();

	// ������� ����
	void					OnBeginMotionFactor();
	void					OnEndMotionFactor();

	// ���� ����
	bool				IsEnemy(MUID uidEnemy);
	virtual bool		IsNowCombat() const override;
	// ��ȯ������ ����
	bool				IsNowReturn() const;
	virtual void		doMakePeace() override;	
	void doMakePeaceForce();

	bool				IsNoneAttackable();
	bool				IsAlwaysAttackable();
	bool				IsFactionAttackable();

	void				ChangeAttackable(NPC_ATTACKABLE_TYPE nAttckable);
	void				EnableInteract(bool bEnable);
	GDamageInfo&		GetLastDamagedInfo()			{ return m_LastDamagedInfo; }
	
	MUID				GetTarget();
	GEntityActor*		GetTargetActor();
			
	virtual void		Warp(vec3 vecPos, vec3 vecDir, bool bRouteToMe) override;

	bool		PushJob_GainBuff(int nBuffID);

	float GetRotateSpeed() const;

	virtual float GetSpellPower() const override;

	virtual float GetStandupTime() override;

	virtual void OnBeginUnableAction() override;
	virtual void OnEndUnableAction() override;

	virtual void OnFinishTalent(GTalentInfo* pTalentInfo);
	
	// do ������
	virtual bool doDamage(MUID uidAttacker, DAMAGE_ATTRIB nDamageAttrib, uint32 nDamage, EFFECT_SOURCE_TYPE nEffectSourceType = EST_NONE, int nEffectSourceID = INVALID_ID) override;	// return : ���ظ� �԰� �׾��°�
	virtual void doDie();	

	void doDieForced();
	virtual void doDieByGather(GEntityPlayer* pPlayer, int nLootID);

	virtual bool doGuard(int nGuardTalentID);
	virtual bool doGuardReleased();
	virtual void DoMotionFactor(MF_STATE nMF, int nWeight, GEntityActor* pOpponent, bool bKnockbackDirSyncToOpponentDir) override;	
	virtual void doTryHit(GEntityActor* pAttacker, GTalentInfo* pTalentInfo, int nAddtiveDamage=0, uint16 nCheckTime=0, int nCapsuleGroup=INVALID_TALENT_ID, int nCapsuleIndex=INVALID_TALENT_ID);
	virtual void OnHit(GHitInfo infoCombat, bool bRouteHit);

	// ��ȭ ����
	virtual void doSay(const wchar_t* szText, float fMaintainTime=0.0f);
	virtual void doBalloon(const wchar_t* szText, float fMaintainTime=0.0f);
	virtual void doShout(const wchar_t* szText, float fMaintainTime=0.0f);
	virtual void doNarration(const wchar_t* szText);
	virtual void doSayAndNarration(const wchar_t* szText, float fMaintainTime=0.0f);

	virtual void doDespawn(bool bSpawnEnable=false);	
	virtual void doAttack(MUID uidTarget, int nDamage=0);
	void doAttackLazy(MUID uidTarget, int nDamage=0);
	virtual void doAssist(GEntityNPC* pAlliedNPC);
	virtual void doYell(int nRange=0);

	virtual void _HandleQuestTrigger(const SENSOR_INFO& infoSensor);

	void SetTauntable(bool bEnable);
	
	void SetHomePoint(const vec3& vPoint, const vec3& vDir);
	const SimpleMarker& GetHomePoint() const;

	int  GetStaticID() const							{ return m_nStaticID; }
	bool IsRepairer();

	void ChangeMode( NPC_MODE nNexMode );
	NPC_MODE		GetMode() const;

	GNPCInfo*		GetNPCInfo() const;
	GNPCInfo*		GetNPCInfoMaster() const;		///< mode�� ������� ��ǥ Info ��ȯ

	virtual CHR_INFO*	GetChrInfo() const;
	NPC_STATUS&			GetNPCStatusForced()				{ return m_NPCStatusForced; }
	NPC_STATUS&			GetNPCStatus()						{ return m_NPCStatus; }
	const NPC_STATUS&	GetNPCStatus() const				{ return m_NPCStatus; }
	const SPAWN_INFO*	GetSpawnInfo()						{ return m_pSpawnInfo; }

	GModuleAI*		GetModuleAI()						{ return m_pModuleAI; }
	GModuleMovable*	GetModuleMovable()					{ return m_pModuleMovable; }
	int				GetStress();
	int				GetGroupID();
	void			SetGroupID(int nGroupID);
	
	int GetUserdata(int index);
	void SetUserdata(int index, int value);
	void InitUserData();
	static void ReserveUserdata(int nNPCID, int nSize);
	static size_t GetUserdataSize(int nNPCID);
	
	virtual GActorMeshInfo* GetMeshInfo()				{ return m_pNPCInfo->m_pMeshInfo; }
	virtual bool IsHittable(GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo);		///< ���� �������� ����
	virtual int GetLevel() const override					{ return m_NPCStatus.nLevel; }
	virtual void SetLevel(int nLevel) override				{ m_NPCStatus.nLevel = nLevel; }
	virtual int GetCHA() const	{ return m_pNPCInfo->nCHA; }
	virtual float GetValidInteractionDistance();		///< ���ͷ��� ���� �Ÿ�

	virtual int8 GetGrade() const { return m_pNPCInfo->nGrade; }
	virtual ARMOR_ATTRIB GetArmorAttrib()	{ return m_pNPCInfo->nArmorType; }

	GEntity*		FindEntity(const MUID& uidEntity);
	virtual GEntityPlayer*	FindPlayer(const MUID& uidPlayer);
	virtual GEntityNPC*		FindNPC(const MUID& uidNPC) const;	
	GEntityPlayer*	FindPlayerByCID(int nCID);	
	
	bool IsNowPuppet();

	// ������������ ����
	virtual  bool IsNowInvincibility() override;

	virtual void Narration(const wchar_t* szText) override;
	virtual void NarrationNow(const wchar_t* szText) override;

	void Move(vec3 vTarPos);
	void MoveToMarker(int nMarkerID);
	void MoveToSensor(unsigned long nSensorID);
	void Attack(GEntityActor* pTarget);
	void Follow(GEntityActor* pActor, float fLeastDistance);	
	bool UseTalent(int nTalentID, MUID uidOpponent, bool bCheckValidate=false);
	bool UseTalentSelf(int nTalentID);	
	bool MoveToUseTalent(GEntityActor* pTargetActor, int nTalentID);
	bool MoveToUseTalentCloserWithDuration(GEntityActor* pTargetActor, int nTalentID, float fDuration);
	void Roam(float fRadius, float fDurationTime=0.0f);
	void Patrol(MWLua::table tableMarkers, int nPatrolType);
	void PatrolInstant( MWLua::table tableMarkers, int nPatrolType );
	void ClearJob();
	void Say(const wchar_t* szText, float fMaintainTime=-1.0f);
	void Ballon(const wchar_t* szText, float fMaintainTime=-1.0f);
	void Shout(const wchar_t* szText, float fMaintainTime=-1.0f);
	void SayAndNarration(const wchar_t* szText, float fMaintainTime=-1.0f);
	void SayAndNarrationNow(const wchar_t* szText);
	void ScriptSelf(const wchar_t* szText);
	void Wait(float fSecond);
	GEntityActor* Aggro(const wchar_t* pszType, float fDuration);
	int GetCurrentTalent() const;
	bool IsNowUsingTalent() const;
	bool IsNowUsingRotateTalent() const;
	void BreakPartJob(uint8 nPartID, MUID uidOpponent);
	bool SetTimer( int nTimerID, float fIntervalTime, bool bRepeat/*=false*/ );
	bool KillTimer( int nTimerID );	
	void Despawn(bool bSpawnEnable);
	void DespawnNow(bool bSpawnEnable);
	void Die(GEntityActor* pKiller);
	void Assist(GEntityNPC* pAlliedNPC);
	void Yell(int nRange);
	
	bool HasJob();
	bool HasMissionJob();
	deque<GJob*> GetMissionJobs();
	int GetSightRange(int nEnemyLevel);
	int GetMaxSightRange();
	void Flee(FleeType nFleeType, float fDurationTime);
	void FaceTo(GEntityActor* pTargetActor);
	void MoveToPos(GEntityActor* pTargetActor, float fDistance, float fDurationTime = JOB_DURATION_INFINITE_TIME);
	void Summon(int nNPCID, vec3 vStartPos);
	void EnableQPMarkPos();
	void DisableQPMarkPos();
	
	bool ExpRewardable() const;
	void DontRewardExp();

	bool QuestRewardable() const;
	void DontRewardQuest();
	GLootInfo* GetLootInfo();

	bool HasSession();
	GFieldNPCSession* GetSession();

	GNPCDebugger* GetDebugger() { return m_pDebugger; }		

	virtual const wchar_t* GetName() const override;

	GBehaviorStateMachine* GetBehaviorFSM(void);
	GJobMgr* GetJobMgr(void);

	GHateTable&			GetHateTable()			{ _ASSERT(m_pHateTable); return (*m_pHateTable); }
	GNPCDecay&			GetNPCDecay()			{ _ASSERT(m_pNPCDecay); return (*m_pNPCDecay);  }
	GNPCWaitFor&		GetNPCWaitFor()			{ _ASSERT(m_pNPCWaitFor); return (*m_pNPCWaitFor);  }
	GNPCTarget&			GetNPCTarget()			{ _ASSERT(m_pNPCTarget); return (*m_pNPCTarget);  }
	GNPCInteraction&	GetNPCInteraction()		{ _ASSERT(m_pNPCInteraction); return (*m_pNPCInteraction);  }
	GNPCLoot&			GetNPCLoot()			{ _ASSERT(m_pNPCLoot); return (*m_pNPCLoot);  }
	GNPCSwallow&		GetNPCSwallow()			{ _ASSERT(m_pSwallow); return (*m_pSwallow);  }
	GNPCQuest&			GetNPCQuest()			{ _ASSERT(m_pNPCQuest); return (*m_pNPCQuest);  }
	GNPCMinion&			GetNPCMinion()			{ _ASSERT(m_pNPCMinion); return (*m_pNPCMinion);  }
	GNPCHitcapsuleGroup& GetNPCHitcapsuleGroup() { _ASSERT(m_pNPCHitcapsuleGroup); return (*m_pNPCHitcapsuleGroup);  }
	GNPCRage&			GetNPCRage()			 { _ASSERT(m_pNPCRage); return (*m_pNPCRage);  }
	GNPCStress&			GetNPCStress()			 { _ASSERT(m_pNPCStress); return (*m_pNPCStress);  }
	GNPCMove&			GetNPCMove()			 { _ASSERT(m_pNPCMove); return (*m_pNPCMove);  }
	GNPCFirstStrike&	GetNPCFirstStrike()		 { _ASSERT(m_pNPCFirstStrike); return (*m_pNPCFirstStrike);  }
	GNPCMode&			GetNPCMode()			 { _ASSERT(m_pNPCMode); return (*m_pNPCMode);  }
	GNPCRewardOwner&	GetNPCRewardOwner()		 { _ASSERT(m_pNPCRewardOwner); return (*m_pNPCRewardOwner);  }
	GNPCRewarder&		GetNPCRewarder()		 { _ASSERT(m_pNPCRewarder); return (*m_pNPCRewarder);  }
	GNPCSetterForTest&	GetNPCSetterForTest()	 { _ASSERT(m_pNPCSetterForTest); return (*m_pNPCSetterForTest);  }
	GJobRunner*			GetJobRunner()			{ return m_pJobRunner; }

	// ���� Ŭ������ ��Ȳ�� ���� NULL�� �� �ֽ��ϴ� -------v
	GNPCAssist*			GetNPCAssist()				{ return m_pNPCAssist;  }
	GNPCBParts*			GetNPCBParts()				{ return m_pNPCBParts;  }
	GNPCSensorChecker*	GetNPCSensorChecker()		{ return m_pNPCSensorChecker;  }
	// ���� Ŭ������ ��Ȳ�� ���� NULL�� �� �ֽ��ϴ� -------^
				
	NPC_AA_TYPE			GetAAType() const;

	void SetThinkable(bool isThinkable)		{ m_isThinkable = isThinkable;	}
	bool IsThinkable(void);

	bool IsObject()	const			{ return NPC_TYPE_OBJECT == m_pNPCInfo->nNpcType; }
	
	SPAWN_ID GetSpawnID() const;
	bool IsStaticSpawned()			{ return INVALID_ID != GetSpawnID(); }
	void SetSpawnID(SPAWN_ID nID);

	void SendNPCIcon();
};

GEntityNPC* ToEntityNPC(GEntity* pEntity);


class GEntityNPC_Leaf : public GEntityNPC, public MMemPool<GEntityNPC_Leaf>
{
public:
	GEntityNPC_Leaf() : GEntityNPC() {}
	virtual ~GEntityNPC_Leaf() {}
};

#endif
