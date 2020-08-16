#pragma once

#include "RPreDefine.h"
#include "RWorldSceneNode.h"
#include "REngine.h"
#include "RTimer.h"
#include "MSingleton.h"
#include "RPassRenderer.h"
#include "RSceneNode.h"
#include "RListener.h"
#include "RResourceLoadingGroup.h"
#include "RInferredRenderingStrategy.h"
#include "RMemPoolReleaser.h"


namespace tbb 
{
	class task_scheduler_init;
};

namespace rs3 {

static const char* TYPENAME_MESH	= "mesh";
static const char* TYPENAME_INDOOR	= "rs";
static const char* TYPENAME_TREE	= "tree";


class RResourceProvider;
class TResourceManager;
class RWorker;
class RAbstractResourceAllocator;
class RLoader;

class RTerrain;
class RStaticMeshSceneNode;
class RPassRenderer;

class RResourceManager;
class RShaderCompositeManager;

// effect ����
class RActorNode;
class REffectSceneNode;
class REffectManager;

// light/shadow ����
class RDirectionalLightSceneNode;
enum RSHADOWMETHOD;
enum RSHADOWFILTERMETHOD;
struct RShadowDesc;
class RShadowStrategy;

class RRenderingStrategy;
enum RRENDERINGMETHOD;

// controller
class RActorController;
class RSceneNodeController;
class RSceneNodeControllerFactory;

class REnvironmentSceneNode;
class RRenderingCallbackInterface;
class RZoneLoadingQuery;

/// ���� ������ ����Ѵ�
class RS_API RSceneManager : public MSingletonRestricted< RSceneManager >
{
private:

public:
	RSceneManager();
	virtual ~RSceneManager();


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// SceneNode Declaration
	//
	MDeclareRTTI;


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� / �Ҹ� / ����
	//
	virtual bool					Create(RDevice* pDevice);
	virtual void					Destroy();
	virtual void					Clear();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// timer
	//
	const RTimer&					GetTimer()		{ return REngine::GetTimer();  }		// rs3 ���� Ÿ�̸�
	void							ResetTimer()	{ REngine::GetTimer().Reset(); }		// Ÿ�̸� ����
	inline void						UpdateTimer();
	inline void						UpdateTimerCustom(DWORD dwElapsed);


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// camera
	//
	RCameraSceneNode*				SetPrimaryCamera(RCameraSceneNode *pCamera); // ���ϰ��� ���� ī�޶�
	RCameraSceneNode*				GetPrimaryCamera()	{ return m_pPrimaryCamera; }

	void							DisablePEEdge();
	void							EnablePEEdge();
	void							AddPEEdgeCount();
	void							MinusPEEdgeCount();


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// terrain
	//
	RTerrain*						GetCurrentTerrain();


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// static mesh
	//
	RStaticMeshSceneNode*			GetCurrentStaticMeshSceneNode();


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// light and shadow
	//
	RDirectionalLightSceneNode*		GetDefaultLight();												// �⺻����


	// ����Ʈ ���̱�. ���ͳ�带 ���϶�, pRelatedSceneNode�� NULL�� ���ܵν��� ���ð� RActor�� ä���ּ���.
	// TODO: Ŭ���̾�Ʈ�� �Űܰ���
	void		AttachEffect( REffectSceneNode* pEffectSceneNode, RSceneNode* pRelatedSceneNode, RActorNode* pRelatedActorSceneNode, const RMatrix& _rBaseFrame = RMatrix::IDENTITY, bool bIgnoreDir = false );

	//////////////////////////////////////////////////////////////////////////
	// �׸��� , TODO: �Űܰ���
	void							SetRealtimeShadowMethod(const RShadowDesc& desc);
	RSHADOWMETHOD					GetRealtimeShadowMethod();
	RShadowStrategy*				GetShadowStrategy();
	RSHADOWFILTERMETHOD				GetShadowFilterType();
	void							SetShadowFilterType(RSHADOWFILTERMETHOD eFilterType);

	void							SetRealtimeRenderingMethod( RRENDERINGMETHOD eMethod, DWORD dwWidth = 0, DWORD dwHeight = 0);
	RRENDERINGMETHOD				GetRealtimeRenderingMethod();
	RRenderingStrategy*				GetRenderingStrategy()			{ return m_pRenderingStrategy;	}

	void							UseLighting( bool b)			{ if( m_pRenderingStrategy) m_pRenderingStrategy->UsetLighting(b);	}

	void							SetSSAO( int nQuality);
	void							UseSSAO( int nQuality) { SetSSAO( nQuality); }
	int								GetSSAO();
	int								IsUsingSSAO() { return GetSSAO(); }

	bool							IsUsingNormalMap();
	void							UseNormalMap( bool b);


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// update / cull / render
	//

	// Primary Camera�� ����
	virtual void					RenderPrimaryCamera(  RRenderingCallbackInterface* pRenderingCallback = NULL);				// �����̸Ӹ� ī�޶� ������
	// ������ Camera�� ����
	virtual void					RenderCamera(RCameraSceneNode* pCamera, RRenderingCallbackInterface* pRenderingCallback = NULL);

	virtual void					Update( unsigned int frameCount = 0 );

	// TODO: Load �ø���� �ε��ϴ� �ų���� ����� �˼��� ��� �ݹ��� �߰��Ǿ���. ���ҽ� �����丵 �۾����� ���ش�
	class RSceneLoadingCallbackInterface	// �ε��� �ݹ�
	{
	public:
		virtual void OnPostCreate( RSceneNode* pSceneNode ) {}
		virtual void OnPostCreateChildren( RSceneNode* pSceneNode ) {}
	};

	virtual bool					LoadXML(RSceneNode *pSceneNodeToLoad, MXmlElement &element, bool bBackgroundCreation = false,RZoneLoadingQuery* pZoneLoadingQuery = NULL);

	//------------------------<< �����丵�� >>
	//Root XML ������ �о�� �Է¾���忡 ������ �־���
	bool	LoadSceneNodeFromRootXMLFile(RSceneNode *pSceneNode, const char* szRootXMLFileName, bool bBackgroundCreation = false,RZoneLoadingQuery* pZoneLoadingQuery = NULL);
	bool	LoadWorldorInstanceFromRootXMLFile(RSceneNode *pSceneNode, const char* szRootXMLFileName, bool bBackgroundCreation = false,RSceneLoadingCallbackInterface* pCallback = NULL);
	bool	LoadSceneNodeInstanceFromRootXMLFile(RSceneNode *pSceneNode, const char* szRootXMLFileName, bool bBackgroundCreation = false);
	bool	LoadWorldSceneNodeFromRootXMLFile(RSceneNode *pSceneNode, const char* szRootXMLFileName, bool bBackgroundCreation = false);
	bool	LoadWorldSceneNodeFromRootXMLFileWithLoadingProgress(RSceneNode *pSceneNode, const char* szRootXMLFileName, bool bBackgroundCreation = false);
	bool	LoadTempEfffectWorldSceneNodeFromRootXMLFile(RSceneNode *pSceneNode, const char* szRootXMLFileName, bool bBackgroundCreation = false);

	virtual bool					SaveRootSceneToXML(const char *szFileName);
	virtual bool					SaveSceneNode(RSceneNode *pSceneNodeToSave, const char *szFileName);
	virtual void					SaveXML(RSceneNode *pSceneNodeToSave, MXmlNode *pXMLNode);

	virtual bool					LoadCommonProperty(RSceneNode *pNode, MXmlElement &element);
	virtual void					SaveCommonProperty(RSceneNode *pNode, MXmlElement &element);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����� ���� / ��� / ����
	//
	virtual RSceneNode*				CreateSceneNode(const char *szNodeName);	// �־��� ���Ŀ� �´� ����� ���·� ����								// scene node factory
	// add scene node to root scene. ��Ʈ ���� ���� ��� ���� ����.
	bool							AddSceneNode(RSceneNode *pChildOfRoot, bool bAddToSceneNodeGroup = false);	
	bool							AddSceneNode(RWorldSceneNode* pWorld, RSceneNode *pChildOfRoot, bool bAddToSceneNodeGroup = false);

	// ī�޶� ��Ʈ ���� ���̰� ������ ���� ��Ʈ ���� ���� �Ǿ� ���� ���� ��츦 ����� ����ϵ��� �Ѵ�.
	// ��� ��Ʈ ���� ���� �� �Ӹ� �ƴ϶� ī�޶� ���� ���� �׻� ��� �ϵ��� ����.
	// ���ø����̼ǿ� "��Ʈ ���� ���� �� ī�޶� �߰� �϶�"�� ������ �ִ� ���� �ʹ� ��Ȥ�ϴ�.
	// ��Ʈ ���� ���� �Ǹ� ī�޶� addChild�϶�� ������ �Ŵ� ������..
	bool							AddCameraSceneNode(RCameraSceneNode *pChildOfRoot, bool bAddToSceneNodeGroup = false);
	// AddSceneNode�� �ݴ� ����
	// �� ī�޶�� ��Ʈ ���� �ٲ㰡�� �Կ� �� �� ī�޶� ��带 ��Ʈ���� ���� �ߴ� �־��� �ϴ� ���� ���������� �ʿ��ϰ� �Ǿ���.
	bool							RemoveSceneNode(RSceneNode *pChildOfRoot, bool bRemoveFromSceneNodeGroup = false);

	RWorldSceneNode*				CreateRootSceneNode(void);
	// Ŀ��Ʈ ��Ʈ �� ��ȯ
	RWorldSceneNode*				GetCurrentRootSceneNode( bool bCreation = true);
	RWorldSceneNode&				GetRootSceneNode()	{ return (*(GetCurrentRootSceneNode( true)));	}


	// ��Ʈ ���� ����. Ŀ��Ʈ ��Ʈ�� ���� �Ǿ� �ִ� ���̸� ���� �Ұ���.
	bool							DeleteRootSceneNode( RWorldSceneNode* pNode);

	bool							PickSceneNode( RPICKINFO& pickInfo, RWorldSceneNode* pRootNode = NULL);

	bool							SetCurrentRootSceneNode( RWorldSceneNode* pNode);	

	//RWorldSceneNode*				GetNodesRootSceneNode( RSceneNode* pSceneNode);	// �ش� ���� �Ѹ��� ã���ش�. �ٺ������ڽ��̸� NULL��ȯ

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Controller Factory
	//
	virtual RSceneNodeController*	CreateSceneNodeController(const char* _pSzControllerName);


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Resource And Resource Manager
	//
	template< typename T_ResourceMgr >
	T_ResourceMgr*					GetResourceMgr(bool _bCreateIfNotExist = true);

	template< typename T_Resource >
	T_Resource*						CreateResource( const char* _pSzResourceName, bool bLoadMonitoring = false );	// ĳ���� ��� �Ͽ� ���ҽ� ���� - COMMENT OZ
	void							ReleaseResource( RResource* _pResource);
	void							ReserveResourceToDelete( RResource* _pResource );
	void							UpdateDeletionQueue();
	void							AddResourceLoadMonitoring( RResource* pResource, bool bBackgroundLoading );
	bool							AddLoadingList( RResource* pResource, bool bBackgroundLoading, RResource::LoadingController* pLoadingController = NULL );

	template< typename T_Resource >
	T_Resource*						GetResource( const char* _pSzResourceName );

	RShaderCompositeManager*		GetShaderCompositeManager();
	REffectManager*					GetEffectManager();

	bool							InitSharedResources();
	void							DestroySharedResources();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// pass renderer
	//
	template< typename T_PassRenderer >
	T_PassRenderer*					GetPassRenderer(bool _bCreateIfNotExist = true);
	RPassRenderer*					GetPassRenderer( RRENDERPASS pass );
	void							ReleasePassRenderer( RRENDERPASS pass );

	void							TurnOffToUpdateAndRender(bool bTurnOff)		{	m_TurnOffToUpdateAndRender = bTurnOff;	}

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Root SceneNode
	//
	//RWorldSceneNodeArray			m_rootSceneNodes;
	RWorldSceneNodeMap				m_rootSceneNodeMap;
	RWorldSceneNode*				m_pCurrentRootSceneNode;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Resource
	//
	RResourceManager**				m_arrayResourceMgr;			// resource manager container
	std::vector< RResource* >		m_resourcesToDelete;		// ���� ���ҽ�

	RResourceLoadingGroup*			m_pBeforeResourceLoadingGroup;
	RResourceLoadingGroup*			m_pCurrentResourceLoadingGroup;

	template< typename T_ResourceID >
	RResourceManager*				GetResourceMgrFromID(int _ResID);	//���ҽ� ID�κ��� RResourceManager������

	//////////////////////////////////////////////////////////////////////////
	// ... Managers ...
	//
	RShaderCompositeManager*		m_pShaderCompositeManager;
	REffectManager*					m_pEffectManager;

	//////////////////////////////////////////////////////////////////////////
	// Pass Renderers
	//
	RPassRenderer*					m_defaultPassRenderers;
	RPassRenderer**					m_arrayPassRenderer;		// pass renderer container


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Primary(Default) SceneNode : Camera, Light, Terrain
	//
	RCameraSceneNode*				m_pPrimaryCamera;			// camera
	//BYTE							m_byReservationCameraAdd;	// 0: false, 1: AddToSceneNodeGroup, -1: NotAddToSceneNodeGroup


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// frame count / timer for update
	//
	unsigned int					m_nLastUpdateFrameCount;
	bool							m_bTimerUpdated;

	//////////////////////////////////////////////////////////////////////////
	// shadow depth map
	RShadowStrategy*				m_pShadowStrategy;
	RSHADOWFILTERMETHOD				m_eFilterType;
	
	//////////////////////////////////////////////////////////////////////////
	// Rendering ( forward, deferred, inferred)
	RRenderingStrategy*				m_pRenderingStrategy;

	//�޸�Ǯ
	RMemPoolReleaser*	m_pMemoryPoolReleaser;

	bool				m_TurnOffToUpdateAndRender;

public:
	RPostEffectInterface*		GetPostEffect(POST_EFFECT_TYPE eType)		{ return m_pRenderingStrategy ? m_pRenderingStrategy->GetPostEffect(eType) : NULL;			}

	// HDR
	void						SetHDREnable( bool b)						{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetHDREnable(b);							}
	bool						IsEnableHDR()								{ return ( m_pRenderingStrategy && m_pRenderingStrategy->IsEnableHDR());					}
	bool						GetHDRBrightPassThreshold( RVector& vOutValue ) { return m_pRenderingStrategy ? m_pRenderingStrategy->GetHDRBrightPassThreshold(vOutValue) : false; }
	bool						SetHDRBrightPassThreshold( RVector vValue )	{ return m_pRenderingStrategy ? m_pRenderingStrategy->SetHDRBrightPassThreshold(vValue) : false; }
	void						SetHDRViewRenderTarget( bool b)				{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetHDRViewRenderTarget(b);				}
	bool						IsHDRViewRenderTarget()						{ return m_pRenderingStrategy ? m_pRenderingStrategy->IsHDRViewRenderTarget() : false;		}

	// DOF ����
	void						SetDOFEnable( bool b)						{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetDOFEnable(b);							}
	bool						IsEnableDOF()								{ return ( m_pRenderingStrategy && m_pRenderingStrategy->IsEnableDOF());					}
	void						SetDOFActive( bool bActive)					{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetDOFActive(bActive);					}
	bool						AddDOFFocalDist(float f)					{ return ( m_pRenderingStrategy && m_pRenderingStrategy->AddDOFFocalDist(f));				}
	bool						AddDOFFocalRange(float f)					{ return ( m_pRenderingStrategy && m_pRenderingStrategy->AddDOFFocalRange(f));				}
	bool						AddDOFCOCSize(float f)						{ return ( m_pRenderingStrategy && m_pRenderingStrategy->AddDOFCOCSize(f));					}
	float						GetDOFFocalDist()							{ return m_pRenderingStrategy ? m_pRenderingStrategy->GetDOFFocalDist() : -1;				}
	float						GetDOFFocalRange()							{ return m_pRenderingStrategy ? m_pRenderingStrategy->GetDOFFocalRange() : -1;				}
	float						GetDOFCOCSize()								{ return m_pRenderingStrategy ? m_pRenderingStrategy->GetDOFCOCSize() : -1;					}
	float						GetDOFNearBlurDist()						{ return m_pRenderingStrategy ? m_pRenderingStrategy->GetDOFNearBlurDist() : -1;			}
	float						GetDOFFarBlurDist()							{ return m_pRenderingStrategy ? m_pRenderingStrategy->GetDOFFarBlurDist() : -1;				}
	float						GetDOFMaxBlurOfBehind()						{ return m_pRenderingStrategy ? m_pRenderingStrategy->GetDOFMaxBlurOfBehind() : -1;			}
	bool						IsDOFEdgeBlur()								{ return m_pRenderingStrategy ? m_pRenderingStrategy->IsDOFEdgeBlur() : false;				}
	bool						SetDOFFocalDist(float f)					{ return ( m_pRenderingStrategy && m_pRenderingStrategy->SetDOFFocalDist(f));				}
	bool						SetDOFFocalRange(float f)					{ return ( m_pRenderingStrategy && m_pRenderingStrategy->SetDOFFocalRange(f));				}
	bool						SetDOFCOCSize(float f)						{ return ( m_pRenderingStrategy && m_pRenderingStrategy->SetDOFCOCSize(f));					}
	bool						SetDOFNearBlurDist(float f)					{ return ( m_pRenderingStrategy && m_pRenderingStrategy->SetDOFNearBlurDist(f));			}
	bool						SetDOFFarBlurDist(float f)					{ return ( m_pRenderingStrategy && m_pRenderingStrategy->SetDOFFarBlurDist(f));				}
	bool						SetDOFMaxBlurOfBehind(float f)				{ return ( m_pRenderingStrategy && m_pRenderingStrategy->SetDOFMaxBlurOfBehind(f));			}
	bool						SetDOFEdgeBlur( bool b= true)				{ return ( m_pRenderingStrategy && m_pRenderingStrategy->SetDOFEdgeBlur(b));				}

	// ScreenMotionBlur ����
	void						SetScreenMotionBlurEnable( bool b)			{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetScreenMotionBlurEnable(b);				}
	bool						IsEnableScreenMotionBlur()					{ return ( m_pRenderingStrategy && m_pRenderingStrategy->IsEnableScreenMotionBlur());		}
	void						SetScreenMotionBlurActive( bool bActive)	{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetScreenMotionBlurActive(bActive);		}
	void						SetScreenMotionBlurSpeed( float f)			{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetScreenMotionBlurSpeed(f);		}
	void						SetScreenMotionBlurSamplerCount( int n)		{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetScreenMotionBlurSamplerCount(n);		}
	void						SetScreenMotionBlurInvalidateRotate( bool b){ if( m_pRenderingStrategy) m_pRenderingStrategy->SetScreenMotionBlurInvalidateRotate(b);		}
	void						SetScreenMotionBlurBasisByCamMove()			{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetScreenMotionBlurBasisByCamMove();		}
	void						SetScreenMotionBlurBasisByEpicenter()		{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetScreenMotionBlurBasisByEpicenter();		}
	void						SetScreenMotionBlurEpicenterScreenCoord( float x, float y)	{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetScreenMotionBlurEpicenterScreenCoord( x,y);		}
	void						SetScreenMotionBlurEpicenterProjectionCoord( float x, float y)	{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetScreenMotionBlurEpicenterProjectionCoord(x,y);		}
	void						SetScreenMotionBlurEpicenterWorld( RCameraSceneNode* pCurrentCamera, RVector3& v)	{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetScreenMotionBlurEpicenterWorld( pCurrentCamera, v);		}
	void						SetScreenMotionBlurPlayScat( float fPlayTime, float fScatTime, float fSpeed, BYTE nScarFactorType)	{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetScreenMotionBlurPlayScat( fPlayTime, fScatTime, fSpeed, nScarFactorType);		}

	// AccumMotionBlur ����
	void						SetAccumMotionBlurEnable( bool b)			{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetAccumMotionBlurEnable(b);					}
	bool						IsEnableAccumMotionBlur()					{ return ( m_pRenderingStrategy && m_pRenderingStrategy->IsEnableAccumMotionBlur());			}
	void						SetAccumMotionBlurActive( bool b)			{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetAccumMotionBlurActive(b);					}
	bool						IsAccumMotionBlurBeing()					{ return ( m_pRenderingStrategy && m_pRenderingStrategy->IsAccumMotionBlurBeing());				}
	bool						SetAccumMotionBlurDecreaseAlpha( float f)	{ return ( m_pRenderingStrategy && m_pRenderingStrategy->SetAccumMotionBlurDecreaseAlpha(f));	}
	bool						SetAccumMotionBlurAccumRate( float f)		{ return ( m_pRenderingStrategy && m_pRenderingStrategy->SetAccumMotionBlurAccumRate(f));	}

	// PixelMotionBlur ����
	void						SetPixelMotionBlurEnable( bool b)			{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetPixelMotionBlurEnable(b);					}
	bool						IsEnablePixelMotionBlur()					{ return ( m_pRenderingStrategy && m_pRenderingStrategy->IsEnablePixelMotionBlur());			}
	void						SetPixelMotionBlurActive( bool b)			{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetPixelMotionBlurActive(b);					}
	void						SetPixelMotionBlurSamplerCount( int n)		{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetPixelMotionBlurSamplerCount(n);			}
	
	//Distortion ����
	void						SetDistortionEnable( bool bEnable)			{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetDistortionEnable(bEnable);					}			
	bool						IsEnableDistortion()						{ return ( m_pRenderingStrategy && m_pRenderingStrategy->IsEnableDistortion());					}

	//PEEdge ����
	void						SetPEQuality(int _iBlurCount, int _iScreenSize)	{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetPEQuality(_iBlurCount, _iScreenSize);	 }
	void						SetPEEdgeEnable( bool bEnable)				{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetPEEdgeEnable(bEnable);						}			
	bool						IsEnablePEEdge()							{ return ( m_pRenderingStrategy && m_pRenderingStrategy->IsEnablePEEdge());					}
	void						SetPEFactorTickNess(const float _fTickness, const float _fBlutFactor, const float _fColorPower)				{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetPEFactorTickNess(_fTickness, _fBlutFactor, _fColorPower); }

	// ����Ʈ ��ƼŬ ����
	void						SetSoftParticleEnable( bool bEnable)		{ if( m_pRenderingStrategy) m_pRenderingStrategy->SetSoftParticleEnable(bEnable);				}			
	bool						IsEnableSoftParticle()						{ return ( m_pRenderingStrategy && m_pRenderingStrategy->IsEnableSoftParticle());				}

	void						GatherAABB( RBoundingBox* _pGatherAABB );


	/// �� ���� Ÿ�� ���� �������� ���� �� �� �ִ�. �׷��Ƿ� ���̹��� Count�� ���� ���� Grade�� ����.
	void						SetRenderTargetGrade( int nGrade);	// App���� ȣ�� �뵵
	int							GetRenderTargetGrade();				// App���� ȣ�� �뵵
};

///////////////////////////////////////////////////////////////////
// TODO: static ��ǵ��� dll �϶� ����� �������� �ʴ´�
//----------------------------------------------------------------------------------------------------
inline void RSceneManager::UpdateTimer()
{
	_ASSERT( m_bTimerUpdated == false );
	REngine::GetTimer().Update();
	m_bTimerUpdated = true;
}
//----------------------------------------------------------------------------------------------------
inline void RSceneManager::UpdateTimerCustom(DWORD dwElapsed)
{
	_ASSERT( m_bTimerUpdated == false );
	REngine::GetTimer().UpdateCustom(dwElapsed);
	m_bTimerUpdated = true;
}
//----------------------------------------------------------------------------------------------------
template< typename T_ResourceMgr >
inline T_ResourceMgr* RSceneManager::GetResourceMgr(bool _bCreateIfNotExist)
{
	_ASSERT( m_arrayResourceMgr );
	RResourceManager* pResourceMgr = m_arrayResourceMgr[ T_ResourceMgr::RES_MGR_ID ];
	if( pResourceMgr == NULL && _bCreateIfNotExist )
	{
		pResourceMgr = new T_ResourceMgr;
		pResourceMgr->Init( this );

		m_arrayResourceMgr[ T_ResourceMgr::RES_MGR_ID ] = pResourceMgr;
	}
	return static_cast<T_ResourceMgr*>(pResourceMgr);
}
//----------------------------------------------------------------------------------------------------
template< typename T_Resource >
inline T_Resource* RSceneManager::CreateResource( const char* _pSzResourceName, bool bLoadMonitoring /*= false*/ )
{
	RResourceManager* pResourceMgr = GetResourceMgr<T_Resource::T_RES_MGR>(true);
	RResource* pResource = pResourceMgr->GetResource(_pSzResourceName);

	if(pResource)	// �̹� �ִٸ�..
		pResource->AddRef();
	else
	{
		pResource = new T_Resource( _pSzResourceName, this );

		if( bLoadMonitoring )
			AddResourceLoadMonitoring( pResource, true );

		pResourceMgr->AddResource( pResource );
	}

	return static_cast< T_Resource* >( pResource );
}
//----------------------------------------------------------------------------------------------------
template< typename T_Resource >
inline T_Resource* RSceneManager::GetResource( const char* _pSzResourceName )
{
	RResourceManager* pResourceMgr = GetResourceMgr<T_Resource::T_RES_MGR>(true);
	RResource* pResource = NULL;

	if (pResourceMgr)
	{
		pResource = pResourceMgr->GetResource(_pSzResourceName);
	}

	return static_cast< T_Resource* >( pResource );
}
//----------------------------------------------------------------------------------------------------
template< typename T_ResourceID >
inline RResourceManager* RSceneManager::GetResourceMgrFromID(int _ResID)
{
	_ASSERT( _ResID >= 0 && _ResID < T_ResourceID::RR_MAX );
	return m_arrayResourceMgr[ _ResID ];
}
//----------------------------------------------------------------------------------------------------
template< typename T_PassRenderer >
inline T_PassRenderer* RSceneManager::GetPassRenderer(bool _bCreateIfNotExist)
{
	_ASSERT( m_arrayPassRenderer );
	RPassRenderer* pPassRenderer = m_arrayPassRenderer[ T_PassRenderer::PASS_RENDERER_ID ];
	if( pPassRenderer == NULL && _bCreateIfNotExist )
	{
		pPassRenderer = new T_PassRenderer;
		pPassRenderer->Init();

		m_arrayPassRenderer[ T_PassRenderer::PASS_RENDERER_ID ] = pPassRenderer;
	}
	return static_cast<T_PassRenderer*>(pPassRenderer);
}
//----------------------------------------------------------------------------------------------------
inline RPassRenderer* RSceneManager::GetPassRenderer( RRENDERPASS pass )
{ 
	RPassRenderer* pPassRenderer = m_arrayPassRenderer[ (int)pass ];
	if(pPassRenderer)
		return pPassRenderer;

	return &m_defaultPassRenderers[ (int)pass ];
}
//----------------------------------------------------------------------------------------------------
inline void RSceneManager::ReleasePassRenderer( RRENDERPASS pass )
{
	if ( m_arrayPassRenderer == NULL )
		return;

	RPassRenderer* pPassRenderer = m_arrayPassRenderer[ (int)pass ];
	if(pPassRenderer)
	{
		delete pPassRenderer;
		m_arrayPassRenderer[ (int)pass ] = NULL;
	}
}

//----------------------------------------------------------------------------------------------------
inline RShadowStrategy*	RSceneManager::GetShadowStrategy()	{	return m_pShadowStrategy;	}
//----------------------------------------------------------------------------------------------------
inline RSHADOWFILTERMETHOD RSceneManager::GetShadowFilterType() { return m_eFilterType; }
//----------------------------------------------------------------------------------------------------
inline void RSceneManager::SetShadowFilterType(RSHADOWFILTERMETHOD eFilterType) { m_eFilterType = eFilterType; }
//----------------------------------------------------------------------------------------------------
inline RShaderCompositeManager*	RSceneManager::GetShaderCompositeManager() { return m_pShaderCompositeManager; }
//----------------------------------------------------------------------------------------------------
inline REffectManager*	RSceneManager::GetEffectManager() { return m_pEffectManager; }
//----------------------------------------------------------------------------------------------------


class RZoneLoadingQuery
{
public:
	RZoneLoadingQuery(void)		{}
	~RZoneLoadingQuery(void)	{}

	void	Add(RSceneNode* pSceneNode);
	bool	IsLoadingCompleted(void);

private:
	typedef std::list<RSceneNode*>	TYPE_SCENENODES;
	TYPE_SCENENODES					m_SceneNodesToLoad;
};

}