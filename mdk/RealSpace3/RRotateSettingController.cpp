#include "StdAfx.h"
#include "RRotateSettingController.h"
#include "RCameraSceneNode.h"

namespace rs3 {

//////////////////////////////////////////////////////////////////////////
// RRotateSettingController

RRotateSettingController::RRotateSettingController( RVector vRotateAxis, float fRotateMin, float fRotateMax )
: RSceneNodeController(RSNCTRL_UPDATE)
, m_vRotateAxis(vRotateAxis)
, m_fRotateMin(fRotateMin)
, m_fRotateMax(fRotateMax)
{
	AddAttribute( RCA_STICKY );
}

void RRotateSettingController::OnAdd()
{
	m_vDirectionOrg.Set( m_pSceneNode->GetDirection() );
	OnEnable();
}

void RRotateSettingController::OnRemove()
{
	ResetLocalTransform();
}

void RRotateSettingController::OnEnable()
{
	ResetLocalTransform();

	RVector vDir( m_vDirectionOrg );
	float fRotate = RandomNumber( m_fRotateMin, m_fRotateMax );
	RMatrix matRot;
	matRot.SetRotationMatrixAxis( m_vRotateAxis, fRotate );

	RVector vScaleOrg = m_pSceneNode->GetScale();

	m_pSceneNode->SetTransform( matRot * m_pSceneNode->GetLocalTransform() );

	// Scale�� UpdateTransform()���� ���� ����Ǳ� ������ SetTransform�ϱ� ���� ���� �������ش�.
	m_pSceneNode->SetScale( vScaleOrg );
}

void RRotateSettingController::OnDisable()
{
	ResetLocalTransform();
}

void RRotateSettingController::ResetLocalTransform()
{
	m_pSceneNode->SetDirection( m_vDirectionOrg );
}

}