#include "stdafx.h"
#include "XCameraHelper.h"
#include "XConst.h"
#include "REngine.h"

void XCameraHelper::ResetFov( RCameraSceneNode* pCameraSceneNode )
{
	_ASSERT( XCONST::CAMERA_FOV > 0 );
	float fFovX = DegreeToRadian( XCONST::CAMERA_FOV );
	float aspectRatio34 = 3.f / 4.f;
	float fDefaultFovY = atanf(tanf(fFovX/2.0f)*aspectRatio34)*2.0f; // 4:3 ȭ�� ���� YFOV

	int nWidth = rs3::REngine::GetConfig().m_nWidth;
	int nHeight = rs3::REngine::GetConfig().m_nHeight;
	float fAspectRatio = float(nWidth) / float(nHeight);

	// �ش� FOVY �� �´� �� FOV �� ���Ѵ�
	float fNewFovX = atanf(tanf(fDefaultFovY/2.0f)*fAspectRatio)*2.0f;

 	pCameraSceneNode->SetFov( fNewFovX, fAspectRatio );
}

void XCameraHelper::SetDOFEnableAndParams( RSceneManager* pSceneManager, bool bEnable )
{
	//if (bEnable == pCameraSceneNode->IsEnableDOF()) return;

	pSceneManager->SetDOFEnable( bEnable);

	if (bEnable)
	{
		pSceneManager->SetDOFFocalDist(XCONST::DOF_FOCUS_DIST);
		pSceneManager->SetDOFFocalRange(XCONST::DOF_FOCUS_RANGE);
		pSceneManager->SetDOFCOCSize(XCONST::DOF_COC_SIZE);
	}
}