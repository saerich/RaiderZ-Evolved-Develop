#include "StdAfx.h"
#include "RCameraSceneNode.h"
// #include "RDevice.h"
#include "RTechnique.h"
#include "RDirectionalLightSceneNode.h"
#include "RXMLTemplate.h"
#include "mmSystem.h"
#include "RProfileInfo.h"
#include "RShadowStrategy.h"
#include "RSpotLightSceneNode.h"
#include "RShaderCompositeManager.h"
#include "RGIProcessor.h"
#include "RShaderComposite.h"
#include "RRenderHelper.h"
#include "RPSSMShadowStrategy.h"
#include "RActor.h"


namespace rs3 {

RViewFrustum	RCameraSceneNode::m_currentViewFrustum;

int CompareSceneNode(void *nullitem, RSceneNode *item1, RSceneNode *item2)
{
	_ASSERT(NULL != item1);
	_ASSERT(NULL != item2);

	if( item1 == item2 )		return 0;
	else if( item1 < item2 )	return -1;
	else						return 1;
	
	return 0;
}

// ������ ��ǥ�谡 �⺻�̴�. �ٲٷ��� �ڵ� ���� �ʿ�
#define RS3_DEFAULT_FOV		( .5f * MMath::PI )
#define RS3_DEFAULT_RATIO	((float)800/(float)600)

MImplementRTTI(RCameraSceneNode, RSceneNode);

RCameraSceneNode::RCameraSceneNode(void) 
: RSceneNode(RRP_CAMERA), m_fNearZ(10.f), m_fFarZ(10000.f)
, m_DepthBuffer(RT_FRAMEBUFFER)
, m_bClipPlaneEnable(false)
, m_camActived(false)
, m_dwClearColor(D3DCOLOR_ARGB(255, 0, 0, 0)), m_fClearDepth(1.0f), m_dwClearStencil(0L)
//, m_bFogOverride(false)
, m_bFogEnable(false), m_fFogFarZ(1.0f), m_fFogNearZ(1.0f), m_dwFogColor(0xffffffff)
, m_fDistanceCullingCriterion( 0.01f), m_fDistanceCullingCriterionTangent(0)
, m_bUnderWater(false), m_fUnderWaterFogFar(1.0f), m_fUnderWaterFogNear(1.0f), m_dwUnderWaterFogColor(0xffffffff)
, m_pTechnique(NULL)
, m_fShadowFar( 4000.0f)
, m_fGrassFar( 3000.0f), m_fGrassNear( 2000.0f)
, m_nLastUpdatedFrameCount(0)
, m_matControllerApplied(RMatrix::IDENTITY), m_bNeedToApplyControlledMat(false)
, m_bUserProjection(false)
, m_bUserView(false)
, m_bTreeUserLOD(false), m_fTreeUserLOD(1.f)
, m_bTreeUserCullDistance(false), m_fTreeUserCullDistance(0.f)
, m_nUserClipPlaneNum(0)
, m_bStaticMeshCullFromPrimaryCamera(false)
, m_bUserViewPort(false)
, m_pOcclusionCullingMgrSet(NULL)
, m_bEnableOcclusionCulling(true)
{
	SetFov(RS3_DEFAULT_FOV,RS3_DEFAULT_RATIO);

	for (int i=1; i<MAX_RENDERTARGET; i++)
		m_RenderTarget[i] = R_NONE;

	m_RenderTarget[0] = RT_FRAMEBUFFER;
	m_nWidth = REngine::GetConfig().m_nWidth;
	m_nHeight = REngine::GetConfig().m_nHeight;

	SetVisible(m_camActived);
	RemoveAllIgnorePass();

	m_pViewFrusumCullingMgr = new RViewFrustumCullingMgr;
	m_pOcclusionCullingMgrSet = new ROcclusionCullingMgrSet;
}

RCameraSceneNode::~RCameraSceneNode(void)
{
	Destroy();

	if( NULL != m_pViewFrusumCullingMgr)
	{
		delete m_pViewFrusumCullingMgr;
		m_pViewFrusumCullingMgr = NULL;
	}

	SAFE_DELETE(m_pOcclusionCullingMgrSet);
}

void RCameraSceneNode::Destroy()
{
	for( int i = 0; i < RCCT_MAX; ++i )
	{
		for(CAMERA_CONTROLLER_MAP::iterator itr = m_mapCameraController[i].begin(); itr != m_mapCameraController[i].end(); ++itr)
		{
			RCameraSceneNodeController* pController = itr->second;
			pController->OnRemoveFromCamera();
			pController->m_pCameraSceneNode = NULL;
			if (pController->IsManagedByCamera())
				delete pController;
		}
		
		m_mapCameraController[i].clear();
	}
}

void RCameraSceneNode::SetUserView(const RMatrix &matrix)
{
	m_bUserView = true;
	m_viewMatrix = matrix;
}

void RCameraSceneNode::Set(const RVector& pos, const RVector& dir, const RVector& up)
{
//	m_position = pos;
//	m_direction = dir;
//	m_up = up;

//	m_bNeedUpdateLocalTransform = true;

	m_bUserView = false;
	SetTransform(pos,dir,up);
}

void RCameraSceneNode::UpdateProjectionMatrix()
{
	if(!m_bUserProjection)
		m_projMatrix.SetProjectionMatrixFovRH( m_fFovY, m_fAspectRatio, m_fNearZ, m_fFarZ);
}

void RCameraSceneNode::UpdateNearDimension()
{
	if(!m_bUserProjection)
	{
		m_fNearHeight = tan( m_fFovY/2) * m_fNearZ * 2;
		m_fNearWidth = tan( m_fFovX/2) * m_fNearZ * 2;
	}
}

void RCameraSceneNode::SetFov(float fFovX,float fAspectRatio)
{
	m_bUserProjection = false;
	m_fAspectRatio = fAspectRatio;
	m_fFovX = fFovX;
	m_fFovY = atanf(tanf(m_fFovX/2.0f)/m_fAspectRatio)*2.0f;
	UpdateProjectionMatrix();
	UpdateNearDimension();
	CalculateDistanceCullingCriterionTangent();
}

void RCameraSceneNode::SetFov(float fFovX)
{
	m_bUserProjection = false;
	SetFov(fFovX,(float)REngine::GetDevice().GetScreenWidth() / (float)REngine::GetDevice().GetScreenHeight());
}

void RCameraSceneNode::SetNearFarZ(float fNearZ,float fFarZ)
{
	m_fNearZ = fNearZ;
	m_fFarZ = fFarZ;
	UpdateProjectionMatrix();
	UpdateNearDimension();
}

void RCameraSceneNode::SetUserProjection(const RMatrix &matrix)
{
	m_bUserProjection = true;
	m_fNearZ = 0.0f;
	m_fFarZ = 0.0f;

	m_projMatrix = matrix;
}

void RCameraSceneNode::SetUserClipPlane(const RPlane *pPlanes, int nNum)
{
	m_nUserClipPlaneNum = ( nNum > 0 ? nNum : 0 );
	if ( m_nUserClipPlaneNum )
	{
		for ( int i = 0; i<nNum; ++i )
			m_UserClipPlanes[i] = pPlanes[i];
	}
}

void RCameraSceneNode::UpdateUserClipPlane(bool bUpdate, bool bProgrammablePipe /* = true */)
{
	RDevice *pDevice = REngine::GetDevicePtr();
	if ( m_nUserClipPlaneNum == 0 || bUpdate == false)
	{
		pDevice->SetClipPlanes(NULL, 0);
		return;
	}

	if ( !bProgrammablePipe )
	{
		pDevice->SetClipPlanes(m_UserClipPlanes, m_nUserClipPlaneNum);
	}
	else 
	{
		RPlane planes[6];
		for ( int i = 0; i<m_nUserClipPlaneNum; ++i )
		{
			GetViewProjectionMatrix().TransformPlane(m_UserClipPlanes[i], planes[i]);
			planes[i].Normalize();
		}

		pDevice->SetClipPlanes(planes, m_nUserClipPlaneNum);
	}
}

void RCameraSceneNode::UpdateViewFrustrum()
{
	UpdateTransform();

	if (!m_bUserView)
		m_viewMatrix.SetLookAtMatrixRH( GetWorldPosition(), GetWorldPosition()+GetWorldDirection(), GetWorldUp() );

	UpdateProjectionMatrix();

	m_viewProjectionMatrix = m_viewMatrix * m_projMatrix;
	m_raw_ProjectionMatrix = m_projMatrix;

	if (m_bClipPlaneEnable)									// near clip plane�� ������ �÷������� ��ȯ�ϴ� �ڵ�
	{
		RMatrix cilpProjection = RMatrix::IDENTITY;

		RPlane transformedPlane;
		RPlane clipPlane = m_ClipPlane;
		clipPlane.Normalize();								// custom clip plane�� ����ȭ

		RMatrix tt;

		m_viewProjectionMatrix.TransformPlane(clipPlane, transformedPlane);		// V-P ��ķ� ��ȯ (inverse+transpose)

		if (transformedPlane.d > 0)												// ����� �ü��� ���� �����̸� ������
		{
			transformedPlane = -transformedPlane;
			transformedPlane.d += 1;
		}

		cilpProjection._13 = transformedPlane.a;								// Z�������� ��ȯ�ϴ� ��� ����
		cilpProjection._23 = transformedPlane.b;
		cilpProjection._33 = transformedPlane.c;
		cilpProjection._43 = transformedPlane.d;

		m_projMatrix *= cilpProjection;											// ������ ���� ��Ŀ� �ռ�

		if (m_RenderTarget[0] != R_NONE)
		{
			// �ݻ� ������ ���̱� ���� ������ ������������ �ణ ��ȯ
			//RMatrix t = RMatrix::IDENTITY;		

			//const RTextureInfo *tInfo = REngine::GetDevice().GetTextureInfo(m_RenderTarget[0]);

			//float offset = 4.0f/tInfo->nWidth;

			////t.SetScale(RVector(1, 1, 0.75f));			// ���� -_-
			//t.SetTranslation(RVector(0, 0, offset));

			//if (GetAsyncKeyState(VK_LCONTROL))
			//	m_projMatrix *= t;
		}

		m_viewProjectionMatrix = m_viewMatrix * m_projMatrix;
	}

	m_viewFrustum.SetFromMatrix(m_viewProjectionMatrix);

	REngine::GetDevice().SetTransform(RST_VIEW, m_viewMatrix);
	REngine::GetDevice().SetTransform(RST_PROJECTION, m_projMatrix);
}

bool RCameraSceneNode::CreateRenderTarget(int width, int height, int idx, RFORMAT fmt, RTEXTUREFILTERTYPE filter , DWORD dwFlags )
{
	m_nWidth = width;
	m_nHeight = height;
	m_RenderTarget[idx] = REngine::GetDevice().CreateRenderTargetTexture(width, height, fmt, filter, dwFlags);
	return m_RenderTarget[idx] != NULL;
}

bool RCameraSceneNode::CreateDepthBuffer(int width, int height, RFORMAT fmt)
{
	m_DepthBuffer = REngine::GetDevice().CreateRenderTargetTexture(width, height, fmt);
	return m_DepthBuffer != NULL;
}

void RCameraSceneNode::DestroyBuffers()
{
	for (int i=0; i<MAX_RENDERTARGET; i++)
	{
		if (m_RenderTarget[i] != RT_FRAMEBUFFER && m_RenderTarget[i] != R_NONE)
			REngine::GetDevice().DeleteTexture(m_RenderTarget[i]);

		m_RenderTarget[i] = R_NONE;
	}

	if (m_DepthBuffer != RT_FRAMEBUFFER && m_DepthBuffer != R_NONE )
		REngine::GetDevice().DeleteTexture(m_DepthBuffer);

	m_DepthBuffer = R_NONE;
	m_nWidth = REngine::GetConfig().m_nWidth;
	m_nHeight = REngine::GetConfig().m_nHeight;
}

void RCameraSceneNode::SetPassIgnore(RRENDERPASS start, RRENDERPASS end)
{
	_ASSERT(start>=0 && start <RRP_MAX);
	_ASSERT(end>=0 && end <RRP_MAX);
	_ASSERT(start<=end);
	for (int i=start; i<=end; i++)
		m_PassIgnored[i] = true;
}

void RCameraSceneNode::RemoveAllIgnorePass()
{
	for (int i=0; i<RRP_MAX; i++)
		m_PassIgnored[i] = false;
}

bool RCameraSceneNode::isHaveStencil()
{
	_ASSERT(m_DepthBuffer!=R_NONE);

	RFORMAT pixelformat;
	if ( m_DepthBuffer == RT_FRAMEBUFFER )
		pixelformat = REngine::GetConfig().depthFormat;
	else
		pixelformat = REngine::GetDevice().GetTextureInfo(m_DepthBuffer)->pixelFormat;

	return ( pixelformat == RFMT_D24S8 );
}


void RCameraSceneNode::SetClearColor(DWORD col) 
{ 
	m_dwClearColor = col; 
}

RMatrix RCameraSceneNode::GetProjectionMatrixLH()
{
	RMatrix matProjectionLH;
	matProjectionLH.SetProjectionMatrixFovLH( m_fFovY, m_fAspectRatio, m_fNearZ, m_fFarZ);
	return matProjectionLH;
}

RMatrix RCameraSceneNode::GetViewMatrixLH()
{
	RMatrix matViewLH;
	matViewLH.SetLookAtMatrixLH( GetWorldPosition(), GetWorldPosition()+GetWorldDirection(), GetWorldUp());
	return matViewLH;
}

void RCameraSceneNode::SetupRender(int nSurface, bool bTatgetCtrl, RRenderingStrategy* pRenderingStrategy)
{
	if (!m_camActived) 
		return;

	if( bTatgetCtrl)
	{
		for (int i=0; i<MAX_RENDERTARGET; i++)
		{
			REngine::GetDevice().SetRenderTarget(i, m_RenderTarget[i], nSurface);
		}

		// ���� ���ٽ��� ���� �� �� ���� ���� ��� BeginRender()���� m_hBeforeDepthBuffer�� ��� -> EndRender()���� �������� - COMMENT OZ - 090622, OZ
		REngine::GetDevice().SetDepthStencilBuffer(m_DepthBuffer);
	}

	// Post ������������ ���� Ÿ�� ���� ���� �ؼ� ������ ���� ���� �ؾ� �� ���۵��� �ִ�. - 090617, OZ
	// Pre�� �������� ��ȸ �� ���̹Ƿ� ���������� ���� ���� ó�� �Ѵ�.
	if( !m_mapCameraController[RCCT_POSTRENDER].empty() )
	{
		for(CAMERA_CONTROLLER_MAP::iterator itr = m_mapCameraController[RCCT_POSTRENDER].begin(); itr != m_mapCameraController[RCCT_POSTRENDER].end();)
		{
			RCameraSceneNodeController* pController = itr->second;

			if( pController->IsRemoveReserved() )
			{
				pController->OnRemoveFromCamera();

				if (pController->IsManagedByCamera())
					delete pController;

				m_mapCameraController[RCCT_POSTRENDER].erase( itr++ );
			}
			else
			{
				++itr;
			}
		}
	}


//	OnClearBuffer();

	UpdateViewFrustrum();

	OnPreRender( this);
}

void RCameraSceneNode::OnClearBuffer()
{
	// TODO: clear �� �ڵ����� ���� ���� ��������� �� �δ°� ������
	if(GetDepthBuffer()!= RT_FRAMEBUFFER)	// �����ӹ��۰� �ƴϸ� Ŭ����
		REngine::GetDevice().Clear(true, true, isHaveStencil(), GetClearColor(), GetClearDepth(), GetClearStencil());
}

void RCameraSceneNode::BeginRender(int nSurface, bool bTatgetCtrl, RRenderingStrategy* pRenderingStrategy)
{
	_ASSERT(GetActive()); // ��Ƽ�갡 �ƴѰ� �ǵ��ΰ��� ?

	RDevice *pDevice = REngine::GetDevicePtr();

	// render target backup
	if( bTatgetCtrl)
	{
		for(unsigned int i=0; i<MAX_RENDERTARGET; ++i )
			m_hBeforeRenderTarget[i] = pDevice->GetRenderTarget(i);
		m_hBeforeDepthBuffer = pDevice->GetDepthStencilBuffer();
	}

	// pre render
	SetupRender( nSurface, bTatgetCtrl, pRenderingStrategy);

	// begin technique
	if (GetTechnique()!=NULL) 
	{
		GetTechnique()->Begin(NULL, TECH_PRESERVE_NOT);
		GetTechnique()->BeginPass(0);
	}
	
}

void RCameraSceneNode::EndRender(bool bTatgetCtrl)
{
	// end technique
	if (GetTechnique()!=NULL) 
	{
		GetTechnique()->EndPass();
		GetTechnique()->End();
	}

	// restore render target
	if( bTatgetCtrl)
	{
		for(unsigned int i=0; i<MAX_RENDERTARGET; ++i )
			REngine::GetDevice().SetRenderTarget(i, m_hBeforeRenderTarget[i]);
		REngine::GetDevice().SetDepthStencilBuffer(m_hBeforeDepthBuffer);
	}

//	OnEndRender();
}

void RCameraSceneNode::OnEndRender()
{
	// ������ ��Ʈ���� ������ ��Ʈ���� ������ �ش�.
	if( !m_mapCameraController[RCCT_POSTRENDER].empty() )
	{
		for(CAMERA_CONTROLLER_MAP::iterator itr = m_mapCameraController[RCCT_POSTRENDER].begin(); itr != m_mapCameraController[RCCT_POSTRENDER].end();)
		{
			RCameraSceneNodeController* pController = itr->second;

			if( pController->IsRemoveReserved() )
			{
				pController->OnRemoveFromCamera();

				if (pController->IsManagedByCamera())
					delete pController;

				m_mapCameraController[RCCT_POSTRENDER].erase( itr++ );
			}
			else
			{
				pController->OnPostRender();
				++itr;
			}
		}
	}
}

bool RCameraSceneNode::CreateFromXML(MXmlElement &element, bool bBackgroundCreation)
{
	// ��׶��� �ε����� �ʴ´�.
	MXmlElement *pElement = &element;
	using namespace TXML;

	m_fNearZ = _GETElement<float>::Value(pElement,_T("NEARZ"),_T("float"));
	m_fFarZ = _GETElement<float>::Value(pElement,_T("FARZ"),_T("float"));
	m_fFovY = _GETElement<float>::Value(pElement,_T("FOVY"),_T("float"));
	m_fAspectRatio = _GETElement<float>::Value(pElement,_T("ASPECTRATIO"), _T("float"));
	return true;
}

void RCameraSceneNode::SaveToXML(MXmlElement &element)
{
	MXmlElement *pElement = &element;

	using namespace TXML;
	
	_SetElementValue(pElement,_T("NEARZ"),_T("float"), m_fNearZ);
	_SetElementValue(pElement,_T("FARZ"),_T("float"), m_fNearZ);
	_SetElementValue(pElement,_T("FOVY"),_T("float"), m_fFovY);
	_SetElementValue(pElement,_T("ASPECTRATIO"), _T("float"), m_fAspectRatio);

	return;
}

void RCameraSceneNode::OnUpdate()
{
	RPFC_THISFUNC;

	_ASSERT( REngine::GetDevicePtr() );

	// ���� �����ӿ��� �ι� ������Ʈ ���� �ʴ´�.
	if(m_nLastUpdatedFrameCount==REngine::GetDevice().GetFrameCount())
		return;

	m_nLastUpdatedFrameCount = REngine::GetDevice().GetFrameCount();

	// ������ ��Ʈ���� ������ ��Ʈ���� ������ �ش�.
	if( !m_mapCameraController[RCCT_PREUPDATE].empty() )
	{
		m_matControllerApplied.MakeIdentity();
		for(CAMERA_CONTROLLER_MAP::iterator itr = m_mapCameraController[RCCT_PREUPDATE].begin(); itr != m_mapCameraController[RCCT_PREUPDATE].end();)
		{
			RCameraSceneNodeController* pController = itr->second;

			if( pController->IsRemoveReserved() )
			{
				pController->OnRemoveFromCamera();
				pController->m_pCameraSceneNode = NULL;

				if (pController->IsManagedByCamera())
					delete pController;

				m_mapCameraController[RCCT_PREUPDATE].erase( itr++ );
			}
			else
			{
				pController->UpdateCameraController(m_matControllerApplied);
				++itr;
			}
		}
		NeedUpdateTransform();
		m_bNeedToApplyControlledMat = true;
	}
	else if (m_bNeedToApplyControlledMat)
	{
		m_matControllerApplied.MakeIdentity();
		m_bNeedToApplyControlledMat = false;
	}
}

void RCameraSceneNode::OnUpdateTransform()
{
	if (m_bNeedToApplyControlledMat)
	{
		(*m_matWorld) = m_matControllerApplied * (*m_matLocal);
	}
}

float RCameraSceneNode::GetDistanceTo(const RVector& worldPosition)
{
	//RVector trPos = worldPosition * GetViewProjectionMatrix();	// w �� ������ �ʴ´�
	//return trPos.z;

	/// ������ ���̱� ���� ���� ����
	//MVector3 operator * ( const MVector3& v, const MMatrix& tm)
	// out.x = in.x*_11 + in.y*_21 + in.z*_31 + _41;
	// out.y = in.x*_12 + in.y*_22 + in.z*_32 + _42;
	// out.z = in.x*_13 + in.y*_23 + in.z*_33 + _43;
	return worldPosition.x*GetViewProjectionMatrix()._13 + worldPosition.y*GetViewProjectionMatrix()._23 + worldPosition.z*GetViewProjectionMatrix()._33 + GetViewProjectionMatrix()._43;
}

float RCameraSceneNode::GetDistanceTo(const RSphere& worldSphere)
{
	float fDistance = GetDistanceTo(worldSphere.center);
	fDistance -= worldSphere.radius;
	if( fDistance < 0.f )
		fDistance = 0.f;

	return fDistance;
}

void RCameraSceneNode::RenderScreenShot(const char* szFilename, RRenderingStrategy* pRenderingStrategy, int nCustomWidth, int nCustomHeight)
{
	RDevice* pDevice = REngine::GetDevicePtr();
	RSceneManager* pSceneManager = REngine::GetSceneManagerPtr();

	if(!pRenderingStrategy)
		pRenderingStrategy = pSceneManager->GetRenderingStrategy();

	// custom 
	int nWidth(m_nWidth), nHeight(m_nHeight);

	if (nCustomWidth > 0)
		nWidth = nCustomWidth;

	if (nCustomHeight > 0)
		nHeight = nCustomHeight;

	RTexture* hRenderTarget = pDevice->CreateRenderTargetTexture(nWidth, nHeight, RFMT_XRGB32);
	RTexture* hDepthStencil = pDevice->CreateRenderTargetTexture(nWidth, nHeight, RFMT_D24X8/*RFMT_D16*/);	// ��ũ�� ���� ���� ���е��� ����
	
	RTexture* hBeforeTarget = pDevice->SetRenderTarget(0, hRenderTarget);
	RTexture* hBeforeDepth = pDevice->SetDepthStencilBuffer(hDepthStencil);

	// 0xff0000ff�� ������ ����?
	DWORD dwClrOrg = GetClearColor();
	SetClearColor( 0xff0000ff);
	pDevice->Clear(true,true,false,0xff0000ff);
	pDevice->BeginScene();

	if(pRenderingStrategy)
	{
		pRenderingStrategy->RenderScene( this, pSceneManager->GetCurrentRootSceneNode(false), pSceneManager->GetShadowStrategy(), NULL);
	}



// 	// Ÿ�� ��Ʈ���� false�� �־� Ÿ���� �ǵ帮���� ���� �״� SetRenderTarget()������ �͵� ��
// 	// ������ ī�޶� ��Ʈ�ѷ����� ���� �� ����ϰ� ���� �� ������ ���� SetRenderTarget()������ �;� ��
// 	BeginRender( 0, false);
// 
// 	pDevice->Clear(true,true,false,0xff0000ff);
// 
// 	// 0xff0000ff�� ������ ����?
// 	DWORD dwClrOrg = GetClearColor();
// 	SetClearColor( 0xff0000ff);
// 
// 	OnClearBuffer(); 
// 
// 	pDevice->BeginScene();
// 
// 	RShadowStrategy* pShadowStrategy = pSceneManager->GetShadowStrategy();
// 	pShadowStrategy->Render( this, pSceneManager);
// 
// 	REngine::GetSceneManager().Cull(this);
// 	Render();
// 
// 	SetClearColor( dwClrOrg);
// 
// 	EndRender(false);
// 	// ī�޶� ��Ʈ�ѷ����� ���� �� ���� ��� �ϴϱ� SetRenderTarget()������ �;��� �ʰڳ�..
// 	OnEndRender();


	SetClearColor( dwClrOrg);

	pDevice->SetRenderTarget(0, hBeforeTarget);
	pDevice->SetDepthStencilBuffer(hBeforeDepth);


	if(szFilename && strlen(szFilename) )
		hRenderTarget->SaveToFile(szFilename);

	pDevice->EndScene();

	pDevice->DeleteTexture(hRenderTarget);
	pDevice->DeleteTexture(hDepthStencil);

	return;
}


void RCameraSceneNode::FindMinMaxZ(const RSceneNode* pSceneNode, float& outMinZ, float& outMaxZ)
{
	if ( !pSceneNode->GetUsable() || !pSceneNode->GetVisible() || pSceneNode->GetOcclued() )
		return;

	const RBoundingBox &aabb = pSceneNode->GetAABB();
	if ( !aabb.IsValid() )	return; // ��ȿ���� ���� �� ����

	RVector3 vCenter = aabb.GetCenter();
	float fRadius = (vCenter - aabb.vmax).Length();

// 	RVector3 vCameraToCenter = vCenter - GetWorldPosition();
// 	RVector3 vViewDirection = GetWorldDirection();
// 
// 	float fZ = DotProduct(vCameraToCenter, vViewDirection);

	RMatrix matView = GetViewMatrix();
	RVector4 vAxisZ(matView._13, matView._23, matView._33, matView._43);

	float vViewSpaceCenterZ = vAxisZ.DotProduct(RVector4(vCenter, 1));

	outMinZ = vViewSpaceCenterZ-fRadius;
	outMaxZ = vViewSpaceCenterZ+fRadius;
}

RVector3 RCameraSceneNode::GetFarClipCornerInView()
{
	RPFC_THISFUNC;
	RVector3 vP = m_viewFrustum.GetPoints()[0];
	m_viewMatrix.TransformVect(vP);
	return RVector3(abs(vP.x),-abs(vP.y),vP.z);
}

void RCameraSceneNode::SetViewPort(int x, int y, int nWidth, int nHeight, float fMinZ, float fMaxZ)
{
	m_UserViewPort = RViewport(x,y,nWidth,nHeight,fMinZ,fMaxZ);
	m_bUserViewPort =true;
}

const void RCameraSceneNode::SetViewPort(const RViewport viewport)
{ 
	m_UserViewPort = viewport; 
	m_bUserViewPort =true;
}

void RCameraSceneNode::CalculateDistanceCullingCriterionTangent()
{
	if( GetDistanceCullingCriterion() > 0 && GetFovY() > 0)
		m_fDistanceCullingCriterionTangent = tan( GetFovY() * GetDistanceCullingCriterion()) / 2;
	else
		m_fDistanceCullingCriterionTangent = 0;
}

void  RCameraSceneNode::SetDistanceCullingCriterion( float f)
{																																			
	m_fDistanceCullingCriterion = f; 
	CalculateDistanceCullingCriterionTangent(); 
}
											
float RCameraSceneNode::GetDistanceCullingCriterion()
{ 
	return m_fDistanceCullingCriterion; 
}

float RCameraSceneNode::GetDistanceCullingCriterionTangent()
{	
	return m_fDistanceCullingCriterionTangent; 
}





//  ���� Ÿ���� ����Ʈ ����Ʈ ��ü������ ����̽��� ���� �ϵ���... - 090619, OZ
// PostEff�� ���� ���� Ÿ���� ���� �ܰ踦 ��ĥ ���� �ִ� - 090617, OZ
//void RCameraSceneNode::AddRenderTarget(int idx, RTexture* pTexture)
//{
//	
//	// ������ �ִ��� üũ
//	std::list<RTexture*>::iterator itr_finder
//		= std::find( m_MannagedRenderTargets[idx].begin(), m_MannagedRenderTargets[idx].end(), pTexture);
//	if(itr_finder != m_MannagedRenderTargets[idx].end())
//		return;
//
//	m_MannagedRenderTargets[idx].push_back( pTexture);
//
//
//}
//
//
// PostEff�� ���� ���� Ÿ���� ���� �ܰ踦 ��ĥ ���� �ִ� - 090617, OZ
//void RCameraSceneNode::DeleteRenderTarget(int idx, RTexture* pTexture)
//{
//
//	// ������ �ִ��� üũ
//	std::list<RTexture*>::iterator itr_finder
//		= std::find( m_MannagedRenderTargets[idx].begin(), m_MannagedRenderTargets[idx].end(), pTexture);
//	// ������ ����
//	if(itr_finder != m_MannagedRenderTargets[idx].end())
//		m_MannagedRenderTargets[idx].erase( itr_finder);
//
//}

}