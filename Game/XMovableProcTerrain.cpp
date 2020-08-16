#include "stdafx.h"
#include "XMovableProcTerrain.h"
#include "XModuleCollision.h"
#include "XModuleMovable.h"
#include "XGame.h"
#include "XMovableProcObjectChecker.h"

// ����׿�
//#include "RDebugPassRenderer.h"
//#include "RCameraSceneNode.h"

void XMovableProcTerrain::Init( XModuleMovable* pProcOwner )
{
	m_pMovableProcOwner = pProcOwner;
	m_pModuleCollision	= pProcOwner->m_pModuleCollision;
	
	m_fFallingStartReadyTime		= 0.0f;
}

bool XMovableProcTerrain::IsProcessable( const XMovableFactor& _rMovableFactor )
{
	if ( _rMovableFactor.GENERAL_STATE.m_bSwim == true ||
		(_rMovableFactor.GRAPPLE.m_bGrappled  &&  !_rMovableFactor.GRAPPLE.m_bGrappledBeThrown)  ||
		gg.game == NULL)
	{
		return false;
	}
	return true;
}

void XMovableProcTerrain::Run( XMovableFactor& _rMovableFactor, const vec3& _rVecCurrentPos, const vec3& _rVecCurrentDir, vec3& _rVecTo, vec3& _rVecDir, vec3& _rUpVec, float fDelta )
{
	// ������ �������� �浹 �˻�
	if ( CheckThrowColision( _rMovableFactor, _rVecCurrentPos, _rVecTo, _rVecDir, fDelta) == true)
		return;



	// �� �浹
	//
	vec3 vOut;
	vec3 vOutNor;
	vec3 now_pos = _rVecCurrentPos;
	// �浹üũ�� ���� �ٴڿ� �굵��, �ٴڿ������� ��������ŭ ���� �÷��ش�. -_-; 
	// ������ ��� ��Ȯ�� ������ �ʰ����� ���� �Ѿ. -_-

	vOut = now_pos;	

	bool bSliding = false;
	bool bFrontCol = false;
	vec3 target = now_pos + _rVecTo;	

	// ������ �ϴ°Ͱ� ���ο� �������� ���� �������� Ư���� ��Ȳ�� �ƴ϶��
	// �̵����� �ʴ°��̹Ƿ� �����۾��� �����ش�.
	if (now_pos.IsEqual(target) && _rMovableFactor.IsJumping() == false && _rMovableFactor.IsFalling() == false )
	{
		#ifdef _DEBUG_SHOW_TERRAIN_PROC
		mlog("���� - Ÿ�ٰ� ��ǥ�� ���Ƽ� MovableProc���� �ʰ� �׳� �ѱ�\n");
		#endif
		return;
	}

	_rMovableFactor.GENERAL_STATE.m_bColTerrain = false;
	_rMovableFactor.GENERAL_STATE.m_bSliding = false;
	_rMovableFactor.GENERAL_STATE.m_bColTerrain_Front = false;

	//�ʿ� ������
	if(m_pModuleCollision->GroundMoveTestMap(now_pos, target, vOut, vOutNor, _rMovableFactor.GENERAL_STATE.m_bJumping, &bSliding, &bFrontCol, &_rMovableFactor.GENERAL_STATE.m_vecTerrainColObejct))
	{
		_rMovableFactor.GENERAL_STATE.m_bSliding = bSliding;
		_rMovableFactor.GENERAL_STATE.m_bColTerrain_Front = bFrontCol;

		m_fFallingStartReadyTime = 0.0f;

		// ����ó�� ���¶��...
		if(_rMovableFactor.GENERAL_STATE.m_bHoldException)
		{
			_rMovableFactor.GENERAL_STATE.m_bSliding = false;
		}

		float fTerrainDot = vOutNor.DotProduct(vec3::AXISZ);

		if (bSliding && !_rMovableFactor.GENERAL_STATE.m_bFallingButNotJump && !_rMovableFactor.GENERAL_STATE.m_bJumping)
		{
			if (vOut.z > now_pos.z)	// �̲������µ� �� ���� ���ٸ�
			{									// �����µ��� �ö󰥷��� �Ѵٴ��� �׷� ���
				_rVecTo.Set(vec3::ZERO);

				_rMovableFactor.GENERAL_STATE.m_bColTerrain_Front = true;
				_rMovableFactor.GENERAL_STATE.m_bColTerrain = true;
				return;	// �׳� �̵�����Ѵ�. �ε巴�� �̲��������� �ٲٸ� ���� ���ڴ�. (��� ���� �������� �̻���)
			}

			if (fTerrainDot < FALLING_LIMIT &&						// ��簡 ������ Falling
				vOut.z != now_pos.z)
			{
				FallingSetting(_rMovableFactor, _rVecCurrentPos);
			}
		}

		// �̲������� �߿� �ٴڿ� ��Ҿ �������ų� �������̸� �׳� �������ų� �������� ���·� ����.
		// �������ٰ� ������ �ٴ��� ���� ���������� ������ falling/jumping state���� ������������ ������ �ִ�.
		// �ٵ� ������ �׷��� ������ �������״� �׳� �ϴ� ������.-_-;
		else 
		{
			if (!_rMovableFactor.GENERAL_STATE.m_bSliding ||
				(_rMovableFactor.GENERAL_STATE.m_bSliding && (now_pos.z == vOut.z)))
			{
				_rMovableFactor.GENERAL_STATE.m_bColTerrain = true;
				_rMovableFactor.VELOCITY.m_vVelocity.z = 0.0f;
			}
		}


		// �ѹ� �� ������Ʈ �浹 �˻縦 �Ѵ�. ���� �� �ֱ� �����̴�.
		// ���ƾ� �Ѵ�.
		// ��� ����� �ʰ� �ؼ� ���߰� �Ѵ�.
		// ������Ʈ �浹�� �����浹ó�� ���̴� �κ��� �ֱ� ������ ���� �κ��̴�.(��: ������ ȭ��)
		// ��� ĳ���Ͱ� Ư���� ��Ȳ�߿����� ������Ʈ �浹�� ����Ѵ�.
		XMovableProcObjectChecker objectChecker;
		bool bObjectCol = objectChecker.CheckObjectCollision(_rMovableFactor, m_pModuleCollision->GetOwner());

		if(bObjectCol && bFrontCol)
		{
			vector<MUID> vecEmpty;
			float fDist = 0.0f;
			vec3 vImpactPos;
			vec3 vImpactDir;
			vec3 tempVecTo = vOut - _rVecCurrentPos;
			if(m_pModuleCollision->ColObject(tempVecTo, _rVecCurrentPos, vImpactPos, vImpactDir, fDist, vecEmpty, false))
			{
				vImpactDir.z = 0.0f;
				vImpactDir.Normalize();

				vOut = _rVecCurrentPos + tempVecTo + (vImpactDir * fDist);
			}
		}

		//#ifdef _DEBUG
		//	RMatrix mworld;s
		//	mworld.MakeIdentity();
		//	mworld.SetTranslation(vOut);
		//	REngine::GetSceneManager().GetPassRenderer< RDebugPassRenderer >()->AddDebugBox(mworld, 5, 5, D3DCOLOR_XRGB(0, 0, 255));
		//#endif
	}
	else 	//�ʿ� ���� ������.
	{
		vec3 vCurrentMoveTo = vOut - _rVecCurrentPos;		// �ٴڿ� ���� �ʾƵ� ���鿡 �浹�� �ɼ� �����Ƿ� vOut�� �������� �����ϰ� �ٽ� �̵� ������ ����Ѵ�
		vec3 vXYColout;
		m_pModuleCollision->CheckXYTerrainCollision(_rVecCurrentPos, vCurrentMoveTo, vXYColout);
		float fGroundZ = vXYColout.z;


		// ���� �ٴ��� ���� ��� �������� �������� �ʰ� �Ѵ�.
		vec3 vNoGroundTestPos	= _rVecCurrentPos + vec3(vCurrentMoveTo.x, vCurrentMoveTo.y, m_pModuleCollision->GetRadius());
		vec3 vNoGroundTestTarget = vNoGroundTestPos;
		//vNoGroundTestPos		+= vec3(_rVecTo.x, _rVecTo.y, m_pModuleCollision->GetRadius());
		vNoGroundTestTarget.z	-= 50000.0f;
		vec3 vNoGroundTestOut, vNoGroundTestNor;
		if(m_pModuleCollision->PickRayToTerrain(vNoGroundTestPos, vNoGroundTestTarget, vNoGroundTestOut, vNoGroundTestNor) == false)
		{
			vOut.z = fGroundZ;

			_rMovableFactor.GENERAL_STATE.m_bColTerrain = true;
			_rMovableFactor.VELOCITY.m_vVelocity.z = 0.0f;
		}
		else
		{
			// Falling üũ
			float fTestDot = vNoGroundTestNor.DotProduct(vec3::AXISZ);

			if(fGroundZ < vOut.z - 200.f ||			// 2����.
				(fTestDot < FALLING_LIMIT && 
				_rMovableFactor.GENERAL_STATE.m_bFallingStartRecording == false && 
				CheckAllowedHeight(_rVecCurrentPos, _rVecTo) == false))
			{
				if(CheckFallingStart(fDelta))
					FallingSetting(_rMovableFactor, _rVecCurrentPos);
			}
		}
	}

	// ���� �浹 �˻��Ѵ�.
	if(CheckTerrainWater(vOut))
	{
		_rMovableFactor.GENERAL_STATE.m_bSwimStart = true;
	}

	_rVecTo = vOut - _rVecCurrentPos;
}


bool XMovableProcTerrain::CheckThrowColision( XMovableFactor& _rMovableFactor, const vec3& _rVecCurrentPos, vec3& _rVecTo, vec3& _rVecDir, float fDelta)
{
	XMovableFactor::SGrapple& GRAPPLE = _rMovableFactor.GRAPPLE;

	// ������ ���󰡴� ���¿��� �浹
	if ( GRAPPLE.m_bGrappled == false)	return false;
	if ( GRAPPLE.m_bGrappledBeThrown == false)  return true;
	if ( GRAPPLE.m_bGrappledStuck == true) return false;


	//  �ӵ��� ������ ����
	if ( GRAPPLE.m_vGrappledVelocity == 0.0f)
		return false;

	_rMovableFactor.GENERAL_STATE.m_bColTerrain = false;

	vec3 vPosCurr = _rVecCurrentPos;
	vPosCurr.z = max( vPosCurr.z,  gg.currentgamestate->GetWorld()->GetGroundZ( vPosCurr) + 5.0f);
	vec3 vMoveTo = _rVecTo;

	while ( 1)
	{
		// ���� ����
		vec3 vPosSt = vPosCurr;
		vec3 vPosEd = vPosSt + vMoveTo;


		// �浹���� ���� ���� ���ϱ�
		vec3 vPosCol, vFaceNormal;
		if ( m_pModuleCollision->PickRayToTerrain( vPosSt, vPosEd, vPosCol, vFaceNormal) == false)
		{
			// ���� �浹�ߴ��� ���θ� ����
			if ( CheckTerrainWater( vPosEd) == true)
			{
				_rMovableFactor.GENERAL_STATE.m_bSwimStart = true;
				return true;
			}

			if(_rMovableFactor.GENERAL_STATE.m_bColTerrain == false &&
				_rMovableFactor.GENERAL_STATE.m_bFallingStartRecording == false)
			{
				FallingSetting(_rMovableFactor, _rVecCurrentPos);
			}
			// ���� ������ �浹 �˻縦 �ٽ� �ǽ��ؼ� �浹 ��ġ�� �ٽ� ã����...

			break;
		}

//#ifdef _DEBUG
//		RMatrix mat;
//		mat.MakeIdentity();
//		mat.SetTranslation( vPosCol);
//		REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugSphere( mat, 5, 120, D3DCOLOR_XRGB(255, 0, 0));
//#endif


		// �浹 �� �ݻ�Ǵ� �ӵ� ����
		vec3 vReflected( 0.0f, 0.0f, 0.0f);
		vec3 vReflectedVel( 0.0f, 0.0f, 0.0f);
		float fTimeCol = ( ( vPosCol - vPosSt).Length() / vMoveTo.Length()) * fDelta;


		// �浹 �� �ӵ��� ����
		float fElapsed = GRAPPLE.m_fGrappledElapsed - fDelta + fTimeCol;
		vec3 vVelocity = GRAPPLE.m_vGrappledVelocity;
		vVelocity.z += 0.5f * GRAVITY * fElapsed * fElapsed;
		float fVelocity = vVelocity.Length();


		// �浹 ������ �ӵ��� ������� �˻�
		if ( fVelocity > 800.0f)
		{
			// �ݻ�Ǵ� ���� ���ϱ� = ( 2 * (-D dot N) ) N + D
			vReflected = ( 2.0f * DotProduct( -vMoveTo, vFaceNormal) ) * vFaceNormal + vMoveTo;
			vReflected = Normalize( vReflected);


			// �浹 �� �ӵ� ���ϱ�
			vReflectedVel = vReflected * ( fVelocity * 0.4f);
			vReflectedVel += -vFaceNormal * (vReflectedVel * 0.7f);
		}


		// �ӵ� �缳��
		GRAPPLE.m_fGrappledElapsed = fDelta - fTimeCol;
		GRAPPLE.m_vGrappledVelocity = vReflectedVel;
		GRAPPLE.m_vGrappledThrownStartPos = vPosCol;

		// �浹 �÷��� ����
		_rMovableFactor.GENERAL_STATE.m_bColTerrain = true;


		//  �ӵ��� ������ ����
		if ( GRAPPLE.m_vGrappledVelocity == 0.0f)
		{
			vMoveTo = vPosCol - vPosCurr;
			break;
		}

		// �浹 �� ��ġ �缳��
		vPosCurr = vPosCol;

		vec3 vNextPos = GRAPPLE.m_vGrappledThrownStartPos + GRAPPLE.m_vGrappledVelocity * GRAPPLE.m_fGrappledElapsed;
		vNextPos.z += 0.5f * GRAVITY * GRAPPLE.m_fGrappledElapsed * GRAPPLE.m_fGrappledElapsed;
		vNextPos.z = max( vNextPos.z ,  gg.currentgamestate->GetWorld()->GetGroundZ( vNextPos) + 5.0f);
		vMoveTo = vNextPos - vPosCurr;

		fDelta -= fTimeCol;


		// �浹 �� ��ġ �缳��
		GRAPPLE.m_vGrappledThrownStartDir = Normalize( vNextPos - vPosCol);
	}


	// �̵��� �Ÿ� ����
	_rVecTo = (vPosCurr + vMoveTo) - _rVecCurrentPos;

	return true;
}

bool XMovableProcTerrain::CheckTerrainWater( vec3& vPos )
{
	//------------------------------------------------------------------------
	// �������� ������ �Ǵ�
	vec3 vOutTerrain = vPos;
	vec3 vOut = vPos;

	m_pModuleCollision->CheckXYTerrainCollision(vPos, vec3(0, 0, 0), vOutTerrain);
	bool bColWater = m_pModuleCollision->PickTerrainWater(vPos, vOut);

	if(bColWater == true)
	{
		// õ���� �����ϴ°�?
		vec3 vUpColTestOut;
		vec3 vUpColTestNor;
		vOutTerrain.z += 1.0f;
		if(m_pModuleCollision->PickRayToTerrain(vOutTerrain, vOut, vUpColTestOut, vUpColTestNor) &&
			vUpColTestOut.z < vOut.z)
		{
			// �����ϸ� �� ���� ��ġ�� ���� �ƴϴ�.
			return false;
		}

		// ������ �������� �����ص� �Ǵ��� ������ ����.
		//if(GetMyPlayerOwner()->IsSwim() == true &&		// ���� �������̸�
		//	m_bSwimOutPossible == false)				// XModuleMovable���� �������� ������ ���� ��Ȳ�̸�...
		//{
		//	return true;								// �������̶�� �Ǵ�.
		//}

		// �� �浹
		if(vOutTerrain.z + SWIMMING_HEIGHT_VALUE < vOut.z &&
			(vOut.z - SWIMMING_HEIGHT_VALUE) >= vPos.z)
		{
			vPos.z = vOut.z - SWIMMING_HEIGHT_VALUE;
			return true;
		}
	}

	return false;
}

void XMovableProcTerrain::FallingSetting( XMovableFactor& _rMovableFactor, const vec3& _rVecCurrentPos )
{
	// ������ �ƴϰ� ������ �ƴϸ�
	// �����ڼ��� ���� �Ӽ� true��.
	if (!_rMovableFactor.GENERAL_STATE.m_bJumping && !_rMovableFactor.GENERAL_STATE.m_bFallingButNotJump)
	{
		if (!_rMovableFactor.GENERAL_STATE.m_bFallingButNotJumpReserved)
		{
			if(_rMovableFactor.GENERAL_STATE.m_bFallingStartRecording == false)
			{
				_rMovableFactor.GENERAL_STATE.m_fFallingStartHeight = _rVecCurrentPos.z;
				_rMovableFactor.GENERAL_STATE.m_bFallingStartRecording = true;
			}
			_rMovableFactor.GENERAL_STATE.m_bFallingButNotJumpReserved = true;
			_rMovableFactor.GENERAL_STATE.m_nFallingButNotJumpStartTime = XGetNowTime();
			// �ٷ� �������� ����� �ƴ϶� ��¦ �����̸� �ֱ� ���� reserved ��¼�� ������ �����. -_-;
		}
	}
}

bool XMovableProcTerrain::CheckAllowedHeight( const vec3& vEntityPosition, const vec3 &vec )
{
	vec3 vMoveDir = vec;
	XMath::NormalizeZ0(vMoveDir);
	float fRadius = m_pModuleCollision->GetRadius();

	vec3 vStart = vEntityPosition + vec3(0, 0, fRadius) + (vMoveDir * fRadius);
	vec3 vEnd  = vStart + vec3(0, 0, -50000.0f);

	vec3 vTestOut = vStart;
	vec3 vTestNor = vec3::AXISZ;

	if(m_pModuleCollision->PickRayToTerrain(vStart, vEnd, vTestOut, vTestNor))
	{
		float fHeight = vEntityPosition.z - vTestOut.z;
		if(fHeight < MOVE_ALLOWED_HEIGHT)
			return true;
	}

	return false;
}

bool XMovableProcTerrain::CheckFallingStart( float fDelta )
{
	// �����ð� �ȿ��� Falling ó���� ���� �ʴ´�.
	m_fFallingStartReadyTime += fDelta;

	if(m_fFallingStartReadyTime >= XCONST::COLLISION_TERRAIN_FALLING_START_TIME)
	{
		return true;
	}

	return false;
}
