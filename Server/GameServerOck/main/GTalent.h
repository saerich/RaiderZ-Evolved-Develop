#ifndef _GTALENT_H
#define _GTALENT_H

#include "GTalentEffector.h"
#include "GTalentMove.h"
#include "GTalentHitcapsule.h"
#include "GTalentScript.h"
#include "GTalentStress.h"
#include "GTalentHit.h"
#include "GTalentLogger.h"
#include "GTalentExpirer.h"
#include "GTalentEventChecker.h"

class GEntityActor;
class GTalentRouter;
class GTalent;
class GTalentInfo;
class GTalentPhaseFSM;

// ��ų �ν��Ͻ�, ��ƼƼ�� ���� ��ų�� ����ϸ� �� Ŭ������ �����ȴ�.
class GTalent
{
protected:
	GTalentInfo*		m_pTalentInfo;
	GEntityActor*		m_pOwner;
	vec3				m_vInitialDir;		///< ó�� �� �ŷ�Ʈ�� �� ���� ����
	vec3				m_vInitialPos;		///< ó�� �� �ŷ�Ʈ�� �� ���� ��ǥ
	vec3				m_vInitialTargetPos;///< ó�� �� �ŷ�Ʈ�� �� ���� ��ǥ ��ǥ
	float				m_fElapsedTime;		///< �ŷ�Ʈ�� ����� �ð�
	float				m_fActTime;			///< Act Phase�� ������ �ð�
	TALENT_PHASE		m_nPhase;
	GTalentRouter*		m_pRouter;
	bool				m_bAutoCastingComplete;
	bool				m_bExpired;
	TALENT_TARGET_INFO	m_Target;
	GTalentEffector		m_TalentEffector;
	GTalentMove			m_TalentMove;
	GTalentHitcapsule	m_TalentHitcapsule;
	GTalentScript		m_TalentScript;
	GTalentStress		m_TalentStress;
	GTalentHit			m_TalentHit;
	GTalentLogger		m_TalentLogger;
	GTalentExpirer		m_TalentExpirer;
	GTalentEventChecker	m_TalentEventChecker;

	// ������ ����
	vector<GTalentObserver*>	m_vecObservers;

private:
	GTalent();

	void			OnEnterPhase(TALENT_PHASE nPhase);
	void			OnLeavePhase(TALENT_PHASE nPhase);
	void			OnPhaseEnd();

	void			InitStates();
	float			InitExpiredTime();

	// ���� �������� ���� ����
	void			AttachObservers();
	// ���� �������� ���� �и�
	void			DetachObservers();

	// �ŷ�Ʈ�� ����� �����Ͽ� ȿ���� ��� ��
	void			ApplyTalentEffect(TALENT_CONDITION nTalentCondition);

	// ��Ʈĸ���� ���, ��Ʈĸ�� ������ ó��
	void			UpdateHit();

	void			ChangeNPCMode();
	
	virtual void	RouteActTalent();

	virtual void	SetExpierdTime();
	
protected:
	// phase
	virtual void	OnStart();
	// �ŷ�Ʈ�� ����ɶ� ȣ�� (Finish or Canceled)
	virtual void	OnExit();
	virtual void	OnUpdatePhasePrepare(float fDelta);
	virtual void	OnUpdatePhaseAct(float fDelta);
	virtual void	OnUpdatePhaseExtra(float fDelta);
	virtual void	OnUpdatePhaseExtra2(float fDelta);
	virtual void	OnUpdatePhaseExtra3(float fDelta);
	virtual void	OnEnterPhasePrepare();
	virtual void	OnEnterPhaseAct();
	virtual void	OnEnterPhaseCanceled();
	virtual void	OnEnterPhaseFinish();
	virtual void	OnEnterPhaseExtra();
	virtual void	OnEnterPhaseExtra2();
	virtual void	OnEnterPhaseExtra3();
	virtual void	OnLeavePhaseAct();
	virtual void	OnLeavePhaseCanceled();
	virtual void	OnLeavePhasePrepare();
	virtual void	OnLeavePhaseFinish();
	virtual void	OnLeavePhaseExtra();
	virtual void	OnLeavePhaseExtra2();
	virtual void	OnLeavePhaseExtra3();
	virtual void	OnHitArea( GEntityActor* pVictim, uint16 nCheckTime, int8 nCapsuleGroupIndex, int8 nCapsuleIndex );

protected:
	void			ChangePhase(TALENT_PHASE nNextPhase);
	
public:
	GTalent(GEntityActor* pOwner, GTalentInfo* pTalentInfo, TALENT_TARGET_INFO uidTarget=TALENT_TARGET_INFO::Invalid(), bool bGainStress=true);
	virtual ~GTalent();

	bool			Update(float fDelta);
	void			UpdateExpired(float fDelta);
	virtual void	Start();

	void			Cancel(bool bPostCommand=true);
	void			CancelForce(bool bPostCommand);
	
	void			ActTalent(TALENT_TARGET_INFO Target);
	void			ApplyDelayedAct();

	void			OnGainDamage(GEntityActor* pTarget);

	bool IsCastingComplete();
	// �ŷ�Ʈ ����� ��ȯ
	GEntityActor*		GetOwner()			{ return m_pOwner; }
	// �ŷ�Ʈ ��ǥ���� ��ȯ
	TALENT_TARGET_INFO&	GetTarget()			{ return m_Target; }
	// �ŷ�Ʈ ��ǥ���� UID ��ȯ
	MUID				GetTargetUID()		{ return m_Target(); }
	// �ŷ�Ʈ �������� ��ȯ
	GTalentInfo*	GetInfo()		{ return m_pTalentInfo; }
	// �ŷ�Ʈ ���̵� ��ȯ
	int				GetID() { return m_pTalentInfo->m_nID; }
	// �ŷ�Ʈ ����ܰ� ��ȯ
	TALENT_PHASE	GetPhase()			{ return m_nPhase; }
	// �����ڰ� �ִ��� ����
	bool			HasVictim()			{ return m_TalentHit.HasVictim(); }

	vec3			GetInitPos()		{ return m_vInitialPos; }
	vec3			GetInitDir()		{ return m_vInitialDir; }
	vec3			GetInitTargetPos()	{ return m_vInitialTargetPos; }
	GTalentMove::MOVEINFO	GetExpectedMoveInfo(float fActElapsedTime);			///< �ش� ElapsedTime���� ����Ǵ� ��ġ��
	GTalentRouter*	GetRouter()			{ return m_pRouter; }
	// ��ǥ������ ��ġ�� ���, ��ȯ���� ��ǥ���Ͱ� �ִ��� ����
	bool			GetTargetPos(vec3& vTargetPos);

	GEntityActor*  GetTargetActor();
	// Act Phase �� ����� �ð� ��ȯ
	float			GetActElapsedTime();
	float			GetExpiredTime();
	
	bool			IsExpired();
	bool			IsNowInvincibleTime();
	bool			IsNowSuperarmorTime();
	bool			IsNowAvoidTime();
	bool			IsNowCasting();
	bool			IsCancelable();
	bool			IsMovable();

	// ACT�ÿ� ��ǥ�� ���� ������ �������� ����
	bool			IsAimingDirectionTalent();

	// �ŷ�Ʈ�� ���������� �˸�
	void			TalentFinish();

	void			HitArea(GEntityActor* pVictim, uint16 nCheckTime, int8 nCapsuleGroupIndex, int8 nCapsuleIndex);
	float			GetElapsedTime() { return m_fElapsedTime; }

	// �������� �����Ŵ
	void			AttachObserver(GTalentObserver* pOvr);	
};

#endif