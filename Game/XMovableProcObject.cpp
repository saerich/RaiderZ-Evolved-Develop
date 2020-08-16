#include "stdafx.h"
#include "XMovableProcObject.h"
#include "XModuleCollision.h"
#include "XModuleMovable.h"
#include "XModuleTalent.h"
#include "XMovableProcObjectChecker.h"

void XMovableProcObject::Init( XModuleMovable* pProcOwner )
{
	m_pMovableProcOwner	= pProcOwner;
	m_pModuleCollision	= pProcOwner->m_pModuleCollision;

}

bool XMovableProcObject::IsProcessable( const XMovableFactor& _rMovableFactor )
{
	if (_rMovableFactor.GRAPPLE.m_bGrappled)
		return false;

	return true;
}

void XMovableProcObject::Run( XMovableFactor& _rMovableFactor, const vec3& _rVecCurrentPos, const vec3& _rVecCurrentDir, vec3& _rVecTo, vec3& _rVecDir, vec3& _rUpVec, float fDelta )
{
	_ASSERT(m_pModuleCollision);

	//----------------------------------------------------------------------//
	// Factor�� �ʱ�ȭ
	_rMovableFactor.GENERAL_STATE.m_bColObject = false;

	//----------------------------------------------------------------------//
	// ��� ĳ���Ͱ� Ư���� ��Ȳ�߿����� ������Ʈ �浹�� ����Ѵ�.
	XMovableProcObjectChecker objectChecker;
	bool bObjectCol = objectChecker.CheckObjectCollision(_rMovableFactor, m_pModuleCollision->GetOwner());

	if(bObjectCol == false)
		return;

	//----------------------------------------------------------------------//
	// ������Ʈ �浹 �˻�
	bool bSliding = (_rMovableFactor.MOVING_DUMMY.m_vDummyPosElipsed == vec3::ZERO);
	

	float fDist = 0.0f;
	vec3 vImpactPos;
	vec3 vImpactDir;
	int nCurrentTalentID = 0;
	
	bool bRub = objectChecker.CheckCanRub(_rMovableFactor, m_pModuleCollision->GetOwner(), _rMovableFactor.GENERAL_STATE.m_nTalentActiveID);
	bool bOffset = objectChecker.CheckCapsuleRadiusOffset(_rMovableFactor, m_pModuleCollision->GetOwner());

	//if(m_pModuleCollision->GetOwner()->GetUID() == XGetMyUID())
	//{
	//	mlog("������Ʈ %f %f %f\n", _rVecTo.x, _rVecTo.y, _rVecTo.z);
	//}
	//if(m_pModuleCollision->GetOwner()->GetType() == XOBJ_NET_PLAYER &&
	//	_rMovableFactor.GENERAL_STATE.m_bJumping)
	//{
	//	mlog("���ν�Ƽ %f %f\n", _rVecTo.x, _rVecTo.y);
	//}

	//----------------------------------------------------------------------//
	//

	_rMovableFactor.GENERAL_STATE.m_vecTerrainColObejct.clear();

	if(m_pModuleCollision->ColObject(_rVecTo, _rVecCurrentPos, vImpactPos, vImpactDir, fDist, _rMovableFactor.GENERAL_STATE.m_vecTerrainColObejct, bRub, bOffset))
	{
		vImpactDir.z = 0.0f;
		vImpactDir.Normalize();
		_rMovableFactor.GENERAL_STATE.m_bColObject = true;

		vec3 vMoveDir = vImpactDir * fDist;

		// ���� �ݴ� �����̸鼭 vMoveDir���� �� ũ�� �ڷ� �з����� �Ǵ� ���̹Ƿ� 
		// �̷��� �Ǹ� �������� �ʴ´�.
		if (_rMovableFactor.KNOCK_BACK.m_bKnockback)
		{
			float _rVecToLenSq = _rVecTo.LengthSq();
			float vMoveDirLensq = vMoveDir.LengthSq();

			if (_rVecToLenSq > 0 && vMoveDirLensq > 0)
			{
				vec3 vToNormal = _rVecTo;
				vToNormal.Normalize();

				float fDot = vToNormal.DotProduct(vImpactDir);
				if (fDot < 0.0f && vMoveDirLensq > _rVecToLenSq)
				{
					return;
				}
			}
		}

		_rVecTo += vMoveDir;
	}
}
