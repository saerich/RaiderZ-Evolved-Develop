#pragma once

#include <vector>

using namespace std;

#include "RTypes.h"
#include "RVType.h"
#include "RSceneNode.h"
#include "RStaticMeshResource.h"

class MFile;

const float RS_LIGHT_SCALE_FACTOR = .25f;
const float RS_LIGHT_INVSCALE_FACTOR = 4.f;

namespace rs3 {

RS_API extern const char *RSID_STATICMESHSCENENODE;

class RMaterial;
class RStaticMeshSceneNode;
class RMaterialResource;
class RShaderCompositeManager;
class REnvironmentSet;
class RStaticMeshPassRenderer;
class RStaticMeshRenderStrategy;
class RStaticMeshDepthRenderStrategy;
class RStaticMeshGStageRenderStrategy;
class RStaticMeshMStageRenderStrategy;

struct RRsRenderHelper {
	WORD m_indicies[INDEX_FLUSH_COUNT];
	int m_nCount;
	int m_nCurMinIndex;
	int m_nCurMaxIndex;
	int m_nMaxIndex;
	int m_nBaseIndex;
	int m_nMaterial;
	int m_nLightmapID;
	RenderInfo* m_pRenderInfo;

	inline void Append(WORD* pIndicies,int nCount,int nMin,int nMax)
	{
		memcpy(&m_indicies[m_nCount],pIndicies,sizeof(WORD)*nCount);
		m_nCount+=nCount;
		m_nCurMaxIndex = max(m_nCurMaxIndex,nMax);
		m_nCurMinIndex = min(m_nCurMinIndex,nMin);
	}

	inline bool isEnoughToInsert(int nCount)
	{
		return m_nCount+nCount<INDEX_FLUSH_COUNT;
	}

	inline void Reset() { m_nCurMaxIndex = m_nCount = 0; m_nCurMinIndex = m_nMaxIndex-1; }
};

struct RSTATICMESHPICKINFO
{
	// picking�� ���� ������
	float		m_fPickDistanceSq;
	bool		m_bPickFound;
	RVector		m_pickOrigin,m_pickTo,m_pickDir;
	RPICKINFO	*m_pPickInfo;
	bool		m_bBackFaceCull;
};

/// �������� �ʴ� ���� �޽�, ���� �����̸� ��Ż�� ����Ǿ��ִ�
class RS_API RStaticMeshSceneNode : public RSceneNode, public RStaticMeshResource, public RResource::LoadingController
{
	friend RStaticMeshPassRenderer;
	friend RStaticMeshRenderStrategy;
	friend RStaticMeshDepthRenderStrategy;

	MDeclareRTTI;
	DECLARE_NODE_ID_STRING(RSID_STATICMESHSCENENODE);
	
	static RShaderCompositeManager *m_pShaderCompositeManager;
	static REnvironmentSet		*m_pEnvironmentSet;

	RVector3				m_worldLightVec;				///< ������ ��ġ�� (����)������ ����
	RVector3				m_worldLightPos;				///< ������ ��ġ�� ������ ������ǥ
	RVector3				m_worldLightColor;				///< ������ ��ġ�� ������ ��
	RVector3				m_worldLightAmbient;			///< ambient color

	// �������� ����
	struct RRENDERINFO
	{
		RGeometry*		pGeometry;
		RRsAABBTree*	pAABBTree;
		RRsNode*		pNode;
	};
	vector<RRENDERINFO>		m_renderNodes[2];

	RVector					m_currentCameraPosition;
	int						m_nCurrentCell;
	int						m_nCullingPlanes;
	const RPlane*			m_pCullingPlanes;						///< culling �� ���� portal �� �̷�� ���
	const RPlane*			m_pCullingNearPlane;
	const RPlane*			m_pCullingFarPlane;
	int						m_nUserCullingPlanes;
	const RPlane*			m_pUserCullingPlanes;					///< culling �� ���� ������ ���� ( near-far ������� )
	const RLightSceneNode*	m_pCurrentLight;
	static RRsRenderHelper	m_rsRenderer;
	unsigned int			m_unRenderCounter;
	int						m_nCurrentLightCount;					///< ���� light multipass �н����� �׸��� ���� ��

	float				m_fRemainedBlendingTime;

	RStaticMeshRenderStrategy*				m_pRenderStrategy;			///< ������ ������
	RStaticMeshDepthRenderStrategy*			m_pDepthRenderStrategy;
	RStaticMeshGStageRenderStrategy*		m_pGStageRenderStrategy;
	RStaticMeshMStageRenderStrategy*		m_pMStageRenderStrategy;
	RStaticMeshRenderStrategy*				m_pCurrentRenderStrategy;	///< ���õ� ������ ����
	
	bool				m_bPointColor;

	// ���� ��Ȳ ����׿�
	int					m_nRenderedTriangles;
	int					m_nRenderedNodes;
	int					m_nRenderedCells;

	// TODO: ���� ���� - charmke
//	int					m_nCullTestCount; 

protected:
	virtual void				OnAddScene();
	virtual void				OnRemoveScene();
	
	virtual int		CalcPriority() override;
	virtual void	OnLoadingComplete(RResource*) override;

	virtual void Cull(RCameraSceneNode* pCamera,const vector<RCULLDESC>& cullDescs, const RPlane* pPlaneNear, const RPlane* pPlaneFar);
	void CullRecurse(RCell *pCell,const RPlane *pPortalPlanes, int nPlanes);
	void CullGeometry(RGeometry* pGeometry);
	void CullNode(RGeometry* pGeometry, RRsAABBTree* pAABBTree, RRsNode* pNode, bool bCulling);

	static const int CULL_OUTSIDE = -1;
	static const int CULL_PARTIAL =  0;
	static const int CULL_INSIDE  =  1;
	int CullTest(RRsNode* pNode);

	virtual void OnRender( RCameraSceneNode* pCurrentCamera, int nSubMaterialID, RRENDERINGMETHOD renderMothod) override;
	virtual void OnRenderShadowMap( RCameraSceneNode* pCurrentCamera, RShadowStrategy* pShadowStrategy ) override;

	virtual bool CreateFromXML(MXmlElement &element, bool bBackgroundCreation = false);
	virtual void SaveToXML(MXmlElement &element);

	bool ClipPlanes(const RVector& pos, const RPlane *pClipPlanes, int nClipPlanes,
		const RVector* pPlaneVertices, int nPlaneVertices, vector<RPlane> &outClippedPlanes, bool bDebugRender = false);

	void ClearRenderNodes();

	void RestoreRenderState();

private:
	RCell		*m_pCurrentCell;

	bool Pick(RRsNode *pNode, RSTATICMESHPICKINFO& info, RVertexOffsetInfo& VertexInfo);

	void GetVertexFromBound(RRsNode* pNode, std::vector<RVt_pos>* vecVertex, const RBoundingBox &aabb, RVertexOffsetInfo& VertexInfo);

	void UpdateAABB();

public:
	RStaticMeshSceneNode();
	virtual ~RStaticMeshSceneNode();

	static bool CreateSharedResources();			// �ʿ��� ���̴��� ���ҽ� �Ҵ�
	static void Destroy();

	void SetCurrentLightmapsGroup( const char* pSzName );
	void BeginBlendingLightmapsGroups( const char* pSzSrcGroup, const char* pSzTargetGroup, float _fProcessTime, bool bDeleteSrcOnComplete = true){}

	int	GetRenderedTriangleCount() { return m_nRenderedTriangles; };
	int	GetRenderedNodeCount() { return m_nRenderedNodes; };
	int	GetRenderedCellCount() { return m_nRenderedCells; };

//	virtual bool Pick(const RVector &origin,const RVector &to,RPICKINFO *pPickInfo);
	virtual bool OnPick(RPICKINFO& pickInfo);

	virtual void OnUpdateAABB() override;

	/// Ư�� ��ġ�� ���� ���� ã�´�
	int FindCell(const RVector& vecPosition);

public:
	int GetVertexFromBound(std::vector<RVt_pos>* vecVertex, const RBoundingBox &aabb);

	// �ӽ�
	void Render( RCameraSceneNode* pCurrentCamera);
	void RenderRaw( RCameraSceneNode* pCurrentCamera);
	void RenderGeometry(RGeometry* pGeometry);

	void RenderGStage( RCameraSceneNode* pCurrentCamera);
	void RenderMStage( RCameraSceneNode* pCurrentCamera);

private:
	void CollectStaticCollisionPolygons(RRsNode* pNode, RSceneNodePolygonCollector* pCollector, RVertexOffsetInfo& VertexInfo) const;
	virtual void CollectStaticCollisionPolygons(RSceneNodePolygonCollector* pCollector) const override;

private:
	bool	m_bEnablePicking;	//��ŷ�� �ɰ��ΰ�?
public:
	void	setEnalbePicking(bool b)	{	m_bEnablePicking = b;		}
	bool	getEnablePicking(void)		{	return m_bEnablePicking;	}
};

}