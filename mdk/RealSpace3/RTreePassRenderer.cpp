#include "stdafx.h"

#include "RTreePassRenderer.h"
#include "RTreeResource.h"
#include "RTreeResourceMgr.h"
#include "RTreeSceneNode.h"

#include "RDirectionalLightSceneNode.h"

#include "RCameraSceneNode.h"
#include "RConfiguration.h"

#include "RRenderHelper.h"
#include "RProfileInfo.h"

#include "RDeviceD3D.h"
#include "RFont.h"
#include "RUtil.h"
#include "RShadowStrategy.h"
#include "RShaderCompositeManager.h"
#include "RShaderFX.h"

#include "RShaderComposite.h"
#include "RSpotLightSceneNode.h"

namespace rs3 {

// ��� ��Ʈ���� �ȷ�Ʈ ��.
// �̸� ���� �ϸ� shader_compile_and_commit.bat�� ���� �ؾ� ��.
#define TREE_INSTAICING_SIZE	20 

//////////////////////////////////////////////////////////////////////////
RTreePassRenderer::STreeBillboardRenderer::STreeBillboardRenderer()
{
	m_vb.vbSize = 0;
}

void RTreePassRenderer::STreeBillboardRenderer::Create(int vsz, RDevice* pDevice)
{
	vertexSize = vsz;
	m_vb.vertexSize = vertexSize;
	m_pDevice = pDevice;
}

void RTreePassRenderer::STreeBillboardRenderer::Destroy()
{
	if (m_vb.vbSize > 0)
	{
		REngine::GetDevice().DeleteVertexBuffer(m_vb.vb);
		m_vb.vb = R_NONE;
		m_vb.vbSize = 0;
	}
}

void RTreePassRenderer::STreeBillboardRenderer::AddVertices(SSpeedTreeBillboard::SVertex* data, int itemNum)
{
	m_vb.AddVertices(data, itemNum);
}

void RTreePassRenderer::STreeBillboardRenderer::ClearVertices()
{
	RPFC_THISFUNC;
	if (m_vb.vbSize > 0)
		m_vb.ClearVertices();
}

int RTreePassRenderer::STreeBillboardRenderer::Render(RTechnique &shaderTechnique)
{
	RPFC_THISFUNC;
	int ret = 0;

	if (m_vb.VertexData.size() > 0)
	{
		m_vb.ApplyData();
		m_vb.SetVertexBufferToDevice();

		UINT nPass = 0;
		shaderTechnique.Begin(&nPass, TECH_PRESERVE_NOT);
		for ( UINT ui = 0; ui<nPass; ++ui )
		{
			shaderTechnique.BeginPass(ui);
			REngine::GetDevice().DrawPrimitive(RPT_TRIANGLELIST, 0, m_vb.VertexData.size() / 3);
			ret += m_vb.VertexData.size() / 3;
			shaderTechnique.EndPass();
		}
		shaderTechnique.End();
	}

	ClearVertices();
	return ret;
}


//////////////////////////////////////////////////////////////////////////
// class TreePassRendererListener : public RListener
// {
// 	RTreePassRenderer* m_pTreePassRenderer;
// public:
// 	TreePassRendererListener( RTreePassRenderer* pTreePassRenderer) : m_pTreePassRenderer( pTreePassRenderer) {}
// 
// 	virtual void Update() override
// 	{
// 		m_pTreePassRenderer->Destroy();
// 		m_pTreePassRenderer->Init();
// 	}
// };
// 
//////////////////////////////////////////////////////////////////////////

RTreePassRenderer::RTreePassRenderer() :m_hBranchVertexFormat(R_NONE),
							m_hFrondVertexFormat(R_NONE),
							m_hLeafCardVertexFormat(R_NONE),
							m_hLeafMeshVertexFormat(R_NONE),
							m_pTreeShader(NULL),
							m_bDoNotRender(false),
							m_bDrawCollisionObject(false),
							m_nPolyCount(0),
							m_fNearLOD(RTREE_DEFAULT_LOD_NEAR),
							m_fFarLOD(RTREE_DEFAULT_LOD_FAR),
							m_pInstancingData(NULL),
							m_pInstancingPosition(NULL),
							m_bUseInstancing(true),
							m_nMRTCount(REngine::GetSceneManager().GetRenderingStrategy()->GetMRTCount())
{	
}

RTreePassRenderer::~RTreePassRenderer() 
{
	SAFE_DELETE_ARRAY( m_pInstancingPosition);
	SAFE_DELETE_ARRAY( m_pInstancingData);



	//http://dev:8181/projects/engine/ticket/1218
	//CSpeedTreeRT ��ü���� ���׷� �޸� ���� ���ٰ� �Ǵ��Ͽ�,
	//�װ��� �����ڵ带 �̷��� �ھƳ���.
	//CSpeedTreeRT���� static������ new�� �Ҵ�޾Ƽ� ������ �Ǵµ�,
	//�� ������ CSpeedTreeRT�� �Ҹ������ �������ش�. 
	//�׷��� CSpeedTreeRT�� �ν��Ͻ��� �ȸ��������, CSpeedTreeRT�� �Ҹ��ڰ� �ȺҸ���,
	//CSpeedTreeRT���� static������ ������ �ȵȴ�.- ignore
	CSpeedTreeRT *p = new CSpeedTreeRT;
	delete p;
}

//////////////////////////////////////////////////////////////////////////

void RTreePassRenderer::InitVertexFormats()
{
	RVertexAttributeEx branchattribs[] = 
	{ 
		///////// mesh data /////////////////
		RVertexAttributeEx( 0, TYPE_VERTEX, 0, FORMAT_FLOAT, 3),
		RVertexAttributeEx( 0, TYPE_NORMAL, 0, FORMAT_FLOAT, 3),
		// �ν��Ͻ��� ���ؼ��� �ؽ��� �ڵ���̼��� �����Ͽ� 4�� ����
		RVertexAttributeEx( 0, TYPE_TEXCOORD, 0, FORMAT_FLOAT, 4),		// tex, wind
		RVertexAttributeEx( 0, TYPE_TEXCOORD, 1, FORMAT_FLOAT, 4),		// shadow, detail
		///////// instancing /////////////////
		RVertexAttributeEx( 1, TYPE_TEXCOORD, 4, FORMAT_FLOAT, 1),		// Idx
	};

	RVertexAttributeEx leafcardattribs[] = 
	{ 
		///////// mesh data /////////////////
		RVertexAttributeEx( 0, TYPE_VERTEX, 0, FORMAT_FLOAT, 4),
		RVertexAttributeEx( 0, TYPE_NORMAL, 0, FORMAT_FLOAT, 3),
		RVertexAttributeEx( 0, TYPE_TEXCOORD, 0, FORMAT_FLOAT, 4), 
		RVertexAttributeEx( 0, TYPE_TEXCOORD, 1, FORMAT_FLOAT, 4),
		RVertexAttributeEx( 0, TYPE_TEXCOORD, 2, FORMAT_FLOAT, 4),
		///////// instancing /////////////////
		RVertexAttributeEx( 1, TYPE_TEXCOORD, 4, FORMAT_FLOAT, 1),		// Idx
	};

	RVertexAttributeEx leafmeshattribs[] = 
	{ 
		///////// mesh data /////////////////
		RVertexAttributeEx( 0, TYPE_VERTEX, 0, FORMAT_FLOAT, 4),
		RVertexAttributeEx( 0, TYPE_NORMAL, 0, FORMAT_FLOAT, 3),
		RVertexAttributeEx( 0, TYPE_TEXCOORD, 0, FORMAT_FLOAT, 4), 
		RVertexAttributeEx( 0, TYPE_TEXCOORD, 1, FORMAT_FLOAT, 3),
		RVertexAttributeEx( 0, TYPE_TEXCOORD, 2, FORMAT_FLOAT, 3),
		RVertexAttributeEx( 0, TYPE_TEXCOORD, 3, FORMAT_FLOAT, 4),
		///////// instancing /////////////////
		RVertexAttributeEx( 1, TYPE_TEXCOORD, 4, FORMAT_FLOAT, 1),		// Idx
	};

	RVertexAttribute billboardattribs[] = 
	{ 
		TYPE_VERTEX, FORMAT_FLOAT, 4,
		TYPE_TEXCOORD, FORMAT_FLOAT, 4, 
		TYPE_TEXCOORD, FORMAT_FLOAT, 4,
		TYPE_TEXCOORD, FORMAT_FLOAT, 3
	};


	m_hBranchVertexFormat = REngine::GetDevice().CreateVertexFormat(branchattribs, _countof(branchattribs));
	m_hFrondVertexFormat = REngine::GetDevice().CreateVertexFormat(branchattribs, _countof(branchattribs));
	m_hLeafCardVertexFormat = REngine::GetDevice().CreateVertexFormat(leafcardattribs, _countof(leafcardattribs));
	m_hLeafMeshVertexFormat = REngine::GetDevice().CreateVertexFormat(leafmeshattribs, _countof(leafmeshattribs));
	m_hBillboardVertexFormat = REngine::GetDevice().CreateVertexFormat(billboardattribs, _countof(billboardattribs));
}


//////////////////////////////////////////////////////////////////////////


// ���������� ������Ʈ ����� �� ���̴� ����� ������Ʈ �ϱ�.
void RTreePassRenderer::SetCommonTreeInstanceConstant( RCameraSceneNode* pCurrentCamera, bool bUseInstancing, bool bCommit)
{
	RPFC_THISFUNC;
	_ASSERT(m_pTreeShader);

	m_scViewProjection.SetMatrix( pCurrentCamera->GetViewProjectionMatrix());
	m_scView.SetMatrix( pCurrentCamera->GetViewMatrix());

	m_scAlphaRef.SetFloat( 0.33f);

	m_scFarDist.SetFloat(pCurrentCamera->GetFarZ());

	if(bCommit)
		m_pTreeShader->CommitChanges();
}


// �� �ν��Ͻ����� ������Ʈ ����� �� ���̴� ����� ������Ʈ �ϱ�.
void RTreePassRenderer::SetTreeInstanceConstant( RCameraSceneNode* pCurrentCamera, const RTreeSceneNode* _pTreeSceneNode, float fAlphaRef)
{
	if(!_pTreeSceneNode)
		return;

	_ASSERT(m_pTreeShader);

	SetCommonTreeInstanceConstant(pCurrentCamera, false, false);

	//m_scWorldViewProjection.SetMatrix( _pTreeSceneNode->GetPositionScaleMatrix() * pCurrentCamera->GetViewProjectionMatrix());
	//m_scWorldView.SetMatrix( _pTreeSceneNode->GetPositionScaleMatrix() * pCurrentCamera->GetViewMatrix());
	m_scWorld.SetMatrix( _pTreeSceneNode->GetPositionScaleMatrix() );
	m_scRotateYValue.SetVector4(_pTreeSceneNode->GetRotateYValue());
	m_scWindMatrixOffset.SetFloat(_pTreeSceneNode->GetWindMatrixOffset());

	m_scAlphaRef.SetFloat( fAlphaRef / 255.0f );

	m_pTreeShader->CommitChanges();
}


// �ν��Ͻ� ���� ����� ������Ʈ �ϱ�
void RTreePassRenderer::SetTreeInstancingConstant( bool bCommit, int nCnt)
{
	RPFC_THISFUNC;
	_ASSERT(m_pTreeShader);

	m_scInstancePosition.SetVector4Array( m_pInstancingPosition, nCnt);
	m_scInstanceData.SetVector4Array( m_pInstancingData, nCnt);

	if(bCommit)
		m_pTreeShader->CommitChanges();
}

//////////////////////////////////////////////////////////////////////////
// Ʈ�� �н� ������ �ڿ� ����
//
//
void RTreePassRenderer::Reset()
{
	m_fTime = 0;
	m_LightDirection	= RVector(1, 1, 0.5);
	m_LightColor		= RVector4(0.5, 0.5, 0.5, 1);
	m_LightAmbient		= RVector4(0.5, 0.5, 0.5, 1);

	m_pSpeedWind->Reset();

	// �̰� ����� Tree �޽��� �׻� �Ȱ��� ����� ���ϴ�.
	CSpeedTreeRT::SetNumWindMatrices(m_pSpeedWind->GetNumWindMatrices());
}

void RTreePassRenderer::Init()
{
	// ���̴� ���� �� m_InstancingBuffer�� ���� ��� �ϹǷ� �ݵ�� ���̴� ���� ������ m_InstancingBuffer�� ���� �Ǿ� ��.
	RTreeResource::m_InstancingBuffer.Create( REngine::GetDevicePtr(), TREE_INSTAICING_SIZE);
	if(!m_pInstancingPosition)
		m_pInstancingPosition = new RVector4[ RTreeResource::m_InstancingBuffer.GetMaxSize()];
	if(!m_pInstancingData)
		m_pInstancingData = new RVector4[ RTreeResource::m_InstancingBuffer.GetMaxSize()];

	// init wind
	CSpeedWind* pSpeedWind = REngine::GetSceneManager().GetResourceMgr<RTreeResourceMgr>()->GetSpeedWind();
	_ASSERT(pSpeedWind);

	m_pSpeedWind = pSpeedWind;
	int nNumWindMatrices = m_pSpeedWind->GetNumWindMatrices();
	int nNumLeafAngleMatrices = m_pSpeedWind->GetNumLeafAngleMatrices();
	_ASSERT( nNumWindMatrices > 0 && nNumWindMatrices <= 6 && "Running Mesh Instancing Suppose NumWindMatrices is 6 or under. See SpeedWind.ini, SpeedWind_default.ini and TREE_INSTAICING_SIZE - by OZ.");
	_ASSERT( nNumLeafAngleMatrices > 0 && nNumLeafAngleMatrices <= 8 && "Running Mesh Instancing Suppose NumLeafAngleMatrices is 8 or under. See SpeedWind.ini, SpeedWind_default.ini and TREE_INSTAICING_SIZE - by OZ.");

	m_vWindMatrices.resize( nNumWindMatrices);
	m_vWindRustleAngles.resize( nNumLeafAngleMatrices);
	m_vWindRockAngles.resize( nNumLeafAngleMatrices);

	for (int i = 0; i < nNumWindMatrices; ++i)
		m_vWindMatrices[i] = m_pSpeedWind->GetWindMatrix((unsigned int) i);

	// Create Shader
	if (!CreateShader(m_nMRTCount))
	{
		_ASSERT(0 && "shader error");
		return;
	}

	m_BillboardRenderer.Create(sizeof(SSpeedTreeBillboard::SVertex), REngine::GetDevicePtr());
	CSpeedTreeRT::SetDropToBillboard(true);

	InitVertexFormats();

	Reset();

	return;
}


//////////////////////////////////////////////////////////////////////////
// Ʈ�� �н� ������ �ڿ��ı�
//
void RTreePassRenderer::Destroy()
{
	RTreeResource::m_InstancingBuffer.Destroy();
	SAFE_DELETE_ARRAY( m_pInstancingPosition);
	SAFE_DELETE_ARRAY( m_pInstancingData);

	m_BillboardRenderer.Destroy();

	DestroyShader();
}

//////////////////////////////////////////////////////////////////////////
// TODO: RTreeResourceMgr �� �ű���
bool RTreePassRenderer::CreateShader(int nMRTCount)
{
	if ( false == REngine::GetDevice().QueryFeature(RQF_PS30) )
		return false;

	vector<string> vShaderDefines;
	char szBuf[200];
	sprintf(szBuf, "%d", m_pSpeedWind->GetNumWindMatrices());

	vShaderDefines.push_back("NUM_WIND_MATRICES");
	vShaderDefines.push_back(szBuf);

	sprintf(szBuf, "%d", m_pSpeedWind->GetNumLeafAngleMatrices());
	vShaderDefines.push_back("MAX_NUM_LEAF_ANGLES");
	vShaderDefines.push_back(szBuf);

	sprintf(szBuf, "%d", RTreeResource::m_InstancingBuffer.GetMaxSize());
	vShaderDefines.push_back("TREE_INSTAICING_SIZE");
	vShaderDefines.push_back(szBuf);

	sprintf(szBuf, "%d", nMRTCount);
	vShaderDefines.push_back("g_iLowRT");
	vShaderDefines.push_back(szBuf);
	
	m_pTreeShader = REngine::GetDevice().CreateShaderFX("SpeedTreeFX.fx", vShaderDefines);
	_ASSERT( m_pTreeShader );

	if (m_pTreeShader)
	{
		// G-stage (Inferred)
		m_mapBranchTechnique[ TREETECH_GSTAGE]			= m_pTreeShader->GetTechnique("BranchGStage");
		m_mapFrondTechnique[ TREETECH_GSTAGE]			= m_pTreeShader->GetTechnique("FrondGStage");
		m_mapLeafCardTechnique[ TREETECH_GSTAGE]		= m_pTreeShader->GetTechnique("LeafCardGStage");
		m_mapLeafMeshTechnique[ TREETECH_GSTAGE]		= m_pTreeShader->GetTechnique("LeafMeshGStage");
		m_mapBillboardTechnique[ TREETECH_GSTAGE]		= m_pTreeShader->GetTechnique("BillboardGStage");
		// M-stage (Inferred)
		m_mapBranchTechnique[ TREETECH_MSTAGE]			= m_pTreeShader->GetTechnique("BranchMStage");
		m_mapFrondTechnique[ TREETECH_MSTAGE]			= m_pTreeShader->GetTechnique("FrondMStage");
		m_mapLeafCardTechnique[ TREETECH_MSTAGE]		= m_pTreeShader->GetTechnique("LeafCardMStage");
		m_mapLeafMeshTechnique[ TREETECH_MSTAGE]		= m_pTreeShader->GetTechnique("LeafMeshMStage");
		m_mapBillboardTechnique[ TREETECH_MSTAGE]		= m_pTreeShader->GetTechnique("BillboardMStage");
		// Deferred
		m_mapBranchTechnique[ TREETECH_DEFERRED]			= m_pTreeShader->GetTechnique("BranchDeferred");
		m_mapFrondTechnique[ TREETECH_DEFERRED]			= m_pTreeShader->GetTechnique("FrondDeferred");
		m_mapLeafCardTechnique[ TREETECH_DEFERRED]		= m_pTreeShader->GetTechnique("LeafCardDeferred");
		m_mapLeafMeshTechnique[ TREETECH_DEFERRED]		= m_pTreeShader->GetTechnique("LeafMeshDeferred");
		m_mapBillboardTechnique[ TREETECH_DEFERRED]		= m_pTreeShader->GetTechnique("BillboardDeferred");
		// Shadowmap ; ������ �� ����
		m_mapBranchTechnique[ TREETECH_SHADOW]			= m_pTreeShader->GetTechnique("BranchShadow");
		m_mapFrondTechnique[ TREETECH_SHADOW]			= m_pTreeShader->GetTechnique("FrondShadow");
		m_mapLeafCardTechnique[ TREETECH_SHADOW]		= m_pTreeShader->GetTechnique("LeafCardShadow");
		m_mapLeafMeshTechnique[ TREETECH_SHADOW]		= m_pTreeShader->GetTechnique("LeafMeshShadow");
		// Projection Shadowmap ; ������ �� ����
		m_mapBranchTechnique[ TREETECH_PROJECTIONSHADOW]		= m_pTreeShader->GetTechnique("BranchProjectionShadow");
		m_mapFrondTechnique[ TREETECH_PROJECTIONSHADOW]		= m_pTreeShader->GetTechnique("FrondProjectionShadow");
		m_mapLeafCardTechnique[ TREETECH_PROJECTIONSHADOW]	= m_pTreeShader->GetTechnique("LeafCardProjectionShadow");
		m_mapLeafMeshTechnique[ TREETECH_PROJECTIONSHADOW]	= m_pTreeShader->GetTechnique("LeafMeshProjectionShadow");

		m_scWorld = m_pTreeShader->GetConstant("g_matWorld");
		m_scViewProjection = m_pTreeShader->GetConstant("g_matViewProjection");
		m_scView = m_pTreeShader->GetConstant("g_matView");
		m_scWindMatrixOffset = m_pTreeShader->GetConstant("g_fWindMatrixOffset");
		m_scWindMatrices = m_pTreeShader->GetConstant("g_amWindMatrices");
		m_scCameraAngle = m_pTreeShader->GetConstant("g_vCameraAngles");
		m_scCameraTrig = m_pTreeShader->GetConstant("g_vCameraAzimuthTrig");
		m_scLeafAngles = m_pTreeShader->GetConstant("g_avLeafAngles");
		m_scLeafAngleScalars = m_pTreeShader->GetConstant("g_vLeafAngleScalars");
		m_scLightAngle = m_pTreeShader->GetConstant("g_vLightAngles");
		m_scInstancePosition = m_pTreeShader->GetConstant("m_avInstancePosition");
		m_scInstanceData = m_pTreeShader->GetConstant("m_avInstanceData");
		m_scFogColor = m_pTreeShader->GetConstant("g_vFogColor");
		m_scFogParam = m_pTreeShader->GetConstant("g_vFogParams");
		m_scRotateYValue = m_pTreeShader->GetConstant("g_vTreeRotationTrig");
		m_scSpecialAzimuth = m_pTreeShader->GetConstant("g_fSpecialAzimuth");
		m_sc360Table = m_pTreeShader->GetConstant("g_v360TexCoords");
		m_scBBNormal = m_pTreeShader->GetConstant("g_amBBNormals");
		m_scBBBinormal = m_pTreeShader->GetConstant("g_amBBBinormals");
		m_scBBTangent = m_pTreeShader->GetConstant("g_amBBTangents");
		m_scAlphaRef = m_pTreeShader->GetConstant("g_fAlphaRef");
		m_scFarDist = m_pTreeShader->GetConstant("g_fFarDist");
		m_scScreenSpaceTransform = m_pTreeShader->GetConstant("g_ScreenspaceTransform");
		m_scShadowValue = m_pTreeShader->GetConstant("g_ShadowValue");
	}

	return true;
}

void RTreePassRenderer::DestroyShader()
{
	if (m_pTreeShader != NULL)
	{
		REngine::GetDevice().DeleteShaderFX(m_pTreeShader);
		m_pTreeShader = NULL;

		m_scWorld.Destroy();
		m_scViewProjection.Destroy();
		m_scView.Destroy();
		m_scWindMatrixOffset.Destroy();
		m_scWindMatrices.Destroy();
		m_scCameraAngle.Destroy();
		m_scCameraTrig.Destroy();
		m_scLeafAngles.Destroy();
		m_scLeafAngleScalars.Destroy();
		m_scInstancePosition.Destroy();
		m_scInstanceData.Destroy();
		m_scFogColor.Destroy();
		m_scFogParam.Destroy();
		m_scLightAngle.Destroy();
		m_scRotateYValue.Destroy();
		m_scSpecialAzimuth.Destroy();
		m_sc360Table.Destroy();
		m_scBBNormal.Destroy();
		m_scBBBinormal.Destroy();
		m_scBBTangent.Destroy();
		m_scAlphaRef.Destroy();
		m_scFarDist.Destroy();
		m_scScreenSpaceTransform.Destroy();
		m_scShadowValue.Destroy();
	}
}


//////////////////////////////////////////////////////////////////////////
// RTreePassRenderer::Update()
// - param : timegap���� �����Ӱ��� �ð��� ����� �մϴ�.
//
void RTreePassRenderer::Update(float timegap)
{
	RPFC_THISFUNC;

	m_fTime += timegap;

	CSpeedTreeRT::SetTime(m_fTime);
	m_pSpeedWind->Advance(m_fTime);

	int nNumMatrices = int(m_pSpeedWind->GetNumWindMatrices( ));
	assert(int(m_vWindMatrices.size( )) == nNumMatrices);
	for (int i = 0; i < nNumMatrices; ++i)
		m_vWindMatrices[i] = m_pSpeedWind->GetWindMatrix((unsigned int) i);

	m_pSpeedWind->GetRockAngles(&m_vWindRockAngles[0]);
	m_pSpeedWind->GetRustleAngles(&m_vWindRustleAngles[0]);
	
}

void RTreePassRenderer::ChangeRTCount(int nCount)
{
	// RT ���Ǵϼ��� �ٲ�Ƿ� �� ������.
	DestroyShader();
	CreateShader(nCount);
}

void RTreePassRenderer::GetLightParams(RSceneManager* pManager)
{
	RDirectionalLightSceneNode* Light = pManager->GetDefaultLight();

	if (Light != NULL)
	{
		m_LightAmbient = Light->GetAmbient() * Light->GetPower();
		m_LightDirection = -Light->GetDirection();
		m_LightColor = Light->GetDiffuse() * Light->GetPower();
	}
}

void RTreePassRenderer::UploadShaderConstants( RCameraSceneNode* pCurrentCamera, RSceneManager* pManager, TREE_TECH_TYPE eType)
{
	float fCameraAzimuth, fCameraPitch;
	const int nNumLeafAngles = m_pSpeedWind->GetNumLeafAngleMatrices( );
	vector<RVector4> m_vLeafAngleUploadTable;
	m_vLeafAngleUploadTable.resize(nNumLeafAngles);

	_ASSERT(int(m_vWindRockAngles.size( )) == nNumLeafAngles);
	_ASSERT(int(m_vWindRustleAngles.size( )) == nNumLeafAngles);

	m_scViewProjection.SetMatrix(pCurrentCamera->GetViewProjectionMatrix());		// View-Projection ��� ����

	for (unsigned int i = 0; i < m_pSpeedWind->GetNumWindMatrices( ); ++i)
		m_scWindMatrices.SetMatrixIndex(m_vWindMatrices[i], i);										// �ٶ� ��ȯ ��� ����

	CSpeedTreeRT::GetCameraAngles(fCameraAzimuth, fCameraPitch);
	// adjust azimuth from range [-180,180] to [0,360]
	fCameraAzimuth = DegreeToRadian(fCameraAzimuth);
	if (fCameraAzimuth < 0.0f)
		fCameraAzimuth += MMath::TWOPI;

	m_scCameraAngle.SetVector2(fCameraAzimuth, DegreeToRadian(fCameraPitch));						// ���� ī�޶� ���� ����

	// upload leaf angle table
	for (int i = 0; i < nNumLeafAngles; ++i)
	{
		m_vLeafAngleUploadTable[i].x = DegreeToRadian(m_vWindRockAngles[i]);
		m_vLeafAngleUploadTable[i].y = DegreeToRadian(m_vWindRustleAngles[i]);
		m_vLeafAngleUploadTable[i].z = m_vLeafAngleUploadTable[i].w = 0;
	}
	m_scLeafAngles.SetVector4Array(&m_vLeafAngleUploadTable[0], nNumLeafAngles);				// �� �������� ���̺� ����
	
	m_LightDirection.Normalize();

	// ������ �׸��� ĳ���ÿ� ���̴� ��� ����
	RVector lightDir = -m_LightDirection;
	float fLightAzimuth = atan2f( lightDir.y, lightDir.x );
	if( fLightAzimuth < 0.0f )
	{
		fLightAzimuth += MMath::TWOPI;
	}
	float fLightPitch = lightDir.AngleBetween( RVector( lightDir.x, lightDir.y, 0.f ).Normalize() );
	m_scLightAngle.SetVector2( fLightAzimuth, fLightPitch );

	float afCameraPos[3], afCameraDir[3];
	CSpeedTreeRT::GetCamera(afCameraPos, afCameraDir);

	float fBillboardAzimuth = atan2f(-afCameraDir[0], afCameraDir[1]);
	if (fBillboardAzimuth < 0.0f)
		fBillboardAzimuth += MMath::TWOPI;
	else if (fBillboardAzimuth >= MMath::TWOPI)
		fBillboardAzimuth = fmod(fBillboardAzimuth, MMath::TWOPI);

	// upload angles to shader
	m_scCameraTrig.SetVector4(sinf(-fCameraAzimuth), cosf(-fCameraAzimuth), -sinf(-fCameraAzimuth), 0.0f);

	m_scSpecialAzimuth.SetFloat(fBillboardAzimuth);

	CSpeedTreeRT::SGeometry::S360Billboard sBillboard;
	CSpeedTreeRT::UpdateBillboardLighting(sBillboard);

	// �� �̰� ��������� �ϴ��� �� �𸣰����� ��·�� ������ (��ü ��? Left-handed/Right-handed����?)
	// �� �̰� xy �� ��������ϴ��� ���� �� �𸣰���. �밡�ٷ� ��������ϴ� ��.�� dubble
	// normals
	float afFourNormals[4][4] =
	{
		{ -sBillboard.m_pNormals[0], -sBillboard.m_pNormals[1], -sBillboard.m_pNormals[2], 0.0f },
		{ -sBillboard.m_pNormals[3], -sBillboard.m_pNormals[4], -sBillboard.m_pNormals[5], 0.0f },
		{ -sBillboard.m_pNormals[6], -sBillboard.m_pNormals[7], -sBillboard.m_pNormals[8], 0.0f },
		{ -sBillboard.m_pNormals[9], -sBillboard.m_pNormals[10], -sBillboard.m_pNormals[11], 0.0f },
	};
	m_scBBNormal.SetMatrix((const float*) afFourNormals);

	// binormals
	float afFourBinormals[4][4] =																	
	{
		{ -sBillboard.m_pBinormals[0], -sBillboard.m_pBinormals[1], -sBillboard.m_pBinormals[2], 0.0f },
		{ -sBillboard.m_pBinormals[3], -sBillboard.m_pBinormals[4], -sBillboard.m_pBinormals[5], 0.0f },
		{ -sBillboard.m_pBinormals[6], -sBillboard.m_pBinormals[7], -sBillboard.m_pBinormals[8], 0.0f },
		{ -sBillboard.m_pBinormals[9], -sBillboard.m_pBinormals[10], -sBillboard.m_pBinormals[11], 0.0f },
	};
	m_scBBBinormal.SetMatrix((const float*) afFourBinormals);

	// tangents
	float afFourTangents[4][4] =
	{
		{ -sBillboard.m_pTangents[0], -sBillboard.m_pTangents[1], -sBillboard.m_pTangents[2], 0.0f },
		{ -sBillboard.m_pTangents[3], -sBillboard.m_pTangents[4], -sBillboard.m_pTangents[5], 0.0f },
		{ -sBillboard.m_pTangents[6], -sBillboard.m_pTangents[7], -sBillboard.m_pTangents[8], 0.0f },
		{ -sBillboard.m_pTangents[9], -sBillboard.m_pTangents[10], -sBillboard.m_pTangents[11], 0.0f },
	};
	m_scBBTangent.SetMatrix((const float*) afFourTangents);

	// ���� �Ķ����
	if (pCurrentCamera->GetFogEnable())
	{
		float fNear = pCurrentCamera->GetFogNear();
		float fFar = pCurrentCamera->GetFogFar();
		float fDist = fFar - fNear;
		// divide by zero�� ���� �ʰ� ������ �Ѱ��ְ� ���̴������� ���ϰ�.
		fDist = ( fDist == 0.0f ? 0 : 1.0f/fDist );
		m_scFogParam.SetVector3(fNear, fFar, fDist);
	}
	else
	{
		m_scFogParam.SetVector3( 0, 0, 0);	
	}
	// ���� �÷�
	DWORD dwFogColor = pCurrentCamera->GetFogColor();
	RVector vecFogColor;
	DWColorToFloat3( &vecFogColor, dwFogColor );
	m_scFogColor.SetVector3( vecFogColor );

	if( eType == TREETECH_MSTAGE)
	{
		// ������ ��ũ�� ���� ��ǥ�� ��ȯ���ִ� ���.
		UINT nScreenWidth = REngine::GetDevice().GetScreenWidth();
		UINT nScreenHeight = REngine::GetDevice().GetScreenHeight();
		RMatrix matToScreen;
		matToScreen.SetScreenSpaceMatrix(nScreenWidth, nScreenHeight);
		RMatrix matView = pCurrentCamera->GetViewMatrix();
		RMatrix matProjection = pCurrentCamera->GetProjectionMatrix();
		RMatrix matScreenSpaceTransform = matView * matProjection * matToScreen;
		m_scScreenSpaceTransform.SetMatrix(matScreenSpaceTransform);
	}

	m_pTreeShader->GetConstant("g_vLightDir").SetVector3(m_LightDirection);

	if(eType == TREETECH_PROJECTIONSHADOW)
	{
		float fShadowValue = 0;

		if(REngine::GetSceneManager().GetDefaultLight())
			fShadowValue = REngine::GetSceneManager().GetDefaultLight()->GetShadowLuminosity();
		else
			fShadowValue = DEFAULT_SHADOW_LUMINOSITY;

		m_scShadowValue.SetFloat(fShadowValue);
	}
}

void RTreePassRenderer::SetTreeInstancingData( int nIdx, RTreeSceneNode* pTreeSceneNode)
{
	if(!m_pInstancingData || nIdx >= RTreeResource::m_InstancingBuffer.GetMaxSize())
		return;

	// ��ġ
	RVector3 vPosition = pTreeSceneNode->GetWorldPosition();
	// �����ϸ�
	float fScale = pTreeSceneNode->GetScale();
	// �� ȸ�� Sin
	float fSinTheta = pTreeSceneNode->GetSinTheta();
	// �� ȸ�� Cos
	float fCosTheta = pTreeSceneNode->GetCosTheta();
	// wind offset
	float fWindOffset = pTreeSceneNode->GetWindMatrixOffset();
	// LOD : ���� �׽�Ʈ ���� �ٷ� ����.
	float fAlphaRef = 0.33f;
	/// �������� ���̵� ������ �̿��Ͽ� leaf�� ���̵� ó��.
	if( pTreeSceneNode->m_SpeedTreeLodInfo.m_fBillboardFadeOut > 0)
	{
		fAlphaRef += pTreeSceneNode->m_SpeedTreeLodInfo.m_fBillboardFadeOut * (1.0f-0.33f);
	}
	
	m_pInstancingPosition[nIdx] = RVector4( vPosition, fAlphaRef);
	m_pInstancingData[nIdx] = RVector4( fScale, fSinTheta, fCosTheta, fWindOffset);

}


//////////////////////////////////////////////////////////////////////////
//	������� Render()�Լ��� ���������� �Լ���,
//
void RTreePassRenderer::RenderBranch(  RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, TREE_TECH_TYPE eType)
{
	RPFC_THISFUNC;

	RTechnique* technique = NULL;
	TECHNIQUE_MAP::iterator itr = m_mapBranchTechnique.find( eType);
	if( itr != m_mapBranchTechnique.end() )
	{
		technique = &(itr->second);
	}
	
	if(technique)
	{
		REngine::GetDevice().SetVertexFormat(m_hBranchVertexFormat);

		technique->Begin(NULL, TECH_PRESERVE_NOT);
		technique->BeginPass();

		MAP_TREECHAIN* pTreeChain = pCurrentCamera->GetViewFrustumCullingMgr()->GetTreeChain();
		MAP_TREECHAIN::iterator TreeChainIter;
		for( TreeChainIter = pTreeChain->begin() ; TreeChainIter != pTreeChain->end() ; ++TreeChainIter )
		{
			RTreeResource* pTreeResource = TreeChainIter->first;

			if( pTreeResource->SetupForRenderBranch() )
			{
				_ASSERT(m_pTreeShader);

				SetBranchTexture( pTreeResource->m_arrayTreeTexture[ RTreeResource::SPT_BRAHCH ]);
				if( eType == TREETECH_MSTAGE || eType == TREETECH_DEFERRED)
					SetBranchDetailTexture( pTreeResource->m_arrayTreeTexture[ RTreeResource::SPT_BRANCH_DETAIL ]);

				SCENE_SET* pSet =  TreeChainIter->second;
				SCENE_SET::iterator SetIter;

				// �ν��Ͻ� ���
				if(m_bUseInstancing)
				{
					int nCnt = 0;
					SetCommonTreeInstanceConstant( pCurrentCamera, true, false);		// commit before render. not here.
					//SCENE_SET::reverse_iterator SetRIter = pSet->rbegin();
					RTreeSceneNode* pLastTreeSceneNode = (RTreeSceneNode*)(*(pSet->rbegin()));	//(*SetRIter);
					for(SetIter = pSet->begin() ; SetIter != pSet->end() ; ++SetIter )
					{
						RTreeSceneNode* pTreeSceneNode = (RTreeSceneNode*)(*SetIter);
						if( pTreeSceneNode->GetVisible() && pTreeSceneNode->GetUsable() && !pTreeSceneNode->GetOcclued() && pTreeSceneNode->IsViewBranchFrond())
						{
							// ������ ����
							SetTreeInstancingData( nCnt, pTreeSceneNode);
							// ������ ����.
							++nCnt;
						}
						if( nCnt > 0)
						{
							// �ν��Ͻ� ���� ���� ������ �����߰ų� ü���� ���̸� ��ο�
							if( nCnt >= pTreeResource->m_InstancingBuffer.GetMaxSize()
								|| pTreeSceneNode == pLastTreeSceneNode)
							{
								// �����Ͱ� ��� �迭�� ���̴� ����� ���� �� commit
								SetTreeInstancingConstant( true, min( nCnt, RTreeResource::m_InstancingBuffer.GetMaxSize()));
								// ����
								pTreeResource->SetupForRenderInstancing( &pTreeResource->m_InstancingBuffer, nCnt);
								RenderBranchInstancing( pTreeResource, &pTreeResource->m_InstancingBuffer, pCurrentCamera, nCnt);
								nCnt = 0;
							}
						}
					}
				}
				// �ν��Ͻ� �̻��
				else
				{
					for(SetIter = pSet->begin() ; SetIter != pSet->end() ; ++SetIter )
					{
						RTreeSceneNode* pTreeSceneNode = (RTreeSceneNode*)(*SetIter);
						if( !pTreeSceneNode->GetVisible() || !pTreeSceneNode->GetUsable() || pTreeSceneNode->GetOcclued() )	continue;
						RenderBranchInstance(pTreeResource, pTreeSceneNode, pCurrentCamera);
					}
				}
			}
		}

		technique->EndPass();
		technique->End();
	}

}


void RTreePassRenderer::RenderFrond(  RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, TREE_TECH_TYPE eType)
{
	RPFC_THISFUNC;

	RTechnique* technique = NULL;
	TECHNIQUE_MAP::iterator itr = m_mapFrondTechnique.find( eType);
	if( itr != m_mapFrondTechnique.end() )
	{
		technique = &(itr->second);
	}

	if( technique)
	{
		REngine::GetDevice().SetCullMode(RCULL_NONE);
		REngine::GetDevice().SetVertexFormat(m_hFrondVertexFormat);
		technique->Begin(NULL, TECH_PRESERVE_NOT);
		technique->BeginPass();

		MAP_TREECHAIN* pTreeChain = pCurrentCamera->GetViewFrustumCullingMgr()->GetTreeChain();
		MAP_TREECHAIN::iterator TreeChainIter;
		for( TreeChainIter = pTreeChain->begin() ; TreeChainIter != pTreeChain->end() ; ++TreeChainIter )
		{
			RTreeResource* pTreeResource = TreeChainIter->first;

			if( pTreeResource->SetupForRenderFrond() )
			{
				_ASSERT(m_pTreeShader);

				SetCompositeTexture( pTreeResource->m_arrayTreeTexture[ RTreeResource::SPT_COMPOSITE ] );

				SCENE_SET* pSet =  TreeChainIter->second;
				SCENE_SET::iterator SetIter;

				// �ν��Ͻ� ���
				if(m_bUseInstancing)
				{
					int nCnt = 0;
					SetCommonTreeInstanceConstant( pCurrentCamera, true, false);
					//SCENE_SET::reverse_iterator SetRIter = pSet->rbegin();
					RTreeSceneNode* pLastTreeSceneNode = (RTreeSceneNode*)(*(pSet->rbegin()));	//(*SetRIter);
					for(SetIter = pSet->begin() ; SetIter != pSet->end() ; ++SetIter )
					{
						RTreeSceneNode* pTreeSceneNode = (RTreeSceneNode*)(*SetIter);
						// �ν��Ͻ����� �׸� �ô� LOD ���� 0���θ� �׸���. Frond�� LOD�� �̰����� üũ�� ����.
						if( pTreeSceneNode->GetVisible() && pTreeSceneNode->GetUsable() && !pTreeSceneNode->GetOcclued() && pTreeSceneNode->IsViewBranchFrond() && pTreeSceneNode->m_SpeedTreeLodInfo.m_nFrondActiveLod >= 0)
						{
							// ������ ����
							SetTreeInstancingData( nCnt, pTreeSceneNode);
							// ������ ����.
							++nCnt;
						}
						if( nCnt > 0)
						{
							// �ν��Ͻ� ���� ���� ������ �����߰ų� ü���� ���̸� ��ο�
							if( nCnt >= pTreeResource->m_InstancingBuffer.GetMaxSize()
								|| pTreeSceneNode == pLastTreeSceneNode)
							{
								// �����Ͱ� ��� �迭�� ���̴� ����� ���� �� commit
								SetTreeInstancingConstant( true, min( nCnt, RTreeResource::m_InstancingBuffer.GetMaxSize()));
								// ����
								pTreeResource->SetupForRenderInstancing( &pTreeResource->m_InstancingBuffer, nCnt);
								RenderFrondInstancing( pTreeResource, &pTreeResource->m_InstancingBuffer, pCurrentCamera, nCnt);
								nCnt = 0;
							}
						}
					}
				}
				// �ν��Ͻ� �̻��
				else
				{
					for(SetIter = pSet->begin() ; SetIter != pSet->end() ; ++SetIter )
					{
						RTreeSceneNode* pTreeSceneNode = (RTreeSceneNode*)(*SetIter);
						if( !pTreeSceneNode->GetVisible() || !pTreeSceneNode->GetUsable() || pTreeSceneNode->GetOcclued() )	continue;
						RenderFrondInstance(pTreeResource, pTreeSceneNode, pCurrentCamera);
					}
				}
			}
		}

		technique->EndPass();
		technique->End();


		//Restore Render State
		REngine::GetDevice().SetCullMode(RCULL_CW);
	}
}

void RTreePassRenderer::RenderLeafCard( RCameraSceneNode* pCurrentCamera,  SCENE_SET* pRenderingList, TREE_TECH_TYPE eType)
{
	RPFC_THISFUNC;

	RTechnique* technique = NULL;
	TECHNIQUE_MAP::iterator itr = m_mapLeafCardTechnique.find( eType);
	if( itr != m_mapLeafCardTechnique.end() )
	{
		technique = &(itr->second);
	}

	if(technique)
	{
		REngine::GetDevice().SetCullMode(RCULL_NONE);
		REngine::GetDevice().SetVertexFormat(m_hLeafCardVertexFormat);
		technique->Begin(NULL, TECH_PRESERVE_NOT);
		technique->BeginPass();

		MAP_TREECHAIN* pTreeChain = pCurrentCamera->GetViewFrustumCullingMgr()->GetTreeChain();
		MAP_TREECHAIN::iterator TreeChainIter;
		for( TreeChainIter = pTreeChain->begin() ; TreeChainIter != pTreeChain->end() ; ++TreeChainIter )
		{
			RTreeResource* pTreeResource = TreeChainIter->first;

			if( pTreeResource->SetupForRenderLeafCard() )
			{
				_ASSERT(m_pTreeShader);
				// commit�� SetTreeInstanceConstant()���� ���� ��.
				m_scLeafAngleScalars.SetVector2(pTreeResource->m_fLeafRockScalar, pTreeResource->m_fLeafRustleScalar);
				SetCompositeTexture( pTreeResource->m_arrayTreeTexture[ RTreeResource::SPT_COMPOSITE ] );

				SCENE_SET* pSet =  TreeChainIter->second;
				SCENE_SET::iterator SetIter;

				// �ν��Ͻ� ���
				if(m_bUseInstancing)
				{
					int nCnt = 0;
					SetCommonTreeInstanceConstant( pCurrentCamera, true, false);
					//SCENE_SET::reverse_iterator SetRIter = pSet->rbegin();
					RTreeSceneNode* pLastTreeSceneNode = (RTreeSceneNode*)(*(pSet->rbegin()));	//(*SetRIter);
					for(SetIter = pSet->begin() ; SetIter != pSet->end() ; ++SetIter )
					{
						RTreeSceneNode* pTreeSceneNode = (RTreeSceneNode*)(*SetIter);
						if( pTreeSceneNode->GetVisible() && pTreeSceneNode->GetUsable() && !pTreeSceneNode->GetOcclued() && pTreeSceneNode->IsViewLeaves())
						{
							// ������ ����
							SetTreeInstancingData( nCnt, pTreeSceneNode);
							// ������ ����.
							nCnt++;
						}
						if( nCnt > 0)
						{
							// �ν��Ͻ� ���� ���� ������ �����߰ų� ü���� ���̸� ��ο�
							if( nCnt >= pTreeResource->m_InstancingBuffer.GetMaxSize()
								|| pTreeSceneNode == pLastTreeSceneNode)
							{
								// �����Ͱ� ��� �迭�� ���̴� ����� ���� �� commit
								SetTreeInstancingConstant( true, min( nCnt, RTreeResource::m_InstancingBuffer.GetMaxSize()));
								// ����
								pTreeResource->SetupForRenderInstancing( &pTreeResource->m_InstancingBuffer, nCnt);
								RenderLeafCardInstancing( pTreeResource, &pTreeResource->m_InstancingBuffer, pCurrentCamera, nCnt);
								nCnt = 0;
							}
						}
					}
				}
				// �ν��Ͻ� �̻��
				else
				{
					for(SetIter = pSet->begin() ; SetIter != pSet->end() ; ++SetIter )
					{
						RTreeSceneNode* pTreeSceneNode = (RTreeSceneNode*)(*SetIter);
						if( !pTreeSceneNode->GetVisible() || !pTreeSceneNode->GetUsable() || pTreeSceneNode->GetOcclued() )	continue;
						RenderLeafCardInstance(pTreeResource, pTreeSceneNode, pCurrentCamera);
					}
				}
			}
		}

		technique->EndPass();
		technique->End();


		//Restore Render State
		REngine::GetDevice().SetCullMode(RCULL_CW);
	}
}

void RTreePassRenderer::RenderLeafMesh(  RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, TREE_TECH_TYPE eType)
{
	RPFC_THISFUNC;

	RTechnique* technique = NULL;
	TECHNIQUE_MAP::iterator itr = m_mapLeafMeshTechnique.find( eType);
	if( itr != m_mapLeafMeshTechnique.end() )
	{
		technique = &(itr->second);
	}

	if(technique)
	{
		//REngine::GetDevice().SetTexture(1, R_NONE);
		REngine::GetDevice().SetCullMode(RCULL_NONE);
		REngine::GetDevice().SetVertexFormat(m_hLeafMeshVertexFormat);
		technique->Begin(NULL, TECH_PRESERVE_NOT);
		technique->BeginPass();

		MAP_TREECHAIN* pTreeChain = pCurrentCamera->GetViewFrustumCullingMgr()->GetTreeChain();
		MAP_TREECHAIN::iterator TreeChainIter;
		for( TreeChainIter = pTreeChain->begin() ; TreeChainIter != pTreeChain->end() ; ++TreeChainIter )
		{
			RTreeResource* pTreeResource = TreeChainIter->first;

			if( pTreeResource->SetupForRenderLeafMesh() )
			{
				_ASSERT(m_pTreeShader);
				// commit�� SetTreeInstanceConstant()���� ���� ��.
				m_scLeafAngleScalars.SetVector2(pTreeResource->m_fLeafRockScalar, pTreeResource->m_fLeafRustleScalar);
				SetCompositeTexture( pTreeResource->m_arrayTreeTexture[ RTreeResource::SPT_COMPOSITE ] );

				SCENE_SET* pSet =  TreeChainIter->second;
				SCENE_SET::iterator SetIter;

				// �ν��Ͻ� ���
				if(m_bUseInstancing)
				{
					int nCnt = 0;
					SetCommonTreeInstanceConstant( pCurrentCamera, true, false);
					//SCENE_SET::reverse_iterator SetRIter = pSet->rbegin();
					RTreeSceneNode* pLastTreeSceneNode = (RTreeSceneNode*)(*(pSet->rbegin()));	//(*SetRIter);
					for(SetIter = pSet->begin() ; SetIter != pSet->end() ; SetIter++ )
					{
						RTreeSceneNode* pTreeSceneNode = (RTreeSceneNode*)(*SetIter);
						if( pTreeSceneNode->GetVisible() && pTreeSceneNode->GetUsable() && !pTreeSceneNode->GetOcclued() && pTreeSceneNode->IsViewLeaves())
						{
							// ������ ����
							SetTreeInstancingData( nCnt, pTreeSceneNode);
							// ������ ����.
							nCnt++;
						}

						if( nCnt > 0)
						{
							// �ν��Ͻ� ���� ���� ������ �����߰ų� ü���� ���̸� ��ο�
							if( nCnt >= pTreeResource->m_InstancingBuffer.GetMaxSize()
								|| pTreeSceneNode == pLastTreeSceneNode)
							{
								// �����Ͱ� ��� �迭�� ���̴� ����� ���� �� commit
								SetTreeInstancingConstant( true, min( nCnt, RTreeResource::m_InstancingBuffer.GetMaxSize()));
								// ����
								pTreeResource->SetupForRenderInstancing( &pTreeResource->m_InstancingBuffer, nCnt);
								RenderLeafMeshInstancing( pTreeResource, &pTreeResource->m_InstancingBuffer, pCurrentCamera, nCnt);
								nCnt = 0;
							}
						}
					}
				}
				// �ν��Ͻ� �̻��
				else
				{
					for(SetIter = pSet->begin() ; SetIter != pSet->end() ; ++SetIter )
					{
						RTreeSceneNode* pTreeSceneNode = (RTreeSceneNode*)(*SetIter);
						if( !pTreeSceneNode->GetVisible() || !pTreeSceneNode->GetUsable() || pTreeSceneNode->GetOcclued() )	continue;
						RenderLeafMeshInstance(pTreeResource, pTreeSceneNode, pCurrentCamera);
					}
				}
			}
		}

		technique->EndPass();
		technique->End();


		//Restore Render State
		REngine::GetDevice().SetCullMode(RCULL_CW);
	}
}


void RTreePassRenderer::RenderBillboardedTrees( RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, TREE_TECH_TYPE eType)
{
	RPFC_THISFUNC;

	RTechnique* technique = NULL;
	TECHNIQUE_MAP::iterator itr = m_mapBillboardTechnique.find( eType);
	if( itr != m_mapBillboardTechnique.end() )
	{
		technique = &(itr->second);
	}

	_ASSERT( m_pTreeShader );
	_ASSERT( technique );

	MAP_TREECHAIN* pTreeChain = pCurrentCamera->GetViewFrustumCullingMgr()->GetTreeChain();
	MAP_TREECHAIN::iterator TreeChainIter;
	for( TreeChainIter = pTreeChain->begin() ; TreeChainIter != pTreeChain->end() ; ++TreeChainIter )
	{
		RTreeResource* pTreeResource = TreeChainIter->first;
		if( NULL == pTreeResource ) continue;

		// update shader constanct
		if( 0 == pTreeResource->m_vTexCoord360.size() ) continue;	//���� ���ҽ��ε��� �ȳ������ÿ���
		m_sc360Table.SetVector4Array((RVector4*)&(pTreeResource->m_vTexCoord360[0]), pTreeResource->m_vTexCoord360.size() / 4);
		//m_scBillboardTexture.SetTexture( pTreeResource->m_arrayTreeTexture[ RTreeResource::SPT_BILLBOARD ], REngine::GetDevicePtr() );
		//m_scBillboardNormalMap.SetTexture( pTreeResource->m_arrayTreeTexture[ RTreeResource::SPT_BILLBOARD_NORMAL], REngine::GetDevicePtr() );
		SetBillboardTexture( pTreeResource->m_arrayTreeTexture[ RTreeResource::SPT_BILLBOARD ]);
		SetBillboardNormalTexture( pTreeResource->m_arrayTreeTexture[ RTreeResource::SPT_BILLBOARD_NORMAL ]);
		m_pTreeShader->CommitChanges();

		// build batch billboard vertices
		pTreeResource->SetupForRenderBillboard();
		SCENE_SET* pSet =  TreeChainIter->second;
		SCENE_SET::iterator SetIter;
		for(SetIter = pSet->begin() ; SetIter != pSet->end() ; ++SetIter )
		{
			RTreeSceneNode* pTreeSceneNode = (RTreeSceneNode*)(*SetIter);
			if( !pTreeSceneNode->GetVisible() || !pTreeSceneNode->GetUsable() || pTreeSceneNode->GetOcclued() )	continue;
			if ( pTreeSceneNode->IsBillboardRenderable() )
			{
				SSpeedTreeBillboard::SVertex* pVertex = pTreeResource->RenderBillboard(
					(float*)&pTreeSceneNode->GetPosition(), pTreeSceneNode->GetRotate(),
					pTreeSceneNode->GetBillboardFade(), pTreeSceneNode->GetScale());

				m_BillboardRenderer.AddVertices( pVertex, 6 );
			}
		}

		// render billboard ( dp call )

		// 360 degree billboard
		int ret = m_BillboardRenderer.Render( *technique);

		m_nPolyCount += ret;
		REngine::GetResourceProfiler().AddPolygon(PPT_TREEBILLBOARD,ret);
		REngine::GetResourceProfiler().AddDrawCall( PPT_TREEBILLBOARD, 1);
	}
}



void RTreePassRenderer::RenderBillboard( RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, TREE_TECH_TYPE eType)
{
	RPFC_THISFUNC;
	REngine::GetDevice().SetCullMode(RCULL_NONE);
	m_scAlphaRef.SetFloat( 0.33f);
	REngine::GetDevice().SetVertexFormat(m_hBillboardVertexFormat);

	RenderBillboardedTrees( pCurrentCamera, pRenderingList, eType);


	//Restore Render State
	REngine::GetDevice().SetCullMode(RCULL_CW);
}

void RTreePassRenderer::RenderCollisionObject( RCameraSceneNode* pCurrentCamera,SCENE_SET* pRenderingList )
{
	RPFC_THISFUNC;
	MAP_TREECHAIN* pTreeChain = pCurrentCamera->GetViewFrustumCullingMgr()->GetTreeChain();
	MAP_TREECHAIN::iterator TreeChainIter;
	for( TreeChainIter = pTreeChain->begin() ; TreeChainIter != pTreeChain->end() ; ++TreeChainIter )
	{
		RTreeResource* pTreeResource = TreeChainIter->first;
		if( NULL == pTreeResource ) continue;

		SCENE_SET* pSet =  TreeChainIter->second;
		SCENE_SET::iterator SetIter;
		for(SetIter = pSet->begin() ; SetIter != pSet->end() ; ++SetIter )
		{
			RTreeSceneNode* pTreeSceneNode = (RTreeSceneNode*)(*SetIter);
			pTreeSceneNode->RenderCollisionObject();
		}
	}
}

void RTreePassRenderer::ClearChain()
{
	// clear rendering
	for( std::vector< RTreeResource* >::iterator itr = m_vecTreeResourceToRender.begin(); itr != m_vecTreeResourceToRender.end(); ++itr )
		(*itr)->m_pFirstRenderChain = NULL;

	m_vecTreeResourceToRender.clear();
}

void RTreePassRenderer::SetRenderState()
{
	RDevice *pDevice = REngine::GetDevicePtr();
	
	pDevice->SetTextureFilter( AS_TREE_COMPOSITE, RTF_BILINEAR );
	//pDevice->SetTextureMaxAnisotropy( AS_TREE_COMPOSITE, 1);
	pDevice->SetTextureFilter( AS_TREE_BILLBOARD_TEXTURE, RTF_BILINEAR );
	//pDevice->SetTextureMaxAnisotropy( AS_TREE_BILLBOARD_TEXTURE, 1);
	pDevice->SetTextureFilter( AS_TREE_BILLBOARD_NORMAL, RTF_BILINEAR );
	//pDevice->SetTextureMaxAnisotropy( AS_TREE_BILLBOARD_NORMAL, 1);
	pDevice->SetTextureFilter( AS_TREE_BRANCH_DETAIL, RTF_BILINEAR );
	//pDevice->SetTextureMaxAnisotropy( AS_TREE_BRANCH_DETAIL, 1 );
	/// �귣ġ�� �ִ� ���漺 ���͸� �����Ѵ�. �������� �̸� ������ ����Ƽ�� �ƴϴ�.
	pDevice->SetTextureFilter( AS_TREE_BRANCH_TEXTURE, REngine::GetConfig().GetTextureFilter() );
	pDevice->SetTextureMaxAnisotropy( AS_TREE_BRANCH_TEXTURE, REngine::GetConfig().GetTextureMaxAnisotropy() );

	pDevice->SetTextureAddressWrap( AS_TREE_COMPOSITE);
	pDevice->SetTextureAddressWrap( AS_TREE_BRANCH_TEXTURE);
	pDevice->SetTextureAddressWrap( AS_TREE_BRANCH_DETAIL);
	pDevice->SetTextureAddressWrap( AS_TREE_BILLBOARD_TEXTURE);
	pDevice->SetTextureAddressWrap( AS_TREE_BILLBOARD_NORMAL);
	
	pDevice->SetTextureMipmapLodBias( AS_TREE_COMPOSITE, REngine::GetConfig().GetSpeedTreeCompositeTextureMipmapLodBias() );
}

void RTreePassRenderer::RestoreRenderState()
{
	RPFC_THISFUNC;
	RDevice *pDevice = REngine::GetDevicePtr();

	pDevice->SetTextureMipmapLodBias( AS_TREE_COMPOSITE, REngine::GetConfig().m_fGlobalTextureMipmapLodBias );
}

void RTreePassRenderer::RenderDepthOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pCulledList, RShadowStrategy* pShadowStrategy )
{
	RPFC_THISFUNC;

	_ASSERT(pCurrentCamera);
	if (pCurrentCamera == NULL) return;

	bool bProjectionShadow = pShadowStrategy->GetShadowDesc().eMethod == SHADOW_PROJECTION;
	RenderForMakingLightMap( pCurrentCamera,pCulledList, pCurrentCamera->GetViewProjectionMatrix(), bProjectionShadow);
}

void RTreePassRenderer::RenderOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, TREE_TECH_TYPE eType)
{
	RPFC_THISFUNC;

	MAP_TREECHAIN* pTreeChain = pCurrentCamera->GetViewFrustumCullingMgr()->GetTreeChain();
	if( pTreeChain->empty())
		return;

	if (m_bDoNotRender)
		return;

	RCameraSceneNode *cam = pCurrentCamera;
	if (cam == NULL) return;

	RVector camPos = cam->GetPosition();
	RVector camDir = cam->GetDirection();
	CSpeedTreeRT::SetCamera((float*)&camPos, (float*)&camDir);


	int nMRTCount = REngine::GetSceneManager().GetRenderingStrategy()->GetMRTCount();
	if(m_nMRTCount != nMRTCount)
	{
		ChangeRTCount(nMRTCount);

		m_nMRTCount = nMRTCount;
	}

	// ligth update
	GetLightParams(REngine::GetSceneManagerPtr());
	UploadShaderConstants( pCurrentCamera, REngine::GetSceneManagerPtr(), eType);					// ���̴� ��� ���ε�

	SetRenderState();

	// �ν��Ͻ� ��� ������ ��� ���� ������ ���۸� ������ �س��´�. �׷��� ������ DX debug vereion�� �� ������ ��´�.
	// Direct3D9: (ERROR) :Stream 1 size is too small
	RTreeResource::SetupForRenderInstancing( &RTreeResource::m_InstancingBuffer, 1);
	// HW �ν��Ͻ� ��� ��ҵ� ����
	RenderBranch( pCurrentCamera, pRenderingList, eType);
 	RenderFrond( pCurrentCamera, pRenderingList, eType);
 	RenderLeafCard( pCurrentCamera, pRenderingList, eType);
 	RenderLeafMesh( pCurrentCamera, pRenderingList, eType);
	// �������� HW �ν��ʹ� ��� ���ϹǷ� ������ ���� ���� ���� ���ƾ� ��.
	RTreeResource::RestoreForRenderInstancing();
	// ������� SW�ν��Ͻ�.
 	RenderBillboard( pCurrentCamera, pRenderingList, eType);


	// ��Ʈ���� �н����� ü���� �ٽ� ��� �ؾ� �ϹǷ� ������Ʈ�� �н������� ü���� Ŭ���� ���� �ʴ´�.
	//if(!bGeometryStageNow)
	//	ClearChain();

	RestoreRenderState();

	// helper rendering - collision
 	if (m_bDrawCollisionObject /*&& !bGeometryStageNow*/)
 		RenderCollisionObject(pCurrentCamera,pRenderingList);
}


void RTreePassRenderer::RenderMStageOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, RRENDERINGMETHOD renderMethod)
{
	RPFC_THISFUNC;

	if( renderMethod == RENDERING_INFERRED)
		RenderOnPass( pCurrentCamera, pRenderingList, TREETECH_MSTAGE);

	RPFC_GPU_CALL;
}


void RTreePassRenderer::RenderForMakingLightMap( RCameraSceneNode* pCurrentCamera,SCENE_SET* pRenderingList, const RMatrix& matShadow, bool bProjectionShadow, const RVector& lightPos /*= RVector(0,0,0)*/, const float fLightRange /*= 0*/ )
{
	/// ����Ʈ�� ���������̱� ������ ���¸� ������ �ǹǷ� �������õ� �ʿ����.
	if (m_bDoNotRender)
		return;

	if ( pCurrentCamera == NULL)
		return;


	UploadShaderConstants( pCurrentCamera, REngine::GetSceneManagerPtr(), TREETECH_PROJECTIONSHADOW);	// ���̴� ��� ���ε�
	SetRenderState();

	m_scAlphaRef.SetFloat( 0.33f);

	TREE_TECH_TYPE eType = bProjectionShadow ? TREETECH_PROJECTIONSHADOW : TREETECH_SHADOW;
	RenderBranch( pCurrentCamera, pRenderingList, eType);
	RenderFrond( pCurrentCamera, pRenderingList, eType);
	RenderLeafCard( pCurrentCamera, pRenderingList, eType);
	RenderLeafMesh( pCurrentCamera, pRenderingList, eType);

	if( m_bUseInstancing)
		RTreeResource::RestoreForRenderInstancing();

	m_scAlphaRef.SetFloat(0);

	RestoreRenderState();
}

void RTreePassRenderer::RenderBranchInstance( RTreeResource* pTreeResource, RTreeSceneNode* pTreeSceneNode, RCameraSceneNode* pCamera)
{
	RPFC_THISFUNC;
	if( pTreeSceneNode->IsViewBranchFrond() )
	{
		SetTreeInstanceConstant( pCamera, pTreeSceneNode, pTreeSceneNode->m_SpeedTreeLodInfo.m_fBranchAlphaTestValue );

		//int ret = pTreeResource->RenderBranch( pTreeSceneNode->m_SpeedTreeLodInfo.m_nBranchActiveLod);
		int ret = pTreeResource->RenderBranch( 0);
		m_nPolyCount += ret;
		REngine::GetResourceProfiler().AddPolygon( PPT_TREEBRANCH,ret);
		REngine::GetResourceProfiler().AddDrawCall( PPT_TREEBRANCH, 1);
	}
}


void RTreePassRenderer::RenderBranchInstancing( RTreeResource* pTreeResource, RTreeInstancingBuffer* pTreeInstancingBuffer, RCameraSceneNode* pCamera, int nCnt)
{
	RPFC_THISFUNC;
	int ret = pTreeResource->RenderBranchInstancing( pTreeInstancingBuffer, nCnt);
	m_nPolyCount += ret;
	REngine::GetResourceProfiler().AddPolygon( PPT_TREEBRANCH,ret);
	REngine::GetResourceProfiler().AddDrawCall( PPT_TREEBRANCH, 1);
}


void RTreePassRenderer::RenderFrondInstance( RTreeResource* pTreeResource, RTreeSceneNode* pTreeSceneNode, RCameraSceneNode* pCamera)
{
	if( pTreeSceneNode->IsViewBranchFrond() )
	{
		SetTreeInstanceConstant( pCamera, pTreeSceneNode, pTreeSceneNode->m_SpeedTreeLodInfo.m_fFrondAlphaTestValue );

		int ret = pTreeResource->RenderFrond( pTreeSceneNode->m_SpeedTreeLodInfo.m_nFrondActiveLod);
		m_nPolyCount += ret;
		REngine::GetResourceProfiler().AddPolygon(PPT_TREEFROND,ret);
		REngine::GetResourceProfiler().AddDrawCall( PPT_TREEFROND, 1);
	}
}


void RTreePassRenderer::RenderFrondInstancing( RTreeResource* pTreeResource, RTreeInstancingBuffer* pTreeInstancingBuffer, RCameraSceneNode* pCamera, int nCnt)
{
	int ret = pTreeResource->RenderFrondInstancing( pTreeInstancingBuffer, nCnt);
	m_nPolyCount += ret;
	REngine::GetResourceProfiler().AddPolygon(PPT_TREEFROND,ret);
	REngine::GetResourceProfiler().AddDrawCall( PPT_TREEFROND, 1);
}


void RTreePassRenderer::RenderLeafCardInstance( RTreeResource* pTreeResource, RTreeSceneNode* pTreeSceneNode, RCameraSceneNode* pCamera)
{
	if( pTreeSceneNode->IsViewLeaves() )
	{
		float fLeafAlphaRef = pTreeSceneNode->m_SpeedTreeLodInfo.m_afLeafAlphaTestValues[0];
		SetTreeInstanceConstant( pCamera, pTreeSceneNode, fLeafAlphaRef );

		int ret = 0;

		int nLeafLodLevel = (int)pTreeSceneNode->m_SpeedTreeLodInfo.m_anLeafActiveLods[0];
		if( nLeafLodLevel != -1 )
		{
			ret += pTreeResource->RenderLeafCard( nLeafLodLevel);
		}

		nLeafLodLevel = (int)pTreeSceneNode->m_SpeedTreeLodInfo.m_anLeafActiveLods[1];
		if( nLeafLodLevel != -1 )
		{
			if( fLeafAlphaRef != pTreeSceneNode->m_SpeedTreeLodInfo.m_afLeafAlphaTestValues[1] )
			{
				m_scAlphaRef.SetFloat( pTreeSceneNode->m_SpeedTreeLodInfo.m_afLeafAlphaTestValues[1] / 255.0f );
				m_pTreeShader->CommitChanges();
			}

			ret += pTreeResource->RenderLeafCard( nLeafLodLevel);
		}

		m_nPolyCount += ret;
		REngine::GetResourceProfiler().AddPolygon(PPT_TREELEAF,ret);
		REngine::GetResourceProfiler().AddDrawCall( PPT_TREELEAF, 1);
	}
}


void RTreePassRenderer::RenderLeafCardInstancing( RTreeResource* pTreeResource, RTreeInstancingBuffer* pTreeInstancingBuffer, RCameraSceneNode* pCamera, int nCnt)
{
	int ret = 0;
	// ������ ���� �ٸ� LOD ���� ������ ���̵� ��/�ƿ� �Ǵ� �� ��� �����ұ�..
	ret += pTreeResource->RenderLeafCard( 0);

	m_nPolyCount += ret;
	REngine::GetResourceProfiler().AddPolygon(PPT_TREELEAF,ret);
	REngine::GetResourceProfiler().AddDrawCall( PPT_TREELEAF, 1);
}

void RTreePassRenderer::RenderLeafMeshInstance( RTreeResource* pTreeResource, RTreeSceneNode* pTreeSceneNode, RCameraSceneNode* pCamera)
{
	if( pTreeSceneNode->IsViewLeaves() )
	{
		float fLeafAlphaRef = pTreeSceneNode->m_SpeedTreeLodInfo.m_afLeafAlphaTestValues[0];
		SetTreeInstanceConstant( pCamera, pTreeSceneNode, fLeafAlphaRef );

		int ret = 0;

		int nLeafLodLevel = (int)pTreeSceneNode->m_SpeedTreeLodInfo.m_anLeafActiveLods[0];
		if( nLeafLodLevel != -1 )
		{
			ret += pTreeResource->RenderLeafMesh( nLeafLodLevel);
		}

		nLeafLodLevel = (int)pTreeSceneNode->m_SpeedTreeLodInfo.m_anLeafActiveLods[1];
		if( nLeafLodLevel != -1 )
		{
			if( fLeafAlphaRef != pTreeSceneNode->m_SpeedTreeLodInfo.m_afLeafAlphaTestValues[1] )
			{
				m_scAlphaRef.SetFloat( pTreeSceneNode->m_SpeedTreeLodInfo.m_afLeafAlphaTestValues[1] / 255.0f );
				m_pTreeShader->CommitChanges();
			}

			ret += pTreeResource->RenderLeafMesh( nLeafLodLevel);
		}

		m_nPolyCount += ret;
		REngine::GetResourceProfiler().AddPolygon(PPT_TREELEAF,ret);
		REngine::GetResourceProfiler().AddDrawCall( PPT_TREELEAF, 1);
	}
}

void RTreePassRenderer::RenderLeafMeshInstancing( RTreeResource* pTreeResource, RTreeInstancingBuffer* pTreeInstancingBuffer, RCameraSceneNode* pCamera, int nCnt)
{
	int ret = 0;
	// ������ ���� �ٸ� LOD ���� ������ ���̵� ��/�ƿ� �Ǵ� �� ��� �����ұ�..
	ret += pTreeResource->RenderLeafMesh( 0);

	m_nPolyCount += ret;
	REngine::GetResourceProfiler().AddPolygon(PPT_TREELEAF,ret);
	REngine::GetResourceProfiler().AddDrawCall( PPT_TREELEAF, 1);
}

void RTreePassRenderer::SetCompositeTexture(RTexture* pTexture)
{
	RShaderCompositeManager* pCompositeManager = REngine::GetSceneManagerPtr()->GetShaderCompositeManager();
	if(!pCompositeManager->GetViewLightingOnly())
		//m_scCompositeMap.SetTexture( pTexture , REngine::GetDevicePtr() );
		REngine::GetDevice().SetTexture(AS_TREE_COMPOSITE,  pTexture);
	else
		//m_scCompositeMap.SetTexture( REngine::GetSceneManager().GetDefaultNoiseTexture() , REngine::GetDevicePtr() );
		REngine::GetDevice().SetTexture(AS_TREE_COMPOSITE,  REngine::GetDevice().GetDefaultTexture());
}


void RTreePassRenderer::SetBranchTexture(RTexture* pTexture)
{
	REngine::GetDevice().SetTexture( AS_TREE_BRANCH_TEXTURE, pTexture);
}

void RTreePassRenderer::SetBranchDetailTexture(RTexture* pTexture)
{
	RPFC_THISFUNC;
	REngine::GetDevice().SetTexture( AS_TREE_BRANCH_DETAIL, pTexture);
}

void RTreePassRenderer::SetBillboardTexture(RTexture* pTexture)
{
	REngine::GetDevice().SetTexture( AS_TREE_BILLBOARD_TEXTURE, pTexture);
}

void RTreePassRenderer::SetBillboardNormalTexture(RTexture* pTexture)
{
	REngine::GetDevice().SetTexture( AS_TREE_BILLBOARD_NORMAL, pTexture);
}

void RTreePassRenderer::RenderGStageOnPass( RCameraSceneNode* pCurrentCamera,  SCENE_SET* pRenderingList, RRENDERINGMETHOD renderMethod)
{
	RPFC_THISFUNC;

	REngine::GetResourceProfiler().StartMeshProfiler();

	switch( renderMethod)
	{
	case RENDERING_INFERRED:
		RenderOnPass( pCurrentCamera, pRenderingList, TREETECH_GSTAGE);
		break;
	case RENDERING_DEFERRED:
		RenderOnPass( pCurrentCamera, pRenderingList, TREETECH_DEFERRED);
		break;
	}

	RPFC_GPU_CALL;
}

}
