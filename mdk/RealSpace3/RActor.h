#ifndef _RActor_h
#define _RActor_h

#include "RVType.h"
#include "RSceneNode.h"
#include "RSkeleton.h"
#include "RActorNode.h"
#include "RAnimationController.h"
#include "RResource.h"
#include "RLODContext.h"
#include "MemPool.h"
#include "RMaterialDecal.h"
#include <unordered_map>

namespace rs3 {

RS_API extern const char *RSID_ACTOR;
RS_API extern const char *RSID_EFFECT; 

class RActorNode;
class RPhysXClothActorNode;
class RSkeleton;
class RActorController;
// class RBlendAnimationController;
class RActorPassRenderer;
class RVertexMotionBlurPassRenderer;
class REdgePassRenderer;
class RPixelMotionBlurPassRenderer;
class RAccumMotionBlurPassRenderer;
class RSceneManager;
class RShaderEffectController;
class RMesh;
class RMeshNode;
class RCullingMgr;

class REngine;

class RListener; // �ִϸ��̼� �̺�Ʈ
class RDistortionParameter;

typedef list<RActorController*> RACTORCONTROLLERLIST;
typedef map<string,RActorNode*> RACTORNODEMAP;

// TODO: ���� ����
class REffectSceneNode;
class RSceneSky;
class RActorNodeBuilder;

enum RANIMATIONTRANSITIONTYPE;


// COMMENT OZ - ����!! ĳ���� �� ��� �ִϸ��̼��� ������ �޽�
////////////////////////////////////////////////////////////////////////////////////////////////////
// ĳ���� �� ��� �ִϸ��̼��� ������ �޽�
//

class RS_API RActor : public RSceneNode, public RResource::LoadingController
{
	friend RActorNode;
	friend RActorController;	// for RefreshAABB
	friend RActorPassRenderer;	// for render factor setting
	friend RVertexMotionBlurPassRenderer;
	friend REdgePassRenderer;
	friend RPixelMotionBlurPassRenderer;
	friend RAccumMotionBlurPassRenderer;
	friend RSceneManager;		// for shader creation
	friend REngine;
	friend RCameraSceneNode;
	friend RCullingMgr;

	// ���� ���ͳ�忡 ���� ���ͳ��(�ڱ��ڽ� ����)���� �з��ϴ� ������
	class CActorNodeDeletionVisitor : public RSceneNodeVisitor
	{
	public:
		CActorNodeDeletionVisitor(RActor* _pCaller) : m_pCaller(_pCaller) {}
		virtual ~CActorNodeDeletionVisitor(){}

		// post order �� traversal �Ҳ���
		virtual bool OnPreOrderVisit( RSceneNode* pSceneNode ){ return true; }
		virtual void OnPostOrderVisit( RSceneNode* pSceneNode )
		{
			RActorNode* pActorNode = static_cast<RActorNode*>(pSceneNode);
			if (pActorNode->m_pActor == m_pCaller)
				m_vecActorNodesToDelete.push_back(pActorNode);
		}

		std::vector<RActorNode*>& GetDeleteNodes() { return m_vecActorNodesToDelete; }

	private:
		std::vector<RActorNode*> m_vecActorNodesToDelete;
		RActor* m_pCaller;
	};

public:
	class RS_API RActorNodeBuilder
	{
	public:
		struct RADDMESHNODE {
			RMeshNode*	pMeshNode;
			RActorNode* pParentNode;
			const char* szActorNodeName;
			bool		bBackground;
		};

		RActorNodeBuilder(RActor* pActor);
		~RActorNodeBuilder();

		void DeleteActorNode(RActorNode* pActorNode);
		void DeleteActorNode(const char* szNodeName);
		void AddMeshNode(RMeshNode* pMeshNode, RActorNode* pParent, const char* szActorNodeName = NULL, bool bBackground = false);

		void Build();

	protected:
		RActor* m_pActor;

		std::vector<RADDMESHNODE*>	m_vecAddMeshList;
		std::vector<RActorNode*>	m_vecDeleteList;
	};
	
	// how to use
	/*
	RActor actor;
	RActor::RActorNodeBuilder builder(&actor);
	builder.DeleteActorNode("actorNodeName1");
	builder.DeleteActorNode("actorNodeName2");
	builder.DeleteActorNode(pActorNode);
	builder.AddMeshNode(pMeshNode1, pParent, ...);
	builder.AddMeshNode(pMeshNode2, pParent, ...);
	builder.AddMeshNode(pMeshNode3, pParent, ...);
	builder.Build();
	*/


public:
	MDeclareRTTI;
	DECLARE_NODE_ID_STRING(RSID_ACTOR);

	virtual bool IsLoadingCompleted();

protected:
	virtual void				DeleteManagingChildren();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// AutoUpdate ������ ���� Context Structure
	//
	struct SAutoUpdateContext
	{
		SAutoUpdateContext()
		{
			m_bMustNotAutoUpdate = false;
			m_bMustAutoUpdate = false;
			m_bPlayingAnimation = false;
			m_nAutoUpdateControllers = 0;
		}

		bool m_bMustNotAutoUpdate;
		bool m_bMustAutoUpdate;
		bool m_bPlayingAnimation;
		int  m_nAutoUpdateControllers;

		bool CheckAutoUpdateContext()
		{
			_ASSERT(m_nAutoUpdateControllers >= 0);

			// AutoUpdate�� ������ ���ϴ°��
			if (m_bMustNotAutoUpdate)
				return false;

			// ���ǿ� ���� AutoUpdate�� ���ϴ� ���
			if (m_bMustAutoUpdate == false &&
				m_bPlayingAnimation == false &&
				m_nAutoUpdateControllers == 0 )
			{
				return false;
			}

			// AutoUpdate
			return true;
		}

	} m_AutoUpdateContext;


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ȯ����� ���� ����
	//
	RVector4					m_vMultiplyColor;				///< Color Factor


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���ͳ��
	//
	std::unordered_map<string,RActorNode*>		m_actorNodeMap;					///< RActorNode �̸��� ����
	vector<RActorNode*>			m_actorNodes;					///< RActorNode ��������� ����
	vector<RActorNode*>			m_sortedRenderableActorNode;	///< physique - phsyique_alpha - non_physique - non_phsyique_alpha
	vector<RActorNode*>			m_animationReferencedNodes;		///< �ִϸ��̼ǿ� �����Ǵ� ��� ���� ����ִ�


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �⺻ �ִϸ��̼� / ������Ʈ ����
	//
	RAnimationController	m_animationController;			///< �⺻ �ִϸ��̼� ��Ʈ�ѷ�
// 	RANIMATIONTRANSITIONTYPE	m_transitionType;
// 	RBlendAnimationController*	m_pTransitionController;		///< �ִϸ��̼��� ��ȯ�� ���� ���� ��Ʈ�ѷ��� �׻� �ϳ� �������ִ�.

	RMatrix						m_matInvWorld;					///< inverse of world
#ifdef USING_RSC_PHYSIQUE_QT
	RQuaternion					m_qInvWorldRot;					///< m_matInvWorld�� ��
	RVector3					m_vInvWorldTran;				///< m_matInvWorld�� ��
	RVector3					m_vInvWorldScale;				///< m_matInvWorld�� �� 
#endif
	int							m_nResultTransformUpdatedFrame;	///< UpdateFinalResultTransforms �� ������ ( �������ӿ� ������ ������Ʈ �����Ѵ� )
	int							m_nAnimationFrameUpdatedFrame;	///< animation frame �� ������ ������Ʈ�Ǵ°� �����Ѵ�

	RListener*					m_pListener;					///< �ִϸ��̼� �̺�Ʈ�� ���� ������


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ��� ���Ϳ� ���밡���� ����������Ʈ ����
	//
//	static void					SetGlobalActorRenderState();	///< �������� ����������Ʈ ����(fog ��)

	bool						m_bUseShade;					///< turn off shade ������ ����������
	bool						m_bUsingActorPassRenderer;		///< ���� ���� �н������� ��뿩��
	bool						m_bUsingEnvironmentTiling;		///< ȯ�� ���� �׸��� ��뿩��
	bool						m_bAlpha2PassRendering;			///< Alpha, Additive ���׸����� ��� ���� ���� ������ ���� 2�н��� �׸� ��

	bool						m_bVertexMotionBlur;			// ���ؽ� ��� �� ����
	bool						m_bEdge;						// �ܰ��� ����
	float						m_fEdgeTickNess;				// �ܰ��� �β�, �Ÿ��� ������ ���� ����
	bool						m_bPEEdge;						// ����Ʈ ����Ʈ �ܰ��� ����
	bool						m_bPEEdgeProj;					// ����Ʈ ����Ʈ �ܰ��� ���õǴ� ����				
	RVector3					m_vEdgeColor;									
	float						m_fMotionBlurBasisFrame;
	bool						m_bAccumMotionBlur;				// ���� ��� �� ����
	int							m_nPixelMotionBlur;				// �ȼ� ��� �� ����

	float						m_fDistanceToCamera;			///< ī�޶� �Ÿ�, OnPreRender()���� ������Ʈ�ȴ�.

	bool						m_bExistCollisionTree;				///< (���� ���) ����ÿ� �浹 ���� ����
	vector<RCapsule*>			m_vecColCapsule;				///< ���� �浹 �˻�

	
	bool						m_bUseDoubleShadow;				///< ���� �׸��� ���� ����
	bool						m_bAOEnable;					///< AO ���� ����

	// Max Ani BoundingBox
	bool						m_bIsMaxAniAABB;				///< Max Ani BoundingBox ������ �ִ°� // EXPORTER_ANI_VER8 �߰� ����
	MBox						m_aabbLocal;					///< local bounding box

	bool						m_bShadowReceiver;

	RMaterialDecal				m_MaterialDecal;

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� / ����
	//
	virtual bool				CreateFromXML(MXmlElement &element, bool bBackgroundCreation);
	virtual void				SaveToXML(MXmlElement &element);

	friend class				RSyncCreateDeviceDependedPart;
	friend class				RAsyncCreateDeviceIndependedPart;
	virtual void				CreateDeviceIndependentPart(RMesh* pMesh);	//����̽��� �������� �κа� ���������� �κ����� ������ ����
	virtual void				CreateDeviceDependentPart(RMesh* pMesh);
	void						CreateMaterials(bool bBackground);

	// ��׶��� �ε� ��Ʈ��
	virtual void				OnLoadingComplete(RResource*);
	virtual int					CalcPriority();
	virtual bool				Load();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update
	//
	virtual void				OnUpdate();
	virtual void				OnUpdateTransform() override;
	virtual void				OnUpdateAABB() override;
	void						UpdateAABB();
	RBoundingBox				ForceUpdateAABB(bool bBillboard = false);	///< Actor�� ActorNode�� AABB�� �����Ѵ�.


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������ / ���������� ������Ʈ
	//
	void						UpdateForRender(void);			///< �������� �ϱ� ���� �ʿ��� ������Ʈ
	void						UpdateAnimation();				///< �ִϸ��̼� ������ ������Ʈ
	void						UpdateMaterialAnimation();		///< material uv ani ������ ������Ʈ
	bool						IsRenderable( RActorNode* pActorNode ); ///< ������ ������ ���ͳ������ üũ

	virtual void				OnPreRender( RCameraSceneNode* pCurrentCamera) override;
	virtual void				OnRender(RCameraSceneNode* pCurrentCamera, int nSubMaterialID, RRENDERINGMETHOD renderMothod) override; // �н��������� ���� OnRender �迭�� �Լ��� �Ҹ��� �ʴ´�.
	void						RenderShadowMap( RCameraSceneNode* pCurrentCamera, RShadowStrategy* pShadowStrategy, RActorNodeRenderStrategy* pRenderStrategy );
	
protected:
	virtual void				RenderAlpha( RCameraSceneNode *pCamera,int iSubMaterial, RRENDERINGMETHOD eMothod) override;


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ��ŷ / ĸ���浹
	//
	virtual bool				OnPick(RPICKINFO& pickInfo);
	void						UpdateColCapsule();				///< ĸ���� �浹 ó��(���� �浹)


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �޽������� �ٽ� ����� �ʿ䰡 ������
	//
	void						OnRestore();
	void						OnInvalidate();


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �������� ���õ� ������ (�ٽ�)����� : sortnodes & lod boneset(s)
	//
	void						CreateRenderInfoAndNotifyToController(bool _bNotifyToController = true);
	void						DeleteActorNodeAndNotifyToController(RActorNode* pActorNode, bool _bNotifyToController = true);


	//////////////////////////////////////////////////////////////////////////
	// ���� ��ȯ
	//
	RActorNode*					NewActorNode(RMeshNode* pMNode);
	void						AddActorNode(RActorNode* pActorNode, RSceneNode* pParent);

	//////////////////////////////////////////////////////////////////////////
	// �ִϸ��̼� ������Ʈ / �̺�Ʈ
	//
	void						UpdateAnimationFrameAndEvent();
//	void						CheckAnimationEventSend();

	//////////////////////////////////////////////////////////////////////////
	// �ִϸ��̼� ��Ʈ�ѷ� �������� ForTest
	//
	void						SetForceAnimationControlPlayDoneForTest(bool bPlayDone);
	void						SetForceAnimationControlLoopForTest(bool bLoop);

	//////////////////////////////////////////////////////////////////////////
	// ����������Ʈ�� �߰�
	virtual void				AddToRenderingList(RRenderingList* pRenderingList)		override;
	virtual void				RemoveFromRenderingList(RRenderingList* pRenderingList)	override;
	

public:

	RActor(RRENDERPASS eRenderPass = RRP_ACTOR); 
	virtual ~RActor();


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� / �Ҹ�
	//
	virtual bool				Create(const char* szMeshName, bool bBackgroundCreation = false);
	virtual bool				Create(RMesh* pMesh, bool bBackgroundCreation = false, bool bLoadMonitoring = false);

	virtual void				Destroy();

	virtual void				OnActorLoadComplete();
	virtual void				OnAutoAnimationLoaded();

	void						ChangeMatetial(string strActorNode, string strSrcMtrl, string strDstMtrl, float fBlendingTime = 0.3f);
	void						ChangeMatetial(string strActorNode, int nSrcMtrl, int nDstMtrl, float fBlendingTime = 0.3f);
	void						ChangeMatetial(string strActorNode, int nSrcMtrl, string strDstMtrl, float fBlendingTime = 0.3f);
	void						ChangeMatetial(int nActorNodeIndex, string strSrcMtrl, string strDstMtrl, float fBlendingTime = 0.3f);
	void						ChangeMatetial(int nActorNodeIndex, int nSrcMtrl, int nDstMtrl, float fBlendingTime = 0.3f);
	void						ChangeMatetial(int nActorNodeIndex, int nSrcMtrl, string strDstMtrl, float fBlendingTime = 0.3f);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Node , Decorator ����
	//
	int							GetActorNodeCount() const;									///< �迭 ũ��
	vector<RActorNode*>&		GetActorNodes(){return m_actorNodes;}			///< �ִϸ��̼ǿ� �����Ǵ� bone set�� ����
	RActorNode*					GetActorNode(const char* szNodeName);						///< �̸����� ��带 ��´�
	bool						GetNodePosition(char* nodename, RVector& vPos);				///< �����ġ���� ���´�. ��带 �����˻��ϹǷ� ���� ����ȭ�ؾ���. - bird
	vector<RActorNode*>&		GetAnimationReferencedNodes() { return m_animationReferencedNodes; }

	RActorNode*					AddMeshNode(RMeshNode* pMeshNode, RActorNode* pParent, const char* szActorNodeName = NULL, bool bBackground = false, bool bNotifyToController = true);		///< ������ �߰� ( ���� �̸��� ������ ��ü ) �Ѵ�
	bool						DeleteActorNode(const char* szNodeName, bool bNotifyToController = true);

	bool						HasClothActorNode(void);


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �޽� ����
	//
	string						GetMeshName();
	RMesh*						m_pMesh;				// ����� ������.
	RMesh*						m_pLowPolyMesh;
	void						LinkLODMetricInfo();

	//////////////////////////////////////////////////////////////////////////
	// update for render ����
	//
	bool						CheckAlreadyUpdatedForRender();
	void						AddUpdateForRenderRefCount(){ m_nUpdateForRenderRefCount++; }
	void						ReleaseUpdateForRenderRefCount(){ m_nUpdateForRenderRefCount--; }
	int							GetUpdateForRenderRefCount() const { return m_nUpdateForRenderRefCount; }
	void						UpdateAnimationReferenceNodes();	///< ������ �������� ���̴� �ִϸ��̼� ���� ��� ���

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Animation ����
	//
	bool						SetAnimation( const char* szAnimation ); ///< �ִϸ��̼� ����, �ִϸ��̼� ��ȯ �����͸� RAnimationBlendTable���� ��´�.
	bool						SetAnimation( RAnimation* pAnimation ); ///< �ִϸ��̼� ����, �ִϸ��̼� ��ȯ �����͸� RAnimationBlendTable���� ��´�.
// 	bool						SetAnimation( const char* szAnimation, RANIMATIONTRANSITIONTYPE transType, float fTransitionTime = 0.1f ); ///< �ִϸ��̼� ����
// 	bool						SetAnimation( RAnimation* pAnimation, RANIMATIONTRANSITIONTYPE transType, float fTransitionTime = 0.1f ); ///< �ִϸ��̼� ����

	RAnimation*					GetAnimation(const char *szAnimationName);	///< �ִϸ��̼��� ����
	RAnimation*					GetAnimation();							///< ��ϵ� ���ϸ��̼��� ���´�.
//	RANIMATIONTRANSITIONTYPE	GetAnimationTransitionStatus();			///< �ִϸ��̼� ��ȯ�� ����
	RAnimationController&	GetAnimationController();

	void						SetSpeed(float s=1.f);					///< ���ϸ��̼� �ӵ��� �����Ѵ�. ( 1.0f = 1��� )
	float						GetSpeed();

	void						Play();									///< �ִϸ��̼� ���
	void						Stop();									///< �ִϸ��̼� stop
	void						Pause();								///< �ִϸ��̼� pause

	bool						IsPlayDone();							///< �÷��������� ����
	bool						IsOncePlayDone();						///< ���ϸ��̼��� �ּ��� �ѹ� �÷��� �Ǿ����� ����

	void						SetFrame(unsigned int nFrame);			///< �ִϸ��̼� Ư�� ���������� ����
	int							GetFrame();

	void						SetListener(RListener* pListener);		///< event listener
	RListener*					GetListener();

	vector<RActorNode*>&		GetAnimationReferrencedNodes();			///< �ִϸ��̼ǿ� �����Ǵ� bone set�� ����
	vector<RActorNode*>&		GetSortedRenderableNodes();				///< ���ĵ� ������ ����
	const RMatrix&				GetWorldInverseTransform();				///< ������ �����
 	const RQuaternion&			GetWorldInverseRotation();				///< ������ ����� ���� 
 	const RVector3&				GetWorldInverseTranslation();			///< ������ ����� ���� 
 	const RVector3&				GetWorldInverseScale();					///< ������ ����� ���� 

	void						LoadAutoAnimation();					///< �ڵ������� ����Ǵ� �ִϸ��̼� �ε�
	void						PlayAutoAnimationRandom();				///< ������ �ڵ������� ����Ǵ� �ִϸ��̼� �÷���
	bool						m_bAutoAnimationEnable;					///< �ε����ķκ��� �ڵ������� �ִϸ��̼��� �����ϴ��� ����
	RAnimation*					m_pAutoAnimation;						///< �ڵ� �ε��� �ִϸ��̼�

	bool						SetLocalAABB( const RBoundingBox* pAABB );

	// ���ø� �׽�Ʈ
	void						GetSamplingNodeData(const char* szNodeName, unsigned int nStartFrame, unsigned int nEndFrame, int nSampleInterval, std::vector< RMatrix >& _rOUt);

	void						SetUseVertexAnimation( bool bUse );		///< ���ؽ� �ִϸ��̼� ��� ���θ� ����

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �浹����
	//
	bool						GetCollisionCreationEnable() const		{ return m_bExistCollisionTree; }
	void						SetCollisionCreationEnable(bool bMake )	{ m_bExistCollisionTree = bMake; }

	void						CollectStaticCollisionPolygonsDo(RSceneNodePolygonCollector* pCollector, DWORD dwAttribute) const;	///< ������ �浹 �޽ø� �����ϴ� �۾��� �ϴ� �Լ�
	virtual void				CollectStaticCollisionPolygons(RSceneNodePolygonCollector* pCollector) const override;	///< static �浹�޽ø� �������ش�
	RActorCollision*			m_pCollision;


	//////////////////////////////////////////////////////////////////////////
	// ��ŷ
	//
	bool						PickActorNode(RPICKINFO& pickInfo);	///< ���ͳ��� ��ŷ


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �׸��� ����
	//
	bool						IsShadowReceiver() { return m_bShadowReceiver; }
	void						SetShadowReceiver(bool bEnable) { m_bShadowReceiver=bEnable; }
	void						SetUseDoubleShadow(bool bUse);
	bool						GetUseDoubleShadow()							{ return m_bUseDoubleShadow; }	

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// AO ����
	//
	void						SetAO(bool bEnable)	{ m_bAOEnable = bEnable; }
	bool						GetAO(void)			{ return m_bAOEnable; }


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������ ���� ����
	//
	void						SetMultiplyColor(const RVector4 &vColor);
	void						ResetMultiplyColor();
	const RVector4&				GetMultiplyColor();
	bool						IsUseMultiplyColor() { return m_vMultiplyColor != RVector4(1,1,1,1); }

	void						SetIlluminationScale(const float fScale);
	void						SetIlluminationScale(string strActorNode, const float fScale);
	float						GetIlluminationScale(string strActorNode);

	RShaderEffectController*	m_pShaderEffectController;
	RShaderEffectController*	GetShaderEffectController() { return m_pShaderEffectController; }


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������ ����
	//
	bool GetUsingEnvironmentTiling()		{ return m_bUsingEnvironmentTiling; }
	void SetUsingPassRenderer(bool _bUsing ){ m_bUsingActorPassRenderer = _bUsing; }

	void SetAlpha2PassRendering( bool bAlpha2PassRender );
	bool GetAlpha2PassRendering()							{ return m_bAlpha2PassRendering; }


	//void Render();
	void RenderSingle( RCameraSceneNode* pCurrentCamera, RRENDERINGMETHOD eMothod);
	void RenderSingleAlpha2Pass(  RCameraSceneNode* pCurrentCamera, RRENDERINGMETHOD eMothod);

	void SetVertexMotionBlur( bool b);	// ���ؽ� ��� �� On/Off
	bool IsVertexMotionBlur()			{ return m_bVertexMotionBlur;					}	// ���ؽ� ��� ��������

	void EnableEdge( const RVector3& _color, float _fTickNess = 2.f );				// �ܰ��� On/Off
	bool IsEdge()						{ return m_bEdge;								}	// �ܰ��� ��������
	void DisableEdge();


	void EnablePEEdge( const RVector3& _vEdgeColor );				// �ܰ��� On/Off
	bool IsPEEdge()						{ return m_bPEEdge;								}	// �ܰ��� ��������
	void DisablePEEdge();


	void SetAccumMotionBlur( bool b);	// ���� ��� �� On/Off
	bool IsAccumMotionBlur()			{ return m_bAccumMotionBlur;					}	// ���� ��� ��������

	void SetPixelMotionBlur( int b);	// �ȼ� ��� �� On/Off
	bool IsPixelMotionBlur()			{ return (bool)(m_nPixelMotionBlur>0);			}	// �ȼ� ��� ��������

	void SetMotionBlurBasisFrame( float f)	{ m_fMotionBlurBasisFrame = f;				}
	float GetMotionBlurBasisFrame()			{ return m_fMotionBlurBasisFrame;			}

	void SetVertexMotionBlurBasisFrame( float f)	{ SetMotionBlurBasisFrame(f);		}
	float GetVertexMotionBlurBasisFrame()			{ return GetMotionBlurBasisFrame();	}

	void					SetDistortion(bool bDistortion);
	bool					IsDistortion();
	bool					IsDistortionRenderable();
	RDistortionParameter*	GetDistortionParam();
	void					SetDistortionParam(RDistortionParameter* param);




	RMaterialDecal* GetMaterialDecal() { return &m_MaterialDecal; }


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������ ���� : ������ ȣ���ϱ� ����..
	bool ReLoadMesh();
	float		GetEdgeTickNess()		{ return m_fEdgeTickNess; }
	bool		GetPEEdgeProjection()	{ return m_bPEEdgeProj; }
	const RVector3&	GetEdgeColor() const			{ return m_vEdgeColor; }
	void		SetEdgeColor( const RVector3& _vEdgeColor);

	void						EnableAddColorEffect( const char* _szNodeName, const RVector3& _vColor );
	void						DisableAddColorEffect( const char* _szNodeName );
	RBoundingBox				SetForceUpdateAABB()	{ return ForceUpdateAABB(); }

public:
	// ����׿� ������
	void RenderSkeleton();
	void RenderMeshCollision();
	void RenderClothCollision();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ��׶��� �ε� / ��Ÿ
	//
	bool						m_bBackgroundCreation;		///< ��׶���� �����Ǵ��� ����
	bool						m_bSceneActor;				///< Import Scene�� ����. �� ���ʹ� ��׶��� �ε��� �켱������ �ִ�.
	int							m_nBGWKPriorityOffset;		///< BackgroundWork Priority Offset;
	DWORD						m_dwBoneColor[RBIPID_END];	///< ����׿� �������Ҷ� ����

	//////////////////////////////////////////////////////////////////////////
	// STATE
	//
	inline bool IsUseShade() { return m_bUseShade; }

	void						SetEnvironmentTexture( const char* _szEnvTexName );
	RTexture*					GetEnvironmentTexture()		{ return m_pEnvironmentTexture; }
	char*						GetEnvironmentTextureName()	{ return m_szEnvTexName; }
	void						EmptyEnvironment();
	

private:
	void						AddToBVH();
	void						RemoveFromBVH();

	char						m_szEnvTexName[128];
	RTexture*					m_pEnvironmentTexture;
	int							m_nUpdateForRenderRefCount;		///< UpdateForRender �� �ʿ��� ������Ʈ

	bool						IsUsingVertexAnimation();
	bool						m_bUsingVertexAnimation;

	bool						DeleteActorNode(RActorNode* pActorNode, bool bNotifyToController = true);
	void						RebuildAnimationOnActorNodeChanged();

	void						PreCreateShader();

	//////////////////////////////////////////////////////////////////////////
	// Builder
	bool						DeleteActorNode(std::vector<RActorNode*> &vecActorNode, bool bNotifyToController = true);	///< ����Ʈ�� ����
	bool						AddMeshNode(std::vector<RActorNodeBuilder::RADDMESHNODE*>& vecActorNode);


	RDistortionParameter*		m_pDistortionParam;				///< ����� �Ķ����
};

//----------------------------------------------------------------------------------------------------
inline int RActor::GetActorNodeCount() const					{ return (int)m_actorNodes.size(); }		///< �迭 ũ��
//----------------------------------------------------------------------------------------------------
inline void RActor::SetListener(RListener* pListener)			{ m_pListener = pListener; }
//----------------------------------------------------------------------------------------------------
inline RListener* RActor::GetListener()							{ return m_pListener; }
//----------------------------------------------------------------------------------------------------
inline void RActor::SetMultiplyColor(const RVector4 &vColor)	{ m_vMultiplyColor = vColor; }
//----------------------------------------------------------------------------------------------------
inline void RActor::ResetMultiplyColor()						{ m_vMultiplyColor = RVector4(1,1,1,1); }
//----------------------------------------------------------------------------------------------------
inline const RVector4& RActor::GetMultiplyColor()				{ return m_vMultiplyColor; }
//----------------------------------------------------------------------------------------------------
inline RAnimationController& RActor::GetAnimationController()	{ return m_animationController; }
//----------------------------------------------------------------------------------------------------

class RS_API RActorMLeaf : public RActor, public MMemPool<RActorMLeaf, 50000>
{
	MDeclareRTTI;
public:
	RActorMLeaf(RRENDERPASS eRenderPass = RRP_ACTOR) : RActor(eRenderPass) {}
	virtual ~RActorMLeaf() {}
};

}

#endif//_RActor_h
