#include "stdafx.h"
#include "RTerrain.h"
#include "RTerrainPatch.h"
#include "RDeviceD3D.h"
#include "RProfileInfo.h"

#include "REnvironmentSet.h"
#include "RCameraSceneNode.h"
#include "RShadowStrategy.h"
#include "RTerrainShader.h"
#include "RShaderFX.h"

#include "RShaderCompositeManager.h"
#include "RUtil.h"
#include "RTerrainPassRenderer.h"
#include "RRenderHelper.h"



namespace rs3 {


////////////////////////////////////////////////////////////////////////////////////

void RTerrain::SetRenderState( RCameraSceneNode* pCurrentCamera, const RMatrix &viewMatrix, const RMatrix &projMatrix, RTerrainShader* pTerrainShader)
{
	RPFC_THISFUNC;
	RMatrix matWVP( (*m_pTerrain->m_matWorld) * viewMatrix * projMatrix );

	if( pTerrainShader)
	{
		if( pCurrentCamera->GetFogEnable())
		{
			float fNear = pCurrentCamera->GetFogNear();
			float fFar = pCurrentCamera->GetFogFar();
			float fDist = fFar - fNear;
			fDist = ( fDist == 0.0f ? 0 : 1.0f/fDist );
			pTerrainShader->m_scFogFactor.SetVector4(fNear, fFar, fDist, 0.0f);

			DWORD dwFogColor = pCurrentCamera->GetFogColor();
			RVector vecFogColor;
			DWColorToFloat3( &vecFogColor, dwFogColor );
			pTerrainShader->m_scFogColor.SetVector3( vecFogColor );

		}
		else
		{
			pTerrainShader->m_scFogFactor.SetVector4( 0, 0, 0, 0);
		}

		pTerrainShader->m_scWorld.SetMatrix((*m_pTerrain->m_matWorld));
		pTerrainShader->m_scWorldViewProj.SetMatrix( matWVP );
		pTerrainShader->m_scWorldView.SetMatrix( (*m_pTerrain->m_matWorld) * viewMatrix );
		pTerrainShader->m_scFarDist.SetFloat(pCurrentCamera->GetFarZ());
	}
}


void RTerrain::SetLayerMaskTexture( RTerrainPatch* pDrawPatch, int nLayerCount)
{
	RDevice *pDevice = REngine::GetDevicePtr();
	_ASSERT(pDevice);

	pDevice->SetTexture( TS_MASKMAP0, pDrawPatch->GetLayerMaskTexture(0));
	// �ι�° ����ũ�� ���̾� ī��Ʈ�� 6 �̻��� ���� ���δ�. 5������� ����ũ �ϳ��� Ŀ�� ����.
	if( nLayerCount > 5)
		pDevice->SetTexture( TS_MASKMAP1, pDrawPatch->GetLayerMaskTexture(1));
}


void RTerrain::SetLayerTileTexture( LayerInfo *pInfo, int nLayerIdx, bool bNormalMap)
{
	// Set Render State
	RDevice *pDevice = REngine::GetDevicePtr();
	_ASSERT(pDevice);

	pDevice->SetTexture( TS_DIFFUSEMAP0+nLayerIdx, pInfo->hTileMap);

	if( bNormalMap )
		pDevice->SetTexture( TS_NORMALMAP0+nLayerIdx , pInfo->hNormalMap);
}


void RTerrain::RenderSingleLayer( RTechnique* pTechnique, std::vector<RTerrainPatch*>& arrPatch, RTerrainShader* pShader)
{
	RPFC_THISFUNC;
	_ASSERT( pTechnique);

	// Set Render State
	RDevice *pDevice = REngine::GetDevicePtr();
	_ASSERT(pDevice);

	RShaderCompositeManager::m_cAlphaRef.SetFloat( 0);
	pDevice->SetTextureAddressClamp(TS_DIFFUSE);
	pDevice->SetTextureFilter(TS_DIFFUSE, RTF_BILINEAR);

	// Set Texture State
	pDevice->SetTexture(TS_DIFFUSE, m_pTerrain->m_hBaseMap);

	RMatrix matTransform;
	matTransform.MakeIdentity();
	matTransform._11 = 1.0f/m_pTerrain->GetWidthSize();
	matTransform._22 =-1.0f/m_pTerrain->GetHeightSize();
	matTransform._42 = 1.0f;
	if( pShader)
	{
		pShader->m_scTextureTransform.SetMatrix(matTransform);;
	}
	else
	{
		// Set Texture Transform
		RShaderCompositeManager::m_cTextureTransform.SetMatrix(matTransform);
		RShaderCompositeManager::m_cTextureFactor.SetVector4(1, 1, 1, 1);
	}


	pDevice->SetFvF( RTerrainPatch::RVt_Terrain::FVF );

	pTechnique->Begin(NULL, TECH_PRESERVE_NOT);
	pTechnique->BeginPass();

	// Draw Primitives
	//for ( size_t si = 0; si<m_pTerrain->m_arrCulledSimplePatch.size(); ++si)
	for ( size_t si = 0; si < arrPatch.size(); ++si)
	{
		//RTerrainPatch *pDrawPatch = m_pTerrain->m_arrCulledSimplePatch[si];
		RTerrainPatch *pDrawPatch = arrPatch[si];
		m_pTerrain->RenderTerrainPatch(pDrawPatch);
	}

	pTechnique->EndPass();
	pTechnique->End();

	if( pShader)
	{

	}
	else
	{
		RShaderCompositeManager::m_cTextureTransform.SetMatrix(RMatrix::IDENTITY);
	}
}

void RTerrain::RenderBakedMapMStage( RCameraSceneNode* pCurrentCamera, bool bLit, RTerrainShader* pShader)
{
	RPFC_THISFUNC;
	RShaderCompositeManager* pCompositeManager = REngine::GetSceneManagerPtr()->GetShaderCompositeManager();
	if(pCompositeManager->GetViewLightingOnly()) return;

	// Set Render State
	RDevice *pDevice = REngine::GetDevicePtr();
	_ASSERT(pDevice);

	RTechnique technique;
	if( pShader)
	{
		RShaderFX* pFX = pShader->GetShaderForBakedMapMStage( true);
		if( pFX)
		{
			technique = pFX->GetTechnique( "MainTechnique");
			RenderSingleLayer( &technique, m_pTerrain->m_arrCulledSimplePatch, NULL);
		}
	}
	
	
}


void RTerrain::RenderBakedMapGStage( RCameraSceneNode* pCurrentCamera, bool bFullMap, RTerrainShader* pShader)
{
	RPFC_THISFUNC;
	RShaderCompositeManager* pCompositeManager = REngine::GetSceneManagerPtr()->GetShaderCompositeManager();
	if(pCompositeManager->GetViewLightingOnly()) return;

	// Set Render State
	RDevice *pDevice = REngine::GetDevicePtr();
	_ASSERT(pDevice);

	// Draw Primitives
	RTechnique technique;
	if( pShader)
	{
		RShaderFX* pFX = pShader->GetShaderForBakedMapGStage( REngine::GetSceneManager().GetRenderingStrategy()->GetMRTCount(), false );
		if( pFX)
		{
			technique = pFX->GetTechnique( "MainTechnique");
			RenderSingleLayer( &technique, bFullMap ? m_pTerrain->m_arrCulledPatch : m_pTerrain->m_arrCulledSimplePatch, pShader);
		}
	}
}


void RTerrain::RenderGrid( RCameraSceneNode* pCurrentCamera, RTerrainShader* pShader)
{
	RPFC_THISFUNC;
	RShaderCompositeManager* pCompositeManager = REngine::GetSceneManagerPtr()->GetShaderCompositeManager();
	if(pCompositeManager->GetViewLightingOnly()) return;

	// Set Render State
	RDevice *pDevice = REngine::GetDevicePtr();
	_ASSERT(pDevice);

	// Draw Primitives
	RTechnique technique;
	if( pShader)
	{
		RShaderFX* pFX = pShader->GetShaderForGrid( 1, false );
		if( pFX)
		{
			// �׸��带 ���� ����
			RFILLMODE fillOrg = pDevice->GetFillMode();
			pDevice->SetFillMode( RFILL_WIRE );
			/// ���̾�� ���� ������ ��Ʈ������ ���� �� ���� ���δ�.
			/// ����ü ���̾�� ���� ������� �𸣰�����, �����ô붧���� M_toolTerrain�� �����ߴ� ���� �״�� ��������.
			pDevice->SetDepthBias( -0.000001f, -0.15f );
			technique = pFX->GetTechnique( "MainTechnique");
			RenderSingleLayer( &technique, m_pTerrain->m_arrCulledPatch, pShader);
			// ���� ���� ����
			pDevice->SetFillMode( fillOrg);
			pDevice->SetDepthBias( 0, 0 );
		}
	}
}


void RTerrain::RenderMarkingLayer( RCameraSceneNode* pCurrentCamera, RTerrainShader* pTerrainShader, int nMarkingLayerNumber)
{
	RPFC_THISFUNC;

	// Set Render State
	RDevice *pDevice = REngine::GetDevicePtr();
	_ASSERT(pDevice);

	RShaderCompositeManager* pCompositeManager = REngine::GetSceneManagerPtr()->GetShaderCompositeManager();
	if(pCompositeManager->GetViewLightingOnly())
		return;

	CheckAndCreateMaskTexture( m_pTerrain->m_arrCulledPatch, pTerrainShader);
	SetRenderStateForSplattingLayerOnePass();
	pDevice->SetBlending( RBLEND_ONE, RBLEND_ONE);	/// ����ŷ ��忡���� Add Color
	RenderMarkingLayerOnePass( m_pTerrain->m_arrCulledPatch, pTerrainShader, 1, nMarkingLayerNumber);
	pDevice->SetBlending( RBLEND_NONE, RBLEND_NONE);
	RestoreRenderState();

}


void RTerrain::RenderSplattingLayerOnePassMStage( RCameraSceneNode* pCurrentCamera, RTerrainShader* pTerrainShader)
{
	RDeviceD3D *pDevice = (RDeviceD3D*)REngine::GetDevicePtr();
	_ASSERT(pDevice);

	RShaderCompositeManager* pCompositeManager = REngine::GetSceneManagerPtr()->GetShaderCompositeManager();
	if(pCompositeManager->GetViewLightingOnly())
	{
		return;
	}


	CheckAndCreateMaskTexture( m_pTerrain->m_arrCulledDetailHighPatch, pTerrainShader);
	SetRenderStateForSplattingLayerOnePass();
	RenderSplattingLayerOnePass( true, m_pTerrain->m_arrCulledDetailHighPatch, pTerrainShader, REngine::GetSceneManager().GetRenderingStrategy()->GetMRTCount(), TTL_COUNT, -1);
	RestoreRenderState();
}


int RTerrain::GetMaskTextureCount( int nLayerCount)
{
	int nMaskTextureCount = 0;
	/// ���̽� Ÿ�� �����Ͽ� 5����� ����ũ �������� Ŀ�� ����
	if( nLayerCount <= 5)
		nMaskTextureCount = 1;
	/// ������ 9����� Ŀ�� �����ϳ� �븻���� �����صξ� 7������ ����
	else if( nLayerCount <= MAX_LAYER_PER_PATCH)	
		nMaskTextureCount = 2;
	/// ��� ���̾� ���� ������ ���� ������� ���� �ض�
	else
		nMaskTextureCount = 0;

	return nMaskTextureCount;
}


void RTerrain::ClearMaskTexture()
{
	for ( int si = 0; si< (GetWidthCount()*GetHeightCount()); ++si )
	{
		ClearMaskTexture( si);
	}
}


void RTerrain::ClearMaskTexture( int nPatch)
{
	if( m_pPatches && nPatch < (GetWidthCount()*GetHeightCount()))
	{
		RTerrainPatch *pDrawPatch = &m_pPatches[nPatch];
		pDrawPatch->DeleteLayerMaskTexture();
	}
}


void RTerrain::MakeMaskTexture( int nPatchIdx, RTerrainShader* pTerrainShader)
{
	RPFC_THISFUNC;

	if( !m_pPatches || (nPatchIdx>=m_iPatchNum) || !m_pLayerInfo || !pTerrainShader)
		return;

	if(!m_pPatches[nPatchIdx].IsNeedUpdateLayerMaskTexture())
		return;
	
	int nLayerIdx = 0;

	DWORD dwMaxLayerWidth = 0;
	DWORD dwMaxLayerHeight = 0;
	
	DWORD dwMaxWidth = 0;
	DWORD dwMaxHeight = 0;

	int nLayerCount = 0;
	for( nLayerIdx = 0; nLayerIdx<m_iLayerNum; nLayerIdx++)
	{
		/// m_pLayerInfo[].phAlphaMask[]�� �˻��ϴ� ���� �ٷ� Ȯ�� ����������, one-pass�� ���� �� phAlphaMask�� �ʿ䰡 ����. 
		/// IsAlphaTextureUsed(DataRaw�� ���� �� ���� �÷��� ���� ��.)�� ���� üũ �Ѵ�. 
		if ( IsAlphaTextureUsed( nPatchIdx, nLayerIdx))
		{
			LayerInfo *pInfo = &(m_pLayerInfo[nLayerIdx]);
			++nLayerCount;
			// ���Ǵ� ���̾� �� �ִ� �ػ��� ���� ���Ͽ� �� �������� ���� ����ũ�� ������ �Ѵ�. 
			dwMaxLayerWidth = max( pInfo->dwAlphaPatchWidth, dwMaxLayerWidth);
			dwMaxLayerHeight = max( pInfo->dwAlphaPatchHeight, dwMaxLayerHeight);
			dwMaxWidth = max( pInfo->dwAlphaWidth, dwMaxWidth);
			dwMaxHeight = max( pInfo->dwAlphaHeight, dwMaxHeight);
		}
	}

	int nMaskTextureCount = GetMaskTextureCount(nLayerCount);

	/// �ؽ��� ���� �� ����. ������ nMaskTextureCount��ŭ ������ �ؾ������� ����� 9�� ������ ���� ������ 2�� �����.
	/// 8�� ���� �����̸� �ι�° ����ŷ �ؽ��ĸ� NULL�� �����Ͽ��� ������ ������
	/// 9�� ���� �� �ι�° ����ŷ �ؽ��ĸ� NULL�� �����ϸ� 9��° Ÿ���� ����ŷ�� mask1.a�� 1�� ������ �ȴ�.
	if( m_pPatches[nPatchIdx].CreateLayerMaskTexture( nMaskTextureCount, dwMaxLayerWidth, dwMaxLayerHeight))
	{
		RTexture* pTargetOrg0 = REngine::GetDevice().GetRenderTarget(0);
		RTexture* pTargetOrg1 = REngine::GetDevice().GetRenderTarget(1);
		RTexture* pTargetOrg2 = REngine::GetDevice().SetRenderTarget(2, NULL);
		RTexture* pTargetOrg3 = REngine::GetDevice().SetRenderTarget(3, NULL);

		/// MRT�� ������ ���ÿ� ����� ����. ����ũ�� �ϳ��� ���δٸ� RT1�� NULL�� �� ���̹Ƿ� �Ű� ���� �ʾƵ� �� �� ����.
		REngine::GetDevice().SetRenderTarget( 0, m_pPatches[nPatchIdx].GetLayerMaskTexture(0));
		REngine::GetDevice().SetRenderTarget( 1, m_pPatches[nPatchIdx].GetLayerMaskTexture(1));
		// �Է� �ҽ��� �� �ؽ��� ������������ �����.
		for( int i=0; i<MAX_LAYER_PER_PATCH-1; i++)
		{
			RDeviceD3D *pDevice = (RDeviceD3D*)REngine::GetDevicePtr();
			_ASSERT( pDevice);
			REngine::GetDevice().SetTexture( i, NULL);
			REngine::GetDevice().SetTextureAddressClamp(i);
			REngine::GetDevice().SetTextureFilter( i, RTF_BILINEAR);
			REngine::GetDevice().SetTextureMaxAnisotropy( i, 1);
		}
		// ���� �ؽ��ĵ��� �Է� �ҽ���
		int nCurrStage = 0;
		bool bFirstLayerSkipped = false;
		for( nLayerIdx = 0; nLayerIdx<m_iLayerNum; nLayerIdx++)
		{
			LayerInfo *pInfo = &(m_pLayerInfo[nLayerIdx]);
			if( IsAlphaTextureUsed( nPatchIdx, nLayerIdx))
			{
				// ù��°(���̽�)�� ���Ǵ� Ÿ�� ���Ĵ� ��ŵ. �������� ��� ������ ����ġ 1�� �԰� �Ǳ� �����̴�.
				if( nCurrStage == 0 && !bFirstLayerSkipped)
				{
					bFirstLayerSkipped = true;
					continue;
				}
				/// ���� �ؽ��� ������Ʈ�� �ʿ��ϸ� �׶� ������Ʈ�� ������.
				/// ������Ʈ �ʿ� ������ ���
				/// 1. ������ ó�� ��. �ʱⰪ�� T
				/// 2. pAlphaTextureRaw�� ���� �� ���. pAlphaTextureRaw�� ���� �ϴ� ������ T�� ���� ��.
				if( pInfo->bNeedUpdateAlphaTexture)
				{
					/// ��ü �������� �ؽ��İ� ���� �ϸ� ũ�⸦ ���Ͽ� ��Ȱ�� ���θ� �����Ͽ� ����
					if( pInfo->pAlphaTexture)
					{
						RTextureInfo pAlphaTexture = pInfo->pAlphaTexture->GetInfo();
						if( pAlphaTexture.nTextureWidth != pInfo->dwAlphaWidth || pAlphaTexture.nTextureHeight != pInfo->dwAlphaHeight)
						{
							REngine::GetDevice().DeleteTexture(pInfo->pAlphaTexture);
							pInfo->pAlphaTexture = NULL;
						}
					}
					/// �ؽ��İ� ������ �ؽ��ĸ� ����
					if( !pInfo->pAlphaTexture)
					{
						RFORMAT alphaTextureFormat = RFMT_ARGB32;
						pInfo->pAlphaTexture = REngine::GetDevice().CreateTexture( pInfo->dwAlphaWidth, pInfo->dwAlphaHeight, alphaTextureFormat);
					}
					/// raw �����͸� ����
					if(pInfo->pAlphaTexture)
						REngine::GetDevice().UpdateTexture( pInfo->pAlphaTexture, 0, 0, 0, pInfo->dwAlphaWidth, pInfo->dwAlphaHeight, 0, 0, pInfo->dwAlphaWidth, pInfo->dwAlphaHeight, (void*)(pInfo->pAlphaTextureRaw));
					/// ó�� �Ϸ� �÷���
					pInfo->bNeedUpdateAlphaTexture = false;
				}
				/// ��ü �������� �ؽ��ĸ� �����Ͽ� ��ġ�� �ش��ϴ� �Ϻθ� �׸��� ����.
				REngine::GetDevice().SetTexture( nCurrStage, pInfo->pAlphaTexture);

				nCurrStage++;
				// �ؽ��� �������� 6�� �� ���� �Ǿ����� ���� ����. �� ��� ���̾�� 7�������� ���̽� ���̾��� ���Ĵ� ���� ���� �����Ƿ� 6������ üũ
				if( nCurrStage >= MAX_LAYER_PER_PATCH-1)
					break;
			}	
		}

		// ��ü �ؽ����� �Ϻθ� ������ �ϴ� ���̹Ƿ� UV�� ����Ͽ� �׷���� ��.
		float fStartX = (float)m_pPatches[nPatchIdx].GetX() / (float)GetWidthCount();
		float fStartY = (float)m_pPatches[nPatchIdx].GetY() / (float)GetHeightCount();
		float fGapX = 1.0f / (float)GetWidthCount();
		float fGapY = 1.0f / (float)GetHeightCount();
		// �ؽ��Ĵ� �����¿� �� �ȼ����� ������ �ξ� ������� �����Ƿ� �� ���� �ݿ��ؾ� ��.
		float fMarginX = 1.0f / (float)dwMaxWidth;
		float fMarginY = 1.0f / (float)dwMaxHeight;

		REngine::GetDevice().SetDepthEnable(false, false);
		//REngine::GetDevice().SetCullMode(RCULL_NONE);

		RTechnique tech = pTerrainShader->GetShaderForMakeMask()->GetTechnique("MainTechnique");
		pTerrainShader->m_scLayerCount.SetInt( nLayerCount);
		tech.Begin(NULL, TECH_PRESERVE_NOT);
		tech.BeginPass();
		RRenderHelper::RenderQuad(0, 0, (float)dwMaxLayerWidth, (float)dwMaxLayerHeight, fStartX-fMarginX, fStartY-fMarginY, fStartX+fGapX+fMarginX, fStartY+fGapY+fMarginY);
		tech.EndPass();
		tech.End();

		REngine::GetDevice().SetDepthEnable(true, true);
		REngine::GetDevice().SetCullMode(RCULL_CW);

		REngine::GetDevice().SetRenderTarget( 0, pTargetOrg0);
		REngine::GetDevice().SetRenderTarget( 1, pTargetOrg1);
		REngine::GetDevice().SetRenderTarget( 2, pTargetOrg2);
		REngine::GetDevice().SetRenderTarget( 3, pTargetOrg3);

		// �ؽ��� ������������ �⺻ ��������
		for( int i=0; i<MAX_LAYER_PER_PATCH-1; i++)
		{
			REngine::GetDevice().SetTextureAddressWrap(i);
			REngine::GetDevice().SetTextureFilter( i, REngine::GetConfig().GetTextureFilter() );
			REngine::GetDevice().SetTextureMaxAnisotropy( i, REngine::GetConfig().GetTextureMaxAnisotropy() );
		}
	}

	m_pPatches[nPatchIdx].NeedUpdateLayerMaskTexture(false);
}


void RTerrain::CheckAndCreateMaskTexture( std::vector<RTerrainPatch*>& arrPatch, RTerrainShader* pTerrainShader)
{
	RPFC_THISFUNC;

	RDeviceD3D *pDevice = (RDeviceD3D*)REngine::GetDevicePtr();
	_ASSERT(pDevice);

	// one pass ������ ���� ���̴��� �����ϰ� �������δ� �������� ������ �ݵ�� �ʿ�.
	if( !pTerrainShader)
		return;

	/// ����ũ �ؽ��� ������Ʈ
	for (size_t si = 0; si < arrPatch.size(); ++si)
	{
		RTerrainPatch *pDrawPatch = arrPatch[si];
		int nPatchIdx = pDrawPatch->GetY() * m_pTerrain->GetWidthCount() + pDrawPatch->GetX(); 
		/// ��ġ�� ���� ����ũ �ؽ��İ� ������Ʈ�� �ʿ��ϸ� ���ش�.
		/// �ʿ� ��� 1 : ������ ���ʿ� �Ǿ� ���� ���� ��
		/// �ʿ� ��� 2 : �� ��� ������Ʈ �ʿ� �÷��׸� ���� �� ���
		if( pDrawPatch->IsNeedUpdateLayerMaskTexture() && pTerrainShader->GetShaderForMakeMask())
		{
			// ��ġ�� ��� ���̾� ������ ������ ����ũ �ؽ��� ����
			MakeMaskTexture( nPatchIdx, pTerrainShader);
		}
	}

	// MakeMaskTexture���� ���� ���带 ȣ���Ͽ� FVF�� ����ǹǷ� �ٽ� �ͷ����� ������ ��������.
	//pDevice->SetFvF(RTerrainPatch::RVt_Terrain::FVF);
}


void RTerrain::SetRenderStateForSplattingLayerOnePass()
{
	RDeviceD3D *pDevice = (RDeviceD3D*)REngine::GetDevicePtr();
	_ASSERT(pDevice);

	pDevice->SetTextureFilter(TS_MASKMAP0, RTF_BILINEAR);
	pDevice->SetTextureFilter(TS_MASKMAP1, RTF_BILINEAR);
	pDevice->SetTextureAddressClamp(TS_MASKMAP0);
	pDevice->SetTextureAddressClamp(TS_MASKMAP1);

	// NVIDIA ���忡 ����. ATI ����.
	for( int i=0; i<MAX_LAYER_PER_PATCH; i++)
	{
		pDevice->SetTextureAddressWrap(TS_DIFFUSEMAP0+i);
		pDevice->SetTextureAddressWrap(TS_NORMALMAP0+i);
		pDevice->SetTextureFilter( TS_DIFFUSEMAP0+i, REngine::GetConfig().GetTextureFilter() );
		pDevice->SetTextureMaxAnisotropy( TS_DIFFUSEMAP0+i, REngine::GetConfig().GetTextureMaxAnisotropy() );
		pDevice->SetTextureFilter( TS_NORMALMAP0+i, REngine::GetConfig().GetTextureFilter() );
		pDevice->SetTextureMaxAnisotropy( TS_NORMALMAP0+i, REngine::GetConfig().GetTextureMaxAnisotropy() );

	}

	pDevice->SetFvF(RTerrainPatch::RVt_Terrain::FVF);
}


void RTerrain::RestoreRenderState()
{
	RDeviceD3D *pDevice = (RDeviceD3D*)REngine::GetDevicePtr();
	_ASSERT(pDevice);

// 	/// restore to default value
// 	if ( m_pTerrain->m_bAnisotropyFilter )
// 	{
// 		// NVIDIA ���忡 ����. ATI ����.
// 		for( int i=0; i<MAX_LAYER_PER_PATCH; i++)
// 		{
// 			pDevice->SetTextureFilter(TS_DIFFUSEMAP0+i, RTF_TRILINEAR);
// 			pDevice->SetTextureFilter(TS_NORMALMAP0+i, RTF_TRILINEAR);
// 			pDevice->SetTextureMaxAnisotropy( TS_DIFFUSEMAP0+i, 1);
// 			pDevice->SetTextureMaxAnisotropy( TS_NORMALMAP0+i, 1);
// 		}
// 	}

	pDevice->SetTextureAddressWrap(TS_MASKMAP0);
	pDevice->SetTextureAddressWrap(TS_MASKMAP1);
}


void RTerrain::RenderMarkingLayerOnePass( std::vector<RTerrainPatch*>& arrPatch, RTerrainShader* pTerrainShader, int nRenderTargetCount, int nMarkinglayer)
{
	RenderSplattingLayerOnePass( false, arrPatch, pTerrainShader, nRenderTargetCount, TTL_COUNT, nMarkinglayer);
}


void RTerrain::RenderSplattingLayerOnePass( bool bLighting, std::vector<RTerrainPatch*>& arrPatch, RTerrainShader* pTerrainShader, int nRenderTargetCount, TERRAIN_TILE_LEVEL tileLevel, int nMarkingLayer)
{
	RPFC_THISFUNC;

	RDeviceD3D *pDevice = (RDeviceD3D*)REngine::GetDevicePtr();
	_ASSERT(pDevice);

	// one pass ������ ���� ���̴��� �����ϰ� �������δ� �������� ������ �ݵ�� �ʿ�.
	if( !pTerrainShader)
		return;

	RTechnique OldTechnique;

	/// ��� ���� ����� ���̱� ���� ��ȭ�� �ִ� �� �� �� �ݿ�.
	/// RShaderConst������ �迭�� �ߺ� üũ�� ���� �ʴ´�. �迭�� �ߺ��Ͽ� ���� ���� ��� �� ���� ���� ����, ������ �ߺ� üũ�� �� ���ش�,
	/// ������, ���� ���÷��� ���� ������ ��ĥ ���� ���� �����Ѵ�. �̸� ���� RShaderConst ���ο��� �ߺ� üũ�� �ϱ⺸�ٴ� �������� �˾Ƽ� �� ����.
	RVector4 vTileTextureScaleAndTranslateArrayPrev[MAX_LAYER_PER_PATCH];
	for( int i=0; i<MAX_LAYER_PER_PATCH; ++i)
	{
		vTileTextureScaleAndTranslateArrayPrev[i] = RVector4(-1,-1,-1,-1);	/// ���� �� �� ���� ��
	}

	for (size_t si = 0; si < arrPatch.size(); ++si)
	{
		RTerrainPatch *pDrawPatch = arrPatch[si];
		int nPatchIdx = pDrawPatch->GetY() * m_pTerrain->GetWidthCount() + pDrawPatch->GetX(); 

		if( pDrawPatch->GetLayerMaskTexture(0))
		{
			int nLayerIdx=0;
			int nLayerCount=0;
			bool bMarkingSetted=0;	/// ���������� ���� �ȵǴ� �ܾ����� �ǹ̸� ���������� �ϱ� ���� ed�� ���� ��
			RVector4 vTileTextureScaleAndTranslateArray[MAX_LAYER_PER_PATCH];
			int nSpecularArray[MAX_LAYER_PER_PATCH];
			int nNormalmapArray[MAX_LAYER_PER_PATCH];
			for( nLayerIdx = 0; nLayerIdx<m_iLayerNum; nLayerIdx++)
			{
				if ( IsAlphaTextureUsed( nPatchIdx, nLayerIdx) )
				{
					LayerInfo *pInfo = &(m_pLayerInfo[nLayerIdx]);
					bool bNormalMap = (tileLevel == TTL_DETAIL_HIGH && pInfo->hNormalMap && REngine::GetSceneManager().GetShaderCompositeManager()->GetUseNormalMap());
					/// Ÿ�ϸ� ���� (��ǻ��, �븻) �ؽ��� ����
					SetLayerTileTexture( pInfo, nLayerCount, bNormalMap);
					/// Ÿ�� �ؽ��� �����ϸ��� ���̾� ���� �ٸ���.
					vTileTextureScaleAndTranslateArray[nLayerCount] = RVector4( 1.0f/pInfo->vTilingUV.x, 1.0f/pInfo->vTilingUV.y, 0, 0);
					nSpecularArray[nLayerCount] = pInfo->bAlphaMaskForSpec;
					nNormalmapArray[nLayerCount] = bNormalMap;

					/// ��ŷ �� ���̾� ��ġ ���� �ε����� ����. 
					if( nMarkingLayer == nLayerIdx)
					{
						pTerrainShader->m_csMarkinglayer.SetFloat( nLayerCount);
						bMarkingSetted = true;
					}

					// �븻�� ����� �����صξ� 7���� �ؽ��� ���̾� ����� ����Ʈ~
					if( ++nLayerCount >= MAX_LAYER_PER_PATCH)
						break;
				}
			}
			/// 
			if( nMarkingLayer>=0 && !bMarkingSetted)
				pTerrainShader->m_csMarkinglayer.SetFloat( -1);

			/// ����ũ �ؽ��� ���� ����
			SetLayerMaskTexture( pDrawPatch, nLayerCount);

			if( nLayerCount > 0)
			{
				/// Ÿ�ϸ� UV�� ���̾� ���� �ٸ���.
				if( memcmp( vTileTextureScaleAndTranslateArray, vTileTextureScaleAndTranslateArrayPrev, sizeof(RVector4)*MAX_LAYER_PER_PATCH) != 0)
					pTerrainShader->m_scTileTextureScaleAndTranslateArray.SetVector4Array( vTileTextureScaleAndTranslateArray, nLayerCount);
				memcpy( vTileTextureScaleAndTranslateArrayPrev, vTileTextureScaleAndTranslateArray, sizeof(RVector4)*MAX_LAYER_PER_PATCH);	/// �� ����
				/// ����ŧ�� ��� ������ ���̾� ���� �ٸ���.
				//if( memcmp( nSpecularArray, nSpecularArrayPrev, sizeof(int)*MAX_LAYER_PER_PATCH) != 0)
				//	pTerrainShader->m_scSpecularArray.SetIntArray( nSpecularArray, nLayerCount);
				//memcpy( nSpecularArrayPrev, nSpecularArray, sizeof(int)*MAX_LAYER_PER_PATCH);	/// �� ����
				/// ���� �ؽ��Ĵ� ���� ū ������ �������� ����. ��, ����ŷ �ؽ����� ���� �״�� ����ϸ� �� ��.
				RTextureInfo textureInfo = pDrawPatch->GetLayerMaskTexture(0)->GetInfo();
				RVector2 vAlphaUVOffset(1.0f/(float)textureInfo.nTextureWidth, 1.0f/(float)textureInfo.nTextureHeight);
				RVector4 vAlphaTextureScaleAndTranslate( 1.0f - vAlphaUVOffset.x*2.f, 1.0f - vAlphaUVOffset.y*2.f, vAlphaUVOffset.x, vAlphaUVOffset.y);
				pTerrainShader->m_scAlphaTextureScaleAndTranslate.SetVector4( vAlphaTextureScaleAndTranslate);	/// Vector4�� ���μ� �ߺ� üũ ��.

				//RTechnique* CurrentTechnique = &TechniqueArray[nLayerCount-1]; 
				RTechnique CurrentTechnique;
				if( RShaderFX* pFX = pTerrainShader->GetShaderForSplattingLayerOnePass( bLighting, nLayerCount, nRenderTargetCount, nSpecularArray, nNormalmapArray, (bool)(nMarkingLayer>=0)))
				{
					if( pFX)
						CurrentTechnique = pFX->GetTechnique( "MainTechnique");
				}

				if( false == OldTechnique.IsEqual( CurrentTechnique ) )
				{
					if(OldTechnique.m_hTechnique)
					{
						OldTechnique.EndPass();
						OldTechnique.End();
					}
					CurrentTechnique.Begin(NULL, TECH_PRESERVE_NOT);
					CurrentTechnique.BeginPass();
				}
				
				CurrentTechnique.CommitChanges();
				m_pTerrain->RenderTerrainPatch(pDrawPatch);
				OldTechnique = CurrentTechnique;
			}
		}
	}

	// ��ũ�� ������
	if(OldTechnique.m_hTechnique)
	{
		OldTechnique.EndPass();
		OldTechnique.End();
	}

}



/// �̰��� ������ ���� deferred G stage ���� ��...
void RTerrain::RenderSplattingLayerOnePassGStage( RCameraSceneNode* pCurrentCamera, RTerrainShader* pTerrainShader)
{
	RPFC_THISFUNC;

	RDeviceD3D *pDevice = (RDeviceD3D*)REngine::GetDevicePtr();
	_ASSERT(pDevice);

	if(!pTerrainShader)
		return;

	RShaderCompositeManager* pCompositeManager = REngine::GetSceneManagerPtr()->GetShaderCompositeManager();
	if(pCompositeManager->GetViewLightingOnly())
	{
		return;
	}

	// ���� ����ũ üũ �� ����
	CheckAndCreateMaskTexture( m_pTerrain->m_arrCulledDetailHighPatch, pTerrainShader);
	CheckAndCreateMaskTexture( m_pTerrain->m_arrCulledDetailLowPatch, pTerrainShader);
	// ��ġ ���� ����
	SetRenderStateForSplattingLayerOnePass();
	RenderSplattingLayerOnePass( false, m_pTerrain->m_arrCulledDetailHighPatch, pTerrainShader, REngine::GetSceneManager().GetRenderingStrategy()->GetMRTCount(), TTL_DETAIL_HIGH, -1);
	RenderSplattingLayerOnePass( false, m_pTerrain->m_arrCulledDetailLowPatch, pTerrainShader, REngine::GetSceneManager().GetRenderingStrategy()->GetMRTCount(), TTL_DETAIL_LOW, -1);
	RestoreRenderState();
}













// �ͷ����� ���� ��η� �׸��� �Ǹ鼭 ������ ������ ������...
void RTerrain::RenderDepthInfo()
{
	_ASSERT( "�����忡���� ���ư��� �ſ�. ���� ������ �����Ŵ� ��ƾ ��� ������BOA��");
}

};