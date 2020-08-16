#include "stdafx.h"
#include "XMovableProcInterpolation.h"
#include "XModuleMovable.h"

#define BOOST_SPEED_MOVE		800.0f
#define BOOST_SPEED_ROTATE		300.0f



XMovableProcInterpolation::XMovableProcInterpolation()
{
}


XMovableProcInterpolation::~XMovableProcInterpolation()
{
}


void XMovableProcInterpolation::Init( XModuleMovable* pProcOwner )
{
	m_pModuleMovable = pProcOwner;
}


bool XMovableProcInterpolation::IsProcessable( const XMovableFactor& _rMovableFactor)
{
	// ������� �ܺο��� ����ó�� ���� ���� ���� ���� ���⼭ ����.
	if (_rMovableFactor.GRAPPLE.m_bGrappled == true)
	{
		return false;
	}

	return _rMovableFactor.INTERPOLATION.m_bWorking;
}


bool _CalcPos( vec3* ret_pos, const vec3& cur_pos, const vec3& tar_pos, float fSpeed, float fDelta)
{
	// 2008-10-27 billcap ����
	vec3 diff = tar_pos - cur_pos;
	vec3 dir = diff;
	XMath::NormalizeZ0( dir);

	if ( fSpeed < 1.0f)			// fSpeed = 0
	{
		*ret_pos = diff;
		return true;
	}


	double distance = (double)( diff.x * diff.x + diff.y * diff.y);
	vec3 vToler = dir * ( fSpeed * fDelta);

	if ( (vToler.x * vToler.x + vToler.y * vToler.y) >= distance)
	{
		*ret_pos = diff;
		return true;
	}


	ret_pos->x = vToler.x;
	ret_pos->y = vToler.y;
	ret_pos->z = vToler.z;


	return false;
}

bool _CalcPos_Direct( vec3* ret_pos, const vec3& cur_pos, const vec3& tar_pos, float fSpeed, float fDelta)
{
	// 2008-10-27 billcap ����
	vec3 diff = tar_pos - cur_pos;
	vec3 dir = diff;
	dir.Normalize();

	if ( fSpeed < 1.0f)			// fSpeed = 0
	{
		*ret_pos = diff;
		return true;
	}


	double distance = (double)( diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
	vec3 vToler = dir * ( fSpeed * fDelta);

	if ( (vToler.x * vToler.x + vToler.y * vToler.y+ vToler.z * vToler.z) >= distance)
	{
		*ret_pos = diff;
		return true;
	}


	ret_pos->x = vToler.x;
	ret_pos->y = vToler.y;
	ret_pos->z = vToler.z;


	return false;
}

bool _CalcDir( vec3* ret_dir, const vec3& cur_dir, const vec3& tar_dir, float fSpeed, float fDelta)
{
	if ( fSpeed < 1.0f)		// fSpeed = 0
	{
		ret_dir->x = tar_dir.x;
		ret_dir->y = tar_dir.y;
		XMath::NormalizeZ0( *ret_dir);

		return true;
	}


	float fAngleSpeed = XMath::DegreeToRadian( fSpeed);
	vec3 vCross = cur_dir.CrossProduct( vec3::AXISZ);
	float fDot = vCross.DotProduct( tar_dir);

	if ( fDot > 0.0f)
		fAngleSpeed = -fAngleSpeed;


	float fRotateRadian = fAngleSpeed * fDelta;
	float fCurToTarRadian = cur_dir.AngleToXY( tar_dir);

	if ( abs( fRotateRadian) > abs( fCurToTarRadian))
	{
		ret_dir->x = tar_dir.x;
		ret_dir->y = tar_dir.y;
		XMath::NormalizeZ0( *ret_dir);

		return true;
	}

	ret_dir->x = cur_dir.x * cos( fRotateRadian) - cur_dir.y * sin( fRotateRadian);
	ret_dir->y = cur_dir.y * cos( fRotateRadian) + cur_dir.x * sin( fRotateRadian);
	XMath::NormalizeZ0( *ret_dir);

	return false;
}


bool _CalcRotate( float& ret_rot, const vec3& cur_dir,  const vec3& tar_dir, float fSpeed, float fDelta)
{
	if ( fSpeed < 1.0f)		// fSpeed = 0
	{
		ret_rot = cur_dir.AngleToXY( tar_dir);
		return true;
	}

	float fCurToTarRadian = cur_dir.AngleToXY( tar_dir);
	float fAngleSpeed = XMath::DegreeToRadian( fSpeed);
	if ( fCurToTarRadian < 0.0f)
		fAngleSpeed = -fAngleSpeed;
	float fRotateRadian = fAngleSpeed * fDelta;

	if ( abs( fRotateRadian) > abs( fCurToTarRadian))
	{
		ret_rot = fCurToTarRadian;
		return true;
	}

	ret_rot = fRotateRadian;
	return false;
}


vec3 _Rotate( float fRot, const vec3& vec)
{
	vec3 ret;

	ret.x = vec.x * cos( fRot) - vec.y * sin( fRot);
	ret.y = vec.y * cos( fRot) + vec.x * sin( fRot);
	XMath::NormalizeZ0( ret);

	return ret;
}


void XMovableProcInterpolation::Run( XMovableFactor& _rMovableFactor, const vec3& _rVecCurrentPos, const vec3& _rVecCurrentDir, vec3& _rVecTo, vec3& _rVecDir, vec3& _rUpVec, float fDelta )
{
	PFC_THISFUNC;

	XMovableFactor::SInterpolation& INTERPOL = _rMovableFactor.INTERPOLATION;
	XMovableFactor::SMovingDummy& MOVING_DUMMY = _rMovableFactor.MOVING_DUMMY;


	// ����ð� ������Ʈ
	if ( INTERPOL.m_fLimitTime > NO_LIMIT_TIME)
		INTERPOL.m_fElipsedTime += fDelta;


	// ������Ʈ
	INTERPOL.m_bUpdated = true;


	// TODO : ���� / �������� ��üó������ �ʰ� �ùķ��̼� �����ش�. ���߿� ������ ���õ� �ڵ� �ٽú���! ( ���� �� �κ��� ������ �ֵ���.. )
	if (_rMovableFactor.IsJumping() || _rMovableFactor.IsFalling())
	{
		INTERPOL.m_vStartPos.z = INTERPOL.m_vTargetPos.z = _rVecCurrentPos.z + ((_rVecTo.z < 0.0f) ? _rVecTo.z : 0.0f)/*�߷¸� ����޵���*/;
		INTERPOL.m_vStartDir = INTERPOL.m_vTargetDir = _rVecDir = _rVecCurrentDir;
		INTERPOL.m_bFinishMove = false;
		INTERPOL.m_bFinishRotate = false;

		if(_rMovableFactor.IsJumping() && _rMovableFactor.GENERAL_STATE.m_bColObject)
		{
			INTERPOL.m_bWorking = false;
			INTERPOL.m_bArrived = true;
			return;
		}
	}

	// ���� ��Ȳ�� ��쿣 �׳� ����
	else if ( INTERPOL.m_bWorking == false  ||  _rMovableFactor.IsInterpolationIgnoringState())
	{
		INTERPOL.m_vStartPos = INTERPOL.m_vTargetPos = _rVecCurrentPos;
		INTERPOL.m_vStartDir = INTERPOL.m_vTargetDir = _rVecDir = _rVecCurrentDir;
		INTERPOL.m_bFinishMove = false;
		INTERPOL.m_bFinishRotate = false;
		INTERPOL.m_fLimitTime = NO_LIMIT_TIME;

		_rMovableFactor.VELOCITY.m_vVelocity.x = 0.0f;
		_rMovableFactor.VELOCITY.m_vVelocity.y = 0.0f;

		return;
	}


	// ���� �̵�/ȸ�� ������ ����
	vec3 _cpos = _rVecCurrentPos;
	vec3 _cdir = _rVecCurrentDir;
	XMath::NormalizeZ0( _cdir);


	// Ÿ�� �̵�/ȸ�� ������ ����
	vec3 _tpos = INTERPOL.m_vTargetPos;
	vec3 _tdir = INTERPOL.m_vTargetDir;
	XMath::NormalizeZ0( _tdir);


	// �̵�/ȸ�� �ӵ� ����
	float _move_speed	= ( INTERPOL.m_nBoostFlags & BOOST_MOVE)	?  BOOST_SPEED_MOVE : INTERPOL.m_fMoveSpeed;
	float _rotate_speed	= ( INTERPOL.m_nBoostFlags & BOOST_ROTATE)	?  BOOST_SPEED_ROTATE : INTERPOL.m_fRotateSpeed;

	// ���� �̵� ó��
	if ( MOVING_DUMMY.m_bHaveDummy == true)
	{
		// ���̰� �̵��� ��ŭ ������Ʈ ��ġ�� �̵������ش�
		_rVecTo += MOVING_DUMMY.m_vDummyPosElipsed;

		//if(m_pModuleMovable->GetOwner()->GetType() == XOBJ_NET_PLAYER)
		//	mlog("< ���� �̵� > %f %f\n", MOVING_DUMMY.m_vDummyPosElipsed.x, MOVING_DUMMY.m_vDummyPosElipsed.y);
		if(INTERPOL.m_bArrived == false)
		{
		//	//// ���̰� �̵��� ��ŭ Ÿ�ٵ� �̵������ش�
			//INTERPOL.m_vTargetPos += MOVING_DUMMY.m_vDummyPosElipsed;
			//_tpos = INTERPOL.m_vTargetPos;
			_cpos = INTERPOL.m_vStartPosForDummyMove + INTERPOL.m_vMovePos;
			INTERPOL.m_fLimitTime = NO_LIMIT_TIME;

			//if(m_pModuleMovable->GetOwner()->GetType() == XOBJ_NET_PLAYER)
			//{
			//	mlog("���� ���� %f %f   ���� �̵��� %f %f (��ǥ %f %f)\n", _cpos.x, _cpos.y, INTERPOL.m_vMovePos.x, INTERPOL.m_vMovePos.y, _tpos.x, _tpos.y);
			//}
		}
	}
	//else
	//{
	//	if(INTERPOL.m_bFinishMove == false && m_pModuleMovable->GetOwner()->GetType() == XOBJ_NET_PLAYER)
	//	{
	//		mlog("-- �׳� ���� ���� %f %f   (��ǥ %f %f)\n", _cpos.x, _cpos.y, _tpos.x, _tpos.y);
	//	}
	//}

	// �̵� ����
	if ( INTERPOL.m_bFinishMove == false)
	{
		vec3 _diff_pos;
		if(INTERPOL.m_eMoveCheckType == MCT2_DIRECT)
		{
			INTERPOL.m_bFinishMove = _CalcPos_Direct( &_diff_pos, _cpos, _tpos, _move_speed, fDelta);
			_rVecTo = _diff_pos;
		}
		else
		{
			INTERPOL.m_bFinishMove = _CalcPos( &_diff_pos, _cpos, _tpos, _move_speed, fDelta);

			if(INTERPOL.m_eMoveCheckType == MCT2_MOUSE_MOVE)
				_rVecTo += vec3(_diff_pos.x, _diff_pos.y, 0.0f);
			else
			{
				_rVecTo += _diff_pos;
				INTERPOL.m_vMovePos += _diff_pos;
			}
		}
	


		// ������ ������ ����
		if ( INTERPOL.m_bFinishMove == true)
		{
			// ���ѽð� ����
			INTERPOL.m_fLimitTime = NO_LIMIT_TIME;


			// ���� �Ϸ�
			INTERPOL.m_bArrived = true;

			INTERPOL.m_bFinishMoveMsg = true;

			INTERPOL.m_vMovePos = vec3::ZERO;
		}

	}


	// ȸ�� ����
	if ( INTERPOL.m_bFinishRotate == false)
	{
		INTERPOL.m_bFinishRotate = _CalcDir( &_rVecDir, _cdir, _tdir, _rotate_speed, fDelta);
	}

	// ���� ȸ�� ó��
	if ( MOVING_DUMMY.m_bHaveDummy == true)
	{
		// ���̰� ȸ���� ��ŭ ������Ʈ ���⵵ ȸ�������ش�
		_rVecDir = _Rotate( MOVING_DUMMY.m_vDummyDirElipsed, _rVecDir);


		// ���̰� ȸ���� ��ŭ Ÿ�ٵ� ȸ�������ش�
		INTERPOL.m_vTargetDir = _Rotate( MOVING_DUMMY.m_vDummyDirElipsed, INTERPOL.m_vTargetDir);
	}



	// �ӵ� ����
	if ( INTERPOL.m_bFinishMove == true)
	{
		_rMovableFactor.VELOCITY.m_vVelocity.x = 0.0f;
		_rMovableFactor.VELOCITY.m_vVelocity.y = 0.0f;
	}
}
