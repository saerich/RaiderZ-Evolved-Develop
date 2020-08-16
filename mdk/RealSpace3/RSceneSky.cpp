#include "stdafx.h"
#include "RDevice.h"
#include "RDeviceD3D.h"

#include "RSceneSky.h"
#include "RSkyManager.h"
#include "RVType.h"
#include "RMesh.h"
#include "RMeshNode.h"
#include "RMeshMgr.h"
#include "RConfiguration.h"
#include "RCameraSceneNode.h"
#include "RShaderCompositeManager.h"
#include "RUtil.h"
#include "RActorNodeRenderStrategy.h"

#include <assert.h>

namespace rs3 {

RSceneSky::RSceneSky( RSkyInfo* pSkyInfo ) 
: RActor(RRP_NORMAL), m_pSkyInfo(pSkyInfo)
{
	m_matLocal->MakeIdentity();
	m_bUsingActorPassRenderer = false;
	m_bSceneActor = true;
	m_nBGWKPriorityOffset = RBGWK_HIGHEST;

	m_bAutoAnimationEnable = true;

	SetUsable(false);
}

RSceneSky::~RSceneSky()
{
}

void RSceneSky::OnAutoAnimationLoaded()
{
	RActor::OnAutoAnimationLoaded();

	if(m_pSkyInfo)
		SetSpeed( m_pSkyInfo->fUVAniSpeed );
}

bool RSceneSky::Create(const char *filename, bool bBackgroundCreation)
{
	bool bResult = RActor::Create(filename, bBackgroundCreation);

	m_bUseShade = false;

	return bResult;
}

void RSceneSky::OnRender( RCameraSceneNode* pCurrentCamera, int nSubMaterialID, RRENDERINGMETHOD renderMothod)
{
	// �ŸŴ����κ��� ������Ʈ ���� ����
}

int	RSceneSky::CalcPriority()
{
	return RBGWK_HIGHEST;
}

void RSceneSky::Update()
{
	RActor::OnUpdate();
}

void RSceneSky::Render( RCameraSceneNode* pCurrentCamera, const RVector4 &vMultiplyColor, float fogFactor, RRENDERINGMETHOD renderMothod)
{
	// ������ OnPreRender���� �ߴ����� OnRegisterToRender������ �ű�ϴ�.
	UpdateForRender();


	// (��å����)�θ𿡼� vis �� �������־��� ������ ���� ������ �ʿ����. 
	// �� ���ͳ��� manual update ��å�̱⶧���� UpdateVisibility�� ���־�� �Ѵ�
	SetVisibility(vMultiplyColor.a);
	UpdateVisibility();

	m_vMultiplyColor = vMultiplyColor;
	
	RVector4 vFogFactor;
	DWORD dwFogColor;
	RShaderCompositeManager::GetFogInfo(vFogFactor, dwFogColor);

	if(pCurrentCamera->GetFogEnable())
	{
		// ���̴����� ���� ������ �����Ͽ��� 0:���׾��� 1:���׸��� ���� ������ �ݴ�� ���� �Ǿ����Ƿ� 1-f ������ �Ѱ��ش�.( develop:rev6487����)
		RShaderCompositeManager::FogOn(0, 0, 1-fogFactor, pCurrentCamera->GetFogColor());
	}
	else
	{
		RShaderCompositeManager::FogOff();
	}



	// �޽� ����
	if ( !m_sortedRenderableActorNode.empty() )
	{
		for( int i=0; i<(int)m_sortedRenderableActorNode.size(); i++)
		{
			RActorNode *pActorNode = m_sortedRenderableActorNode[i];
			pActorNode->UpdateVisibility();

			RActorNodeRenderStrategySky RenderStrategy;
			pActorNode->RenderSingle( pCurrentCamera, &RenderStrategy );
		}
	}

	RShaderCompositeManager::FogOn(vFogFactor.x, vFogFactor.y, vFogFactor.z, dwFogColor);
}

}