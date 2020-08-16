#include "stdafx.h"
#include "XMovableProcWater.h"
#include "XModuleCollision.h"
#include "XModuleMovable.h"

#include "RDebugPassRenderer.h"
#include "RCameraSceneNode.h"
void XMovableProcWater::Init( XModuleMovable* pProcOwner )
{
	m_pMovableProcOwner	= pProcOwner;
	m_pModuleCollision	= pProcOwner->m_pModuleCollision;

	_ASSERT(m_pModuleCollision);
}

bool XMovableProcWater::IsProcessable( const XMovableFactor& _rMovableFactor )
{
	if (_rMovableFactor.GENERAL_STATE.m_bSwim == false ||
		_rMovableFactor.GRAPPLE.m_bGrappled == true ||
		m_pModuleCollision->GetOwner()->GetEntityType() == ETID_NPC )
	{
		return false;
	}

	return true;
}

void XMovableProcWater::Run( XMovableFactor& _rMovableFactor, const vec3& _rVecCurrentPos, const vec3& _rVecCurrentDir, vec3& _rVecTo, vec3& _rVecDir, vec3& _rUpVec, float fDelta )
{
	vec3 vWaterPos;
	vec3 now_pos		= _rVecCurrentPos;
	vec3 target			= now_pos + _rVecTo;
	vec3 vOut			= now_pos;
	float fWaterLimit	= SWIMMING_HEIGHT_VALUE;

	if(m_pModuleCollision->PickTerrainWater(target, vWaterPos))
	{
		// ���� �ִٸ�...
		// �����浹 üũ
		vec3 vNor;
		target			= now_pos + vec3(_rVecTo.x, _rVecTo.y, 0.0f);

		//�ʿ� ������
		if(m_pModuleCollision->WaterMoveTestMap(now_pos, target, fWaterLimit, vOut, vNor, &_rMovableFactor.GENERAL_STATE.m_vecTerrainColObejct))
		{
			float fDot = vNor.DotProduct(RVector::AXISZ);

			if(vWaterPos.z - vOut.z <= fWaterLimit)
			{
				vOut.z = vWaterPos.z - fWaterLimit;

				if(fDot >= 0.76f)
				{
					// �������� ������ �ִ� ������ �ȴ�.
					_rMovableFactor.GENERAL_STATE.m_bSwimEnd = true;
				}

				_rVecTo = vOut - _rVecCurrentPos;
				return;
			}

			vWaterPos.x = vOut.x;
			vWaterPos.y = vOut.y;
		}
		else
		{
			// �ʿ� ���� ����ó�� - ���� �ؿ� �����ȴٸ�...
			vec3 vOutTerrain;
			m_pModuleCollision->CheckXYTerrainCollision(target, vec3(0, 0, 0), vOutTerrain);
			if(vOutTerrain.z > vWaterPos.z)
			{
				_rMovableFactor.GENERAL_STATE.m_bSwimEnd = true;
				_rVecTo = vOutTerrain - _rVecCurrentPos;
				return;
			}
		}

		// ���⼭ ��ǥ ���̰��� ���� ���̴�. 
		// ������ �ؿ� �����Ƿ� ��ǥ�� ���� ������.
		vWaterPos.z	-= fWaterLimit;
		vOut	= vWaterPos;
		_rVecTo = vOut - _rVecCurrentPos;
		_rMovableFactor.VELOCITY.m_vVelocity.z = 0.0f;
	}
	else
	{
		_rMovableFactor.GENERAL_STATE.m_bSwimEnd = true;
	}
}