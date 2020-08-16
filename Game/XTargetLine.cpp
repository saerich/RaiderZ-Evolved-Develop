#include "stdafx.h"
#include "XTargetLine.h"
#include "XNonPlayer.h"

XTargetLine::XTargetLine(XObject* pObject)
: XTarget(pObject)
, m_pTarget(NULL)
{

}

XTargetLine::~XTargetLine()
{
	SAFE_DELETE(m_pTarget);
}

void XTargetLine::UpdateTarget()
{

	// ī�޶� ���� ��������
	vec3 vPos;
	vec3 vDir;
	GetCameraInfo(vPos, vDir);

	// Ÿ�� ��ȿ�Ѱ�?
	CheckExistTargetObj();

	// Ÿ�� ������Ʈ
	UpdateTarget(vPos, vDir);
}

void XTargetLine::CheckExistTargetObj()
{
	if( m_pTarget != NULL)
	{
		XObject* pTargetObject = gg.omgr->FindObject(m_pTarget->uidTarget);
		if(pTargetObject == NULL)
		{
			DelTargetAll();
		}
	}
}

// �������� ������ ����
void XTargetLine::DelTargetAll()
{
	PFC_THISFUNC;

	if (m_pTarget == NULL) return;

	m_TargetRenderer.DelTargetEffect(m_pTarget->uidEffect);

	SAFE_DELETE(m_pTarget);
}

void XTargetLine::UpdateTarget( vec3& pos, vec3& dir )
{
	PFC_THISFUNC;

	///************************************************************************/
	///* // [10/10/2007 isnara] �ϴ� Ÿ���� ���� �ϳ� ��°ɷ� �Ǿ� �ִ�.     */ 
	///************************************************************************/
	float fNearDis	= 99999.0f;
	stTargetID targetObject;

	for(XObjectMap::iterator itor = gg.omgr->BeginItor(); itor != gg.omgr->EndItor(); ++itor)
	{
		XObject* pObj = (*itor).second;
		if (m_pOwner == pObj) continue;

		//------------------------------------------------------------------------
		// ��� ���� Ÿ�� �˻�
		if(CheckTargetAllowed(pObj) == false)	continue;

		//------------------------------------------------------------------------
		// ��뿡 ���� üũ
		// Ÿ�� ������ �ȵŴ°�?
		if(CheckTargetObject(pObj, pos) == false)	continue;

		//------------------------------------------------------------------------
		// Ÿ�� �˻�
		//stTargetID targetObject;
		CheckLineTarget(pObj, pos, dir, targetObject, fNearDis);
	}

	// ���� Ÿ��
	if(targetObject.uidTarget != MUID::ZERO)
	{
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
		m_pTarget->time				= timeGetTime();
	}
	else
	{
		// ���ٸ�...
		DelTargetAll();
	}
}

bool XTargetLine::CheckLineTarget( XObject* pObject, vec3& pos, vec3& dir, stTargetID& outTargetObject, float& fOutDist )
{
	PFC_THISFUNC;

	bool bColl = false;

	//------------------------------------------------------------------------
	// �浹 ����


	// ���ʿ� �ִ� ����� üũ
	vec3 vObjectDir = (pObject->GetPosition() - pos).Normalize();
	if(vObjectDir.DotProduct(dir) > 0)
	{
		if(CheckLineTargetObj(pObject, pos, dir,outTargetObject, fOutDist))
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

bool XTargetLine::CheckTargetObject( XObject* pObject, vec3& pos)
{
	PFC_THISFUNC;

	if( m_pTarget &&
		m_pTarget->uidTarget != pObject->GetUID())
	{
		if(CheckTargetHitCapsule(pObject) == false)
			return false;

		return CSTalentInfoHelper::CheckEffectiveRange(pObject->GetPosition(), pos, m_fTargetRange);		// Ÿ�� ��Ÿ��� ������
	}

	return true;
}

bool XTargetLine::CheckLineTargetObj( XObject* pObject, vec3& pos, vec3& dir, stTargetID& outTargetObject, float& fOutDist )
{
	PFC_THISFUNC;

	if(pObject->GetModuleCollision())
	{
		vec3 vCenterPos;
		float fCurDist			= fOutDist;

		// 1. Pick���� �˻�
		if(pObject->GetModuleCollision()->PickRayToHitEffectObject(pos, dir, fCurDist, vCenterPos, fOutDist, outTargetObject.nTargetGroupID, outTargetObject.nTargetCapsuleID))
		{
			outTargetObject.uidTarget			= pObject->GetUID();
			outTargetObject.vTargetPos			= pos + (dir * fOutDist);

			return true;
		}
	}

	return false;
}

bool XTargetLine::GetTargetIDByUID( MUID uid, int& nOutGroupID, int& nOutDummyID )
{
	if( m_pTarget &&
		m_pTarget->uidTarget == uid)
	{
		nOutGroupID = m_pTarget->nTargetGroupID;
		nOutDummyID = m_pTarget->nTargetCapsuleID;

		return true;
	}

	return false;
}

bool XTargetLine::CheckOverlapDummy( stTargetID& targetObject )
{
	// ���� ����Ǿ� �ִ� Ÿ�ٰ� �˻縦 �Ѵ�.
	// Ÿ�� ������ �־�� �Ѵ�. �׷��� �񱳸� �ϴѱ�...
	if( m_pTarget &&
		m_pTarget->uidTarget != MUID::ZERO)
	{
		if(targetObject.uidTarget == m_pTarget->uidTarget &&
			targetObject.nTargetGroupID == m_pTarget->nTargetGroupID &&
			targetObject.nTargetCapsuleID == m_pTarget->nTargetCapsuleID)					
		{
			// ���ٸ�...
			return true;
		}
	}

	return false;
}
