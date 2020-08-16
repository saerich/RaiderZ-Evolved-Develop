// V_EditObjectList.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "VL_SceneNodeList.h"

//#include "CoreToolRS3.h"
//#include "CoreToolRS3_View.h"
#include "C_ControlCursor.h"
#include "C_XMLEditInfoFile.h"

#include "C_ControlMediator.h"
#include "VL_ToolBar.h"
#include "M_LogicObject.h"

#include "VD_WaterEditor.h"
#include "M_LogicObject_SceneNodes.h"

#include "RActorNode.h"

#include "M_Visitors.h"
#include "WS_WorkSpaceManager.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSceneNodeTreeCtrl, CTreeCtrlExM)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CSceneNodeTreeCtrl, CTreeCtrlExM)

void CSceneNodeTreeCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) {
	NMTVCUSTOMDRAW* pcd = (NMTVCUSTOMDRAW*)pNMHDR;
	HTREEITEM hItem;

	switch ( pcd->nmcd.dwDrawStage ) {
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;     
		break;
	case CDDS_ITEMPREPAINT :
		hItem = (HTREEITEM)pcd->nmcd.dwItemSpec;
		rs3::RSceneNode* pNode = (rs3::RSceneNode*)GetItemData(hItem);
		if (pNode && pNode->QueryAttribute(rs3::RSNA_DO_NOT_SAVE) ) {
			pcd->clrText = RGB(196,196,196);
		} else {
			HTREEITEM   hItem = (HTREEITEM)pcd->nmcd.dwItemSpec;

			if ( GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED )
			{
				pcd->clrText = GetSysColor(COLOR_HIGHLIGHTTEXT);    
				pcd->clrTextBk = GetSysColor(COLOR_HIGHLIGHT);
			}
		}
		*pResult = CDRF_DODEFAULT;// do not set *pResult = CDRF_SKIPDEFAULT
		break;
	}    
} 

CObjectPropertyGridCtrl::CObjectPropertyGridCtrl() :
	m_pSelectedSceneNode_( NULL ),
	m_bFocused( false )
{
}

CObjectPropertyGridCtrl::~CObjectPropertyGridCtrl()
{
}

void CObjectPropertyGridCtrl::SetSelectedSceneNode( rs3::RSceneNode* _pSelectedNode )
{
	m_bFocused = false;

	RemoveAll();

	m_pSelectedSceneNode_ = _pSelectedNode;
	if( m_pSelectedSceneNode_ != NULL )
	{
		CControlSceneProperty* pProperty = dynamic_cast<CControlSceneProperty*>((CCoreObject*)m_pSelectedSceneNode_->GetUserData());
		CLogicObject* pLogicObject = dynamic_cast<CLogicObject*>((CCoreObject*)m_pSelectedSceneNode_->GetUserData());
		if(pLogicObject)
			pProperty = pLogicObject->GetProperty();

		if( pProperty != NULL )
		{
			// sticky ������ ���� �Ұ�
			bool bSticky = m_pSelectedSceneNode_->QueryAttribute(rs3::RSNA_DO_NOT_SAVE);

			pProperty->Init( this, m_pSelectedSceneNode_, !bSticky );
		}
	}

	RedrawWindow();
}




void CObjectPropertyGridCtrl::OnPropertyChanged( CMFCPropertyGridProperty* pProp ) const
{
	if ( m_pSelectedSceneNode_ )
	{
		CMFCPropertyGridColorProperty* pPropColor = dynamic_cast<CMFCPropertyGridColorProperty*>( pProp );
		if( pPropColor )
			pPropColor->SetColor( pPropColor->GetColor() );

		CCoreToolPropertyGridCtrl::OnPropertyChanged( pProp );

		CControlSceneProperty* pProperty = dynamic_cast<CControlSceneProperty*>((CCoreObject*)m_pSelectedSceneNode_->GetUserData());
		CLogicObject* pLogicObject = dynamic_cast<CLogicObject*>((CCoreObject*)m_pSelectedSceneNode_->GetUserData());
		if(pLogicObject)
			pProperty = pLogicObject->GetProperty();

		if(!pProperty) return;

		pProperty->OnPropertyChanged( pProp, m_pSelectedSceneNode_ );
	}
}

void CObjectPropertyGridCtrl::OnChangeSelection( CMFCPropertyGridProperty* pNewSel, CMFCPropertyGridProperty* pOldSel )
{
	if( pNewSel )
		m_bFocused = true;
	else
		m_bFocused = false;
}

//check box observer
void CDlgSceneNodeList::CTreeCheckBoxObserver::ObserveCheckBox( HTREEITEM hClickedItem )
{
	_ASSERT( m_pEditObjectList );

	bool bWillCheck = (m_pEditObjectList->m_ctlObjectTree.GetCheck( hClickedItem ) == FALSE );

	// �θ� �ִٸ�,
	if( m_pEditObjectList->m_ctlObjectTree.GetParentItem(hClickedItem) )
	{
		m_pEditObjectList->m_ctlObjectTree.SetCheck( hClickedItem, bWillCheck? TRUE : FALSE );
		rs3::RSceneNode* pNode = (rs3::RSceneNode*)m_pEditObjectList->m_ctlObjectTree.GetItemData(hClickedItem);
		if( pNode )
		{
			Process( pNode, bWillCheck );
		}
	}
	else // �θ� ���� �ֻ������
		UpdateCheckBox(bWillCheck, hClickedItem );
}

void CDlgSceneNodeList::CTreeCheckBoxObserver::UpdateCheckBox(bool _bWillCheck, HTREEITEM hTreeItem )
{
	_ASSERT( m_pEditObjectList );

	m_pEditObjectList->m_ctlObjectTree.SetCheck( hTreeItem, _bWillCheck? TRUE : FALSE );
	rs3::RSceneNode* pNode = (rs3::RSceneNode*)m_pEditObjectList->m_ctlObjectTree.GetItemData(hTreeItem);
	if( pNode )
	{
		Process(pNode, _bWillCheck );
	}

	for( HTREEITEM hChild = m_pEditObjectList->m_ctlObjectTree.GetChildItem( hTreeItem ); hChild != NULL; )
	{
		UpdateCheckBox( _bWillCheck, hChild );
		hChild = m_pEditObjectList->m_ctlObjectTree.GetNextSiblingItem(hChild);
	}
}

void CDlgSceneNodeList::CTreeCheckBoxObserver::Process(rs3::RSceneNode* pSceneNode, bool _bWillCheck )
{
	_ASSERT( pSceneNode );

	pSceneNode->SetVisible( _bWillCheck );

	CVisibleSetter visibleSetter( _bWillCheck );
	pSceneNode->Traverse( &visibleSetter );

	// ����Ʈ�� ������ Play, Stop���ش�
	if( _bWillCheck )
	{
		CEffectStarter effectStarter;
		pSceneNode->Traverse( &effectStarter );
	}
	else
	{
		CEffectStopper effectStopper;
		pSceneNode->Traverse( &effectStopper );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

// CEditObjectList ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgSceneNodeList, CDialog)
CDlgSceneNodeList::CDlgSceneNodeList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSceneNodeList::IDD, pParent)
	, m_bHierachyView(FALSE)
	, m_pEditWater_(NULL)
	, m_bShowList_(false)
	, m_nSelectedObjectCount(0)
	, m_pPropCtrl(NULL)
{
}

CDlgSceneNodeList::~CDlgSceneNodeList()
{
}

BOOL CDlgSceneNodeList::OnInitDialog(){
	CDialog::OnInitDialog();

	RECT rect;
	/*rect.left = 9;
	rect.top = 430;
	rect.right = 319;
	rect.bottom = 630;*/

	rect.left	= 325;
	rect.top	= 27;
	rect.right	= 635;
	rect.bottom = 619;

	m_ctlObjProperty_.Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rect, this, (UINT)-1);
	m_pPropCtrl = m_UI.createPropGridCtrl(&m_ctlObjProperty_);

	// create treeview control
	m_ctlObjectTree.ClearSelection();

	#pragma warning (disable : 4312 )
	HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(m_hWnd, GWLP_HINSTANCE);	// ���� ���� ���ֳ��� ?
	#pragma warning (default : 4312 )

	// comctl ������ 6.0 �̻����� �ö󰡸� ( xp ) ���߼����� ����� ���۾��ϰ� 2���� ���õ˴ϴ�
	// by pok. 2010. 11. 15. Ʈ�����߼��� ���� �����߽���.
	//DWORD dwMajor, dwMinor;
	//AtlGetCommCtrlVersion(&dwMajor, &dwMinor);
	//_ASSERT( dwMajor == 5  && dwMinor == 82 );

	CImageList* pImageList = m_pControlMediator_->GetWorkSpaceMgr()->GetImageList();
	m_ctlObjectTree.SetImageList( pImageList, TVSIL_NORMAL);

	m_treeCheckBoxObserver.m_pEditObjectList = this;
	m_ctlObjectTree.RegisterCheckBoxObserver( &m_treeCheckBoxObserver );

	// create water editor dialog
	ASSERT(m_pEditWater_ == NULL);
	m_pEditWater_ = new CDlgWaterEditor;
	m_pEditWater_->Create(CDlgWaterEditor::IDD, this);

	return TRUE;
}

void CDlgSceneNodeList::OnDestroy()
{
	ASSERT(m_pEditWater_);
	m_pEditWater_->DestroyWindow();
	delete m_pEditWater_;

	if(m_pPropCtrl)
	{
		m_pPropCtrl->DestroyWindow();
		SAFE_DELETE(m_pPropCtrl);
	}

	m_ctlObjProperty_.DestroyWindow();

	// �������� ������ ( ���� ���� �������� �ų�尡 ������ ) ������ �ִ�
	m_ctlObjectTree.DeleteAllItems();
	m_ctlObjectTree.DestroyWindow();

	CDialog::OnDestroy();
}

bool CDlgSceneNodeList::IsListableNode( const rs3::RSceneNode *pSceneNode )
{
	CControlSceneProperty* pProperty = dynamic_cast<CControlSceneProperty*>((CCoreObject*)pSceneNode->GetUserData());
	CLogicObject* pLogicObject = dynamic_cast<CLogicObject*>((CCoreObject*)pSceneNode->GetUserData());
	bool bListableNode = pProperty || pLogicObject;
	return bListableNode;
}

void CDlgSceneNodeList::UpdateTreeView(HTREEITEM hTreeItem, const rs3::RSceneNode *pNode)
{
	using namespace rs3;

	const RSceneNodeList &rList = pNode->GetChildren();
	for ( RSceneNodeList::const_iterator it = rList.begin(); it != rList.end(); ++it )
	{
		RSceneNode *pNode = (*it);

		if( !IsListableNode(pNode) ) continue;

		const char *pNodeID = pNode->GetNodeIDString();
		const char* name = pNode->GetNodeName().c_str();

		// Tree�� SceneNode�� �ִ´�. �̸��� ���������, <NodeID> �� ���´�.
		HTREEITEM hCurrentItem = m_ctlObjectTree.InsertItem(name, hTreeItem);
		if ( pNode->GetNodeName().empty())
		{
			CString strNodeID;
			strNodeID.Format("<%s>", pNodeID);
			m_ctlObjectTree.SetItemText(hCurrentItem, strNodeID);
		}

		int nImage = GetImageIndex( pNode );
		m_ctlObjectTree.SetItemImage(hCurrentItem, nImage, nImage);

		// tree �� pSceneNode �����͸� �ִ´�.
		m_ctlObjectTree.SetItemData(hCurrentItem,(DWORD_PTR)pNode);
		
		// set view check
		m_ctlObjectTree.SetCheck( hCurrentItem, pNode->GetVisible()? 1 : 0 );

		// bool bSticky = pNode->QueryAttribute( RSNA_DO_NOT_SAVE );
		// TODO: ������ ȸ������ ǥ�����ְ� �ʹ�

		UpdateTreeView(hCurrentItem, pNode);
	}
}

void CDlgSceneNodeList::UpdateTreeViewByClass(const rs3::RSceneNode *pNode)
{
	using namespace rs3;

	const RSceneNodeList &rList = pNode->GetChildren();
	for ( RSceneNodeList::const_iterator it = rList.begin(); it != rList.end(); ++it )
	{
		const RSceneNode *pChild = (*it);

		if( !IsListableNode(pChild) ) continue;

		// by pok. Ʈ���信�� �Ⱥ��̰� �Һκ� ����
		if( ( pChild->GetNodeName() != "DummyEnvLight0" ) && stricmp(pChild->GetNodeIDString(), rs3::RSID_EFFECT) != 0 )
		{
			if (m_nodesByClass[ pChild->GetNodeIDString() ].find(pChild->GetNodeName()) != m_nodesByClass[ pChild->GetNodeIDString() ].end())
			{
				std::string strPureName( pChild->GetNodeName().substr( 0, pChild->GetNodeName().length() - 4));
				std::string strFileName( strPureName + ".xml" );

				char szBuffer[256];
				szBuffer[0] = '\0';
				int nNumber = CXMLEditInfoFile::GetReference().GetNumbering(&strFileName);
				sprintf(szBuffer,"%s_%03d", strPureName.c_str(), nNumber);

				rs3::RToolSceneManager* pToolSceneManager = m_pControlMediator_->GetToolSceneManager();
				_ASSERT(pToolSceneManager);

				rs3::RSceneNode* pSameNameSceneNode =  pToolSceneManager->GetSceneNodeByName(szBuffer);
				while ( pSameNameSceneNode != NULL )
				{
					nNumber = CXMLEditInfoFile::GetReference().GetNumbering(&strFileName);
					sprintf(szBuffer,"%s_%03d", strPureName.c_str(), nNumber);
					pSameNameSceneNode = pToolSceneManager->GetSceneNodeByName(szBuffer);
				}
				// ���� �����ϱ� ���� const ������
				const_cast< RSceneNode* >(pChild)->SetNodeName( szBuffer );
			}
			m_nodesByClass[ pChild->GetNodeIDString() ][ pChild->GetNodeName() ] = pChild;
		}

		UpdateTreeViewByClass(pChild);
	}
}

void CDlgSceneNodeList::UpdateTreeControl()
{
	// Ʈ�� ��Ʈ���� ������Ʈ �Ҷ� �����ִ� �׸� �����ϱ�
	std::set< const char* > setClassToExpend;
	for( SCENE_CLASS_HITEM::iterator itr = m_sceneClassHItem.begin(); itr != m_sceneClassHItem.end(); ++itr )
	{
		//�������
		if( itr->second.second )
			setClassToExpend.insert( itr->first );
	}

	// Ʈ����Ʈ�� ��ġ
	int nVScrollPos = m_ctlObjectTree.GetScrollPos( SB_VERT );
	int nHScrollPos = m_ctlObjectTree.GetScrollPos( SB_HORZ );

	m_nodesByClass.clear();
	m_sceneClassHItem.clear();

	// lock window for tree control update
	// by pok, �����Ӷ����� �ּ�ó���߽��ϴ�.
	//m_ctlObjectTree.LockWindowUpdate();

	// tree control reset
	m_ctlObjectTree.DeleteAllItems();

	if(!m_bHierachyView)
	{
		// ��带 ID ���� ������
		UpdateTreeViewByClass( &m_pControlMediator_->GetToolSceneManager()->GetRootSceneNode() );

		// ID ���� ���� ��带 Ʈ���� ��Ʈ�Ѱ� ������ �ش�.
		for(NODEMAPBYCLASS::iterator i = m_nodesByClass.begin(); i!=m_nodesByClass.end(); i++)
		{
			MAP_NAME_SCENENODE::iterator itr_scene = i->second.begin();
			if( itr_scene == i->second.end() )
				continue;

			const char* currentNodeIDString = i->first;

			if( !IsListableNode(itr_scene->second) ) continue;

			HTREEITEM hCurrentClassItem = m_ctlObjectTree.InsertItem(  currentNodeIDString , TVI_ROOT);

			int nImage = GetImageIndex( itr_scene->second );

			m_ctlObjectTree.SetItemImage(hCurrentClassItem, nImage, nImage);
			m_ctlObjectTree.SetCheck( hCurrentClassItem, TRUE );

			// class hitem ������
			bool bExpend(false); // �����׸��� ã���� �����ϸ鼭 ������
			std::set< const char* >::iterator itr = setClassToExpend.find( currentNodeIDString );
			if( itr != setClassToExpend.end() )
				bExpend = true;
			m_sceneClassHItem.insert( SCENE_CLASS_HITEM::value_type( currentNodeIDString, pair< HTREEITEM, bool>(hCurrentClassItem, bExpend)) );

			// ���� ��� �ֱ�
			for( ; itr_scene != i->second.end(); ++itr_scene )
			{
				const rs3::RSceneNode* pNode = itr_scene->second;
				CString strNodeName;
				if ( pNode->GetNodeName().empty())
					strNodeName.Format("<%s>", pNode->GetNodeIDString() );
				else
					strNodeName = pNode->GetNodeName().c_str();

				HTREEITEM hCurrentItem = m_ctlObjectTree.InsertItem(  strNodeName , hCurrentClassItem);
				m_ctlObjectTree.SetItemImage(hCurrentItem, nImage, nImage);

				// tree �� pSceneNode �����͸� �ִ´�.
				m_ctlObjectTree.SetItemData(hCurrentItem,(DWORD_PTR)pNode);

				// set view check
				m_ctlObjectTree.SetCheck( hCurrentItem, pNode->GetVisible() ? 1 : 0 );
			}
		}
	}
	else
	{
		UpdateTreeView(TVI_ROOT, &m_pControlMediator_->GetToolSceneManager()->GetRootSceneNode());
	}

	// unlock window for tree control update
	// by pok, �����Ӷ����� �ּ�ó�� �߽��ϴ�.
	//m_ctlObjectTree.UnlockWindowUpdate();

	// ������ ���ȴ� �׸� �ٽ� ����α�
	for( SCENE_CLASS_HITEM::iterator itr = m_sceneClassHItem.begin(); itr != m_sceneClassHItem.end(); ++itr )
	{
		//�������
		if( itr->second.second )
			m_ctlObjectTree.Expand( itr->second.first, TVE_EXPAND );
	}
	
	// ���� ��ġ ����
	m_ctlObjectTree.SetScrollPos( SB_VERT, nVScrollPos );
	m_ctlObjectTree.SetScrollPos( SB_HORZ, nHScrollPos );
}

void CDlgSceneNodeList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_OBJECT, m_ctlObjectTree);

	// object property
	DDX_Check(pDX, IDC_CHECK_HIERACHY_VIEW, m_bHierachyView);
	DDX_Text(pDX, IDC_EDIT_OBJECT_SELECT_COUNT, m_nSelectedObjectCount);
}

BEGIN_MESSAGE_MAP(CDlgSceneNodeList, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_OBJECT, &CDlgSceneNodeList::OnTvnSelchangedTreeObject)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_OBJECT, &CDlgSceneNodeList::OnNMDblclkTreeObject)
	ON_BN_CLICKED(IDC_CHECK_HIERACHY_VIEW, &CDlgSceneNodeList::OnBnClickedCheckHierachyView)
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE_OBJECT, &CDlgSceneNodeList::OnTvnItemexpandedTreeObject)
	ON_BN_CLICKED(IDC_BUTTON_EXPAND_TOGGLE, &CDlgSceneNodeList::OnBnClickedButtonExpandToggle)
	ON_EN_CHANGE(IDC_EDIT_OBJECT_SELECT_COUNT, &CDlgSceneNodeList::OnEnChangeEditObjectSelectCount)
	ON_BN_CLICKED(IDC_BUTTON_PLAY_EFFECT, &CDlgSceneNodeList::OnBnClickedButtonPlayEffect)
END_MESSAGE_MAP()


// CEditObjectList �޽��� ó�����Դϴ�.

void CDlgSceneNodeList::Listen(CoreMessage& _message)
{
	switch( _message.nID )
	{
		case VIEW_MESSAGE::SCENE_LOADED :
		case VIEW_MESSAGE::SCENE_IMPORTED:
		{
			m_pControlMediator_->GetHelperAction()->RemoveAllSelection(true);
			UpdateTreeControl();
		}break;
		case VIEW_MESSAGE::LOGIC_LOADED :
			m_bShowList_ = false;
			ShowWindow(SW_HIDE);
			break;
		case VIEW_MESSAGE::ACTIVE_VIEW :
			if(m_pControlMediator_->m_mapInfo.m_bSceneEditableMap)
			{
				FooButton* pGroupBtn = m_pControlMediator_->GetViewPtr<CCoreToolToolBar>()->m_btnGroup;

				if(pGroupBtn[WORKBENCH_STATE::OBJECT_MOVE].isChecked())
					m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_MOVE);
				else if(pGroupBtn[WORKBENCH_STATE::OBJECT_NEW].isChecked())
					m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_NEW);
				else if(pGroupBtn[WORKBENCH_STATE::OBJECT_ROTATE].isChecked())
					m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_ROTATE);
				else if(pGroupBtn[WORKBENCH_STATE::OBJECT_SCALE].isChecked())
					m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_SCALE);
				else if(pGroupBtn[WORKBENCH_STATE::OBJECT_SELECT].isChecked())
					m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_SELECT);
			}
			break;
		case VIEW_MESSAGE::UNDO_OBJ_NEW_DEL:
		case VIEW_MESSAGE::SCENE_BREAK_INSTANCE:
		if(m_pControlMediator_->m_mapInfo.m_bLoaded && m_pControlMediator_->m_mapInfo.m_bSceneEditableMap)
		{
			m_pControlMediator_->GetHelperAction()->RemoveAllSelection(true);
			UpdateTreeControl();
		}
		break;

		case VIEW_MESSAGE::UNDO_MOVE:
		if(m_pControlMediator_->m_mapInfo.m_bLoaded && m_pControlMediator_->m_mapInfo.m_bSceneEditableMap)
		{
			m_pControlMediator_->GetHelperAction()->SetSelectedObjectCenterAndActionFrame();
			ObserveSelection(NULL, true, false);
		}
		break;

		case VIEW_MESSAGE::WB_MOUSE_L_UP:
		// �ϴ��� ���϶���
		if (m_pControlMediator_->m_mapInfo.m_bSceneEditableMap)
			ObserveSelection(NULL, true, false);
		break;

		case VIEW_MESSAGE::LOGIC_SELECT:
			{
				CLogicObject_SceneNode* pLogicObjectSceneNode = dynamic_cast<CLogicObject_SceneNode*>( _message.pLogicObject );
				if( pLogicObjectSceneNode )
				{
					SetSelectNode_( TVI_ROOT, pLogicObjectSceneNode->GetModelSceneNode(), true );	// select
					UpdatePropertyUI();
					UpdateSelectCount_();
				}
			}break;
		case VIEW_MESSAGE::LOGIC_UNSELECT:
			{
				CLogicObject_SceneNode* pLogicObjectSceneNode = dynamic_cast<CLogicObject_SceneNode*>( _message.pLogicObject );
				if( pLogicObjectSceneNode )
				{
					SetSelectNode_( TVI_ROOT, pLogicObjectSceneNode->GetModelSceneNode(), false );	// unselect
					UpdatePropertyUI();
					UpdateSelectCount_();
				}
			}break;
// 		case VIEW_MESSAGE::LOGIC_SELECTION_CLEAR:
// 			m_setSelectedNodes.clear();
// 			ObserveSelection(NULL, true, true);
// 			break;
	}
}

void CDlgSceneNodeList::OnCancel()
{
	m_bShowList_ = false;
	ShowWindow(SW_HIDE);
}
void CDlgSceneNodeList::OnOK()
{
	m_bShowList_ = false;
	ShowWindow(SW_HIDE);
}

void CDlgSceneNodeList::ShowToggle()
{
	m_bShowList_ = !m_bShowList_;
	int nShowState = m_bShowList_ ? SW_SHOW : SW_HIDE;
	ShowWindow(nShowState);
}

HTREEITEM CDlgSceneNodeList::SetSelectNode_(HTREEITEM hTreeItem, rs3::RSceneNode *pSceneNode, bool bSelect )
{
	using namespace rs3;

	for ( HTREEITEM hChildItem = m_ctlObjectTree.GetChildItem(hTreeItem) ;
			hChildItem != NULL; hChildItem = m_ctlObjectTree.GetNextSiblingItem(hChildItem))
	{
		RSceneNode *pTreeSceneNode = (RSceneNode*)m_ctlObjectTree.GetItemData(hChildItem);
		if (pTreeSceneNode == pSceneNode )
		{
// 			if( bSelect )
// 				m_setSelectedNodes.insert( pSceneNode );
// 			else
// 				m_setSelectedNodes.erase( pSceneNode );

			// �������¿� �������� UI �� ���� ������ ���۵� ����̴�. �����ؾ� ����� �����Ѵ�
			// �� OnTvnSelchangedTreeObject -> GetHelperAction::AddSelectection -> VIEW_MESSAGE::LOGIC_SELECT -> ����
			bool bPreviousSelected = ( m_ctlObjectTree.GetItemState( hChildItem, TVIS_SELECTED ) & TVIS_SELECTED) != FALSE;
			if( bPreviousSelected != bSelect )
			{
				m_ctlObjectTree.SelectItemEx(hChildItem, bSelect ? TRUE : FALSE);

				// ���� unselect�� �ȵǼ� ����� �غ���.. �̰��� ����
//				CString strText = m_ctlObjectTree.GetItemText( hChildItem );
//				mlog( "  tree %s %d\n", (LPCSTR)strText, bSelect );
			}
			return hChildItem;
		}

		HTREEITEM hItem = SetSelectNode_(hChildItem, pSceneNode, bSelect);
		if ( hItem )
			return hItem;
	}
	return NULL;
}

void CDlgSceneNodeList::SetListLock(bool _bLock)
{
	m_ctlObjectTree.EnableWindow(_bLock ? TRUE : FALSE);
}

void CDlgSceneNodeList::ViewStateToggle(const char* _pSzObjectClassName)
{
	SCENE_CLASS_HITEM::iterator itr = m_sceneClassHItem.find( _pSzObjectClassName );
	if( itr != m_sceneClassHItem.end() )
		m_treeCheckBoxObserver.ObserveCheckBox( itr->second.first );
}

bool CDlgSceneNodeList::GetViewState(const char* _pSzObjectClassName)
{
	BOOL bCheck(FALSE);
	SCENE_CLASS_HITEM::iterator itr = m_sceneClassHItem.find( _pSzObjectClassName );
	if( itr != m_sceneClassHItem.end() )
		bCheck = m_ctlObjectTree.GetCheck( itr->second.first );

	return (bCheck == TRUE );
}

void CDlgSceneNodeList::RecheckAllSelection_(SCENEOBJECTMAP *pSelectionList)
{
	m_ctlObjectTree.ClearSelection();
	m_ctlObjectTree.SelectItem(NULL);

	for (SCENEOBJECTMAP::iterator itr = pSelectionList->begin(); itr != pSelectionList->end(); ++itr)
	{
		HTREEITEM hItem = SetSelectNode_(TVI_ROOT, itr->first);
		itr->second = (void*)hItem;
	}
}

void CDlgSceneNodeList::UpdateSelectCount_()
{
	LOGIC_OBJECTLIST *pLogicList = m_pControlMediator_->GetHelperAction()->GetSelectedLogicObject();

	m_nSelectedObjectCount = 0;
	if ( pLogicList )
		m_nSelectedObjectCount = static_cast<int>(pLogicList->size());

//	m_nSelectedObjectCount = m_setSelectedNodes.size();

	UpdateData(FALSE);
}

void CDlgSceneNodeList::RemoveSelection(HTREEITEM hItem)
{
	m_ctlObjectTree.SetItemState(hItem, 0, TVIS_SELECTED);

	m_ctlObjProperty_.SetSelectedSceneNode(NULL);
	UpdateSelectCount_();
}

void CDlgSceneNodeList::UpdatePropertyUI()
{
	LOGIC_OBJECTLIST *pLogicList = m_pControlMediator_->GetHelperAction()->GetSelectedLogicObject();

	if(pLogicList->empty())
	{
		m_pPropCtrl->ShowWindow(SW_HIDE);
		m_ctlObjProperty_.SetSelectedSceneNode(NULL);
		return;
	}


	// property control
	std::list<CBaseObject*> pReflectList;
	int nSelectedCnt = 0;
	for(LOGIC_OBJECTLIST::iterator itr = pLogicList->begin(); itr!=pLogicList->end(); ++itr)
	{
		//---------------------------------------------
		//Get SceneNode
		rs3::RSceneNode* pSceneNode = NULL;
		CLogicObject* pLogicObject = *itr;
		CLogicObject_SceneNode* pLogicObjectSceneNode = dynamic_cast<CLogicObject_SceneNode*>( pLogicObject );
		if(pLogicObjectSceneNode)
			pSceneNode = pLogicObjectSceneNode->GetModelSceneNode();
		//---------------------------------------------

		if( pLogicObject->GetEditable() == false ) continue;

		if( pSceneNode != NULL )
		{
//			CControlSceneProperty* pProperty = pLogicObjectSceneNode->GetProperty();
			CBaseObject* pReflectProperty =pLogicObjectSceneNode->GetReflectProperty();;


			if(pReflectProperty)
				pReflectList.push_back(pReflectProperty);
			else
			{
				++nSelectedCnt;
				if (nSelectedCnt == 1)
				{
					m_pPropCtrl->ShowWindow(SW_HIDE);
					m_ctlObjProperty_.SetSelectedSceneNode(pSceneNode);
				}
				else if (nSelectedCnt == 2)
				{
					// ���߼��� ����
					m_ctlObjProperty_.SetSelectedSceneNode(NULL);
				}
			}
		}
	}


	if(!pReflectList.empty())
	{
		m_pPropCtrl->RemoveAll();			

		m_UI.BuildUIForObjects(m_pPropCtrl, pReflectList);

		m_pPropCtrl->ShowWindow(SW_SHOW);
		m_pPropCtrl->ExpandAll(true);
	}
	
	// m_ctlObjProperty_.SetSelectedSceneNode �� ���ָ� ��Ŀ���� �Ҵ´�.
	::SetFocus(m_pControlMediator_->GetWBWindowHandle());
}


void CDlgSceneNodeList::ObserveSelection(rs3::RSceneNode *pNewSelectionSceneNode, bool _bUpdateUI, bool _bSelectionClear, bool _bReCheckAllSelected)
{
	SCENEOBJECTMAP *pSceneList = m_pControlMediator_->GetHelperAction()->GetSelectedSceneObjectPtr();

	if( _bUpdateUI )
	{
		// tree control
		if( pNewSelectionSceneNode )
		{
			// ����
			if (_bReCheckAllSelected)
			{
				RecheckAllSelection_(pSceneList);
			}
			else
			{
				HTREEITEM hItem = SetSelectNode_(TVI_ROOT, pNewSelectionSceneNode);
				(*pSceneList)[pNewSelectionSceneNode] = hItem;
			}

			//adjust HScroll
			//m_ctlObjectTree.SetScrollPos( SB_HORZ, 58 );
		}

		UpdatePropertyUI();
	}

	if( pNewSelectionSceneNode == NULL && _bSelectionClear)
	{
		m_ctlObjectTree.ClearSelection();
		m_ctlObjectTree.SelectItem(NULL);
	}

	UpdateSelectCount_();
}

void CDlgSceneNodeList::OnTvnSelchangedTreeObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	if (pNMTreeView->action != 0)
		return;

	if (m_ctlObjectTree.GetClickedItem() == NULL)
		return;
	
	// UI ������� ���� �ʱ�ȭ
	m_pControlMediator_->GetHelperAction()->RemoveAllSelection(false);
// 	m_setSelectedNodes.clear();

	HTREEITEM hSelectedItem = m_ctlObjectTree.GetFirstSelectedItem();
	if (hSelectedItem == NULL)
		return;

	// UI ������ Ŭ���� ���� �������Ƿ� UI������� ����
// 	SCENEOBJECTMAP *pSceneList = m_pControlMediator_->GetHelperAction()->GetSelectedSceneObjectPtr();

	for ( ; hSelectedItem != NULL; hSelectedItem = m_ctlObjectTree.GetNextSelectedItem(hSelectedItem))
	{
		rs3::RSceneNode *pSceneNode = (rs3::RSceneNode*)m_ctlObjectTree.GetItemData(hSelectedItem);
// 		m_setSelectedNodes.insert( pSceneNode );
		if ( pSceneNode )
		{
			// TODO: �ų�� ������� �Ⱦ�� ���� ������
			CLogicObject_SceneNode* pLogicObjectSceneNode = dynamic_cast<CLogicObject_SceneNode*>( (CCoreObject*)pSceneNode->GetUserData() );
			if(pLogicObjectSceneNode)
			{
				// TODO. by pok. 2010. 11. 19.
				// ������ UI�� ������Ʈ �Ǵ� ����.
				// ���⸦ ���ؼ� VIEW_MESSAGE::LOGIC_SELECT �޽����� �߼۵ǰ� �׿� ���� �ߺ� UI ������Ʈ�� �̷������.
				m_pControlMediator_->GetHelperAction()->AddSelectection( pLogicObjectSceneNode, false);
			}
//			else
//				m_pControlMediator_->GetHelperAction()->AddSelectection(pSceneNode, NULL, false);
// 			(*pSceneList)[pSceneNode] = hSelectedItem;
		}
	}

	// selection�� �� �����̹Ƿ�, ���̻� ���ο� �������� ���� UI ������Ʈ�� ���� Property UI ������Ʈ
	ObserveSelection( NULL, true, false );
}

void CDlgSceneNodeList::OnNMDblclkTreeObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	rs3::RToolCamera* pCamear = m_pControlMediator_->GetToolCamera();
	if( pCamear == NULL)
		return;

	if( m_pControlMediator_->GetHelperAction()->GetSelectedLogicObject()->empty() )
		return;

	CLogicObject* pLogicObject = m_pControlMediator_->GetHelperAction()->GetSelectedLogicObject()->front();

	// �Ϲ����� ��ü ����Ʈ ����Ŭ���̸� �� ��ü�� ȭ���߽����� �ű��.
	bool bGoToObjectCenter(true);

	CLogicObject_SceneNode* pLogicObjectSceneNode = dynamic_cast<CLogicObject_SceneNode*> ( pLogicObject );
	if( pLogicObjectSceneNode )
	{
		rs3::RSceneNode* pSceneNode = pLogicObjectSceneNode->GetModelSceneNode();
		if( _stricmp( pSceneNode->GetNodeIDString(), rs3::RSID_WATERPLANE ) == 0 ) // �� ���
		{
			_ASSERT( m_pEditWater_ );
			m_pEditWater_->ResetWaterPlane( (rs3::RWaterPlane*)(pSceneNode) );
			m_pEditWater_->ShowWindow(SW_SHOW);
			bGoToObjectCenter = false;
		}
	}

	rs3::RVector vecPos;
	if( pLogicObject->GetPosition( vecPos ) )
	if( bGoToObjectCenter )
		pCamear->SetPositionToViewCenter( vecPos, Tool3Const::CONST_WB_STATIC_OBJ_TO_CENTER );

	*pResult = 0;
}

void CDlgSceneNodeList::OnBnClickedCheckHierachyView()
{
	UpdateData(TRUE);
	
	m_pControlMediator_->GetHelperAction()->RemoveAllSelection(true);
	UpdateTreeControl();
}

void CDlgSceneNodeList::OnTvnItemexpandedTreeObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	TVITEM  item;
	item.mask = TVIF_HANDLE;
	item.hItem = pNMTreeView->itemNew.hItem;
	m_ctlObjectTree.GetItem(&item);            // ������ ������ �˾Ƴ���.

	for( SCENE_CLASS_HITEM::iterator itr = m_sceneClassHItem.begin(); itr != m_sceneClassHItem.end(); ++itr )
	{
		if( itr->second.first == item.hItem )
		{
			if(item.state & TVIS_EXPANDED)
				itr->second.second = true;
			else
				itr->second.second = false;
		}
	}

	*pResult = 0;
}

void CDlgSceneNodeList::ExpandTreeControlToggle()
{
	// ��� Ʈ����Ʈ���� �����ִٸ� �����ְ� �ƴϸ� �ݾ��ش�
	bool bAllTreeItemClosed(true);
	for( SCENE_CLASS_HITEM::iterator itr = m_sceneClassHItem.begin(); itr != m_sceneClassHItem.end(); ++itr )
	{
		if( itr->second.second )
			bAllTreeItemClosed = false;
	}

	for( SCENE_CLASS_HITEM::iterator itr = m_sceneClassHItem.begin(); itr != m_sceneClassHItem.end(); ++itr )
	{
		itr->second.second = bAllTreeItemClosed; // ��� ������ ����̶�� ������ �������� ���
		m_ctlObjectTree.Expand( itr->second.first, bAllTreeItemClosed ? TVE_EXPAND : TVE_COLLAPSE);
	}
}

bool CDlgSceneNodeList::HasActiveSceneEditor()
{
	if ( m_pEditWater_->IsActive() )
	{
		return true;
	}
	else
	{
		if ( m_ctlObjProperty_.IsFocused() && GetFocus() == &m_ctlObjProperty_ )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

void CDlgSceneNodeList::OnBnClickedButtonExpandToggle()
{
	ExpandTreeControlToggle();
}

void CDlgSceneNodeList::ShowWaterEditor()
{
	struct WaterSelector
	{
		static bool Selector( rs3::RSceneNode* pSceneNode)
		{
			if ( _stricmp(pSceneNode->GetNodeIDString(), rs3::RSID_WATERPLANE) == 0 )
					return true;
			else
				return false;
		}
	};
	std::vector< rs3::RSceneNode* > vecWater;
	m_pControlMediator_->GetToolSceneManager()->GetRootSceneNode().SerializeToVector(WaterSelector::Selector, vecWater);
	if (vecWater.empty() == false)
	{
		_ASSERT( m_pEditWater_ );
		m_pEditWater_->ResetWaterPlane( (rs3::RWaterPlane*)(vecWater[0]) );
		m_pEditWater_->ShowWindow(SW_SHOW);
	}
}

int CDlgSceneNodeList::GetImageIndex( const rs3::RSceneNode* pNode )
{
	CLogicObject* pLogicObject = m_pControlMediator_->GetToolSceneManager()->GetLogicObjectLinkedTo( pNode );
	int nImage = pLogicObject ? pLogicObject->GetIconIndex() : 0;

	return nImage;
}

void CDlgSceneNodeList::OnEnChangeEditObjectSelectCount()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// __super::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CDlgSceneNodeList::OnBnClickedButtonPlayEffect()
{
	LOGIC_OBJECTLIST *pLogicList = m_pControlMediator_->GetHelperAction()->GetSelectedLogicObject();
	for(LOGIC_OBJECTLIST::iterator itr = pLogicList->begin(); itr!=pLogicList->end(); ++itr)
	{
		//---------------------------------------------
		//Get SceneNode
		rs3::RSceneNode* pSceneNode = NULL;
		CLogicObject* pLogicObject = *itr;
		CLogicObject_SceneNode* pLogicObjectSceneNode = dynamic_cast<CLogicObject_SceneNode*>( pLogicObject );
		if(pLogicObjectSceneNode)
			pSceneNode = pLogicObjectSceneNode->GetModelSceneNode();
		if(pSceneNode)
		{
			CEffectStarter effectStarter;
			pSceneNode->Traverse( &effectStarter );
		}
	}
}
