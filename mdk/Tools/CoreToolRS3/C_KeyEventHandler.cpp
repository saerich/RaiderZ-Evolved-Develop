#include "StdAfx.h"

#include "MainFrm.h"

#include "C_KeyEventHandler.h"

#include "C_ControlMediator.h"
#include "C_ControlMediatorHelperResource.h"
#include "C_ControlMediatorHelperAction.h"
#include "C_ControlMediatorHelperRender.h"
#include "C_StatusBarHandler.h"

#include "WS_WorkSpaceManager.h"
#include "M_ToolCmdMgr.h"

#include "VL_ToolBar.h"
#include "VL_SceneNodeList.h"

#include "VR_TerrainHeight.h"
#include "VR_TerrainFlat.h"
#include "VR_TerrainAverage.h"
#include "VR_TerrainSplat.h"
#include "VR_TerrainRelax.h"
#include "VR_TerrainGrass.h"
#include "VR_EnvWeather.h"
#include "VR_EtcMapView.h"
#include "VR_EtcDecal.h"
#include "VR_EtcDev.h"

#include "VD_DlgInputCommon.h"
#include "VD_DlgInputNpc.h"
#include "VD_DlgXmlListEditor.h"
#include "VD_DlgOption.h"

#include "V_MFCHelper.h"

CKeyEventHandler& CKeyEventHandler::GetReference()
{
	static CKeyEventHandler eventHandler;
	return eventHandler;
}

CKeyEventHandler::CKeyEventHandler(void)
: m_bMenuKeyCanceled( false )
{
}

CKeyEventHandler::~CKeyEventHandler(void)
{
}

void CKeyEventHandler::InitHandler(CControlMediator* _pControlMediator)
{
	m_pControlMediator_ = _pControlMediator;
	m_pToolBar_ = _pControlMediator->GetViewPtr<CCoreToolToolBar>();

	//terrain
	m_pEditTerrainHeight_ = _pControlMediator->GetViewPtr<CRollupTerrainHeight>();
	m_pEditTerrainAver_ = _pControlMediator->GetViewPtr<CRollupTerrainAverage>();
	m_pEditTerrainFlat_ = _pControlMediator->GetViewPtr<CRollupTerrainFlat>();
	m_pEditTerrainSplat_ = _pControlMediator->GetViewPtr<CRollupTerrainSplat>();
	m_pEditTerrainRelax_ = _pControlMediator->GetViewPtr<CRollupTerrainRelax>();
	m_pEditTerrainGrass_ = _pControlMediator->GetViewPtr<CRollupTerrainGrass>();

	//object
	m_pEditObjectList_ = _pControlMediator->GetViewPtr<CDlgSceneNodeList>();

	//lightmap
	//m_pEditLightMap_ =  _pControlMediator->GetViewPtr<CEditEnvLightMap>();

	//Map View
	m_pEditMapView_ = _pControlMediator->GetViewPtr<CRollupEtcMapView>();

	//Decal
	m_pEtcDecal_ = _pControlMediator->GetViewPtr<CRollupEtcDecal>();

	//Develop
	m_pEtcDev_ = _pControlMediator->GetViewPtr<CRollupEtcDev>();
}

void CKeyEventHandler::MapEditAreaControl_(bool _positive)
{
	int size = CMFCHelper::GetBrushWorkAreaSize();

	if(_positive)
	{
		size += 1;
	}else
	{
		size -= 1;
	}

	CMFCHelper::SetBrushWorkAreaSize(size);
}

/// text edit �� ��Ŀ���� ���ִ��� üũ�Ѵ�
bool CKeyEventHandler::IsFocusWindowEditable()
{
	HWND hWnd = GetFocus();

	char szBuffer[256];
	GetClassName( hWnd, szBuffer, sizeof(szBuffer) );

	if( strcmp(szBuffer,"Edit") == 0 ) return true;

	return false;
}

void CKeyEventHandler::SysKeyEvent(UINT _charParam, bool _bKeyDown, bool& _ref_bKeyIntercepted)
{
	// Alt Ű, F10Ű, Alt + �ٸ�Ű �� ��� �� �Լ��� ó��
	if( (UINT)'Z' == _charParam && _bKeyDown )
	{
		rs3::RToolCamera* pCamera = m_pControlMediator_->GetToolCamera();
		if(pCamera != NULL)
			pCamera->SetPositionToSelectCenter(m_pControlMediator_->GetHelperAction());
		return;
	}

	// alt �� ������ ���� ���� Ŭ�� ������ menukey �� �����Ȱ�� �޴��� ������ �ʵ��� �����ڵ�
	// mfc 90 �� �⺻ ������ ������ �޶� �� �ڵ�
	if( _charParam == VK_MENU )
	{
		if( _bKeyDown == false && m_bMenuKeyCanceled == true )
		{
			_ref_bKeyIntercepted = true;
			m_bMenuKeyCanceled = false;
		}
	}
}

void CKeyEventHandler::CancelMenuKey()
{
	if( GetAsyncKeyState( VK_MENU ) )
		m_bMenuKeyCanceled = true;
}

void CKeyEventHandler::CallBackAppKeyEvent(UINT _charParam, bool _bKeyDown, bool& _ref_bKeyIntercepted)
{
	//����Ű�� �ȸԾ���ϴ� ��Ȳ �Ǻ�
	if( !m_pControlMediator_->GetHelperResource()->IsKeyEventAcceptableInNewMapDlg() ||
		CDlgInputCommon::m_isActive || CDlgInputNpc::m_isActive ||
		CDlgXmlListEditor::m_isActive || CDlgOption::m_isActive ||
		m_pEditObjectList_->HasActiveSceneEditor() ||
		IsFocusWindowEditable() )
		return;
	
	//control ���ִ� ����Ű�� Accelerator���� �� ���� �� �� �����Ƿ� �긮�� �ʰ� ó���� ����
	if(GetAsyncKeyState(VK_CONTROL))
	{
		if (_bKeyDown)
		{
			CKeyEventHandlerHelper* pKeyEventHelper = m_pControlMediator_->GetWorkSpaceMgr()->GetHelperKeyEventHandler();
			if (pKeyEventHelper)
				pKeyEventHelper->OnPressedCtrlCompositeHotKey(_charParam, m_pControlMediator_);
		}

		if(GetAsyncKeyState(VK_SHIFT) )
		{
			//Save All
			if( (UINT)'S' == _charParam && _bKeyDown && m_pControlMediator_->m_mapInfo.m_bLoaded)
			{
				AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_SAVEALL);
//				theApp.OnFileSaveAsAll();
				return;
			}
		}
		else
		{
			//Undo
			if( (UINT)'Z' == _charParam && _bKeyDown && m_pControlMediator_->m_mapInfo.m_bLoaded)
			{
				m_pControlMediator_->GetToolCmdMgr()->UndoCommand();
				return;
			}

			//Save All
			if( (UINT)'S' == _charParam && _bKeyDown && m_pControlMediator_->m_mapInfo.m_bLoaded)
			{
				AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_SAVEALL);
//				theApp.OnFileSaveall();
				return;
			}

			////////////////////////////////////////////////////////////
			//
			// Copy And Paste
			//copy
			if( (UINT)'C' == _charParam && _bKeyDown && m_pControlMediator_->m_mapInfo.m_bLoaded)
			{
				m_pControlMediator_->GetHelperAction()->CopyShallowSelectedEntityObject();
				return;
			}
			//paste
			if( (UINT)'V' == _charParam && _bKeyDown && m_pControlMediator_->m_mapInfo.m_bLoaded)
			{
				m_pControlMediator_->GetHelperAction()->PasteCopiedEntityObject();
				return;
			}

			//ī�޶� ����Ű
			const static int MOUSE_WHELL_DELTA = 120; //ī�޶� ��
			rs3::RToolCamera* pCamera = m_pControlMediator_->GetToolCamera();
			if(pCamera != NULL)
			{
				if((VK_ADD == _charParam || (UINT)187 == _charParam )&&_bKeyDown)
				{
					pCamera->MoveFront(MOUSE_WHELL_DELTA * m_pControlMediator_->m_cameraInfo.WHEEL_ZOOM_STEP);
					pCamera->UpdateViewFrustrum();

				}else if ((VK_SUBTRACT == _charParam || (UINT)189 == _charParam ) && _bKeyDown)
				{
					pCamera->MoveFront(- (MOUSE_WHELL_DELTA) * m_pControlMediator_->m_cameraInfo.WHEEL_ZOOM_STEP);
					pCamera->UpdateViewFrustrum();
				}

				// ī�޶� ��ġ �̵�
				if ((VK_F1 <= _charParam) && (_charParam <= VK_F8) && _bKeyDown)
				{
					const rs3::RToolCamera::LOCATION_INFO* pLocationInfo = pCamera->GetLocation(_charParam - VK_F1);
					pCamera->SetLocation(pLocationInfo);
				}
			}
		}
		
		//control ����Ű�� �������ش�.
		return;
	}

	if(GetAsyncKeyState(VK_SHIFT))
	{
		// ī�޶� ��ġ ����
		if ((VK_F1 <= _charParam) && (_charParam <= VK_F8) && _bKeyDown)
		{
			rs3::RToolCamera* pCamera = m_pControlMediator_->GetToolCamera();
			if(pCamera != NULL)
			{
				pCamera->RememberLocation(_charParam - VK_F1);
				m_pEditMapView_->UpdateCameraLocationList();
			}
		}

		//Global Context Menu;
		/*if( (VK_SPACE == _charParam) && _bKeyDown )
			m_pView_->ContextMenuPop();*/

		//Object View Toggle - Shift + 1 ~
		if( ((UINT)'1' == _charParam) && _bKeyDown) m_pEditObjectList_->ViewStateToggle( rs3::RSID_TERRAINSCENENODE );
		if( ((UINT)'2' == _charParam) && _bKeyDown) m_pEditObjectList_->ViewStateToggle( rs3::RSID_LIGHTSCENENODE );
		if( ((UINT)'3' == _charParam) && _bKeyDown) m_pEditObjectList_->ViewStateToggle( rs3::RSID_ACTOR );
		if( ((UINT)'4' == _charParam) && _bKeyDown) m_pEditObjectList_->ViewStateToggle( rs3::RSID_TREE );
		if( ((UINT)'5' == _charParam) && _bKeyDown) m_pEditTerrainGrass_->ViewToggleGrass();
		if( ((UINT)'6' == _charParam) && _bKeyDown) m_pEditObjectList_->ViewStateToggle( rs3::RSID_WATERPLANE );

		//shift ����Ű�� �������ش�.
		if(VK_OEM_PERIOD == _charParam && _bKeyDown)
		{
			m_pControlMediator_->m_cameraInfo.WHEEL_ZOOM_STEP += 1;

			m_pEditMapView_->UpdateCameraControlInfo();
			m_cstrStatusMessage_.Format("ī�޶� �� �ӵ� ���� : %d", (int)m_pControlMediator_->m_cameraInfo.WHEEL_ZOOM_STEP);
			CStatusBarHandler::GetInstance()->ShowStatusBarMessage(m_cstrStatusMessage_);
		}
		if(VK_OEM_COMMA == _charParam && _bKeyDown)
		{
			m_pControlMediator_->m_cameraInfo.WHEEL_ZOOM_STEP -= 1;
			if(m_pControlMediator_->m_cameraInfo.WHEEL_ZOOM_STEP < 0)
				m_pControlMediator_->m_cameraInfo.WHEEL_ZOOM_STEP = 0;

			m_pEditMapView_->UpdateCameraControlInfo();
			m_cstrStatusMessage_.Format("ī�޶� �� �ӵ� ���� : %d", (int)m_pControlMediator_->m_cameraInfo.WHEEL_ZOOM_STEP);
			CStatusBarHandler::GetInstance()->ShowStatusBarMessage(m_cstrStatusMessage_);
		}
		return;
	}

	//delete ����Ű, delete�� ���� �������� ���ϼ� �ִ�.
	//���⼭�� ������Ʈ ���� Ŀ�ǵ��̸� �������� ����� ��Ų��. Ŀ�ǵ� ����ÿ� ����
	if( (VK_DELETE == _charParam) && _bKeyDown )
	{
		if(!m_pControlMediator_->m_mapInfo.m_bLoaded) return;

		WORKBENCH_STATE::STATE wbstate = m_pControlMediator_->GetCurrentWBState();
		if(	wbstate != WORKBENCH_STATE::OBJECT_MOVE  && wbstate != WORKBENCH_STATE::OBJECT_ROTATE &&
			wbstate != WORKBENCH_STATE::OBJECT_SCALE && wbstate != WORKBENCH_STATE::OBJECT_SELECT &&
			wbstate != WORKBENCH_STATE::OBJECT_NEW )
			return;

		m_pControlMediator_->GetHelperAction()->DeleteSelectedObject();
		return;
	}

	//On Terrain
	if(((UINT)'Z' == _charParam) && _bKeyDown && m_pToolBar_->m_btnOnTerrain.IsWindowEnabled())
	{
		WORKBENCH_STATE::STATE wbstate = m_pControlMediator_->GetCurrentWBState();
		if( wbstate == WORKBENCH_STATE::TERRAIN_GRASS )
		{
			m_pEditTerrainGrass_->OnBnClickedButtonGrassOnTerrain();
			return;
		}

		//selected object
		if(	wbstate == WORKBENCH_STATE::OBJECT_MOVE  || wbstate == WORKBENCH_STATE::OBJECT_ROTATE ||
			wbstate == WORKBENCH_STATE::OBJECT_SCALE || wbstate == WORKBENCH_STATE::OBJECT_SELECT ||
			wbstate == WORKBENCH_STATE::OBJECT_NEW )
		{
			m_pToolBar_->OnBnClickedTopObjOnTerrain();
			return;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// ���� Ű Ȯ���� ���� �ʴ� ����Ű ��
	// �� ������ ���� ����Ű�� Ű �Է��� �ٸ� �����쿡���� Ȯ���Ѵ�.
	//
	//////////////////////////////////////////////////////////////////////////


	// DOF ����
	float fFactor = 1;
	if(GetAsyncKeyState(VK_CONTROL))
		fFactor = 10;

	if(GetAsyncKeyState(VK_NUMPAD8))
	{
		rs3::RToolSceneManager* pCamera = m_pControlMediator_->GetToolSceneManager();
		pCamera->AddDOFFocalDist( 50 * fFactor);
		CStatusBarHandler::GetInstance()->ShowCurrentCameraDOF( pCamera->GetDOFFocalDist(), pCamera->GetDOFFocalRange(), pCamera->GetDOFCOCSize());
	}
	if(GetAsyncKeyState(VK_NUMPAD2))
	{
		rs3::RToolSceneManager* pCamera = m_pControlMediator_->GetToolSceneManager();
		pCamera->AddDOFFocalDist( -50 * fFactor);
		CStatusBarHandler::GetInstance()->ShowCurrentCameraDOF( pCamera->GetDOFFocalDist(), pCamera->GetDOFFocalRange(), pCamera->GetDOFCOCSize());
	}
	if(GetAsyncKeyState(VK_NUMPAD4))
	{
		rs3::RToolSceneManager* pCamera = m_pControlMediator_->GetToolSceneManager();
		pCamera->AddDOFFocalRange( -50 * fFactor);
		CStatusBarHandler::GetInstance()->ShowCurrentCameraDOF( pCamera->GetDOFFocalDist(), pCamera->GetDOFFocalRange(), pCamera->GetDOFCOCSize());
	}
	if(GetAsyncKeyState(VK_NUMPAD6))
	{
		rs3::RToolSceneManager* pCamera = m_pControlMediator_->GetToolSceneManager();
		pCamera->AddDOFFocalRange( 50 * fFactor);
		CStatusBarHandler::GetInstance()->ShowCurrentCameraDOF( pCamera->GetDOFFocalDist(), pCamera->GetDOFFocalRange(), pCamera->GetDOFCOCSize());
	}
	if(GetAsyncKeyState(VK_NUMPAD9))
	{
		rs3::RToolSceneManager* pCamera = m_pControlMediator_->GetToolSceneManager();
		pCamera->AddDOFCOCSize( 0.1f * fFactor);
		CStatusBarHandler::GetInstance()->ShowCurrentCameraDOF( pCamera->GetDOFFocalDist(), pCamera->GetDOFFocalRange(), pCamera->GetDOFCOCSize());
	}
	if(GetAsyncKeyState(VK_NUMPAD3))
	{
		rs3::RToolSceneManager* pCamera = m_pControlMediator_->GetToolSceneManager();
		pCamera->AddDOFCOCSize( -0.1f * fFactor);
		CStatusBarHandler::GetInstance()->ShowCurrentCameraDOF( pCamera->GetDOFFocalDist(), pCamera->GetDOFFocalRange(), pCamera->GetDOFCOCSize());
	}

	//���׺���
	if(((UINT)'F' == _charParam) && _bKeyDown )
	{
		if(m_pControlMediator_->m_mapInfo.m_bLoaded)
			m_pControlMediator_->GetViewPtr<CRollupEnvWeather>()->ToggleFog();
	}

	//��ü�������� ���� ��ü ��ױ�
	if(((UINT)'C' == _charParam) && _bKeyDown && m_pToolBar_->m_btnObjectLock.IsWindowEnabled())
	{
		_ref_bKeyIntercepted = true;
		WORKBENCH_STATE::STATE wbstate = m_pControlMediator_->GetCurrentWBState();
		if(	wbstate == WORKBENCH_STATE::OBJECT_MOVE  || wbstate == WORKBENCH_STATE::OBJECT_ROTATE ||
			wbstate == WORKBENCH_STATE::OBJECT_SCALE || wbstate == WORKBENCH_STATE::OBJECT_SELECT )
		{

			if( m_pToolBar_->m_btnObjectLock.isChecked() == true)
				m_pToolBar_->m_btnObjectLock.check(false);
			else
				m_pToolBar_->m_btnObjectLock.check(true);
			m_pToolBar_->OnBnClickedTopLock();
		}
	}

	//ī�޶� ����Ű
	if (((UINT)'W' == _charParam) && _bKeyDown) m_pControlMediator_->m_cameraInfo.m_bKeyW = true;
	if (((UINT)'W' == _charParam) && !_bKeyDown) m_pControlMediator_->m_cameraInfo.m_bKeyW = false;
	if (((UINT)'A' == _charParam) && _bKeyDown) m_pControlMediator_->m_cameraInfo.m_bKeyA = true;
	if (((UINT)'A' == _charParam) && !_bKeyDown) m_pControlMediator_->m_cameraInfo.m_bKeyA = false;
	if (((UINT)'S' == _charParam) && _bKeyDown) m_pControlMediator_->m_cameraInfo.m_bKeyS = true;
	if (((UINT)'S' == _charParam) && !_bKeyDown) m_pControlMediator_->m_cameraInfo.m_bKeyS = false;
	if (((UINT)'D' == _charParam) && _bKeyDown) m_pControlMediator_->m_cameraInfo.m_bKeyD = true;
	if (((UINT)'D' == _charParam) && !_bKeyDown) m_pControlMediator_->m_cameraInfo.m_bKeyD = false;

	/////// ��������
	//
	if(m_pControlMediator_->m_mapInfo.m_eMapEditType == MAP_EDIT_TYPE_SCENE)
	{
		if (m_pControlMediator_->GetToolSceneManager()->GetCurrentTerrain())
		{
			//���÷��ÿ��� ���̾� ����
			if(m_pControlMediator_->GetCurrentWBState() == WORKBENCH_STATE::TERRAIN_SPLAT)
			{
				if( (VK_PRIOR == _charParam) && _bKeyDown)
				{
					_ref_bKeyIntercepted = true;
					m_pEditTerrainSplat_->m_ctlLayer.SendMessage(WM_KEYDOWN, VK_UP);
				}
				else if( (VK_NEXT == _charParam) && _bKeyDown)
				{
					_ref_bKeyIntercepted = true;
					m_pEditTerrainSplat_->m_ctlLayer.SendMessage(WM_KEYDOWN, VK_DOWN);
				}
			}
		}

		//���� '[' �� ']'�� ���� ũ�� ���� ó��
		if(((UINT)VK_OEM_6 == _charParam)&&_bKeyDown) MapEditAreaControl_(true);
		if(((UINT)VK_OEM_4 == _charParam)&&_bKeyDown) MapEditAreaControl_(false);

		//���� '+' �� '-'�� ���� ũ�� ���� ó��
		if(((UINT)VK_ADD == _charParam || (UINT)187 == _charParam ||
			(UINT)VK_SUBTRACT == _charParam || (UINT)189 == _charParam ) &&_bKeyDown)
		{
			if(m_pControlMediator_->GetCurrentWBState() == WORKBENCH_STATE::TERRAIN_HEIGHT)
			{
				if((UINT)VK_ADD == _charParam || (UINT)187 == _charParam)
					//��ũ�� ��� ���ø� �Լ�
					T_MapStrengthControl_(m_pEditTerrainHeight_, true, 0.5f);
				else
					T_MapStrengthControl_(m_pEditTerrainHeight_, false, 0.5f);
			}
			else if(m_pControlMediator_->GetCurrentWBState() == WORKBENCH_STATE::TERRAIN_AVER)
			{
				if((UINT)VK_ADD == _charParam || (UINT)187 == _charParam)
					T_MapStrengthControl_(m_pEditTerrainAver_, true, 1.0f);
				else
					T_MapStrengthControl_(m_pEditTerrainAver_, false, 1.0f);
			}
			else if(m_pControlMediator_->GetCurrentWBState() == WORKBENCH_STATE::TERRAIN_SPLAT)
			{
				if((UINT)VK_ADD == _charParam || (UINT)187 == _charParam)
					T_MapStrengthControl_(m_pEditTerrainSplat_, true, 0.5f);
				else
					T_MapStrengthControl_(m_pEditTerrainSplat_, false, 0.5f);
			}
			else if(m_pControlMediator_->GetCurrentWBState() == WORKBENCH_STATE::TERRAIN_FLAT)
			{
				if((UINT)VK_ADD == _charParam || (UINT)187 == _charParam)
					T_MapStrengthControl_(m_pEditTerrainFlat_, true, 1.0f);
				else
					T_MapStrengthControl_(m_pEditTerrainFlat_, false, 1.0f);
			}
			else if(m_pControlMediator_->GetCurrentWBState() == WORKBENCH_STATE::TERRAIN_RELAX)
			{
				if((UINT)VK_ADD == _charParam || (UINT)187 == _charParam)
					T_MapStrengthControl_(m_pEditTerrainRelax_, true, 0.2f);
				else
					T_MapStrengthControl_(m_pEditTerrainRelax_, false, 0.2f);
			}
			else if(m_pControlMediator_->GetCurrentWBState() == WORKBENCH_STATE::TERRAIN_GRASS)
			{
				if((UINT)VK_ADD == _charParam || (UINT)187 == _charParam)
					T_MapStrengthControl_(m_pEditTerrainGrass_, true, 1.0f);
				else
					T_MapStrengthControl_(m_pEditTerrainGrass_, false, 1.0f);
			}
		}

		//H - ������, B - ���÷���, G - ��պ귯��, V - �����ϰ�, L - Relax, N - Grass
		if((UINT)'H' == _charParam && _bKeyDown) m_pControlMediator_->GetWorkSpaceMgr()->TriggerWBState(WORKBENCH_STATE::TERRAIN_HEIGHT, true);
		if((UINT)'B' == _charParam && _bKeyDown) m_pControlMediator_->GetWorkSpaceMgr()->TriggerWBState(WORKBENCH_STATE::TERRAIN_SPLAT, true);
		if((UINT)'G' == _charParam && _bKeyDown) m_pControlMediator_->GetWorkSpaceMgr()->TriggerWBState(WORKBENCH_STATE::TERRAIN_AVER, true);
		if((UINT)'V' == _charParam && _bKeyDown) m_pControlMediator_->GetWorkSpaceMgr()->TriggerWBState(WORKBENCH_STATE::TERRAIN_FLAT, true);
		if((UINT)'L' == _charParam && _bKeyDown) m_pControlMediator_->GetWorkSpaceMgr()->TriggerWBState(WORKBENCH_STATE::TERRAIN_RELAX, true);
		if((UINT)'N' == _charParam && _bKeyDown) m_pControlMediator_->GetWorkSpaceMgr()->TriggerWBState(WORKBENCH_STATE::TERRAIN_GRASS, true);
	}

	///////////////////////////////////////////////////////
	//
	// ��ü ����
	//T - Select E - Scale, R - Rotate, Q - Move
	if( m_pControlMediator_->m_mapInfo.m_bLoaded )
	{
		if((UINT)'T' == _charParam && _bKeyDown) m_pControlMediator_->GetWorkSpaceMgr()->TriggerWBState(WORKBENCH_STATE::OBJECT_SELECT, true);
		if((UINT)'E' == _charParam && _bKeyDown) m_pControlMediator_->GetWorkSpaceMgr()->TriggerWBState(WORKBENCH_STATE::OBJECT_SCALE, true);
		if((UINT)'R' == _charParam && _bKeyDown) m_pControlMediator_->GetWorkSpaceMgr()->TriggerWBState(WORKBENCH_STATE::OBJECT_ROTATE, true);
		if((UINT)'Q' == _charParam && _bKeyDown) m_pControlMediator_->GetWorkSpaceMgr()->TriggerWBState(WORKBENCH_STATE::OBJECT_MOVE, true);

		if( m_pControlMediator_->m_mapInfo.m_bSceneEditableMap )
		{
			// ��ü ����Ʈ(Ʈ����Ʈ��) ��ü ���� �ݱ�
			if( (VK_MULTIPLY == _charParam) && _bKeyDown)
			{
				_ref_bKeyIntercepted = true;
				m_pEditObjectList_->ExpandTreeControlToggle();
			}
		}
	}

	//�������� F1
	if(VK_F1 == _charParam && _bKeyDown) m_pControlMediator_->GetHelperRender()->ToggleViewSideInfoPanel();

	//1st-3rd ī�޶� �ٲٱ� F2
	if(VK_F2 == _charParam && _bKeyDown)
	{
		m_pControlMediator_->GetHelperAction()->SetNavigationModeToggle();
		if(m_pControlMediator_->IsWBNavigation())
			CStatusBarHandler::GetInstance()->ShowStatusBarMessage("Navigation Mode");
	}

	//���۷��� �޽� F3
	if(VK_F3 == _charParam && _bKeyDown)
		m_pControlMediator_->GetHelperResource()->ShowRefMeshToggle();

	//�׸��� F4 �׹����� / Ʈ�� �浹�ڽ� ����
	if(VK_F4 == _charParam && _bKeyDown)
	{
		//static BOOL bGrid = FALSE;

		//bGrid = !bGrid;
		//rs3::RToolSceneManager* pSceneManager = m_pControlMediator_->GetToolSceneManager();

		////by pok, ���� �浹���⵵ �����ؾ�����..
		////rs3::RTreeSystem* pTreeSystem = pSceneManager->GetTreeSystem();
		//rs3::RToolTerrain *pTerrain = (rs3::RToolTerrain*)pSceneManager->GetCurrentTerrain();
		//if (pTerrain)
		//	pTerrain->SetGridFlag(bGrid ? true : false);

		rs3::RToolSceneManager* pSceneManager = m_pControlMediator_->GetToolSceneManager();
		rs3::RToolTerrain* pTerrain = (rs3::RToolTerrain*)pSceneManager->GetCurrentTerrain();

		static int viewStatus = 0;
		viewStatus = ++viewStatus % 3;
		switch( viewStatus )
		{
		case 0:
			{
				pSceneManager->SetWireFrameDrawMode( false );

				if( pTerrain )
				{
					pTerrain->SetGridFlag( false );
				}
			}
			break;

		case 1:
			{
				pSceneManager->SetWireFrameDrawMode( false );

				//by pok, ���� �浹���⵵ �����ؾ�����..
				//rs3::RTreeSystem* pTreeSystem = pSceneManager->GetTreeSystem();
				if( pTerrain )
				{
					pTerrain->SetGridFlag( true );
				}
			}
			break;

		case 2:
			{
				pSceneManager->SetWireFrameDrawMode( true );
				if( pTerrain )
				{
					pTerrain->SetGridFlag( false );
				}
			}
			break;
		}
	}

	//��ǥ����� F5 - x, F6 - y, F7 - z, F8 - ��ȯ
	if(VK_F5 == _charParam && _bKeyDown) m_pToolBar_->SetAxis(CCoreToolToolBar::TB_AXIS_X);
	if(VK_F6 == _charParam && _bKeyDown) m_pToolBar_->SetAxis(CCoreToolToolBar::TB_AXIS_Y);
	if(VK_F7 == _charParam && _bKeyDown) m_pToolBar_->SetAxis(CCoreToolToolBar::TB_AXIS_Z);
	if(VK_F8 == _charParam && _bKeyDown) m_pToolBar_->SetAxis(CCoreToolToolBar::TB_AXIS_COMBO);

	//pause F10
	//F10�� �޴��ٿ��̶� ���������Ϳ��� �������̵� ���־���.
	//if(VK_F10 == _charParam && !_bKeyDown) m_pControlMediator_->GetHelperAction()->SetPauseToggle();

	// ī�޶� �̵��ӵ� ��ȭ
	if((UINT)'P' == _charParam && _bKeyDown)
	{
		if(GetAsyncKeyState(VK_OEM_2)) // ?ǥ�� �ۼ�
			m_pControlMediator_->m_cameraInfo.KEY_STEP += 10;
		else
			m_pControlMediator_->m_cameraInfo.KEY_STEP += 1000;

		m_pEditMapView_->UpdateCameraControlInfo();
		m_cstrStatusMessage_.Format("ī�޶� WASD �̵��ӵ� ���� : %d", (int)m_pControlMediator_->m_cameraInfo.KEY_STEP);
		CStatusBarHandler::GetInstance()->ShowStatusBarMessage(m_cstrStatusMessage_);

	}
	if((UINT)'O' == _charParam && _bKeyDown)
	{
		if(GetAsyncKeyState(VK_OEM_2)) // ?ǥ�� �ۼ�
			m_pControlMediator_->m_cameraInfo.KEY_STEP -= 10;
		else
			m_pControlMediator_->m_cameraInfo.KEY_STEP -= 1000;

		if(m_pControlMediator_->m_cameraInfo.KEY_STEP < 0)
			m_pControlMediator_->m_cameraInfo.KEY_STEP = 0;

		m_pEditMapView_->UpdateCameraControlInfo();
		m_cstrStatusMessage_.Format("ī�޶� WASD �̵��ӵ� ���� : %d", (int)m_pControlMediator_->m_cameraInfo.KEY_STEP);
		CStatusBarHandler::GetInstance()->ShowStatusBarMessage(m_cstrStatusMessage_);
	}

	//���ΰ�ħ, VK_OEM_3 = ~
	if(VK_OEM_3 == _charParam && _bKeyDown)
	{
		m_pControlMediator_->GetHelperResource()->RefreshZone();
	}

	// snap to grid ����/����
	if( VK_OEM_1 == _charParam && _bKeyDown )
	{
		CMainFrame* pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
		pMainFrame->DecreaseSnapToGridSize();
	}

	if( VK_OEM_7 == _charParam && _bKeyDown )
	{
		CMainFrame* pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
		pMainFrame->IncreaseSnapToGridSize();
	}

	// �Ϲ����� Ű���� ����Ű
	if (_bKeyDown)
	{
		CKeyEventHandlerHelper* pKeyEventHelper = m_pControlMediator_->GetWorkSpaceMgr()->GetHelperKeyEventHandler();
		if (pKeyEventHelper)
			pKeyEventHelper->OnPressedNoneCompositeHotKey(_charParam, m_pControlMediator_);
	}
}
