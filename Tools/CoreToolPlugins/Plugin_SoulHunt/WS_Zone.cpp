#include "stdafx.h"
#include "WS_Zone.h"
#include "WS_Field.h"
#include "WS_Scene.h"

#include "RCollisionTreeBuilder.h"
#include "M_CollisionTree.h"
#include "M_PathFinder.h"
#include "M_Visitors.h"
#include "MPathEngineExporter.h"
#include "../../../Server/GameServer/main/GPickingZTable.h"
#include "../../../Server/GameServer/main/GPathFinder.h"
#include "../../../Server/GameServer/main/GCollisionTree.h"


#include "VL_SceneNodeList.h"
#include "VR_TerrainHeight.h"
#include "VR_TerrainAverage.h"
#include "VR_TerrainFlat.h"
#include "VR_TerrainSplat.h"
#include "VR_TerrainRelax.h"
#include "VR_TerrainGrass.h"
#include "VR_ZoneObjectNew.h"
#include "VR_EntityObjectList.h"
#include "VR_EnvWeather.h"
#include "VR_EtcPathFind.h"
#include "VR_EtcCollision.h"
#include "VR_EtcCamera.h"
#include "VR_EtcMapView.h"
#include "VR_EtcDecal.h"
#include "VR_EtcDev.h"

#include "C_XMLResourceList.h"
#include "C_XMLConfig.h"

#include "C_ControlMediatorHelperResource.h"
#include "C_StatusBarHandler.h"
#include "M_ToolLogicObjectManager.h"

#include "MFileSystem.h"
#include <algorithm>

#include "C_ControlDoActionImpl.h"
#include "C_ControlMediatorHelperActionAddon.h"
#include "C_ControlMediatorHelperRenderAddon.h"
#include "C_ControlMediatorHelperResourceAddon.h"
#include "C_KeyEventHandler.h"

#include "C_ControlPopupActionImpl.h"

const char* CWorkSpaceZone::GetWorkSpaceKeyword()
{
	return "zone.xml";
}

void CWorkSpaceZone::InitWorkSpace(CWorkSpaceUiSetter& _rUiSetter, CWorkSpaceHelpersAndActions& _rHelperAndAction)
{
	// ����
	int nTerrainTabIdx = _rUiSetter.AddTab("����", WORKBENCH_STATE::TERRAIN_HEIGHT);
	_rUiSetter.AddRollup(nTerrainTabIdx, CRollupTerrainHeight::GetID(), true, WORKBENCH_STATE::TERRAIN_HEIGHT, true);
	_rUiSetter.AddRollup(nTerrainTabIdx, CRollupTerrainAverage::GetID(), false, WORKBENCH_STATE::TERRAIN_AVER, true);
	_rUiSetter.AddRollup(nTerrainTabIdx, CRollupTerrainFlat::GetID(), false, WORKBENCH_STATE::TERRAIN_FLAT, true);
	_rUiSetter.AddRollup(nTerrainTabIdx, CRollupTerrainSplat::GetID(), false, WORKBENCH_STATE::TERRAIN_SPLAT, true);
	_rUiSetter.AddRollup(nTerrainTabIdx, CRollupTerrainRelax::GetID(), false, WORKBENCH_STATE::TERRAIN_RELAX, true);
	_rUiSetter.AddRollup(nTerrainTabIdx, CRollupTerrainGrass::GetID(), false, WORKBENCH_STATE::TERRAIN_GRASS, true);

	_rUiSetter.BindWBState(WORKBENCH_STATE::TERRAIN_HEIGHT, nTerrainTabIdx, CRollupTerrainHeight::GetID());
	_rUiSetter.BindWBState(WORKBENCH_STATE::TERRAIN_AVER, nTerrainTabIdx, CRollupTerrainAverage::GetID());
	_rUiSetter.BindWBState(WORKBENCH_STATE::TERRAIN_FLAT, nTerrainTabIdx, CRollupTerrainFlat::GetID());
	_rUiSetter.BindWBState(WORKBENCH_STATE::TERRAIN_SPLAT, nTerrainTabIdx, CRollupTerrainSplat::GetID());
	_rUiSetter.BindWBState(WORKBENCH_STATE::TERRAIN_RELAX, nTerrainTabIdx, CRollupTerrainRelax::GetID());
	_rUiSetter.BindWBState(WORKBENCH_STATE::TERRAIN_GRASS, nTerrainTabIdx, CRollupTerrainGrass::GetID());

	// ��ü
	int nObjectTabIdx = _rUiSetter.AddTab("��ü", WORKBENCH_STATE::OBJECT_SELECT);
	_rUiSetter.AddRollup(nObjectTabIdx, CRollupZoneObjectNew::GetID(), true, WORKBENCH_STATE::OBJECT_NEW, false);

	_rUiSetter.BindWBState(WORKBENCH_STATE::OBJECT_NEW, nObjectTabIdx, CRollupZoneObjectNew::GetID());
	_rUiSetter.BindWBState(WORKBENCH_STATE::OBJECT_SELECT, nObjectTabIdx, CDlgSceneNodeList::GetID());
	_rUiSetter.BindWBState(WORKBENCH_STATE::OBJECT_MOVE, nObjectTabIdx, CDlgSceneNodeList::GetID());
	_rUiSetter.BindWBState(WORKBENCH_STATE::OBJECT_ROTATE, nObjectTabIdx, CDlgSceneNodeList::GetID());
	_rUiSetter.BindWBState(WORKBENCH_STATE::OBJECT_SCALE, nObjectTabIdx, CDlgSceneNodeList::GetID());


	// ȯ��
	int nEnvTabIdx = _rUiSetter.AddTab("ȯ��");
	_rUiSetter.AddRollup(nEnvTabIdx, CRollupEnvWeather::GetID(), true, WORKBENCH_STATE::STATE_NUM, false);

	// Entity
	int nEntityTabIdx = _rUiSetter.AddTab("Entity");
	_rUiSetter.AddRollup(nEntityTabIdx, CRollupEntityObjectList::GetID(), true, WORKBENCH_STATE::OBJECT_SELECT, false);

	// ��Ÿ
	int nEtcTabIdx = _rUiSetter.AddTab("��Ÿ", WORKBENCH_STATE::ETC_PATH_SELECT);
	_rUiSetter.AddRollup(nEtcTabIdx, CRollupEtcPathFind::GetID(), true, WORKBENCH_STATE::ETC_PATH_SELECT, true);
	_rUiSetter.AddRollup(nEtcTabIdx, CRollupEtcCollision::GetID(), true, WORKBENCH_STATE::ETC_PATH_SELECT, true);
	_rUiSetter.AddRollup(nEtcTabIdx, CRollupEtcCamera::GetID(), false, WORKBENCH_STATE::ETC_CAMERA, true);
	_rUiSetter.AddRollup(nEtcTabIdx, CRollupEtcMapView::GetID(), false, WORKBENCH_STATE::ETC_MAP_VIEW, true);
	_rUiSetter.AddRollup(nEtcTabIdx, CRollupEtcDev::GetID(), false, WORKBENCH_STATE::ETC_DEV, true);
	_rUiSetter.AddRollup(nEtcTabIdx, CRollupEtcDecal::GetID(), false, WORKBENCH_STATE::ETC_DECAL, true);
	
	_rUiSetter.BindWBState(WORKBENCH_STATE::ETC_PATH_SELECT, nEtcTabIdx, CRollupEtcPathFind::GetID());
	_rUiSetter.BindWBState(WORKBENCH_STATE::ETC_CAMERA, nEtcTabIdx, CRollupEtcCamera::GetID());
	_rUiSetter.BindWBState(WORKBENCH_STATE::ETC_MAP_VIEW, nEtcTabIdx, CRollupEtcMapView::GetID());
	_rUiSetter.BindWBState(WORKBENCH_STATE::ETC_DEV, nEtcTabIdx, CRollupEtcDev::GetID());
	_rUiSetter.BindWBState(WORKBENCH_STATE::ETC_DECAL, nEtcTabIdx, CRollupEtcDecal::GetID());

	// helpers and actions
	_rHelperAndAction.m_pDoActionObjectMove = new CDoActionObjectMoveImpl;
	_rHelperAndAction.m_pDoActionObjectNew = new CDoActionObjectNewImpl;
	_rHelperAndAction.m_pDoActionObjectRotate = new CDoActionObjectRotateImpl;
	_rHelperAndAction.m_pDoActionObjectScale = new CDoActionObjectScaleImpl;
	_rHelperAndAction.m_pDoActionObjectSelect = new CDoActionObjectSelectImpl;
	_rHelperAndAction.m_pDoActionEtcPathSelect = new CDoActionEtcPathSelectImpl;

	_rHelperAndAction.m_pEntityActionHelper = new CControlMediatorHelperEntityActionImpl(m_pControlMediator_);
	_rHelperAndAction.m_pNewMapResourceHelper = new CControlMediatorHelperNewMapResourceImpl(m_pControlMediator_, CControlMediatorHelperNewMapResourceImpl::E_ZONE);
	_rHelperAndAction.m_pObjectRenderHelper = new CControlMediatorHelperObjectRendererImpl;

	_rHelperAndAction.m_pPopupActionRepository = new CControlPopupActionRepository;
	class CCreateCl2Functor : public CCoreToolFunctor
	{
	public:
		CCreateCl2Functor(CControlMediator* _pControlMediator){ m_pControlMediator_ = _pControlMediator; }
		virtual ~CCreateCl2Functor(){}

		virtual bool Run()
		{
			CWorkSpaceField* pFieldWS = m_pControlMediator_->GetWorkSpacePtr<CWorkSpaceField>();
			if (pFieldWS)
				pFieldWS->CreateCollisionTreeBuild();

			return true;
		}

		virtual const char* GetFuncName()
		{
			return "�浹����(.cl2) ����";
		}

	private:
		CControlMediator* m_pControlMediator_;

	};
	_rHelperAndAction.m_pPopupActionRepository->AddManagedResNewFunctor(new CCreateCl2Functor(m_pControlMediator_));
	CRunGameFunctor* pRunGameFunctor = new CRunGameFunctor(m_pControlMediator_);
	_rHelperAndAction.m_pPopupActionRepository->AddManagedToolEtcFunctor(pRunGameFunctor);
	_rHelperAndAction.m_pKeyEventHandlerHelper = new CSoulHuntKeyboardHelper(pRunGameFunctor);


	class CCreateNaviMesh : public CCoreToolFunctor
	{
	public:
		CCreateNaviMesh(CControlMediator* _pControlMediator){ m_pControlMediator_ = _pControlMediator; }
		virtual ~CCreateNaviMesh(){}

		virtual bool Run()
		{
			CRollupEtcPathFind* pEtc = m_pControlMediator_->GetViewPtr<CRollupEtcPathFind>();

			if( pEtc )
				pEtc->OnBnClickedButtonPathCreate();

			return true;
		}

		virtual const char* GetFuncName()
		{
			return "�׺���̼� �޽� ����";
		}

	private:
		CControlMediator* m_pControlMediator_;
	};

	_rHelperAndAction.m_pPopupActionRepository->AddManagedResNewFunctor(new CCreateNaviMesh(m_pControlMediator_));
	//CRunGameFunctor* pRunGameFunctor2 = new CRunGameFunctor(m_pControlMediator_);
	//_rHelperAndAction.m_pPopupActionRepository->AddManagedToolEtcFunctor(pRunGameFunctor2);
	//_rHelperAndAction.m_pKeyEventHandlerHelper = new CSoulHuntKeyboardHelper(pRunGameFunctor2);
}

void CWorkSpaceZone::HaltWorkSpace( CWorkSpaceHelpersAndActions& _rHelperAndAction )
{
	SAFE_DELETE(_rHelperAndAction.m_pDoActionObjectMove);
	SAFE_DELETE(_rHelperAndAction.m_pDoActionObjectNew);
	SAFE_DELETE(_rHelperAndAction.m_pDoActionObjectRotate);
	SAFE_DELETE(_rHelperAndAction.m_pDoActionObjectScale);
	SAFE_DELETE(_rHelperAndAction.m_pDoActionObjectSelect);
	SAFE_DELETE(_rHelperAndAction.m_pDoActionEtcPathSelect);

	SAFE_DELETE(_rHelperAndAction.m_pEntityActionHelper);
	SAFE_DELETE(_rHelperAndAction.m_pNewMapResourceHelper);
	SAFE_DELETE(_rHelperAndAction.m_pObjectRenderHelper);
	SAFE_DELETE(_rHelperAndAction.m_pKeyEventHandlerHelper);

	SAFE_DELETE(_rHelperAndAction.m_pPopupActionRepository);
}

void CWorkSpaceZone::OnRollupExpandClicked(CListenableView* pRollup)
{
	if (pRollup == m_pControlMediator_->GetViewPtr<CRollupEntityObjectList>())
	{
		m_pControlMediator_->m_mapInfo.m_bEditSceneObject = false;
	}
	else
	{
		m_pControlMediator_->m_mapInfo.m_bEditSceneObject = true;
	}
}

void CWorkSpaceZone::OnWBObjectEditMouseMessage(VIEW_MESSAGE::ID message)
{
	m_pControlMediator_->GetViewPtr<CDlgSceneNodeList>()->Listen( CoreMessage(message) );
}

MAP_EDIT_TYPE CWorkSpaceZone::OnOpenFile( const char* _pSzFileName )
{
	//�� �ε�
	MAP_EDIT_TYPE editType = m_pControlMediator_->LoadZone(_pSzFileName, MAP_EDIT_TYPE_SCENE, true);
	if( editType != MAP_EDIT_TYPE_ERROR)
	{
		LoadZoneCommonXmlAndNavigationMesh(_pSzFileName);
	}

	return editType;
}

void CWorkSpaceZone::OnSceneSaved( const char* _filename )
{
	// zone ���� �������� ����
	// by pok. make cl2 �Ҷ� ����ǰ� ����. �׷����� server zone resource builder�� �ùٷ� �����Ѵ�.
	//CControlMediatorHelperResource::CopyToServerZoneFolder(_filename);

	//////////////////////////////////////////////////////////////////////////
	// �� �ε��� ������ ��ȯ�Ҽ� �ֵ��� max script export
	//
	string strMaxScriptName = MGetPurePath( _filename ) + MGetPureFileName( _filename ) + ".ms";
	m_pControlMediator_->GetToolSceneManager()->ExportMaxScript(strMaxScriptName.c_str());

	// ���� baked map ����
	RToolTerrain* pTerrain = (RToolTerrain*)m_pControlMediator_->GetToolSceneManager()->GetCurrentTerrain();
	if( pTerrain )
		pTerrain->RecreateAndSaveBakedMap();

	// dds����
	// by pok. make cl2 �Ҷ� ����ǰ� ����. �׷����� server zone resource builder�� �ùٷ� �����Ѵ�.
	//std::string strDDSFileName = m_pControlMediator_->m_mapInfo.m_strScenePath + MGetPureFileName(MGetPureFileName(m_pControlMediator_->m_mapInfo.m_strSceneFile)) + "_dis.dds";
	//CControlMediatorHelperResource::CopyToServerZoneFolder(strDDSFileName.c_str());

	// Common XML
	CWorkSpaceField* pFieldWS = m_pControlMediator_->GetWorkSpacePtr<CWorkSpaceField>();
	if (pFieldWS)
		pFieldWS->SaveCommonEntityXML(_filename, true);

	// Resource XML (res.xml)
	string strRestListName = MGetPurePath( _filename ) + MGetPureFileName(MGetPureFileName( _filename )) + ".res.xml";
	m_pControlMediator_->GetToolSceneManager()->SaveResourceListToXML(strRestListName.c_str());
}

class CSaveAsZoneHelper
{
public:
	void AddExceptExt(const char* _pSzExt)
	{
		exceptExtLists_.push_back(_pSzExt);
	}

	void CopyForSaveAs(const std::string& _orgZoneName, const std::string& _saveAsZoneName)
	{
		std::string zone_ext(".zone.xml");
		WIN32_FIND_DATA ffd;
		HANDLE hFind = INVALID_HANDLE_VALUE;

		size_t org_dim = _orgZoneName.rfind("\\") + 1;
		std::string org_dir = _orgZoneName.substr(0, org_dim);
		std::string org_zone_name = _orgZoneName.substr(org_dim, _orgZoneName.length() - org_dim - zone_ext.length());
		std::transform(org_zone_name.begin(), org_zone_name.end(), org_zone_name.begin(), towlower);

		size_t save_as_dim = _saveAsZoneName.rfind("\\") + 1;
		std::string save_as_dir = _saveAsZoneName.substr(0, save_as_dim);
		std::string save_as_name = _saveAsZoneName.substr(save_as_dim, _saveAsZoneName.length() - save_as_dim - zone_ext.length());

		hFind = FindFirstFile( (org_dir+"*").c_str(), &ffd);
		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			}
			else
			{
				std::string strFromFileName(ffd.cFileName);
				std::string strToFiileName(strFromFileName);

				bool bExcept = false;
				for (std::vector<std::string>::iterator itr = exceptExtLists_.begin(); itr != exceptExtLists_.end(); ++itr)
				{
					if (strFromFileName.find(*itr) != std::string::npos)
					{
						bExcept = true;
						break;
					}
				}

				if (!bExcept)
				{
					std::transform(strFromFileName.begin(), strFromFileName.end(), strFromFileName.begin(), towlower);
					size_t pos = strFromFileName.find(org_zone_name);

					if (pos == 0)
					{
						strToFiileName = save_as_name + strFromFileName.substr(org_zone_name.length());
					}

					::CopyFile( (org_dir+strFromFileName).c_str(), (save_as_dir+strToFiileName).c_str(), FALSE );
				}
			}
		}
		while (FindNextFile(hFind, &ffd) != 0);

		FindClose(hFind);

		//std::string zone_ext(".zone.xml");
		//for (std::vector<std::string>::iterator itr = extLists_.begin(); itr != extLists_.end(); ++itr)
		//{
		//	std::string from_name = _orgZoneName.substr(_orgZoneName.find(zone_ext), zone_ext.length()) + (*itr);
		//	std::string to_name = _saveAsZoneName.substr(_saveAsZoneName.find(zone_ext), zone_ext.length()) + (*itr);
		//	::CopyFile(from_name.c_str(), to_name.c_str(), FALSE );
		//}
	}

private:
	std::vector<std::string> exceptExtLists_;
};

/// �����ϱ� ���� dirlight �� �ٲ��ش� true �̸� save , false �̸� restore
void CWorkSpaceZone::SaveRestoreDirectionalLight(rs3::RDirectionalLightSceneNode* pLight, bool bSave)
{
	if( pLight == NULL ) return;

	static rs3::RVector4 oldDiffuse, oldAmbient;
	static float oldLightPower;
	static rs3::RVector3 oldSky, oldGround;

	if( bSave )
	{
		oldDiffuse = pLight->GetDiffuse();
		oldAmbient = pLight->GetAmbient();
		oldLightPower = pLight->GetPower();
		oldSky = pLight->GetSkyColor();
		oldGround = pLight->GetGroundColor();

		// default light �⺻������ ����
		pLight->SetLightDiffuse(1,1,1,1);
		pLight->SetLightAmbient(0,0,0,0);
		pLight->SetPower(1.f);
		pLight->SetSkyColor( rs3::RVector(0,0,0) );
		pLight->SetGroundColor( rs3::RVector(0,0,0) );
	}else
	{
		pLight->SetLightDiffuse(oldDiffuse);
		pLight->SetLightAmbient(oldAmbient);
		pLight->SetPower(oldLightPower);
		pLight->SetSkyColor(oldSky);
		pLight->SetGroundColor(oldGround);
	}
}

std::string CWorkSpaceZone::OnSaveFile(const char* _pSzFileName, bool bSaveAs)
{
	rs3::RToolSceneManager *pSceneManager = m_pControlMediator_->GetToolSceneManager();
	rs3::RDirectionalLightSceneNode* pLight = pSceneManager->GetDefaultLight();

	// zone �� ȯ�濡 ���� ���� ������ �ٲ�� ������ �׻� �⺻������ �����Ѵ�
	SaveRestoreDirectionalLight( pLight, true );

	CStatusBarHandler::GetInstance()->ShowStatusBarMessage("�� ������");
	std::string strFileName = m_pControlMediator_->m_mapInfo.m_strScenePath + m_pControlMediator_->m_mapInfo.m_strSceneFile;

	if (bSaveAs)
	{
		CSaveAsZoneHelper helper;
		helper.CopyForSaveAs(strFileName, _pSzFileName);

		strFileName = _pSzFileName;
	}

	m_pControlMediator_->SaveZone(strFileName.c_str(), true, bSaveAs);

	// ���� ���� ����
	SaveRestoreDirectionalLight( pLight, false );

	m_pControlMediator_->GetViewPtr<CRollupEtcPathFind>()->OnZoneSaved();
	return strFileName;
}

void CWorkSpaceZone::LoadZoneCommonXmlAndNavigationMesh( const char* _pSzZoneName )
{
	// zone common.xml (Common Entity) �б�
	CWorkSpaceField* pFieldWS = m_pControlMediator_->GetWorkSpacePtr<CWorkSpaceField>();
	if (pFieldWS)
	{
		std::string strZoneName(_pSzZoneName);
		size_t nPos = strZoneName.find(".zone.xml");
		if (nPos != std::string::npos )
		{
			strZoneName.replace(nPos, 11, ".common.xml");
			bool bZoneEditMode = m_pControlMediator_->m_mapInfo.m_bSceneEditableMap;
			if( pFieldWS->LoadCommonEntityXML(strZoneName.c_str(), bZoneEditMode) )
			{
				if (bZoneEditMode)
					m_pControlMediator_->GetViewPtr<CRollupEntityObjectList>()->UpdateListBox(true, false);
			}
			else
			{
				mlog("zone�� common.xml �� �о���� ���߽��ϴ�.\n");
			}
		}
	}

	// Load Navigation mesh
	CString strFilename = _pSzZoneName;
	strFilename.Replace(".zone.xml","");

	g_PathfinderTest.Release();

	// iTerrainCallback ���.
	RToolSceneManager* pManager = m_pControlMediator_->GetToolSceneManager();
	_ASSERT(pManager);

	RTerrain *pTerrain = pManager->GetCurrentTerrain();
	RHeightField *pHeightField = (pTerrain!=NULL ? pTerrain->GetHeightFieldPtr() : NULL);

	if (g_PathfinderTest.LoadZone(strFilename.GetString(), pHeightField))
	{
		g_PathfinderTest.m_arrayInvalidPosition.clear();
		g_PathfinderTest.MakeNavMesh();
	}
	else
	{
		pManager->AddErrorMessage("Navigation mesh error");
		//MessageBox(NULL, "Navigation mesh error", "ERROR", MB_OK);
	}
}

void CWorkSpaceZone::OnFileModified( const string& strFileName )
{
	// WS Scene �� �ִ� ���� DLL �� �޶� static ������ ����ϴ� 
	// m_pControlMediator_->GetWorkSpacePtr< CWorkSpaceScene > �Լ��� 
	// WS Scene �� �������� ��� down cast

	CWorkSpace* pWorkSpace = m_pControlMediator_->GetWorkSpaceMgr()->GetWorkSpace("scene.xml");
	
	CWorkSpaceScene* pWorkSpaceScene = dynamic_cast < CWorkSpaceScene* > ( pWorkSpace );
	if( pWorkSpaceScene )
		pWorkSpaceScene->OnFileModified(strFileName);
}