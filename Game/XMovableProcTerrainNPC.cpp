#include "stdafx.h"
#include "XMovableProcTerrainNPC.h"
#include "XModuleCollision.h"
#include "XModuleMovable.h"

void XMovableProcTerrainNPC::Init( XModuleMovable* pProcOwner )
{
	m_pMovableProcOwner	= pProcOwner;
	m_pModuleCollision	= pProcOwner->m_pModuleCollision;

	_ASSERT(m_pModuleCollision);
	_ASSERT(m_pModuleCollision->GetOwner()->GetEntityType() == ETID_NPC || m_pModuleCollision->GetOwner()->GetEntityType() == ETID_CUTSCENE);
}

bool XMovableProcTerrainNPC::IsProcessable( const XMovableFactor& _rMovableFactor )
{
	if (_rMovableFactor.GRAPPLE.m_bGrappled)
		return false;

	return true;
}

void XMovableProcTerrainNPC::Run( XMovableFactor& _rMovableFactor, const vec3& _rVecCurrentPos, const vec3& _rVecCurrentDir, vec3& _rVecTo, vec3& _rVecDir, vec3& _rUpVec, float fDelta )
{
	_ASSERT(gg.game);

	// �� �浹
	//
	vec3 vOut				= _rVecCurrentPos;
	vec3 vOutNor;
	const float fCollRadius = m_pModuleCollision->GetRadius();
	vec3 vTo				= _rVecTo;
	vec3 now_pos			= _rVecCurrentPos;

	if(_rMovableFactor.INTERPOLATION.m_bWorking &&
		_rMovableFactor.INTERPOLATION.m_eMoveCheckType == MCT2_TARGET_HEGITH)
		now_pos.z				= _rMovableFactor.INTERPOLATION.m_vTargetPos.z;

	now_pos.z				+= NPC_PICKING_MAP_HEIGHT;			// �������� radius ��ŭ �÷��� ������... NPC���� �Ѿ �� �ִ� ���� ��ŭ�ؼ� �˻�����.

	// �˹�ô� ������ ���� �浹 �˻縦 �Ѵ�.
	if(_rMovableFactor.KNOCK_BACK.m_bKnockback)
	{
		now_pos.z = _rVecCurrentPos.z + fCollRadius;			// �˹��� ������ ���߱� ���ؼ� z���� radius ��ŭ�Ѵ�.
		//----------------------------------------------------------------------------
		// �˹� �浹 �˻�
		vTo.z = 0.0f;
		vec3 vNorVecTo		= vTo;
		vNorVecTo.Normalize();
		vec3 start_pos		= now_pos - vNorVecTo;
		vec3 target			= now_pos + vTo + (vNorVecTo * fCollRadius);
		float fDistance		= vTo.Length();

		if(m_pModuleCollision->PickRayToTerrain(start_pos, target, vOut, vOutNor))
		{
			float fPickDist = vOut.DistanceTo(start_pos);
/*			if (fPickDist < fCollRadius)
			{
				fDistance = fPickDist - fCollRadius;
			}
			else */if (fPickDist < fDistance + fCollRadius)
			{
				fDistance = fPickDist - fCollRadius;
			}
		}

		vOut	= _rVecCurrentPos + vNorVecTo * fDistance;
		vTo		= vOut - _rVecCurrentPos;
	}

	// �������� ���� Direct�� ������ ���� �ұ����� Picking�� �Ѵ�. �߷°��� ���ԵǾ����� �ʴ�.
	if(_rMovableFactor.INTERPOLATION.m_eMoveCheckType == MCT2_DIRECT)
	{
		// Direct�� �Դٴ°� Interpolation���� �̵����� Direct�� ���߾��������Ƿ� �浹 vEnd���� �ű⿡ ���߾�� �Ѵ�.
		vec3 vEnd = _rVecCurrentPos + vTo + vec3(0.0f, 0.0f, -1.0f);
		if(m_pModuleCollision->PickRayToTerrain(now_pos, vEnd, vOut, vOutNor) == false)
			return;
	}
	else if (_rMovableFactor.VELOCITY.m_bUsingGravity)
	{
		// NPC�� �̵��� XY �浹üũ�� ���� �ʴ´�.
		// ������ fCollRadius��ŭ �÷��� ���� ����� �ؾ� ����� �� ���� ���� ���´�. 
		// �׸��� CheckXYTerrainCollision()���� ���� ���� �ٷ� ���� �ϹǷ� fCollRadius��ū �ٽ� �� �ʿ䰡 ����.
		m_pModuleCollision->CheckXYTerrainCollision(now_pos, vTo, vOut, 0.0f);

		//if (REngine::m_strDebugFlags == "a")
		{
			CheckAbnormalObstacle(vOut, vTo, _rVecCurrentPos);
		}
	}

	UpdateMoveCheckType(_rMovableFactor, _rVecCurrentPos);

	_rVecTo = vOut - _rVecCurrentPos;
}

bool XMovableProcTerrainNPC::CheckAbnormalObstacle( vec3& _out_nextPos, const vec3& _nextTo, const vec3& _currentPos )
{
	bool bApplied = false;

	float fGuardHeight = 20.f;
	float fGuardNext = 20.f;

	//static std::list< float > distList;

	float fMovedZ = _out_nextPos.z - _currentPos.z;
	if ( abs(fMovedZ) > fGuardHeight)
	{
		vec3 vNextNextTmp = _out_nextPos - _currentPos;
		vNextNextTmp.z = 0;
		vNextNextTmp = vNextNextTmp.Normalize() * fGuardNext;

		vec3 vNextNextPoint = _out_nextPos + vNextNextTmp;
		vNextNextPoint.z += NPC_PICKING_MAP_HEIGHT;

		vec3 vNextNextTo = _nextTo + vNextNextTmp;

		m_pModuleCollision->CheckXYTerrainCollision(vNextNextPoint, vNextNextTo, vNextNextTmp, 0.0f);
		float fPredicated = vNextNextTmp.z - _out_nextPos.z;
		if ( abs(fPredicated) > fGuardHeight  && (fMovedZ*fPredicated) < 0 )
		{
			_out_nextPos.z = _currentPos.z;
			bApplied = true;
			//static int a = 1;
			//mlog("ȿȿ %d\n", ++a);
		}
	}

	//distList.push_front(fMovedZ);
	//while (distList.size() > 30)
	//{
	//	distList.pop_back();
	//}

	return bApplied;
}

void XMovableProcTerrainNPC::UpdateMoveCheckType( XMovableFactor& _rMovableFactor, const vec3& _rVecCurrentPos )
{
	vec3 vMoveDir = (_rMovableFactor.INTERPOLATION.m_vTargetPos - _rVecCurrentPos).Normalize();
	vec3 vMoveDirZ0 = vMoveDir;
	XMath::NormalizeZ0(vMoveDirZ0);

	float fHeightGap = _rMovableFactor.INTERPOLATION.m_vTargetPos.z - _rVecCurrentPos.z;
	float fDot = vMoveDir.DotProduct(vMoveDirZ0);
	float fMoveLen = (_rMovableFactor.INTERPOLATION.m_vTargetPos - _rVecCurrentPos).Length();	
	
	if(fDot < SLIDING_LIMIT && fMoveLen <= MOVE_LEVITATION_LEN)		// 10 ���� �̳����� ���ߺξ��Ѵ�.
	{
		// ��簡 ����.
		if(fHeightGap >= MOVE_DIRECT_CHECK_HEIGHT)
			_rMovableFactor.INTERPOLATION.m_eMoveCheckType = MCT2_DIRECT;
		else if(fHeightGap > MOVE_TARGET_HEIGHT_CHECK_HEIGHT)
			_rMovableFactor.INTERPOLATION.m_eMoveCheckType = MCT2_TARGET_HEGITH;
	}
	else if(_rMovableFactor.INTERPOLATION.m_eMoveCheckType != MCT2_MOUSE_MOVE)
	{
		if(fHeightGap < MOVE_DIRECT_CHECK_HEIGHT && fHeightGap > MOVE_TARGET_HEIGHT_CHECK_HEIGHT)
			_rMovableFactor.INTERPOLATION.m_eMoveCheckType = MCT2_TARGET_HEGITH;
		else 
			_rMovableFactor.INTERPOLATION.m_eMoveCheckType = MCT2_NONE;
	}
}
