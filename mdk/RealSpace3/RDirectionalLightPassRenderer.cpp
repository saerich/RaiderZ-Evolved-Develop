#include "StdAfx.h"
#include "RDirectionalLightPassRenderer.h"
#include "RShaderCompositeManager.h"
#include "RDirectionalLightSceneNode.h"
#include "RShaderFX.h"
#include "RLightingShader.h"
#include "RShadowStrategy.h"
#include "RPSSMShadowStrategy.h"
#include "REnvironmentSet.h"
#include "RGIProcessor.h"


#define SUB_LIGHT_COUNT_MAX 4


namespace rs3 {

//#define RT_SHADOW

RDirectionalLightPassRenderer::RDirectionalLightPassRenderer(void)
{
}

RDirectionalLightPassRenderer::~RDirectionalLightPassRenderer(void)
{
}

void RDirectionalLightPassRenderer::RenderLightingToLBuffer( RCameraSceneNode* pCamera, SCENE_SET* pCulledList, RLightingShader* pLightingShader, DWORD dwWidth, DWORD dwHeight, RGIProcessor* pGIProcessor, RRENDERING_MODE eRRM)
{
	RPFC_THISFUNC;


	// TODO: pCulledList�κ��� ������ �ϰ� �����ؾ� �Ѵ�.
	RDirectionalLightSceneNode* pDirLight = REngine::GetSceneManager().GetDefaultLight();
	SCENE_SET* pSubLightList = pCamera->GetViewFrustumCullingMgr()->GetCulledList(RRP_SUBLIGHT);
	if( pDirLight == NULL )
	{
		if(pSubLightList->empty()==false)
			RenderSubLightOnly(pCamera, pLightingShader, pSubLightList, dwWidth, dwHeight);

		return;
	}

	SetDirectionalLightShaderConstant(pCamera, pLightingShader, pDirLight);
	int nSubLightCount = SetSubLightShaderConstant(pCamera, pLightingShader, pSubLightList);

	bool bSpecular = eRRM != RRM_NO_SPECULAR_RIM_LIGHT;
	bool bShadowOnlyGI = pGIProcessor && pGIProcessor->IsShadowOnly();
	bool bDirLightOnlyGI = pGIProcessor && !pGIProcessor->IsAllLight();


	/// �ε��츸 �ͽ���Ʈ
	if( bShadowOnlyGI)
	{
		ExportShadow(pGIProcessor, pLightingShader, pDirLight, dwWidth, dwHeight);
	}



	/// GI ó���� All Light( ��� ����Ʈ�� ���� ����)�� �ƴϸ� �𷺼ų� ����Ʈ������ ó���� ���̹Ƿ� ���⼭ ó�� 
	/// GI�� ������ ��ü ���� Ÿ�ٿ� ����
	if( bDirLightOnlyGI)
		pGIProcessor->Render( pCamera, dwWidth, dwHeight, false);



	SetRenderState(bDirLightOnlyGI, bShadowOnlyGI, pGIProcessor);
	{
		/// �ͽ���Ʈ �� �ε��츦 ����Ʈ�� �ô� filter �� lerp�� ���� �Ѵ�. bShadowOnlyGI�� ���̸� ������ �ͽ���Ʈ �Ǿ���.
		if( RShaderFX* pShader = pLightingShader->CreateDirectionalLightShader( pDirLight, bSpecular, bSpecular, false, bShadowOnlyGI, bDirLightOnlyGI, !bShadowOnlyGI, !bShadowOnlyGI, nSubLightCount) )
		{
			RTechnique tech = pShader->GetTechnique( "MainTechnique");
			pLightingShader->RenderQuad( &tech, dwWidth, dwHeight);
		}
	}
	RestoreRenderState();
}

void RDirectionalLightPassRenderer::SetDirectionalLightShaderConstant( RCameraSceneNode* pCamera, RLightingShader* pLightingShader, RDirectionalLightSceneNode* pDirLight )
{

	RVector3 vLightDir = Normalize( -pDirLight->GetDirection() );
	pCamera->GetViewMatrix().TransformNormal(vLightDir);
	RVector3 vLightColor = RVector(pDirLight->GetDiffuse().r, pDirLight->GetDiffuse().g, pDirLight->GetDiffuse().b) * pDirLight->GetPower();
	RVector3 vLightAmbient = RVector(pDirLight->GetAmbient().r, pDirLight->GetAmbient().g, pDirLight->GetAmbient().b) * pDirLight->GetPower();

	pLightingShader->m_scLightVec.SetVector3( vLightDir );
	pLightingShader->m_scLightColor.SetVector3( vLightColor );
	pLightingShader->m_scLightAmbient.SetVector3( vLightAmbient );


	if(pDirLight->IsUseSkySpecular())
	{
		RVector3 vSkyDir(0,0,1);
		pCamera->GetViewMatrix().TransformNormal(vSkyDir);
		pLightingShader->m_scSkySpecularTerm.SetVector4( RVector4(vSkyDir, pDirLight->GetSkySpecular()) );
	}


	if( pDirLight->GetDirection().Length() > 0.f )
	{
		pLightingShader->m_scShadowLuminosity.SetFloat(pDirLight->GetShadowLuminosity());

		//Set Shadow constant
		SetShadowRenderState( pCamera, pLightingShader );
		SetCloudRenderState( pCamera, pLightingShader );
	}
}

int RDirectionalLightPassRenderer::SetSubLightShaderConstant(RCameraSceneNode* pCamera, RLightingShader* pLightingShader, SCENE_SET* pSubLightList)
{
	RPFC_THISFUNC;

	RVector3 vSubLightDir[SUB_LIGHT_COUNT_MAX];
	RVector3 vSubLightColor[SUB_LIGHT_COUNT_MAX];

	int nLightCount = min(SUB_LIGHT_COUNT_MAX, pSubLightList->size());

	SCENE_SET::iterator SubLightIter = pSubLightList->begin();
	for(int i=0; i<nLightCount; ++i)
	{
		RSubLightSceneNode* pSubLight = MDynamicCast(RSubLightSceneNode, *SubLightIter);
		_ASSERT(pSubLight);

		RVector3 vDirection = -pSubLight->GetDirection().Normalize();
		pCamera->GetViewMatrix().TransformNormal(vDirection);

		vSubLightDir[i] = vDirection;
		vSubLightColor[i] = pSubLight->GetColorVector();

		++SubLightIter;
	}

	pLightingShader->m_scSubLightDir.SetVector3Array(vSubLightDir, nLightCount);
	pLightingShader->m_scSubLightColor.SetVector3Array(vSubLightColor, nLightCount);	


	REnvironmentSet* pEnvironmentSet = REngine::GetSceneManager().GetCurrentRootSceneNode(false)->GetEnvironmentSet();
	if( pEnvironmentSet )
		pLightingShader->m_scSubLightIntensity.SetFloat( pEnvironmentSet->GetSubLightIntensity() );
	else
		pLightingShader->m_scSubLightIntensity.SetFloat(1.0f);
	

	return nLightCount;
}

void RDirectionalLightPassRenderer::SetShadowRenderState( RCameraSceneNode* pCamera, RLightingShader* pLightingShader )
{
	if( REngine::GetSceneManager().GetRealtimeShadowMethod() == SHADOW_NONE )
		return;

	if( !pCamera )
		return;

	_ASSERT( pLightingShader );

	RShadowStrategy* pShadowStrategy = REngine::GetSceneManager().GetShadowStrategy();

	_ASSERT( pShadowStrategy );
	if ( pShadowStrategy == NULL)
		return;


	REngine::GetDevice().SetTexture( AS_DEPTHMAP ,pShadowStrategy->GetCurrentDepthMap());
	REngine::GetDevice().SetTextureAddressClamp(AS_DEPTHMAP);

	bool bHWFiltering = pShadowStrategy->GetNeedHWFiltering();
	REngine::GetDevice().SetTextureFilter(AS_DEPTHMAP, bHWFiltering ?  RTF_BILINEAR : RTF_POINT);

	RMatrix matView = pCamera->GetViewMatrix();
	RMatrix matViewInv = RMatrix::IDENTITY;

	//L-Stage������ View ���� ��ġ�� ����ϱ� ������ World ���������� ��ȯ�� �߰������ش�.
	matView.GetInverse(&matViewInv);

	if( REngine::GetSceneManager().GetRealtimeShadowMethod() == SHADOW_PSSM )
	{
		RPSSMShadowStrategy* pPSSM = (RPSSMShadowStrategy*)(pShadowStrategy);

		RMatrix matLightTransform[4];
		RMatrix matLightViewTransform;

		for(int i=0; i<pPSSM->GetSplitCount(); ++i)
		{
			pPSSM->SetCurrentCameraIndex(i);
			matLightTransform[i] = matViewInv * pPSSM->GetCurrentLightTransform();

			RMatrix matLightView = matViewInv * pPSSM->GetCurrentLightViewTransform();
			matLightViewTransform.m[i*4 +0] = matLightView._13;
			matLightViewTransform.m[i*4 +1] = matLightView._23;
			matLightViewTransform.m[i*4 +2] = matLightView._33;
			matLightViewTransform.m[i*4 +3] = matLightView._43;
		}

		pLightingShader->m_scLightTransformArray.SetMatrixArray( matLightTransform, pPSSM->GetSplitCount() );
		pLightingShader->m_scLightViewTransformArray.SetMatrix( matLightViewTransform );

		pLightingShader->m_scCSMSplitInfo.SetVector4Array(pPSSM->GetSplitInfo(), MAX_PSSM_SPLIT_COUNT);
		pLightingShader->m_scSplitCount.SetFloat(pPSSM->GetSplitCount());
		pLightingShader->m_scDepthBias.SetVector4(pPSSM->GetDepthBias());

		pPSSM->SetCurrentCameraIndex(0);
	}
	else
	{
		RMatrix matLightView = matViewInv * pShadowStrategy->GetCurrentLightViewTransform();
		pLightingShader->m_scLightViewTransform.SetVector4(RVector4(matLightView._13, matLightView._23, matLightView._33, matLightView._43));
		
		pLightingShader->m_scLightTransform.SetMatrix( matViewInv * pShadowStrategy->GetCurrentLightTransform());
		pLightingShader->m_scDepthBias.SetVector4(((RDepthMapShadowStrategy*)pShadowStrategy)->GetDepthBias());
	}		

	pLightingShader->m_scDepthTexelSize.SetFloat( 1.f / float(pShadowStrategy->GetCurrentDepthMapSize()) );
}

void RDirectionalLightPassRenderer::SetCloudRenderState( RCameraSceneNode* pCamera, RLightingShader* pLightingShader )
{
	if( !pCamera )
		return;

	_ASSERT( pLightingShader );

	REnvironmentSet *pEnvironmentSet = REngine::GetSceneManager().GetCurrentRootSceneNode(false)->GetEnvironmentSet();
	if( pEnvironmentSet && pEnvironmentSet->m_vecTilling.empty()==false)
	{
		RDevice *pDevice = REngine::GetDevicePtr();

		RMatrix matViewInv;
		pCamera->GetViewMatrix().GetInverse(&matViewInv);

		float fAlpha = 1.0f - pEnvironmentSet->m_vecTilling[0].fVisibility;

		pLightingShader->m_scCloudFactor.SetFloat(fAlpha);
		pLightingShader->m_scCloudTransform.SetMatrix(matViewInv*pEnvironmentSet->m_vecTilling[0].matTransform);

		if( pEnvironmentSet->m_vecTilling.size() > 1 )
		{
			float fAlpha2 = 1.0f - pEnvironmentSet->m_vecTilling[1].fVisibility;
			pLightingShader->m_scCloudFactor2.SetFloat(fAlpha2);
			pLightingShader->m_scCloudTransform2.SetMatrix(matViewInv*pEnvironmentSet->m_vecTilling[1].matTransform);
		}

		pDevice->SetTexture(AS_CLOUDMAP, pEnvironmentSet->hTillingTexture);
		pDevice->SetTextureAddressWrap(AS_CLOUDMAP);
		pDevice->SetTextureFilter(AS_CLOUDMAP, RTF_BILINEAR);
	}
}

void RDirectionalLightPassRenderer::SetRenderState(bool bDirLightOnlyGI, bool bShadowOnlyGI, RGIProcessor* pGIProcessor)
{
	RPFC_THISFUNC;
	REngine::GetDevice().SetBlending(RBLEND_ONE, RBLEND_ONE);
	REngine::GetDevice().SetDepthEnable(false, false);
	REngine::GetDevice().SetCullMode(RCULL_CCW);



	if( bDirLightOnlyGI)
	{
		_ASSERT(pGIProcessor);
		if(pGIProcessor)
		{
			REngine::GetDevice().SetTexture( 0, pGIProcessor->GetRTGI());
			REngine::GetDevice().SetTextureFilter( 0, RTF_LINEAR);
		}
	}
	if( bShadowOnlyGI)
	{
		_ASSERT(pGIProcessor);
		if(pGIProcessor)
		{
			REngine::GetDevice().SetTexture( 1, pGIProcessor->GetRTShadow());
			REngine::GetDevice().SetTextureFilter( 1, RTF_POINT);
		}
	}
}

void RDirectionalLightPassRenderer::RestoreRenderState()
{
	REngine::GetDevice().SetDepthEnable(true, true);
	REngine::GetDevice().SetBlending(RBLEND_NONE);	
	REngine::GetDevice().SetCullMode(RCULL_CW);
}

void RDirectionalLightPassRenderer::RenderSubLightOnly(RCameraSceneNode* pCamera, RLightingShader* pLightingShader, SCENE_SET* pSubLightList, DWORD dwWidth, DWORD dwHeight)
{
	RPFC_THISFUNC;
	int nSubLightCount = SetSubLightShaderConstant(pCamera, pLightingShader, pSubLightList);

	SetRenderState(false, false, NULL);
	{
		if( RShaderFX* pShader = pLightingShader->CreateShaderForSubLight(nSubLightCount) )
		{
			RTechnique tech = pShader->GetTechnique("SubLightTechnique");
			pLightingShader->RenderQuad( &tech, dwWidth, dwHeight);
		}
	}
	RestoreRenderState();
}

void RDirectionalLightPassRenderer::ExportShadow(RGIProcessor* pGIProcessor, RLightingShader* pLightingShader, RDirectionalLightSceneNode* pDirLight, DWORD dwWidth, DWORD dwHeight)
{
	RPFC_THISFUNC;
	RTexture* pOrgRT = REngine::GetDevice().SetRenderTarget( 0, pGIProcessor->GetRTShadow());

	bool bShadowFilter =true;
	bool bShadowLerp = true;

	REngine::GetDevice().SetDepthEnable(false, false);

	if( RShaderFX* pShader = pLightingShader->CreateDirectionalLightShader( pDirLight, false, false, true, false, false, bShadowFilter, bShadowLerp, 0) )
	{

		_ASSERT(pGIProcessor->GetRTShadow()->GetInfo().nTextureWidth==dwWidth && pGIProcessor->GetRTShadow()->GetInfo().nTextureHeight==dwHeight);
		pLightingShader->RenderQuad( &pShader->GetTechnique( "MainTechnique"), dwWidth, dwHeight);
	}


	REngine::GetDevice().SetRenderTarget( 0, pOrgRT);
}

}
