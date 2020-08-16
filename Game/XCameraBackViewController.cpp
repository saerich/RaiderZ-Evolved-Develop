#include "StdAfx.h"
#include "XCameraBackViewController.h"
#include "XModuleEntity.h"
#include "XModuleMovable.h"
#include "XModuleCollision.h"
#include "XBaseCameraSceneNode.h"
#include "RSpeedGrass.h"
#include "XCameraHelper.h"
#include "XCameraManager.h"
#include "XCameraControllerGroup.h"

XCameraBackViewController::XCameraBackViewController(void)
: XCameraSceneNodeController( RCCT_PREUPDATE, XCameraTypeStr[CAMERA_MAIN_BACKVIEW], true )
{
	m_bFreeLook = false;
	m_bFreeLookLimitDegree = false;

	m_bSyncToTargetDir = false;
}

XCameraBackViewController::XCameraBackViewController( RCAMERACONTROLLERTYPE eType, const char* _pSzCameraControllerID, bool _bManagedByCamera )
: XCameraSceneNodeController( eType, _pSzCameraControllerID, _bManagedByCamera )
{
	m_bFreeLook = false;
	m_bFreeLookLimitDegree = false;

	m_bSyncToTargetDir = false;
}

XCameraBackViewController::~XCameraBackViewController(void)
{
}

void XCameraBackViewController::Init( MUID uidObject )
{
	m_uidObject = uidObject;

	_UpdateCamera(0.0001f);

	XObject* pObject = gg.omgr->Find(m_uidObject);
	if ((pObject) && (pObject->GetModuleEntity()))
	{
		vec3 dir = pObject->GetModuleEntity()->GetDirection();
		SetDirection(dir);

		GetSceneNode()->SetTargetMUID(m_uidObject);
	}

	SetCollisionRadius(m_pCameraSceneNode->GetNearZ(),m_pCameraSceneNode->GetFovX(), m_pCameraSceneNode->GetFovY());

	if( rs3::REngine::GetSceneManagerPtr())
		rs3::REngine::GetSceneManagerPtr()->SetHDREnable( XGETCFG_VIDEO_HDR);
	XCameraHelper::SetDOFEnableAndParams( rs3::REngine::GetSceneManagerPtr(), XGETCFG_VIDEO_DOF);

	if (global.smgr->GetCurrentTerrain())
	{
		if ( global.smgr != NULL  &&  global.smgr->GetCurrentTerrain())
		{
			int _range = XGETCFG_VIDEO_GRNDOBJVISIBLERANGE;
			global.smgr->GetCurrentTerrain()->SetGrassLodParams( _range, _range + 2000);
			global.smgr->GetCurrentTerrain()->SetDetailDistance( _range * 3);
		}
	}
}

void XCameraBackViewController::SetDirection( const vec3& dir, const vec3& up/*=vec3(0.0f, 0.0f, 1.0f)*/ )
{
	XPolarCoord* pPolarCoord = GetPolarCoord();

	vec3 eye = dir;
	eye.Normalize();
	eye = eye * pPolarCoord->fRadius;

	MMath::CartesianToSpherical(eye, pPolarCoord->fRadius, pPolarCoord->fPhi, pPolarCoord->fTheta);

	GetSceneNode()->SetPolarCoord(pPolarCoord);
	m_pCameraSceneNode->SetDirection(dir, up);
}

void XCameraBackViewController::_UpdateCamera( float fDelta )
{
	UpdateSceneNode();

	if (gg.omgr == NULL) return;
	XObject* pObject = gg.omgr->Find(m_uidObject);
	if (pObject == NULL) return;

	XModuleEntity* pModuleEntity = pObject->GetModuleEntity();
	XModuleMovable* pModuleMovable = pObject->GetModuleMovable();
	XModuleCollision* pModuleCollision = pObject->GetModuleCollision();
	if (pModuleEntity == NULL ||
		pModuleMovable == NULL ||
		pModuleCollision == NULL) 
		return;

	vec3 tar = pModuleEntity->GetPosition();

	// �����߿��� XModuleMovable���� ��ġ���� ��������. (ī�޶�� ���� ���̺��� 1/2��ŭ�� �ö󰣴�.)
	if(pModuleMovable->GetMovableFactor().GENERAL_STATE.m_bJumping)
		tar = pModuleMovable->GetMovableFactor().GENERAL_STATE.m_vJumpPosForCamera;

	tar.z += XCONST::CAMERA_TARGET_HEIGHT;

	XPolarCoord* pPolarCoord = GetPolarCoord();

	// ��Ʈ�ѿ� ���� ���� �̵� ó��
	m_GroundTarget.StartEndUpdate(fDelta, pPolarCoord);

	// �� ��Ʈ�ѿ� ���� ������Ʈ 
	vec3 eye, position, direction;
	vec3 vColPos;
	if(UpdateController(pPolarCoord, tar, position, eye, direction, fDelta))
	{
		direction.Normalize();

		// �� ĳ���͸� �������� �񲸺��̰� �ϱ� ����
		vec3 right = CrossProduct(direction, UP_VECTOR);
		right.z = 0.0f;
		right.Normalize();

		vColPos = position+ (right * XCONST::CAMERA_TARGET_LEFT);

		// ���� �浹 üũ
		vector<RCollisionPolygon> objectCol;
		const XMovableFactor::SState& GENERAL_STATE = pModuleMovable->GetMovableFactor().GENERAL_STATE;
		vector<MUID> ObjectList = GENERAL_STATE.m_vecTerrainColObejct;
		pModuleCollision->BuildTerrainColObjectPolygon(&ObjectList, objectCol);
		m_Collision.SetTerrainColObject(&objectCol);
		bool bColTerrain = CheckTerrainCollision(vColPos, tar, vColPos);

		// ī�޶� �ε巴�� �̵�
		if (fDelta > 0.001f)
		{
			UpdateCameraSmoothMove(vColPos, tar, direction, vColPos, direction, fDelta, bColTerrain);
		}
	}
	else
	{
		vColPos = position;
	}

	// ������
	m_pCameraSceneNode->Set(vColPos, direction);	//UpdateCameraController()���� scenenode�� ����� �ݿ��� �ȵǾ �߰� �� �ݿ��� �ȵɱ�?
}

void XCameraBackViewController::CalcMaxPayneCameraZ( float& r, float& theta )
{
	// ī�޶� ������ ��������...
	if(theta < MMath::HALFPI)
	{
		// ���� ���� ���� �������� ���Ѵ�.
		float fPullValue	= (MMath::HALFPI - theta) / (MMath::HALFPI - MIN_CAMERA_THETA);			// 0.1���� ���� �������� 0.1�̱� �����̴�.

		// ���� ���� ī�޶� ��ġ�� ���ϱ�
		float fCameraTheta	= BezierCurve(0.0f, 0.1f, 0.6f, 1.0f, fPullValue);
		theta				+= fCameraTheta;

		// ���� ���� ī�޶� �Ÿ��� ���ϱ�
		float fPullValue2	= (MMath::HALFPI - theta) / (MMath::HALFPI - MIN_CAMERA_THETA);
		float fCameraDist	= BezierCurve(0.0f, 1.4f, 1.3f, MMath::HALFPI, fPullValue2);
		float fNewDist		= ((1.0f - fCameraDist) * r) + (fCameraDist * 10.0f);
		r					= min(fNewDist, r);
	}
}

void XCameraBackViewController::UpdateCamera()
{
	_UpdateCamera(0.00001f);
}

void XCameraBackViewController::UpdateCamera( float fDelta, int nDeltaX, int nDeltaY )
{
	XPolarCoord* pPolarCoord = GetPolarCoord();

	int nInvert = 1;
	if ( XGETCFG_CONTROL_INVERTEDMOUSE == true)
	{
		nInvert = -1;
	}

	if(m_GroundTarget.IsGroundTargetActive())
		nDeltaY *= 0.8f;

	//pPolarCoord->fPhi -= ( 0.003f * nDeltaX /*/ CalCameraSpeed( 100 - XGETCFG_CONTROL_MOUSESENSITIVITY)*/);
	//pPolarCoord->fTheta += (nInvert * 0.003f * nDeltaY /*/ CalCameraSpeed( 100 - XGETCFG_CONTROL_MOUSESENSITIVITY)*/);
	pPolarCoord->fPhi -= ( (float)nDeltaX * CalCameraSpeed( XGETCFG_CONTROL_MOUSESENSITIVITY) );
	pPolarCoord->fTheta += ( (float)nInvert * (float)nDeltaY * CalCameraSpeed( XGETCFG_CONTROL_MOUSESENSITIVITY));
	pPolarCoord->fTheta = max(min(pPolarCoord->fTheta, MAX_CAMERA_THETA), MIN_CAMERA_THETA);

	//if (m_bFreeLookLimitDegree)
	//{
	//	float toler = m_fFreeLookLimitTolerPhi;
	//	if (coord.fPhi < m_fFreeLookLimitPhi - toler) coord.fPhi = m_fFreeLookLimitPhi - toler;
	//	else if (coord.fPhi > m_fFreeLookLimitPhi + toler) coord.fPhi = m_fFreeLookLimitPhi + toler;
	//}

	GetSceneNode()->SetPolarCoord(pPolarCoord);
	_UpdateCamera(fDelta);
}

vec3 XCameraBackViewController::GetObjectDir()
{
	if (m_bFreeLookLimitDegree)
	{
		XPolarCoord* pPolarCoord = GetPolarCoord();

		float toler = m_fFreeLookLimitTolerPhi;
		if (pPolarCoord->fPhi < m_fFreeLookLimitPhi - toler) 
		{
			pPolarCoord->fPhi = m_fFreeLookLimitPhi - toler;
		}
		else if (pPolarCoord->fPhi > m_fFreeLookLimitPhi + toler) 
		{
			pPolarCoord->fPhi = m_fFreeLookLimitPhi + toler;
		}

		vec3 eye = MMath::SphericalToCartesian(pPolarCoord->fRadius, pPolarCoord->fPhi, pPolarCoord->fTheta);
		eye.z = m_pCameraSceneNode->GetDirection().z;
		eye.Normalize();
		return eye;
	}
	else
	{
		//return vec3(m_matLocal._21,m_matLocal._22,m_matLocal._23);
		return m_pCameraSceneNode->GetDirection();
	}
}

void XCameraBackViewController::LimitDegreeOnFreeLook( float fTolerRad )
{
	m_bFreeLookLimitDegree = true;
	m_fFreeLookLimitPhi = GetPolarCoord()->fPhi;

	m_fFreeLookLimitTolerPhi = fTolerRad;
}

void XCameraBackViewController::LimitDegreeOnFreeLook2( float fTolerRad )
{
	m_bFreeLookLimitDegree = true;

	XPolarCoord* pPolarCoord = GetPolarCoord();

	float toler = m_fFreeLookLimitTolerPhi;
	if (pPolarCoord->fPhi < m_fFreeLookLimitPhi - toler) 
	{
		pPolarCoord->fPhi = m_fFreeLookLimitPhi - toler;
	}
	else if (pPolarCoord->fPhi > m_fFreeLookLimitPhi + toler) 
	{
		pPolarCoord->fPhi = m_fFreeLookLimitPhi + toler;
	}

	m_fFreeLookLimitTolerPhi = fTolerRad;
	m_fFreeLookLimitPhi = pPolarCoord->fPhi;
}

void XCameraBackViewController::ReleaseLimitDegreeOnFreeLook()
{
	m_bFreeLookLimitDegree = false;
}

void XCameraBackViewController::StartGroundTarget()
{
	// ī�޶�� ������̶�� �����Ų��.
	EndCameraLock();

	m_GroundTarget.Start(GetPolarCoord());
}

void XCameraBackViewController::EndGroundTarget()
{
	m_GroundTarget.End(GetPolarCoord());
}

vec3 XCameraBackViewController::CalcCameraDirection( float fRadius, float fPhi, float fTheta )
{
	// ī�޶� ��� üũ
	if(m_bSyncToTargetDir)
	{
		if (gg.omgr == NULL) return vec3::AXISY;
		XObject* pObject = gg.omgr->Find(m_uidObject);
		if (pObject == NULL) return vec3::AXISY;
		XModuleEntity* pModuleEntity = pObject->GetModuleEntity();
		if(pModuleEntity == NULL) return vec3::AXISY;

		return pModuleEntity->GetDirection() * fRadius;
	}

	if (fRadius <= 0.0f) 
		fRadius = 0.001f;

	return MMath::SphericalToCartesian(fRadius, fPhi, fTheta);
}

vec3 XCameraBackViewController::CalcCameraPosition( vec3& veye, float fRadius, float fPhi, float fTheta )
{
	if(m_bFreeLook)
		return veye;

	CalcMaxPayneCameraZ(fRadius, fTheta);

	return MMath::SphericalToCartesian(fRadius, fPhi, fTheta);
}

void XCameraBackViewController::StartCameraLock( MUID& uidTarget, wstring& strBoneName, float& fDurationTime )
{
	// �׶��� Ÿ�� ���¿����� �۵����� �ʴ´�.
	if(m_GroundTarget.IsGroundTargetState())
		return;

	m_CameraLock.Start(m_pCameraSceneNode, GetPolarCoord(), m_uidObject, uidTarget, strBoneName, fDurationTime);
}

void XCameraBackViewController::EndCameraLock()
{
	m_CameraLock.End(GetPolarCoord(), m_pCameraSceneNode);
}

bool XCameraBackViewController::IsCameraLock()
{
	return m_CameraLock.IsCameraLock();
}

bool XCameraBackViewController::UpdateController( XPolarCoord* pPolarCoord, vec3 tar, vec3 &position, vec3 &eye, vec3 &direction, float fDelta )
{
	if(m_CameraInteration.IsActive())
	{
		tar.z -= XCONST::CAMERA_TARGET_HEIGHT;
		m_CameraInteration.UpdateCamera(fDelta, tar, position, direction);
		return false;
	}
	else if(m_GroundTarget.IsGroundTargetActive())
	{
		// ���� Ÿ��
		m_GroundTarget.CalcPosDirForGroundTarget(pPolarCoord, tar, position, direction);
	}
	else if(m_CameraLock.IsCameraLockStartEnd())
	{
		m_CameraLock.StartEndUpdate(fDelta, pPolarCoord, m_pCameraSceneNode, tar, position, direction);
	}
	else if(m_CameraLock.IsCameraLockActive())
	{
		// ī�޶� ��
		m_CameraLock.CalcPosDirForCameraLock(fDelta, pPolarCoord, m_pCameraSceneNode, position, direction);
	}
	else
	{
		// ���̽�
		eye = CalcCameraDirection(pPolarCoord->fRadius, pPolarCoord->fPhi, pPolarCoord->fTheta);
		direction = eye;

		eye = CalcCameraPosition(eye, pPolarCoord->fRadius, pPolarCoord->fPhi, pPolarCoord->fTheta);

		position = tar - eye;
	}

	return true;
}

void XCameraBackViewController::StartInteraction(MUID& uidTarget)
{
	if (gg.omgr == NULL) return;
	XObject* pObject = gg.omgr->Find(m_uidObject);
	if(pObject == NULL) return;

	// ī�޶�� ������̶�� �����Ų��.
	EndCameraLock();

	m_CameraInteration.Start(uidTarget, pObject->GetPosition(), m_pCameraSceneNode);
}

void XCameraBackViewController::EndInteration()
{
	if (gg.omgr == NULL) return;
	XObject* pObject = gg.omgr->Find(m_uidObject);
	if(pObject == NULL) return;

	m_CameraInteration.End(pObject->GetPosition());
}

void XCameraBackViewController::ForceEndInteration()
{
	m_CameraInteration.ForceEnd();
}
