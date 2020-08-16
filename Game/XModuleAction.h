#ifndef _XMODULE_ACTION_H
#define _XMODULE_ACTION_H

#include "XModule.h"
#include "XModuleMotion.h"
#include "XDef.h"
#include "CSMotionFactorInfo.h"
#include "XActor.h"
#include "XActionDie.h"

class XModuleMotion;
class XMovableGoParam;

struct _MovementInfo;

struct stServerKnockbackVictimInfo
{
	vec3	vKnockbackDir;
	vec3	vKnockbackVictimPos;
	vec3	vKnockbackVictimTargetPos;

	stServerKnockbackVictimInfo(vec3& vDir, vec3& vPos, vec3& vTargetPos)
	{
		vKnockbackDir = vDir;
		vKnockbackVictimPos = vPos;
		vKnockbackVictimTargetPos = vTargetPos;
	}
};

struct stMotionFactorResult
{
	bool	bUseMFAnimation;

	stMotionFactorResult()
	{
		bUseMFAnimation			= false;
	}
};

// ����� ����Ǵ� �ð��� üũ�ؼ� ��ȿ�� ���� ��� ������ �����ϴ� Ŭ����
class XMotionStopChecker
{
private:
	bool	m_bCheck;
	DWORD	m_dwTimer;

public:
	XMotionStopChecker()	{ Reset();				}

	void Reset()			{ m_bCheck = false;		}

	bool IsValid()
	{
		DWORD curr_time = timeGetTime();
		if ( m_bCheck == false)
		{
			m_dwTimer = curr_time;
			m_bCheck = true;
		}

		return ( (curr_time - m_dwTimer) > 200);
	}
};



#define  BUFF_ANIMATION_START_NAME	L"none_"

/// @brief ������Ʈ�� �׼��� FSM ���·� �����ϴ� ��� - ��Ǻ��� ���� ����
/// @author �����
/// @see
/// �� ����� XMyPlayer, XNetPlayer, XNonPlayer ��� ����ϹǷ� �� ��⿡ ������ �޼��� ����(post) ���� �Լ��� ������ �ȵ˴ϴ�.
class XModuleAction : public XModule, public MMemPool<XModuleAction>
{
	DECLARE_ID(XMID_ACTION);

private:
	XActionDie						m_ActionDie;
	bool							m_bMFStunNow;

	bool							m_bHoldMotion;
	float							m_fHoldMotionCounter;

	XMotionStopChecker				m_MotionStopChecker;
	vec3							m_vPrevPos;
	vec3							m_vPrevDir;

	//////////////////////////////////////////////////////////////////////////
	// �������
	//
	//void							ActMF_BeatenFake(MDIRECTION nDirection);
	//bool							ActMF_BeatenFake2(int nWeight, MDIRECTION nDirection);
	bool							ActMF_Beaten(int nWeight, MDIRECTION nDirection);
	bool							ActMF_Knockback(vec3& dir, vec3& pos, vec3& targetPos, int nWeight, bool bChangeMotion = true);
	bool							ActMF_FakeKnockback(vec3& dir, vec3& pos, vec3& targetPos, int nWeight);
	bool							ActMF_Drag(vec3& dir, vec3& pos, vec3& targetPos, int nWeight);
	bool							ActMF_Stun(int nWeight);
	bool							ActMF_Deflect(int nWeight);	
	bool							ActMF_Throwup(vec3& dir, vec3& pos, vec3& targetPos, int nWeight);
	bool							ActMF_Knockdown(int nWeight);
	bool							ActMF_Uppered(vec3& dir, vec3& pos, vec3& targetPos, int nWeight);
	bool							ActMF_Grappled(const vec3& targetpos, const vec3& targetdir, int nWeight);


	//////////////////////////////////////////////////////////////////////////
	// ��� Ÿ�� / �̸�
	//
	const wchar_t*					GetBeatenMotionName(ENTITY_TYPEID nEntityTypeID, MDIRECTION nDirection);
	inline const XMotionTypes		GetMotionType(unsigned int nMovementFlags);
	const wchar_t*					GetMoveMotionName();	// �޸���� �����̳�
	const wchar_t*					GetIdelMotionName();	// ���󿡼� ������ �������� ������

	void							SetMyMovementAction( XMovableGoParam* pGoParam, bool bMove );

	bool							CheckChangeStanceMotion(bool bMove, const wchar_t* szMotionName, XMotionTypes nType);
	bool							CheckUsableUpperMotion(bool bMove, const wchar_t* szMotionName, XMotionTypes nType);
	void							CheckPostMFAnimation(bool bSetAni);

protected:
	virtual void					OnInitialized();
	virtual void					OnFinalized()	{}
	virtual void					OnSubscribeEvent();
	virtual XEventResult			OnEvent(XEvent& msg);
	virtual void					OnUpdate(float fDelta);

	XModuleMotion*					m_pModuleMotion;
	XModuleMovable*					m_pModuleMovable;

	vec3							m_vAimingDir;	//Aiming Direction
	XActor*							GetActorOwner()	{ return static_cast<XActor*>(m_pOwner); }	///< ������ ������Ʈ

	

	bool							IsSwim();
	void							ChangeMotionToIdleMotion();
public:

	//////////////////////////////////////////////////////////////////////////
	// ���� / �Ҹ�
	//
	XModuleAction(XObject* pOwner=NULL);
	virtual ~XModuleAction(void);


	//////////////////////////////////////////////////////////////////////////
	// Actor ����
	//
	bool							Jump(XMotionTypes nMotionType, bool bForced=false, vec3* vResultDir = NULL);
	bool							Tumble(XMotionTypes nMotionType);

	void							UpdateMyMovementAction(float fDelta, XMovableGoParam* pGoParam);
	void							UpdateNetMovementAction(float fDelta);
	void							UpdateNonMovementAction(float fDelta);

	void							SetMyMovementActionByMouseMove(XMovableGoParam* pGoParam);

	void 							Stop( bool bForced =false);
	void 							StopMove(bool bForced=false);
	void 							StopUpperAni();
	void							ActionFalling();
	bool							ActionAiming();
	void							ActionGuard();
	void							ActionGuardRelease();
	void 							Shoot();
	void 							Guard();
	void 							ChangeStance(CHAR_STANCE nStanceTo, bool bQuick);
	void							ChangeStanceMoveOrStop(CHAR_STANCE nStanceTo, bool bQuick, bool bMove);
	void 							ChangeWeapon(CHAR_STANCE nSwitching, bool bQuick);
	void							Idle();
	void							ActionRebirth();	// ��Ȱ�� ���� �׼� ó��
	void 							Guarded(bool bKeepGuard);
	void							GuardKnockBack();
	void							SitDown();
	void							Swim();
	void							SitRise();
	void							ActionBedSleep();

	bool							ChangeLootingMotion(const wchar_t* szAniName=NULL);
	bool							ChangeMotionQuestInteraction();

	void							EndStun(bool bSendMotionEvent = true);

	void							SetPause( bool bPause);

	//////////////////////////////////////////////////////////////////////////
	// �������
	//
	void 							ActMF(MF_STATE nState, int nWeight, UIID nAttackerUIID, stServerKnockbackVictimInfo stServerKnockbackInfo, int nAttackedTalentID, stMotionFactorResult& outResult);
	void 							ActKnockBack(vec3& dir, int nWeight, bool bChangeMotion=true);

	// �÷��̾� �׼� ����
	bool 							IsCurAiming();
	vec3 							GetAimingDir()	{ return m_vAimingDir;	}
	void							InitPrevDir()	{ m_vPrevDir = m_pOwner->GetDirection(); }		// �����̼� üũ �ʱ�ȭ

	// NPC �׼�
	void							NPC_Walk();

	// ���� �׼�
	void							StartBuffAnimation(int nBuffID, wstring& strAniName);
	void							EndBuffAnimation(int nBuffID);

	//XModuleMotion
	wstring							GetCurrentMotionName();
	float							GetMotionTime();
	bool							IsCurrMotionPlayDone();
	bool							IsCameraLook();

	bool							CheckUsableBeatenFake();
	bool							IsStun()	{ return m_bMFStunNow; }

	// Die
	void 							Die(MF_STATE nMFState = MF_NONE, bool bSetAni = true);
	void							EndDie();			// ������ ���� ������ ó�� ����
	void 							DoneDIe();
	bool							IsDieAniEnd();
	bool							CheckUsableDieAnimation();

};
//----------------------------------------------------------------------------------------------------
inline const XMotionTypes XModuleAction::GetMotionType( unsigned int nMovementFlags )
{
	if ( (nMovementFlags & 0x000F) == 0)
		nMovementFlags = MOVEMENTFLAG_FORWARD;

	XMotionTypes nMotionType = MT_DEFAULT;

	if (nMovementFlags & MOVEMENTFLAG_FORWARD) nMotionType = MT_FORWARD;
	else if (nMovementFlags & MOVEMENTFLAG_BACKWARD) nMotionType = MT_BACKWARD;
	else if (nMovementFlags & MOVEMENTFLAG_LEFT) nMotionType = MT_LEFT;
	else if (nMovementFlags & MOVEMENTFLAG_RIGHT) nMotionType = MT_RIGHT;

	return nMotionType;
}
//----------------------------------------------------------------------------------------------------
MDIRECTION GetMFBeatenDirection(XActor* pAttacker, XActor* pVictim, int nTalentID);
//----------------------------------------------------------------------------------------------------

#endif