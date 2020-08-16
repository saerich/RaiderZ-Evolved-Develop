#pragma once

#include "RPassRenderer.h"
#include "RMesh.h"
#include "RMeshNode.h"
#include "RActorNode.h"

namespace rs3 {

class RShaderFX;
class RDevice;
class RTechnique;
class RMaterial;
class REnvironmentSet;
class RPostEffectDistortion;
class RActorNodeRenderStrategyShadowMap;

enum ACTOR_TECH_TYPE {
	ACTORTECH_GSTAGE_IN = 0,
	ACTORTECH_MSTAGE_IN,
	ACTORTECH_GSTAGE_DE,
	ACTORTECH_MSTAGE_DE,
	ACTORTECH_SHADOW,
	ACTORTECH_COUNT
};

// ACTOR_TECH_TYPE�� ¦�� �¾ƾ�..
const RRENDERINGMETHOD renderMethodByTech[] = {
	RENDERING_INFERRED,
	RENDERING_INFERRED,
	RENDERING_DEFERRED,
	RENDERING_DEFERRED,
	RENDERING_FORWARD,
	RENDERING_NULL,
	RENDERING_END
};

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���� �н�������
//
//  1. RActor::GetUsingPass(virtual) ���� m_bUsingActorPassRenderer �÷��׸� ����
//     �˸´� RenderCandidate�� �����.( RRP_ACTOR / RRP_NORMAL )
//      - �Ϲ����� �������� ���ٸ�, OnPreRender / OnRender / OnPostRender �� ȣ��ǰ�,
//        OnPreRender���� UpdateForRender(������ OnPreRender), OnRender���� RenderSingle�� ȣ��ȴ�.
//
//
//  3. RActor::RenderSingle�� �������� �����Ҽ� ������,
//     ��� ���Ϳ� ���밡���� SetGlobalActorRenderState�� UpdateGlobalActorTransform�� �ϰ�
//     ���ͳ���� RenderSingle�� ȣ���Ѵ�.
//
//  4. RActorNode::RenderSingle�� �޽ó��� ���� ���� ���ؽ��� ������ ���ؽ� ������ �ϰ�
//     �޽ó���� ���� ���׸����� ������ ���׸��� ������ �Ѵ�.
//      - ���׸����� ������ å���� RActorPassRender�� ������.
//        (�������� å���� RActorNode::Render�� ������ �־���, BeginMaterial�� ���� ������ �߾���. )
//
//  5. RActorNode�� ���̴� ����Ʈ�� ��ũ���� RActorNode�� �����Ǵ� �������� ���ͷκ��� ���
//     ���׸��� ���� �ε����� �� ������ �ְ�, RenderSingle �� RActorPassRenderer::PassOnRender���� ����Ѵ�.
//
//  6. ȯ�� Ÿ�ϸ� ����Ʈ�� ���, �ʿ��� ����̽� ������ å���� RActorPassRenderer�� ������.
//
//  7. RActorPassRenderer�� ������� �ʴ� ����
//      - REffectActor(Ÿ�ϸ�, ����Ʈ���� ���� �ȸԾ�� �ϴ� ����) / RSceneSky / RWaterActor �� ���ڰ� ������ å���� ������.
//
//////////////////////////////////////////////////////////////////////////////////////////////////

class RS_API RActorPassRenderer : public RPassRenderer
{
public:
	RActorPassRenderer();
	virtual ~RActorPassRenderer(){}


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pass Renderer ID ����
	//
	DECLARE_PASS_RENDERER_ID( RRP_ACTOR );


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� / �Ҹ�
	//
	virtual void		Init();
	virtual void		Destroy();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// update / rendering
	//
	virtual bool		IsCurrentShaderRejective(){ return true; }
	virtual void		RenderDepthOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pCulledList, RShadowStrategy* pShadowStrategy ) override;
	void				RenderDoubleShadowDepthOnPass( RCameraSceneNode* pCurrentCamera, RShadowStrategy* pShadowStrategy );

	virtual void		RenderGStageOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, RRENDERINGMETHOD renderMethod) override;
	virtual void		RenderMStageOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, RRENDERINGMETHOD renderMethod) override;
	
	template<class KEY_TYPE>
	void				RenderOnPass(RCameraSceneNode* pCurrentCamera, RActorNodeRenderStrategy* pActorRenderStrategy, RRenderChain<KEY_TYPE>* pActorNodeRenderChain);

	/// �ν��Ͻ� ���� ó��
	virtual void		RenderInstancingNode( RActorNode* pActorNode, RCameraSceneNode* pCurrentCamera, RActorNodeRenderStrategy* pActorRenderStrategy, RTechnique& currentTechnique, int& nInstanceCnt, RActorNode** pInstancingActorNode, int index);
	virtual void		RenderNode(RCameraSceneNode* pCamera, RActorNodeRenderStrategy* pActorRenderStrategy, RActorNode* pActorNode, int nInstanceCnt, RTechnique& currentTechnique, int index, bool bAbleInstancing);

	virtual void		SetActorNodeInstancingData(int nIdx, RActorNode* pNode);
	virtual void		SetInstancingConstant(int nCnt);
	virtual bool		BeginTechnique(RTechnique& currentTechnique, RShaderFX* pShader);
	virtual bool		EndTechnique( RTechnique& currentTechnique);
	virtual void		SetupForRenderInstancing( RActorNodeInstancingBuffer* pInstancingBuffer, UINT nCnt);
	virtual void		RestoreForRenderInstancing();

	inline void			EnableRender()	{ m_bRender = true; }
	inline void			DisableRender()	{ m_bRender = false;}
	inline bool			IsRenderEnabled(){ return m_bRender; }


protected:
	virtual void		UpdateShaderIDForMStage(RSCID& rscComponents, bool bInstancing, bool bNoiseRef, bool bBlendingMaterial, bool bMultiplyColor) {}
	virtual void		AddShaderEffectShaderIDForGStage(RActorNode* pActorNode, RSCID& rscComponents);
	virtual void		AddShaderEffectShaderIDForMStage(RActorNode* pActorNode, RSCID& rscComponents);
	void				RenderTypeOnPass(RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, bool bUseBlending, RRENDERPASS _passType);

protected:

	bool CheckCastShadow(RActor* pActor, RShadowStrategy* pShadowStrategy, RCameraSceneNode* pCurrentCamera);

	static RActorNodeInstancingBuffer			m_InstancingBuffer;
	static RMatrix*								m_pInstancingData;

	bool				m_bRender;

private:
	void		RenderShadowMap(RCameraSceneNode* pCurrentCamera, RShadowStrategy* pShadowStrategy, bool bDoubleShadowPass );
};

}
