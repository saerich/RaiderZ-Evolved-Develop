#pragma once

#include "RTypes.h"
#include "RSceneNode.h"
#include "RShaderComponentID.h"
#include "RLoadingWorkUnit.h"
#include "RMaterialResource.h"
#include "RResourceLoader.h"
#include "MMemPool.h"
//#include "RTechnique.h"
#include "RStlCustomAlloc.h"


namespace rs3 {

class RMeshNode;
class RActor;
class RActorPassRenderer;
class RVertexMotionBlurPassRenderer;
class RPixelMotionBlurPassRenderer;
class RAccumMotionBlurPassRenderer;
class RActorCollision;
class REnvironmentSet;
class RAnimation;
class RAnimationNode;
class RActorController;
class RActorNodeController;
// class RAnimationController;
// class RAnimationControllerNode;
class RActorAlignController;

class RLightMapPacker;
class REffectSceneNode;
class RMaterialResource;
class RShaderFX;
class MCollision;
class RActorNodeConstraintController;
class RAdditivePassRenderer;
class RActorNodeRenderStrategy;
struct ComponentDesc_GStage;
struct ComponentDesc_MStage;

enum RRENDERINGMETHOD;

RS_API extern const char *RSID_ACTORNODE;

///////////////////////////////////////////////////
// 
typedef list<RActorNodeController*> RACTORNODECONTROLLERLIST;

enum ACTOR_NODE_TYPE
{
	ACTOR_NODE_COMMON = 0,
	ACTOR_NODE_PHYSIQUE,
	ACTOR_NODE_CLOTH,

	ACTOR_NODE_TYPE_NUM
};


enum PIXEL_MOTION_BLUR_MODE
{
	PMB_NONE = 0,
	PMB_HALF,
	PMB_FULL,
	PMB_MODE,
};

class RS_API RActorNode : public RSceneNode, public RResource::LoadingController
{
	MDeclareRTTI;

	friend RActor;
	friend RActorPassRenderer;
	friend RVertexMotionBlurPassRenderer;
	friend RPixelMotionBlurPassRenderer;
	friend RAccumMotionBlurPassRenderer;
	friend RActorController;
	friend RAdditivePassRenderer;
// 	friend RAnimationController;
// 	friend RAnimationControllerNode;
public:
	DECLARE_NODE_ID_STRING(RSID_ACTORNODE);

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� / �Ҹ�
	//
	bool							Create();				///< ����, Device Independent
	void							Destroy();
	virtual void					OnDestroy();

	virtual void					OnAdd();				// ���Ϳ� �������ų� ������ �Ҹ���� ���
	virtual void					OnRemove();

	//////////////////////////////////////////////////////////////////////////
	// �ε�
	//
	virtual bool					OnCreateDeviceIndependentPart();	///< ��帶�� �ٸ� ����
	virtual bool					OnCreateDeviceDependentPart();		///< ����̽� �������� ���ҽ� �����
	bool							OnCreateMaterials(bool bBackground = false);	///< Material ����
	virtual bool					IsLoadingCompleted();
//	void							UpdateLoadingPriority();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���ͳ�� �Ӽ�
	//
	ACTOR_NODE_TYPE					GetActorNodeType()	{ return m_eActorNodeType; }

	// BoneReferenceCount ����
	bool							CreateBoneReferenceTable();
	void							DestroyBoneReferenceTable();
	int								GetBoneRefCount()	{ return m_nBoneRefCount; }
//	float							GetBoneImportance()	{ return m_fBoneImportance; }

	// ������ ���ͳ���� ���
	bool							HasRefBone()		{ return (m_refBones.empty() == false); }

	virtual bool					Validate();						// material, �� ���� �˻��Ͽ� �̻��� �ִ��� ����
	virtual bool					GetVisible()	const	override;
	virtual bool					GetUsable()		const	override;	

	//////////////////////////////////////////////////////////////////////////
	// Parts Color
	void							SetPartsColor(DWORD dwPartsColor);
	DWORD							GetPartsColor();

	/////////////////////////////////////////////////////////////////////////
	// Noise Fade out
	void							SetNoiseRefColor1( float fSize, DWORD dwSectionColor);
	void							SetNoiseRefColor2( float fSize, DWORD dwSectionColor);
	bool							IsApplyNoise() { return m_fNoiseRefResult<1.0f; }

	// �ִϸ��̼� ��Ʈ�� ����
	const RMatrix&					GetResultTransform();				// ���� �ִϸ��̼� ��� ��� ��´�
	const RQuaternion&				GetResultRotation();
	const RVector3&					GetResultTranslation();
	const RVector3&					GetResultScale();
	RQuaternion						GetRotation();						// ���� �ִϸ��̼� ȸ������ ��´�

//	RAnimationControllerNode*		GetAnimationNodeController() { return m_pAnimationController; }

	const string&					m_strName;				// �����â���� ���� ������ ���������� �ʿ����
	RMeshNode*						m_pMeshNode;					// �ڽŰ� ����� ���. RActor::NewActorNode(). ü�� ������ key ���� �� �ϳ��ε� ��� ��.
//	RActorNode*						m_pParent;						// �θ�
//	RActorNode*						m_pCurrentParent;				// ����ִϸ��̼��� �θ�, Ư�� �ִϸ��̼ǿ����� �޶������� �ִ�
//	vector<RActorNode*>				m_refBones;						// �����ϴ� �� ���̺�
	vector<RActorNode*,RStlCustomAlloc<RActorNode*>>				m_refBones;	// �����ϴ� �� ���̺�
	vector<RMatrix,RStlCustomAlloc<RMatrix>>					m_matBones;						// �� ��Ʈ����
	vector<RMatrix,RStlCustomAlloc<RMatrix>>					m_matBoneBases;					// ���� base * �� inv base
#ifdef USING_RSC_PHYSIQUE_QT
	
	vector<RQuaternion,RStlCustomAlloc<RQuaternion>>				m_quatBoneBaseRots;					// ���� base * �� inv base -> �� ��Ʈ������ ���� ���ʹϾ� 
	vector<RVector4,RStlCustomAlloc<RVector4>>				m_vBoneBaseTrans;					// ���� base * �� inv base -> �� ��Ʈ������ ���� �̵��� 	Vector4�� ���� - 090722, OZ
	vector<RVector3,RStlCustomAlloc<RVector3>>				m_vBoneBaseScales;					// ���� base * �� inv base -> �� ��Ʈ������ ���� �����ϰ� 
#endif

#ifdef USING_RSC_PHYSIQUE_QT
	vector<RQuaternion,RStlCustomAlloc<RQuaternion>>		m_quatBoneRots;					// �� ��Ʈ������ ���� ���ʹϾ� 
	vector<RVector4,RStlCustomAlloc<RVector4>>				m_vBoneTrans;					// �� ��Ʈ������ ���� �̵��� 	Vector4�� ���� - 090722, OZ
	vector<RQuaternion,RStlCustomAlloc<RQuaternion>>		m_quatBoneRotsPrev;				// �� ��Ʈ������ ���� ���ʹϾ� 
	vector<RVector4,RStlCustomAlloc<RVector4>>				m_vBoneTransPrev;				// �� ��Ʈ������ ���� �̵��� 	Vector4�� ���� - 090722, OZ
#endif
#ifdef USING_RSC_PHYSIQUE_DQ
	vector<RDQuaternion,RStlCustomAlloc<RQuaternion>>			m_dqBoneDQ;						// �� ��Ʈ������ ��� ���ʹϾ� 
#endif
	// tip : Decompose() ���



	RActor*							m_pActor;						// frame ��..�������� �����ϱ� ����. �ڽ��� ���ʸ� ����

	vector<RMaterialResource*,RStlCustomAlloc<RMaterialResource*>>		m_vecMaterialTable;				// �������� �ٸ��� ����ϱ� ���� MeshNode�� ���� ������ �����ؼ� ������. 

	RActorNode*						GetParentActorNode();

	int								AddBoneRef();
	int								ReleaseBoneRef();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������Ʈ ����
	//
	virtual void					ResetTransform();
	RMatrix							GetBaseLocalTransform();		// ���� �޽��� base Ʈ�������� ��´�
//	void							UpdateWorldTransform();

	virtual bool					OnPick(RPICKINFO& pickInfo);

	void							UpdateLocalAnimation();			// �ִϸ��̼��� ������Ʈ�Ѵ�
	void							ForceUpdateAnimation();			// �ִϸ��̼��� ������ (�θ� �����Ͽ�) ������Ʈ�Ѵ�

	void							ReserveUpdateVertexBuffer(RAnimationNode* pAniNode, int nFrame); // ���ؽ� �ִϸ��̼��̳� soft ��Ų������ vb ������Ʈ�� ����

	void							GetLocalTransformAt(DWORD nFrame, RMatrix& outMatrix);	///< Ư�� �������� local tm �� ��´�
	void							GetWorldTransformAt(DWORD nFrame, RMatrix& outMatrix);	///< Ư�� �������� world tm �� ��´�
	void							GetResultTransformAt(DWORD nFrame, RMatrix& outMatrix);	///< Ư�� �������� world tm �� ��´�


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������ ����
	//

	/// �������ϱ� �� vertex ������ ������Ʈ�� �ʿ��Ѱ��. �� actornode �������� �ѹ� �Ҹ���
	virtual void					UpdateTransformAndVertex();
	/// vertex buffer, index buffer, vertex format ����
	virtual void					SetVertexStream(  bool b2ndBufferMng = true);
	/// node ���� ���� ���� ����� �Ұ�
	virtual void					SetNodeRenderStateForEdge( RCameraSceneNode* pCurrentCamera);
	virtual void					SetNodeRenderState(const RMatrix& matView, const RMatrix& matViewProj);
	virtual void					SetNodeRenderStateForBlur(  RCameraSceneNode* pCurrentCamera);
	virtual void					SetBlendMaterialRenderState();

	virtual void					RenderPrimitive(int index);		// ���׸��� ���ߵ� ���ؽ��� DP
	
	virtual void					RenderAllNodePrimitive();

	virtual	void					UpdateVisibility() override;

	virtual	void					UpdateNoiseRef() override;

	bool							CheckInstancingCondition();		// �ν��Ͻ� ��� ������ ���������� üũ

	bool							IsVertexMotionBlur(void);		
	float							GetMotionBlurBasisFrame(void);	
	bool							IsAccumMotionBlur(void);			
	bool							IsPixelMotionBlur(void);			
	bool							IsPEEdge(void);

	void							RenderSingle(  RCameraSceneNode* pCurrentCamera, RActorNodeRenderStrategy* pRenderStrategy );
	void							RenderSingleOneMaterial( RCameraSceneNode* pCurrentCamera, int nSubMaterialID, RActorNodeRenderStrategy* pRenderStrategy);

	bool							IsRenderable(void);
	bool							CheckDoRender();

	bool							PreCreateShader( bool bBackgroundCreation );


	RShaderFX*						CheckAndCreateTechniqueTableOnMaterialForGStage( RRENDERINGMETHOD renderMethod, int nSubMaterialID, bool bInstancing/* = false*/, bool bNoiseRef, bool bBackgroundCreation/* = true*/);
	RShaderFX*						CheckAndCreateTechniqueTableOnMaterialForMStage( RRENDERINGMETHOD renderMethod, int nSubMaterialID, bool bInstancing/* = false*/, bool bNoiseRef, bool bBackgroundCreation/* = true*/);

	RSCID							GetBaseComponent() { return m_baseComponents; }

	ComponentDesc_GStage			GetGStageComponentDesc(int nSubMaterialID, bool bInstancing, bool bNoiseRef);
	ComponentDesc_MStage			GetMStageComponentDesc(bool bAddColorPass, int nSubMaterialID, bool bInstancing, bool bNoiseRef);
	ComponentDesc_MStage			GetMaskingPassComponentDesc(int nSubMaterialID);


public:
	virtual void					RenderNormal(DWORD dwColor = 0xffff0000);	// ����� ���� : ���
	virtual void					RenderWire(DWORD dwColor = 0x80ffffff);		// ����� ���� : ���̾�
	virtual void					OnRender(RCameraSceneNode* pCurrentCamera, int nSubMaterialID, RRENDERINGMETHOD renderMothod) override;
	virtual void					RenderAlpha( RCameraSceneNode *pCamera, int iSubMaterial,RRENDERINGMETHOD eMothod) override;

	void							RenderShadowMap( RCameraSceneNode* pCurrentCamera, RShadowStrategy* pShadowStrategy, RActorNodeRenderStrategy* pRenderStrategy );


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���׸��� ����
	//
	bool							GetSubMaterialMask(int nSubMaterial);
	void							SetSubMaterialMask(int nSubMaterial,bool bShow);
	void							SetAllSubMaterialMask(bool bShow);
	RMaterial*						GetSubMaterial(int nIndex) { return m_vecMaterialTable.at(nIndex)->m_pMaterial; }

	// ���� ����
	bool							m_bBlendingMaterial;			///< ������ Ȱ��ȭ �Ǿ��ִ��� ����
	int								m_nBlendingMaterialID;
	RMaterial*						m_pBlengingMaterial;		///< ���� �� ���� ����
	float							m_fBlendingFactor;			///< DstFactor( Factor ) * AS_BLENDING_MATERIALMAP + SrcFactor( 1 - Factor ) * DIFFUSE
	float							m_fBlendingTime;			///< �� ���� �ð�
	float							m_fBlendingAccTime;			///< ���� �ð� 
	// !�ܺο��� ���� ����ϴ� �Լ��� �ƴϴ� // RActor::ChangeMatetial �� ActorNode�� ���ؼ� �������
	void							ChangeMtrlSet(string strSrcMtrl, string strDstMtrl, float fBlendingTime = 0.3f);
	void							ChangeMtrlSet(int nSrcMtrl, int nDstMtrl, float fBlendingTime = 0.3f);
	void							ChangeMtrlSet(int nSrcMtrl, string strDstMtrl, float fBlendingTime = 0.3f);

	bool							HasTranslucentMaterial();	// ������ ������ �������ֳ�
	bool							HasAlphaMaterial();			// ���� (additive �� blending) material�� �����ϴ°�

	bool							IsUsePhysiue();
	bool							IsUseTexCoord();
	bool							IsUseExtraTexCoord();
	bool							IsUsePointColor();

	bool							IsBlendingMaterial()	{ return m_bBlendingMaterial; }

	bool							IsNoShade();
	bool							IsUseAddColor(RMaterial* pMaterial);

	bool							IsAlpha();

	const RSCID&					GetShaderIDGStage(int nSubMaterialID) const;
	const RSCID&					GetShaderIDMStage(int nSubMaterialID) const;

	float							GetIlluminationScale();



	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���õ� ActorNodeConstraintController
	//
 	void							RemoveRelatedController( RActorNodeConstraintController* pController );
 	void							AddRelatedController( RActorNodeConstraintController* pController );

	//////////////////////////////////////////////////////////////////////////
	// ������Ʈ
	void							UpdateFinalResultTransforms();

	bool							GetAddColorState()	{ return m_bAddColor; }

	virtual void					SetShaderTransformConstant(const RMatrix& matView, const RMatrix& matViewProj);

	bool							CanInstancing() { return m_bCanInstancing; }

protected:
	explicit						RActorNode(ACTOR_NODE_TYPE eActorNodeType);
	virtual ~RActorNode();

	bool							PickLocal(const RVector &origin,const RVector &to,RPICKINFO *pPickInfo);

	RMatrix							m_matResult;			// ���� �ִϸ��̼��� �θ�κ��� ������ ���
#ifdef USING_RSC_PHYSIQUE_QT
	RVector3						m_vTranResult;
	RQuaternion						m_qRotResult;
	RVector3						m_vScaleResult;
#endif

	vector<RSCID,RStlCustomAlloc<RSCID>> m_vecGStageRSCIDs;
	vector<RSCID,RStlCustomAlloc<RSCID>> m_vecMStageRSCIDs;

	
	RSCID							m_baseComponents;		///< ��ü�� �׸��� ���� �⺻ ���̴� ������Ʈ

	DWORD							m_dwPartsColor;			///< Parts Color

	DWORD							m_dwNoiseRefColor1;
	float							m_fNoiseRefSection1;
	DWORD							m_dwNoiseRefColor2;
	float							m_fNoiseRefSection2;

	bool							m_bCanInstancing;

//	RShaderFX*						GetShader(unsigned int nMtrlIndex, bool bMstage);
	void							GetMeshCollision(std::vector<MCollision*>& _rOut, RActorCollision* _pCollsion);

	void							EnableAddColorEffect(const RVector3& color);
	void							DisableAddColorEffect();


	//void							UpdateTechniqueTableForGStage(int nSubMaterialID, bool bInstancing);
	//void							UpdateTechniqueTableForMStage(int nSubMaterialID, bool bInstancing);

//	virtual void					OnAddedToActor();							///< ���ͳ�忡 ��������

private:

	bool							m_bAddColor;
	RVector3						m_vAddColor;


	bool							m_bCreated;
	ACTOR_NODE_TYPE					m_eActorNodeType;
	vector<bool,RStlCustomAlloc<bool>>					m_subMaterialMask;

	int								m_nBoneRefCount;		// bone ���۷��� ī��Ʈ
//	float							m_fBoneImportance;		// ���� �߿䵵

//	RAnimationControllerNode*		m_pAnimationController;
	RSceneNodeController*			m_pAlignController;
	int								m_nUpdateVertexBufferFrame;	// vertex ani ������Ʈ �ؾ� �� ������
	RAnimationNode*					m_pUpdateVertexAniNode;		// vertex ������Ʈ�� �޾� �� AnimationNode

	RActorNode*						m_pNextRenderChain;		// ���� ����ü��
	std::list<RActorNodeConstraintController*,RStlCustomAlloc<RActorNodeConstraintController*>>	m_relatedControllers;	// �����Ǿ��ִ� ��Ʈ�ѷ���

	float							m_fIlliminationScale;	// Self illumination Scale
	void							SetIlluminationScale(const float fScale);	

	bool							CheckRenderPerMaterialToShadowMap();
	void							SetAddEffectComponents( RSCID& _rscComponents );

	bool							IsApplyMaterialDecal(bool bTexCoord, RMaterial* pMaterial);
};

//----------------------------------------------------------------------------------------------------
inline const RMatrix& RActorNode::GetResultTransform()		{ return m_matResult; }
#ifdef USING_RSC_PHYSIQUE_QT
inline const RVector3& RActorNode::GetResultTranslation()	{ return m_vTranResult; }
inline const RQuaternion& RActorNode::GetResultRotation()	{ return m_qRotResult; }
inline const RVector3& RActorNode::GetResultScale()			{ return m_vScaleResult; }
#endif
//----------------------------------------------------------------------------------------------------



inline bool RActorNode::GetSubMaterialMask(int nSubMaterial)
{
	if( m_subMaterialMask.empty() )
//		return true;	// �⺻������ true�� �����ϸ� ��׶��� �ε� ������ �׸��� ����
		return false;

	if(nSubMaterial<0)
		return true;

	return m_subMaterialMask[nSubMaterial];
}
//----------------------------------------------------------------------------------------------------
inline void RActorNode::SetPartsColor(DWORD dwPartsColor)	{	m_dwPartsColor = dwPartsColor;	}
inline DWORD RActorNode::GetPartsColor()					{	return m_dwPartsColor;	}
//----------------------------------------------------------------------------------------------------
inline void RActorNode::SetNoiseRefColor1( float fSize, DWORD dwSectionColor)	{ m_dwNoiseRefColor1 = dwSectionColor; m_fNoiseRefSection1 = fSize; }
inline void RActorNode::SetNoiseRefColor2( float fSize, DWORD dwSectionColor)	{ m_dwNoiseRefColor2 = dwSectionColor; m_fNoiseRefSection2 = fSize; }
//----------------------------------------------------------------------------------------------------
inline void RActorNode::ReserveUpdateVertexBuffer(RAnimationNode* pAniNode, int nFrame)	// ���ؽ� �ִϸ��̼��̳� soft ��Ų������ vb ������Ʈ�� ����
{
	m_nUpdateVertexBufferFrame = nFrame;
	m_pUpdateVertexAniNode = pAniNode;
}
//----------------------------------------------------------------------------------------------------
inline RActorNode* RActorNode::GetParentActorNode() 
{
	_ASSERT(m_pParent!=NULL);
	if((RSceneNode*)m_pActor==m_pParent)
		return NULL;
	return (RActorNode*)m_pParent;
}

inline void RActorNode::SetIlluminationScale(const float fScale)	{ m_fIlliminationScale = fScale; }
//----------------------------------------------------------------------------------------------------
inline float RActorNode::GetIlluminationScale()						{ return m_fIlliminationScale; }


class RS_API RActorNodeMLeaf : public RActorNode, public MMemPool<RActorNodeMLeaf, 50000>
{
	MDeclareRTTI;
public:
	RActorNodeMLeaf(ACTOR_NODE_TYPE eActorNodeType) : RActorNode(eActorNodeType) {}
	virtual ~RActorNodeMLeaf() {}
};

}
