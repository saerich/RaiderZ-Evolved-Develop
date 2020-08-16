#include "stdafx.h"
#include "RMeshMgr.h"
#include "C_ControlMediator.h"
#include "C_ControlDoAction.h"
#include "C_XMLConfig.h"
#include "C_XMLXConst.h"
#include "C_XMLEditInfoFile.h"
#include "C_StatusBarHandler.h"
#include "C_XMLResourceList.h"

//#include "CoreToolRS3.h"
#include "RApplication.h"
#include "RTerrain.h"
#include "RConfiguration.h"
#include "M_ToolActor.h"
#include "M_ToolTerrain.h"

#include "RUtil.h"
#include "RCollisionTreeBuilder.h"

#include "RSpeedGrass.h"
//#include "RSceneCloud.h"
#include "RRenderHelper.h"
#include "REnvironmentSet.h"

//manager or helper
#include "C_ControlMediatorHelperResource.h"
#include "C_ControlMediatorHelperRender.h"
#include "C_ControlMediatorHelperAction.h"
#include "C_MintManager.h"
#include "M_ToolCmdMgr.h"
#include "M_ToolLogicObjectManager.h"

#include "RTreeResourceMgr.h"

#include "RTreePassRenderer.h"
#include "RWaterPassRenderer.h"
#include "MProfiler.h"
#include "RProfiler.h"

#include <string>

#include "WS_WorkSpaceManager.h"
#include "VR_EtcCollision.h"

//#include "WS_CutScene.h"

using namespace rs3;
CControlMediator::CControlMediator()
{
	//�����̳� �ʱ�ȭ
	ZeroMemory(WB_ActionTable_,  sizeof(WB_ActionTable_));

	//ActionTable �ʱ�ȭ
	CControlDoAction::InitActionTable(WB_ActionTable_);
	m_bNavigation_ = false;
	m_bDeviceInited_ = false;

	//�ڿ� �ʱ�ȭ
	m_pCurrentDevice_ = NULL;
	m_pCurrentCamera_ = NULL;
	m_pCurrentSceneManager_ = NULL;
	m_pEnvironmentSet_ = NULL;

	m_wbCurrentState_ = WORKBENCH_STATE::ETC_DEV;
	m_wbPreviousState_ = WORKBENCH_STATE::ETC_DEV;
	m_nEditMode = EDITMODE_NONE;

	//mouse �ʱ�ȭ
	m_mouseInfo.mouseState.bDrag = false;
	m_mouseInfo.mouseState.bMDrag = false;
	m_mouseInfo.mouseState.bLButton = false;
	m_mouseInfo.mouseState.bMButton = false;
	m_mouseInfo.mouseState.bRButton = false;
	m_mouseInfo.mouseState.bEvented = false;
	m_mouseInfo.setIgnoreMouseMovedPickingRActor( false );
	m_mouseInfo.m_nWheelDelta = 0;
	m_mouseInfo.m_bPopAble  = true;
	m_mouseInfo.m_bCaptured = false;

	//helper / manager class
	m_pCmdMgr_ = new CToolCmdMgr;
	m_pLogicObjectMgr_ = new CToolLogicObjectManager(this);
	m_pWorkSpaceMgr_ = new CWorkSpaceManager(this);
	m_pWorkSpaceMgr_->LoadPlugins();

	m_pHelperResource_ = new CControlMediatorHelperResource;
	m_pHelperAction_ = new CControlMediatorHelperAction;
	m_pHelperRender_ = new CControlMediatorHelperRender;
	
	// ���� ���� ����
	m_pFileWatcher = new FileWatcher("../../Data");
	m_pFileWatcher->InstallChangedFileSetCallback( fastdelegate::MakeDelegate( this, &CControlMediator::Report ) );
}

CControlMediator::~CControlMediator()
{
	SAFE_DELETE(m_pCmdMgr_);
	SAFE_DELETE(m_pLogicObjectMgr_);
	SAFE_DELETE(m_pWorkSpaceMgr_);

	SAFE_DELETE(m_pHelperResource_);
	SAFE_DELETE(m_pHelperAction_);
	SAFE_DELETE(m_pHelperRender_);
	SAFE_DELETE(m_pFileWatcher);
}

bool CControlMediator::CreateWBDeviceAndNotify(UINT nWidth, UINT nHeight, HWND _hWnd, UINT _workTime)
{
	ASSERT(m_pCurrentDevice_==NULL);
	if (nWidth == 0) nWidth = 640;
	if (nHeight == 0) nHeight = 480;

	//workTime ����
	int worktime = 100/_workTime;
	ELAPSE_IN_TIMER = static_cast<DWORD>(worktime);

	if ( REngine::GetConfig().LoadFromXMLFile(FILENAME_CONFIG) == false )
	{
		MessageBox(NULL,"REngine::GetConfig().xml ������ ���� �� �����ϴ�.", "Error", MB_OK);
		return false;
	}

	//set device configuration to create in config
	REngine::GetConfig().m_nWidth = nWidth;
	REngine::GetConfig().m_nHeight = nHeight;
	REngine::GetConfig().m_bFullScreen = false;
	REngine::GetConfig().pixelFormat = RFMT_ARGB32;
	REngine::GetConfig().depthFormat = RFMT_D24S8;

	//set config from config file
	if ( CXMLConfig::GetInstance()->LoadXML(FILENAME_CONFIG) == false )
	{
		MessageBox(NULL,"REngine::GetConfig().xml ������ ���� �� �����ϴ�.", "Error", MB_OK);
		return false;
	}

	// game const ���� �б�
	if (XCONST::Load("../../Data/Zone/Zone_Utils/const.xml") == false)
	{
		mlog("Failed To Load const.xml\n");
	}

	// �о� �����Ŀ� ���� ������ ����
	m_pCmdMgr_->InitCommandManager(this);
	m_pHelperResource_->InitMediatorHelper_(this);
	m_pHelperAction_->InitHelperAction(this);
	m_pLogicObjectMgr_->LoadTablesSafely();

	// 3D ��ġ ����� --
	m_pCurrentDevice_ = new RDeviceD3D;
	if ( !m_pCurrentDevice_->Create( _hWnd ))
	{
		AfxMessageBox("Device ��ü ������ �����߽��ϴ�.");
		SAFE_DELETE(m_pCurrentDevice_);
		return false;
	}

	REngine::RegisterDevice(m_pCurrentDevice_);

	// �������� ����̽��� �⺻������ ���̴� ����
	REngine::GetConfig().m_bUsingShader = true;
	m_pCurrentDevice_->SetLighting(false);

	// �����ϵ� ���̴� ���
	if( REngine::IsDevelopVersion() == false )
	{
		REngine::GetConfig().m_bUsingCompiledShader = true;
		REngine::GetConfig().m_bUsingCompiledShaderCaching = true;
		REngine::GetConfig().m_strCompiledShaderCachePath = "Caching/";
	}

	// �� �Ŵ��� ����� --
	_ASSERT( m_pCurrentSceneManager_ == NULL );
	m_pCurrentSceneManager_ = new RToolSceneManager(this);
	REngine::RegisterSceneManager(m_pCurrentSceneManager_);
	if ( m_pCurrentSceneManager_->Create(m_pCurrentDevice_) == false )
	{
		mlog("���Ŵ��� ���� ����\n");
		DestoryResource();
		DestroyDevice();
		return false;
	}
	if(!REngine::InitSharedResource())
	{
		AfxMessageBox("���� ���ҽ� ( ���̴� �� ) ������ �����߽��ϴ�.");
		DestoryResource();
		DestroyDevice();
		return false;
	}

	// ȯ�� ����
	ASSERT( m_pEnvironmentSet_ == NULL );
	m_pEnvironmentSet_ = new rs3::REnvironmentSet;
	m_pCurrentSceneManager_->AddSceneNode(m_pEnvironmentSet_);
	m_pEnvironmentSet_->AddAttribute( RSNA_DO_NOT_SAVE );

	//ī�޶� ���� / �����
	m_cameraInfo.ROTATE_STEP = CXMLConfig::GetInstance()->fRotateStep;
	m_cameraInfo.MOVE_STEP   = CXMLConfig::GetInstance()->fPanStep;
	m_cameraInfo.KEY_STEP    = CXMLConfig::GetInstance()->fVelocity;
	m_cameraInfo.WHEEL_ZOOM_STEP   = CXMLConfig::GetInstance()->fWheelZoomFactor;

	m_pCurrentCamera_ = new rs3::RToolCamera;
	m_pCurrentCamera_->SetClearColor(D3DCOLOR_XRGB( 35,135,195)); //�⺻����
	m_pCurrentCamera_->SetNearFarZ( 5.0f, CXMLConfig::GetInstance()->fMaxFarZ);
	m_pCurrentCamera_->AddAttribute( RSNA_DO_NOT_SAVE );

	//Distortion Ȱ��ȭ
	m_pCurrentSceneManager_->SetDistortionEnable(true);


	// PassRenderer ���� --
	REngine::GetSceneManager().GetPassRenderer< rs3::RTreePassRenderer >()->SetTreeLOD(XCONST::TREE_LOD_NEAR, XCONST::TREE_LOD_FAR);
	
	m_pCurrentSceneManager_->AddSceneNode(m_pCurrentCamera_);
	m_pCurrentSceneManager_->SetPrimaryCamera(m_pCurrentCamera_);
	m_pCurrentCamera_->SetPosAndTarget(rs3::RVector(120, -120, 120), rs3::RVector(0, 0, 0), true);

	//��ũ��ġ �ʱ�ȭ
	ResizeWBDevice(nWidth, nHeight);
	m_wbhWnd_ = _hWnd;

	// ����̽� ���� Ŭ������ �ʱ�ȭ
	m_pHelperRender_->InitHelperRender(this, nWidth, nHeight);
	CMintManager::GetReference().InitMintManager( this );

	// �ʱ� ������Ʈ
	RunRS3();

	//��ġ�� ����������� �信 �˸�
	BroadCastMessage(VIEW_MESSAGE::DEVICE_CREATED);

	m_bDeviceInited_ = true;

	GetGPUProfiler().Create();

	return true;
}
void CControlMediator::RunRS3()
{
	
	if( m_pCurrentSceneManager_ == NULL )
		return;

	PFC_B("CControlMediator::RunRS3()");
	// ����̽� ���� �˻�
	ASSERT(m_pCurrentDevice_);
	static RDEVICESTATUS lastDeviceStatus = RDEVICE_OK;
	RDEVICESTATUS deviceStatus = m_pCurrentDevice_->QueryStatus();
	if(deviceStatus==RDEVICE_LOST)
	{
		lastDeviceStatus = deviceStatus;
		return;
	}
	else if(deviceStatus==RDEVICE_RESTORED)
	{
		m_pCurrentDevice_->ResetDevice();
	}

	//pause Ȯ��
	if(m_pHelperAction_->IsPause())
	{
		m_pHelperRender_->RenderPause();
		return;
	}

	// update
	m_pCurrentSceneManager_->UpdateTimer();
	if(m_pWorkSpaceMgr_->GetCurrentWorkSpace() != NULL)
	{
		m_pWorkSpaceMgr_->GetCurrentWorkSpace()->OnBetweenCoreToolUpdateAndRS3Render();
	}

	GetHelperAction()->UpdateSelectedObjects();

	m_pCurrentSceneManager_->Update();

	// file ���� ����
	m_pFileWatcher->Update();

	// camera update
	RSceneNodeUpdateVisitor updateVisitor;
	m_pCurrentCamera_->Traverse(&updateVisitor);
	m_pCurrentCamera_->UpdateUserClipPlane(true, REngine::GetConfig().m_bUsingShader);

	// ref mesh
	m_pHelperResource_->UpdateRefMesh();

	// render
	m_pCurrentDevice_->Clear(true, true, true, D3DCOLOR_XRGB(35,135,195), 1.0f, 0L);
	m_pCurrentDevice_->BeginScene();

	m_pCurrentSceneManager_->RenderPrimaryCamera();

	m_pCurrentCamera_->UpdateUserClipPlane(false);
	m_pHelperRender_->OnDrawHelperRender();

	m_pCurrentDevice_->EndScene();
	m_pCurrentDevice_->Flip();

	m_pHelperRender_->OnUpdateHelperRender();

	m_DebuggingInfo.SendOut(m_wbhWnd_);
	GetProfileManager().ResetCurrentInfo();

	PFC_E;
}

void CControlMediator::ResizeWBDevice(UINT _w, UINT _h)
{
	m_wbWidth_ = _w;
	m_wbHeight_ = _h;

	float ratio = (float)_w/(float)_h;
	if ( m_pCurrentDevice_ != NULL )
	{
		REngine::GetConfig().m_nWidth = (int)_w;
		REngine::GetConfig().m_nHeight = (int)_h;

		CWorkSpace* pWS = GetWorkSpaceMgr()->GetCurrentWorkSpace();
		pWS->OnLostDevice();
		m_pCurrentDevice_->ResetDevice();
		pWS->OnRestoreDevice();
		
		if(m_pCurrentCamera_)
			m_pCurrentCamera_->SetFov( DegreeToRadian((float)XCONST::CAMERA_FOV), ratio);
		
		CMintManager::GetReference().SetWorkspaceSize(m_wbWidth_, m_wbHeight_);
	}
}

MAP_EDIT_TYPE CControlMediator::LoadZone(const char* _pStrName, MAP_EDIT_TYPE _wantedMapType, bool _bWithRelatedConfig)
{
	ASSERT(m_pCurrentSceneManager_);

	//set zone info
	char sz_path[128];
	char sz_file[128];
	sz_path[0] = '\0';
	sz_file[0] = '\0';
	SplitPath(_pStrName, sz_path, sz_file);
	m_mapInfo.m_strScenePath = sz_path;
	m_mapInfo.m_strSceneFile = sz_file;
	m_mapInfo.m_strImportSceneFile.clear();

	DWORD dwStartTime = GetTickCount();

	// �ε��Ǳ�����, ������ �޵𿡵��ʹ� ���̾����ɷ� ����
	m_pHelperResource_->SetMediatorAfterSceneLoaded(MAP_EDIT_TYPE_SCENE);
	if(LoadSceneAndEnvFromXML_(_pStrName, _wantedMapType) == false)
	{
		mlog("Environment .xml �б� ����\n");
		
		m_mapInfo.m_strScenePath = "";
		m_mapInfo.m_strSceneFile = "";

		m_mapInfo.m_eMapEditType = MAP_EDIT_TYPE_ERROR;

		mlog("zone loading time : %6.2f sec\n", (float)(GetTickCount() - dwStartTime)/1000.f);
		return m_mapInfo.m_eMapEditType;
	}

	mlog("zone loading time : %6.2f sec\n", (float)(GetTickCount() - dwStartTime)/1000.f);

	//config ���� �ε�����, �޵𿡵��� ������ ������ ����
	m_pHelperResource_->SetMediatorAfterSceneLoaded(MAP_EDIT_TYPE_SCENE);
	if(_bWithRelatedConfig)
		m_pHelperResource_->ManageConfigFileRelatedMap(_pStrName, CControlMediatorHelperResource::MAP_CONFIG_LOAD, true);

	// map info ����
	if (_wantedMapType != MAP_EDIT_TYPE_SCENE)
		m_pHelperResource_->SetMediatorAfterSceneLoaded(_wantedMapType);

	// ���� �ε��Ǿ��ٰ� view���� �˸���
	// m_pEffect�� V_EnvWeather���� Load�ȴ�.
	BroadCastMessage(VIEW_MESSAGE::SCENE_LOADED);
	return m_mapInfo.m_eMapEditType;
}

void CControlMediator::SaveZone(const char* _filename, bool _bSort, bool _bSaveAs)
{
	ASSERT(m_pCurrentSceneManager_);
	if(!m_mapInfo.m_bLoaded)
		return;

	std::string strFileName;
	if(_filename == NULL)
		strFileName = m_mapInfo.m_strScenePath + m_mapInfo.m_strSceneFile;
	else
	{
		strFileName = _filename;
		if( _bSaveAs )
		{
			char sz_path[128];
			char sz_file[128];
			sz_path[0] = '\0';
			sz_file[0] = '\0';
			SplitPath(_filename, sz_path, sz_file);
			m_mapInfo.m_strScenePath = sz_path;
			m_mapInfo.m_strSceneFile = sz_file;
		}
	}
	const char* filename = strFileName.c_str();

	// �ϴ��� �ƿ���� XML ����
	if(_bSort && m_mapInfo.m_eMapEditType == MAP_EDIT_TYPE_SCENE && m_pCurrentSceneManager_->GetCurrentTerrain())
	{
		MXml xml;
		m_pCurrentSceneManager_->SaveRootSceneToXML(filename, xml);
		m_pHelperResource_->SortZoneXmlAndSave(filename, xml);
	}
	else
		m_pCurrentSceneManager_->SaveRootSceneToXML(filename);

	// Env���� ����
	ASSERT(m_pEnvironmentSet_);
	char szBuffer[MAX_PATH];
	szBuffer[0] = '\0';
	strcpy(szBuffer, filename);
	char *p = strrchr(szBuffer,'.');
	strcpy(p,".env.xml");
	m_pEnvironmentSet_->SaveToFile(szBuffer);

	// WorkSpace�� �뺸, ����� zone workspace��
	CWorkSpace* pZoneWorkSpace = m_pWorkSpaceMgr_->GetWorkSpace("zone.xml");
	if (pZoneWorkSpace)
	{
		pZoneWorkSpace->OnSceneSaved(filename);
	}

	//config file save
	if( _bSaveAs )
		m_pHelperResource_->ManageConfigFileRelatedMap(filename, CControlMediatorHelperResource::MAP_CONFIG_SAVE_AS, true);
	else
		m_pHelperResource_->ManageConfigFileRelatedMap(filename, CControlMediatorHelperResource::MAP_CONFIG_SAVE, true);
}

MAP_EDIT_TYPE CControlMediator::LoadWorkSpaceFile(const char* _pStrName, const std::string& _rWsKeyword)
{
	ASSERT(m_pCurrentSceneManager_);
	CWorkSpace* pWorkSpace = m_pWorkSpaceMgr_->GetWorkSpace(_rWsKeyword);
	_ASSERT(pWorkSpace);

	if (_rWsKeyword == "field.xml")		m_nEditMode = EDITMODE_FIELD;
	else if (_rWsKeyword == "zone.xml")	m_nEditMode = EDITMODE_ZONE;
	else	
	{
		//	ASSERT(false && "unknown ws keyword");
	}

	// set helpers
	CControlMediatorHelperEntityAction* pEntityActionHelper = m_pWorkSpaceMgr_->GetHelperEntityAction();
	CControlMediatorHelperObjectRenderer* pObjectRenderHelper = m_pWorkSpaceMgr_->GetHelperObjectRenderer();
	m_pWorkSpaceMgr_->SetHelperEntityAction( pWorkSpace->m_WorkspaceHelpersAndActions_.m_pEntityActionHelper );
	m_pWorkSpaceMgr_->SetHelperObjectRenderer( pWorkSpace->m_WorkspaceHelpersAndActions_.m_pObjectRenderHelper );

	MAP_EDIT_TYPE mapOpenResult = pWorkSpace->OnOpenFile(_pStrName);

	// reset helpers
	m_pWorkSpaceMgr_->SetHelperEntityAction( pEntityActionHelper );
	m_pWorkSpaceMgr_->SetHelperObjectRenderer( pObjectRenderHelper );

	return mapOpenResult;
}

std::string CControlMediator::SaveWorkSpaceFile(const char* _pSzFileName, bool bSaveAs)
{
	BroadCastMessage( VIEW_MESSAGE::PRE_SAVE_DOCUMENT );

	std::string strFileName = m_pWorkSpaceMgr_->GetCurrentWorkSpace()->OnSaveFile(_pSzFileName, bSaveAs);

	BroadCastMessage( VIEW_MESSAGE::POST_SAVE_DOCUMENT );

	return strFileName;
}

void CControlMediator::CreateZone()
{
	if(m_pCurrentSceneManager_ == NULL)
	{
		AfxMessageBox("�� ���� �����ϱ� ���� - �� �Ŵ����� �����ϴ�.");
		return;
	}

	// ȯ�� ����
	ASSERT(m_pEnvironmentSet_);
	m_pEnvironmentSet_->CreateEnvironment("default");
	
	// �ƿ����� ���� ���
	if( m_mapInfo.m_strImportSceneFile.empty() )
	{
		// ��������
		rs3::RToolTerrain* pTerrain = static_cast<RToolTerrain*>(m_pCurrentSceneManager_->CreateSceneNode(RSID_TERRAINSCENENODE));
		m_pCurrentSceneManager_->AddSceneNode(pTerrain, true);
		m_pCurrentSceneManager_->AttachLogicObject( pTerrain );

		// �⺻ ����Ʈ��
		rs3::RDirectionalLightSceneNode *pLightNode = new RDirectionalLightSceneNode();
		pLightNode->SetNodeName("TerrainLightMap0");
		_ASSERT( pLightNode->GetUserData() == NULL );
		m_pCurrentSceneManager_->AttachLogicObject( pLightNode );

		m_pCurrentSceneManager_->AddSceneNode(pLightNode, true);

		char szName[MAX_PATH];
		szName[0] = '\0';

		SplitPath(m_mapInfo.m_strSceneFile.c_str(), NULL, szName);
		if ( char *p = strchr(szName,'.'))
			*p = '\0';

		// create new terrain
		if(!pTerrain->CreateNewTerrain(m_mapInfo.m_strScenePath.c_str(), szName, m_mapInfo.m_strTerrainBaseTile.c_str(), m_mapInfo.m_colFace + 1, m_mapInfo.m_rowFace + 1, m_mapInfo.m_width, m_mapInfo.m_height))
		{
			AfxMessageBox("���� ������ �����߽��ϴ�.");
			return;
		}
	}
	else // INDOOR_ZONE
	{
		// �̰� �پ��־�� editable �ϰ� scene �� �ε��Ѵ�
		m_mapInfo.m_bEditSceneObject = true;	///< TODO: �̰� �� �����׿�

		m_pCurrentSceneManager_->LoadEditableSceneXML(m_mapInfo.m_strImportSceneFile.c_str());

		m_pHelperResource_->SetAutoAnimationProperty();
	}

	// xml �����о� ����
	m_pCurrentSceneManager_->GetRootSceneNode().SetXMLFileName( m_mapInfo.m_strSceneFile );

	// �޵����� ����
	m_pHelperResource_->SetMediatorAfterSceneLoaded();

	// �� �ε� �̺�Ʈ ��� �� �������̼� ���ϻ���
	// ZONE_LOADED �޽����� ��۵ɶ� �ɶ� Ǯ ����� �ʿ��� ������ �ε�ȴ�.
	// �̶�, Save�� Zone Information ������ �ʿ��ϴ�.
	string strTemp(m_mapInfo.m_strScenePath + m_mapInfo.m_strSceneFile);
	CXMLEditInfoFile::GetReference().SaveZoneInformationFile(strTemp.c_str(), true);
	BroadCastMessage(VIEW_MESSAGE::SCENE_LOADED);

	// ���� ��Ƽ �÷��� ���������� null �׼��� �߰�
	m_pCmdMgr_->RecordCommand(NULL);

	if ( m_pEnvironmentSet_ )
		m_pEnvironmentSet_->LoadWeatherFromXML();

	// refresh zone
	m_pHelperResource_->RefreshZone();
}

void CControlMediator::CreateWorkSpaceFile(const char* _pSzWorkSpaceName, MAP_EDIT_TYPE _mapType)
{
	CWorkSpace* pWorkspace = m_pWorkSpaceMgr_->GetWorkSpace(_pSzWorkSpaceName);
	if (pWorkspace)
	{
		m_pWorkSpaceMgr_->SetHelperEntityAction( pWorkspace->m_WorkspaceHelpersAndActions_.m_pEntityActionHelper );
		m_pWorkSpaceMgr_->SetHelperObjectRenderer( pWorkspace->m_WorkspaceHelpersAndActions_.m_pObjectRenderHelper );

		pWorkspace->OnCreateWorkspaceFile();

		ASSERT(m_pCurrentSceneManager_);
		m_mapInfo.m_eMapEditType = _mapType;

		//�ʵ尡 �ε��Ǿ��ٰ� view���� �˸���, ���� �ʵ�� ���� ������ � �ʿ�
		if (_mapType != MAP_EDIT_TYPE_SCENE)
		{
			m_mapInfo.m_bEditSceneObject = false;
			m_mapInfo.m_bSceneEditableMap = false;
			BroadCastMessage(VIEW_MESSAGE::LOGIC_LOADED);
		}
		else
		{
			m_mapInfo.m_bEditSceneObject = true;
			m_mapInfo.m_bSceneEditableMap = true;
			BroadCastMessage(VIEW_MESSAGE::SCENE_LOADED);
		}
	}
}

void CControlMediator::ResetResource()
{
	m_pCurrentDevice_->SetLighting(false);

	//�� �Ŵ��� ���� && �� �Ŵ��� �������ΰ� ����
	if(m_pCurrentSceneManager_)
	{
		// CommandManager Reset
		// ���Ŵ��� ����� ��
		m_pCmdMgr_->ResetCommandManager();

		// ���Ŵ����� �پ��ִ� RefMesh �����
		m_pHelperResource_->ResetRefMesh(NULL);

		//environment set �����
		if ( m_pEnvironmentSet_ )
		{
			m_pEnvironmentSet_->RemoveFromParent();
			delete m_pEnvironmentSet_;

		}

		// SceneManager Clear
		BroadCastMessage(VIEW_MESSAGE::BEFORE_SCENE_CLEAR);

		// Logic Object Manager
		// BEFORE_SCENE_CLEAR �޽��� ��, ���Ŵ��� Clear ����
		m_pLogicObjectMgr_->ResetLogicObjectManager();

		// �� �Ŵ��� Clear
		m_pCurrentSceneManager_->Clear();

		// WorkSpace Resource Reset
		CWorkSpace* pCurrentWorkSpace = m_pWorkSpaceMgr_->GetCurrentWorkSpace();
		if (pCurrentWorkSpace)
			pCurrentWorkSpace->OnResetResourceAfterSceneCleared(false);

		m_pEnvironmentSet_ = new rs3::REnvironmentSet; // �̱����̶�..
		m_pCurrentSceneManager_->AddSceneNode(m_pEnvironmentSet_);
		m_pEnvironmentSet_->AddAttribute( RSNA_DO_NOT_SAVE );
	}

	//Helper
	m_pHelperAction_->ResetHelperAction();
	m_pHelperRender_->ResetHelperRender(m_pCurrentSceneManager_);

	//tmp file �����
	m_pHelperResource_->CreateMiniMapAndSetCamera("_minimap.tmp", CControlMediatorHelperResource::MINI_DELETE);
}

void CControlMediator::DestoryResource()
{
	// Environment Set
	if ( m_pEnvironmentSet_ )
	{
		m_pEnvironmentSet_->RemoveFromParent();
		delete m_pEnvironmentSet_;
		m_pEnvironmentSet_ = NULL;
	}

	// Ŀ�ǵ� ����
	// ���Ŵ��� �ʱ�ȭ ������ ���ش�.
	m_pCmdMgr_->ResetCommandManager();

	//���� ������ ī�޶���� ���� �����ش�.
	if(m_pCurrentCamera_ != NULL)
	{
		m_pCurrentCamera_->RemoveFromParent();
		delete m_pCurrentCamera_;
		m_pCurrentCamera_ = NULL;
	}

	if( m_pCurrentSceneManager_ != NULL)
	{
		// �� �Ŵ����� ���õ� Helper ��ü �����
		m_pHelperRender_->DestroyHelperRender();
		CMintManager::GetReference().DestroyMintManager();

		m_pHelperResource_->ResetRefMesh(NULL);

		// SceneManager Clear
		BroadCastMessage(VIEW_MESSAGE::BEFORE_SCENE_CLEAR);

		// Logic Object Manager
		// BEFORE_SCENE_CLEAR �޽��� ��, ���Ŵ��� Clear ����
		m_pLogicObjectMgr_->ResetLogicObjectManager();

		// �� �Ŵ��� �ʱ�ȭ
		m_pCurrentSceneManager_->Clear();

		// WorkSpace Resource Reset
		CWorkSpace* pCurrentWorkSpace = m_pWorkSpaceMgr_->GetCurrentWorkSpace();
		if (pCurrentWorkSpace)
			pCurrentWorkSpace->OnResetResourceAfterSceneCleared(true);

		// Broadcast Resource destroy message to view
		BroadCastMessage(VIEW_MESSAGE::BEFORE_RES_DESTROY);

		// Destroy Resource
		REngine::DestroySharedResource();
		m_pCurrentSceneManager_->Destroy();

		delete m_pCurrentSceneManager_;
		m_pCurrentSceneManager_ = NULL;
		REngine::RegisterSceneManager(NULL);
	}

	//tmp file �����
	m_pHelperResource_->CreateMiniMapAndSetCamera("_minimap.tmp", CControlMediatorHelperResource::MINI_DELETE);
}

void CControlMediator::DestroyDevice()
{
	if( m_pCurrentDevice_ != NULL)
	{
		m_pCurrentDevice_->Destroy();
		delete m_pCurrentDevice_;
		m_pCurrentDevice_ = NULL;
		REngine::RegisterDevice(NULL);
	}
}

bool CControlMediator::LoadSceneAndEnvFromXML_(const char* _pStrZoneName, MAP_EDIT_TYPE _eMapType)
{
	if (_eMapType != MAP_EDIT_TYPE_SCENE)
		m_pHelperResource_->SetMediatorAfterSceneLoaded(_eMapType);

	// ���������� AsyncLoading�� ���� �ʴ´�.
	if ( !m_pCurrentSceneManager_->LoadEditableSceneXML(_pStrZoneName) )
	{
		m_pCurrentSceneManager_->AddErrorMessage(".zone.xml ���� �б� ����");
		//AfxMessageBox(".zone.xml ���� �б� ����");
		return false;
	}

	//Set for Environment
	ASSERT(m_pEnvironmentSet_);
	char szBuffer[MAX_PATH];
	szBuffer[0] = '\0';
	strcpy(szBuffer, _pStrZoneName);
	char *p = strrchr(szBuffer,'.');
	strcpy(p,".env.xml");

	CRollupEtcCollision* pCollision = GetViewPtr<CRollupEtcCollision>();
	if( pCollision->GetCollisionTree().GetCollisionTree() == NULL )
		pCollision->GetCollisionTree().Load( pCollision->GetCollisionFileName().c_str() );

	if ( m_pEnvironmentSet_->CreateFromFile(szBuffer, false) == false )
		m_pCurrentSceneManager_->AddErrorMessage("�߸��� ȯ������ : env.xml ������ �߸��Ǿ��ֽ��ϴ�.");

	return true;
}

void CControlMediator::SetCurrentWBState(WORKBENCH_STATE::STATE _state)
{
	m_wbPreviousState_ = m_wbCurrentState_;
	m_wbCurrentState_ = _state;
	m_mouseInfo.setIgnoreMouseMovedPickingRActor( false );

	BroadCastMessage( VIEW_MESSAGE::WB_STATE_CHANGED );
}

void CControlMediator::WB_CallBackAction()
{
	// �ʱ�ȭ�� �ȵǾ��ְų� ���ҽ��� ������ ���¿��� ȣ��Ǵ� ��찡 �ִ�.
	if (m_pCurrentCamera_ == NULL)
		return;

	//camera callback action
	m_cameraInfo.m_bCameraMode = m_pCurrentCamera_->DoCameraCallBackAction(this);

	//workbench callBackAction
	if( !m_cameraInfo.m_bCameraMode &&
		CControlDoAction::IsCallBackAction(m_wbCurrentState_) )
		WB_ActionTable_[m_wbCurrentState_](this);
}

void CControlMediator::WB_OnTimeAction()
{
	// �ʱ�ȭ�� �ȵǾ��ְų� ���ҽ��� ������ ���¿��� ȣ��Ǵ� ��찡 �ִ�.
	if (m_pCurrentCamera_ == NULL)
		return;

	if(m_pCurrentCamera_->DoCameraOnTimeActionAndBreak(this, m_bNavigation_))
		return;

	//workbench OnTime Action
	if( !m_cameraInfo.m_bCameraMode &&
		!CControlDoAction::IsCallBackAction(m_wbCurrentState_) &&
		!CMintManager::GetReference().IsActiveQuadPopup() )
		WB_ActionTable_[m_wbCurrentState_](this);
}

string CControlMediator::m_errorString;

string& CControlMediator::GetErrorString()
{
	return m_errorString;
}

void CControlMediator::ClearErrorString()
{
	m_errorString.clear();
}

void CControlMediator::AddErrorString( const char* szText )
{
	m_errorString += szText;
}

void CControlMediator::Report(const FileWatcher::StringVec& changedFiles)
{
	CWorkSpace* pWorkspace = m_pWorkSpaceMgr_->GetCurrentWorkSpace();

	for (FileWatcher::StringVec::const_iterator cur = changedFiles.begin(); 
		cur != changedFiles.end(); 
		++cur)
	{
		mlog("������ ����Ǿ����ϴ�. %s\n", (*cur).c_str() );
		if(pWorkspace)
			pWorkspace->OnFileModified( *cur );
	}
}