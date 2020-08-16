#pragma once;

#include "XMovableFactor.h"


// Movable Go - Interpolation Go & Direction Go - �� ���õ� ���
#define CONST_MOVABLE_INTERPOLATION_CHANGE_MOTION_MARGIN_SQ_DIST	5.f			/* �̰ͺ��� ������ ����� �������� �ʰ� ���� */
#define CONST_MOVABLE_INTERPOLATION_IGNORE_SPECIAL_STATE_SQ_SIDT	10000000.f	/* �̰ͺ��� ũ�� Ư����Ȳ�� �����ϰ� ������ ���� */
#define CONST_MOVABLE_INTERPOLATION_VIRTUAL_TARGET_MIN_SQ_DIST		0.5f		/* �̰ͺ��� ������ ����Ÿ���� ������ ��ǥ�� ���� */
#define CONST_MOVABLE_INTERPOLATION_VIRTUAL_TARGET_MAX_SQ_DIST		1000000.f	/* �̰ͺ��� ũ�� ����Ÿ���� ������ ��ǥ�� ����, 10���� */
#define CONST_MOVABLE_INTERPOLATION_VIRTUAL_TARGET_DETERM_FACTOR	500.f		/* �� ���� Ŭ���� �հ��� ����Ÿ�� ����, 5���� �� */
#define CONST_MOVABLE_INTERPOLATION_DEFAULT_ROTATION_SPEED			200.f		/* OnRotaion �ݹ��� ������ �⺻ ȸ�� �ӵ� */
#define CONST_MOVABLE_INTERPOLATION_IGNORE_MOVE_ON_TALENT_SQ_DIST	40000.f		/* Ż��Ʈ ���� ª�� �Ÿ��� �����̸� �����Ѵ�. 2���� */


// TriggerGo �� �����ϱ� ���� �Ķ����
class XMovableGoParam : public MMemPool< XMovableGoParam >
{
public:
	enum E_GO_TYPE
	{
		E_GO_DIRECTION = 0,
		E_GO_INTERPOLATION,
		E_GO_MOUSE_MOVE
	};


	E_GO_TYPE			m_nType;

	vec3				m_vTargetPos;
	vec3				m_vTargetDir;

	float				m_fMoveSpeed;
	float				m_fRotateSpeed;
	unsigned int		m_nBoostFlags;

	unsigned int		m_nMovementFlags;
	bool				m_bRunMotion;

	float				m_fLimitTime;

	MOVE_CHECK_TYPE		m_eMoveCheckType;

	union
	{
		bool			m_bKeyPressed;
		bool			m_bForceInterpolation;
	};



public:
	XMovableGoParam(E_GO_TYPE _eGoType)
		: m_nType(_eGoType)
		, m_vTargetPos(vec3::ZERO)
		, m_vTargetDir(vec3::ZERO)
		, m_fMoveSpeed(0.0f)
		, m_fRotateSpeed(DEFAULT_ROTATE_SPEED)
		, m_nBoostFlags( BOOST_NONE)
		, m_nMovementFlags(0)
		, m_fLimitTime( NO_LIMIT_TIME)
		, m_bRunMotion(true)
		, m_bKeyPressed(false)
		, m_eMoveCheckType(MCT2_NONE)
	{
	}

	virtual ~XMovableGoParam()
	{
	}


	inline void			Set( const vec3& vTargetPos, const vec3& vTargetDir, float fMoveSpeed, float fRotateSpeed, unsigned int nBoostFlags, unsigned int nMovementFlags, bool bRunMotion, float fLimitTime =NO_LIMIT_TIME);
	inline void			SetRotate( const vec3& vTargetDir, float fRotateSpeed);

	inline void			SetKeyPress(bool bKeyPressed);
 	inline void			SetGoInterpolationForce(bool bForce);
 	inline void			SetGoInterpolationMotion(unsigned int nMovementFlags, bool bRun);
	inline void			SetMoveCheckType(MOVE_CHECK_TYPE eType) { m_eMoveCheckType = eType; } 

	inline E_GO_TYPE	GetParamType()			{ return m_nType; }

	inline bool			IsKeyPressed()			{ return m_bKeyPressed; }
	inline bool			IsForceInterpolation()	{ return m_bForceInterpolation; }
};



//----------------------------------------------------------------------------------------------------
inline void XMovableGoParam::Set(const vec3& vTargetPos, const vec3& vTargetDir, float fMoveSpeed, float fRotateSpeed, unsigned int nBoostFlags, unsigned int nMovementFlags, bool bRunMotion, float fLimitTime)
{
	m_vTargetPos.Set(vTargetPos);
	m_vTargetDir.Set(vTargetDir);

	m_fMoveSpeed = fMoveSpeed;
	m_fRotateSpeed = fRotateSpeed;
	m_nBoostFlags = nBoostFlags;

	m_nMovementFlags = nMovementFlags;
	m_bRunMotion = bRunMotion;

	m_fLimitTime = fLimitTime;

	m_bForceInterpolation = false;
}
//----------------------------------------------------------------------------------------------------
inline void XMovableGoParam::SetKeyPress(bool bKeyPressed)
{
	_ASSERT( m_nType == E_GO_DIRECTION );
	m_bKeyPressed = bKeyPressed;
}
//----------------------------------------------------------------------------------------------------
inline void XMovableGoParam::SetGoInterpolationForce(bool bForce)
{
	// ������ Interpolation �ǵ��� ����
	m_bForceInterpolation = bForce;
}
//----------------------------------------------------------------------------------------------------
inline void XMovableGoParam::SetGoInterpolationMotion(unsigned int nMovementFlags, bool bRun)
{
	_ASSERT( m_nType == E_GO_INTERPOLATION );

	// �ӵ��� ��� 0�̸� Interpolation �ǹ̰� ����.
	// �ܺο��� _ASSERT�� �߻����� �ʵ��� SetGoInterpolation�� �������־���Ѵ�.
	_ASSERT( (m_fMoveSpeed > FLT_EPSILON) || (m_fRotateSpeed > FLT_EPSILON) );

	// ������ ���� �÷��� ����
	m_nMovementFlags = nMovementFlags;
	m_bRunMotion = bRun;
}

void XMovableGoParam::SetRotate( const vec3& vTargetDir, float fRotateSpeed )
{
	m_vTargetDir.Set(vTargetDir);
	m_fRotateSpeed = fRotateSpeed;
}

//----------------------------------------------------------------------------------------------------
