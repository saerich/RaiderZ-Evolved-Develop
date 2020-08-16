#pragma once

namespace rs3 {

class RSceneManager;
class RDevice;
struct RCULLDESC;
class RShadowStrategy;
enum RRENDERINGMETHOD;

//////////////////////////////////////////////////////////////////////////
//
// �н������� ���� ��ũ��
//
//////////////////////////////////////////////////////////////////////////
#define DECLARE_PASS_RENDERER_ID(x)		enum { PASS_RENDERER_ID = x };

//////////////////////////////////////////////////////////////////////////
//
// �н�������
//
//  1. RSceneManager::Cull / RSceneManager::UpdateAndCull ����
//     ī�޶��� RegisterToRender�� ȣ��Ǿ� ������� OnRegisterToRender�� �Ҹ���,
//     ������ ������ �ڽŵ��� ��å�� �н��������� ����� �Ѵ�.
//
//  2. RCameraSceneNode::RenderPass ���� �н��� �´� �н��������� �ִٸ�
//     PassRenderer�� PassOnRender�� ���� �������� �Ѵ�.
//
//////////////////////////////////////////////////////////////////////////
class RS_API RPassRenderer
{
public:
	RPassRenderer(){}
	virtual ~RPassRenderer(){}

	//////////////////////////////////////////////////////////////////////////
	// Pass Renderer ID ����
	//
	DECLARE_PASS_RENDERER_ID( RRP_NONE );


	//////////////////////////////////////////////////////////////////////////
	// ���� / �Ҹ�
	//
	virtual void Init()		{}
	virtual void Destroy()	{}
	virtual void Reset()	{}

	//////////////////////////////////////////////////////////////////////////
	// update
	//
	virtual void Update(float fTimeDelta)	{ }

	//////////////////////////////////////////////////////////////////////////
	// culling
	//
	virtual	bool CullByViewFrustum(const RSceneNode* pNode,RCameraSceneNode* pCamera);
	virtual bool CullByPlanes(RCameraSceneNode* pCamera, const RSceneNode* pNode, const vector<RCULLDESC>& cullDescs, const RPlane* pPlaneNear, const RPlane* pPlaneFar );

	//////////////////////////////////////////////////////////////////////////
	// rendering
	//
	virtual bool IsCurrentShaderRejective()	{ return true; }
	virtual void RenderOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pCulledList, RRENDERINGMETHOD eMothod);
	virtual void RenderDepthOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pCulledList, RShadowStrategy* pShadowStrategy);
	
	virtual void RenderGStageOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, RRENDERINGMETHOD renderMethod);
	virtual void RenderMStageOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, RRENDERINGMETHOD renderMethod);

	virtual void RenderZStageOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, RRENDERINGMETHOD renderMethod);

protected:
	bool CullDefault( const RBox& aabb, const vector<RCULLDESC>& cullDescs );
};

}
