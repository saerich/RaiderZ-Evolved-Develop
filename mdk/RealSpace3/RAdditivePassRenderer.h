#pragma once

#include "RPassRenderer.h"
#include "RMesh.h"
#include "RMeshNode.h"
#include "RActorNode.h"
#include "RActorPassRenderer.h"

namespace rs3 {

class RShaderFX;
class RDevice;
class RTechnique;
class RMaterial;
class REnvironmentSet;

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���� �н�������
//
//  1. RActor::GetUsingPass(virtual) ���� m_bUsingActorPassRenderer �÷��׸� ����
//     �˸´� RenderCandidate�� �����.( RRP_ACTOR / RRP_NORMAL )
//      - �Ϲ����� �������� ���ٸ�, OnPreRender / OnRender / OnPostRender �� ȣ��ǰ�,
//        OnPreRender���� UpdateForRender(������ OnPreRender), OnRender���� RenderSingle�� ȣ��ȴ�.
//
//  2. RActor::OnRegisterToRender ������ UpdateForRender�� ȣ��ǰ�,
//     UpdateForRender���� UpdateAnimationAndMakeRenderChain�� ���� m_bUsingActorPassRenderer�� ���� ��쿡 ����
//     �������� ����� ����ü���� ���������.
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
//  8. ����ü���̶�, �������� ����� ���� ���� ���� ��ũ�� ����Ʈ�̰� RTreeSceneNode�� RActorNode���� ������ ����ϰ� �ִ�.
//
//////////////////////////////////////////////////////////////////////////////////////////////////

class RS_API RAdditivePassRenderer : public RActorPassRenderer
{
public:
	RAdditivePassRenderer();
	virtual ~RAdditivePassRenderer(){}


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pass Renderer ID ����
	//
	DECLARE_PASS_RENDERER_ID( RRP_ALPHAADDITIVE );


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� / �Ҹ�
	//
	virtual void		Init();
	virtual void		Destroy();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// update / rendering
	//
	virtual void		RenderGStageOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, RRENDERINGMETHOD renderMethod) override;
	virtual void		RenderMStageOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, RRENDERINGMETHOD renderMethod) override;
	virtual void		RenderDepthOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pCulledList, RShadowStrategy* pShadowStrategy ) override;
};

}
