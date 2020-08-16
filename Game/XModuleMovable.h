#ifndef _XMODULE_MOVABLE_H
#define _XMODULE_MOVABLE_H

#include "MTypelist.h"

#include "XModule.h"
#include "XMovableGoParam.h"
#include "XMovableProcessor.h"
#include "XMotion.h"
#include "XCharacter.h"
#include "XTimer.h"
#include "XObjectManager.h"

class XModuleEntity;
class XModuleCollision;

// ������Ʈ �̵��� �����ϴ� ���
class XModuleMovable : public XModule, public MMemPool<XModuleMovable>
{
	DECLARE_ID(XMID_MOVABLE);

	friend class XMovableProcessor;
	friend class XMovableProcInterpolation;
	friend class XMovableProcObject;
	friend class XMovableProcTerrain;
	friend class XMovableProcTerrainNPC;
	friend class XMovableProcWater;
	friend class XMovableProcThrow;

	//////////////////////////////////////////////////////////////////////////
	// ModuleMovable�� �̵������ �޾����� �ش� �̵��� Ÿ�������� �浹Ȯ��
	// ���� ���� �Ǵ��Ͽ� ���������� ModuleEntity�� ��ǥ�� �����Ѵ�.
	//
	XModuleEntity*			m_pModuleEntity;
	XModuleCollision*		m_pModuleCollision;
	E_OWNER_TYPE			m_eOwnerType;


	//////////////////////////////////////////////////////////////////////////
	// Movable Factor
	//
	XMovableFactor			m_MovableFactor;
	bool					m_bActive;

	//////////////////////////////////////////////////////////////////////////
	// OnUpdate
	// _PreUpdate -> _RunMovableProcessor -> _PostUpdate ������ ȣ��
	// _PreUpdate������ �ӵ��� ���õ� ������ ����
	// _RunMovableProcessor������ �����ӿ� ���õ� ����/�浹����
	// ���μ��� ����� ���� �̵���ǥ ����
	// _PostUpdate������ ����Ȯ�ι� ������¿� ���� �޽��� ������
	//
	void					_PreUpdateEnvironment();			// ȯ�濡 ���õ� ����
	void					_PreUpdateAnimationDummyNode();		// ���̳�� ���� ��
	void					_PreUpdateKnockback(float fDelta);	// �˹� ���� ��
	void					__PreUpdateKnockback_UpVelocity( float fDelta, XMovableFactor::SKnockBack &KNOCK_BACK, vec3 &vForce );

	void					_RunMovableProcessor(float fDelat);	// �����ӿ� ���õ� ���μ��� ���. ���� �̵���ǥ�� �����ȴ�.

	void					_PostUpdateJump();					// ����
	void					_PostUpdateFalling();				// ������ - ������ ���� ���ϻ��´� �ƴϴ�.
	void					_PostUpdateReleaseJumpAndFall();	// ����(Jump)�� ������(Falling) ���� Ǯ�� - Landing
	void					_PostUpdateHoldExpception(float fDelta);		// ������ �������� ���� ó��
	void					_PostUpdateThrow();					// ������ ���� Ȯ��
	void					_PostUpdateSwim();					// ���� ���� Ȯ��
	void					_PostUpdateInterpolation();			// ���� ������(Interpolation)�� ���� ���� Ȯ��
	void					_PostUpdateKnockback();				// �˹�
	void					_PostUpdateDie();					// �׾����� ó��
	void					_PostUpdateMouseMove(float fDelta);	// ���콺 �̵�ó��

	//////////////////////////////////////////////////////////////////////////
	// Movable Processor
	//
	enum MOVABLE_PROCESSOR
	{
		E_MOVABLE_PROCESSOR_INTERPOLATION,
		E_MOVABLE_PROCESSOR_THROW,
		E_MOVABLE_PROCESSOR_OBJECT,
		E_MOVABLE_PROCESSOR_TERRAIN,
		E_MOVABLE_PROCESSOR_WATER,

		E_MOVABLE_PROCESSOR_NUM
	};
	XMovableProcessor*		m_MovableProcessors[E_MOVABLE_PROCESSOR_NUM];

	void					DebugCheckRunProc(XMovableProcessor* pMovableProcessor, float fDelta, vec3 vCurrentPos, vec3 vTo);

	bool					CheckUsableDummyLoc();

protected:

	virtual void			OnInitialized();
	virtual void			OnUpdate(float fDelta);
	virtual XEventResult	OnEvent(XEvent& msg);
	virtual void			OnSubscribeEvent();

public:

	XModuleMovable(XObject* pOwner=NULL);
	virtual ~XModuleMovable(void);

	void SetInitialFactors(bool bUsingGravity);

	//////////////////////////////////////////////////////////////////////////
	//
	// ������Ʈ�ÿ� �ʿ��� �������� �������ִ� �޽��
	// ������Ʈ �÷��׿� Movable Factor ���� �����ؼ�
	// OnUpdate�ÿ� ������ �ݴϴ�.
	//
	//////////////////////////////////////////////////////////////////////////

	void					TriggerGo( XMovableGoParam* pParam);	///< XMovableGoParam ������� ������ �˸�
	inline void				TriggerStop();							///< ������ ����. �ӵ� 0
	void					TriggerRotate(XMovableGoParam* pParam);

	void					TriggerSwim(bool bSwim);				///< ��������
	inline bool				TriggerJump(vec3* vResultDir = NULL, float fRatio =1.0f);	///< ��������, �ö󰡱�/�������� ��� ����
	inline void				TriggerFall();							///< ������ ���� �ϰ��� �ƴ� ����������
	inline void				TriggerDie(bool bDie);

	inline void				TriggerTalentActive(int nTalentActiveID);
	inline void				TriggerTalentDeActive();
	inline void				TriggerStance();
	inline void				TriggerStunActive();
	inline void				TriggerStunDeActive();

	inline void				TriggerGuardDefense(bool bDefense);

	//////////////////////////////////////////////////////////////////////////
	// �˹���� Ʈ����
	//
	void					TriggerKnockback(const vec3& dir, const vec3& pos, const vec3& target, float fPower, float fVelocity, float fKBEventDist);
	void					TriggerUppered( const vec3& dir, const vec3& pos, const vec3& target, float fPower, float fUPDownTime, float fUpVelocity );
	void					TriggerDrag(const vec3& dir, const vec3& pos, const vec3& target, float fPower, float fKBEventDist);


	//////////////////////////////////////////////////////////////////////////
	// ��������, �������� ���� Ʈ����
	//
	inline void				TriggerGrappledStart( const MUID& _vAttacker, float fVelocity, vec3 vHitPos, float fInterpolationTime = 0.0f);
	void					TriggerGrappledBeThrown();
	inline void				TriggerGrappledEnd();

	//////////////////////////////////////////////////////////////////////////
	// ���� ����
	//
	void					ForceStop(bool bAlsoStopAccelation = false);
	void					ForceJump(vec3* vResultDir = NULL, float fRatio =1.0f);

	//////////////////////////////////////////////////////////////////////////
	// ����
	//
	void					Warp(const vec3& vPos, const vec3& vDir);

	//////////////////////////////////////////////////////////////////////////
	// Movable Factor
	//
	const XMovableFactor&	GetMovableFactor()	{ return m_MovableFactor; }
	void					InitMovableFactorFalling();
	
	//////////////////////////////////////////////////////////////////////////
	// ShovePlayer ������Ʈ
	//
	void					UpdateShove(vec3& vTo, float fDelta);

	void SetActive(bool b) { m_bActive = b; }
};

//----------------------------------------------------------------------------------------------------
inline void XModuleMovable::TriggerStop()
{
	if (((m_MovableFactor.GENERAL_STATE.m_bGoingDirection) && (m_MovableFactor.GENERAL_STATE.m_bLanding)) ||
		m_MovableFactor.MOUSE_MOVE.m_bWorking)
		ForceStop();
}
//----------------------------------------------------------------------------------------------------
inline void XModuleMovable::ForceStop(bool bAlsoStopAccelation)
{
	m_MovableFactor.GENERAL_STATE.m_bGoingDirection = false;
	m_MovableFactor.INTERPOLATION.m_bWorking = false;
	m_MovableFactor.MOUSE_MOVE.m_bWorking = false;

	m_MovableFactor.VELOCITY.m_vVelocity.Set(vec3::ZERO);
	m_MovableFactor.VELOCITY.m_bUsingAccel = !bAlsoStopAccelation;
}
//----------------------------------------------------------------------------------------------------
inline bool XModuleMovable::TriggerJump(vec3* vResultDir, float fRatio)
{
	if(m_MovableFactor.GENERAL_STATE.m_bJumping || m_MovableFactor.GENERAL_STATE.m_bSliding || m_MovableFactor.GRAPPLE.m_bGrappled) 
		return false;

	ForceJump(vResultDir, fRatio);
	return true;
}
//----------------------------------------------------------------------------------------------------
inline void XModuleMovable::ForceJump(vec3* vResultDir, float fRatio)
{
	vec3 vCurrentVel( m_MovableFactor.VELOCITY.m_vVelocity );
	vCurrentVel.z = 0.f;
	vCurrentVel.Normalize();

	if ( vResultDir)
		vResultDir->Set( vCurrentVel);


	vCurrentVel.x *= XCONST::MOVE_SPEED_NONE * fRatio;
	vCurrentVel.y *= XCONST::MOVE_SPEED_NONE * fRatio;
//	vCurrentVel.z = XCONST::JUMP_SPEED * ( (fRatio > 1.0f) ? (1.0f / fRatio) : fRatio);
	vCurrentVel.z = XCONST::JUMP_SPEED * fRatio;

	m_MovableFactor.GENERAL_STATE.m_bLanding		= false;
	m_MovableFactor.GENERAL_STATE.m_bJumping		= true;
	m_MovableFactor.GENERAL_STATE.m_bJumpAscending	= true;
	m_MovableFactor.VELOCITY.m_vVelocity = vCurrentVel;
	m_MovableFactor.GENERAL_STATE.m_vJumpPosForCamera = m_pOwner->GetPosition();
	m_MovableFactor.INTERPOLATION.m_fJumpSpeedRatio = fRatio;
}
//----------------------------------------------------------------------------------------------------
inline void XModuleMovable::TriggerFall()
{
	m_MovableFactor.GENERAL_STATE.m_bFallingButNotJump = true;
}
//----------------------------------------------------------------------------------------------------
inline void	XModuleMovable::TriggerGrappledStart( const MUID& _vAttacker, float fVelocity, vec3 vHitPos, float fInterpolationTime /*= 0.0f*/)
{
	m_MovableFactor.GENERAL_STATE.m_bJumping			= false;
	m_MovableFactor.GENERAL_STATE.m_bFallingButNotJump	= false;
	m_MovableFactor.GENERAL_STATE.m_bLanding			= false;
	m_MovableFactor.GENERAL_STATE.m_bSwim				= false;
	m_MovableFactor.GENERAL_STATE.m_bColTerrain			= false;

	m_MovableFactor.GRAPPLE.m_bGrappled = true;
	m_MovableFactor.GRAPPLE.m_uidGrappledAttacker = _vAttacker;
	m_MovableFactor.GRAPPLE.m_vGrappledVelocity = fVelocity;
	m_MovableFactor.GRAPPLE.m_bGrappledBeThrown = false;
	m_MovableFactor.GRAPPLE.m_fGrappledElapsed = 0.0f;
	m_MovableFactor.GRAPPLE.m_fGrappledStartInterpolationTime = fInterpolationTime;
	m_MovableFactor.GRAPPLE.m_fGrappledRemainStartInterpolationTime = fInterpolationTime;
	m_MovableFactor.GRAPPLE.m_vGrappledStartDir = m_pOwner->GetDirection();
	m_MovableFactor.GRAPPLE.m_vGrappledReadyPos = m_pOwner->GetPosition();
	m_MovableFactor.GRAPPLE.m_vGrappledStartPos = vHitPos;
	m_MovableFactor.GRAPPLE.m_vGrappledEndDir = m_pOwner->GetDirection();
}
//----------------------------------------------------------------------------------------------------
inline void	XModuleMovable::TriggerGrappledEnd()
{
	m_MovableFactor.GRAPPLE.m_bGrappled = false;
	m_MovableFactor.GRAPPLE.m_bGrappledStuck = false;
	m_MovableFactor.GRAPPLE.m_uidGrappledAttacker = MUID::ZERO;
	m_MovableFactor.GRAPPLE.m_bGrappledBeThrown = false;
	m_MovableFactor.GRAPPLE.m_vGrappledVelocity = vec3::ZERO;
}
//----------------------------------------------------------------------------------------------------
inline void XModuleMovable::TriggerTalentActive(int nTalentActiveID)
{
	m_MovableFactor.GENERAL_STATE.m_bTalentActive		= true;
	m_MovableFactor.GENERAL_STATE.m_nTalentActiveID		= nTalentActiveID;
}
//----------------------------------------------------------------------------------------------------
inline void	XModuleMovable::TriggerTalentDeActive()
{
	m_MovableFactor.GENERAL_STATE.m_bTalentActive		= false;
	m_MovableFactor.GENERAL_STATE.m_nTalentActiveID		= -1;
}
//----------------------------------------------------------------------------------------------------
inline void XModuleMovable::TriggerGo( XMovableGoParam* pParam )
{
	_ASSERT( pParam != NULL );

	// �⺻ ��� �ʱ�ȭ
	m_MovableFactor.GENERAL_STATE.m_bGoingDirection = false;
	m_MovableFactor.INTERPOLATION.m_bWorking = false;
	m_MovableFactor.MOUSE_MOVE.m_bWorking = false;

	if(pParam->m_nType == XMovableGoParam::E_GO_DIRECTION)
	{
		// �ӵ� ����
		float fSpeed = pParam->m_fMoveSpeed;
		vec3& vMovementDir = pParam->m_vTargetDir.Normalize();
		if (! m_MovableFactor.GENERAL_STATE.m_bLanding)
		{
			fSpeed *= 0.2f;
		}

		m_MovableFactor.VELOCITY.m_vVelocity.x = vMovementDir.x * fSpeed;
		m_MovableFactor.VELOCITY.m_vVelocity.y = vMovementDir.y * fSpeed;

		// ���� �÷��� ����
		m_MovableFactor.GENERAL_STATE.m_bGoingDirection = true;
	}
	else if(pParam->m_nType == XMovableGoParam::E_GO_INTERPOLATION)
	{
		// ���������̼� ���μ����� ���� �̵�.
		// SetPosDir�� �Ҷ� ���������̼� ���μ����� Ȱ��ȭ �ȴ�.
		// �ӵ��� ���������̼� ���μ��� �� ó�����ش�.
		m_MovableFactor.INTERPOLATION.Set(
			pParam->m_vTargetPos, pParam->m_vTargetDir, pParam->m_fMoveSpeed, pParam->m_fRotateSpeed, pParam->m_nBoostFlags,
			pParam->m_nMovementFlags, pParam->m_bRunMotion, pParam->m_fLimitTime, pParam->m_bForceInterpolation, pParam->m_eMoveCheckType);

		m_MovableFactor.INTERPOLATION.m_vStartPosForDummyMove = m_pOwner->GetPosition();
	}
	else if(pParam->m_nType == XMovableGoParam::E_GO_MOUSE_MOVE)
	{
		// ���콺�� �̵�
		// ���������̼ǰ� ������ �̵��̴�.
		// Ÿ���� �з��� ���ؼ� ��� �ϱ� ���ؼ� �߰� �Ȱ��̴�.
		m_MovableFactor.INTERPOLATION.Set(
			pParam->m_vTargetPos, pParam->m_vTargetDir, pParam->m_fMoveSpeed, pParam->m_fRotateSpeed, pParam->m_nBoostFlags,
			pParam->m_nMovementFlags, pParam->m_bRunMotion, pParam->m_fLimitTime, pParam->m_bForceInterpolation, pParam->m_eMoveCheckType);

		m_MovableFactor.MOUSE_MOVE.m_bWorking = true;
	}
}
//----------------------------------------------------------------------------------------------------
inline void XModuleMovable::TriggerStance()
{
	m_MovableFactor.GENERAL_STATE.m_bStance = true;
}
//----------------------------------------------------------------------------------------------------
inline void XModuleMovable::TriggerStunActive()
{
	m_MovableFactor.GENERAL_STATE.m_bStun = true;
}
//----------------------------------------------------------------------------------------------------
inline void XModuleMovable::TriggerStunDeActive()
{
	m_MovableFactor.GENERAL_STATE.m_bStun = false;
}
//----------------------------------------------------------------------------------------------------
inline void XModuleMovable::TriggerDie( bool bDie )
{
	ForceStop();

	m_MovableFactor.GENERAL_STATE.m_bDie = bDie;
}

inline void XModuleMovable::TriggerGuardDefense( bool bDefense )
{
	m_MovableFactor.GENERAL_STATE.m_bGuardDefense = bDefense;
}
//----------------------------------------------------------------------------------------------------
#endif // _XMODULE_MOTION_H