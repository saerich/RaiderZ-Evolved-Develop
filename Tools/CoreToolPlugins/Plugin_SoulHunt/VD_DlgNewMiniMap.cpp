// VD_DlgNewMiniMap.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
//#include "CoreToolRS3.h"
#include "VD_DlgNewMiniMap.h"
#include "C_ControlMediator.h"

#include "RTreePassRenderer.h"
#include "RWaterPassRenderer.h"
#include "VL_SceneNodeList.h"

#include "M_ToolStaticMeshSceneNode.h"

#include "WS_WorkSpaceManager.h"
#include "WS_Field.h"

// GDI Plus
#include <GdiPlus.h> 
#include <atlconv.h>

// CDlgNewMiniMap ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgNewMiniMap, CDialog)

CDlgNewMiniMap::CDlgNewMiniMap(CWnd* pParent /*=NULL*/)
: CDialog(CDlgNewMiniMap::IDD, pParent)
{
}

CDlgNewMiniMap::~CDlgNewMiniMap()
{
}

void CDlgNewMiniMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgNewMiniMap, CDialog)
END_MESSAGE_MAP()

bool CDlgNewMiniMap::CreateMiniMap(CControlMediator* _pControlMediator)
{
	using namespace rs3;
	CControlMediator::MAP_INFO& mapInfo = _pControlMediator->m_mapInfo;

	RDevice* pDevice = REngine::GetDevicePtr();
	_ASSERT( NULL != pDevice );

	RToolSceneManager* pSceneManager = _pControlMediator->GetToolSceneManager();
	ASSERT(pSceneManager);

	// camera
	RToolCamera* pCamera = _pControlMediator->GetToolCamera();
	ASSERT(pCamera);

	// ��ũ���� ������
	const int nWidth = 2048;
	const int nHeight = 2048;

	// �������� ����
	RVector vOrgCamPos = pCamera->GetPosition();
	RVector vOrgCamDir = pCamera->GetDirection();

	float fOldNearZ = pCamera->GetNearZ();
	float fOldFarZ = pCamera->GetFarZ();
	float fOldFovX = pCamera->GetFovX();

	REngine::GetSceneManager().GetPassRenderer<RTreePassRenderer>()->SetTreeLOD( 1000000, 1000000 );

	RTerrain* pTerrain = _pControlMediator->GetToolSceneManager()->GetCurrentTerrain();
	RToolStaticMeshSceneNode* pStaticMesh = static_cast<RToolStaticMeshSceneNode*>(_pControlMediator->GetToolSceneManager()->GetCurrentStaticMeshSceneNode());


	RBoundingBox aabb;
	aabb.Initialize();
	if( pTerrain )
	{
		aabb = pTerrain->GetAABB();
	}

	pSceneManager->GatherAABB( &aabb );

	if( pStaticMesh )
	{
		if( pTerrain )
			aabb.Add( pStaticMesh->GetAABB() );
		else
			aabb = pStaticMesh->GetAABB();
	}

	// StaticMesh�� ��� ���� ������ �������Ѵ�
	bool bStaticMeshRenderAll = false;
	if( pStaticMesh )
	{
		bStaticMeshRenderAll = pStaticMesh->IsRenderAll();
		pStaticMesh->SetRenderAll( true );
	}

	// ī�޶� ��ġ�� ��� ���ʿ� ��ġ
	RVector aabbCenter = aabb.GetCenter();
	const RVector camPos = RVector( aabbCenter.x, aabbCenter.y, aabb.vmax.z );
	const RVector camDir = RVector( 0.f, 0.f, -1.f );
	const RVector camUp = RVector( 0.f, 1.f, 0.f );
	pCamera->Set( camPos, camDir, camUp );

	// view �� identity �� �ǵ��� ����
	RVector vLength = aabb.vmax - aabb.vmin;
	if( vLength.z == 0.0f )
	{
		vLength.z = 1.0f;
	}
	vLength.z *= 1.1f;
	RMatrix orthoProjection;
	orthoProjection.MakeIdentity();
	orthoProjection.SetOrthoRH( vLength.x, vLength.y, 0.0f, vLength.z );
	pCamera->SetUserProjection( orthoProjection );
	pCamera->UpdateViewFrustrum();

	// ���� ����
	bool bFoged = pCamera->GetFogEnable();
	if( bFoged )
		pCamera->SetFogEnable(false);

	// ���Ͻ� �ø� ����
	bool bOldDistanceCullEnable = pCamera->GetDistanceCullEnable();
	pCamera->SetDistanceCullEnable( false );

	CWorkSpaceField* pField = _pControlMediator->GetWorkSpacePtr<CWorkSpaceField>();
	_ASSERT(pField);
	int fieldId = pField->m_fieldInfo.m_nReferZone;
	string szField = pField->m_fieldInfo.m_strFieldFile;

	char szFieldID[512] = {0, };
	sprintf_s(szFieldID, sizeof(szFieldID), "..\\..\\Data\\Interface\\minimap\\%05d.bmp", fieldId);

	// �� ������Ʈ
	pSceneManager->UpdateTimer();
	pSceneManager->Update();



	//���� ũ�� �缳��
	RRenderingStrategy* pRenderingStrategy = pSceneManager->GetRenderingStrategy();
	_ASSERT( pRenderingStrategy );
	pRenderingStrategy->Invalidate();
	pRenderingStrategy->Init( nWidth, nHeight );
	pRenderingStrategy->SetBackgroundBufferDivideSize(1);

	RWaterPassRenderer* pWaterPassRenderer = pSceneManager->GetPassRenderer<RWaterPassRenderer>(false);
	if( pWaterPassRenderer )
		pWaterPassRenderer->RecreateBuffer();

	// ��ũ����
	pCamera->RenderScreenShot(szFieldID, NULL, nWidth, nHeight);
	REngine::GetDevice().Flip();

	//���� ����
	pRenderingStrategy->Invalidate();
	pRenderingStrategy->Init( pDevice->GetScreenWidth(), pDevice->GetScreenHeight() );
	pRenderingStrategy->SetBackgroundBufferDivideSize(2);

	if( pWaterPassRenderer )
		pWaterPassRenderer->RecreateBuffer();

	// ���� ����
	if( bFoged )
		pCamera->SetFogEnable(true);

	// ���Ͻ� �ø� ����
	pCamera->SetDistanceCullEnable( bOldDistanceCullEnable );

	// ī�޶� ����ġ
	pCamera->SetFov(fOldFovX);
	pCamera->SetNearFarZ(fOldNearZ, fOldFarZ);
	pCamera->SetPosAndDir(vOrgCamPos, vOrgCamDir);
	pCamera->UpdateViewFrustrum();

	// StaticMesh ������ ��� ����
	if( pStaticMesh )
		pStaticMesh->SetRenderAll( bStaticMeshRenderAll );

	return true;
}
