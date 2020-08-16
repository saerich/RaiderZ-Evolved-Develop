#include "stdafx.h"
#include "XTargetGround.h"
#include "XEffectInvoker.h"
#include "XEffectManager.h"
#include "XCameraManager.h"
#include "XInteractionInfo.h"
#include "XModuleUI.h"

XTargetGround::XTargetGround(XObject* pObject)
: XTarget(pObject)
{
	m_uidTargetEffect.SetZero();
	m_vecDecalEffectData.clear();
	m_fSize = 1.0f;
}

XTargetGround::~XTargetGround()
{
	for(vector<EFFECT_RESULT_DATA>::iterator it = m_vecDecalEffectData.begin(); it != m_vecDecalEffectData.end(); ++it)
	{
		if(it->type == CET_MODEL ||
			it->type == CET_DECALEFFECT)
			global.emgr->DeleteXEffect(it->uidEffect);
	}

	m_vecDecalEffectData.clear();

	ReleaseCameraForTargetGround();
	ReleaseGroundTargetPartyMiniHpbar();

	gvar.MyInfo.Party.ClearPartyMemberEffect(ME_HIGHTLIGHT);
}

void XTargetGround::UpdateTarget()
{
	// ���콺 �����Ͱ� Ȱ��ȭ�Ǿ� �־�� �Ѵ�.
	vec3 vTerrainPos;
	if (global.ui->IsMouseVisible() == true)
	{
		if(GetTerrainPosFromMouse(vTerrainPos) == false)
			return;
	}
	else
	{
		if(GetTerrainPosFromCamera(vTerrainPos) == false)
			return;
	}

	UpdateGroundTargetEffect(vTerrainPos);
	UpdateGroundTargetObjectGlowEffect(vTerrainPos);

	m_Target.vTargetPos = vTerrainPos;
}

bool XTargetGround::CheckTerrainTarget( vec3 vStart, vec3 vDir, vec3& vColPos )
{
	float fRange = m_fTargetRange;

	float fDot = vDir.DotProduct(-vec3::AXISZ);
	if(fDot > 0.0f)
	{
		float angle = acosf(fDot) * 180.0f / MMath::PI;
		float fRatio = (angle - 60.0f) / 30.0f;
		if(fRatio < 0.0f)
			fRatio = 0.0f;
		fRange = fRatio * m_fTargetRange;
	}

	// ��ġ �˻�
	ColTerrainPos(vDir, fRange, vColPos);
	return true;
}

void XTargetGround::CreateGroundTarget( float fSize )
{
	EFFECT_POST_TRANSFORM postData;
	postData.vSize = vec3(fSize, fSize, XCONST::DECALEFFECT_DEPTH_VALUE);

	vec3 vTerrainPos;
	if(GetTerrainPosFromCamera(vTerrainPos) == false)
		return;

	XEffectSinglyInvoker decalInvoker;
	decalInvoker.Invoke(wstring(L"decal_Interaction"), vTerrainPos, vec3(vec3::AXISY), vec3(vec3::AXISZ), &m_vecDecalEffectData, &postData);

	m_fSize = fSize;

	SetCameraForTargetGround();
}

void XTargetGround::UpdateGroundTargetEffect( vec3 vPos )
{
	for(vector<EFFECT_RESULT_DATA>::iterator it = m_vecDecalEffectData.begin(); it != m_vecDecalEffectData.end(); ++it)
	{
		if(it->type == CET_DECALEFFECT)
		{
			XDecalEffect* pDecal = global.emgr->GetXDecalEffect(it->uidEffect);

			if(pDecal)
			{
				pDecal->SetDecalEffectData(vPos, m_fSize, m_fSize, XCONST::DECALEFFECT_DEPTH_VALUE);
			}
		}
	}	
}

void XTargetGround::SetCameraForTargetGround()
{
	if ( global.camera->IsPrimaryCameraType( CAMERA_MAIN_BACKVIEW))
	{
		XCameraBackViewController* pBackViewCamera = global.camera->GetCameraController<XCameraBackViewController>();
		if (pBackViewCamera)
		{
			pBackViewCamera->StartGroundTarget();
		}
	}
}

void XTargetGround::ReleaseCameraForTargetGround()
{
	if ( global.camera->IsPrimaryCameraType( CAMERA_MAIN_BACKVIEW))
	{
		XCameraBackViewController* pBackViewCamera = global.camera->GetCameraController<XCameraBackViewController>();
		if (pBackViewCamera)
		{
			pBackViewCamera->EndGroundTarget();
		}
	}
}

void XTargetGround::ColTerrainPos( vec3 vDir, float fRange, vec3& vColPos )
{
	float fDist = 0.0f;
	float fMaxValue = TERRAIN_HEIGHT_COLLISION_VALUE;

	// ��ġ
	vec3 vMaxDir = vDir;
	XMath::NormalizeZ0(vMaxDir);
	vec3 vMaxPos = m_pOwner->GetPosition() + (vMaxDir * fRange);

	// ���� Max �˻�
	vec3 vCheckMaxPosStart = vMaxPos + vec3(0, 0, 1.0f);
	vec3 vCheckMaxPosEnd = vMaxPos + vec3(0, 0, fMaxValue * -1.0f);
	vec3 vMaxHeight = vCheckMaxPosEnd;
	if(CheckColTerrain(vCheckMaxPosStart, vCheckMaxPosEnd, fDist, &vMaxHeight))
	{
		fMaxValue = fDist * -1.0f;
	}

	// ��ġ ��ŷ
	vec3 vStartPos = vMaxPos + vec3(0, 0, fMaxValue * -1.0f);
	vec3 vEndPos = vMaxPos + vec3(0, 0, fMaxValue);

	// �ϴ� �浹 ��ġ�� ���Ѵ�.
	// �浹�� �ȵǸ� �� ��ġ�� ��´�.
	vColPos = vMaxPos;

	CheckColTerrain(vStartPos, vEndPos, fDist, &vColPos);
}

bool XTargetGround::GetTerrainPosFromCamera( vec3& vTerrainPos )
{
	// ī�޶� ���� ��������
	vec3 vPos;
	vec3 vDir;
	GetCameraInfo(vPos, vDir);

	// ���� ���� �ø���.
	vPos.z += 50.0f;

	vTerrainPos = vec3::ZERO;
	return CheckTerrainTarget(vPos, vDir, vTerrainPos);
}

void XTargetGround::UpdateGroundTargetObjectGlowEffect( vec3 vPos )
{
	for(XObjectMap::iterator itor = gg.omgr->BeginItor(); itor != gg.omgr->EndItor(); ++itor)
	{
		XObject* pObj = (*itor).second;

		DisableGlowEffect(pObj);
		SetGroundTargetPartyMiniHpbar(pObj, false);
		gvar.MyInfo.Party.SetPartyMemberEffect(pObj->GetUID(), ME_NONE);

		//------------------------------------------------------------------------
		// ��� ���� Ÿ�� �˻�
		// ������ ȿ�������� ��û���� ���ؼ� ó��
		if(pObj->GetUID() != XGetMyUID())
		{
			if(CheckTargetAllowed(pObj) == false)	continue;
		}

		//------------------------------------------------------------------------
		// ��뿡 ���� üũ
		// Ÿ�� ������ �ȵŴ°�?
		if(CheckTargetObject(pObj, vPos) == false)	continue;

		EnableGlowEffect(pObj);
		SetGroundTargetPartyMiniHpbar(pObj, true);
		gvar.MyInfo.Party.SetPartyMemberEffect(pObj->GetUID(), ME_HIGHTLIGHT);
	}
}

void XTargetGround::EnableGlowEffect( XObject* pOwner )
{
	XGetInteractionInfo().SetInteractionSearchUID(pOwner->GetUID());

	vec4 vColor = XCONST::GROUND_TARGET_EDGE_COLOR;

	global.emgr->EnablePEEdge((MUID)pOwner->GetUID(), RVector4(vColor.r, vColor.g, vColor.b, 1.8f), 2.2f, 1.35f, false);
}

void XTargetGround::DisableGlowEffect( XObject* pOwner )
{
	XGetInteractionInfo().SetInteractionSearchUID(MUID::ZERO);
	
	global.emgr->DisablePEEdge((MUID)pOwner->GetUID());
}

bool XTargetGround::CheckTargetObject( XObject* pObject, vec3& vPos )
{
	if(CheckTargetHitCapsule(pObject) == false)
		return false;

	float fDist = (pObject->GetPosition() - vPos).Length();
	if(fDist <= m_fSize)
		return true;

	return false;
}

bool XTargetGround::GetTerrainPosFromMouse( vec3& vTerrainPos )
{
	// 2���� �׶��� Ÿ��
	vec3 vPos;
	vec3 vDir;
	if(GetTrans3DCoordinates(MEvent::GetMouseClientPos().x, MEvent::GetMouseClientPos().y, vPos, vDir) == false)
		return false;

	if(m_pOwner->GetModuleCollision())
	{
		vec3 vNor;

		const float PICK_MIN_Z = TERRAIN_HEIGHT_COLLISION_VALUE;
		const float PICK_MAX_Z = TERRAIN_HEIGHT_COLLISION_VALUE * -1.0f;

		vec3 vMovePos = vPos + (vDir * 5000.0f);

		if(m_pOwner->GetModuleCollision()->PickRayToTerrain(vPos, vMovePos, vTerrainPos, vNor, CollisionAttribute::ALL) == false)
			return false;

		vec3 vTestMaxPos = vTerrainPos;
		vTestMaxPos.z = m_pOwner->GetPosition().z;
		float fTestLen = (m_pOwner->GetPosition() - vTestMaxPos).Length();
		if(fTestLen > m_fTargetRange)
		{
			vec3 vMaxDir = vDir;
			XMath::NormalizeZ0(vMaxDir);
			vec3 vMaxPos = m_pOwner->GetPosition() + (vMaxDir * m_fTargetRange);

			// ��ġ ��ŷ
			vec3 vStartPos = vMaxPos + vec3(0, 0, PICK_MAX_Z);
			vec3 vEndPos = vMaxPos + vec3(0, 0, PICK_MIN_Z);

			if(m_pOwner->GetModuleCollision()->PickRayToTerrain(vStartPos, vEndPos, vTerrainPos, vNor, CollisionAttribute::ALL) == false)
				return false;		
		}
	}

	return true;
}

void XTargetGround::SetGroundTargetPartyMiniHpbar( XObject* pOwner, bool bShow )
{
	if(gvar.MyInfo.Party.IsPartyMember(pOwner->GetUID()) == false)
		return;

	XModuleUI* pModuleUI = pOwner->GetModuleUI();
	if(pModuleUI)
	{
		pModuleUI->SetShowPartyMiniHpBar(bShow);
	}
}

void XTargetGround::ReleaseGroundTargetPartyMiniHpbar()
{
	for(XObjectMap::iterator itor = gg.omgr->BeginItor(); itor != gg.omgr->EndItor(); ++itor)
	{
		XObject* pObj = (*itor).second;
		SetGroundTargetPartyMiniHpbar(pObj, false);
	}
}
