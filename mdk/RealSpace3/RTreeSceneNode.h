#pragma once

#include "RSceneNode.h"
#include "RTreeResource.h"
#include "RTreePassRenderer.h"

#include "RLoadingWorkUnit.h"
#include "MMemPool.h"

namespace rs3 {

RS_API extern const char *RSID_TREE;

class RS_API RTreeSceneNode : public RSceneNode, public RResource::LoadingController, public MMemPool<RTreeSceneNode>
{
	friend class RTreeResource;
	friend class RTreeResourceMgr;
	friend class RTreePassRenderer;
	friend class RCullingMgr;	//�����ؾ���
	friend class RTreeRenderingList;
	friend class RRenderingList;

public:
	RTreeSceneNode();
	~RTreeSceneNode();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// SceneNode declaration
	//
	MDeclareRTTI;
	DECLARE_NODE_ID_STRING(RSID_TREE);
//	DECLARE_USING_PASS(RRP_SPEEDTREE);


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Setter for LOD
	//
	void						SetUserLOD(bool l) { m_bUserLOD = l;}
	void						SetLOD(float x) { m_fLod = x; }		// 0.0 : ������ ~ 1.0 : �ְ� LOD


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// XML�� ����/�б�
	//
	virtual bool				CreateFromXML(MXmlElement &element, bool bBackgroundCreation);
	virtual void				SaveToXML(MXmlElement &element);


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Build / Clone
	//
	bool						Build(const char* filename, bool bBackgroundCreation = false);
	virtual RSceneNode*			Clone();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//LOD���
	virtual bool				CalculateLOD(RCameraSceneNode* pCamera, const RSphere* pBoundingSphere) override;


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Collision
	//
	RTreeCollisionObject*		GetTreeCollisionInfo(RTreeCollisionType type);


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// tool support
	//
	bool						SelectFrustumTest(const RViewFrustum& Frustum);		// ���콺 �巡�׷κ��� ������ �����������κ��� ����
	const std::string&			GetTreeFileName();									// ���ҽ��� �̸��� �˷��� �Ҷ�
	void						RegisterForMakingLightMap( RCameraSceneNode* pCurrentCamera);			// ����Ʈ���� ����� ���� �н��������� ���

	const float*				GetLocalAABB() const { return m_afBoundingBox; }

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	static float				m_fLodFarDist;
	static void					SetLodFarDist( float f)	{ m_fLodFarDist = f;	}
	static float				GetLodFarDist()			{ return m_fLodFarDist;	}

	virtual bool				IsLoadingCompleted();

private:
	RTreeResource*				m_pTreeResource;
	RTreeSceneNode*				m_pNextRenderChain;

	float						m_afBoundingBox[6];			// tree's bounding box

	bool						m_bUserLOD;					// ����ڰ� ���� LOD ������ �����ϴ°�?
	bool						m_bNeedUpdateMatrix;		//? �Ⱦ�����

	//////////////////////////////////////////////////////////////////////////


	float                       m_fWindMatrixOffset;		// used to assigned instances of the same tree different wind matrices

	float                       m_fLod;						// 1.0 = highest (fully 3D), 0.0 = lowest (billboard)

	CSpeedTreeRT::SLodValues	m_SpeedTreeLodInfo;			//���ǵ�Ʈ������ �������ÿ� �ʿ��� LOD���� ����

	float						m_fShadowing;


	//////////////////////////////////////////////////////////////////////////


	float						m_PrevSin;
	float						m_PrevCos;
	float						m_PrevAtan2;

	RMatrix						m_PositionScaleMatrix;		// Rotation���� �����ϰ� Position, Scale�� �̸� ����� Matrix.
															// WorldViewProjection Matrix�� ���̴� ����� �����ϱ� ���� �뵵.
															// ViewProjection Matrix�� Position, Scale���� ���̴� �����
															// �Ѱ� ���ϸ�, float���е� ������ ���� ������ �����.
	float						m_fCosTheta;				// Z�� ���� ȸ�� ��.
	float						m_fSinTheta;				// Z�� ���� ȸ�� ��.

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// RSceneNode �� ���� ������ ����
	//

	virtual void				OnUpdateTransform() override;
	virtual void				OnUpdateAABB() override;

	virtual void				OnRender( RCameraSceneNode* pCurrentCamera, int nSubMaterialID, RRENDERINGMETHOD renderMothod) override;
	virtual void				OnRenderShadowMap( RCameraSceneNode* pCurrentCamera , RShadowStrategy* pShadowStrategy ) override { OnRender( pCurrentCamera, -1, RENDERING_NULL); }

	virtual bool				OnPick(RPICKINFO& pickInfo);
	virtual void				CollectStaticCollisionPolygons(RSceneNodePolygonCollector* pCollector) const override;

	virtual void				AddToRenderingList(RRenderingList* pRenderingList)			override;
	virtual void				RemoveFromRenderingList(RRenderingList* pRenderingList)		override;


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Tree Background Loader
	//
// 	class TREE_LOADER : public RLoadingWorkUnit
// 	{
// 	protected:
// 		virtual int				CalcPriority();
// 		virtual void			OnBackgroundWork();
// 		virtual void			OnUpdateCompleteWork();
// 	public:
// 		TREE_LOADER(RTreeSceneNode* pTreeSceneNode) : m_pTreeSceneNode(pTreeSceneNode){}
// 		virtual ~TREE_LOADER() { InvalidateWork(); }
// 	
// 		RTreeSceneNode* m_pTreeSceneNode;
// 
// 		virtual const char*		GetName()			{ return "tree"; }
// 		virtual int				GetDebuggingID() { return 1; }
// 	} *m_pLoadingWorkUnit;

	virtual int				CalcPriority();
	virtual void			OnLoadingComplete(RResource*);

private:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ڽŰ� friend class �������� �ʿ��� ���� �޽��
	//
	void						SetUsableAndUpdate_();
	void						RenderCollisionObject();

	bool						UpdateLOD(RCameraSceneNode *pCamera, float fNearLod, float fFarLod, bool bUseFogFar);

	void                        SetWindMatrixOffset(float fOffset)	{ m_fWindMatrixOffset = float(int(10.0f * fOffset)); }
	float						GetWindMatrixOffset() const			{ return m_fWindMatrixOffset;	}

	bool						IsViewLeaves() { return (m_SpeedTreeLodInfo.m_anLeafActiveLods[0] != -1) || (m_SpeedTreeLodInfo.m_anLeafActiveLods[1] != -1); }
	bool						IsViewBranchFrond() { return (m_fLod > 0.0f); }
	bool						IsBillboardRenderable(void){ return m_SpeedTreeLodInfo.m_fBillboardFadeOut>0.0f ? true : false;	}	//�� Ʈ���� �ش��ϴ� �����带 �׷��ߵǳ�?

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���̴��� ���ε�� ���͸� ��ȯ�ϴ� �޼ҵ��
	//
	RVector4					GetPositionScale() const;						// xyz�� ��ġ, w�� �������� �־� ��ȯ
	RVector4					GetRotateYValue() const;						// sin, cos, -sin, 0 ��ȯ
	float						GetRotate();									// ������ġ�κ��� ��� ���Ҵ��� ��ȯ (z�����θ� ���ٰ� ����)
	float						GetScale() const { return RSceneNode::GetScale().x; }		// �����ϰ� ��ȯ - �� ������ �����ϰ� �����ϵȴٰ� ����
	const RMatrix&				GetPositionScaleMatrix() const { return m_PositionScaleMatrix; } // ȸ������ ������ Position, Scale�� �ݿ��� Matrix�� ��ȯ
	float                       GetBillboardFade(void) const { return m_SpeedTreeLodInfo.m_fBillboardFadeOut; }
	float						GetCosTheta() const	{ return m_fCosTheta;	}
	float						GetSinTheta() const	{ return m_fSinTheta;	}

private://���ǵ�Ʈ����
	void	InitSpeedTreeLODInfo(void);

};

//----------------------------------------------------------------------------------------------------
inline float RTreeSceneNode::GetRotate()
{
	RVector direction = GetDirection();
	if (direction.x != m_PrevSin && direction.y != m_PrevCos) 
	{
		m_PrevAtan2 = atan2(direction.y, direction.x);
		if( m_PrevAtan2 < 0.0f )
			m_PrevAtan2 += MMath::TWOPI;

		m_PrevSin = direction.x;
		m_PrevCos = direction.y;
	}
	return m_PrevAtan2;
}
//----------------------------------------------------------------------------------------------------
inline RVector4 RTreeSceneNode::GetPositionScale() const
{
	RVector4 ret;
	RVector pos = GetPosition();
	ret.x = pos.x;
	ret.y = pos.y;
	ret.z = pos.z;
	ret.w = m_scale.x;
	return ret;
}
//----------------------------------------------------------------------------------------------------
inline RVector4 RTreeSceneNode::GetRotateYValue() const
{
	RVector direction = GetDirection();

	RVector4 ret;
	ret.x = direction.x;
	ret.y = direction.y;
	ret.z = - direction.x;
	ret.w = 0;
	return ret;
}
//----------------------------------------------------------------------------------------------------

} // end of namespace
