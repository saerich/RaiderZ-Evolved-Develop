#include "StdAfx.h"
#include "C_ControlMediator.h"
#include "C_ControlMediatorHelperResource.h"
#include "C_ControlMediatorHelperAction.h"
#include "C_ControlCursor.h"

#include "M_ToolCmdMgr.h"
#include "M_CommandBuffer.h"

#include "C_XMLConfig.h"
#include "C_XMLUserConfig.h"
#include "C_XMLEditInfoFile.h"
#include "C_RubyManager.h"
#include "RUtil.h"
#include "M_ToolActor.h"
#include "M_ToolDecal.h"

#include "VR_EnvWeather.h"
#include "VL_SceneNodeList.h"

#include "RAnimationResource.h"

#include "RRenderHelper.h"

#include "MFileSystem.h"

#include <map>
#include <vector>
#include <float.h>

#include "WS_WorkSpaceManager.h"

CControlMediatorHelperResource::CControlMediatorHelperResource(void)
: m_pRefActor_(NULL)
, m_pControlMediator_(NULL)
{
	m_pToolDecal = new rs3::RToolDecal();	
}

CControlMediatorHelperResource::~CControlMediatorHelperResource(void)
{
	SAFE_DELETE(m_pToolDecal);
}
using namespace rs3;
void CControlMediatorHelperResource::SetMediatorWithLoadedTerrain_()
{
	//�� ����
	CControlMediator::MAP_INFO& mapInfo = m_pControlMediator_->m_mapInfo;
	RToolSceneManager* pSceneManager = m_pControlMediator_->GetToolSceneManager();
	RToolTerrain* pTerrain = (RToolTerrain*) pSceneManager->GetCurrentTerrain();

	//Zone���� �Ҹ�
	mapInfo.m_bLoaded = true;
	mapInfo.m_bSceneEditableMap = true;
	mapInfo.m_bEditSceneObject = true;

	mapInfo.m_orgX = 0.f;
	mapInfo.m_orgY = 0.f;
	mapInfo.m_width = pTerrain->GetWidthSize();
	mapInfo.m_height = pTerrain->GetHeightSize();

	mapInfo.m_rowFace = pTerrain->GetHeightMap();
	mapInfo.m_colFace = pTerrain->GetWidthMap();

	//pick ignore scene �ʱ�ȭ
//	pSceneManager->m_pPickingIgnoreSceneNode = NULL;

	//ī�޶� �ʱ�ȭ
	CreateMiniMapAndSetCamera("_minimap.tmp", MINI_JUST_CAMERA_SET);
}

// rs3::RSceneNode* g_pSceneNode = NULL;

void CControlMediatorHelperResource::SetMediatorWithNoneTerrain_()
{
	CControlMediator::MAP_INFO& mapInfo = m_pControlMediator_->m_mapInfo;
	mapInfo.m_bLoaded = true;
	mapInfo.m_bSceneEditableMap = true;
	mapInfo.m_bEditSceneObject = true;

	// map�� ũ�� �˾ƿ��� --
	// ��� ���͸� ���ؿͼ� AABB�� x,y�� ���� �������ϱ�
	struct Impl
	{
		static bool GetAllMesh( rs3::RSceneNode* pSceneNode )
		{
// 			bool bStaticSceneNode = (stricmp(pSceneNode->GetNodeIDString(), RSID_STATICMESHSCENENODE) == 0) ? true : false;
// 			if (bStaticSceneNode)
// 				g_pSceneNode = pSceneNode;

			if ( stricmp(pSceneNode->GetNodeIDString(), RSID_ACTOR) == 0 
 				|| stricmp(pSceneNode->GetNodeIDString(), RSID_STATICMESHSCENENODE) == 0
				|| stricmp(pSceneNode->GetNodeIDString(), RSID_TERRAINSCENENODE) == 0)
				return true;
			else
				return false;
		}
	};

	_ASSERT( m_pControlMediator_ );
	rs3::RToolSceneManager* pSceneManager = m_pControlMediator_->GetToolSceneManager();

	_ASSERT( pSceneManager );
// 	g_pSceneNode = NULL;
	std::vector< rs3::RSceneNode* > vSceneNode;
	pSceneManager->GetRootSceneNode().SerializeToVector( Impl::GetAllMesh, vSceneNode );
//	pSceneManager->m_pPickingIgnoreSceneNode = g_pSceneNode;

	std::vector< rs3::RSceneNode* >::iterator itr = vSceneNode.begin();
	if( itr != vSceneNode.end() )
	{
		CFileFind finder;
		
		RSceneNode* pSceneNode = *itr;
		RBoundingBox kAABB = pSceneNode->GetAABB();
		for (; itr != vSceneNode.end(); ++itr )
		{
			RSceneNode* pSceneNode = *itr;
			if ( pSceneNode->GetUsable() )
			{
				if ( pSceneNode->GetAABB().vmin.x != FLT_MAX )
					kAABB.Add(pSceneNode->GetAABB());
			}
		}

		mapInfo.m_orgX = kAABB.minx;
		mapInfo.m_orgY = kAABB.miny;
		mapInfo.m_width = kAABB.maxx - kAABB.minx;
		mapInfo.m_height = kAABB.maxy - kAABB.miny;

		mapInfo.m_rowFace = 0;
		mapInfo.m_colFace = 0;

		mapInfo.m_aabb = kAABB;
	}

	//ī�޶� �ʱ�ȭ
	CreateMiniMapAndSetCamera("_minimap.tmp", MINI_JUST_CAMERA_SET);
}

void CControlMediatorHelperResource::ManageConfigFileRelatedMap(const char* _pStrName, MAP_CONFIG_FILE _type, bool _bNeedEditInfoFile)
{
	RToolCamera* pCamera = m_pControlMediator_->GetToolCamera();
	bool bZone = m_pControlMediator_->m_mapInfo.m_bSceneEditableMap;

	//�ʰ� ���õ� config�� �ε�
	if(_type == MAP_CONFIG_LOAD)
	{
		//�ʰ� ���õ� config���� �ε�
		if(CXMLUserConfig::GetReference().LoadConfigRelatedMapFile(_pStrName, pCamera))
		{
			//ī�޶�
			RVector vPos, vDir;
			if(CXMLUserConfig::GetReference().GetLastCameraInfo(vPos, vDir))
			{
				pCamera->SetPosAndDir(vPos, vDir);
			}
		}

		if (_bNeedEditInfoFile)
		{
			// �� ������ �ʿ��� �������Ϸε�
			if(bZone) //zone�̶��
			{
				if ( CXMLEditInfoFile::GetReference().LoadZoneInformationFile(_pStrName) == false)
				{
					AfxMessageBox("�� �������� ���Ͽ� ������ �ֽ��ϴ�.");
					CXMLEditInfoFile::GetReference().SaveZoneInformationFile(_pStrName, true);

				}
			}
			else
			{
				if ( CXMLEditInfoFile::GetReference().LoadFieldInformationFile(_pStrName) == false)
				{
					if (_pStrName)
					{
						std::string fullPath(_pStrName);
						int find_token = fullPath.rfind("\\") + 1;

						std::string strPath(fullPath.substr(0, find_token));
						std::string strName(fullPath.substr(find_token, fullPath.length() - find_token - 4)); //Ȯ���� ����

						AfxMessageBox(("Not found, so re-new " + strPath + "editor_info\\" + strName + ".eoi").c_str());
					}
					else
					{
						AfxMessageBox("�ʵ� �������� ������ �������� �ʾҽ��ϴ�.");

					}
					CXMLEditInfoFile::GetReference().SaveFieldInformationFile(_pStrName, true);

				}
			}
		}
	}
	//�ʰ� ���õ� config�� ���̺�
	else
	{
		// Extended Information
		CXMLUserConfig::GetReference().SetLastCameraInfo(pCamera->GetPosition(), pCamera->GetDirection());
		CXMLUserConfig::GetReference().SetLastWork((int) m_pControlMediator_->m_wbCurrentState_);

		//Last EnvSet Info
		int time, weather, viewBOOL;
		std::string strEnvSetName;
		m_pControlMediator_->GetViewPtr<CRollupEnvWeather>()->GetWeatherInfo(viewBOOL, time, weather, strEnvSetName);
		CXMLUserConfig::GetReference().SetEnvironmentInfo(viewBOOL ? true : false, time, weather, strEnvSetName);

		//Save MapRelated Config File
		CXMLUserConfig::GetReference().SaveConfigRelatedMapFile(_pStrName, pCamera);

		if (_bNeedEditInfoFile)
		{
			if(bZone)
			{
				bool bForceSave = false;
				if( _type == MAP_CONFIG_SAVE_AS )
					bForceSave = true;

				CXMLEditInfoFile::GetReference().SaveZoneInformationFile(_pStrName, bForceSave, bForceSave);
			}
			else
				CXMLEditInfoFile::GetReference().SaveFieldInformationFile(_pStrName);
		}
	}
}

void CControlMediatorHelperResource::CreateMiniMapAndSetCamera(const char* _szFileName, MINI_MAP _type)
{
	CControlMediator::MAP_INFO& mapInfo = m_pControlMediator_->m_mapInfo;

	if(_type == MINI_DELETE)
	{
		std::string strMiniMapTmp(mapInfo.m_strScenePath + _szFileName);
		const char* szMiniMapTmp = strMiniMapTmp.c_str();
		if(GetFileAttributes(szMiniMapTmp) != INVALID_FILE_ATTRIBUTES)
			DeleteFile(szMiniMapTmp);
	}
	else
	{
		RToolSceneManager* pSceneManager = m_pControlMediator_->GetToolSceneManager();
		ASSERT(pSceneManager);
		RToolCamera* pCamera = m_pControlMediator_->GetToolCamera();
		ASSERT(pCamera);
		RVector vOrgCamPos = pCamera->GetPosition();
		RVector vOrgCamDir = pCamera->GetDirection();

		if (_type != MINI_NONE_CAMERA_SET)
		{
			float fXCenter = mapInfo.m_width * 0.5f;
			float fYCenter = mapInfo.m_height * 0.5f;
			float DISTANCE = mapInfo.m_width * 0.35f; // * 30.0f;

			RVector vCenter(fXCenter, fYCenter, 0);
			RVector vCamPos(fXCenter, fYCenter, 500000);

			const RVector* pCenter = pSceneManager->GetVerticalContactPos( vCenter, NULL );
			if ( pCenter)
			{
				vCenter = *pCenter;
				vCamPos = vCenter + RVector(0, -DISTANCE, DISTANCE * 1.7);
			}
			else
			{
				RVector vMax = mapInfo.m_aabb.vmax;
				RVector vMin = mapInfo.m_aabb.vmin;
				vCenter = ( vMax + vMin ) * 0.5f;

				RVector vHalf = vMax - vCenter;
				float fLength = vHalf.Length();
				vCamPos = RVector(0.8f,-0.8f,0.8f) * fLength;
			}
// 			else
// 			{
// 				// vCenter, vCamPos ���� ������ �ʱⰪ���� ����
// 			}

			pCamera->SetPosAndTarget(vCamPos, vCenter, true);
			pCamera->RememberLocation(RToolCamera::DEFAULT_CAMERA);
		}

		if (_type != MINI_JUST_CAMERA_SET)
		{
			RDeviceD3D* pDevice = m_pControlMediator_->GetCurrentDevice();
			//���� ����
			bool bFoged = pCamera->GetFogEnable();
			if( bFoged )
				pCamera->SetFogEnable(false);

			pSceneManager->UpdateTimer();
			pSceneManager->Update();

			pDevice->Clear(true, true, true, D3DCOLOR_XRGB(35,135,195), 1.0f, 0L);
			pDevice->BeginScene();
			pSceneManager->RenderPrimaryCamera();
			pDevice->EndScene();
			pDevice->Flip();

			std::string strTemp(mapInfo.m_strScenePath + _szFileName);
			pDevice->SaveScreenShot( strTemp.c_str() );

			// ����
			if( bFoged )
				pCamera->SetFogEnable(true);
		}

		// ī�޶� ����ġ
		pCamera->SetPosAndDir(vOrgCamPos, vOrgCamDir);
	}
}

void CControlMediatorHelperResource::RefreshZone()
{
	if(m_pControlMediator_->m_mapInfo.m_bLoaded == false || m_pControlMediator_->m_mapInfo.m_bSceneEditableMap == false)
		return;

	//�𷡽ð� ����
	HCURSOR hOld = SetCursor(LoadCursor(NULL, IDC_WAIT));

	//ī�޶� ��ġ ����
	m_pControlMediator_->m_pCurrentCamera_->RememberLocation(RToolCamera::TMP_CAMERA);
	//�� ���濩�� ����
	bool bMapChanged = m_pControlMediator_->GetToolCmdMgr()->IsMapChanged();

	std::string fileName(m_pControlMediator_->m_mapInfo.m_strScenePath + "_temp.zone.xml");
	std::string orgFileName(m_pControlMediator_->m_mapInfo.m_strScenePath + m_pControlMediator_->m_mapInfo.m_strSceneFile );

	const char* szFileName = fileName.c_str();
	m_pControlMediator_->SaveZone(szFileName, false, false);
	//�������� �������� ���� ����
	CXMLEditInfoFile::GetReference().SaveZoneInformationFile(szFileName, true, true);

	m_pControlMediator_->ResetResource();
	m_pControlMediator_->LoadSceneAndEnvFromXML_(szFileName, MAP_EDIT_TYPE_SCENE);

	// �� �Ŵ����� �б��о� ������
	RWorldSceneNode* pWorldSceneNode = &m_pControlMediator_->GetToolSceneManager()->GetRootSceneNode();
	pWorldSceneNode->SetXMLFileName(orgFileName);

	RTerrain* pTerrain = m_pControlMediator_->GetToolSceneManager()->GetCurrentTerrain();
	if(pTerrain)
		pTerrain->SetPathAndResourceName();

	//terrain�� ������ ���� �ε��� / �ƿ���� ����
	if(m_pControlMediator_->GetToolSceneManager()->GetCurrentTerrain() != NULL)
		SetMediatorWithLoadedTerrain_();
	else
		SetMediatorWithNoneTerrain_();

	//ī�޶� ��ġ ȸ��
	const RToolCamera::LOCATION_INFO* pLocationInfo = m_pControlMediator_->m_pCurrentCamera_->GetLocation(RToolCamera::TMP_CAMERA);
	m_pControlMediator_->m_pCurrentCamera_->SetLocation(pLocationInfo);

	//�������� �������� �ε�
	CXMLEditInfoFile::GetReference().LoadZoneInformationFile(szFileName);

	//���۷��� �޽� �缳��
	ResetRefMesh( CXMLUserConfig::GetReference().GetRefMeshName() );

	//�� ���濩�� ����
	if(bMapChanged)
		m_pControlMediator_->GetToolCmdMgr()->RecordCommand(NULL);

	//���� �ε��Ǿ��ٰ� view���� �˸���
	m_pControlMediator_->BroadCastMessage( VIEW_MESSAGE::SCENE_LOADED );

	//temp���� �����
	DeleteTempFile();

	//�𷡽ð� ��
	SetCursor(hOld);
}

void CControlMediatorHelperResource::ResetRefMesh(const char* _pMeshName)
{
	ASSERT(m_pControlMediator_->m_pCurrentSceneManager_);
	//�ϴ� �����
	if(m_pRefActor_ != NULL)
	{
		m_pRefActor_->RemoveFromParent();
		SAFE_DELETE(m_pRefActor_);
	}
	if(_pMeshName != NULL)
	{
		m_pRefActor_ = new rs3::RToolActor(m_pControlMediator_);
		//m_pRefActor_->SetManager( m_pControlMediator_->m_pCurrentSceneManager_ );
		if(m_pRefActor_->Create(_pMeshName) == false)
		{
			SAFE_DELETE(m_pRefActor_);
			return;
		}
		m_pRefActor_->Stop();
		m_pControlMediator_->m_pCurrentSceneManager_->AddSceneNode(m_pRefActor_);
		m_pRefActor_->SetVisible(false);
		m_pRefActor_->SetNodeName( "RefferenceMesh" );
		m_pRefActor_->SetCullState( false );
		m_pRefActor_->AddAttribute( RSNA_DO_NOT_SAVE );
		m_pRefActor_->AddAttribute( RSNA_DO_NOT_SAVE_COLLISION );
	}
}

void CControlMediatorHelperResource::UpdateRefMesh()
{
	if( m_pRefActor_ && m_pRefActor_->GetVisible() )
	{
		RSceneNodeUpdateVisitor visitor;
		m_pRefActor_->Traverse(&visitor);
	}
}

void CControlMediatorHelperResource::ShowRefMeshToggle()
{
	static bool bShow = false;
	if(m_pRefActor_ == NULL || m_pControlMediator_->m_pCurrentSceneManager_ == NULL)
		return;

	bShow = !bShow;
	if(bShow)
	{
		//ResetRefMesh( CXMLUserConfig::GetReference().GetRefMeshName() );
		m_pControlMediator_->m_mouseInfo.setIgnoreMouseMovedPickingRActor( false );
		m_pControlMediator_->GetHelperAction()->SetMouseLineAndPickingPosition();
		const RVector3* pContactPos = m_pControlMediator_->GetHelperAction()->GetCurrentPickingPosition();
		if (!pContactPos)
		{
			bShow = false;
			AfxMessageBox("���� ���콺 ��ġ�� ���۷��� �޽ø� �ø��� �����ϴ�.");
			return;
		}

		m_pRefActor_->SetPosition( *pContactPos );

		RVector actorDirection = m_pControlMediator_->m_pCurrentCamera_->GetDirection();
		actorDirection.z = 0;
		m_pRefActor_->SetDirection(actorDirection);
		m_pRefActor_->SetVisible(true);

		UpdateRefMesh();
	}
	else
		m_pRefActor_->SetVisible(false);
}

void CControlMediatorHelperResource::DeleteTempFile()
{
	//temp ���� �����
	CFileFind finder;
	std::vector<std::string> tempFiles;
	std::string fileFilter(m_pControlMediator_->m_mapInfo.m_strScenePath + "_temp*");
	BOOL bWorking = finder.FindFile(fileFilter.c_str());
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		tempFiles.push_back(m_pControlMediator_->m_mapInfo.m_strScenePath + (LPCTSTR) finder.GetFileName());
	}

	for(std::vector<std::string>::iterator itr = tempFiles.begin();
		itr != tempFiles.end(); ++itr)
	{
		DeleteFile(itr->c_str());
	}
}

void CControlMediatorHelperResource::SortZoneXmlWithRuby(MXml& _refZoneXml)
{
	MXmlPrinter zonePrinter;
	_refZoneXml.Doc()->Accept(&zonePrinter);

	std::string strZoneXml(zonePrinter.CStr());
	CRubyManager::GetReference().CallRubyForSortingZoneXml(strZoneXml);

	_refZoneXml.Clear();
	_refZoneXml.LoadStream( strZoneXml.c_str() );
}

void CControlMediatorHelperResource::SortZoneXmlAndSave(const char* _pSzFileName, MXml& _refZoneXml)
{	
	MXml sortedXml;
	sortedXml.LoadStream("<?xml version=\"1.0\" encoding=\"UTF-8\" ?><ROOT><PROPERTY></PROPERTY><SCENE></SCENE></ROOT>");
	MXmlElement* pSortedSceneXml = sortedXml.Doc()->FirstChildElement("ROOT")->FirstChildElement("SCENE");

	MXmlElement* pRoot = _refZoneXml.Doc()->FirstChildElement("ROOT");
	if(pRoot == NULL) return;

	MXmlElement* pScene = pRoot->FirstChildElement("SCENE");
	if(pScene == NULL) return;

	//sort for aabb tree
	typedef std::map< std::string, MXmlElement* > NAME_ELEMENT_MAP;
	NAME_ELEMENT_MAP mapToSort;

	//sort for static mesh
	mapToSort.clear();
	MXmlElement* pStaticMesh = pScene->FirstChildElement(RSID_STATICMESHSCENENODE);
	while(pStaticMesh != NULL)
	{
		const char* szValue = pStaticMesh->FirstChildElement("COMMON")->Attribute("name");
		if (szValue)
		{
			mapToSort.insert( NAME_ELEMENT_MAP::value_type( szValue, pStaticMesh ) );
		}
		pStaticMesh = pStaticMesh->NextSiblingElement(RSID_STATICMESHSCENENODE);
	}
	for(NAME_ELEMENT_MAP::iterator itr = mapToSort.begin(); itr != mapToSort.end(); ++itr)
		pSortedSceneXml->InsertEndChild( *(itr->second) );

	//sort for terrain
	MXmlElement* pTerrain = pScene->FirstChildElement(RSID_TERRAINSCENENODE);
	while(pTerrain != NULL)
	{
		pSortedSceneXml->InsertEndChild( *pTerrain );
		pTerrain = pTerrain->NextSiblingElement(RSID_TERRAINSCENENODE);
	}

	//sort for occluder
	MXmlElement* pOccluder = pScene->FirstChildElement(RSID_OCCLUDER);
	while(pOccluder != NULL)
	{
		pSortedSceneXml->InsertEndChild( *pOccluder );
		pOccluder = pOccluder->NextSiblingElement(RSID_OCCLUDER);
	}

	//sort for direction light
	MXmlElement* pDirLight = pScene->FirstChildElement(RSID_DIRECTIONALLIGHTSCENENODE);
	while(pDirLight != NULL)
	{
		pSortedSceneXml->InsertEndChild( *pDirLight );
		pDirLight = pDirLight->NextSiblingElement(RSID_DIRECTIONALLIGHTSCENENODE);
	}

	//sort for sub light
	MXmlElement* pSubLight = pScene->FirstChildElement(RSID_SUBLIGHTSCENENODE);
	while(pSubLight != NULL)
	{
		pSortedSceneXml->InsertEndChild( *pSubLight );
		pSubLight = pSubLight->NextSiblingElement(RSID_SUBLIGHTSCENENODE);
	}

	//sort for spot light
	MXmlElement* pSpotLight = pScene->FirstChildElement(RSID_SPOTLIGHTSCENENODE);
	while(pSpotLight != NULL)
	{
		pSortedSceneXml->InsertEndChild( *pSpotLight );
		pSpotLight = pSpotLight->NextSiblingElement(RSID_SPOTLIGHTSCENENODE);
	}

	//sort for omni light
	MXmlElement* pPointLight = pScene->FirstChildElement(RSID_LIGHTSCENENODE);
	while(pPointLight != NULL)
	{
		pSortedSceneXml->InsertEndChild( *pPointLight );
		pPointLight = pPointLight->NextSiblingElement(RSID_LIGHTSCENENODE);
	}

	//sort for water plane
	mapToSort.clear();
	MXmlElement* pWaterPlane = pScene->FirstChildElement(RSID_WATERPLANE);
	while(pWaterPlane != NULL)
	{
		MXmlElement* pCommon = pWaterPlane->FirstChildElement("COMMON");
		if (pCommon)
		{
			const char* strName = pCommon->Attribute("name");

			if (!strName)
			{
				AfxMessageBox( "�߸��� �Ӽ��� ����带 �����Ϸ��մϴ�.");
			}
			else
			{
				mapToSort.insert( NAME_ELEMENT_MAP::value_type( strName, pWaterPlane ) );
			}
		}
		else
		{
			AfxMessageBox( "�߸��� �Ӽ��� ����带 �����Ϸ��մϴ�.");
		}
		pWaterPlane = pWaterPlane->NextSiblingElement(RSID_WATERPLANE);
	}
	for(NAME_ELEMENT_MAP::iterator itr = mapToSort.begin(); itr != mapToSort.end(); ++itr)
		pSortedSceneXml->InsertEndChild( *(itr->second) );

	//sort for actor
	mapToSort.clear();
	MXmlElement* pActor = pScene->FirstChildElement(RSID_ACTOR);
	while(pActor != NULL)
	{
		MXmlElement* pCommon = pActor->FirstChildElement("COMMON");
		if (pCommon)
		{
			const char* strName = pCommon->Attribute("name");
			if (!strName)
			{
				AfxMessageBox( "�߸��� �Ӽ��� ����带 �����Ϸ��մϴ�.");
			}
			else
			{
				mapToSort.insert( NAME_ELEMENT_MAP::value_type( strName, pActor ) );
			}
		}
		else
		{
			AfxMessageBox( "�߸��� �Ӽ��� ����带 �����Ϸ��մϴ�.");
		}
		pActor = pActor->NextSiblingElement(RSID_ACTOR);
	}
	for(NAME_ELEMENT_MAP::iterator itr = mapToSort.begin(); itr != mapToSort.end(); ++itr)
		pSortedSceneXml->InsertEndChild( *(itr->second) );

	// sort for scene instance
	mapToSort.clear();
	MXmlElement* pSceneNode = pScene->FirstChildElement(RSID_SCENEINSTANCE);
	while(pSceneNode != NULL)
	{
		MXmlElement* pCommon = pSceneNode->FirstChildElement("COMMON");
		if (pCommon)
		{
			const char* strName = pCommon->Attribute("name");
			if (!strName)
			{
				AfxMessageBox( "�߸��� �Ӽ��� ����带 �����Ϸ��մϴ�.");
			}
			else
			{
				mapToSort.insert( NAME_ELEMENT_MAP::value_type( strName, pSceneNode ) );
			}
		}
		else
		{
			AfxMessageBox( "�߸��� �Ӽ��� ����带 �����Ϸ��մϴ�.");
		}
		pSceneNode = pSceneNode->NextSiblingElement(RSID_SCENEINSTANCE);
	}
	for(NAME_ELEMENT_MAP::iterator itr = mapToSort.begin(); itr != mapToSort.end(); ++itr)
		pSortedSceneXml->InsertEndChild( *(itr->second) );

	//sort for tree
	mapToSort.clear();
	MXmlElement* pTree = pScene->FirstChildElement(RSID_TREE);
	while(pTree != NULL)
	{
		MXmlElement* pCommon = pTree->FirstChildElement("COMMON");
		if (pCommon)
		{
			const char* strName = pCommon->Attribute("name");
			if (!strName)
			{
				AfxMessageBox( "�߸��� �Ӽ��� ������Դϴ�.");
			}
			else
			{
				mapToSort.insert( NAME_ELEMENT_MAP::value_type( strName, pTree ) );
			}
		}
		else
		{
			AfxMessageBox( "�߸��� �Ӽ��� ������Դϴ�.");
		}
		pTree = pTree->NextSiblingElement(RSID_TREE);
	}
	for(NAME_ELEMENT_MAP::iterator itr = mapToSort.begin(); itr != mapToSort.end(); ++itr)
		pSortedSceneXml->InsertEndChild( *(itr->second) );

	sortedXml.SaveFile(_pSzFileName);
}

void CControlMediatorHelperResource::SetAutoAnimationProperty()
{
	class CAutoAnimationSetVisitor : public rs3::RSceneNodeVisitor
	{
	public:
		virtual bool OnPreOrderVisit( rs3::RSceneNode* pSceneNode ){ return true; }
		virtual void OnPostOrderVisit( rs3::RSceneNode* pSceneNode )
		{
			rs3::RActor* pActor = MDynamicCast(rs3::RActor, pSceneNode);
			if (pActor)
			{
				pActor->LoadAutoAnimation();
				if( pActor->m_pAutoAnimation && pActor->m_pAutoAnimation->GetResource()->GetState()==RRS_LOADED )
					pActor->m_bAutoAnimationEnable = true;
			}
		}
	} autoAnimationSetter;

	rs3::RToolSceneManager* pSceneManager = m_pControlMediator_->GetToolSceneManager();
	rs3::RSceneNode* pSceneNode = const_cast< rs3::RSceneNode* >( (RSceneNode*)&pSceneManager->GetRootSceneNode() );
	pSceneNode->Traverse(&autoAnimationSetter);
}

std::string CControlMediatorHelperResource::MakeServerZoneFile( const char* _pStrFileName )
{
	std::string strPureFileName = MGetRemovePathName(string(_pStrFileName));
	std::string serverFileName = CXMLConfig::GetInstance()->strServerZoneFullPath + strPureFileName;

	return serverFileName;
}

void CControlMediatorHelperResource::CopyToServerZoneFolder( const char* _pStrFileName )
{
	std::string strPureFileName = MGetRemovePathName(string(_pStrFileName));
	std::string serverFileName = CXMLConfig::GetInstance()->strServerZoneFullPath + strPureFileName;

	::CopyFile(_pStrFileName, serverFileName.c_str(), FALSE );
}

void CControlMediatorHelperResource::CopyToServerSystemFolder( const char* _pStrFileName )
{
	std::string strPureFileName = MGetRemovePathName(string(_pStrFileName));
	std::string serverFileName = CXMLConfig::GetInstance()->strServerPath + "system/" + strPureFileName;

	::CopyFile(_pStrFileName, serverFileName.c_str(), FALSE );
}

void CControlMediatorHelperResource::SetMediatorAfterSceneLoaded(MAP_EDIT_TYPE _eEditType)
{
	m_pControlMediator_->m_mapInfo.m_eMapEditType = MAP_EDIT_TYPE_SCENE;

	//terrain�� ������ ���� �ε��� / �ƿ���� ����
	if (_eEditType == MAP_EDIT_TYPE_SCENE)
	{
		if(m_pControlMediator_->m_pCurrentSceneManager_->GetCurrentTerrain())
		{
			SetMediatorWithLoadedTerrain_();
		}
		else
		{
			SetMediatorWithNoneTerrain_();
		}
	}
	else
	{
		m_pControlMediator_->m_mapInfo.m_eMapEditType = _eEditType;
		m_pControlMediator_->m_mapInfo.m_bEditSceneObject = false;
		m_pControlMediator_->m_mapInfo.m_bSceneEditableMap = false;
	}
}

bool CControlMediatorHelperResource::IsKeyEventAcceptableInNewMapDlg()
{
	int nSize = 0;
	CControlMediatorHelperNewMapResource** pNewMapHelper = m_pControlMediator_->GetWorkSpaceMgr()->GetHelperAllNewMapResources(nSize);
	for (int i = 0; i < nSize; ++i)
	{
		if (false == pNewMapHelper[i]->IsKeyEventHandlerAcceptable())
			return false;
	}

	return true;
}
