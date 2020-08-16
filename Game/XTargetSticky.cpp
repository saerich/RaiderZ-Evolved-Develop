#include "stdafx.h"
#include "XTargetSticky.h"
#include "XNonPlayer.h"

XTargetSticky::XTargetSticky(XObject* pObject)
: XTargetLine(pObject)
{

}

XTargetSticky::~XTargetSticky()
{

}

void XTargetSticky::UpdateTarget()
{
	// ī�޶� ���� ��������
	vec3 vPos;
	vec3 vDir;
	GetCameraInfo(vPos, vDir);

	// Ÿ�� ��ȿ�Ѱ�?
	CheckExistTargetObj();

	// Ÿ�� ������Ʈ
	UpdateStickyTarget(vPos, vDir);
}

void XTargetSticky::UpdateStickyTarget( vec3& pos, vec3& dir )
{
	///************************************************************************/
	///* // [10/10/2007 isnara] �ϴ� Ÿ���� ���� �ϳ� ��°ɷ� �Ǿ� �ִ�.     */ 
	///************************************************************************/
	float fNearDis	= 99999.0f;
	stTargetID targetObject;

	for(XObjectMap::iterator itor = gg.omgr->BeginItor(); itor != gg.omgr->EndItor(); ++itor)
	{
		XObject* pObj = (*itor).second;

		//------------------------------------------------------------------------
		// ��� ���� Ÿ�� �˻�
		if(CheckTargetAllowed(pObj) == false)	continue;

		//------------------------------------------------------------------------
		// ��뿡 ���� üũ
		// Ÿ�� ������ �ȵŴ°�?
		if(CheckTargetObject(pObj) == false)	continue;

		//------------------------------------------------------------------------
		// ��Ÿ üũ
		// �κ� ������ �ִ°�?
		XModuleCollision* pModuleCollision = pObj->GetModuleCollision();
		if(pModuleCollision == NULL)	continue;

		//------------------------------------------------------------------------
		// Ÿ�� �˻�
		CheckLineTarget(pObj, pos, dir, targetObject, fNearDis);
	}

	// ���� Ÿ��
	if(targetObject.uidTarget != MUID::ZERO)
	{
		if (global.ui)
			global.ui->TargetUILock();

		// ���� �ϳ���� ���ǿ� �˻縦 �Ѵ�.
		// ������ ������?
		if(CheckOverlapDummy(targetObject))
		{
			// ����
			return;
		}

		// ���� �ϳ��� ������ �׳� �� ����� �ٽ� �˻��� �Ѵ�.
		DelTargetAll();

		//mlog("Ÿ�� ���� %d %d\n", targetObject.nTargetGroupID, targetObject.nTargetDummyID);

		// Ÿ�� ����
		m_pTarget = new stTargetID();
		m_pTarget->uidTarget		= targetObject.uidTarget;
		m_pTarget->nTargetGroupID	= targetObject.nTargetGroupID;
		m_pTarget->nTargetCapsuleID	= targetObject.nTargetCapsuleID;
	}
	else
	{
		// ���ٸ�...
		DelTargetAll();

		if (global.ui)	global.ui->TargetUIAim();
	}
}

bool XTargetSticky::CheckLineTarget( XObject* pObject, vec3& pos, vec3& dir, stTargetID& outTargetObject, float& fOutDist )
{
	bool bColl = false;

	//------------------------------------------------------------------------
	// �浹 ����
	// ���ʿ� �ִ� ����� üũ
	vec3 vObjectDir = (pObject->GetPosition() - m_pOwner->GetPosition()).Normalize();
	if(vObjectDir.DotProduct(m_pOwner->GetDirection()) > 0)
	{
		if(CheckLineTargetObj(pObject, pos, dir, outTargetObject, fOutDist))
		{
			bColl = true;
		}	
	}

	//------------------------------------------------------------------------
	// �浹�̸� ���� �浹 �˻�
	if(bColl)
	{
		vec3 vTargetPos = outTargetObject.vTargetPos;

		float fDistTerrain = 0.0f;
		vec3 vTerrainPos;
		if(CheckColTerrain(pos, vTargetPos, fDistTerrain, &vTerrainPos))
		{
			if(fOutDist > fDistTerrain)
			{
				//#ifdef _DEBUG
				//		global.smgr->GetPrimaryCamera()->GetPassRenderer< RDebugPassRenderer >()->AddDebugLine(pos, vTerrainPos, 2.0f, D3DCOLOR_XRGB(255, 0, 0));
				//#endif
				//mlog("���� �浹 %f(%f)\n", fDistTerrain, fNearDis);
				outTargetObject.Init();
				return false;
			}
		}

		return true;
	}

	return false;
}

bool XTargetSticky::CheckTargetObject( XObject* pObject )
{
	if( pObject )
	{
		return CSTalentInfoHelper::CheckEffectiveRange(pObject->GetPosition(), m_pOwner->GetPosition(), m_fTargetRange);		// Ÿ�� ��Ÿ��� ������
	}

	return false;
}

bool XTargetSticky::CheckLineTargetObj( XObject* pObject, vec3& pos, vec3& dir, stTargetID& outTargetObject, float& fOutDist )
{
	if(pObject->GetModuleCollision())
	{
		vec3 vCenterPos;
		float fCurDist			= fOutDist;

		if(pObject->GetModuleCollision()->IsHaveHitEffectCount())
		{
			// ��Ʈ ����Ʈ ĸ���� �˻�
			// 1. Ray �˻�
			if(pObject->GetModuleCollision()->PickRayToHitEffectObject(pos, dir, fCurDist, vCenterPos, fOutDist, outTargetObject.nTargetGroupID, outTargetObject.nTargetCapsuleID)) 
			{
				outTargetObject.uidTarget			= pObject->GetUID();
				outTargetObject.vTargetPos			= vCenterPos;

				return true;
			}

			// 2. 1���� �ȵŸ� Sticky ������� �˻�
			if(pObject->GetModuleCollision()->LineRayToHitEffectObject(pos, dir, fCurDist, vCenterPos, fOutDist, outTargetObject.nTargetGroupID, outTargetObject.nTargetCapsuleID))
			{
				outTargetObject.uidTarget			= pObject->GetUID();
				outTargetObject.vTargetPos			= vCenterPos;

				return true;
			}
		}
		else
		{
			// �浹ĸ�� Sticky ������� �˻�
			MCapsule outCapsule;
			if(pObject->GetModuleCollision()->PickRayToColCapsule(pos, dir, fCurDist, outCapsule, vCenterPos, fOutDist))
			{
				outTargetObject.uidTarget			= pObject->GetUID();
				outTargetObject.vTargetPos			= vCenterPos;

				return true;
			}
		}
	}

	return false;
}

