#ifndef _RCAMERA_SCENE_NODE_H
#define _RCAMERA_SCENE_NODE_H

#include "RCameraSceneNodeController.h"
#include "RSceneManager.h"
#include "RViewFrustum.h"
#include "RDevice.h"
#include "RVType.h"

#include "RViewFrustumCullingMgr.h"
#include "ROcclusionCullingMgr.h"

#include <map>
#include <set>


namespace rs3 {

class RInferredRenderingStrategy;

RS_API extern const char *RSID_CAMERASCENENODE;
class RTechnique;
class RGIProcessor;
class RRenderingStrategy;

enum RRENDERINGMETHOD;

class RAlphaPassRenderNode {
public:
	virtual void Render() { }
};

class RS_API RCameraSceneNode : public RSceneNode
{
	MDeclareRTTI;
//	DECLARE_USING_PASS(RRP_CAMERA);
	DECLARE_NODE_ID_STRING(RSID_CAMERASCENENODE);

//	friend class RSceneManager;
	friend class RCameraPostController;
	friend class RCameraSceneNodeController;

protected:
	bool			m_camActived;

	// ���� ������ ���� ����
	bool			m_bUserProjection;
	bool			m_bUserView;

	// ���������� ���� ����
	float			m_fNearZ,m_fFarZ;
	float			m_fFovY,m_fFovX;
	float			m_fAspectRatio;			///< width / height

	float			m_fNearWidth;
	float			m_fNearHeight;

	RMatrix			m_viewMatrix;
	RMatrix			m_projMatrix;
	RMatrix			m_viewProjectionMatrix;
	RMatrix			m_raw_ProjectionMatrix;
	RViewFrustum	m_viewFrustum;
	RMatrix			m_matControllerApplied;
	bool			m_bNeedToApplyControlledMat;

	// �ټ��� RenderTarget ���� �� DepthStencil ����
	RTexture*		m_RenderTarget[MAX_RENDERTARGET];
	RTexture*		m_DepthBuffer;

	// RT�� ��� �ϴ� ��ó���� �������̸� ������ �ʿ��ϴ� - 090617, OZ
	//std::list<RTexture*>		m_MannagedRenderTargets[MAX_RENDERTARGET];
	
	RPlane			m_ClipPlane;
	bool			m_bClipPlaneEnable;

	// User ClipPlane ����
	RPlane			m_UserClipPlanes[6];
	int				m_nUserClipPlaneNum;

	bool			m_PassIgnored[RRP_MAX];	// �� ī�޶�� �� �÷��װ� true�� �� �н��� ���������� �ʽ��ϴ�.

	// Rendertarget width, height
	int				m_nWidth;
	int				m_nHeight;

	// Clear ����, �ʱ���̰�, �ʱ⽺�ٽǰ�
	DWORD			m_dwClearColor;
	float			m_fClearDepth;
	DWORD			m_dwClearStencil;

	RTechnique*		m_pTechnique;

	// Fog (���� ī�޶� ���� ����)
	float			m_fFogNearZ, m_fFogFarZ;
	DWORD			m_dwFogColor;
	bool			m_bFogEnable;
	bool			m_bUnderWater;
	float			m_fUnderWaterFogNear, m_fUnderWaterFogFar;
	DWORD			m_dwUnderWaterFogColor;

	// Shadow
	float			m_fShadowFar;

	// ���� user LOD
	bool			m_bTreeUserLOD;
	float			m_fTreeUserLOD;

	// ���� user Cull Distance
	bool			m_bTreeUserCullDistance;
	float			m_fTreeUserCullDistance;

	// Grass
	float			m_fGrassFar;
	float			m_fGrassNear;

	// LOD
	float			m_fDistanceCullingCriterion;
	float			m_fDistanceCullingCriterionTangent;
	// ��Ŭ�����ø� �������
	bool			m_bEnableOcclusionCulling;

	// ���� �������ǰ� �ִ� ����
	static RViewFrustum			m_currentViewFrustum;

	// ī�޶� ��Ʈ�ѷ� �۵��� OnUpdate���� �ϰ� ������Ʈ�� ��ƽ�� �ѹ��� �ϰ� �;
	// RActor���� ��� ������ ī��Ʈ��(�������Ӵ�) �ѹ�������Ʈ ����� ����մϴ�.
	virtual void				OnUpdate();
	virtual void				OnUpdateTransform() override;
	int							m_nLastUpdatedFrameCount;	// ������Ʈ�� �����Ӽ� ( �������ӿ� ������ ������Ʈ �����Ѵ� )

	// camera controller container
	typedef std::map< std::string, RCameraSceneNodeController* > CAMERA_CONTROLLER_MAP;
	CAMERA_CONTROLLER_MAP		m_mapCameraController[RCCT_MAX];
	
	// StaticMesh �ø��� PrimaryCamera�� �Ѵ�.
	bool						m_bStaticMeshCullFromPrimaryCamera;

	//User ViewPort ����
	bool						m_bUserViewPort;
	RViewport					m_UserViewPort;

private:
	void						UpdateProjectionMatrix();
	void						UpdateNearDimension();

	//////////////////////////////////////////////////////////////////////////
	// ���� ����� ���� ������, ������ ���ķ� �����ϱ� ���� �����صд�
	//RCameraSceneNode*			m_pBeforeCamera;
	RTexture*					m_hBeforeRenderTarget[MAX_RENDERTARGET];
	RTexture*					m_hBeforeDepthBuffer;

	//�ø��Ŵ���
	RCullingMgr*				m_pViewFrusumCullingMgr;
	//��Ŭ���� �ø��Ŵ���
	ROcclusionCullingMgrSet*	m_pOcclusionCullingMgrSet;

public:
	RCameraSceneNode(void);
	virtual ~RCameraSceneNode(void);

	void						Destroy();

	virtual bool				CreateFromXML(MXmlElement &element, bool bBackgroundCreation);
	virtual void				SaveToXML(MXmlElement &element);

	inline const RMatrix&		GetViewProjectionMatrix() const;
	inline const RMatrix&		GetRawProjectionMatrix() const;
	inline const RMatrix&		GetProjectionMatrix() const;
	inline const RMatrix&		GetViewMatrix() const;
	inline const RViewFrustum&	GetViewFrustum() const;
	inline		 RViewFrustum*	GetViewFrustumPtr();

	RMatrix						GetProjectionMatrixLH();
	RMatrix						GetViewMatrixLH();

	inline const float			GetNearWidth() { return	m_fNearWidth; }
	inline const float			GetNearHeight() { return	m_fNearHeight; }

	// Camera�� ViewMatrix �� ������ ������ Set �Լ��� ����սô�. SetTransform�� �ǵ���� �������� �ʽ��ϴ�.
	void						SetUserView(const RMatrix &matrix);
	void						Set(const RVector& pos, const RVector& dir, const RVector& up=RVector(0,0,1));
	void						SetFov(float fFovX);
	void						SetFov(float fFovX,float fAspectRatio);
	void						SetNearFarZ(float fNearZ,float fFarZ);
	void						SetUserProjection(const RMatrix &matrix);

	virtual void				UpdateViewFrustrum();

	// ����Ÿ���� ����, idx�� ���ø����̼ǿ��� ������ �����ؾ� ��, �� �Լ� �Ⱥθ��� �����ӹ��ۿ� �������մϴ�.
	bool						CreateRenderTarget(int width, int height, int idx, RFORMAT fmt = RFMT_ARGB32, RTEXTUREFILTERTYPE filter = RTF_LINEAR, DWORD dwFlags = 0);

	// ���̹��۸� ����, ũ��� �ּ��� ����Ÿ�ٰ� ���ƾ� ��
	bool						CreateDepthBuffer(int width, int height, RFORMAT fmt = RFMT_D16);

	// ����Ÿ��/���̹��� ����
	void						DestroyBuffers();

	// Near Clip Plane, projection matrix�� ���Ǿ� �����ϴ� �͵�
	void						SetClipPlane(const RPlane& p) { m_ClipPlane = p; }
	void						SetClipPlaneEnable(bool b) { m_bClipPlaneEnable = b; }
	bool						GetClipPlaneEnable() { return m_bClipPlaneEnable; }

	// User Clip Plane ( device�� UserClipPlane�� Wrapper�̸鼭, fixed�� shader �������ÿ� ������ ������ �����ϵ��� )
	void						SetUserClipPlane(const RPlane *pPlanes, int nNum);
	int							GetUserClipPlaneNum() { return m_nUserClipPlaneNum; }
	const RPlane*				GetUserClipPlane()	{	return m_UserClipPlanes; }

	void						UpdateUserClipPlane(bool bUpdate, bool bProgrammablePipe = true);

	void						SetRenderTarget(int idx, RTexture* pTexture);
	RTexture*					GetRenderTarget(int idx);	// ����Ÿ������ ������ �ؽ��ĸ� ����
	RTexture**					GetRenderTargetLP(int idx);	// ����Ÿ������ ������ �ؽ��ĸ� ����. DOF�� ��� HDR���� ���� �� RT stage 0�� �׻� �����ϰ��� �Ѵ� - 090612, OZ
	void						SetDepthBuffer(RTexture* pTexture);
	RTexture*					GetDepthBuffer();

	void						SetActive(bool p);			// ī�޶� Ȱ��ȭ�ƴ��� ����, *���* Ȱ��ȭ�� ī�޶�� ���� �ڱ� �ڽ��� ����Ÿ�ٿ� �ڱ��� �������� �������մϴ�.
	bool						GetActive();				// RDevice::SetPrimaryCamera�� �����ϸ� �ڵ����� Ȱ��ȭ �˴ϴ�. Primary���� �������� �ڵ����� ��Ȱ��.

	void						SetPassIgnore(RRENDERPASS start, RRENDERPASS end = (RRENDERPASS)(RRP_MAX-1));	// start���� end������ �н��� �����ϵ��� �����մϴ�. start�� ���� �� start���� ������
	void						SetPassIgnore(RRENDERPASS pass, bool bIgnore);
	void						RemoveAllIgnorePass();				// �н� ���� ������ ��� ����� ��� �н��� �������ϵ��� ��
	bool						GetPassIgnore(RRENDERPASS pass);	// �ش� �н��� ���� �������� ����

	void						SetClearColor(DWORD col);			// ����Ÿ�� Ŭ���� ���� ����/��ȯ
	DWORD						GetClearColor();

	void						SetClearDepth(float d);
	float						GetClearDepth();

	void						SetClearStencil(DWORD s);
	DWORD						GetClearStencil();

	bool						isHaveStencil();					// ���ٽǹ��۸� ������ �ִ°�!?
	bool						isActive();

	float						GetNearZ() const ;
	float						GetFarZ() const ;

	float						GetAspectRatio() { return m_fAspectRatio; }
	float						GetFovX() { return m_fFovX; }
	float						GetFovY() { return m_fFovY; }

	float						GetDistanceTo(const RVector& worldPosition);
	float						GetDistanceTo(const RSphere& worldSphere);

	//-------------------------------------------------------------------------------------
	// Fog ����
	void						SetFog(float n, float f, DWORD col) 
								{ m_fFogNearZ = n; m_fFogFarZ = f; m_dwFogColor = col; }
	void						SetFogEnable(bool bEnable) { m_bFogEnable = bEnable; }
	void						SetFogNear(float fNear) { m_fFogNearZ = fNear; }
	void						SetFogFar(float fFar) { m_fFogFarZ = fFar; }
	void						SetFogColor(DWORD dwColor) { m_dwFogColor = dwColor; }

	void						SetUnderWaterFog(float n, float f, DWORD col) 
								{ m_fUnderWaterFogNear = n; m_fUnderWaterFogFar = f; m_dwUnderWaterFogColor = col; }
	void						SetUnderWaterFogEnable(bool bUnderWater)	{ m_bUnderWater =bUnderWater; }
	void						SetUnderWaterFogNear(float fNear)	{ m_fUnderWaterFogNear =fNear; }
	void						SetUnderWaterFogFar(float fFar)	{ m_fUnderWaterFogFar =fFar; }
	void						SetUnderWaterFogColor(DWORD dwColor) { m_dwUnderWaterFogColor = dwColor; }
	
	
	bool						GetFogEnable() { return m_bFogEnable; }
	float						GetFogNear() { return m_bUnderWater ? m_fUnderWaterFogNear : m_fFogNearZ; }
	float						GetFogFar() { return m_bUnderWater ? m_fUnderWaterFogFar : m_fFogFarZ; }
	DWORD						GetFogColor() { return m_bUnderWater ? m_dwUnderWaterFogColor:m_dwFogColor; }
	bool						GetUnderWaterFogEnable()	{ return m_bUnderWater; }
	//-------------------------------------------------------------------------------------
	

	void						SetShadowFar( float fFar)				{ m_fShadowFar = fFar;		}
	float						GetShadowFar()							{ return m_fShadowFar;		}

	void						SetGrassFar( float fFar)				{ m_fGrassFar = fFar;		}
	float						GetGrassFar()							{ return m_fGrassFar;		}
	void						SetGrassNear( float fNear)				{ m_fGrassNear = fNear;		}
	float						GetGrassNear()							{ return m_fGrassNear;		}

	void						SetTreeUserLOD(bool bEnable, float fLOD);
	bool						GetTreeUserLODEnabled()				{ return m_bTreeUserLOD; }
	float						GetTreeUserLOD()					{ return m_fTreeUserLOD; }

	void						SetDistanceCullingCriterion( float f);
	float						GetDistanceCullingCriterion();
	float						GetDistanceCullingCriterionTangent();
	void						CalculateDistanceCullingCriterionTangent();

	virtual bool				GetTargetPosition( RVector& outPos ) { return false; } // ī�޶� �ٶ󺸴� ����� �ִ� ��� �� ��ġ�� ���´�.

	virtual void				StartShader() {}
	virtual void				EndShader() {}

	void						SetTechnique(RTechnique* pTechnique) { m_pTechnique = pTechnique; }
	RTechnique*					GetTechnique() { return m_pTechnique; }

	virtual bool				OnTestLoadingComplete(){ return true; }

	void						SetStaticMeshCullFromPrimaryCamera( bool b ) { m_bStaticMeshCullFromPrimaryCamera = b; }
	bool						IsStaticMeshCullFromPrimaryCamera() { return m_bStaticMeshCullFromPrimaryCamera; }

	//View �����󿡼� pSceneNode�� �����ϴ� ������ �ִ�, �ּ� Z���� ã�´�.
	void						FindMinMaxZ(const RSceneNode* pSceneNode, float& outMinZ, float& outMaxZ);

	RVector3					GetFarClipCornerInView();

	void						SetEnableOcclusionCulling(bool bEnable)		{	m_bEnableOcclusionCulling = bEnable;	}
	bool						GetEnableOcclusionCulling(void)				{	return m_bEnableOcclusionCulling;	}

	//////////////////////////////////////////////////////////////////////////
	// ������ ����
	void						BeginRender(int nSurface = 0, bool bTatgetCtrl = true, RRenderingStrategy* pInferredRenderingStrategy = NULL);	// cubemap �ϰ�� 0~5
	void						SetupRender(int nSurface = 0, bool bTargetCtrl = true, RRenderingStrategy* pInferredRenderingStrategy = NULL);
	void						EndRender( bool bTargetCtrl = true);

	void						RenderScreenShot(const char* szFilename, RRenderingStrategy* pRenderingStrategy, int nCustomWidth, int nCustomHeight);


	// ī�޶� ��Ʈ�ѷ� �߰� / ���� / ���
	void						AddCameraController( RCameraSceneNodeController* _pCameraController );
	RCameraSceneNodeController*	GetCameraController( RCAMERACONTROLLERTYPE _eType, const char* _pSzContollerName);

	virtual void				OnEndRender();		// EndRender ���Ŀ� �ҷ�����
	virtual void				OnClearBuffer();

	//  ���� Ÿ���� ����Ʈ ����Ʈ ��ü������ ����̽��� ���� �ϵ���... - 090619, OZ
	//void						AddRenderTarget(int idx, RTexture* pTexture);	// PostEff�� ���� ���� Ÿ���� ���� �ܰ踦 ��ĥ ���� �ִ� - 090617, OZ
	//void						DeleteRenderTarget(int idx, RTexture* pTexture);	// PostEff�� ���� ���� Ÿ���� ���� �ܰ踦 ��ĥ ���� �ִ� - 090617, OZ

	//ViewPort ����
	bool						IsHaveUserViewPort()	{ return m_bUserViewPort; }
	void						SetViewPort(int x, int y, int nWidth, int nHeight, float fMinZ = 0, float fMaxZ = 1);
	const void					SetViewPort(const RViewport viewport);
	RViewport					GetViewPort()	{ return m_UserViewPort; }

	//�ø��Ŵ���
	RCullingMgr*				GetViewFrustumCullingMgr(void)		{	return m_pViewFrusumCullingMgr;	}
	ROcclusionCullingMgrSet*	GetOcclusionCullingMgrSet(void)		{	return m_pOcclusionCullingMgrSet;	}


protected:
	virtual void				OnPreRender(  RCameraSceneNode* pCurrentCamera) {}	// beginrender �ÿ� �ҷ�����



};

//----------------------------------------------------------------------------------------------------
inline const RMatrix& RCameraSceneNode::GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }
//----------------------------------------------------------------------------------------------------
inline const RMatrix& RCameraSceneNode::GetRawProjectionMatrix() const { return m_raw_ProjectionMatrix; }
//----------------------------------------------------------------------------------------------------
inline const RMatrix& RCameraSceneNode::GetProjectionMatrix() const { return m_projMatrix; }
//----------------------------------------------------------------------------------------------------
inline const RMatrix& RCameraSceneNode::GetViewMatrix() const { return m_viewMatrix; }
//----------------------------------------------------------------------------------------------------
inline const RViewFrustum& RCameraSceneNode::GetViewFrustum() const { return m_viewFrustum;		}
inline		 RViewFrustum* RCameraSceneNode::GetViewFrustumPtr()    { return &m_viewFrustum;	}
//----------------------------------------------------------------------------------------------------
inline void	RCameraSceneNode::SetRenderTarget(int idx, RTexture* pTexture) { m_RenderTarget[idx] = pTexture; }
//----------------------------------------------------------------------------------------------------
inline RTexture*	RCameraSceneNode::GetRenderTarget(int idx) { return m_RenderTarget[idx]; }
//----------------------------------------------------------------------------------------------------
inline RTexture**	RCameraSceneNode::GetRenderTargetLP(int idx) { return &m_RenderTarget[idx]; }
//----------------------------------------------------------------------------------------------------
inline void	RCameraSceneNode::SetDepthBuffer(RTexture* pTexture) { m_DepthBuffer = pTexture; }
//----------------------------------------------------------------------------------------------------
inline RTexture*	RCameraSceneNode::GetDepthBuffer() { return m_DepthBuffer; }
//----------------------------------------------------------------------------------------------------
inline void RCameraSceneNode::SetActive(bool p) { m_camActived = p; SetVisible(m_camActived); }
//----------------------------------------------------------------------------------------------------
inline bool RCameraSceneNode::GetActive() { return m_camActived; }
//----------------------------------------------------------------------------------------------------
inline bool RCameraSceneNode::isActive()	{ return m_camActived; }
//----------------------------------------------------------------------------------------------------
inline void RCameraSceneNode::SetPassIgnore(RRENDERPASS pass, bool bIgnore)	{ m_PassIgnored[pass] = bIgnore; }
//----------------------------------------------------------------------------------------------------
inline bool RCameraSceneNode::GetPassIgnore(RRENDERPASS pass) { return m_PassIgnored[pass]; }
//----------------------------------------------------------------------------------------------------
inline DWORD RCameraSceneNode::GetClearColor() { return m_dwClearColor; }
//----------------------------------------------------------------------------------------------------
inline void RCameraSceneNode::SetClearDepth(float d) { m_fClearDepth = d; }
//----------------------------------------------------------------------------------------------------
inline float RCameraSceneNode::GetClearDepth() { return m_fClearDepth; }
//----------------------------------------------------------------------------------------------------
inline void RCameraSceneNode::SetClearStencil(DWORD s) { m_dwClearStencil = s; }
//----------------------------------------------------------------------------------------------------
inline DWORD RCameraSceneNode::GetClearStencil() { return m_dwClearStencil; }
//----------------------------------------------------------------------------------------------------
inline float RCameraSceneNode::GetNearZ()	const { return m_fNearZ; }
//----------------------------------------------------------------------------------------------------
inline float RCameraSceneNode::GetFarZ()	const { return m_fFarZ;  }
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
inline void RCameraSceneNode::AddCameraController( RCameraSceneNodeController* _pCameraController )
{
	_ASSERT( _pCameraController );
	CAMERA_CONTROLLER_MAP::iterator itr = m_mapCameraController[_pCameraController->GetType()].find( _pCameraController->GetCameraControllerID() );
	if( itr != m_mapCameraController[_pCameraController->GetType()].end() )
	{
		RCameraSceneNodeController* pBeforeController = itr->second;

		// ���������� ���ο�Ϳ� �����Ű�°��� �ƴ϶��, ������ �����
		if ( _pCameraController->IsBlendableBeforeController()  == false )
		{
			if (_pCameraController->IsManagedByCamera())
				delete pBeforeController;

			pBeforeController = NULL;
		}

		// ��Ʈ�ѷ��� ī�޶� �߰�, ���� ī�޶� ��Ʈ�ѷ��� �ִٸ� ������ �Ѱ��ش�.
		_pCameraController->InsertToCameraWhenControlAdded( this, pBeforeController );
		itr->second = _pCameraController;
	}
	else
	{
		_pCameraController->InsertToCameraWhenControlAdded( this, NULL );
		m_mapCameraController[_pCameraController->GetType()].insert( CAMERA_CONTROLLER_MAP::value_type( _pCameraController->GetCameraControllerID(), _pCameraController ) );
	}
}

//----------------------------------------------------------------------------------------------------
inline RCameraSceneNodeController* RCameraSceneNode::GetCameraController(  RCAMERACONTROLLERTYPE _eType, const char* _pSzContollerName)
{
	CAMERA_CONTROLLER_MAP::iterator itr = m_mapCameraController[_eType].find( _pSzContollerName );
	if ( itr != m_mapCameraController[_eType].end() )
		return itr->second;
	else
		return NULL;
}
//----------------------------------------------------------------------------------------------------
inline void	RCameraSceneNode::SetTreeUserLOD(bool bEnable, float fLOD)
{
	m_bTreeUserLOD = bEnable;
	m_fTreeUserLOD = fLOD;
}



}


#endif
