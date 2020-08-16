#ifndef _XMOVABLE_FACTOR_H
#define _XMOVABLE_FACTOR_H

const float DEFAULT_MOVE_SPEED		= 100.0f;
const float DEFAULT_ROTATE_SPEED	= 100.0f;


// flags of boost speed
#define BOOST_NONE		0x00
#define BOOST_MOVE		0x01
#define BOOST_ROTATE	0x02

#define NO_LIMIT_TIME	0.0f

enum MOVE_CHECK_TYPE
{
	MCT2_NONE,
	MCT2_TARGET_HEGITH,			// Ÿ�� ���̷� �浹 üũ
	MCT2_DIRECT,				// ���� �˻縦 ���� �ʰ� ����亸�� �մϴ�.
	MCT2_MOUSE_MOVE
};

class XMovableFactor
{
private:

	friend class XModuleMovable;

	vec3	m_vResultDir;
	vec3	m_vResultPos;
	vec3	m_vResultUpVec;

public:

	XMovableFactor()
		: m_vResultPos(vec3::ZERO)
		, m_vResultDir(vec3::ZERO)
		, m_vResultUpVec(vec3::AXISZ)
	{
	}

	~XMovableFactor()
	{
	}


	//////////////////////////////////////////////////////////////////////////
	// �ӵ�
	//
	struct SVelocity
	{
		SVelocity()
			: m_vAccel(vec3::ZERO)
			, m_vVelocity(vec3::ZERO)
			, m_bUsingAccel(true)
			, m_bUsingGravity(true)
		{
		}
		vec3					m_vVelocity;				// ������ �ӵ���
		vec3					m_vAccel;					// �������� ���Ӱ�
		bool					m_bUsingAccel;				// ���ӵ� ��뿩��
		bool					m_bUsingGravity;			// �߷� ��뿩��

	} VELOCITY;


	//////////////////////////////////////////////////////////////////////////
	// ����
	//
	struct SState
	{
		SState()
			: m_bFallingButNotJump(false)
			, m_bFallingButNotJumpReserved(false)
			, m_bFallingStartRecording(false)
			, m_nFallingButNotJumpStartTime(0)
			, m_fFallingStartHeight(0.0f)
			, m_fFallingEndHeight(0.0f)

			, m_bJumping(false)
			, m_bJumpAscending(false)
			, m_vJumpPosForCamera(vec3::ZERO)
			, m_fCalJumpHeight(0.0f)

			, m_bLanding(true)

			, m_bColTerrain(false)
			, m_bColObject(false)
			, m_bColTerrain_Front(false)

			, m_bSliding(false)

			, m_bSwimStart(false)
			, m_bFallingSwimStart(false)
			, m_bSwim(false)
			, m_bSwimEnd(false)

			, m_bGoingDirection(false)

			, m_bTalentActive(false)
			, m_nTalentActiveID(-1)

			, m_bStance(false)
			, m_bStun(false)

			, m_bHoldException(false)
			, m_fHoldExceptionTime(0.0f)
			, m_nHoldExceptionFrame(0)

			, m_bDie(false)
			
			, m_bGuardDefense(false)
		{
		}

		// falling
		bool					m_bFallingButNotJump;
		bool					m_bFallingButNotJumpReserved;	// �������¼��� falling�� �ƴ϶� ��� ���� �д�.
		bool					m_bFallingStartRecording;
		int						m_nFallingButNotJumpStartTime;
		float					m_fFallingStartHeight;
		float					m_fFallingEndHeight;

		// jump
		bool					m_bJumping;			//����
		bool					m_bJumpAscending;	//������ ������.
		vec3					m_vJumpPosForCamera;// ī�޶� ���� ���� ��ġ ���
		float					m_fCalJumpHeight;

		// ����
		bool					m_bLanding;

		// �浹
		bool					m_bColTerrain;		// ����
		bool					m_bColObject;		// ������Ʈ
		bool					m_bColTerrain_Front;// ���� ����
		vector<MUID>			m_vecTerrainColObejct;	// ���� �浹�� ���� ������Ʈ(npc)

		// �̲�����
		bool					m_bSliding;	

		// ����
		bool					m_bSwimStart;
		bool					m_bFallingSwimStart;
		bool					m_bSwim;
		bool					m_bSwimEnd;

		// ����(Ÿ�ٿ� ���� ���������̼��� �ƴ� Ű �Է¿� ���� ����)
		bool					m_bGoingDirection;

		// Ż��Ʈ �����
		bool					m_bTalentActive;
		int						m_nTalentActiveID;

		// ���Ľ�
		bool					m_bStance;

		// ����
		bool					m_bStun;

		// �������� ����ó��
		bool					m_bHoldException;
		float					m_fHoldExceptionTime;
		int						m_nHoldExceptionFrame;

		// ����
		bool					m_bDie;

		// ����
		bool					m_bGuardDefense;

	} GENERAL_STATE;


	//////////////////////////////////////////////////////////////////////////
	// �̵� ���� ����
	//
	struct SInterpolation
	{
		SInterpolation();
		~SInterpolation();

		bool					m_bWorking;				///< �۵�����

		vec3					m_vStartPos;			///< Position of start
		vec3					m_vStartDir;			///< Direction of start

		vec3					m_vTargetPos;			///< Position of target
		vec3					m_vTargetDir;			///< Direction of target

		vec3					m_vStartPosForDummyMove;///< Dummy�� ���� �̵��� �ʿ��� ���۰�
		vec3					m_vMovePos;				///< �̵��Ѱ�

		float					m_fMoveSpeed;			///< Speed of movement
		float					m_fRotateSpeed;			///< Speed of rotation
		unsigned int			m_nBoostFlags;			///< Flag of boost speed of movement and rotation
		float					m_fJumpSpeedRatio;		///< ���� ���ǵ� ����

		unsigned int			m_nMovementFlags;		///< Flags of movement

		bool					m_bRunMotion;			///< Run animation

		bool					m_bFinishMove;			///< Flag of finishing move
		bool					m_bFinishRotate;		///< Flag of finishing rotate
		bool					m_bFinishMoveMsg;		///< �̵� ���� �޼���

		float					m_fLimitTime;			///< Maximum time to complete
		float					m_fElipsedTime;			///< ElipsedTime;

		bool					m_bUpdated;				///< �� �����Ͱ� �Է� �� ���Ŀ� ������Ʈ�� �Ǿ����� ����
		bool					m_bArrived;				///< ���������̼��ؼ� ��ǥ ��ġ�� ����������
		bool					m_bForceInterpolation;	///< ������ ������� �ʴ� �⺻��å�� ������ �����ϰ� �� ���

		MOVE_CHECK_TYPE			m_eMoveCheckType;		///< �̵� �˻� ���


		void					Set( const vec3& vTargetPos, const vec3& vTargetDir, float fMoveSpeed, float fRotateSpeed, unsigned int nBoostFlags, unsigned int nMovementFlags, bool bRunMotion, float fLimitTime, bool bForce, MOVE_CHECK_TYPE eMoveType);
		void					SetRoate(const vec3& vTargetDir, float fRotateSpeed);

	} INTERPOLATION;


	//////////////////////////////////////////////////////////////////////////
	// �ڿ������� �������� ���� ���� ���̳��
	//
	struct SMovingDummy
	{
		SMovingDummy() :
			m_bHaveDummy( false),
			m_vDummyPosition( vec3::ZERO),
			m_vDummyPosElipsed( vec3::ZERO),
			m_vDummyDirection( -vec3::AXISY),
			m_vDummyDirElipsed( 0.0f)
			{
		}

		bool					m_bHaveDummy;					// ���� ��带 ������ �ִ��� ����

		vec3					m_vDummyPosition;				// ���̳�� ��ġ��
		vec3					m_vDummyPosElipsed;				// ���̳�� ��ġ ��ȭ��
		vec3					m_vDummyDirection;				// ���̳�� ���Ⱚ
		float					m_vDummyDirElipsed;				// ���̳�� ���� ��ȭ��

	} MOVING_DUMMY;


	//////////////////////////////////////////////////////////////////////////
	// KnockBack
	//
	struct SKnockBack
	{
		SKnockBack()
			: m_fKBEventDistance(0.f)
			, m_fKBFullPower(0.0f)
			, m_fKBPower(0.f)
			, m_fKBVel(0.f)
			, m_fKBUpVelocity(0.0f)
			, m_fKBFullTime(0.0f)
			, m_fKBElapsedTime(0.0f)
			, m_vKnockbackDir(vec3::ZERO)
			, m_vKnockBackDelta(vec3::ZERO)
			, m_bKnockback(false)
			, m_bKBSentEvent(false)
			, m_vKnockBackCalcPos(vec3::ZERO)
			, m_fUPDownTime(0.0f)
		{

		}


		float					m_fKBEventDistance;	// �˹� �̺�Ʈ�� ���;� �ϴ� ���� �Ÿ�
		float					m_fKBFullPower;
		float					m_fKBPower;
		float					m_fKBVel;

		float					m_fKBFullTime;
		float					m_fKBElapsedTime;

		float					m_fKBUpVelocity;
		float					m_fUPDownTime;

		vec3					m_vKnockbackDir;
		vec3					m_vKnockBackDelta;	// �˹� ��Ÿ
		vec3					m_vKnockBackCalcPos; // �˹� ���� ��ġ
		bool					m_bKnockback;
		
		bool					m_bKBSentEvent;

	} KNOCK_BACK;


	//////////////////////////////////////////////////////////////////////////
	// ���
	//
	struct SGrapple
	{
		SGrapple()
			: m_bGrappled( false)
			, m_uidGrappledAttacker( MUID::ZERO)
			, m_bGrappledAttach(false)
			, m_bGrappledBeThrown( false)
			, m_bGrappledStuck(false)
			, m_vGrappledThrownStartPos( vec3::ZERO)
			, m_vGrappledThrownStartDir( -vec3::AXISY)
			, m_vGrappledVelocity( vec3::ZERO)
			, m_vGrappledStartDir( vec3::AXISY )
			, m_vGrappledStartPos( vec3::ZERO )
			, m_vGrappledEndDir( vec3::AXISY )
			, m_vGrappledReadyPos( vec3::ZERO )
			, m_fGrappledElapsed( 0.0f)
			, m_fGrappledStartInterpolationTime(0.0f)
			, m_fGrappledRemainStartInterpolationTime(0.0f)
		{
		} 

		bool					m_bGrappled;				// ���� ���� ����
		MUID					m_uidGrappledAttacker;		// ���� ���� ��� ĳ���� UID
		bool					m_bGrappledAttach;			// ������ ����
		bool					m_bGrappledBeThrown;		// ������ ������ ���� ����
		bool					m_bGrappledStuck;			// ������ ���ư��� �ʰ� ������ ����̴�.
		vec3					m_vGrappledThrownStartPos;	// ���ư� �����ϴ� ��ġ
		vec3					m_vGrappledThrownStartDir;	// ���ư� �����ϴ� ����
		vec3					m_vGrappledVelocity;		// ���ư� �� �ӵ� ����
		vec3					m_vGrappledStartDir;		// ��� ������ ����
		vec3					m_vGrappledStartPos;		// ��� ������ ��ġ
		vec3					m_vGrappledEndDir;			// ��� ������ ����
		vec3					m_vGrappledReadyPos;		// ��� �غ� ��ġ
		float					m_fGrappledElapsed;			// ���ư��� �ð�
		float					m_fGrappledStartInterpolationTime; // ���۽� ����ó�� �� �ð�...
		float					m_fGrappledRemainStartInterpolationTime; // ���۽� ����ó�� �� �ð�...

	} GRAPPLE;

	//////////////////////////////////////////////////////////////////////////
	// ���콺 �̵�
	//
	struct SMouseMove
	{
		SMouseMove()
			: m_bWorking(false)
			, m_bColEnd(false)
			, m_fColFrontCheckTime(0.0f)
		{
		}

		float					m_fColFrontCheckTime;

		bool					m_bWorking;				///< �۵�����
		bool					m_bColEnd;				///< �浹�� ����

	} MOUSE_MOVE;

public:
	
	//////////////////////////////////////////////////////////////////////////
	//
	// �����Ӱ� ���õ� ���� ����(����) �Լ�
	//
	//////////////////////////////////////////////////////////////////////////

	bool					IsMoveMF() const;
	bool					IsJumping()	const		{ return GENERAL_STATE.m_bJumping; }
	bool					IsFalling()	const		{ return GENERAL_STATE.m_bFallingButNotJump; }
	bool					IsSliding()	const		{ return GENERAL_STATE.m_bSliding; }
	bool					IsGoingDirection() const{ return GENERAL_STATE.m_bGoingDirection; }

	bool					IsTalentActive() const	{ return GENERAL_STATE.m_bTalentActive; }
	bool					IsStance() const		{ return GENERAL_STATE.m_bStance; }
	bool					IsStun() const			{ return GENERAL_STATE.m_bStun; }

	bool					IsInterpolationIgnoringState() const;
	bool					IsInterpolationStopped()	const;
	bool					IsInterpolationRunning()	const;

	bool					IsMoveDummyNode() const	{ return MOVING_DUMMY.m_bHaveDummy; }

	bool					IsGrappled() const { return GRAPPLE.m_bGrappled;}

	bool					IsGuardDefense() const	{ return GENERAL_STATE.m_bGuardDefense; }
};


#endif // _XMOVABLE_FACTOR_H