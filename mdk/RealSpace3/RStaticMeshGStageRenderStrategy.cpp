
#include "stdafx.h"
#include "RStaticMeshSceneNode.h"
#include "RShaderCompositeManager.h"
#include "RStaticMeshGStageRenderStrategy.h"
#include "RMaterial.h"

namespace rs3 {


bool RStaticMeshGStageRenderStrategy::CheckAndSetTechnique(  RCameraSceneNode* pCurrentCamera, RenderInfo* pRenderInfo, int nLightMapID, bool bFogEnable )
{
	m_pShaderCompositeManager->SetMaterialRenderState(pRenderInfo->m_pMaterial, true, true, false);

	RDevice* pDevice = REngine::GetDevicePtr();
	// �ϴ��� DSF�� ���� ������Ʈ�� ������Ʈ�� ���� ó���� �Ǳ� �������� ����ƽ �޽��� ���� ó���� ����.
	//http://dev:8181/projects/engine/ticket/1063
// 	if( pMaterial->dwFlags & RM_FLAG_ADDITIVE )
// 	{
// 		// ���� �ֵ�Ƽ��� ������Ʈ�� �н����� �׸��� �ʴ´�.
// 		return false;
// 	}
// 	else if( pMaterial->dwFlags & RM_FLAG_USEOPACITY || m_pStaticMesh->m_bPointColor )
// 	{
// 		// ���� ���� ���� ������Ʈ�� �н����� �׸��� �ʴ´�.
// 		return false;
// 	}
// 	else	// ���� �ƴ� ����
	{
		pDevice->SetFogEnable(bFogEnable);
		pDevice->SetBlending(RBLEND_NONE,RBLEND_NONE);
	}

	m_pTechnique = &pRenderInfo->m_GStageTechnique;

	return true;
}


}