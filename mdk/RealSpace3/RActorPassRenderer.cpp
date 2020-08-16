#include "stdafx.h"

#include "RActorPassRenderer.h"
#include "RSceneManager.h"
#include "RActor.h"
#include "RMeshNode.h"
#include "RMaterial.h"
#include "RActorNode.h"
#include "RCameraSceneNode.h"
#include "RProfileInfo.h"
#include "RShaderCompositeManager.h"
#include "RShaderFX.h"
#include "RDirectionalLightSceneNode.h"
#include "mmsystem.h"
#include "RPostEffectDistortion.h"
#include "RShadowStrategy.h"
#include "RShaderEffectController.h"
#include "RUtil.h"
#include "RActorNodeRenderStrategy.h"


namespace rs3 {

RActorNodeInstancingBuffer	RActorPassRenderer::m_InstancingBuffer;	// �켱�� ��� �ν��Ͻ��� �ϳ� ����
RMatrix*	RActorPassRenderer::m_pInstancingData = NULL;


RActorPassRenderer::RActorPassRenderer()
: m_bRender(true)
{
}

void RActorPassRenderer::Init()
{
	m_InstancingBuffer.Create( REngine::GetDevicePtr());

	if(!m_pInstancingData)
		m_pInstancingData = new RMatrix[ m_InstancingBuffer.GetMaxSize()];
}

void RActorPassRenderer::Destroy()
{
	m_InstancingBuffer.Destroy();

	SAFE_DELETE_ARRAY( m_pInstancingData);
}

void RActorPassRenderer::RenderDepthOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pCulledList, RShadowStrategy* pShadowStrategy )
{
	RPFC_THISFUNC;
	if ( m_bRender == false )
		return;

	_ASSERT(pShadowStrategy);

	RenderShadowMap(pCurrentCamera, pShadowStrategy, false);
}

void RActorPassRenderer::RenderDoubleShadowDepthOnPass( RCameraSceneNode* pCurrentCamera, RShadowStrategy* pShadowStrategy )
{
	RPFC_THISFUNC;

	if ( m_bRender == false )
		return;

	_ASSERT(pShadowStrategy);

	RenderShadowMap(pCurrentCamera, pShadowStrategy, true);
}

void RActorPassRenderer::RenderShadowMap(RCameraSceneNode* pCurrentCamera, RShadowStrategy* pShadowStrategy, bool bDoubleShadowPass )
{
	RPFC_THISFUNC;

	RActorNodeRenderStrategyShadowMap DepthShadowRenderStrategy;
	RActorNodeRenderStrategyProjectionShadowMap ProjectionRenderStrategy;

	RActorNodeRenderStrategyShadowMap* pRenderStrategy = 
		pShadowStrategy->GetShadowDesc().eMethod==SHADOW_PROJECTION ?
		&ProjectionRenderStrategy : &DepthShadowRenderStrategy;

	pRenderStrategy->SetShadowStrategy(pShadowStrategy);

	if(bDoubleShadowPass)
		RenderOnPass(pCurrentCamera, pRenderStrategy, pCurrentCamera->GetViewFrustumCullingMgr()->GetActorNodeDoubleShadowChain());
	else
		RenderOnPass(pCurrentCamera, pRenderStrategy, pCurrentCamera->GetViewFrustumCullingMgr()->GetActorNodeShadowChain());
}

void RActorPassRenderer::RenderMStageOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, RRENDERINGMETHOD renderMethod)
{
	// 	TIMEPROFILE_THIS_FUNCTION;
	RPFC_THISFUNC;

	RRenderChain<RActorNodeKey>* pActorNodeRenderChain = pCurrentCamera->GetViewFrustumCullingMgr()->GetActorNodeChain();

	switch( renderMethod)
	{
	case RENDERING_INFERRED:
		{
			RActorNodeRenderStrategyMStageInferred RenderStrategy;
			RenderOnPass( pCurrentCamera, &RenderStrategy, pActorNodeRenderChain);
		}
		break;
	case RENDERING_DEFERRED:
		{
			RActorNodeRenderStrategyMStageAddColor RenderStrategy;
			RenderOnPass( pCurrentCamera, &RenderStrategy, pActorNodeRenderChain);
		}
		break;
	}

	RPFC_GPU_CALL;
}


void RActorPassRenderer::RenderGStageOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, RRENDERINGMETHOD renderMethod)
{
	// 	TIMEPROFILE_THIS_FUNCTION;
	RPFC_THISFUNC;

	RRenderChain<RActorNodeKey>* pActorNodeRenderChain = pCurrentCamera->GetViewFrustumCullingMgr()->GetActorNodeChain();

	switch( renderMethod)
	{
	case RENDERING_INFERRED:
		{
			RActorNodeRenderStrategyGStageInferred RenderStrategy;
			RenderOnPass( pCurrentCamera, &RenderStrategy, pActorNodeRenderChain);
		}
		break;
	case RENDERING_DEFERRED:
		{
			if(REngine::GetSceneManager().GetRenderingStrategy()->GetRenderingMode()==RRM_DIFFUSE_ONLY)
			{
				RActorNodeRenderStrategyDiffuseOnly RenderStrategy;
				RenderOnPass( pCurrentCamera, &RenderStrategy, pActorNodeRenderChain);
			}
			else
			{
				RActorNodeRenderStrategyGStageDeferred RenderStrategy;
				RenderOnPass( pCurrentCamera, &RenderStrategy, pActorNodeRenderChain);
			}
		}
		break;
	}

	RPFC_GPU_CALL;
}


// �ν��Ͻ� ���� ����� ������Ʈ �ϱ�
void RActorPassRenderer::SetInstancingConstant(int nCnt)
{
	RShaderCompositeManager::m_cInstancingMatrices.SetMatrixArray( m_pInstancingData, nCnt);
}


void RActorPassRenderer::SetActorNodeInstancingData(int nIdx, RActorNode* pNode)
{
	PFC_THISFUNC;

	if(!pNode || !m_pInstancingData || nIdx >= m_InstancingBuffer.GetMaxSize() || nIdx < 0)
		return;

	RMatrix mxWorld = pNode->GetWorldTransform();
	RVector4 vColor = RVector4( pNode->m_dwPartsColor);


	// �����Ϳ� ����. ��Ʈ������ 14,24,34,44�� 0,0,0,1�̶� ���� �ϰ� �� �ڸ��� �÷� ������ ä��
	mxWorld._14 = vColor.x;
	mxWorld._24 = vColor.y;
	mxWorld._34 = vColor.z;
	mxWorld._44 = pNode->m_pActor->GetAO() ? 0.0f : 1.0f ;
	m_pInstancingData[nIdx] = mxWorld;
}


void RActorPassRenderer::RenderInstancingNode( RActorNode* pActorNode, RCameraSceneNode* pCurrentCamera, RActorNodeRenderStrategy* pActorRenderStrategy, 
									 RTechnique& currentTechnique, int& nInstanceCnt,
									 RActorNode** ppInstancingActorNode, int index)
{
	PFC_THISFUNC;

	// �ν��Ͻ��� ������ ���� �ν��Ͻ� ���ۿ� �����͸� �����ϰ�
	// ������ ������ ���ڶ�� ���� �� ���� ���( ���� ����� �ʰ� ī���͸� �ʱ�ȭ).
	SetActorNodeInstancingData(nInstanceCnt, pActorNode);
	++nInstanceCnt;
	*ppInstancingActorNode = pActorNode;

	if( nInstanceCnt >= m_InstancingBuffer.GetMaxSize())
	{
		RenderNode(pCurrentCamera, pActorRenderStrategy, pActorNode, nInstanceCnt, currentTechnique, index, true);

		nInstanceCnt = 0;
		*ppInstancingActorNode = NULL;
	}
}

void RActorPassRenderer::RenderNode(RCameraSceneNode* pCamera, RActorNodeRenderStrategy* pActorRenderStrategy, 
												 RActorNode* pActorNode, int nInstanceCnt, RTechnique& currentTechnique, int index, bool bAbleInstancing)
{
	RPFC_THISFUNC;
	pActorNode->SetVertexStream(false);

	if(bAbleInstancing)
	{
		pActorRenderStrategy->ResetNodeRenderState();

		// �����Ͱ� ��� �迭�� ���̴� ����� ���� �� commit
		SetInstancingConstant( min( nInstanceCnt, m_InstancingBuffer.GetMaxSize()));
	}
	else
	{
		// �ν��Ͻ� ���� �ô� ��� �� ���� ������Ʈ ���� ����
		RMaterial* pMaterial = index<0 ? NULL : pActorNode->GetSubMaterial(index);
		pActorRenderStrategy->SetNodeRenderState( pCamera, pActorNode, pMaterial);
	}


	SetupForRenderInstancing( &m_InstancingBuffer, nInstanceCnt);

	RShaderFX* pShader = pActorRenderStrategy->GetShader(pActorNode, index, bAbleInstancing);
	if( BeginTechnique(currentTechnique, pShader) )
	{
		currentTechnique.CommitChanges();

		if(index<0)
			pActorNode->RenderAllNodePrimitive();
		else
			pActorNode->RenderPrimitive( index );
	}
}

template<class KEY_TYPE>
void RActorPassRenderer::RenderOnPass(RCameraSceneNode* pCurrentCamera, RActorNodeRenderStrategy* pActorRenderStrategy, RRenderChain<KEY_TYPE>* pActorNodeRenderChain)
{
	// 	TIMEPROFILE_THIS_FUNCTION;
	

	if (m_bRender == false)
		return;

	if( !pActorNodeRenderChain)
		return;

	_ASSERT( pCurrentCamera);
	_ASSERT(pActorRenderStrategy);
	if(pActorRenderStrategy==NULL)
		return;

	RPFC_B("RActorPassRenderer::RenderOnPass Chain Loop");

	//Timer Reset & Start
	REngine::GetResourceProfiler().StartMeshProfiler();

	RPFC_THISFUNC;

	pActorRenderStrategy->Begin();
	{
		RTechnique currentTechnique;
		//////////////////////////////////////////////////
		// ������ ������ �޽ó�庰�� ��ȸ
		RRenderChainIterator<KEY_TYPE>	Iter;
		for( Iter = pActorNodeRenderChain->Begin(); Iter != pActorNodeRenderChain->End() ; Iter.Next() )
		{
			SCENE_SET* pSet = Iter.GetSceneSet();
			if( pSet->empty() )	continue;

			KEY_TYPE key = Iter.GetKey();


			//Set per-chain state
			pActorRenderStrategy->SetMaterialRenderState((RActorNode*)(*pSet->begin()), key.m_pMaterial);


			RActorNode* pInstancingActorNode = NULL;
			int nInstanceCnt = 0;

			for( SCENE_SET::iterator ActorNodeIter = pSet->begin() ; ActorNodeIter != pSet->end() ; ++ActorNodeIter)
			{
				RActorNode* pActorNode = (RActorNode*) (*ActorNodeIter);

				if(pActorNode->CheckDoRender()==false)
					continue;

				if(pActorNode->GetSubMaterialMask(key.m_nSubMaterial)==false)
					continue;

				if( pActorRenderStrategy->IsRender(pCurrentCamera, pActorNode, key.m_nSubMaterial) == false )
					continue;


				if( pActorRenderStrategy->CheckInstancing(pActorNode, key.m_pMaterial) && (pSet->size() > 1) )
					RenderInstancingNode( pActorNode, pCurrentCamera, pActorRenderStrategy, currentTechnique, nInstanceCnt, &pInstancingActorNode, key.m_nSubMaterial);
				else
					RenderNode(pCurrentCamera, pActorRenderStrategy, pActorNode, 1, currentTechnique, key.m_nSubMaterial, false);
			}

			// �ν��Ͻ� ���ۿ� �����ִ� ���� ������ ����.
			if( nInstanceCnt > 0 && pInstancingActorNode)
			{
				RenderNode(pCurrentCamera, pActorRenderStrategy, pInstancingActorNode, nInstanceCnt, currentTechnique, key.m_nSubMaterial, true);
			}
		}
		RPFC_E;

		if(currentTechnique.m_bUsing)
			EndTechnique( currentTechnique);

		_ASSERT(RTechnique::m_bUsing==false);


		//////////////////////////////////////////////////////////////////////////
		// state ����
		RestoreForRenderInstancing();
	}
	pActorRenderStrategy->End();	
}

bool RActorPassRenderer::BeginTechnique(RTechnique& currentTechnique, RShaderFX* pShader)
{
	RPFC_THISFUNC;

	if( pShader==NULL )
		return false;
	
	RTechnique technique = pShader->GetTechniqueByIndex(0);
	if( false == technique.IsEqual(currentTechnique) )
	{
		if(currentTechnique.m_bUsing)
		{
			currentTechnique.EndPass();
			currentTechnique.End();
		}

		currentTechnique = technique;

		currentTechnique.Begin(NULL, TECH_PRESERVE_NOT );
		currentTechnique.BeginPass();
	}

	return true;
}


bool RActorPassRenderer::EndTechnique( RTechnique& currentTechnique)
{
	currentTechnique.EndPass();
	currentTechnique.End();
	currentTechnique.m_hTechnique=0;

	return true;
}


void RActorPassRenderer::SetupForRenderInstancing( RActorNodeInstancingBuffer* pInstancingBuffer, UINT nCnt)
{
	RPFC_THISFUNC;
	REngine::GetDevice().SetupForRenderInstancing( pInstancingBuffer ? pInstancingBuffer->GetBuffer() : R_NONE, nCnt);
}


void RActorPassRenderer::RestoreForRenderInstancing()
{
	RPFC_THISFUNC;
	REngine::GetDevice().RestoreForRenderInstancing();
}

bool RActorPassRenderer::CheckCastShadow(RActor* pActor, RShadowStrategy* pShadowStrategy, RCameraSceneNode* pCurrentCamera)
{
	if( pActor->GetAlpha2PassRendering() || pActor->GetNoiseRefResult() < 1)
		return false;

	return pShadowStrategy->IsCastShadow(pCurrentCamera->GetViewMatrix(), pCurrentCamera->GetViewProjectionMatrix(), pActor->GetSphere());
}

void RActorPassRenderer::AddShaderEffectShaderIDForGStage(RActorNode* pActorNode, RSCID& rscComponents)
{
	RActor* pActor = pActorNode->m_pActor;

	bool bPointColor = pActorNode->IsUsePointColor();

	if( pActor->GetShaderEffectController())
	{
		rscComponents = pActor->GetShaderEffectController()->CommitShaderGStage( &rscComponents, bPointColor, pActor->m_bUseShade, pActor->m_fDistanceToCamera );
	}
}

void RActorPassRenderer::AddShaderEffectShaderIDForMStage(RActorNode* pActorNode, RSCID& rscComponents)
{
	RActor* pActor = pActorNode->m_pActor;

	bool bPointColor = pActorNode->IsUsePointColor();

	if( pActor->GetShaderEffectController())
	{
		rscComponents = pActor->GetShaderEffectController()->CommitShaderMStage( &rscComponents, bPointColor, pActor->m_bUseShade);
	}
}


void RActorPassRenderer::RenderTypeOnPass(RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, bool bUseBlending, RRENDERPASS _passType)
{
	// 	TIMEPROFILE_THIS_FUNCTION;
	RPFC_THISFUNC;
	if(!IsRenderEnabled() || !pRenderingList->size())
		return;

	RShaderCompositeManager* pCompositeManager = REngine::GetSceneManagerPtr()->GetShaderCompositeManager();
	RDevice* pDevice = REngine::GetDevicePtr();
	if( !pCompositeManager || !pDevice)
		return;


	REngine::GetResourceProfiler().StartMeshProfiler();

	SCENE_SET::iterator	iter;
	for( iter = pRenderingList->begin() ; iter != pRenderingList->end() ; ++iter)
	{
		RSceneNode *pSceneNode = (*iter);
		if( !pSceneNode->GetVisible() || !pSceneNode->GetUsable() || pSceneNode->GetOcclued() )	continue;

		RActor* pActor = MDynamicCast( RActor, pSceneNode);

		// ���� ��� �׸���
		for(size_t i=0;i<pActor->m_sortedRenderableActorNode.size();++i)
		{
			RActorNode* pActorNode = pActor->m_sortedRenderableActorNode[i];
			if( !pActorNode->IsRenderable())
				continue;

			pActorNode->SetVertexStream();

			for (int index = 0; index < pActorNode->m_pMeshNode->m_nMtrlCnt ; ++index)
			{
				if( pActorNode->GetSubMaterialMask(index) == false )
					continue;


				RSCID rscComponents = pCompositeManager->MakeComponentBitflagForMStage(pActorNode->GetMaskingPassComponentDesc(index));
				UpdateShaderIDForMStage( rscComponents, false, false, false, pActorNode->m_pActor->IsUseMultiplyColor() );

				/// �ȼ� ��� ���� ���� ���� �� �ô� ����ŷ �뵵�̹Ƿ� Ư�� ȿ���� ���� ���� ���� ���̰�, ���ؽ� ��� �� �ô� Ư�� ȿ���� ���� ���� �ʱ�� ����.
				/// �׷��Ƿ� AddShaderEffectShaderIDForMStage()�� ȣ������ �ʴ´�.

				if( RRP_POSTEFFECT_EDGE == _passType )	
				{
					// ���� �ɼ��� �ִ� ������Ʈ���� �ܰ����� ����� �ȵȴ�.
					if( pActorNode->IsAlpha() || pActorNode->HasAlphaMaterial())
					{
						continue;
					}
				}

				RShaderFX* pShader = pCompositeManager->GetShader( rscComponents, true);
				if( NULL == pShader )
					continue;

				// ���̴�
				pCompositeManager->SetMaterialRenderState( pActorNode->GetSubMaterial(index), false, false, false );
				pActorNode->SetNodeRenderState( pCurrentCamera->GetViewMatrix(), pCurrentCamera->GetViewProjectionMatrix() );

				switch( _passType )
				{
				case RRP_VBLUR:
					pActorNode->SetNodeRenderStateForBlur( pCurrentCamera);
					break;

				case RRP_POSTEFFECT_EDGE:
					{
						pDevice->SetStencilEnable(true);
						pDevice->SetStencilOp(RSTENCILOP_KEEP, RSTENCILOP_KEEP, RSTENCILOP_INCR);

						RShaderCompositeManager::m_cPEEdgeColor.SetVector3( pActor->GetEdgeColor() );
					}
					break;

				case RRP_EDGE:
					{
						REngine::GetDevice().SetCullMode(RCULL_CCW);	// ����� �׸���.

						//float fTickNess = thickness X camera dist X fovx / screen width;
						RVector vPos = pActorNode->GetWorldPosition();
						float fDist = pCurrentCamera->GetDistanceTo( vPos );

						RVector2 TickNess;
						TickNess.x = pActor->GetEdgeTickNess() * fDist * pCurrentCamera->GetFovX() / (float)REngine::GetDevice().GetScreenWidth();
						TickNess.y = pActor->GetEdgeTickNess() * fDist * pCurrentCamera->GetFovY() / (float)REngine::GetDevice().GetScreenHeight();
						RShaderCompositeManager::m_cTickNess.SetVector2( TickNess );
					}
					break;
				}


				if(bUseBlending)
					pDevice->SetBlending( RBLEND_SRCALPHA, RBLEND_INVSRCALPHA);
				else
					pDevice->SetBlending( RBLEND_NONE, RBLEND_NONE);


				if( _passType == RRP_POSTEFFECT_EDGE && pActor->GetPEEdgeProjection() )
					pDevice->SetDepthEnable(false, false);
				else
					pDevice->SetDepthEnable(true, false);


				// ���̴� ��ũ�� ���	
				RTechnique technique = pShader->GetTechniqueByIndex(0);
				technique.Begin(NULL, TECH_PRESERVE_NOT);
				technique.BeginPass();
				pActorNode->RenderPrimitive(index);
				technique.EndPass();
				technique.End();
			}
		}
	}

	// ���� ������Ʈ ����
	pDevice->SetDepthEnable(true, true);
	pDevice->SetBlending(RBLEND_NONE);
	pDevice->SetCullMode(RCULL_CW);

	pDevice->SetStencilEnable(false);	
	pDevice->SetStencilOp(RSTENCILOP_KEEP, RSTENCILOP_KEEP, RSTENCILOP_KEEP);
}

}
