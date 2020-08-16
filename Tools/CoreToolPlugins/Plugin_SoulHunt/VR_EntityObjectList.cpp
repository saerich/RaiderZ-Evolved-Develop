// V_EditEntityObjectList.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "VR_EntityObjectList.h"

#include "C_ControlMediator.h"
#include "C_XMLConfig.h"
#include "C_XMLUserConfig.h"
#include "C_XMLEditInfoFile.h"

#include "C_ControlMediatorHelperRender.h"
#include "C_ControlMediatorHelperAction.h"
#include "C_ControlCursor.h"

#include "M_ToolCmdMgr.h"
#include "M_ToolLogicObjectManager.h"
#include "M_EntityCommandBuffer.h"
#include "M_LogicObjectCommandBuffer.h"

#include "VL_ToolBar.h"
#include "VL_ResourceInfo.h"
#include "VR_EntityEnvironment.h"

#include "V_MFCHelper.h"
#include "VD_DlgInputNpc.h"
#include "VD_DlgInputSensorWarp.h"
#include "VD_DlgInputColorPicker.h"
#include "VD_DlgInputCommon.h"
#include "VD_DlgInputBool.h"
#include "VD_DlgInputTransform.h"

#include "WS_Field.h"

#include "VG_Gizmo.h"

#include <algorithm>

#include "MStringUtil.h"

#define EDITING_ID_WARNING_MESSAGE "Entity Object�� ���� ID �ѹ����� �ٽ��մϴ�. \n\n\
���ο� Entity Object�� �� ��ȣ ���İ� �Ҵ�Ǹ� \n\
�� ��ɿ� ���� ����۾��� �������� �ʽ��ϴ�."

#define SUB_SPAWN		"spawn"
#define SUB_MAP_OBJ		"map_obj"
#define SUB_GATHER_OBJ	"gather_obj"

//
void CStyleListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	if ((int)lpDIS->itemID < 0)
		return;

	int		iPenSize	= 2;
	CDC		*pDC		= CDC::FromHandle(lpDIS->hDC);	
	
	COLORREF bg			= ::GetSysColor(COLOR_WINDOW);
	COLORREF fg			= ::GetSysColor(COLOR_WINDOWTEXT);

	CEntityObject *pEntityObject = (CEntityObject*)(lpDIS->itemData);
	if ( pEntityObject->GetEditable() == false )
		fg = RGB(250, 1, 1);
	
	// ���õǾ��ų�, Focus �� ���� �� ����
	if ( /*(lpDIS->itemAction & ODA_FOCUS) && (lpDIS->itemState & ODS_FOCUS)*/
		(lpDIS->itemState & ODS_SELECTED) )
		std::swap(bg, fg);

	// Draw background

	// -------------------------------------------------------------------
	CBrush brush(bg);
	pDC->FillRect(&lpDIS->rcItem, &brush);

	// Draw text
	//--------------------------------------------------------------------
	UINT nFormat = DT_SINGLELINE;

	int		 nOldBkMode		= pDC->SetBkMode(TRANSPARENT);
	COLORREF cOldFg			= pDC->SetTextColor(fg);
	CString	 sItemText;

	CRect rc(lpDIS->rcItem);
	rc.DeflateRect(iPenSize, iPenSize);

	GetText((int)lpDIS->itemID, sItemText);
	pDC->DrawText(sItemText, -1, &rc, nFormat);

	pDC->SetTextColor(cOldFg);
	pDC->SetBkMode(nOldBkMode);


#if 0
	// Draw disabled
	//--------------------------------------------------------------------
	if(pItem->bActive == FALSE)
	{
		CRect rc(lpDIS->rcItem);

		rc.DeflateRect(iPenSize, iPenSize);

		CDC memDC0;

		memDC0.CreateCompatibleDC(pDC);

		AlphaBlendU(*pDC, rc.left, rc.top, rc.Width(), rc.Height(),
			memDC0, rc.left, rc.top, rc.Width(), rc.Height(), 50);

		memDC0.DeleteDC();
	}
#endif
}

// CEditEntityObjectList ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CRollupEntityObjectList, CDialog)

CRollupEntityObjectList::CRollupEntityObjectList(CWnd* pParent /*=NULL*/)
	: CDialog(CRollupEntityObjectList::IDD, pParent)	
{
	ResetObjectData();

	for(int i = 0; i < CEntityObject::ENTITY_OBJ_NUM; ++i)
	{
		m_arrayBaseEntityObject_[i] = NULL;
	}
	m_pSensorWarpCustom_ = NULL;
	m_pEntityAreaCustom_ = NULL;
	m_pTrackingSpawnObject_ = NULL;

	m_pGroupTopBarBtn_ = NULL;

	m_fSectorHeight = 0.0f;

	m_EffectInfoMgr.Load("../../data/system/effect.xml");
}

CRollupEntityObjectList::~CRollupEntityObjectList()
{
}

void CRollupEntityObjectList::ResetObjectData()
{
	m_bVisibleSpawn_ = m_bVisibleMarker_ = m_bVisibleSensor_ = m_bVisibleSound_ = m_bVisibleArea_ = m_bVisibleRoom_ =TRUE;
	m_bVisibleSpawnOld_ = m_bVisibleMarkerOld_ = m_bVisibleSensorOld_ = m_bVisibleSoundOld_ = m_bVisibleAreaOld_ = m_bVisibleRoomOld_ =TRUE;
	m_bHideSpawnHandlerOld_ = m_bViewMapObjHandlerOld_ = FALSE;

	m_bEnableCommonEntity_ = false;
	m_bEnableReadOnly_ = false;

	m_bPreVisibleSpawn_ = false;
	m_bPreViewOnlySelected = false;	
}

void CRollupEntityObjectList::ResetObjectCheckBox()
{
	CButton *pCheck;

	pCheck = (CButton *)GetDlgItem(IDC_CHECK_INVISIBLE_SPAWN);
	pCheck->SetCheck(TRUE);

	pCheck = (CButton *)GetDlgItem(IDC_CHECK_INVISIBLE_MARKER);
	pCheck->SetCheck(TRUE);

	pCheck = (CButton *)GetDlgItem(IDC_CHECK_INVISIBLE_SENSOR);
	pCheck->SetCheck(TRUE);

	pCheck = (CButton *)GetDlgItem(IDC_CHECK_INVISIBLE_SOUND);
	pCheck->SetCheck(TRUE);

	pCheck = (CButton *)GetDlgItem(IDC_CHECK_INVISIBLE_AREA);
	pCheck->SetCheck(TRUE);

	pCheck = (CButton *)GetDlgItem(IDC_CHECK_INVISIBLE_ROOM);
	pCheck->SetCheck(TRUE);
}


BOOL CRollupEntityObjectList::OnInitDialog(){
	CDialog::OnInitDialog();

	//Specifies the position and size of the static control. It can be either a RECT structure or a CRect object.
	m_editModeLabel_.Create("Edit Mode", WS_CHILD | WS_VISIBLE, CRect(8, 4, 200, 20), this, 0x188);
	m_contentFont_.CreatePointFont(100, "Arial bold");
	m_editModeLabel_.SetFont(&m_contentFont_);

	for (int i_btn =0; i_btn < CEntityObject::ENTITY_OBJ_NUM; i_btn++)
	{
		m_btnEntityObjectNew_[i_btn].setType(FooButton::hotCustomRadioButton);
	}

	NoneSelectNewEntityObject_();

	RegisterResizer();

	CCoreToolEdit::EDIT_ENTER_HANDLER posHandler(this, &CRollupEntityObjectList::OnSearchObjectList );
	m_editSearchList.SetCallback(posHandler);
	return TRUE;
}

void CRollupEntityObjectList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ENTITY_EDIT_LIST, m_ctlList_);
	DDX_Control(pDX, IDC_TREE_ENTITY_LIST_PREVIEW, m_ctlTreePreview_);

	//View, List���� ���̰� �Ⱥ��̰�
	DDX_Check(pDX, IDC_CHECK_INVISIBLE_SPAWN, m_bVisibleSpawn_);
	DDX_Check(pDX, IDC_CHECK_INVISIBLE_MARKER, m_bVisibleMarker_);
	DDX_Check(pDX, IDC_CHECK_INVISIBLE_SENSOR, m_bVisibleSensor_);
	DDX_Check(pDX, IDC_CHECK_INVISIBLE_SOUND, m_bVisibleSound_);
	DDX_Check(pDX, IDC_CHECK_INVISIBLE_AREA, m_bVisibleArea_);
	DDX_Check(pDX, IDC_CHECK_INVISIBLE_ROOM, m_bVisibleRoom_);

	DDX_Control(pDX, IDC_BUTTON_NEW_SPAWN, m_btnEntityObjectNew_[CEntityObject::ENTITY_SPAWN]);
	DDX_Control(pDX, IDC_BUTTON_NEW_MARKER, m_btnEntityObjectNew_[CEntityObject::ENTITY_MARKER]);
	DDX_Control(pDX, IDC_BUTTON_NEW_SENSOR, m_btnEntityObjectNew_[CEntityObject::ENTITY_SENSOR]);
	DDX_Control(pDX, IDC_BUTTON_NEW_SPAWN_MAP_OBJ, m_btnEntityObjectNew_[CEntityObject::ENTITY_SPAWN_MAP_OBJ]);
	DDX_Control(pDX, IDC_BUTTON_NEW_SPAWN_GATHER, m_btnEntityObjectNew_[CEntityObject::ENTITY_SPAWN_GATHER]);
	DDX_Control(pDX, IDC_BUTTON_NEW_SOUND, m_btnEntityObjectNew_[CEntityObject::ENTITY_SOUND]);
	DDX_Control(pDX, IDC_BUTTON_NEW_AREA, m_btnEntityObjectNew_[CEntityObject::ENTITY_AREA]);
	DDX_Control(pDX, IDC_BUTTON_NEW_ROOM, m_btnEntityObjectNew_[CEntityObject::ENTITY_ROOM]);

	DDX_Control(pDX, IDC_CHECK_VIEW_NPC, m_checkViewOnlySelected);
	DDX_Control(pDX, IDC_CHECK_HIDE_SPAWN_HANDLER, m_checkHideSpawnHandler);
	DDX_Control(pDX, IDC_CHECK_VIEW_MAP_OBJ_HANDLER, m_checkViewMapObjHandler);	
	DDX_Text(pDX, IDC_EDIT_SECTOR_HEIGHT, m_fSectorHeight);

	DDX_Control(pDX, IDC_EDIT_SEARCH_LIST, m_editSearchList);
}

void CRollupEntityObjectList::OnUpdateCmdUI(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

BEGIN_MESSAGE_MAP(CRollupEntityObjectList, CDialog)
	ON_LBN_SELCHANGE(IDC_ENTITY_EDIT_LIST, &CRollupEntityObjectList::OnLbnSelchangeEntityList)
	ON_BN_CLICKED(IDC_BUTTON_SCRIPT_EDIT, &CRollupEntityObjectList::OnBnClickedButtonScriptEdit)
	ON_BN_CLICKED(IDC_CHECK_INVISIBLE_SPAWN, &CRollupEntityObjectList::OnBnClickedCheckInvisibleSpawn)
	ON_BN_CLICKED(IDC_CHECK_INVISIBLE_MARKER, &CRollupEntityObjectList::OnBnClickedCheckInvisibleMarker)
	ON_BN_CLICKED(IDC_CHECK_INVISIBLE_SENSOR, &CRollupEntityObjectList::OnBnClickedCheckInvisibleSensor)
	ON_BN_CLICKED(IDC_CHECK_INVISIBLE_SOUND, &CRollupEntityObjectList::OnBnClickedCheckInvisibleSound)
	ON_BN_CLICKED(IDC_CHECK_INVISIBLE_AREA, &CRollupEntityObjectList::OnBnClickedCheckInvisibleArea)
	ON_BN_CLICKED(IDC_CHECK_INVISIBLE_ROOM, &CRollupEntityObjectList::OnBnClickedCheckInvisibleRoom)
	ON_BN_CLICKED(IDC_BUTTON_NEW_SPAWN, &CRollupEntityObjectList::OnBnClickedButtonNewSpawn)
	ON_BN_CLICKED(IDC_BUTTON_NEW_MARKER, &CRollupEntityObjectList::OnBnClickedButtonNewMarker)
	ON_BN_CLICKED(IDC_BUTTON_NEW_SENSOR, &CRollupEntityObjectList::OnBnClickedButtonNewSensor)
	ON_BN_CLICKED(IDC_BUTTON_ELEMENT_ADD, &CRollupEntityObjectList::OnBnClickedButtonElementAdd)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_ENTITY_LIST_PREVIEW, &CRollupEntityObjectList::OnNMDblclkTreeEntityListPreview)
	ON_BN_CLICKED(IDC_BUTTON_ELEMENT_DEL, &CRollupEntityObjectList::OnBnClickedButtonElementDel)
	ON_LBN_DBLCLK(IDC_ENTITY_EDIT_LIST, &CRollupEntityObjectList::OnLbnDblclkEntityEditList)
	ON_BN_CLICKED(IDC_BUTTON_NEW_SPAWN_MAP_OBJ, &CRollupEntityObjectList::OnBnClickedButtonNewSpawnMapObj)
	ON_BN_CLICKED(IDC_BUTTON_NEW_SOUND, &CRollupEntityObjectList::OnBnClickedButtonNewSound)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_ENTITY_LIST_PREVIEW, &CRollupEntityObjectList::OnNMRClickTreeEntityListPreview)
	ON_BN_CLICKED(IDC_BUTTON_NEW_SPAWN_GATHER, &CRollupEntityObjectList::OnBnClickedButtonNewSpawnGather)
	ON_BN_CLICKED(IDC_BUTTON_NEW_AREA, &CRollupEntityObjectList::OnBnClickedButtonNewArea)
	ON_BN_CLICKED(IDC_BUTTON_NEW_ROOM, &CRollupEntityObjectList::OnBnClickedButtonNewRoom)
	ON_EN_CHANGE(IDC_EDIT_SECTOR_HEIGHT, &CRollupEntityObjectList::OnEnChangeEditSectorHeight)
	ON_BN_CLICKED(IDC_CHECK_VIEW_NPC, &CRollupEntityObjectList::OnBnClickedCheckViewOnlySelected)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CRollupEntityObjectList::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_NPC_RELOAD, &CRollupEntityObjectList::OnBnClickedButtonNpcReload)
END_MESSAGE_MAP()

void CRollupEntityObjectList::Listen(CoreMessage& _message)
{
	switch(_message.nID)
	{
		case VIEW_MESSAGE::DEVICE_CREATED:
		{
			m_pGroupTopBarBtn_ = m_pControlMediator_->GetViewPtr<CCoreToolToolBar>()->m_btnGroup;
		}
		break;
		case VIEW_MESSAGE::ACTIVE_VIEW:
		{
			ASSERT(m_pGroupTopBarBtn_);
			if(m_pGroupTopBarBtn_[WORKBENCH_STATE::OBJECT_NEW].isChecked() )
			{
				OnBnClickedButtonNewSpawn();
			}
			else if(m_pGroupTopBarBtn_[WORKBENCH_STATE::OBJECT_MOVE].isChecked())
			{
				m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_MOVE);
				NoneSelectNewEntityObject_();
				m_editModeLabel_.SetWindowText(_T("Entity Object Move"));
			}
			else if(m_pGroupTopBarBtn_[WORKBENCH_STATE::OBJECT_ROTATE].isChecked())
			{
				m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_ROTATE);
				NoneSelectNewEntityObject_();
				m_editModeLabel_.SetWindowText(_T("Entity Object Rotate"));
			}
			else if(m_pGroupTopBarBtn_[WORKBENCH_STATE::OBJECT_SCALE].isChecked())
			{
				m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_SCALE);
				NoneSelectNewEntityObject_();
				m_editModeLabel_.SetWindowText(_T("Entity Object Scale"));
			}
			else if(m_pGroupTopBarBtn_[WORKBENCH_STATE::OBJECT_SELECT].isChecked())
			{
				m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_SELECT);
				NoneSelectNewEntityObject_();
				m_editModeLabel_.SetWindowText(_T("Entity Object Select"));
			}
		}
		break;
		case VIEW_MESSAGE::SCENE_LOADED:
		{
			SetEnableEntityUI_(false, true);

			if(!LoadBaseEntityObject_())
				AfxMessageBox("���� ������ ��ƼƼ ������Ʈ�� ���ø��� �����ϴ�. ��Ÿ XML�� �ùٸ��� Ȯ���� ������.");
		}
		break;
		case VIEW_MESSAGE::BEFORE_SCENE_CLEAR:
		{
			NoneSelectNewEntityObject_();
			ResetBaseEntityObject_();
		}
		break;
		case VIEW_MESSAGE::LOGIC_LOADED:
		{
			ResetObjectData();
			ResetObjectCheckBox();

			SetEnableEntityUI_(true, true);
			UpdateListBox(true, false);
			UpdateSectorHeight();
			UpdateVisible_();
		}
		break;
		case VIEW_MESSAGE::UNDO_OBJ_NEW_DEL:
		{
			if(m_pControlMediator_->m_mapInfo.m_bLoaded && !m_pControlMediator_->m_mapInfo.m_bSceneEditableMap)
			{
				m_pControlMediator_->GetHelperAction()->RemoveAllSelection(false);
				UpdateListBox(true, true);
			}

			m_pTrackingSpawnObject_ = NULL;
		}
		break;
		case VIEW_MESSAGE::UNDO_ENTITY_EDIT:
		{
			OnLbnSelchangeEntityList();
			// 

		}
		break;

		// TODO, ���߿� Entity�� Logic�� �������� �̰��� ���ش�.
		case VIEW_MESSAGE::LOGIC_RENDER : 
		{
			ENTITY_OBJECTLIST* pEntities = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject();
			LOGIC_OBJECTLIST* pLogics = m_pControlMediator_->GetHelperAction()->GetSelectedLogicObject();

			if (false == pEntities->empty() && true == pLogics->empty() && m_pControlMediator_->GetCurrentWBState() == WORKBENCH_STATE::OBJECT_MOVE)
			{
				for (ENTITY_OBJECTLIST::iterator itr = pEntities->begin(); itr != pEntities->end(); ++itr)
					pLogics->push_back((CLogicObject*)*itr);

				CoreMessage msg(VIEW_MESSAGE::SELECTION_CHANGED);
				m_pControlMediator_->GetViewPtr<VG_Gizmo>()->Listen(msg);
				msg.nID = VIEW_MESSAGE::LOGIC_MOVE;
				m_pControlMediator_->GetViewPtr<VG_Gizmo>()->Listen(msg);
				msg.nID = VIEW_MESSAGE::LOGIC_RENDER;
				m_pControlMediator_->GetViewPtr<VG_Gizmo>()->Listen(msg);

				pLogics->clear();
			}
		}
		break;
		
		case VIEW_MESSAGE::LOGIC_NEW :
		{
			UpdateSpawnVisible(true);
		}
		break;

		case VIEW_MESSAGE::SELECTION_CHANGED:
		{
			UpdateVisibleSelectedFieldNPC();
		}
		break;
	}
}

void CRollupEntityObjectList::SetEnableEntityUI_(bool bEditField, bool bEnableCommonEntity)
{
	int nEnable = bEditField? TRUE : FALSE;
	m_btnEntityObjectNew_[CEntityObject::ENTITY_SPAWN].EnableWindow(nEnable);
	m_btnEntityObjectNew_[CEntityObject::ENTITY_MARKER].EnableWindow(nEnable);
	m_btnEntityObjectNew_[CEntityObject::ENTITY_SENSOR].EnableWindow(nEnable);
	m_btnEntityObjectNew_[CEntityObject::ENTITY_SPAWN_MAP_OBJ].EnableWindow(nEnable);
	m_btnEntityObjectNew_[CEntityObject::ENTITY_SPAWN_GATHER].EnableWindow(nEnable);
	m_btnEntityObjectNew_[CEntityObject::ENTITY_AREA].EnableWindow(nEnable);
	
	m_bEnableCommonEntity_ = bEnableCommonEntity;
	if (bEnableCommonEntity)
	{
		m_btnEntityObjectNew_[CEntityObject::ENTITY_SOUND].EnableWindow(TRUE);
		
	}
	else
	{
		m_editModeLabel_.SetWindowText(_T("Entity Object"));
		m_btnEntityObjectNew_[CEntityObject::ENTITY_SOUND].EnableWindow(FALSE);
	}


	if (bEditField)
	{
		m_btnEntityObjectNew_[CEntityObject::ENTITY_ROOM].EnableWindow(FALSE);
	}
	else
	{
		m_btnEntityObjectNew_[CEntityObject::ENTITY_ROOM].EnableWindow(TRUE);
	}


	{
		DWORD dwButtonID[] = 
		{ 
			IDC_BUTTON_SCRIPT_EDIT, 
			IDC_BUTTON_ELEMENT_ADD,
			IDC_BUTTON_ELEMENT_DEL,
			0
		};

		for ( int i = 0; dwButtonID[i]; ++i )
		{
			CWnd *pWnd = GetDlgItem(dwButtonID[i]);
			if ( pWnd )
				pWnd->EnableWindow(nEnable);
		}
	}
}

void CRollupEntityObjectList::OnCancel()
{
}

void CRollupEntityObjectList::OnOK()
{
}

void CRollupEntityObjectList::OnLbnSelchangeEntityList()
{
	int nSel = m_ctlList_.GetCurSel();
	if ( nSel < 0 )
	{
		ClearTreeMap_();
		return;
	}

	// Get the indexes of all the selected items.
	int nCount = m_ctlList_.GetSelCount();
	CArray<int,int> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	m_ctlList_.GetSelItems(nCount, aryListBoxSel.GetData());

	m_pControlMediator_->GetHelperAction()->RemoveAllSelection(false);

	CEntityObject* pEntityObject = NULL;
	bool bSameType = false;

	ClearTreeMap_();

	BOOL bTreeEnable = TRUE;

	for ( int i = 0; i<nCount; ++i )
	{
		pEntityObject = (CEntityObject*)m_ctlList_.GetItemDataPtr(aryListBoxSel[i]);
		m_pControlMediator_->GetHelperAction()->AddSelectection( pEntityObject, false);
		if(pEntityObject != NULL)
		{
			bSameType = UpdateSelection_(pEntityObject);
			if (pEntityObject->GetEditable()==false)
				bTreeEnable = FALSE;
		}
	}

	if(pEntityObject != NULL)
	{
		ConfirmSelection_(bSameType);
		m_ctlTreePreview_.EnableWindow(bTreeEnable);

		// Custom Selection
		if (nCount == 1)
		{
			if (pEntityObject->GetType() == CEntityObject::ENTITY_AREA ||
				pEntityObject->GetType() == CEntityObject::ENTITY_ROOM)
			{
				CEntityArea* pAreaEntity = static_cast<CEntityArea*>(pEntityObject);
				HTREEITEM hCylinderTreeItem = SetAreaCylinderElement(pAreaEntity);

				if (hCylinderTreeItem != NULL)
				{
					m_ctlTreePreview_.Select(hCylinderTreeItem, TVGN_FIRSTVISIBLE);
					m_ctlTreePreview_.SelectItem(hCylinderTreeItem);
					m_ctlTreePreview_.SetFocus();
				}
				else
				{
					pAreaEntity->m_nSubCylinderSelectedIdx = -1;
					pAreaEntity->UpdateAABB();
				}
			}
		}
	}
}

void CRollupEntityObjectList::UpdateListBox(bool _bLockAndUnlock, bool _bKeepPreSelected)
{
	int nCurSel = -1;
	if (_bKeepPreSelected)
	{
		nCurSel = m_ctlList_.GetCurSel();
	}
	m_ctlList_.ResetContent();
	ClearTreeMap_();

	if( _bLockAndUnlock )
		m_ctlList_.LockWindowUpdate();

	for(int i = 0; i < CEntityObject::ENTITY_OBJ_NUM; ++i)
	{
		CEntityObject::TYPE eCurrentType = (CEntityObject::TYPE)i;
		ENTITY_OBJECTLIST* pList = m_pControlMediator_->GetToolLogicObjectMgr()->GetEntityObjectList<CEntityObject>(eCurrentType);
		if (pList == NULL)
			continue;

		for(ENTITY_OBJECTLIST::iterator itr = pList->begin(); itr != pList->end(); ++itr)
		{
			if( (eCurrentType == CEntityObject::ENTITY_SPAWN && m_bVisibleSpawn_) ||
				(eCurrentType == CEntityObject::ENTITY_MARKER && m_bVisibleMarker_) ||
				(eCurrentType == CEntityObject::ENTITY_SENSOR && m_bVisibleSensor_) ||
				(eCurrentType == CEntityObject::ENTITY_SOUND && m_bVisibleSound_) ||
				(eCurrentType == CEntityObject::ENTITY_AREA && m_bVisibleArea_) ||
				(eCurrentType == CEntityObject::ENTITY_ROOM && m_bVisibleRoom_)
				)
			{
				int nIndex = m_ctlList_.AddString( (*itr)->GetObjectName() );
				m_ctlList_.SetItemDataPtr(nIndex, (void*)(*itr) );
			}
		}
	}

	if (nCurSel >= 0)
	{
		int nListCount = m_ctlList_.GetCount();
		if (nCurSel >= nListCount)
		{
			nCurSel = nListCount - 1;
		}
		m_ctlList_.SetSel(nCurSel, TRUE);
	}

	if( _bLockAndUnlock )
		m_ctlList_.UnlockWindowUpdate();
}

void CRollupEntityObjectList::ResetEntityScript(int _fieldID)
{
	char szBuffer[256];
	szBuffer[0] = '\0';
	sprintf(szBuffer,"Field_%d.lua", _fieldID);

	m_strScriptFilePath_ = szBuffer;
}

void CRollupEntityObjectList::ResetBaseEntityObject_()
{
	for(int i = 0; i < CEntityObject::ENTITY_OBJ_NUM; ++i)
	{
		if(m_arrayBaseEntityObject_[i] != NULL)
		{
			m_arrayBaseEntityObject_[i]->DestroyLogicObject();
			delete m_arrayBaseEntityObject_[i];
			m_arrayBaseEntityObject_[i] = NULL;
		}
	}

	if (m_pSensorWarpCustom_ != NULL)
	{
		m_pSensorWarpCustom_->DestroyLogicObject();
		delete m_pSensorWarpCustom_;
		m_pSensorWarpCustom_ = NULL;
	}

	m_pEntityAreaCustom_ = NULL;
	m_pTrackingSpawnObject_ = NULL;
}

void CRollupEntityObjectList::RemoveTrackingSpawnIf( CEntityObject* _pEntityObject )
{
	if(_pEntityObject != NULL && _pEntityObject->GetType() == CEntityObject::ENTITY_SPAWN)
	{
		if(_pEntityObject == m_pTrackingSpawnObject_ )
			m_pTrackingSpawnObject_ = NULL;
	}
}

// �ʵ� ��ũ��Ʈ ����
void CRollupEntityObjectList::OnBnClickedButtonScriptEdit()
{
	std::string scriptFullPath( "\"" + CXMLConfig::GetInstance()->strServerFieldScriptFullPath + m_strScriptFilePath_ + "\"");
	ShellExecute(NULL, "open", CXMLUserConfig::GetReference().GetTextEditor().c_str(), scriptFullPath.c_str(), NULL, SW_SHOWNORMAL);
}

bool CRollupEntityObjectList::SetSelectEntityObject(CEntityObject* _pEntityObject, BOOL _bFlag)
{
	int nCount = m_ctlList_.GetCount();
	if ( nCount < 1 )
		return false;

	for ( int i = 0; i<nCount; ++i )
	{
		CEntityObject *pListEntityObject = (CEntityObject*)m_ctlList_.GetItemDataPtr(i);
		if ( pListEntityObject == _pEntityObject )
		{
			//���õȰ��� �ִٸ� true
			m_ctlList_.SetSel(i, _bFlag);
			return true;
		}
	}
	//���õȰ��� �����ٸ� false
	return false;
}

void CRollupEntityObjectList::SetSelectNone()
{
	m_ctlList_.SetSel(-1, FALSE);
	UpdateVisibleSelectedFieldNPC();
	ClearTreeMap_();
}

void CRollupEntityObjectList::OnBnClickedCheckInvisibleSpawn()
{
	UpdateData(TRUE);
	UpdateVisible_();

	m_pControlMediator_->GetHelperAction()->RemoveAllSelection(false);
	UpdateListBox(true, false);
}

void CRollupEntityObjectList::OnBnClickedCheckInvisibleMarker()
{
	UpdateData(TRUE);
	UpdateVisible_();

	m_pControlMediator_->GetHelperAction()->RemoveAllSelection(false);
	UpdateListBox(true, false);
}

void CRollupEntityObjectList::OnBnClickedCheckInvisibleSensor()
{
	UpdateData(TRUE);
	UpdateVisible_();

	m_pControlMediator_->GetHelperAction()->RemoveAllSelection(false);
	UpdateListBox(true, false);
}

void CRollupEntityObjectList::OnBnClickedCheckInvisibleSound()
{
	UpdateData(TRUE);
	UpdateVisible_();

	m_pControlMediator_->GetHelperAction()->RemoveAllSelection(false);
	UpdateListBox(true, false);
}

void CRollupEntityObjectList::OnBnClickedCheckInvisibleArea()
{
	UpdateData(TRUE);
	UpdateVisible_();

	m_pControlMediator_->GetHelperAction()->RemoveAllSelection(false);
	UpdateListBox(true, false);
}

void CRollupEntityObjectList::OnBnClickedCheckInvisibleRoom()
{
	UpdateData(TRUE);
	UpdateVisible_();

	m_pControlMediator_->GetHelperAction()->RemoveAllSelection(false);
	UpdateListBox(true, false);
}

void CRollupEntityObjectList::UpdateVisible_()
{
	for(int i = 0; i < CEntityObject::ENTITY_OBJ_NUM; ++i)
	{
		CEntityObject::TYPE eCurrentType = (CEntityObject::TYPE)i;
		ENTITY_OBJECTLIST* pList = m_pControlMediator_->GetToolLogicObjectMgr()->GetEntityObjectList<CEntityObject>(eCurrentType);
		if (pList == NULL)
			continue;

		for(ENTITY_OBJECTLIST::iterator itr = pList->begin(); itr != pList->end(); ++itr)
		{
			if( (eCurrentType == CEntityObject::ENTITY_SPAWN && !m_bVisibleSpawn_) ||
				(eCurrentType == CEntityObject::ENTITY_MARKER && !m_bVisibleMarker_) ||
				(eCurrentType == CEntityObject::ENTITY_SENSOR && !m_bVisibleSensor_) ||
				(eCurrentType == CEntityObject::ENTITY_SOUND && !m_bVisibleSound_) ||
				(eCurrentType == CEntityObject::ENTITY_AREA && !m_bVisibleArea_) ||
				(eCurrentType == CEntityObject::ENTITY_ROOM && !m_bVisibleRoom_)
				)
				(*itr)->SetVisible(false);
			else
				(*itr)->SetVisible(true);
		}
	}

	UpdateSpawnVisible();
}

void CRollupEntityObjectList::SetListLock(bool _bLock)
{
	m_ctlList_.EnableWindow(_bLock ? TRUE : FALSE);
}

bool CRollupEntityObjectList::LoadBaseEntityObject_()
{
	CToolLogicObjectManager* pLogicObjectManager = m_pControlMediator_->GetToolLogicObjectMgr();
	ASSERT(pLogicObjectManager);

	// repository init
	if (pLogicObjectManager->GetEntityObjectRepository().empty())
	{
		pLogicObjectManager->InitLogicObjectManager<CEntityObject>();
	}

	SElementAtXML* pEntitySpawnProperty = pLogicObjectManager->GetPropertyTable("SPAWN");
	if(pEntitySpawnProperty == NULL)
		return false;

	SElementAtXML* pEntityMarkerProperty = pLogicObjectManager->GetPropertyTable("MARKER");
	if(pEntityMarkerProperty == NULL)
		return false;

	SElementAtXML* pEntitySensorProperty = pLogicObjectManager->GetPropertyTable("SENSOR");
	if(pEntitySensorProperty == NULL)
		return false;

	SElementAtXML* pEntitySpawnMapObjectProperty = pLogicObjectManager->GetPropertyTable("MAP_OBJECT");
	if(pEntitySpawnMapObjectProperty == NULL)
		return false;

	SElementAtXML* pEntitySpawnGatherObjectProperty = pLogicObjectManager->GetPropertyTable("GATHER_OBJECT");
	if(pEntitySpawnGatherObjectProperty == NULL)
		return false;

	SElementAtXML* pEntitySensorCustom = pLogicObjectManager->GetPropertyTable("SENSOR_WARP");
	if(pEntitySensorCustom == NULL)
		return false;

	SElementAtXML* pEntitySoundProperty = pLogicObjectManager->GetPropertyTable("SOUND");
	if (pEntitySoundProperty == NULL)
		return false;

	SElementAtXML* pEntityAreaProperty = pLogicObjectManager->GetPropertyTable("AREA");
	if (pEntityAreaProperty == NULL)
		return false;

	SElementAtXML* pRoomProperty = pLogicObjectManager->GetPropertyTable("ROOM");
	if (pRoomProperty == NULL)
		return false;

	_ASSERT(m_pSensorWarpCustom_ == NULL);
	m_pSensorWarpCustom_ = CEntityObject::CreateEntityObject(CEntityObject::ENTITY_SENSOR);
	m_pSensorWarpCustom_->LoadPropertyFromXML(pEntitySensorCustom);

	CEntityObject* pEntityObjectSpawn = CEntityObject::CreateEntityObject(CEntityObject::ENTITY_SPAWN);
	pEntityObjectSpawn->LoadPropertyFromXML(pEntitySpawnProperty);
	m_arrayBaseEntityObject_[CEntityObject::ENTITY_SPAWN] = pEntityObjectSpawn;

	CEntityObject* pEntityObjectMarker = CEntityObject::CreateEntityObject(CEntityObject::ENTITY_MARKER);
	pEntityObjectMarker->LoadPropertyFromXML(pEntityMarkerProperty);
	m_arrayBaseEntityObject_[CEntityObject::ENTITY_MARKER] = pEntityObjectMarker;

	CEntityObject* pEntityObjectSensor = CEntityObject::CreateEntityObject(CEntityObject::ENTITY_SENSOR);
	pEntityObjectSensor->LoadPropertyFromXML(pEntitySensorProperty);
	m_arrayBaseEntityObject_[CEntityObject::ENTITY_SENSOR] = pEntityObjectSensor;

	CEntityObject* pEntityMapObjectSpawn = CEntityObject::CreateEntityObject(CEntityObject::ENTITY_SPAWN);
	pEntityMapObjectSpawn->LoadPropertyFromXML(pEntitySpawnMapObjectProperty);
	m_arrayBaseEntityObject_[CEntityObject::ENTITY_SPAWN_MAP_OBJ] = pEntityMapObjectSpawn;

	CEntityObject* pEntityObjectSound = CEntityObject::CreateEntityObject(CEntityObject::ENTITY_SOUND);
	pEntityObjectSound->LoadPropertyFromXML(pEntitySoundProperty);
	pEntityObjectSound->SettingEditMode();
	m_arrayBaseEntityObject_[CEntityObject::ENTITY_SOUND] = pEntityObjectSound;

	CEntityObject* pEntityGatherObject = CEntityObject::CreateEntityObject(CEntityObject::ENTITY_SPAWN);
	pEntityGatherObject->LoadPropertyFromXML(pEntitySpawnGatherObjectProperty);
	m_arrayBaseEntityObject_[CEntityObject::ENTITY_SPAWN_GATHER] = pEntityGatherObject;

	CEntityObject* pEntityArea = CEntityObject::CreateEntityObject(CEntityObject::ENTITY_AREA);
	pEntityArea->LoadPropertyFromXML(pEntityAreaProperty);
	m_arrayBaseEntityObject_[CEntityObject::ENTITY_AREA] = pEntityArea;
	m_pEntityAreaCustom_ = static_cast<CEntityArea*>(pEntityArea);

	CEntityObject* pRoom = CEntityObject::CreateEntityObject(CEntityObject::ENTITY_ROOM);
	pRoom->LoadPropertyFromXML(pRoomProperty);
	pRoom->SettingEditMode();
	m_arrayBaseEntityObject_[CEntityObject::ENTITY_ROOM] = pRoom;

	return true;
}

void CRollupEntityObjectList::OnBnClickedButtonNewSpawn()
{
	ASSERT(m_pGroupTopBarBtn_);
	if ( !m_btnEntityObjectNew_[CEntityObject::ENTITY_SPAWN].IsWindowEnabled())
		return;

	NoneSelectNewEntityObject_(CEntityObject::ENTITY_SPAWN);
	m_editModeLabel_.SetWindowText("New Spawn");

	m_pGroupTopBarBtn_[WORKBENCH_STATE::OBJECT_NEW].check(true);
	m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_NEW);
}

void CRollupEntityObjectList::OnBnClickedButtonNewSpawnMapObj()
{
	if ( !m_btnEntityObjectNew_[CEntityObject::ENTITY_SPAWN_MAP_OBJ].IsWindowEnabled())
		return;

	NoneSelectNewEntityObject_(CEntityObject::ENTITY_SPAWN_MAP_OBJ);
	m_editModeLabel_.SetWindowText("New MapObject");

	m_pGroupTopBarBtn_[WORKBENCH_STATE::OBJECT_NEW].check(true);
	m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_NEW);
}

void CRollupEntityObjectList::OnBnClickedButtonNewSpawnGather()
{
	if ( !m_btnEntityObjectNew_[CEntityObject::ENTITY_SPAWN_GATHER].IsWindowEnabled())
		return;

	NoneSelectNewEntityObject_(CEntityObject::ENTITY_SPAWN_GATHER);
	m_editModeLabel_.SetWindowText("New GatherObject");

	m_pGroupTopBarBtn_[WORKBENCH_STATE::OBJECT_NEW].check(true);
	m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_NEW);
}

void CRollupEntityObjectList::OnBnClickedButtonNewMarker()
{
	ASSERT(m_pGroupTopBarBtn_);
	if ( !m_btnEntityObjectNew_[CEntityObject::ENTITY_MARKER].IsWindowEnabled())
		return;

	NoneSelectNewEntityObject_(CEntityObject::ENTITY_MARKER);
	m_editModeLabel_.SetWindowText("New Marker");

	m_pGroupTopBarBtn_[WORKBENCH_STATE::OBJECT_NEW].check(true);
	m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_NEW);
}

void CRollupEntityObjectList::OnBnClickedButtonNewSensor()
{
	ASSERT(m_pGroupTopBarBtn_);
	if ( !m_btnEntityObjectNew_[CEntityObject::ENTITY_SENSOR].IsWindowEnabled())
		return;

	NoneSelectNewEntityObject_(CEntityObject::ENTITY_SENSOR);
	m_editModeLabel_.SetWindowText("New Sensor");

	m_pGroupTopBarBtn_[WORKBENCH_STATE::OBJECT_NEW].check(true);
	m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_NEW);
}

void CRollupEntityObjectList::OnBnClickedButtonNewSound()
{
	ASSERT(m_pGroupTopBarBtn_);
	if ( !m_btnEntityObjectNew_[CEntityObject::ENTITY_SOUND].IsWindowEnabled())
		return;

	NoneSelectNewEntityObject_(CEntityObject::ENTITY_SOUND);
	m_editModeLabel_.SetWindowText("New Sound");

	m_pGroupTopBarBtn_[WORKBENCH_STATE::OBJECT_NEW].check(true);
	m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_NEW);
}

void CRollupEntityObjectList::OnBnClickedButtonNewArea()
{
	ASSERT(m_pGroupTopBarBtn_);
	if ( !m_btnEntityObjectNew_[CEntityObject::ENTITY_AREA].IsWindowEnabled())
		return;

	NoneSelectNewEntityObject_(CEntityObject::ENTITY_AREA);
	m_editModeLabel_.SetWindowText("New Area");

	m_pGroupTopBarBtn_[WORKBENCH_STATE::OBJECT_NEW].check(true);
	m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_NEW);
}

void CRollupEntityObjectList::OnBnClickedButtonNewRoom()
{
	ASSERT(m_pGroupTopBarBtn_);
	if ( !m_btnEntityObjectNew_[CEntityObject::ENTITY_ROOM].IsWindowEnabled())
		return;

	NoneSelectNewEntityObject_(CEntityObject::ENTITY_ROOM);
	m_editModeLabel_.SetWindowText("New Room");

	m_pGroupTopBarBtn_[WORKBENCH_STATE::OBJECT_NEW].check(true);
	m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_NEW);
}

void CRollupEntityObjectList::NoneSelectNewEntityObject_(CEntityObject::TYPE _exceptionType)
{
	for (int i = 0; i < CEntityObject::ENTITY_OBJ_NUM; ++i)
	{
		if (i == (int)_exceptionType)
		{
			m_btnEntityObjectNew_[i].check(true);
		}
		else
		{
			m_btnEntityObjectNew_[i].check(false);
		}
	}

	if (m_pEntityAreaCustom_)
		m_pEntityAreaCustom_->m_bCreateShapeMode = false;
}

CEntityObject* CRollupEntityObjectList::GetCurSelBaseEntityObject()
{
	if( m_btnEntityObjectNew_[CEntityObject::ENTITY_SPAWN].isChecked() )
	{
		if(m_pTrackingSpawnObject_ != NULL && m_pTrackingSpawnObject_->GetSubInfo() == SUB_SPAWN)
			return m_pTrackingSpawnObject_;
		else
			return m_arrayBaseEntityObject_[CEntityObject::ENTITY_SPAWN];
	}
	else if (m_btnEntityObjectNew_[CEntityObject::ENTITY_SPAWN_MAP_OBJ].isChecked())
	{
		if(m_pTrackingSpawnObject_ != NULL && m_pTrackingSpawnObject_->GetSubInfo() == SUB_MAP_OBJ)
			return m_pTrackingSpawnObject_;
		else
			return m_arrayBaseEntityObject_[CEntityObject::ENTITY_SPAWN_MAP_OBJ];
	}
	else if (m_btnEntityObjectNew_[CEntityObject::ENTITY_SPAWN_GATHER].isChecked())
	{
		if(m_pTrackingSpawnObject_ != NULL && m_pTrackingSpawnObject_->GetSubInfo() == SUB_GATHER_OBJ)
			return m_pTrackingSpawnObject_;
		else
			return m_arrayBaseEntityObject_[CEntityObject::ENTITY_SPAWN_GATHER];
	}
	else if ( m_btnEntityObjectNew_[CEntityObject::ENTITY_MARKER].isChecked() )
	{
		return m_arrayBaseEntityObject_[CEntityObject::ENTITY_MARKER];
	}
	else if ( m_btnEntityObjectNew_[CEntityObject::ENTITY_SENSOR].isChecked() )
	{
		return m_arrayBaseEntityObject_[CEntityObject::ENTITY_SENSOR];
	}
	else if ( m_btnEntityObjectNew_[CEntityObject::ENTITY_SOUND].isChecked() )
	{
		return m_arrayBaseEntityObject_[CEntityObject::ENTITY_SOUND];
	}
	else if ( m_btnEntityObjectNew_[CEntityObject::ENTITY_AREA].isChecked() )
	{
		return m_arrayBaseEntityObject_[CEntityObject::ENTITY_AREA];
	}
	else if ( m_btnEntityObjectNew_[CEntityObject::ENTITY_ROOM].isChecked() )
	{
		return m_arrayBaseEntityObject_[CEntityObject::ENTITY_ROOM];
	}
	else
		return NULL;
}

void CRollupEntityObjectList::OnBnClickedButtonElementAdd()
{
	if (m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->empty())
		return;

	CEntityObject* pEntity = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->front();

	if (pEntity->GetType() == CEntityObject::ENTITY_SPAWN)
	{
		OnAddSpawnNpcElement_();
	}
	else if (pEntity->GetType() == CEntityObject::ENTITY_SENSOR)
	{
		OnAddSensorWarpElement_();
	}
	else if (pEntity->GetType() == CEntityObject::ENTITY_AREA)
	{
		OnAddAreaCylinderElement_();
	}
	else if (pEntity->GetType() == CEntityObject::ENTITY_ROOM)
	{
		OnAddRoomCylinderElement_();
	}
}

void CRollupEntityObjectList::OnBnClickedButtonElementDel()
{
	HTREEITEM hItem = m_ctlTreePreview_.GetSelectedItem();
	if(hItem == NULL)
	{
		AfxMessageBox("��ƼƼ���� ���� ������Ʈ�� �������ּ���");
		return;
	}

	CString cstrNodeName(m_ctlTreePreview_.GetItemText(hItem));
	if(cstrNodeName.Find("NPC") == 0)
	{
		HTREEITEM hChild = m_ctlTreePreview_.GetNextItem(hItem, TVGN_CHILD);
		CEntityObject::EntityAttributeMap::iterator itr = m_mapTreeViewAssociatedAttributes_.find( hChild );
		if( itr != m_mapTreeViewAssociatedAttributes_.end() )
		{
			bool bOnlyOneSelection = false;
			bool bOnlyOneNpc = false;
			if( m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->size() == 1)
				bOnlyOneSelection = true;

			SElementAtXML::AttributeList& vecAttribute = itr->second;
			for(SElementAtXML::AttributeList::iterator itr_attr = vecAttribute.begin();
				itr_attr != vecAttribute.end(); ++itr_attr)
			{
				//������ attribute �� element ã��
				SElementAtXML* pSelectedNpcElement = (*itr_attr)->m_pClientElement;
				int nNpcIdToDelete = (int)(*pSelectedNpcElement->m_pdElementID);

				for(ENTITY_OBJECTLIST::iterator itr_entity = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->begin();
					itr_entity != m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->end(); ++itr_entity)
				{
					SElementAtXML* pRootElement = (*itr_entity)->GetRootElement();

					//NPC ��常 �߷�����
					std::vector< SElementAtXML* > vecNpcElement;
					std::vector< int > vecNpcIndex;
					int nNpcIndex = 0;
					for(SElementAtXML::ElementList::iterator itr_entity_element = pRootElement->m_vecChildElement.begin();
						itr_entity_element != pRootElement->m_vecChildElement.end(); ++ itr_entity_element, ++nNpcIndex)
					{
						if( (*itr_entity_element)->m_strNodeName == "NPC" )
						{
							vecNpcElement.push_back( *itr_entity_element );
							vecNpcIndex.push_back(nNpcIndex);
						}
					}

					if( vecNpcElement.size() == 1 )
					{
						bOnlyOneNpc = true;
						continue;
					}

					int nNpcIndexOffset = 0;
					for(SElementAtXML::ElementList::iterator itr_npc = vecNpcElement.begin();
						itr_npc != vecNpcElement.end(); ++itr_npc, ++nNpcIndexOffset)
					{
						SElementAtXML* pCurrentNpcElement = *itr_npc;
						if( pSelectedNpcElement == pCurrentNpcElement )
						{
							pCurrentNpcElement->DestroyElement();
							pRootElement->m_vecChildElement.erase( pRootElement->m_vecChildElement.begin() + vecNpcIndex[nNpcIndexOffset] );

							m_pControlMediator_->GetWorkSpacePtr<CWorkSpaceField>()->ClearRenderSceneNodes();

							CEntitySpawn* pEntitySpawn = static_cast<CEntitySpawn*>(*itr_entity);
							pEntitySpawn->RemoveNpc( nNpcIdToDelete, true );
							pEntitySpawn->UpdateColor();

							UpdateSpawnVisible(true);
							break;
						}
						if(bOnlyOneSelection)
							m_pTrackingSpawnObject_ = *itr_entity;
					}
				}
			}

			if(bOnlyOneNpc)
			{
				if(bOnlyOneSelection)
					AfxMessageBox("Spawn �� �ϳ��̻��� NPC�� �־�� �մϴ�.");
				else
					AfxMessageBox("NPC�� �ϳ��� �ִ� Spawn�� ���ؼ��� NPC������ �������� �ʾҽ��ϴ�.");

			}

			m_pControlMediator_->GetToolCmdMgr()->RecordCommand(NULL);
			OnLbnSelchangeEntityList();
		}
	}
	else if (cstrNodeName.Find("WARP") == 0)
	{
		HTREEITEM hChild = m_ctlTreePreview_.GetNextItem(hItem, TVGN_CHILD);
		CEntityObject::EntityAttributeMap::iterator itr = m_mapTreeViewAssociatedAttributes_.find( hChild );
		if( itr != m_mapTreeViewAssociatedAttributes_.end() )
		{
			SElementAtXML::AttributeList& vecAttribute = itr->second;
			for(SElementAtXML::AttributeList::iterator itr_attr = vecAttribute.begin();
				itr_attr != vecAttribute.end(); ++itr_attr)
			{
				//������ attribute �� element ã��
				SElementAtXML* pSelectedWarpElement = (*itr_attr)->m_pClientElement;
				for(ENTITY_OBJECTLIST::iterator itr_entity = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->begin();
					itr_entity != m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->end(); ++itr_entity)
				{
					SElementAtXML* pRootElement = (*itr_entity)->GetRootElement();

					//warp ��常 �߷�����
					std::vector< SElementAtXML* > vecWarpElement;
					std::vector< int > vecWarpIndex;
					int nWarpIndex = 0;
					for(SElementAtXML::ElementList::iterator itr_entity_element = pRootElement->m_vecChildElement.begin();
						itr_entity_element != pRootElement->m_vecChildElement.end(); ++ itr_entity_element, ++nWarpIndex)
					{
						if( (*itr_entity_element)->m_strNodeName == "WARP" )
						{
							vecWarpElement.push_back( *itr_entity_element );
							vecWarpIndex.push_back(nWarpIndex);
						}
					}

					int nWarpIndexOffset = 0;
					for(SElementAtXML::ElementList::iterator itr_npc = vecWarpElement.begin();
						itr_npc != vecWarpElement.end(); ++itr_npc, ++nWarpIndexOffset)
					{
						SElementAtXML* pCurrentWarpElement = *itr_npc;
						if( pSelectedWarpElement == pCurrentWarpElement )
						{
							pCurrentWarpElement->DestroyElement();
							pRootElement->m_vecChildElement.erase( pRootElement->m_vecChildElement.begin() + vecWarpIndex[nWarpIndexOffset] );
							break;
						}
					}
				}
			}
			m_pControlMediator_->GetToolCmdMgr()->RecordCommand(NULL);
			OnLbnSelchangeEntityList();
		}
	}
	else if (cstrNodeName.Find("CYLINDER") == 0)
	{
		HTREEITEM hChild = m_ctlTreePreview_.GetNextItem(hItem, TVGN_CHILD);
		CEntityObject::EntityAttributeMap::iterator itr = m_mapTreeViewAssociatedAttributes_.find( hChild );
		if( itr != m_mapTreeViewAssociatedAttributes_.end() )
		{
			SElementAtXML::AttributeList& vecAttribute = itr->second;
			for(SElementAtXML::AttributeList::iterator itr_attr = vecAttribute.begin();
				itr_attr != vecAttribute.end(); ++itr_attr)
			{
				//������ attribute �� element ã��
				SElementAtXML* pSelectedCylinderElement = (*itr_attr)->m_pClientElement;
				for(ENTITY_OBJECTLIST::iterator itr_entity = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->begin();
					itr_entity != m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->end(); ++itr_entity)
				{
					SElementAtXML* pRootElement = (*itr_entity)->GetRootElement();

					//cylinder ��常 �߷�����
					std::vector< SElementAtXML* > vecCylinderElement;
					std::vector< int > vecCylinderpIndex;
					int nCylinderIndex = 0;
					for(SElementAtXML::ElementList::iterator itr_entity_element = pRootElement->m_vecChildElement.begin();
						itr_entity_element != pRootElement->m_vecChildElement.end(); ++ itr_entity_element, ++nCylinderIndex)
					{
						if( (*itr_entity_element)->m_strNodeName == "CYLINDER" )
						{
							vecCylinderElement.push_back( *itr_entity_element );
							vecCylinderpIndex.push_back(nCylinderIndex);
						}
					}

					int nCyinderIndexOffset = 0;
					for(SElementAtXML::ElementList::iterator itr_cylinder = vecCylinderElement.begin();
						itr_cylinder != vecCylinderElement.end(); ++itr_cylinder, ++nCyinderIndexOffset)
					{
						SElementAtXML* pCurrentCylinderElement = *itr_cylinder;
						if( pSelectedCylinderElement == pCurrentCylinderElement )
						{
							pCurrentCylinderElement->DestroyElement();
							pRootElement->m_vecChildElement.erase( pRootElement->m_vecChildElement.begin() + vecCylinderpIndex[nCyinderIndexOffset] );
							break;
						}
					}
				}
			}
			m_pControlMediator_->GetToolCmdMgr()->RecordCommand(NULL);
			OnLbnSelchangeEntityList();
		}
	}
	else
	{
		AfxMessageBox("����� ���� ������Ʈ�� �����߽��ϴ�.");
	}
}

void CRollupEntityObjectList::OnNMDblclkTreeEntityListPreview(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!m_bEnableCommonEntity_)
		return;

	HTREEITEM hItem = m_ctlTreePreview_.GetSelectedItem();
	if(hItem == NULL) return;

	//������� Ȯ�� - ������ �ƴѿ�Ҵ� Ȯ�� Ȥ�� ���
	HTREEITEM hChild = m_ctlTreePreview_.GetNextItem(hItem, TVGN_CHILD);
	if(hChild == NULL)
	{
		CEntityObject::EntityAttributeMap::iterator itr = m_mapTreeViewAssociatedAttributes_.find( hItem );
		if( itr != m_mapTreeViewAssociatedAttributes_.end() )
		{
			SElementAtXML::AttributeList& vecAttribute = itr->second;
			size_t selectedAttributeSize = vecAttribute.size();
			//�ϳ��϶�
			if( selectedAttributeSize == 1)
				EditEntityAttributeValue_( vecAttribute.begin(), vecAttribute.end(), hItem, false);
			//�������϶�
			else if ( selectedAttributeSize > 1 )
				EditEntityAttributeValue_( vecAttribute.begin(), vecAttribute.end(), hItem, true);
		}
		
		for(ENTITY_OBJECTLIST::iterator itr = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->begin();
			itr != m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->end(); ++itr)
		{
			CEntityObject *pObj = (*itr);
			pObj->Update();
		}
	}

	*pResult = 0;
}

// Tree �� ����Ŭ�� ���� ��, �����Ǵ� �κ�
void CRollupEntityObjectList::EditEntityAttributeValue_(
	SElementAtXML::AttributeList::iterator _itrAttrBegin,
	SElementAtXML::AttributeList::iterator _itrAttrEnd,
	HTREEITEM& _refCurrentTreeItem, bool _bMulti)
{
	CString cstrValue("");
	SAttributeAtXML* pEntityAttribute = *_itrAttrBegin;
	switch(pEntityAttribute->m_eAttrType)
	{
		case SAttributeAtXML::E_POS:
		{
			if(!_bMulti)
			{
				CEntityObject* pEntityObject = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->front();
				_ASSERT(pEntityObject);
				pEntityObject->NotifyPosAttributeDClickedJustOneEntity(pEntityAttribute);
			}
		}
		break;
		case SAttributeAtXML::E_DIR:
		{
			if(!_bMulti)
			{
				CEntityObject* pEntityObject = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->front();
				_ASSERT(pEntityObject);
				pEntityObject->NotifyDirAttributeDClickedJustOneEntity(pEntityAttribute);
			}
		}
		break;
		case SAttributeAtXML::E_UP:
		{
			if(!_bMulti)
			{
				CEntityObject* pEntityObject = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->front();
				_ASSERT(pEntityObject);
				pEntityObject->NotifyUpAttributeDClickedJustOneEntity(pEntityAttribute);
			}
		}
		break;
		case SAttributeAtXML::E_FLOAT:
		{
			float fOldValue = (float)pEntityAttribute->m_vGenericNum[0];

			//�ϳ��̰ų� ������ ����϶�
			if(!_bMulti || m_ctlTreePreview_.GetItemText( _refCurrentTreeItem ).Find(" : _") < 0)
				cstrValue.Format("%2.f", fOldValue);
			
			CEntityObject* pEntityObject = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->front();
			if (pEntityObject->GetType() == CEntityObject::ENTITY_SOUND)
			{
				if (pEntityAttribute->m_strAttrName == "distance_min" ||
					pEntityAttribute->m_strAttrName == "distance_max")
				{
					SElementAtXML::AttributeList& vAttr = pEntityObject->GetRootElement()->m_vecAttribute;
					for (SElementAtXML::AttributeList::iterator itr = vAttr.begin(); itr != vAttr.end(); ++itr)
					{
						SAttributeAtXML* attr = *itr;
						if (attr->m_strAttrName == "BGM" && attr->m_vGenericNum[0] == 1)
							return;
					}
				}
			}

			CDlgInputCommon dlg(cstrValue, pEntityAttribute->m_strAttrName.c_str(), NULL);
			if(dlg.DoModal() == IDOK)
			{
				pEntityAttribute->m_vGenericNum[0] = atof(dlg.GetInputData());

				ICommandBuffer* pCmdBuffer = NULL;
				if(_bMulti)
				{
					pCmdBuffer = new CGroupedCommandBuffer;
					static_cast<CGroupedCommandBuffer*>(pCmdBuffer)->AddBuffer(
						new CEntityAttrGenVec1Buffer(NULL, pEntityAttribute, (double)fOldValue, (pEntityAttribute->m_strAttrName + " ����"))
					);
					//������ ����
					for( _itrAttrBegin++; _itrAttrBegin != _itrAttrEnd; ++_itrAttrBegin)
					{
						SAttributeAtXML* pMultiAttribute = *_itrAttrBegin;
						pMultiAttribute->m_vGenericNum[0] = pEntityAttribute->m_vGenericNum[0];
						static_cast<CGroupedCommandBuffer*>(pCmdBuffer)->AddBuffer(
							new CEntityAttrGenVec1Buffer(NULL, pMultiAttribute, (double)fOldValue, (pMultiAttribute->m_strAttrName + " ����"))
						);
					}
				}
				else
				{
					pCmdBuffer = new CEntityAttrGenVec1Buffer(NULL, pEntityAttribute, fOldValue, (pEntityAttribute->m_strAttrName + " ����"));
					CEntityObject* pEntityObject = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->front();
					if( pEntityObject != NULL)
					{
						pEntityObject->NotifyAttributeChangedJustOneEntity(pEntityAttribute);

						if (pEntityObject->GetType() == CEntityObject::ENTITY_SPAWN)
						{
							m_pTrackingSpawnObject_ = pEntityObject;
						}
						else if (pEntityObject->GetType() == CEntityObject::ENTITY_AREA)
						{
							OnCustomFloatEditArea_(pEntityObject, pEntityAttribute);
						}
						else if (pEntityObject->GetType() == CEntityObject::ENTITY_ROOM)
						{
							OnCustomFloatEditArea_(pEntityObject, pEntityAttribute);
						}
					}
				}

				pEntityAttribute->UpdateMFCTreeItem(m_ctlTreePreview_);
				m_pControlMediator_->GetToolCmdMgr()->RecordCommand(pCmdBuffer);
			}
		}
		break;
		case SAttributeAtXML::E_RADIUS:
		{
			//�ϳ��̰ų� ������ ����϶�
			if(!_bMulti || m_ctlTreePreview_.GetItemText( _refCurrentTreeItem ).Find(" : _") < 0)
				cstrValue.Format("%2.f", pEntityAttribute->m_vGenericNum[0]);
			
			CDlgInputCommon dlg(cstrValue, pEntityAttribute->m_strAttrName.c_str(), NULL);
			if(dlg.DoModal() == IDOK)
			{
				float fNewSize = (float)atof(dlg.GetInputData());
				ENTITY_OBJECTLIST* pSelectedEntityObject = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject();
				CGroupedCommandBuffer* pGroupCmdBuffer = new CGroupedCommandBuffer;
				for(ENTITY_OBJECTLIST::iterator itr = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->begin();
					itr != m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->end(); ++itr)
				{
					CEntityObject* pEntityObject = *itr;
					float fOldSize;
					if (pEntityObject->GetScale(fOldSize) )
					{
						CEntityScaleCmdBuffer* pCmdBuffer = new CEntityScaleCmdBuffer(pEntityObject, fOldSize);
						pGroupCmdBuffer->AddBuffer(pCmdBuffer);
						pEntityObject->SetScale(fNewSize);
					}

					if(_bMulti == false)
					{
						pEntityObject->NotifyAttributeChangedJustOneEntity(pEntityAttribute);

						if (pEntityObject->GetType() == CEntityObject::ENTITY_SPAWN)
						{
							m_pTrackingSpawnObject_ = pEntityObject;
						}
						else if (pEntityObject->GetType() == CEntityObject::ENTITY_AREA)
						{
							_ASSERT(m_pEntityAreaCustom_);
							SElementAtXML::AttributeList& vAttr = m_pEntityAreaCustom_->GetRootElement()->m_vecChildElement[0]->m_vecAttribute;
							for (SElementAtXML::AttributeList::iterator itr = vAttr.begin(); itr != vAttr.end(); ++itr)
							{
								SAttributeAtXML* attr = *itr;
								if (attr->m_strAttrName == "radius")
								{
									attr->m_vGenericNum[0] = (double)fNewSize;
								}
							}
						}
						else if (pEntityObject->GetType() == CEntityObject::ENTITY_ROOM)
						{
							_ASSERT(m_pEntityAreaCustom_);
							SElementAtXML::AttributeList& vAttr = m_pEntityAreaCustom_->GetRootElement()->m_vecChildElement[0]->m_vecAttribute;
							for (SElementAtXML::AttributeList::iterator itr = vAttr.begin(); itr != vAttr.end(); ++itr)
							{
								SAttributeAtXML* attr = *itr;
								if (attr->m_strAttrName == "radius")
								{
									attr->m_vGenericNum[0] = (double)fNewSize;
								}
							}
						}
					}
				}
				pEntityAttribute->UpdateMFCTreeItem(m_ctlTreePreview_);
				m_pControlMediator_->GetToolCmdMgr()->RecordCommand(pGroupCmdBuffer);
			}
		}
		break;
		case SAttributeAtXML::E_STRING:
		{
			//�ϳ��� �����ؼ� �̸��� ����������
			if(pEntityAttribute->m_strAttrName == "name")
			{
				if(_bMulti) return;

				cstrValue = pEntityAttribute->m_strGenericString.c_str();
				CDlgInputCommon dlg(cstrValue, pEntityAttribute->m_strAttrName.c_str(), pEntityAttribute->m_strAttrTypeExtra.c_str());
				if(dlg.DoModal() == IDOK)
				{
					std::string strOldValue(pEntityAttribute->m_strGenericString);

					//����
					pEntityAttribute->m_strGenericString = dlg.GetInputData();

					CEntityAttrGenStringBuffer* pCmdBuffer =
						new CEntityAttrGenStringBuffer(pEntityAttribute, strOldValue, (pEntityAttribute->m_strAttrName + " ����"));

					//UI update
					pEntityAttribute->UpdateMFCTreeItem(m_ctlTreePreview_);
					//�� ������ �̸��� ��� list �̸��� �ٲ��ش�.
					if( pEntityAttribute->m_pClientElement->m_pParentElement == NULL)
					{
						int nCurSel = m_ctlList_.GetCurSel();
						CEntityObject* pEntityObject = (CEntityObject*)m_ctlList_.GetItemDataPtr(nCurSel);

						m_ctlList_.DeleteString(nCurSel);
						m_ctlList_.InsertString(nCurSel, pEntityAttribute->m_strGenericString.c_str());
						m_ctlList_.SetItemDataPtr(nCurSel, (void*)pEntityObject);
						m_ctlList_.SetSel(nCurSel);

						pEntityObject->NotifyAttributeChangedJustOneEntity(pEntityAttribute);
						if(pEntityObject->GetType() == CEntityObject::ENTITY_SPAWN)
							m_pTrackingSpawnObject_ = pEntityObject;
					}

					m_pControlMediator_->GetToolCmdMgr()->RecordCommand(pCmdBuffer);
				}
				return;
			}

			if(pEntityAttribute->m_strAttrName == "color")
			{
				DWORD dwColor = 0;
				CEntityObject* pEntityObject = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->front();
				if(pEntityObject->GetType() == CEntityObject::ENTITY_SPAWN)
					dwColor = static_cast<CEntitySpawn*>(pEntityObject)->GetColor();

				BOOL bAutoColor = pEntityAttribute->m_strGenericString == "auto" ? TRUE : FALSE;
				CDlgInputColorPicker dlg( dwColor, bAutoColor );
				if( dlg.DoModal() == IDOK )
				{
					for(ENTITY_OBJECTLIST::iterator itr = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->begin();
						itr != m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->end(); ++itr)
					{
						//Spawn�� ���ؼ� �۵�
						if( (*itr)->GetType() != CEntityObject::ENTITY_SPAWN ) continue;
						CEntitySpawn* pEntitySpawn = (CEntitySpawn*)(*itr);

						if(dlg.m_nAutoColor == TRUE)
							pEntitySpawn->SetAutoColor();
						else
							pEntitySpawn->SetColor( dlg.GetColorValue() );

						if(_bMulti == false)
						{
							pEntitySpawn->NotifyAttributeChangedJustOneEntity(pEntityAttribute);
							m_pTrackingSpawnObject_ = pEntitySpawn;
						}
					}
					pEntityAttribute->UpdateMFCTreeItem(m_ctlTreePreview_);
				}
				return;
			}

			//�ϳ��̰ų� ������ ����϶�
			if(!_bMulti || m_ctlTreePreview_.GetItemText( _refCurrentTreeItem ).Find(" : _") < 0)
				cstrValue = pEntityAttribute->m_strGenericString.c_str();

			CDlgInputCommon dlg(cstrValue, pEntityAttribute->m_strAttrName.c_str(), pEntityAttribute->m_strAttrTypeExtra.c_str());
			if(dlg.DoModal() == IDOK)
			{
				std::string strOldValue(pEntityAttribute->m_strGenericString);
				pEntityAttribute->m_strGenericString = dlg.GetInputData();
				ICommandBuffer* pCmdBuffer = NULL;
				if(_bMulti)
				{
					pCmdBuffer = new CGroupedCommandBuffer;
					static_cast<CGroupedCommandBuffer*>(pCmdBuffer)->AddBuffer(
						new CEntityAttrGenStringBuffer(pEntityAttribute, strOldValue, (pEntityAttribute->m_strAttrName + " ����"))
					);

					//������ ����
					for( _itrAttrBegin++; _itrAttrBegin != _itrAttrEnd; ++_itrAttrBegin)
					{
						SAttributeAtXML* pMultiAttribute = *_itrAttrBegin;
						pMultiAttribute->m_strGenericString = pEntityAttribute->m_strGenericString;
						static_cast<CGroupedCommandBuffer*>(pCmdBuffer)->AddBuffer(
							new CEntityAttrGenStringBuffer(pMultiAttribute, strOldValue, (pMultiAttribute->m_strAttrName + " ����"))
						);
					}
				}
				else
				{
					pCmdBuffer = new CEntityAttrGenStringBuffer(pEntityAttribute, strOldValue, (pEntityAttribute->m_strAttrName + " ����"));
					CEntityObject* pEntityObject = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->front();
					if( pEntityObject != NULL)
					{
						pEntityObject->NotifyAttributeChangedJustOneEntity(pEntityAttribute);
						if (pEntityObject->GetType() == CEntityObject::ENTITY_SPAWN)
							m_pTrackingSpawnObject_ = pEntityObject;
					}
				}

				pEntityAttribute->UpdateMFCTreeItem(m_ctlTreePreview_);
				m_pControlMediator_->GetToolCmdMgr()->RecordCommand(pCmdBuffer);
			}
		}
		break;
		case SAttributeAtXML::E_UINT:
		{
			if(pEntityAttribute->m_strAttrName == "id")
			{
				//���� id�� �� ���� Attribute�� id���ٸ� EntityObject Numbering�� �ٽ��Ѵ�.
				ASSERT(pEntityAttribute->m_pClientElement);
				if(pEntityAttribute->m_pClientElement->m_pParentElement == NULL && !_bMulti)
				{
					AfxMessageBox(EDITING_ID_WARNING_MESSAGE);

					int nOldValue = (int)pEntityAttribute->m_vGenericNum[0];
					cstrValue.Format("%d", (int)nOldValue);

					CDlgInputCommon dlg(cstrValue, pEntityAttribute->m_strAttrName.c_str(), NULL);
					if(dlg.DoModal() == IDOK)
					{
						int nCurSel = m_ctlList_.GetCurSel();
						CEntityObject* pEntityObject = (CEntityObject*)m_ctlList_.GetItemDataPtr(nCurSel);
						ASSERT(pEntityObject);

						int nNumToReset = atoi(dlg.GetInputData());
						pEntityAttribute->m_vGenericNum[0] = double(nNumToReset);

						CXMLEditInfoFile::GetReference().ResetEntityNumbering<CEntityObject>(pEntityObject->GetType(), nNumToReset);
						pEntityObject->NotifyAttributeChangedJustOneEntity(pEntityAttribute);
						if(pEntityObject->GetType() == CEntityObject::ENTITY_SPAWN)
							m_pTrackingSpawnObject_ = pEntityObject;

						//Object ID�� ������ ���, ID������ �������ص��� ViewPtr��ġ�� �̵��� Object�� ������ �����ش�. 
						m_pControlMediator_->GetToolLogicObjectMgr()->SortEntityObject<CEntityObject>(pEntityObject->GetType());
						m_pControlMediator_->GetViewPtr<CRollupEntityObjectList>()->UpdateListBox(true, false);
						m_pControlMediator_->GetHelperAction()->AddSelectection( pEntityObject, true );
						m_pControlMediator_->GetViewPtr<CRollupEntityObjectList>()->OnLbnSelchangeEntityList();
						
						pEntityAttribute->UpdateMFCTreeItem(m_ctlTreePreview_);
						m_pControlMediator_->GetToolCmdMgr()->RecordCommand(NULL);
					}
					return;
				}

				//Ư����� : NPC�� ���
				if(pEntityAttribute->m_pClientElement->m_strNodeName != "NPC")
					return;

				CDlgInputNpc dlg(m_pControlMediator_, (int)pEntityAttribute->m_vGenericNum[0], "NPC ����");
				if(dlg.DoModal() == IDOK)
				{
					//�ߺ��� NPC id���� Ȯ���ϱ�
					double dIdToChange = (double)dlg.m_nNpcID;
					double dOldValue = pEntityAttribute->m_vGenericNum[0];

					CGroupedCommandBuffer* pCmdBuffer = new CGroupedCommandBuffer;
					bool bHasInvalidNpc = false;

					for(; _itrAttrBegin != _itrAttrEnd; ++_itrAttrBegin)
					{
						SAttributeAtXML* pAttribute = *_itrAttrBegin;
						SElementAtXML* pClientElement = pAttribute->m_pClientElement;

						//���õȰ��� Spawn (�ϴ�, NPC�Ӽ��� spawn �ۿ� ����)
						for(ENTITY_OBJECTLIST::iterator itr_spawn = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->begin();
							itr_spawn != m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->end(); ++itr_spawn)
						{
							CEntitySpawn* pEntitySpawn = static_cast<CEntitySpawn*>(*itr_spawn);
							SElementAtXML* pEntityRootElement = pEntitySpawn->GetRootElement();
							if(pEntityRootElement == NULL) continue;

							SElementAtXML::ElementList npcElementList;
							std::vector<double> vecNpcId;
							for(SElementAtXML::ElementList::iterator itr_entity_ele = pEntityRootElement->m_vecChildElement.begin();
								itr_entity_ele != pEntityRootElement->m_vecChildElement.end(); ++itr_entity_ele)
							{
								if( (*itr_entity_ele)->m_strNodeName == "NPC" && (*itr_entity_ele)->m_pdElementID != NULL )
								{
									npcElementList.push_back( *itr_entity_ele );
									vecNpcId.push_back( *(*itr_entity_ele)->m_pdElementID );
								}
							}

							SElementAtXML::ElementList::iterator itr_npc_ele = std::find( npcElementList.begin(), npcElementList.end(), pClientElement);
							if ( itr_npc_ele != npcElementList.end() )
							{
								std::vector<double>::iterator itr_id = std::find( vecNpcId.begin(), vecNpcId.end(), dIdToChange);
								if( itr_id != vecNpcId.end() )
								{
									bHasInvalidNpc = true;
								}
								else
								{
									pAttribute->m_vGenericNum[0] = dIdToChange;
									pCmdBuffer->AddBuffer(
										new CEntityAttrGenVec1Buffer(pEntitySpawn, pAttribute, dOldValue, (pAttribute->m_strAttrName + " ����"))
									);
									int nNpcIdToAdd = (int)dIdToChange;

									// �������ϴ� ����� Ŭ����
									m_pControlMediator_->GetWorkSpacePtr<CWorkSpaceField>()->ClearRenderSceneNodes();

									pEntitySpawn->RemoveNpc( (int)dOldValue );
									pEntitySpawn->AddNpc( m_pControlMediator_->GetToolSceneManager(), m_pControlMediator_->GetToolLogicObjectMgr(), nNpcIdToAdd);
									pEntitySpawn->UpdateColor();

									UpdateSpawnVisible(true);
								}
								break;
							}
						}
					}

					if(_bMulti == false)
					{
						CEntityObject* pEntityObject = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->front();
						m_pTrackingSpawnObject_ = pEntityObject;
					}
					pEntityAttribute->UpdateMFCTreeItem(m_ctlTreePreview_);
					m_pControlMediator_->GetToolCmdMgr()->RecordCommand(pCmdBuffer);

					if(bHasInvalidNpc)
						AfxMessageBox("�����Ϸ��� ���̵�� ���� NPC�� �����Ǿ� �������� ���� ������ �ֽ��ϴ�.");
				}
			}
			else
			{
				double dOldValue = pEntityAttribute->m_vGenericNum[0];

				//�ϳ��̰ų� ������ ����϶�
				if(!_bMulti || m_ctlTreePreview_.GetItemText( _refCurrentTreeItem ).Find(" : _") < 0)
					cstrValue.Format("%d", (int)dOldValue);

				CDlgInputCommon dlg(cstrValue, pEntityAttribute->m_strAttrName.c_str(), NULL);
				if(dlg.DoModal() == IDOK)
				{
					pEntityAttribute->m_vGenericNum[0] = (double)atoi(dlg.GetInputData());

					ICommandBuffer* pCmdBuffer = NULL;
					if(_bMulti)
					{
						pCmdBuffer = new CGroupedCommandBuffer;
						static_cast<CGroupedCommandBuffer*>(pCmdBuffer)->AddBuffer(
							new CEntityAttrGenVec1Buffer(NULL, pEntityAttribute, dOldValue, (pEntityAttribute->m_strAttrName + " ����"))
						);

						//������ ����
						for( _itrAttrBegin++; _itrAttrBegin != _itrAttrEnd; ++_itrAttrBegin)
						{
							SAttributeAtXML* pMultiAttribute = *_itrAttrBegin;
							pMultiAttribute->m_vGenericNum[0] = pEntityAttribute->m_vGenericNum[0];
							static_cast<CGroupedCommandBuffer*>(pCmdBuffer)->AddBuffer(
								new CEntityAttrGenVec1Buffer(NULL, pMultiAttribute, dOldValue, (pMultiAttribute->m_strAttrName + " ����"))
							);
						}
					}
					else
					{
						CEntityObject* pEntityObject = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->front();
						if( pEntityObject != NULL)
						{
							pEntityObject->NotifyAttributeChangedJustOneEntity(pEntityAttribute);
							if (pEntityObject->GetType() == CEntityObject::ENTITY_SPAWN)
								m_pTrackingSpawnObject_ = pEntityObject;
						}

						pCmdBuffer = new CEntityAttrGenVec1Buffer(NULL, pEntityAttribute, dOldValue, (pEntityAttribute->m_strAttrName + " ����"));
					}

					pEntityAttribute->UpdateMFCTreeItem(m_ctlTreePreview_);
					m_pControlMediator_->GetToolCmdMgr()->RecordCommand(pCmdBuffer);
				}
			}
		}
		break;
		case SAttributeAtXML::E_BOOL:
		{
			double dOldValue = pEntityAttribute->m_vGenericNum[0];

			CDlgInputBool dlg(pEntityAttribute->m_strAttrName.c_str(), (int)dOldValue);
			if(dlg.DoModal() == IDOK)
			{
				pEntityAttribute->m_vGenericNum[0] = (double)dlg.m_nBoolValue;
				ICommandBuffer* pCmdBuffer = NULL;
				if(_bMulti)
				{
					pCmdBuffer = new CGroupedCommandBuffer;
					static_cast<CGroupedCommandBuffer*>(pCmdBuffer)->AddBuffer(
						new CEntityAttrGenVec1Buffer(NULL, pEntityAttribute, dOldValue, (pEntityAttribute->m_strAttrName + " ����"))
					);
					//������ ����
					for( _itrAttrBegin++; _itrAttrBegin != _itrAttrEnd; ++_itrAttrBegin)
					{
						SAttributeAtXML* pMultiAttribute = *_itrAttrBegin;
						pMultiAttribute->m_vGenericNum[0] = pEntityAttribute->m_vGenericNum[0];
						static_cast<CGroupedCommandBuffer*>(pCmdBuffer)->AddBuffer(
							new CEntityAttrGenVec1Buffer(NULL, pMultiAttribute, dOldValue, (pMultiAttribute->m_strAttrName + " ����"))
						);
					}
				}
				else
				{
					CEntityObject* pEntityObject = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->front();
					if( pEntityObject != NULL)
					{
						pEntityObject->NotifyAttributeChangedJustOneEntity(pEntityAttribute);
						if (pEntityObject->GetType() == CEntityObject::ENTITY_SPAWN)
							m_pTrackingSpawnObject_ = pEntityObject;
						else if (pEntityObject->GetType() == CEntityObject::ENTITY_SOUND)
						{
							if (pEntityAttribute->m_strAttrName == "BGM" &&
								pEntityAttribute->m_vGenericNum[0] == 1)
							{
								SElementAtXML::AttributeList& vAttr = pEntityObject->GetRootElement()->m_vecAttribute;
								for (SElementAtXML::AttributeList::iterator itr = vAttr.begin(); itr != vAttr.end(); ++itr)
								{
									SAttributeAtXML* attr = *itr;
									if (attr->m_strAttrName == "distance_min")
									{
										attr->m_vGenericNum[0] = 0;
										attr->UpdateMFCTreeItem(m_ctlTreePreview_);
									}
									else if (attr->m_strAttrName == "distance_max")
									{
										attr->m_vGenericNum[0] = 0;
										attr->UpdateMFCTreeItem(m_ctlTreePreview_);
										break;
									}
								}
							}
						}
					}

					pCmdBuffer = new CEntityAttrGenVec1Buffer(NULL, pEntityAttribute, dOldValue, (pEntityAttribute->m_strAttrName + " ����"));
				}

				pEntityAttribute->UpdateMFCTreeItem(m_ctlTreePreview_);
				m_pControlMediator_->GetToolCmdMgr()->RecordCommand(pCmdBuffer);
			}
		}
		break;
	}
}

void CRollupEntityObjectList::OnLbnDblclkEntityEditList()
{
	rs3::RToolCamera* pCamear = m_pControlMediator_->GetToolCamera();
	if( pCamear == NULL)
		return;

	if(m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->size() == 0) return;

	CEntityObject* pEntityObject = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->front();
	if( pEntityObject == NULL )
		return;

	rs3::RVector vecSelected;
	if (pEntityObject->GetPosition(vecSelected) == false )
		return;

	pCamear->SetPositionToViewCenter( vecSelected, Tool3Const::CONST_WB_ENTITY_OBJ_TO_CENTER );
}

void CRollupEntityObjectList::SetEditingModeReadOnly(bool _bReadOnly, bool _bShowSpawn)
{
	if (_bReadOnly)
	{
		if (false == m_bEnableReadOnly_)
		{
			m_bVisibleSpawnOld_  =  m_bVisibleSpawn_;
			m_bVisibleMarkerOld_ =	m_bVisibleMarker_;
			m_bVisibleSensorOld_ =	m_bVisibleSensor_;
			m_bVisibleSoundOld_  =	m_bVisibleSound_;
			m_bVisibleAreaOld_   =	m_bVisibleArea_;
			m_bVisibleRoomOld_   = 	m_bVisibleRoom_;

			m_bHideSpawnHandlerOld_ = m_checkHideSpawnHandler.GetCheck();
			m_bViewMapObjHandlerOld_ = m_checkViewMapObjHandler.GetCheck();

			m_bEnableReadOnly_ = true;
		}

		SetEnableEntityUI_(false, false);

		m_bVisibleMarker_ = FALSE;
		m_bVisibleSensor_ = FALSE;
		m_bVisibleSound_ = FALSE;
		m_bVisibleArea_ = FALSE;
		m_bVisibleRoom_ = FALSE;

		m_checkHideSpawnHandler.SetCheck(TRUE);
		m_checkViewMapObjHandler.SetCheck(FALSE);

		// spawn entity view
		SetRenderSpawnEntity(_bShowSpawn);

		for(int i = 0; i < CEntityObject::ENTITY_OBJ_NUM; ++i)
		{
			CEntityObject::TYPE eCurrentType = (CEntityObject::TYPE)i;
			ENTITY_OBJECTLIST* pList = m_pControlMediator_->GetToolLogicObjectMgr()->GetEntityObjectList<CEntityObject>(eCurrentType);
			if (pList == NULL)
				continue;

			for(ENTITY_OBJECTLIST::iterator itr = pList->begin(); itr != pList->end(); ++itr)
			{
				// ���� �Ұ�
				(*itr)->SetEditable(false);
			}
		}
	}
	else
	{
		if (true == m_bEnableReadOnly_)
		{
			m_bVisibleSpawn_  =  m_bVisibleSpawnOld_;
			m_bVisibleMarker_ =  m_bVisibleMarkerOld_;
			m_bVisibleSensor_ =  m_bVisibleSensorOld_;
			m_bVisibleSound_  =  m_bVisibleSoundOld_;
			m_bVisibleArea_   =  m_bVisibleAreaOld_;
			m_bVisibleRoom_   =  m_bVisibleRoomOld_;

			m_checkHideSpawnHandler.SetCheck(m_bHideSpawnHandlerOld_);
			m_checkViewMapObjHandler.SetCheck(m_bViewMapObjHandlerOld_);

			m_bEnableReadOnly_ = false;

			UpdateData(FALSE);
		}
	}
}

void CRollupEntityObjectList::SetRenderSpawnEntity( bool _bShowSpawn )
{
	m_bVisibleSpawn_ = _bShowSpawn ? TRUE : FALSE;
	UpdateData(FALSE);
	UpdateVisible_();
}

void CRollupEntityObjectList::OnAddSpawnNpcElement_()
{
	CDlgInputNpc dlg(m_pControlMediator_, 1, "NPC �߰�");
	if(dlg.DoModal() == IDOK)
	{
		int nNpcIdToAdd = dlg.m_nNpcID;
		int nSpawnNum = 0;
		CEntityObject* pEntityObjectToTracking = NULL;
		for(ENTITY_OBJECTLIST::iterator itr = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->begin();
			itr != m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->end(); ++itr)
		{
			if( (*itr)->GetType() == CEntityObject::ENTITY_SPAWN)
			{
				CEntitySpawn* pEntitySpawn = static_cast<CEntitySpawn*>(*itr);

				std::vector<double> vecNpcId;
				for(SElementAtXML::ElementList::iterator itr_attr = pEntitySpawn->GetRootElement()->m_vecChildElement.begin();
					itr_attr != pEntitySpawn->GetRootElement()->m_vecChildElement.end(); ++itr_attr)
				{
					SElementAtXML* pEntityElement = *itr_attr;
					if( pEntityElement->m_strNodeName == "NPC" )
						vecNpcId.push_back( *pEntityElement->m_pdElementID );
				}

				//���� NPC�� �ִٸ� �������� �ʴ´�.
				std::vector<double>::iterator itr_npc = std::find( vecNpcId.begin(), vecNpcId.end(), nNpcIdToAdd);
				if(itr_npc != vecNpcId.end())
					continue;

				SElementAtXML* pRootProperty = m_arrayBaseEntityObject_[CEntityObject::ENTITY_SPAWN]->GetRootElement();
				ASSERT(pRootProperty);
				SElementAtXML* pNpcProperty = NULL;

				//�⺻ ���̽� EntityProperty���� NPC Property ��������
				for(SElementAtXML::ElementList::iterator itr_element = pRootProperty->m_vecChildElement.begin();
					itr_element != pRootProperty->m_vecChildElement.end(); ++itr_element)
				{
					if( (*itr_element)->m_strNodeName == "NPC" )
						pNpcProperty = *itr_element;
				}


				if(pNpcProperty != NULL)
				{
					SElementAtXML* pNpcElement = new SElementAtXML;
					pNpcElement->m_strNodeName = pNpcProperty->m_strNodeName;

					CEntityObject::CopyElement(pEntitySpawn, pNpcProperty, pNpcElement, false);
					*pNpcElement->m_pdElementID = nNpcIdToAdd;

					// NPC �߰�
					pEntitySpawn->AddNpc( m_pControlMediator_->GetToolSceneManager(), m_pControlMediator_->GetToolLogicObjectMgr(), nNpcIdToAdd );

					//�߰��ϴ� NPC�� ���� "default" ���� false��
					for(SElementAtXML::AttributeList::iterator itr = pNpcElement->m_vecAttribute.begin();
						itr != pNpcElement->m_vecAttribute.end(); ++itr)
					{
						if( (*itr)->m_strAttrName == "default" )
							(*itr)->m_vGenericNum[0] = 0;
					}

					pEntitySpawn->GetRootElement()->m_vecChildElement.push_back(pNpcElement);
					pNpcElement->m_pParentElement = pEntitySpawn->GetRootElement();
					++nSpawnNum;
					pEntityObjectToTracking = pEntitySpawn;
				}
			}
		}
		if(nSpawnNum == 1)
			m_pTrackingSpawnObject_ = pEntityObjectToTracking;

		m_pControlMediator_->GetToolCmdMgr()->RecordCommand(NULL);
		OnLbnSelchangeEntityList();
	}
}

void CRollupEntityObjectList::OnAddSensorWarpElement_()
{
	CDlgInputSensorWarp* pDlg = CMFCHelper::CreateDLLDlgModalOKed<CDlgInputSensorWarp>(m_pControlMediator_);
	if(pDlg)
	{
		for(ENTITY_OBJECTLIST::iterator itr = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->begin();
			itr != m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->end(); ++itr)
		{
			if( (*itr)->GetType() == CEntityObject::ENTITY_SENSOR)
			{
				CEntitySensor* pEntitySensor = static_cast<CEntitySensor*>(*itr);

				SElementAtXML* pRootProperty = m_pSensorWarpCustom_->GetRootElement();
				ASSERT(pRootProperty);

				SElementAtXML* pWarpProperty = NULL;

				//�⺻ ���̽� EntityProperty���� Sensor Warp Property ��������
				for(SElementAtXML::ElementList::iterator itr_element = pRootProperty->m_vecChildElement.begin();
					itr_element != pRootProperty->m_vecChildElement.end(); ++itr_element)
				{
					if( (*itr_element)->m_strNodeName == "WARP" )
						pWarpProperty = *itr_element;
				}

				if(pWarpProperty != NULL)
				{
					SElementAtXML* pWarpElement = new SElementAtXML;
					pWarpElement->m_strNodeName = pWarpProperty->m_strNodeName;

					CEntityObject::CopyElement(pEntitySensor, pWarpProperty, pWarpElement, false);

					// �����ʵ�/Ʈ���̾��ʵ� ���п� ���� ������� �ʴ� �Ӽ� ����
					SElementAtXML::AttributeList::iterator itr = pWarpElement->m_vecAttribute.begin();
					while(itr != pWarpElement->m_vecAttribute.end())
					{
						SAttributeAtXML* pAttribute = *itr;
						bool isUnusedAttribute = false;

						if (pDlg->m_nWarpFieldType == 0)
						{
							if (pAttribute->m_strAttrName == "field_group" || pAttribute->m_strAttrName == "quest")
							{
								isUnusedAttribute = true;
							}
						}
						else if (pDlg->m_nWarpFieldType == 1)
						{
							if (pAttribute->m_strAttrName == "field" || pAttribute->m_strAttrName == "marker")
							{
								isUnusedAttribute = true;
							}
						}

						if (isUnusedAttribute)
						{
							SAFE_DELETE(pAttribute);
							itr = pWarpElement->m_vecAttribute.erase(itr);
						}
						else
						{
							itr++;
						}
					}


					// �Ӽ��� �Է�
					for(SElementAtXML::AttributeList::iterator itr = pWarpElement->m_vecAttribute.begin();
						itr != pWarpElement->m_vecAttribute.end(); ++itr)
					{
						SAttributeAtXML* pAttribute = *itr;

						// ���� ���
						if ( pAttribute->m_strAttrName == "type" )
						{
							string strString = "field";
							if (pDlg->m_nGateType == 1)
							{
								strString = "innroom";
							}

							pAttribute->m_strGenericString = strString;
						}
						else if ( pAttribute->m_strAttrName == "title" )
						{
							pAttribute->m_strGenericString = pDlg->m_strTitle.GetString();
						}
						else if( pAttribute->m_strAttrName == "condition" )
						{
							pAttribute->m_vGenericNum[0] = (float)pDlg->m_nCondition;
						}
						else if (pAttribute->m_strAttrName == "gate_type")
						{
							string strString = "auto";

							if (pDlg->m_nGateType == 1)
							{
								strString = "manual";
							}

							pAttribute->m_strGenericString = strString;
						}
						else if (pAttribute->m_strAttrName == "field")
						{
							pAttribute->m_vGenericNum[0] = (float)pDlg->m_nField;
						}
						else if ( pAttribute->m_strAttrName == "marker" )
						{
							pAttribute->m_vGenericNum[0] = (float)pDlg->m_nMarker;
						}
						else if ( pAttribute->m_strAttrName == "field_group")
						{
							pAttribute->m_vGenericNum[0] = (float)pDlg->m_nFieldGroupID;
						}
						else if (pAttribute->m_strAttrName == "quest")
						{
							pAttribute->m_vGenericNum[0] = (float)pDlg->m_nQuestID;
						}
					}

					pEntitySensor->GetRootElement()->m_vecChildElement.push_back(pWarpElement);
					pWarpElement->m_pParentElement = pEntitySensor->GetRootElement();
				}
			}
		}
		OnLbnSelchangeEntityList();

		delete pDlg;
	}
}

void CRollupEntityObjectList::OnNMRClickTreeEntityListPreview(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!m_bEnableCommonEntity_)
		return;

	CPoint ptScreen;
	if (!GetCursorPos(&ptScreen))
		return;

	CPoint ptClient(ptScreen);
	m_ctlTreePreview_.ScreenToClient(&ptClient);

	UINT flags;
	HTREEITEM hItem = m_ctlTreePreview_.HitTest(ptClient, &flags);
	if(hItem == NULL)
		return;

	m_ctlTreePreview_.SelectItem(hItem);

	//������� Ȯ��
	HTREEITEM hChild = m_ctlTreePreview_.GetNextItem(hItem, TVGN_CHILD);
	if(hChild == NULL)
		return;

	int nSelectedCnt = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->size();
	if (1 != nSelectedCnt)
		return;

	CEntityObject* pEntity = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject()->front();
	_ASSERT(pEntity);
	if (NULL == pEntity)
		return;

	// change for TM setting
	using namespace rs3;
	bool bChangeEntityEditingMode = false;
	RVector3 vLocalPos(RVector3::ZERO);
	std::string strSelectedNodeName( m_ctlTreePreview_.GetItemText(hItem).GetString() );
	pEntity->NotifyPosAttributeDClickedJustOneEntity(NULL);
	if (strSelectedNodeName == "EFFECT")
	{
		pEntity->GetPosition(vLocalPos);

		// ������ ���� �ٲ��� �ϱ� ���� position�� �����ߴµ� direction�� �����ߴٰ� �Ͽ� ���ú���
		bChangeEntityEditingMode = pEntity->NotifyDirAttributeDClickedJustOneEntity(pEntity->GetPosAttribute());

		// ����Ʈ���� �����Ǿ� ���� �ʾ� ���� �ٲ��� �ȵ� ���
		if (!bChangeEntityEditingMode)
		{
			AfxMessageBox("Effect�� �����Ǿ� ���� �ʽ��ϴ�.");
			return;
		}
	}

	// get TM for dialog
	RVector3 *pPos(NULL), *pDir(NULL), *pUp(NULL);
	RVector3 vWorldPos(RVector3::ZERO), vPos(RVector3::ZERO), vPosLocal(RVector3::ZERO), vDir(RVector3::AXISY), vUp(RVector3::AXISZ);

	if (pEntity->GetPosition(vWorldPos))
	{
		vPos = vWorldPos - vLocalPos;
		pPos = &vPos;
	}

	if (pEntity->GetDirection(vDir))
		pDir = &vDir;

	if (pEntity->GetUp(vUp))
		pUp = &vUp;

	CDlgInputTransform dlg((strSelectedNodeName+" Ʈ������ ����").c_str(), pPos, pDir, pUp);
	if (dlg.DoModal() == IDOK)
	{
		CGroupedCommandBuffer* pGroupCmdBuffer = new CGroupedCommandBuffer;
		pGroupCmdBuffer->AddBuffer( new  CLogicRotateCmdBuffer(pEntity, vDir, vUp) );
		pGroupCmdBuffer->AddBuffer( new  CEntityMoveCmdBuffer(pEntity, vWorldPos) );
		m_pControlMediator_->GetToolCmdMgr()->RecordCommand(pGroupCmdBuffer);

		pEntity->SetPosition( vLocalPos + dlg.GetPos() );
		pEntity->SetDirectionUp( dlg.GetDir().Normalize(), dlg.GetUp().Normalize() );

		OnLbnSelchangeEntityList();
		if (bChangeEntityEditingMode)
		{
			// ������ ���� �ٲ��� �ϱ� ���� position�� �����ߴµ� direction�� �����ߴٰ� �Ͽ� ���ú���
			pEntity->NotifyDirAttributeDClickedJustOneEntity(pEntity->GetPosAttribute());
		}
	}
	else
	{
		OnLbnSelchangeEntityList();
	}

	*pResult = 0;
}

void CRollupEntityObjectList::SetVisibleFieldNPC( bool bRenderingOnlySelected )
{
	// Ŭ����
	m_pControlMediator_->GetWorkSpacePtr<CWorkSpaceField>()->ClearRenderSceneNodes();

	// ���
	CToolLogicObjectManager* pLogicObjectMgr = m_pControlMediator_->GetToolLogicObjectMgr();
	if (bRenderingOnlySelected == false)
	{
		ENTITY_OBJECTLIST* pList = m_pControlMediator_->GetToolLogicObjectMgr()->GetEntityObjectList<CEntityObject>(CEntityObject::ENTITY_SPAWN);
		if (pList != NULL)
		{
			for(ENTITY_OBJECTLIST::iterator itr = pList->begin(); itr != pList->end(); ++itr)
			{
				CEntitySpawn* pEntitySpawn = (CEntitySpawn*)(*itr);

				if( pEntitySpawn->GetVisible() == false)
					continue;

				RSceneNode* pNpcSceneNode = pEntitySpawn->GetNpcToRender();
				if (pNpcSceneNode)
				{
					m_pControlMediator_->GetWorkSpacePtr<CWorkSpaceField>()->RegisterToRenderSceneNode(pNpcSceneNode);
				}
			}
		}
	}
	else
	{
		// ������ �͸� ������
		ENTITY_OBJECTLIST* pListSelectedEntityObject = m_pControlMediator_->GetHelperAction()->GetSelectedEntityObject();
		for(ENTITY_OBJECTLIST::iterator itr = pListSelectedEntityObject->begin();
			itr != pListSelectedEntityObject->end(); ++itr)
		{
			CEntityObject* pEntityObject = *itr;
			if( pEntityObject->GetVisible() == false)
				continue;  //�׳� ��ŵ

			if(pEntityObject->GetType() == CEntityObject::ENTITY_SPAWN)
			{
				CEntitySpawn* pEntitySpawn = static_cast<CEntitySpawn*>(pEntityObject);
				pEntitySpawn->RenderNpcForSelection(m_pControlMediator_);
			}
		}
	}
}

bool CRollupEntityObjectList::IsCustomObjectNewMode()
{
	if (m_pEntityAreaCustom_)
		return m_pEntityAreaCustom_->m_bCreateShapeMode;

	return false;
}

void CRollupEntityObjectList::SetToCustomObjectNewMode()
{
	if (m_pEntityAreaCustom_)
		m_pEntityAreaCustom_->m_bCreateShapeMode = true;
}

void CRollupEntityObjectList::OnAddAreaCylinderElement_()
{
	OnBnClickedButtonNewArea();
	SetToCustomObjectNewMode();

	m_pControlMediator_->m_mouseInfo.m_bPopAble = false;
	m_pControlMediator_->m_mouseInfo.mouseState.bRButton = false;
	L3DCursor::GetInstance()->SetType(L3DCursor::RCONE, false);
	L3DCursor::GetInstance()->SetColor(D3DCOLOR_XRGB(128, 128, 128));
}

void CRollupEntityObjectList::OnAddRoomCylinderElement_()
{
	OnBnClickedButtonNewRoom();
	SetToCustomObjectNewMode();

	m_pControlMediator_->m_mouseInfo.m_bPopAble = false;
	m_pControlMediator_->m_mouseInfo.mouseState.bRButton = false;
	L3DCursor::GetInstance()->SetType(L3DCursor::RCONE, false);
	L3DCursor::GetInstance()->SetColor(D3DCOLOR_XRGB(128, 128, 128));
}

HTREEITEM CRollupEntityObjectList::SetAreaCylinderElement( CEntityArea* _pArea )
{
	HTREEITEM hTreeItem = NULL;
	const RVector *pOrigin(NULL), *pTo(NULL);

	m_pControlMediator_->GetHelperAction()->SetMouseLineAndPickingPosition();
	m_pControlMediator_->GetHelperAction()->GetCurrentMouseLine(&pOrigin, &pTo);

	if (pOrigin && pTo)
	{
		std::vector<std::pair<SAttributeAtXML*,MBox>> vBoxes;
		_pArea->GetCylinderAABBs(vBoxes);
		int i = 0;
		for (std::vector<std::pair<SAttributeAtXML*,MBox>>::iterator itr = vBoxes.begin(); itr != vBoxes.end(); ++itr)
		{
			if (itr->second.ContainsLineSegment(*pOrigin, *pTo))
			{
				hTreeItem = m_ctlTreePreview_.GetParentItem(itr->first->m_hTreeItem);

				_pArea->m_CylinderPos = itr->second.GetCenter();
				_pArea->m_CylinderPos.z = itr->second.minz;
				_pArea->m_nSubCylinderSelectedIdx = i;
				break;
			}

			++i;
		}
	}

	return hTreeItem;
}

void CRollupEntityObjectList::UpdateAreaCylinderUI( SAttributeAtXML* _pCylinderAttribute )
{
	if (_pCylinderAttribute)
	{
		_pCylinderAttribute->UpdateMFCTreeItem(m_ctlTreePreview_);
	}
}

void CRollupEntityObjectList::OnCustomFloatEditArea_( CEntityObject* _pArea, SAttributeAtXML* _pEditedAttribute )
{
	if (_pEditedAttribute->m_strAttrName == "minz_hint")
	{
		SElementAtXML* pRootElement = _pArea->GetRootElement();
		for (SElementAtXML::ElementList::iterator itr_cyl = pRootElement->m_vecChildElement.begin();
			itr_cyl != pRootElement->m_vecChildElement.end(); ++itr_cyl)
		{
			SElementAtXML* pCylElement = *itr_cyl;
			for (SElementAtXML::AttributeList::iterator itr_cyl_elem = pCylElement->m_vecAttribute.begin();
				itr_cyl_elem != pCylElement->m_vecAttribute.end(); ++itr_cyl_elem)
			{
				SAttributeAtXML* attr = *itr_cyl_elem;
				if (attr->m_strAttrName == "Position")
				{
					attr->m_vGenericNum[2] = _pEditedAttribute->m_vGenericNum[0];
					attr->UpdateMFCTreeItem(m_ctlTreePreview_);
					break;
				}
			}
		}
	}
	else if (_pEditedAttribute->m_strAttrName == "maxz_hint")
	{
		SElementAtXML* pRootElement = _pArea->GetRootElement();
		for (SElementAtXML::ElementList::iterator itr_cyl = pRootElement->m_vecChildElement.begin();
			itr_cyl != pRootElement->m_vecChildElement.end(); ++itr_cyl)
		{
			SElementAtXML* pCylElement = *itr_cyl;
			double dZ = 0;
			for (SElementAtXML::AttributeList::iterator itr_cyl_elem = pCylElement->m_vecAttribute.begin();
				itr_cyl_elem != pCylElement->m_vecAttribute.end(); ++itr_cyl_elem)
			{
				SAttributeAtXML* attr = *itr_cyl_elem;
				if (attr->m_strAttrName == "Position")
				{
					dZ = attr->m_vGenericNum[2];
				}
				else if (attr->m_strAttrName == "height")
				{
					attr->m_vGenericNum[0] = _pEditedAttribute->m_vGenericNum[0] - dZ;
					attr->UpdateMFCTreeItem(m_ctlTreePreview_);
					break;
				}
			}
		}
	}
	else
	{
		// remember last edit value
		_ASSERT(m_pEntityAreaCustom_);
		SElementAtXML::AttributeList& vAttr = m_pEntityAreaCustom_->GetRootElement()->m_vecChildElement[0]->m_vecAttribute;
		for (SElementAtXML::AttributeList::iterator itr = vAttr.begin(); itr != vAttr.end(); ++itr)
		{
			SAttributeAtXML* attr = *itr;
			if (attr->m_strAttrName == _pEditedAttribute->m_strAttrName)
			{
				attr->m_vGenericNum[0] = _pEditedAttribute->m_vGenericNum[0];
				break;
			}
		}
	}
}

void CRollupEntityObjectList::OnEnChangeEditSectorHeight()
{
	float fOldSectorHeight = m_fSectorHeight;
	UpdateData(TRUE);

	if (m_fSectorHeight < 0.0f)
	{
		AfxMessageBox("SectorHeight�� ������ �� �� �����ϴ�.");
		m_fSectorHeight = fOldSectorHeight;
		
		UpdateData(FALSE);
	}

	CWorkSpaceField* pField = m_pControlMediator_->GetWorkSpacePtr<CWorkSpaceField>();
	_ASSERT(pField);

	pField->SetSectorHeight(m_fSectorHeight);
}

void CRollupEntityObjectList::UpdateSectorHeight()
{
	CWorkSpaceField* pField = m_pControlMediator_->GetWorkSpacePtr<CWorkSpaceField>();
	_ASSERT(pField);

	m_fSectorHeight = pField->GetSectorHeight();
}

void CRollupEntityObjectList::UpdateSpawnVisible( bool bForce /*= false*/ )
{
	// �ɼ� ���濡 ���� ������Ʈ �Լ�
	if(m_bPreVisibleSpawn_ != m_bVisibleSpawn_ ||
		m_bPreViewOnlySelected != (BOOL)IsRenderingOnlySelected() ||
		bForce == true)
	{
		// npc visible Update
		SetVisibleFieldNPC(IsRenderingOnlySelected());

		m_bPreVisibleSpawn_ = m_bVisibleSpawn_;
		m_bPreViewOnlySelected = IsRenderingOnlySelected();
	}
}

void CRollupEntityObjectList::OnBnClickedCheckViewOnlySelected()
{
	UpdateData(TRUE);
	UpdateVisible_();
}

void CRollupEntityObjectList::UpdateVisibleSelectedFieldNPC()
{
	// ���õ� npc�� visible�϶����� ������Ʈ �Լ� (����, �������)
	if(IsRenderingOnlySelected() == false)
		return;

	SetVisibleFieldNPC(true);
}

void CRollupEntityObjectList::OnSearchObjectList()
{
	CString strBuffer;
	m_editSearchList.GetWindowText(strBuffer);

	vector<string> vecObjectList;
	ParseFindString((LPSTR)(LPCSTR)strBuffer, vecObjectList);

	m_ctlList_.ResetContent();
	ClearTreeMap_();

	m_ctlList_.LockWindowUpdate();

	for(int i = 0; i < CEntityObject::ENTITY_OBJ_NUM; ++i)
	{
		CEntityObject::TYPE eCurrentType = (CEntityObject::TYPE)i;
		ENTITY_OBJECTLIST* pList = m_pControlMediator_->GetToolLogicObjectMgr()->GetEntityObjectList<CEntityObject>(eCurrentType);
		if (pList == NULL)
			continue;

		for(ENTITY_OBJECTLIST::iterator itr = pList->begin(); itr != pList->end(); ++itr)
		{
			if(FindObjectList(vecObjectList, (*itr)->GetObjectName()))
			{
				int nIndex = m_ctlList_.AddString( (*itr)->GetObjectName() );
				m_ctlList_.SetItemDataPtr(nIndex, (void*)(*itr) );
			}
		}
	}

	m_ctlList_.UnlockWindowUpdate();

	m_pControlMediator_->GetHelperAction()->RemoveAllSelection(false);
}

void CRollupEntityObjectList::ParseFindString( string strParse, vector<string>& outObjectList )
{
	outObjectList.clear();

	while(1)
	{
		int fndpos = strParse.find(",");
		if (fndpos != string::npos)
		{
			string strSub = strParse.substr(0, fndpos);
			strSub = cml2::TrimBlanks(strSub);

			outObjectList.push_back(strSub);

			strParse = strParse.substr(fndpos+1);
		}
		else
		{
			outObjectList.push_back(cml2::TrimBlanks(strParse));

			break;
		}
	}
}

bool CRollupEntityObjectList::FindObjectList( vector<string>& vecFindList, string strObjectName )
{
	strObjectName = cml2::ToLower(strObjectName);

	for(vector<string>::iterator it = vecFindList.begin(); it != vecFindList.end(); ++it)
	{
		string strTemp = cml2::ToLower(*it);
		if(strTemp == strObjectName)
			return true;

		int nIndex = strObjectName.find(strTemp.c_str());
		if(nIndex != string::npos)
			return true;
	}

	return false;
}

void CRollupEntityObjectList::OnBnClickedButtonSearch()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	OnSearchObjectList();
}

void CRollupEntityObjectList::RegisterResizer()
{
	BOOL bOk = FALSE;

	bOk = m_resizer.Hook(this);
	ASSERT( bOk == TRUE);


	bOk = m_resizer.CreateSplitContainer(_T("InnerSplit"), IDC_ENTITY_EDIT_LIST, IDC_TREE_ENTITY_LIST_PREVIEW);
	ASSERT( bOk == TRUE);

	bOk = m_resizer.SetAnchor(_T("InnerSplit"), ANCHOR_ALL);
	ASSERT( bOk == TRUE);

	bOk = m_resizer.SetShowSplitterGrip(_T("InnerSplit"), TRUE );
	ASSERT( bOk == TRUE);

	m_resizer.SetShowResizeGrip(TRUE);

	bOk = m_resizer.InvokeOnResized();
}

void CRollupEntityObjectList::OnBnClickedButtonNpcReload()
{
	//�𷡽ð� ����
	HCURSOR hOld = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	//NPC.xml ���� ���ε�
	m_pControlMediator_->GetToolLogicObjectMgr()->ReLoadPropertyAndNpcTable_();

	CToolLogicObjectManager* pLogicObjectMgr = m_pControlMediator_->GetToolLogicObjectMgr();
	rs3::RToolSceneManager* pSceneManager = m_pControlMediator_->GetToolSceneManager();
	ENTITY_OBJECTLIST* pSpawnList = pLogicObjectMgr->GetEntityObjectList<CEntityObject>( CEntityObject::ENTITY_SPAWN );

	// �������ϴ� ����� Ŭ����
	m_pControlMediator_->GetWorkSpacePtr<CWorkSpaceField>()->ClearRenderSceneNodes();

	for(ENTITY_OBJECTLIST::iterator itr = pSpawnList->begin(); itr != pSpawnList->end(); ++itr)
	{
		CEntitySpawn* pEntitySpawn = (CEntitySpawn*)(*itr);
		std::map<int, rs3::RSceneNode*>* mapNpcIdNode = pEntitySpawn->GetMapNpcIdNode();
		std::vector<int> vecNPCID;

		for(std::map<int, rs3::RSceneNode*>::iterator itrMap = mapNpcIdNode->begin(); itrMap != mapNpcIdNode->end(); ++itrMap)
		{
			vecNPCID.push_back((*itrMap).first);
		}

		for(std::vector<int>::iterator itrNPCID = vecNPCID.begin(); itrNPCID != vecNPCID.end(); ++itrNPCID )
		{
			pEntitySpawn->RemoveNpc(*itrNPCID);
			pEntitySpawn->AddNpc( pSceneManager, pLogicObjectMgr, *itrNPCID);
		}

		pEntitySpawn->UpdateColor();
	}

	UpdateSpawnVisible(true);

	//�𷡽ð� ��
	::SetCursor(hOld);

	AfxMessageBox("NPC Reload Complete!");
}
