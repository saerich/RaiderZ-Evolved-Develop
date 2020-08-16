#ifndef _RSCENEGRAPH_H
#define _RSCENEGRAPH_H

#include "RTypes.h"
#include "RDuplicateUpdateCheckContext.h"
#include "MRTTI.h"
#include "RViewFrustum.h"
#include "MXml.h"
#include <list>
#include <set>
#include "ROccluderContext.h"
#include "RLODNode.h"

using namespace std;

namespace rs3 {

class RSceneNode;
class RWorldSceneNode;
class RSceneNodeController;
class RDevice;
class RCameraSceneNode;
class RSceneManager;
class RLightSceneNode;
class RMultiPassLightSceneNode;
class RSpotLightSceneNode;
class RStencilLightSceneNode;
class RRenderHelper;
class RViewFrustum;
class RSceneNodeController;
class RScreenEffectManager;
class RCullingMgr;
class RRenderingList;
class RInferredRenderingStrategy;
class RShadowStrategy;

enum RRENDERINGMETHOD;

typedef std::set<RSceneNode*> SCENE_SET;
typedef std::list<RSceneNodeController*> RSCENENODECONTROLLERLIST;
typedef std::list<RSceneNode*> RSceneNodeList;
typedef std::list<bool> BoolList;
typedef vector<RSceneNode*> RSceneNodeArray;
typedef vector<RWorldSceneNode*> RWorldSceneNodeArray;
typedef set<RWorldSceneNode*> RWorldSceneNodeMap;
typedef bool (* RSerializeTest)( RSceneNode* pSceneNode );

/// ��Ʈ�ѷ��� ������Ʈ �Ǵ� Ÿ�ֿ̹� ���� Ÿ���� ������
enum RSNCTRL_TYPE
{
	RSNCTRL_PREUPDATE,
	RSNCTRL_UPDATE,
	RSNCTRL_POSTUPDATE,
	RSNCTRL_POSTANIMATION,
	RSNCTRL_PRERENDER,
	RSNCTRL_CUSTOMUPDATE,
	RSNCTRL_MAX,
};

struct RUPDATENODE {
	bool		bValid;		// �� �÷��װ� false �̸� �����Ǿ�� �Ѵ�
	RSceneNode*	pSceneNode;	// ������Ʈ�� ���
};

typedef list<RUPDATENODE> RAutoUpdateNodeList;
typedef RAutoUpdateNodeList::iterator RAutoUpdateNodeListItr;


//////////////////////////////////////////////////////////////////////////
//
// �����н� : ������ ī�޶� ���� RENDERPASS�� ������� �������� �մϴ�.
//
// ���ǻ��� :
//		�Ϲ������� Update�� Render�� ���е�����, �ٸ� ī�޶� ��ǥ�踦 ����ϴ�
//		���� ������ �н��� ������ ������ �н����� ���� ������Ʈ�� �ش��ϴ�
//		�ø��� �׿����� ����ü�� ������ ���� �մϴ�. �̷����,
//		SceneManager���� �����س��� ����ü���� �����н����ʹ� ����� �� �����ϴ�.
//		����, RRP_NOT_SAFE_RENDERCHAIN �н� ���Ŀ��� ���Ŵ��� ������Ʈ�� �����
//		(Primary Camera�κ��� �����) ����ü�ο� �������� �н��� ����ϼ���.
//
//////////////////////////////////////////////////////////////////////////
enum RRENDERPASS
{
	RRP_NONE = -1,
	RRP_CAMERA = 0,
	RRP_WORLDSCENE,
	RRP_DIRLIGHT,
	RRP_SUBLIGHT,
	RRP_LIGHT,
	RRP_OCCLUDER,
	RRP_SHADOWMAP,
	RRP_PREEFFECT,

	RRP_START_PASS_OF_BACKGROUND,
	RRP_SKYBOX,
	RRP_END_PASS_OF_BACKGROUND,

	RRP_START_PASS_OF_GSTAGE,		/// G-stage ���� �� �н� ���� ������ ���� ��.
	RRP_START_PASS_OF_MSTAGE,		/// M-stage ���� �� �н� ���� ������ ���� ��.
	RRP_START_PASS_OF_ZSTAGE,		/// Z-stage ���� �� �н� ���� ������ ���� ��.
	
	RRP_SPEEDTREE,
	RRP_NORMAL,

	RRP_ACTOR,

	RRP_TERRAIN,
	RRP_STATICMESH,				// static mesh�� ���ĸ��׸����� ����.
	RRP_END_PASS_OF_GSTAGE,		// G-stage ���� �� �н� ���� ������ ���� ��.
	RRP_END_PASS_OF_ZSTAGE,		// Z-stage ���� �� �н� ���� ������ ���� ��.
	RRP_VBLUR,					// ���ؽ� ��� ��. ����Ʈ�� �׷����� ���� �׷������ؼ� �н��� �߰��Ѵ�. 
	RRP_EDGE,					// �ܰ���
	RRP_NOT_SAFE_RENDERCHAIN,	///< ������ ���� �н��� ����ü���� �籸�� ���־�� �մϴ�.
	RRP_DECAL_POLYGON,			///< Polygon Decal
	RRP_DECAL_PROJECTION,		///< Projection Decal
	RRP_WATER,
	RRP_DISTORTION,
	RRP_ALPHAADDITIVE,
	RRP_PARTICLE,
	RRP_ALPHA,					///< Alpha�� AlphaAdditive���� �ؿ� �־�� AlphaAdditive�� ������ �ʽ��ϴ�.
	RRP_SPOTLIGHT,
	RRP_WEATHEREFFECT,

	RRP_POSTEFFECT,

	RRP_DEBUG,					// ����׿� �н�
	RRP_2D,						// ���ÿ��� ���� �ؽ�Ʈ ���

	RRP_END_PASS_OF_MSTAGE,		// M-stage ���� �� �н� ���� ������ ���� ��.


	// ����ŷ���� ���̴� �͵� ���� 
	RRP_ABLUR,					// ���� ��� ��.
	RRP_PBLUR,					// �ȼ� ��� ��.
	RRP_POSTEFFECT_EDGE,		// ����Ʈ ����Ʈ�� �ܰ���
	RRP_FINAL_SCENE,			// ����Ʈ ����Ʈ ������

	RRP_MAX
};

const static RRENDERPASS RRP_RENDER_BEGIN	= RRP_SUBLIGHT;
const static RRENDERPASS RRP_RENDER_END		= (RRENDERPASS)(RRP_DEBUG-1);
const static RRENDERPASS RRP_RENDER_END_ALL	= (RRENDERPASS)(RRP_MAX-1);

const static RRENDERPASS RRP_CULL_BEGIN = RRP_SUBLIGHT;
const static RRENDERPASS RRP_CULL_END	= (RRENDERPASS)(RRP_MAX-1);

//////////////////////////////////////////////////////////////////////////
//
// Frustum Test Method
//
//////////////////////////////////////////////////////////////////////////
enum RFRUSTUMTESTMETHOD
{
	RFTM_FAST,
	RFTM_NORMAL,
	RFTM_DETAIL
};

//////////////////////////////////////////////////////////////////////////
/// �ų�带 ������Ʈ �ϴ� ���
enum RUPDATEMETHOD
{
	RUM_UPDATE_MANUALLY,		//< �ų�带 �������� ������Ʈ ���־�� �Ѵ�
	RUM_UPDATE_WHEN_VISIBLE,	//< ȭ�鿡 ���������� ������Ʈ�� �Ҹ����
	RUM_UPDATE_ALWAYS			//< �׻� ������Ʈ�� �Ҹ���
};

//////////////////////////////////////////////////////////////////////////
//
// SceneNode Picking
//
//////////////////////////////////////////////////////////////////////////
enum RPICKMETHOD
{
	RPM_FAST,
	RPM_NORMAL,
	RPM_DETAIL,
};

class RS_API RPICKINFO
{
	friend RSceneManager;
	RVector tempPickDirection;	// �ӽ�, ������� ����

public:
	RPICKINFO() : inMethod(RPM_NORMAL), outDistanceSq(FLT_MAX), outNode(NULL) { }
	RPICKINFO( const RVector& vPickOrigin, const RVector& vPickTo, RPICKMETHOD pick_method )
		: inPickOrigin(vPickOrigin)
		, inPickTo(vPickTo)
		, inMethod(pick_method)
		, outDistanceSq(FLT_MAX)
		, outNode(NULL)
	{}

	RVector			inPickOrigin;		// �Է� : pick�� ������. -> ���� �ΰ��� ä���ְ� ȣ��
	RVector			inPickTo;			// �Է� : pick�� ����.
	RPICKMETHOD		inMethod;			// �Է� : pick ���

	RVector			outPosition;		// ��ȯ : ��ġ
	RVector			outNormal;			// ��ȯ : normal, ���� �� ����
	float			outDistanceSq;		// ��ȯ : PickOrigin������ �Ÿ��� ����
	RSceneNode*		outNode;			// ��ȯ : scenenode �� pointer
};


//////////////////////////////////////////////////////////////////////////
//
// SceneNodeVisitor
//		����带 Traverse�Ҷ� PreOrder�� PostOrder���� �湮�Ͽ� �ൿ�ϴ� ��ü
//		�Ϲ������� PreOrder������ �۾��� ó��
//		PreOrder�� ���, ���ϰ��� true�϶� ���� Traverse  ����
//
//////////////////////////////////////////////////////////////////////////

// Visitor Interface
class RS_API RSceneNodeVisitor
{
public:
	RSceneNodeVisitor(){}
	virtual ~RSceneNodeVisitor(){}

	virtual bool OnPreOrderVisit( RSceneNode* pSceneNode )	= 0;
	virtual void OnPostOrderVisit( RSceneNode* pSceneNode )	= 0;
};

class RS_API RSceneNodeConstVisitor
{
public:
	RSceneNodeConstVisitor(){}
	virtual ~RSceneNodeConstVisitor(){}

	virtual bool OnPreOrderVisit( const RSceneNode* pSceneNode )	= 0;
	virtual void OnPostOrderVisit( const RSceneNode* pSceneNode )	= 0;
};

// Update Visitor
class RS_API RSceneNodeUpdateVisitor : public RSceneNodeVisitor
{
public:
	RSceneNodeUpdateVisitor(){}
	virtual ~RSceneNodeUpdateVisitor(){}

	virtual bool OnPreOrderVisit( RSceneNode* pSceneNode );
	virtual void OnPostOrderVisit( RSceneNode* pSceneNode );
};

class RS_API RSceneNodeUpdateTransformVisitor : public RSceneNodeVisitor
{
public:
	RSceneNodeUpdateTransformVisitor(){}
	virtual ~RSceneNodeUpdateTransformVisitor(){}

	virtual bool OnPreOrderVisit( RSceneNode* pSceneNode );
	virtual void OnPostOrderVisit( RSceneNode* pSceneNode ) {}
};


//////////////////////////////////////////////////////////////////////////
//
// RSceneNodePolygonCollector
//		�ű׷������� �������� �����ϱ����� �������̽�
//
//////////////////////////////////////////////////////////////////////////

class RS_API RSceneNodePolygonCollector
{
public:
	RSceneNodePolygonCollector(){}
	virtual ~RSceneNodePolygonCollector(){}

	virtual void	HandleError(const std::string& strError) = 0;
	virtual DWORD	GetZoneMaterialID(const std::string &strMaterialName) = 0;
	virtual void	OnAddPolygon( const RVector& p1, const RVector& p2, const RVector& p3, WORD wNodeFlag, WORD wUserFlag, DWORD dwMaterial ) = 0;
};


//////////////////////////////////////////////////////////////////////////
//
// SceneNodeAttribute
//
//////////////////////////////////////////////////////////////////////////
enum RSCENENODE_ATTRIBUTE : DWORD
{
	//RSNA_HAS_SCENE_DESC			= 1 << 1,	///< �� ��ũ������ �������� ����. �����Ǹ� �ٸ������� ������ �켱�ؼ� ���� xml�� �������� �д´�.
	RSNA_DO_NOT_SAVE			= 1 << 2,	///< Scene ����ÿ� ������� �ʴ´�.
	RSNA_NO_LIGHTMAP			= 1 << 3,	///< ����Ʈ�� ��������. �� �÷��װ� �����Ǹ� ����Ʈ���� ������ �ʴ´�.
	RSNA_NO_HIERARCHY_UPDATE	= 1 << 4,	///< ���� ������Ʈ�� ���� �ʴ´�.
	RSNA_NO_DEBUG_RENDERING		= 1 << 5,	///< ����� �������� ���� �ʴ´�.
	RSNA_HIERARCHY_TREE			= 1 << 6,	///< �������� Ʈ�� ������ ����带 �����Ͽ� ����
	RSNA_DO_NOT_SAVE_COLLISION	= 1 << 7,	///< Collision ( *.cl2 ) ����ÿ� ������� �ʴ´�.
};


//////////////////////////////////////////////////////////////////////////
//
// ����� ��ũ��
//
//////////////////////////////////////////////////////////////////////////
//#define DECLARE_USING_PASS(x)			virtual RRENDERPASS GetUsingPass() const	{ return x; }
#define DECLARE_PASS_RENDERER_ID(x)		enum { PASS_RENDERER_ID = x };
#define DECLARE_NODE_ID_STRING(x)		virtual const char* GetNodeIDString() const { return x; } \
										static const char* GetNodeIDFromType() { return x; }

//////////////////////////////////////////////////////////////////////////
//
// ��� ������� base
//
//////////////////////////////////////////////////////////////////////////
class RS_API RSceneNode : public RLODNode, public RHaveAttributes< RSCENENODE_ATTRIBUTE >
{
	friend RSceneNodeUpdateVisitor;
	friend RSceneNodeUpdateTransformVisitor;
	friend RSceneNodeController;

	friend RSceneManager;
	friend RRenderHelper;

	friend RCameraSceneNode;
	friend RMultiPassLightSceneNode;
	friend RSpotLightSceneNode;
	friend RCullingMgr;
	friend RRenderingList;
	friend RInferredRenderingStrategy;

public:

	//////////////////////////////////////////////////////////////////////////
	// SceneNode Declaration
	//
	MDeclareRootRTTI;

//	DECLARE_USING_PASS(RRP_NORMAL);
	DECLARE_NODE_ID_STRING("unknown");

	RRENDERPASS GetUsingPass() const;


protected:

	//////////////////////////////////////////////////////////////////////////
	// ��ӹ��� �ڽĵ��� ������ǵ�
	//
	virtual void				OnUpdateTransform(){}	// UpdateTransform�ÿ�
	virtual void				OnPreUpdate(){}			// Update�ÿ�
	virtual void				OnUpdate(){}			// OnPreUpdate -> OnUpdate -> UpdateTransform -> children update -> OnPostUpdate ������ �Ҹ��ϴ�
	virtual void				OnUpdateAABB(){};		// UpdateBoundingVolume���� ȣ��
	virtual bool				OnUpdateSphere();		// UpdateBoundingVolume���� ȣ��, ���� false : �ٿ������ �Ⱥ��� , ���� true : �ٿ������ ����	
	virtual void				OnPostUpdate(){}

	virtual void				OnPreRender( RCameraSceneNode* pCurrentCamera){}			// OnRegisterToRender(����) -> OnPreRender -> OnRender -> OnPostRender
	virtual void				OnRender( RCameraSceneNode* pCurrentCamera, int nSubMaterialID, RRENDERINGMETHOD renderMothod){}
	virtual void				OnRenderGStage( RCameraSceneNode* pCurrentCamera, int nSubMaterialID, RRENDERINGMETHOD renderMothod){}
	virtual void				OnRenderMStage( RCameraSceneNode* pCurrentCamera, int nSubMaterialID, RRENDERINGMETHOD renderMothod){}
	virtual void				OnRenderZStage( RCameraSceneNode* pCurrentCamera, int nSubMaterialID, RRENDERINGMETHOD renderMothod){}
	virtual void				OnPostRender( RCameraSceneNode* pCurrentCamera){}

	virtual void				OnAdd(){}				// �θ� �������ų� ������ �Ҹ���� ���
	virtual void				OnRemove(){}

	virtual void				OnAddScene();			// �ű׷����� �������ų� ������ �Ҹ���� ���
	virtual void				OnRemoveScene();

	virtual void				OnMessage(char* szMsg, void* param)	{}			// �޽���, �Ƹ� �뵵�� char ������ SceneNode�� Control �Ϸ��Ҷ�..
	virtual bool				OnPick(RPICKINFO& pickInfo) { return false; }	//��ŷ, ������� AABB�� ��ŷ�� �Ұ��.

	virtual void				OnRenderShadowMap( RCameraSceneNode* pCurrentCamera, RShadowStrategy* pShadowStrategy ){}

	virtual void				DeleteManagingChildren();	///< �����ϴ� �ڽĳ����� �����Ѵ�

	virtual void				AddToRenderingList(RRenderingList* pRenderingList);
	virtual void				RemoveFromRenderingList(RRenderingList* pRenderingList);

	//////////////////////////////////////////////////////////////////////////
	// ����� �Ӽ���ӹ��� �������� �аų� �����ϴ� �������� ����ϴ� ���
	//
	std::string					m_strNodeName;				// SceneNode �̸�

	bool						m_bCullingNode;				// �ø��� ������ �Ǵ�������

	int							m_nUpdatedFrame;			// �ڽ��� ������Ʈ �� ������, Update�� �ҷ����� �ߺ��� �����ϴ� �뵵

	RDuplicateUpdateCheckContext	m_TransformUpdateContext;
	RDuplicateUpdateCheckContext	m_VisibilityUpdateContext;
	RDuplicateUpdateCheckContext	m_NoiseRefUpdateContext;

	RMatrixA16*					m_matLocal;					// local transform�� position, rotation(dir,up,right) ���� �̷������, scale���� �����ִ�.
	RMatrixA16*					m_matWorld;					// �������ÿ� ���Ǵ� world transform
	RMatrixA16*					m_matWorldPrev;				// �������ÿ� ���Ǵ� world transform ������ �� 

	RQuaternion					m_qRotWorld;				// ���� �뵵�� ���. �޸� �Ƴ��� �����ͷ� �ؾ� �ұ�...
	RVector						m_vTranWorld;				// ���� �뵵�� ���. �޸� �Ƴ��� �����ͷ� �ؾ� �ұ�...
	RVector						m_vScaleWorld;				// ���� �뵵�� ���. �޸� �Ƴ��� �����ͷ� �ؾ� �ұ�...

	
#ifdef USING_RSC_PHYSIQUE_QT	// ���� ���������� ���� �ʾƵ� �Ǳ� ������ �޸𸮰� �Ʊ���.. �ڵ� ���� �������������� �޸𸮸� �����̶� �Ƴ���
	RQuaternion					m_qRotWorldPrev;			// ���� �뵵�� ���. �޸� �Ƴ��� �����ͷ� �ؾ� �ұ�...
	RVector						m_vTranWorldPrev;			// ���� �뵵�� ���. �޸� �Ƴ��� �����ͷ� �ؾ� �ұ�...
	RVector						m_vScaleWorldPrev;			// ���� �뵵�� ���. �޸� �Ƴ��� �����ͷ� �ؾ� �ұ�...
	RQuaternion					m_qRotLocal;				// ���� �뵵�� ���. �޸� �Ƴ��� �����ͷ� �ؾ� �ұ�...
	RVector						m_vTranLocal;				// ���� �뵵�� ���. �޸� �Ƴ��� �����ͷ� �ؾ� �ұ�...
	RVector						m_vScaleLocal;				// ���� �뵵�� ���. �޸� �Ƴ��� �����ͷ� �ؾ� �ұ�...
#endif

	RVector						m_scale;					// local transform �� scale ��
	bool						m_bUseForPhysiqueBone;		// ������ ���Ǵ� ���� �뵵����. �� �׷����� ���̹��� ������ ���߿� ����. �켱�� ������������ �̽��� ���̹�
	float						m_fSBasecale;					// uniform �������� ��� 
	
	float						m_fVisibility;				// Visibility ��
	float						m_fVisibilityResult;		// ������ Visibility ��

	float						m_fNoiseRef;				/// ������ ���� �׽�Ʈ�� ���۷��� ��
	float						m_fNoiseRefResult;			/// ������ NoiseRef ��

	RSCENENODECONTROLLERLIST	m_sceneNodeControllers[RSNCTRL_MAX];	// ����� ��Ʈ�ѷ� �����̳�
	DWORD						m_dwSceneCount;							// ������ �� ���� Count


	RWorldSceneNode*			m_pRootScene;				// ���� �ڽ��� �����ϴ� Ʈ�� ���� �� ����� ��Ʈ �� ���



// TODO : �̰͵� private ���� �ΰ�ʹ�..
protected:
	RSceneNode*					m_pParent;					// �������� ����
	RSceneNodeList				m_children;					// (����� ������� �θ�/�ڽ� ����)


	

private:
	RRENDERPASS					m_eRenderPass;

	bool						m_bRegisteredCandidate;

	bool						m_bAutoUpdate;				// �ڵ����� �ҷ����� ������Ʈ( Update() )�� �ʿ�����
	bool						m_bAutoUpdateRegistered;	// �ڵ����� �ҷ����� ������Ʈ�� ��ϵǾ�����
	RAutoUpdateNodeListItr		m_itrAutoUpdateItr;			// �ڵ� ������Ʈ ��Ͽ��� ���ﶧ �ѹ��� �����ϱ� ���� ���ͷ����� ����
	bool						RegisterAutoUpdate();
	void						UnregisterAutoUpdate();

	DWORD						m_dwRefreshedTime;			// �� ������ �ȿ��� ������ ���� ������ ������ �� ���� �ִ�. ���� �ð��� ���Ͽ� ������ ���ϵ��� ����. - 090716, OZ

	// TODO: #16828 ���� �ڵ���� ȣȯ���� ���� protected�� �д�. ���� �� �� private�� �����ؾ� �Ѵ�. - charmke
protected:
	RBoundingBox				m_aabb;						// SceneNode AABB, ���� ���� �����ϴ� �� �������� private�� �ξ���. SetAABB()�� ���� �����ؾ� �Ѵ�.
private:
	RSphere						m_sphere;					// ����� �� �ٿ��
	bool						UpdateSpatialTree();		///< �������� Ʈ���� �籸���Ѵ�.
	bool						m_bNeedUpdateBoundingVolume;// �ٿ�� ������ ������Ʈ �ؾ� �ϴ°�

protected:
	void						NeedUpdateBoundingVolume()	{ m_bNeedUpdateBoundingVolume = true; }
	void						SetAutoUpdate(bool bEnable);			// �� �Ŵ������� �����Ǵ� ������Ʈ

public:
	// TODO: #16828 ���� �ڵ���� ȣȯ���� ���� public�� �д�. ���� �� �� �����ؾ� �Ѵ�. - charmke
	void						UpdateBoundingVolume();

public:
	
	RSceneNode(RRENDERPASS eRenderPass = RRP_NORMAL);
	virtual ~RSceneNode();

	//////////////////////////////////////////////////////////////////////////
	// ���� / �б� / ����
	// From XML/To XML �� �� element�� �ش� Scene�� ROOT element�� ���ڷ� �Ѿ�´�.

	virtual bool				CreateFromXML(MXmlElement &element, bool bBackgroundCreation){ return false; }
	virtual void				SaveToXML(MXmlElement &element){}

	bool						LoadCommonProperty(MXmlElement &element);	// �⺻�Ӽ� �б�

	virtual RSceneNode*			Clone()	{ return NULL; }					// ���� ����
	void						Clear();									// ����带 ������ �� ���·� ����� �ֱ�

	void						AddToSceneNodeGroup(RSceneNode* pSceneNode);		// ����� �׷쿡 ���, Deep Deletion ����
	void						RemoveFromSceneNodeGroup(RSceneNode* pSceneNode);	// ����� �׷쿡�� ����, �����۵� �Ҷ�... COMMENT OZ
	RSceneNode*					GetGroupOwner();

	bool						IsNeedHierarchicalUpdate();///< ���� ������Ʈ�� �ؾ� �ϴ��� ����
	int							GetUpdatedFrame();		///< ���� Update �� frame �� ��´�
	void						NeedUpdateTransform();	///< transform �� �ٽ� ����ؾ� �ϴ��� ����
	bool						UpdateTransform();		///< transform �� �ٽ� ����Ѵ�, ���ϰ��� tm �� �ٲ������ ����

	void						NeedUpdateVisibility();	///< Visibility�� �ٽ� ����ؾ� �ϴ��� ����
	virtual	void				UpdateVisibility();

	void						NeedUpdateNoiseRef();	///< NoiseRef�� �ٽ� ����ؾ� �ϴ��� ����
	virtual	void				UpdateNoiseRef();

	void						NeedUpdateAllParentDependentValues();///< �θ� ������ �Ķ���� ��� �ٽ� ����ؾ� �ϴ��� ����
	void						UpdateAllParentDependentValues();	 ///< �θ� ������ �Ķ���͸� ��� ������Ʈ �Ѵ�	

	bool						IsAutoUpdate(){ return m_bAutoUpdate; }	// AutoUpdate����..

	inline bool					IsPrimaryCamRenerable()	{ return m_bPrimaryCamRenderable; }

	bool						IsNeedUpdateBoundingVolume(){ return m_bNeedUpdateBoundingVolume; }

	const RBoundingBox&			GetAABB()	const		{ return m_aabb; }
	// TODO: #16828 ���� �ڵ���� ȣȯ���� ���� �޼��带 �ٽ� ��ȴ�. ���� �� �� �����ؾ� �Ѵ�. - charmke
	RBoundingBox&				GetAABB()				{ return m_aabb; }
	void						SetAABB(const RAABB& aabb)	{ m_aabb = aabb; NeedUpdateBoundingVolume(); }

	//�� ü�� (�������ҿ� ���δ�)
	bool						UseSpatialTree(void);
	const RSphere&				GetSphere()	const				{ return m_sphere; }
	void						SetSphere(RSphere& sphere)		{ m_sphere = sphere; NeedUpdateBoundingVolume(); }
	void						DebugRenderSphere(DWORD dwColor,float plusRadius = 0.0f);

	virtual bool				IsLoadingCompleted()	{ return true; }	// �ε� ��û�� ���� ��� �������� ����

	void						SetRootSceneNode( RWorldSceneNode* p);	//{ m_pRootScene = p;			} + ���� Ʈ�� Ž��
	RWorldSceneNode*			GetRootSceneNode()						{ return m_pRootScene;		}

	//////////////////////////////////////////////////////////////////////////
	// �浹�� picking �� ���� �͵�
	//
	bool						Pick(RPICKINFO& pickInfo);
	// pFarPlane - [0] : RV_FAR, pPlanes - �� ���� Plane�� 
//	virtual bool				FrustumTest(const RPlane *pPlanes, int nNum)	{	return true; }
	virtual void				CollectStaticCollisionPolygons(RSceneNodePolygonCollector* pCollector) const {} // static �浹�޽ø� �������ش�


	//////////////////////////////////////////////////////////////////////////
	// ��� ����
	//
	bool						AddChild(RSceneNode *pChild);
	bool						RemoveChild(RSceneNode *pChild);
	bool						RemoveFromParent();

	void						SetParent(RSceneNode* pParent);
	RSceneNode*					GetParent()			{ return m_pParent; }
	const RSceneNodeList&		GetChildren() const;

	void						ExclusionTraverseRefMesh( RSceneNodeConstVisitor* pVisitor ) const;

	void						Traverse( RSceneNodeVisitor* pVisitor );
	void						Traverse( RSceneNodeConstVisitor* pVisitor ) const;
	void						SerializeToVector( RSerializeTest pSerailTestFunc, std::vector< RSceneNode* >& refSerialedVector ) const ; //�ڽ� ����带 ���ͷ� ����ȭ

	//////////////////////////////////////////////////////////////////////////
	// controllers
	//
	void						AddController(RSceneNodeController* pController);
	bool						RemoveController(RSceneNodeController* pController);
	void						UpdateSceneNodeControllers( RSNCTRL_TYPE eType, RCameraSceneNode* pCamera = NULL );
	void						ReNewContollers();
	void						EnableControllers(bool bEnable);
	RSceneNodeController*		GetFirstControllerByName( RSNCTRL_TYPE eType, const char* szControllerName);
	RSCENENODECONTROLLERLIST*	GetControllers( RSNCTRL_TYPE eType );


	//////////////////////////////////////////////////////////////////////////
	// ����� �Ӽ� : Transform / position / ...
	//
	const RMatrixA16&			GetLocalTransform() const;			// local transform �� ��
	const RMatrixA16&			GetWorldTransform() const;			// �θ� ������ world = local �̴�.
	const RMatrixA16&			GetWorldTransformPrev() const;	

	const RQuaternion&			GetLocalRotation() const;			
	const RVector&				GetLocalTranslation() const;		
	const RVector&				GetLocalScale() const;		

	const RQuaternion&			GetWorldRotation() const;			
	const RVector&				GetWorldTranslation() const;		
	const RVector&				GetWorldScale() const;		

	const RQuaternion&			GetWorldRotationPrev() const;		
	const RVector&				GetWorldTranslationPrev() const;	
	const RVector&				GetWorldScalePrev() const;	

	void						SetTransform(const RMatrix& tm);	// local transform �� ����
	void						SetTransform(const RVector& pos, const RVector& dir, const RVector& up=RVector(0,0,1));

	void						SetPosition(float x,float y,float z);
	void						SetPosition(const RVector& pos);
	void						SetDirection(const RVector& dir, const RVector& up=RVector(0,0,1));
	void						SetRotation(const RQuaternion& quat);

	RVector						GetPosition() const;
	RVector						GetDirection() const;
	RVector						GetRight() const;
	RVector						GetUp() const;

	RVector						GetWorldPosition() const;	// ������ǥ�� ��´�. �������ÿ��� ������������ ��ǥ, ������Ʈ�ÿ��� ������������ ��ǥ�̴�.
	RVector						GetWorldDirection() const;
	RVector						GetWorldRight() const;
	RVector						GetWorldUp() const;
	RVector						GetWorldPositionPrev() const;

	void						SetScale(const RVector& scale);
	const RVector&				GetScale() const;
	float						GetScaleUniform() const;
	RVector						GetScaleFromWorldTransform();

	void						SetNodeName(const std::string &strName)	{	m_strNodeName = strName; }
	const string&				GetNodeName()	const					{	return m_strNodeName;	 }

	void						Message(char* szMsg, void* param);
	DWORD						GetLastSceneCount()	{ return m_dwSceneCount; }
	void						UpdateLastSceneCount(DWORD count)	{	m_dwSceneCount = count; }

	void						SetVisibility(float fVisibility);	// �������ϰ� ���̴� ����
	float						GetVisibility();
	float						GetVisibilityResult() const;	///< �θ�κ��� ������ ������

	void						SetNoiseRef(float fVisibility);	// ������ ���̵� �ƿ� ��
	float						GetNoiseRef();
	float						GetNoiseRefResult();	///< �θ�κ��� ������ ������ ���̵� �ƿ� ��

	inline void					SetUseForPhysiqueBone( bool b = true)	{ m_bUseForPhysiqueBone = b;	}	
	inline bool					IsUseForPhysiqueBone()					{ return m_bUseForPhysiqueBone;	}	

	bool						IsCullingNode()							{ return m_bCullingNode;		}
	bool						IsRegisteredCandidate()					{ return m_bRegisteredCandidate;}
	void						SetRegisteredCandidate( bool b)			{ m_bRegisteredCandidate = b;	}

	void						SetAttached( bool b)					{ m_bAttached = b;				}
	bool						IsAttached()							{ return m_bAttached;			}

	//////////////////////////////////////////////////////////////////////////
	// Usable : ������Ʈ, ���� ���� / Visible : ���� ����
	//
	inline void					SetVisible(bool bFlag)	{ m_bVisible = bFlag; }	//���̰� �Ⱥ��̰� ����
	virtual bool				GetVisible() const		{ return m_bVisible && !GetOcclued() && IsLODCullingPassed(); }

	inline void					SetOcclued(bool bFlag)	{ m_bOccluded = bFlag; }	
	virtual bool				GetOcclued() const		{ return m_bOccluded; }


	inline  void				SetUsable(bool bFlag)	{ m_bUsableSceneNode = bFlag; }
	virtual  bool				GetUsable()	const		{ return m_bUsableSceneNode; }

	//////////////////////////////////////////////////////////////////////////
	// Render
	//
	void PreRender(RCameraSceneNode *pCamera);
	void PostRender(RCameraSceneNode *pCamera);

	virtual void RenderAlpha( RCameraSceneNode *pCamera,int iSubMaterial, RRENDERINGMETHOD eMothod){}
	virtual void Render( RCameraSceneNode* pCurrentCamera, int nSubMaterialID, RRENDERINGMETHOD renderMothod){ OnRender( pCurrentCamera, nSubMaterialID,renderMothod );}
	virtual void RenderGStage( RCameraSceneNode* pCurrentCamera, int nSubMaterialID, RRENDERINGMETHOD renderMothod){ OnRenderGStage( pCurrentCamera, nSubMaterialID,renderMothod );}
	virtual void RenderMStage( RCameraSceneNode* pCurrentCamera, int nSubMaterialID, RRENDERINGMETHOD renderMothod){ OnRenderMStage( pCurrentCamera, nSubMaterialID,renderMothod );}
	virtual void RenderZStage( RCameraSceneNode* pCurrentCamera, int nSubMaterialID, RRENDERINGMETHOD renderMothod){ OnRenderZStage( pCurrentCamera, nSubMaterialID,renderMothod );}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����������
	//
	void						SetUserData(void *ptr)	{	m_pUserData = ptr; }
	void*						GetUserData()	const	{	return m_pUserData; }

	void						AddAABB( RBoundingBox* _pAABB );

	//////////////////////////////////////////////////////////////////////////
	// ����������Ƽ
	//
	void						PushUserProperty(const string& strPropertyName, const string& strValue);
	void						RemoveUserProperty(unsigned int index);
	const string&				GetUserPropertyName(unsigned int index) const;
	const string&				GetUserPropertyValue(unsigned int index) const;
	void						SetUserPropertyValue(unsigned int index, const string& strValue);
	unsigned int				GetUserPropertyCount(void) const	{ return m_vecUserProperty.size(); }

private:

	//////////////////////////////////////////////////////////////////////////
	// Update / Render Flag
	//
	bool						m_bUsableSceneNode;			///< ������Ʈ + ������ ����
	bool						m_bVisible;					///< ������ ����
	bool						m_bOccluded;				///< 
	bool						m_bAttached;				///< �ű׷����� �޷��ִ��� ����

	bool						m_bPrimaryCamRenderable;	///< Primary Camera ������ ����


	//////////////////////////////////////////////////////////////////////////
	// ����� �Ӽ�
	//
	void*						m_pUserData;				///< ����� ���� ������

	//////////////////////////////////////////////////////////////////////////
	// ����� �׷�, Deep Delete ����
	//
	unsigned int				m_nManagingSceneNodeCount;
	RSceneNode*					m_pSceneNodeGroupOwner;


	//////////////////////////////////////////////////////////////////////////
	// clear - �� ������ ����� �ֱ�
	//

	// ����� �׷��� ���� ������ �ڽİ� �������� ���� �ڽ����� ����
	class RSceneNodeChildrenCleanUpVisitor : public RSceneNodeVisitor
	{
	public:
		RSceneNodeChildrenCleanUpVisitor(){}
		virtual ~RSceneNodeChildrenCleanUpVisitor(){}

		virtual bool			OnPreOrderVisit(RSceneNode* pSceneNode );
		virtual void			OnPostOrderVisit(RSceneNode* pSceneNode ){}

		RSceneNode*				m_pRequestSceneNode;
		std::vector< RSceneNode* > m_vecNodeToChangeParent;
	};
	friend class RSceneNodeChildrenCleanUpVisitor;

	//////////////////////////////////////////////////////////////////////////
	// ����������Ƽ
	//
	vector<pair<string, string>> m_vecUserProperty;
};
//----------------------------------------------------------------------------------------------------
inline void RSceneNode::NeedUpdateTransform()	{ m_TransformUpdateContext.NeedUpdate(); }
inline void RSceneNode::NeedUpdateVisibility()	{ m_VisibilityUpdateContext.NeedUpdate(); }
inline void RSceneNode::NeedUpdateNoiseRef()	{ m_NoiseRefUpdateContext.NeedUpdate(); }
//----------------------------------------------------------------------------------------------------
inline int RSceneNode::GetUpdatedFrame()		{ return m_nUpdatedFrame; }
//----------------------------------------------------------------------------------------------------
inline const RMatrixA16& RSceneNode::GetLocalTransform() const					{ return (*m_matLocal);	}
inline const RMatrixA16& RSceneNode::GetWorldTransform() const					{ return (*m_matWorld);	}
inline const RMatrixA16& RSceneNode::GetWorldTransformPrev() const				{ return (*m_matWorldPrev);}	

inline const RQuaternion& RSceneNode::GetWorldRotation() const				{ return m_qRotWorld;	}	
inline const RVector& RSceneNode::GetWorldTranslation() const				{ return m_vTranWorld;	}	
inline const RVector& RSceneNode::GetWorldScale() const						{ return m_vScaleWorld;	}
//----------------------------------------------------------------------------------------------------

#ifdef USING_RSC_PHYSIQUE_QT
inline const RQuaternion& RSceneNode::GetLocalRotation() const				{ return m_qRotLocal;	}	
inline const RVector& RSceneNode::GetLocalTranslation() const				{ return m_vTranLocal;	}	
inline const RVector& RSceneNode::GetLocalScale() const						{ return m_vScaleLocal;	}
inline const RQuaternion& RSceneNode::GetWorldRotationPrev() const			{ return m_qRotWorldPrev;	}	
inline const RVector& RSceneNode::GetWorldTranslationPrev() const			{ return m_vTranWorldPrev;	}	
inline const RVector& RSceneNode::GetWorldScalePrev() const					{ return m_vScaleWorldPrev;	}	
#endif
//----------------------------------------------------------------------------------------------------
inline const RSceneNodeList& RSceneNode::GetChildren() const	{ return m_children; }
//----------------------------------------------------------------------------------------------------
inline void RSceneNode::SetVisibility(float fVisibility)
{
	_ASSERT(fVisibility>=0.0f);
	m_fVisibility = fVisibility;
	NeedUpdateVisibility();
}
//----------------------------------------------------------------------------------------------------
inline float RSceneNode::GetVisibility()						{ return m_fVisibility; }
//----------------------------------------------------------------------------------------------------
inline float RSceneNode::GetVisibilityResult() const			{ return m_fVisibilityResult; }
//----------------------------------------------------------------------------------------------------
inline void RSceneNode::SetNoiseRef(float fRef)
{
	_ASSERT(fRef>=0.0f);
	m_fNoiseRef = fRef;
	NeedUpdateNoiseRef();
}
//----------------------------------------------------------------------------------------------------
inline float RSceneNode::GetNoiseRef()						{ return m_fNoiseRef; }
//----------------------------------------------------------------------------------------------------
inline float RSceneNode::GetNoiseRefResult()					{ return m_fNoiseRefResult; }
//----------------------------------------------------------------------------------------------------
inline RVector RSceneNode::GetWorldPosition() const				{ return RVector(m_matWorld->_41,m_matWorld->_42,m_matWorld->_43); }
//----------------------------------------------------------------------------------------------------
inline RVector RSceneNode::GetWorldRight() const				{ return RVector(m_matWorld->_11, m_matWorld->_12, m_matWorld->_13); }
//----------------------------------------------------------------------------------------------------
inline RVector RSceneNode::GetWorldDirection() const			{ return RVector(m_matWorld->_21,m_matWorld->_22,m_matWorld->_23); }
//----------------------------------------------------------------------------------------------------
inline RVector RSceneNode::GetWorldUp() const					{ return RVector(m_matWorld->_31,m_matWorld->_32,m_matWorld->_33); }
//----------------------------------------------------------------------------------------------------
inline RVector RSceneNode::GetWorldPositionPrev() const			{ return RVector(m_matWorldPrev->_41,m_matWorldPrev->_42,m_matWorldPrev->_43); }
//----------------------------------------------------------------------------------------------------
inline void RSceneNode::SetPosition(float x,float y,float z)	{ SetPosition(RVector(x,y,z)); }
//----------------------------------------------------------------------------------------------------
inline void RSceneNode::SetPosition(const RVector& pos)
{

#ifdef USING_RSC_PHYSIQUE_QT
	if( IsUseForPhysiqueBone())
		m_vTranLocal = pos;
#endif

	m_matLocal->_41 = pos.x; m_matLocal->_42 = pos.y; m_matLocal->_43 = pos.z;

	NeedUpdateTransform();
}
//----------------------------------------------------------------------------------------------------
inline void RSceneNode::SetScale(const RVector& scale)	{
	m_scale = scale; 	NeedUpdateTransform(); 
#ifdef USING_RSC_PHYSIQUE_QT
	m_vScaleLocal = scale;
#endif
}
//----------------------------------------------------------------------------------------------------
inline float RSceneNode::GetScaleUniform() const	{ return m_scale.z;}		
//----------------------------------------------------------------------------------------------------
inline RVector RSceneNode::GetPosition() const		{ return RVector(m_matLocal->_41,m_matLocal->_42,m_matLocal->_43); }
//----------------------------------------------------------------------------------------------------
inline RVector RSceneNode::GetDirection() const		{ return RVector(m_matLocal->_21,m_matLocal->_22,m_matLocal->_23); }
//----------------------------------------------------------------------------------------------------
inline RVector RSceneNode::GetRight() const			{ return RVector(m_matLocal->_11,m_matLocal->_12,m_matLocal->_13); }
//----------------------------------------------------------------------------------------------------
inline RVector RSceneNode::GetUp() const			{ return RVector(m_matLocal->_31,m_matLocal->_32,m_matLocal->_33); }
//----------------------------------------------------------------------------------------------------
inline void RSceneNode::AddToSceneNodeGroup(RSceneNode* pSceneNode) // ����� �׷쿡 ���
{
	_ASSERT( pSceneNode->m_pSceneNodeGroupOwner == NULL );
	++m_nManagingSceneNodeCount;
	pSceneNode->m_pSceneNodeGroupOwner = this;
}
//----------------------------------------------------------------------------------------------------
inline void RSceneNode::RemoveFromSceneNodeGroup(RSceneNode* pSceneNode) // ����� �׷쿡 ���
{
	if( pSceneNode->m_pSceneNodeGroupOwner == this)
	{
		--m_nManagingSceneNodeCount;
		pSceneNode->m_pSceneNodeGroupOwner = NULL;
	}
}
//----------------------------------------------------------------------------------------------------
inline RSceneNode* RSceneNode::GetGroupOwner()
{
	return m_pSceneNodeGroupOwner;
}
//----------------------------------------------------------------------------------------------------
inline RRENDERPASS RSceneNode::GetUsingPass() const	{ return m_eRenderPass; }
//----------------------------------------------------------------------------------------------------
inline RSCENENODECONTROLLERLIST* RSceneNode::GetControllers( RSNCTRL_TYPE eType ) { return &m_sceneNodeControllers[eType]; }

}

#endif
