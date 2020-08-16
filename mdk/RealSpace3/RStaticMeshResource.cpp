#include "StdAfx.h"
#include "RStaticMeshResource.h"
#include "RMaterialResource.h"
#include "RToken.h"
#include "RCameraSceneNode.h"
#include "RUtil.h"
#include "RVersions.h"
#include "RResourceID.h"
#include "RResourceLoader.h"
#include "RMaterialResourceMgr.h"
#include "RMaterial.h"
#include "RStaticMeshResourceFileLoadImpl.h"
#include "RShadowStrategy.h"
#include "REnvironmentSet.h"
#include "RShaderCompositeManager.h"
#include "REnvironmentSet.h"
#include "RShaderFX.h"

namespace rs3 {

bool	bStaicMeshRenderAll = false;

RPortal::~RPortal()
{
	SAFE_DELETE(m_pVertices);
}

RGeometry::RGeometry() : 
m_hVertexBuffer(R_NONE),
m_pPolygonInfo(NULL),
m_pVertices(NULL),
m_pIndices(NULL),
m_pNodes(NULL),
m_dwFVF(0),
m_nNode(0),
m_nPolygon(0),
m_nVertices(0),
m_nIndices(0),
m_nTrees(0)
{

}

RGeometry::~RGeometry()
{
	while(!m_pTrees.empty())
	{
		delete m_pTrees.back();
		m_pTrees.pop_back();
	}
	SAFE_DELETE_ARRAY(m_pPolygonInfo);
	SAFE_DELETE_ARRAY(m_pVertices);
	SAFE_DELETE_ARRAY(m_pIndices);
	SAFE_DELETE_ARRAY(m_pNodes);

	REngine::GetDevice().DeleteVertexBuffer(m_hVertexBuffer);
}

void RGeometry::MakeVertexInfo( RVertexOffsetInfo& info )
{
	//	RVector pos;		// position
	//	RVector normal;		// normal vector
	//  DWORD	color;
	//	float	tu1, tv1;	// texture coordinates
	//	float	tu2, tv2;	// lightmap texture coord.

	// ���߿� FVF ���� Ŭ���� ���鶧 ��� �� �˻��ؼ� �����ϰ� �ٲ���
	if( m_nVertices )
	{
		// Pos
		info.pos = info.size;
		info.size += RFVF[RFVF_XYZ].size;
		// Nor
		info.normal = info.size;
		info.size += RFVF[RFVF_NORMAL].size;
	}

	if( m_dwFVF & RFVF[RFVF_DIFFUSE].fvf )
	{
		info.color = info.size;
		info.size += RFVF[RFVF_DIFFUSE].size;
	}

	if( m_nVertices )
	{
		// texture coordinates
		info.texCoord = info.size;
		info.size += RFVF[RFVF_TEX1].size + RFVF[RFVF_TEX2].size;
	}
}

RCell::RCell()
{
}

RCell::~RCell()
{
	for(IDGEOMETRYMAP::iterator itr = m_mapGeometryList.begin() ; itr!=m_mapGeometryList.end(); ++itr)
		delete itr->second;
	m_mapGeometryList.clear();

	for(size_t i = 0; i < m_cellFaces.size(); ++i)
		delete m_cellFaces[i];
	m_cellFaces.clear();
}

bool RCell::Contains(const RVector& p)	// �� ���� �����ϰ� �ִ���
{
	for(list<RPlane>::iterator i=m_planes.begin();i!=m_planes.end();++i)
	{
		RPlane &plane = *i;
		if(plane.GetDistanceTo(p)<0) return false;
	}
	return true;
}

#define MREAD(x) { if(!file.Read(&x,sizeof(x))) return false; }

void CalcTangentBinormal(const RVector &x0, float u0, float v0,
						 const RVector &x1, float u1, float v1,
						 const RVector &x2, float u2, float v2,
						 RVector *outTangent, RVector *outBinormal)
{
	if(outTangent)
		*outTangent = (v2-v0)*(x1-x0) -(v1-v0)*(x2-x0);
	if(outBinormal)
		*outBinormal = -(u2-u0)*(x1-x0) +(u1-u0)*(x2-x0);
}

//////////////////////////////////////////////////////////////////////////
// RStaticMeshResource

const char* RStaticMeshResource::m_pSzDefaultLigntmapsGroup = "DefaultLightmaps";

RStaticMeshResource::RStaticMeshResource(void)
: RResource("staticmesh", RRESOURCETYPE::RR_STATICMESH)
, m_hIndexBuffer(R_NONE)
{
	class EnvironmentListener : public RListener
	{
		RStaticMeshResource* m_pResource;
	public:
		EnvironmentListener(RStaticMeshResource* pResource) : m_pResource(pResource) {}

		virtual void Update() override
		{
			m_pResource->CreateShaders();		
		}
	};

	m_pEventListener = new EnvironmentListener(this);
}

RStaticMeshResource::~RStaticMeshResource(void)
{
	Destroy();

	SAFE_DELETE(m_pEventListener);
}

void RStaticMeshResource::Destroy()
{
	// event ����
	REngine::GetEventBroadcaster(RE_ENVIRONMENT_CHANGE)->RemoveListener( m_pEventListener );
	REngine::GetEventBroadcaster(RE_SHADOW_CHANGE)->RemoveListener( m_pEventListener );

	for(int i = 0; i < GetCellCount(); i++)
	{
		RCell* pCell= GetCell(i);
		for ( RCell::IDGEOMETRYMAP::iterator it = pCell->m_mapGeometryList.begin(); it != pCell->m_mapGeometryList.end(); ++it )
		{
			RGeometry* pGeometry = (RGeometry*)(it->second);
			for(int j = 0; j < pGeometry->m_nNode; j++)
			{
				RRsNode* pNode = &pGeometry->m_pNodes[j];
				if( pNode->m_nIndicies )
				{
					// delete ib
					REngine::GetDevice().DeleteIndexBuffer(pNode->m_hIB);
				}
			}
		}
	}

	// Material Resource ����
	for(int i=0;i<(int)m_vecMaterials.size();i++)
	{
		REngine::GetSceneManager().ReleaseResource( m_vecMaterials.at(i) );
	}
	m_vecMaterials.clear();
	//	SAFE_DELETE_ARRAY(m_pRenderInfo);

	for(int i=0;i<(int)m_portals.size();i++)
		delete m_portals[i];

	for(int i=0;i<(int)m_cells.size();i++)
		delete m_cells[i];
}

void RStaticMeshResource::MakeDirectoryNameFromFileName( const char* pSzFileName, char* pOut, int nCount )
{
	const char *lastPathToken = max(strrchr(pSzFileName,'/'), strrchr(pSzFileName,'\\'));
	if(lastPathToken) {
		int nLength = lastPathToken-pSzFileName+1;
		_ASSERT(nLength < nCount );
		strncpy(pOut,pSzFileName,nLength);
		pOut[nLength] = 0;
	}else
		pOut[0]=0;
}


bool RStaticMeshResource::OpenDescription(const char *szFileName, bool bBackground)
{
	MFile file;
	if(!file.Open(szFileName))
		return false;

	MakeDirectoryNameFromFileName(szFileName, m_szCurrentDirectory, sizeof(m_szCurrentDirectory) );

	char *buffer;
	buffer=new char[file.GetLength()+1];
	file.Read(buffer,file.GetLength());
	buffer[file.GetLength()]=0;

	MXml aXml;
	if(!aXml.LoadStream(buffer))
	{
		delete [] buffer;
		return false;
	}

	bool bSuccess = false;
	MXmlElement* pElement = aXml.DocHandle().FirstChildElement().Element();
	if( pElement )
	{
		// TODO: ����Ʈ�� �ε� ���ϰ� - ������ �������� �����Ҷ� ������ ������.
		// ���� ũ�Ⱑ ũ�� �޸𸮰� �����Ͽ� �ε� �����ϸ� ����� ����Ʈ�ʺ��� �ε� �Ѵ�.
		//MXmlElement* pLightMapListElement = pElement->FirstChildElement( RTOK_LIGHTMAPS );
		//if( pLightMapListElement )
		//	OpenLightmaps( pLightMapListElement, bBackground );

		MXmlElement* pMaterialListElement = pElement->FirstChildElement( RTOK_MATERIALLIST );
		if( pMaterialListElement )
			OpenMaterials( pMaterialListElement, bBackground );

		bSuccess = true;
	}

	delete[] buffer;
	file.Close();

	return bSuccess;
}

bool RStaticMeshResource::OpenMaterials(MXmlElement* pElement, bool bBackground)
{
	char szTagName[256];

	MXmlElement* pMaterialElement = pElement->FirstChildElement();

	int nCount=0;
	for(; pMaterialElement != NULL; pMaterialElement = pMaterialElement->NextSiblingElement())
	{
		strcpy(szTagName, pMaterialElement->Value());
		if(stricmp(szTagName,"MATERIAL")==0)
		{
			string strMaterialName;
			_Attribute( strMaterialName, pMaterialElement, "name");

			// give a unique name
			char szNameBuffer[256];
			sprintf(szNameBuffer, "%s_%d_%s", m_strName.c_str(), nCount++, strMaterialName.c_str() );
			RMaterialResource* pMaterialResource = REngine::GetSceneManager().CreateResource< RMaterialResource >( szNameBuffer );

//			RMaterialResource* pMaterialResource = REngine::GetSceneManager().CreateResource< RMaterialResource >( strMaterialName.c_str() );
			if( pMaterialResource->GetState() == RRS_EMPTY )
			{
				pMaterialResource->Open(pMaterialElement, bBackground);
			}
			m_vecMaterials.push_back(pMaterialResource);
			REngine::GetSceneManager().AddLoadingList( pMaterialResource, bBackground );
		}
	}
	return true;
}

void RStaticMeshResource::RecreateVertexBuffer()
{
	for(int i = 0; i < (int)m_cells.size(); ++i)
	{
		RCell *pCell = m_cells[i];
		for(RCell::IDGEOMETRYMAP::iterator itr = pCell->m_mapGeometryList.begin() ; itr != pCell->m_mapGeometryList.end(); ++itr)
		{
			RGeometry* pGeometry = itr->second;
			if(pGeometry->m_hVertexBuffer)
				REngine::GetDevice().DeleteVertexBuffer(pGeometry->m_hVertexBuffer);
			pGeometry->m_hVertexBuffer = REngine::GetDevice().CreateVertexBuffer(pGeometry->m_VertexInfo.size * pGeometry->m_nVertices, pGeometry->m_VertexInfo.size, pGeometry->m_pVertices);
		}
	}
}

bool RStaticMeshResource::Fill()
{
	MFile file;
	if(!file.Open(m_strFileName.c_str())) return false;

	RHEADER header;

	MREAD(header);
	m_dwVersion = header.dwVersion;
	if(header.dwID!=RS3_ID || header.dwVersion>RS3_VERSION)
	{
		mlog("%s : %d , %d version required.\n",m_strFileName.c_str(),header.dwVersion,RS3_VERSION);
		file.Close();
		return false;
	}

	RStaticMeshResourceFileLoadImpl *pImpl = NULL;
	if ( header.dwVersion <= RS3_VER1 )
		pImpl = new RStaticMeshResourceFileLoadImpl_v1;
	else if( header.dwVersion == RS3_VER2 )
		pImpl = new RStaticMeshResourceFileLoadImpl_v2;
	else if( header.dwVersion == RS3_VER3 )
		pImpl = new RStaticMeshResourceFileLoadImpl_v3;
	else if( header.dwVersion == RS3_VER4 )
		pImpl = new RStaticMeshResourceFileLoadImpl_v4;
	_ASSERT(pImpl);

	int nPortals;
	MREAD(nPortals);
	for(int i=0;i<nPortals;i++)
	{
		RPortal *pPortal = new RPortal;
		int nLength;
		MREAD(nLength);
		pPortal->m_strName.resize(nLength);
		file.Read( &pPortal->m_strName[0], nLength );
		MREAD(pPortal->m_nVertices);
		pPortal->m_pVertices = new RVector[pPortal->m_nVertices];
		if(!file.Read(pPortal->m_pVertices,sizeof(RVector)*pPortal->m_nVertices))
			return false;
		MREAD(pPortal->m_nCellID1);
		MREAD(pPortal->m_nCellID2);
		m_portals.push_back(pPortal);
	}

	// VER2
	pImpl->LoadCell(file, this);

	delete pImpl;

	file.Close();

	// connect cells by portal
	for(int i = 0; i < nPortals; i++) // nPortals -> ������ ���� ��
	{
		RPortal *pPortal = m_portals[i];

		// -1 �ΰ��� global �� ��Ÿ����

		// �������� ���� ��Ż�� ������ �ɷ�����.
		if(pPortal->m_nCellID1 < 0 || pPortal->m_nCellID1>= (int)m_cells.size()) continue;
		if(pPortal->m_nCellID2 < 0 || pPortal->m_nCellID2>= (int)m_cells.size()) continue;

		// shape �� ��/�� ������ �����Ƿ� �Ǵ��ؾ� �Ѵ�
		RCell *pCellFront = m_cells[pPortal->m_nCellID1];
		RCell *pCellBack = m_cells[pPortal->m_nCellID2];

		RPlane testPlane;
		testPlane.SetPlane(pPortal->m_pVertices[0],pPortal->m_pVertices[1],pPortal->m_pVertices[2]);

		RVector aPointCell1 = .5f * ( pCellFront->m_bb.vmax + pCellFront->m_bb.vmin );
		if(testPlane.GetDistanceTo(aPointCell1) < 0 ) {	 // pCellFront �� ����
			swap(pCellFront,pCellBack);
		}

		pPortal->m_plane = testPlane;
		pCellFront->m_portals.push_back(make_pair(pPortal,pCellBack));

		// �޸� ��Ż�� ���� �ִ´�
		RPortal *pPortalBack = new RPortal;
		pPortalBack->m_nCellID1 = pPortal->m_nCellID2;
		pPortalBack->m_nCellID2 = pPortal->m_nCellID1;
		pPortalBack->m_nVertices = pPortal->m_nVertices;
		pPortalBack->m_pVertices = new RVector[pPortal->m_nVertices];
		for(int j=0;j<pPortal->m_nVertices;j++)	// ���ؽ��� �������� ����
			pPortalBack->m_pVertices[j] = pPortal->m_pVertices[pPortal->m_nVertices-j-1];
		pPortalBack->m_plane = -testPlane;
		m_portals.push_back(pPortalBack);

		pCellBack->m_portals.push_back(make_pair(pPortalBack,pCellFront));
	}

	// material �ε�
	//if( !REngine::GetSceneManagerPtr()->GetCurrentResLoadingGroup() )
	//{
	//	bool bBackgroundLoading = IsBackgroundCreated();
	//	for( int j = 0; j < (int)m_vecMaterials.size(); ++j )
	//	{
	//		RMaterialResource* pMaterialResource = m_vecMaterials[j];
	//#ifdef _STATICMESH_BACKGROUND_LOADING
	//		pMaterialResource->SetLoadingPriority( RBGWK_SLEEP );
	//#endif
	//		pMaterialResource->BuildAll(bBackgroundLoading);
	//	}
	//}

	return true;
}

RResource::LOAD_RESULT RStaticMeshResource::Load()
{
	RecreateVertexBuffer();

	// create ib
	for(int i = 0; i < GetCellCount(); i++)
	{
		RCell* pCell= GetCell(i);
		for ( RCell::IDGEOMETRYMAP::iterator it = pCell->m_mapGeometryList.begin(); it != pCell->m_mapGeometryList.end(); ++it )
		{
			RGeometry* pGeometry = (RGeometry*)(it->second);
			for(int j = 0; j < pGeometry->m_nNode; j++)
			{
				RRsNode* pNode = &pGeometry->m_pNodes[j];
				if( pNode->m_nIndicies )
				{
					// create ib
					pNode->m_hIB = REngine::GetDevice().CreateIndexBuffer(pNode->m_nIndicies, RFMT_INDEX16, pNode->m_pIndicies);
				}
			}
		}
	}

	CreateShaders();

	// scenemanager event ����
	REngine::GetEventBroadcaster(RE_ENVIRONMENT_CHANGE)->AddListener( m_pEventListener );
	REngine::GetEventBroadcaster(RE_SHADOW_CHANGE)->AddListener( m_pEventListener );

	return LOADED;
}

void RStaticMeshResource::CreateShaderForDepth(RenderInfo* pRenderInfo)
{
	//////////////////////////////////////////////////////////////////////////
	// �׸��ڿ� ���̴�

	if( REngine::GetSceneManager().GetRealtimeShadowMethod() == SHADOW_NONE )
		return;

	RShaderCompositeManager* m_pShaderCompositeManager = REngine::GetSceneManager().GetShaderCompositeManager();
	RShadowStrategy* pShadowStrategy = REngine::GetSceneManager().GetShadowStrategy();
	RSCID rscComponents = RSC_POSITION | m_pShaderCompositeManager->GetVertexTexcoordComponent() | pShadowStrategy->GetDepthWriteComponent();

	if( pRenderInfo->m_depthEffectId != rscComponents )
	{
		RShaderFX* pShader = m_pShaderCompositeManager->GetShader( rscComponents, false );
		pRenderInfo->m_depthTechnique = pShader->GetTechniqueByIndex(0);
		pRenderInfo->m_depthEffectId = rscComponents;
	}
}

void RStaticMeshResource::CreateShaderForGStage(RenderInfo* pRenderInfo)
{
	RShaderCompositeManager* pShaderCompositeManager = REngine::GetSceneManager().GetShaderCompositeManager();
	RMaterial* pMaterial = pRenderInfo->m_pMaterial;

	ComponentDesc_GStage desc;
	desc.pMtrl = pMaterial;
	desc.bPhysique = false;
	desc.bTexCoord = true;
	desc.bExtraTexCoord = false;
	desc.bVertexColor = false;
	desc.bNoiseRef = false;
	desc.bInstancing = false;
	desc.bMultiplyColor = false;
	desc.bAO = false;
	desc.bBlendingMaterial = false;

	RSCID rscComponents = pShaderCompositeManager->MakeComponentBitflagForGStage( RENDERING_INFERRED, desc);
	RTechnique technique = pShaderCompositeManager->GetShader( rscComponents, false )->GetTechniqueByIndex(0);	

	if( pRenderInfo->m_GStageEffectId != rscComponents )
	{
		RShaderFX* pShader = pShaderCompositeManager->GetShader( rscComponents, false );
		pRenderInfo->m_GStageTechnique = pShader->GetTechniqueByIndex(0);
		pRenderInfo->m_GStageEffectId = rscComponents;
	}
}

void RStaticMeshResource::CreateShaderForMStage(RenderInfo* pRenderInfo, bool bFogEnable)
{
	RShaderCompositeManager* pShaderCompositeManager = REngine::GetSceneManager().GetShaderCompositeManager();
	RMaterial* pMaterial = pRenderInfo->m_pMaterial;


	ComponentDesc_MStage desc;

	desc.pMtrl = pMaterial;
	desc.bAddColorPass = false;
	desc.bPhysique = false;
	desc.bVertexColor = pRenderInfo->m_bPointColor;
	desc.bTexCoord = true;
	desc.bExtraTexCoord = false;
	desc.bUseShade = true;
	desc.bBlendingMaterial = false;
	desc.fVisibility = 1.0f;
	desc.bNoiseRef = false;
	desc.bInstancing = false;
	desc.bMultiplyColor = false;

	// ����ƽ �Ž��� �������� ���� ���̹Ƿ� DSF�� ��� ���� �ʴ´�.
	RSCID rscComponents = pShaderCompositeManager->MakeComponentBitflagForMStage(desc);

//	REnvironmentSet* pEnvironmentSet = pShaderCompositeManager->UpdateEnvironmentSet();

	RSCID& currentEffectID		 = bFogEnable ? pRenderInfo->m_MStageFogEffectId : pRenderInfo->m_MStageEffectId;
	RTechnique& currentTechnique = bFogEnable ? pRenderInfo->m_MStageFogTechnique : pRenderInfo->m_MStageTechnique;

	if( currentEffectID != rscComponents )
	{
		RShaderFX* pShader = pShaderCompositeManager->GetShader( rscComponents, false );
		if(!pShader)
		{
			mlog("shader create error : shader change from %s to %s\n",
				currentEffectID.ToString(10).c_str(), rscComponents.ToString(10).c_str() );

			// ���������Ƿ�, default ���̴��� ��ȯ
			RSCID rscDefaultShaderComponents = RSC_POSITION | RSC_NORMAL | RSC_TRANSFORM;
			RShaderFX* pEffect = pShaderCompositeManager->GetShader( rscDefaultShaderComponents, false);
			currentTechnique = pEffect->GetTechniqueByIndex(0);
			currentEffectID = rscDefaultShaderComponents;
			return;
		}

		currentTechnique = pShader->GetTechniqueByIndex(0);
		currentEffectID = rscComponents;
	}
}

void RStaticMeshResource::CreateShaders()
{
	for(int i = 0; i < GetCellCount(); i++)
	{
		RCell* pCell= GetCell(i);
		for ( RCell::IDGEOMETRYMAP::iterator it = pCell->m_mapGeometryList.begin(); it != pCell->m_mapGeometryList.end(); ++it )
		{
			RGeometry* pGeometry = (RGeometry*)(it->second);
			for(int j = 0; j < pGeometry->m_nTrees; j++)
			{
				RRsAABBTree* pTree = pGeometry->m_pTrees[j];

				RenderInfo* pRenderInfo = &pTree->m_renderInfo;
				pRenderInfo->m_pMaterial = m_vecMaterials[ pTree->m_nMaterial ]->m_pMaterial;

				// �⺻ ���̴� ����
				pRenderInfo->m_bPointColor = (pGeometry->m_VertexInfo.color >= 0) ? true : false;

				// VertexColor�� LightMap�� ������ ���� �Ǵ� ��Ģ �߰�.
				//if( pTree->m_nLightmapID >= 0 )
				//	bPointColor = false;

				CreateShaderForDepth( pRenderInfo );
				CreateShaderForGStage( pRenderInfo );
				CreateShaderForMStage( pRenderInfo, true );
				CreateShaderForMStage( pRenderInfo, false );
			}
		}
	}
}

}