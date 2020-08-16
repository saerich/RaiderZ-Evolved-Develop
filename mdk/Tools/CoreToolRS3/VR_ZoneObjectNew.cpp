// V_EditObjectNew.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "VR_ZoneObjectNew.h"

//#include "CoreToolRS3.h"
#include "C_ControlMediator.h"
#include "C_ControlCursor.h"
#include "C_ControlPreviewSceneManager.h"
#include "C_XMLConfig.h"

#include "WS_WorkSpaceManager.h"

#include "V_MFCHelper.h"
#include "VL_SceneNodeList.h"
#include "VD_DlgCommonFileOpen.h"

#include "MFileSystem.h"
#include "RSceneNodeID.h"
#include "RWaterSceneNode.h"
#include "RSpotLightSceneNode.h"

// CRollupZoneObjectNew ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CRollupZoneObjectNew, CDialog)
CRollupZoneObjectNew::CRollupZoneObjectNew(CWnd* pParent /*=NULL*/)
: CDialog(CRollupZoneObjectNew::IDD, pParent)
, m_strCurrentFile("")
, m_strCurrentType("")
, m_strFullPathName("")
, m_nCreateSceneType(AS_INSTANCE)
{
	m_ctrlTreeNewList_.m_pObjectNewWindow = this;
	m_ctrlTreeNewList_.m_margin.SetPoint(0, 200);
}

CRollupZoneObjectNew::~CRollupZoneObjectNew()
{
}

void CRollupZoneObjectNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_AS_INSTANCE, m_nCreateSceneType);
	DDX_Control(pDX, IDC_TAB_FILE, m_ctlFileTab);
	DDX_Control(pDX, IDC_TREE_NEW_LIST, m_ctrlTreeNewList_);
}

using namespace rs3;

void CRollupZoneObjectNew::Listen(CoreMessage& _message)
{
	if( _message.nID == VIEW_MESSAGE::DEVICE_CREATED )
	{
		char *tName[TOTAL_NODETYPE] =
		{
			("���ν��Ͻ�"),
			("����Ʈ"),
			("���ǵ�Ʈ��"),
			("����"),
		};

		// Tab ���� Init
		m_ctlFileTab.SetCurFocus(0);
		TCITEM tabItem;
		tabItem.mask = TCIF_TEXT;
		for ( int i = 0; i<TOTAL_NODETYPE; ++i )
		{
			tabItem.pszText = tName[i];
			m_ctlFileTab.InsertItem(i,&tabItem);
		}

		UpdateFileList_();
		UpdateFileListCtrl_(m_ctlFileTab.GetCurFocus());
	}
	else if ( _message.nID == VIEW_MESSAGE::SCENE_LOADED )
	{
		NoneSelect();

		UpdateFileList_();
		UpdateFileListCtrl_(m_ctlFileTab.GetCurFocus());

		// �� ���ϱ�Ģ�� ���� ��ó�� 
		CString zonePath(m_pControlMediator_->m_mapInfo.m_strSceneFile.c_str());
		zonePath.Replace(".zone.xml", "_");
		m_strPreConcWaterFileName_ = zonePath.GetString();
		m_nSizePreConcForWater_ = m_strPreConcWaterFileName_.size();
	}
	else if (_message.nID == VIEW_MESSAGE::ACTIVE_VIEW)
	{
		m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::OBJECT_NEW);
	}
}

void CRollupZoneObjectNew::UpdateFileList_()
{
	// ���� ���� List�� ������Ʈ �Ѵ�.
	const char *m_filefilter[E_TOTAL_FILE_NODE] =
	{
		/* scene	*/ ".scene.xml\0",
		/* effect	*/ ".effect.scene.xml\0",
		/* SpeedTree*/ ".spt\0"
	};

	const char* m_pathFilter[E_TOTAL_FILE_NODE] =
	{
		/* scene	*/ CXMLConfig::GetInstance()->pcMapObjectPath,
		/* effect	*/ CXMLConfig::GetInstance()->pcEffectpath,
		/* SpeedTree*/ CXMLConfig::GetInstance()->pcTreepath
	};

	for ( UINT ui = 0; ui < E_TOTAL_FILE_NODE; ++ui )
	{
		m_arrayFile[ui].clear();

		RPathSplitter pathSpliter;
		pathSpliter.SetPath( m_pathFilter[ui] );

		for( size_t j = 0; j < pathSpliter.GetPathCount(); ++j )
		{
			const string& strPath = pathSpliter.GetPath(j);
			string::value_type findPos = strPath.find(m_pathFilter[ui]);
			if( findPos == string::npos ) continue;

			// pcMapObjectPath �� ���� ��� ��� + �����̸� ����
			vector<string> strFiles;
			CMFCHelper::GetFileFromPathWithFilter(strFiles, strPath.c_str(), m_filefilter[ui]);

			for( unsigned int i=0;i< strFiles.size(); i++ )
			{
				string strRelativeFilename = strFiles[i].substr( strPath.length() );
				m_arrayFile[ui].push_back( strRelativeFilename );
			}
		}
	} // next ui
}

// Ʈ�� ��Ʈ�ѿ� �������� �߰��ϵ�, �߰� ��ΰ� ������ ��带 ����鼭 �ִ´� 
HTREEITEM CRollupZoneObjectNew::AddToTreeItem( std::map<std::string, HTREEITEM >& mapFile, string& strFullName)
{
	if( strFullName.empty() )	// ���ȣ�� ���� ����
		return TVI_ROOT;

	int nStrokPos = strFullName.find_last_of("/");

	string strFolderName;
	string strNodeName;

	// ������ "\" �� ã�Ƽ� ��ο� ��� �̸��� �и�
	if (nStrokPos != std::string::npos)
	{
		strFolderName = strFullName.substr(0,nStrokPos);
		strNodeName = strFullName.substr( nStrokPos+1 );
	}
	else
		strNodeName = strFullName;

	std::map<std::string, HTREEITEM >::iterator itr = mapFile.find(strFolderName);

	HTREEITEM hItemParent;
	if (itr == mapFile.end())
	{
		// �� ��θ� ���ȣ��� (����ų�) ����
		hItemParent = AddToTreeItem( mapFile, strFolderName );
		mapFile[strFolderName] = hItemParent;
	}
	else
		hItemParent = itr->second;

	HTREEITEM hItem = m_ctrlTreeNewList_.InsertItem(strNodeName.c_str(), hItemParent);

	return hItem;
}


void CRollupZoneObjectNew::UpdateFileListCtrl_(int nIndex)
{
	m_ctrlTreeNewList_.DeleteAllItems();
	m_ctrlTreeNewList_.ShowWindow(SW_HIDE);

	if ( nIndex == SCENE )
	{
		UpdateData(FALSE);
		GetDlgItem( IDC_RADIO_AS_INSTANCE )->EnableWindow(TRUE);
		GetDlgItem( IDC_RADIO_AS_COPY )->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem( IDC_RADIO_AS_INSTANCE )->EnableWindow(FALSE);
		GetDlgItem( IDC_RADIO_AS_COPY )->EnableWindow(FALSE);
	}

	if( nIndex >= 0 && nIndex < E_TOTAL_FILE_NODE )
	{
		std::map<std::string, HTREEITEM > mapFile;
		for ( int i = 0; i<(int)m_arrayFile[nIndex].size(); ++i )
		{
			std::string& strFile = m_arrayFile[nIndex][i];

			AddToTreeItem( mapFile, strFile );
		}

		m_ctrlTreeNewList_.SelectItem( m_ctrlTreeNewList_.GetFirstVisibleItem() );

		m_ctrlTreeNewList_.ShowWindow(SW_SHOW);
		GetDlgItem( IDC_BUTTON_ETC_WATER_PLANE_CREATE )->ShowWindow(SW_HIDE);
	}
	else if ( nIndex == ETC )
	{
		m_ctrlTreeNewList_.ShowWindow(SW_HIDE);

		GetDlgItem( IDC_BUTTON_ETC_WATER_PLANE_CREATE )->ShowWindow(SW_SHOW);
	}
}

BEGIN_MESSAGE_MAP(CRollupZoneObjectNew, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_FILE, OnTcnSelchangeTabFile)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_NEW_LIST, &CRollupZoneObjectNew::OnTvnSelchangedTreeNewList)
	ON_BN_CLICKED(IDC_BUTTON_ETC_WATER_PLANE_CREATE, &CRollupZoneObjectNew::OnBnClickedButtonEtcWaterPlaneCreate)
	ON_BN_CLICKED(IDC_RADIO_AS_INSTANCE, &CRollupZoneObjectNew::OnBnClickedRadioAsInstance)
	ON_BN_CLICKED(IDC_RADIO_AS_COPY, &CRollupZoneObjectNew::OnBnClickedRadioAsCopy)
END_MESSAGE_MAP()

// CRollupZoneObjectNew �޽��� ó�����Դϴ�.

void CRollupZoneObjectNew::OnTcnSelchangeTabFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateFileListCtrl_( m_ctlFileTab.GetCurFocus());
	*pResult = 0;
}

void CRollupZoneObjectNew::OnTvnSelchangedTreeNewList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	//m_ctlFileTab.Invalidate();
	//m_ctrlTreeNewList_.Invalidate();

	if (pNMTreeView->action == 0)
	{
		*pResult = 0;
		return;
	}

	HTREEITEM hItem = m_ctrlTreeNewList_.GetSelectedItem();
	if( hItem == NULL)
	{
		*pResult = 0;
		return;
	}

	if (NULL != m_ctrlTreeNewList_.GetChildItem(hItem))
	{
		//m_ctrlTreeNewList_.Expand(hItem, TVE_COLLAPSE);
		//m_ctrlTreeNewList_.Expand(hItem, TVE_EXPAND);
		//m_ctrlTreeNewList_.SelectItem(hItem);
		//m_ctrlTreeNewList_.SetItemState( hItem, 1, TVIS_SELECTED );
		//m_ctrlTreeNewList_.SetFocus();
		*pResult = 0;
		return;
	}

	int nIndex = m_ctlFileTab.GetCurFocus();
	CString strFileName = m_ctrlTreeNewList_.GetItemText(hItem);
	m_strCurrentFile = strFileName.GetString();

	switch ( nIndex )
	{
	case SCENE:
		m_strCurrentType = RSID_SCENEINSTANCE;
		m_strFullPathName = m_strCurrentFile;
		break;
	case EFFECT:
		m_strCurrentType = RSID_EFFECTINSTANCE;
		m_strFullPathName = m_strCurrentFile;
		break;
	case TREE:
		m_strCurrentType = RSID_TREE;
		m_strFullPathName = m_strCurrentFile;
		break;
	}

	// ��ü�������¸� New���·� �����Ų��
	if( m_pControlMediator_->m_mapInfo.m_bLoaded &&
		m_pControlMediator_->GetCurrentWBState() != WORKBENCH_STATE::OBJECT_NEW )
		m_pControlMediator_->GetWorkSpaceMgr()->TriggerWBState(WORKBENCH_STATE::OBJECT_NEW, true);

	NotifyToNewAction();

	// TODO: preview window ����� �����ҵ��� ����
	// preview ������ �ٽ� �ߴٸ� CControlPreviewSceneManager �� ���� �����Ұ�

	/*
	// ������Ʈ ������ ��ü�� ����� -
	RSceneNode *pNode = CControlPreviewSceneManager::GetInstance()->LoadSceneNode(m_strFullPathName.c_str(), m_strCurrentType.c_str());
	if ( pNode == NULL )
		return;

	CControlPreviewSceneManager::GetInstance()->UpdateRender(pNode);
	pNode->RemoveFromParent();
	SAFE_DELETE(pNode);
	
	// Preview �����쿡 ������ �Ѵ�.
	CWnd *pWnd = GetDlgItem(IDC_OBJECT_PREVIEW);
	CDC *pDC = pWnd->GetDC();
	if ( pDC == NULL )
		return;

	RDeviceD3D *pd3ddevice = m_pControlMediator_->GetCurrentDevice();
	ASSERT(pd3ddevice);

	HDC hDC = pDC->GetSafeHdc();
	if ( hDC == NULL )
		return;

	RECT rt;
	pWnd->GetClientRect(&rt);

	RTexture* hTexture = CControlPreviewSceneManager::GetInstance()->GetRenderTexture();
	LPDIRECT3DBASETEXTURE9 lpTexture = pd3ddevice->GetD3DTexture(hTexture);
	_ASSERT(lpTexture->GetType()==D3DRTYPE_TEXTURE);

	CMFCHelper::D3DTextureToHDC(hDC,(LPDIRECT3DTEXTURE9)lpTexture,&rt);
	pWnd->ReleaseDC(pDC);
	*/

	*pResult = 0;
}

void CRollupZoneObjectNew::NoneSelect()
{
	// Ʈ�� ��Ʈ�� ���� ��ȿȭ
	/*HTREEITEM hItem = m_ctrlTreeNewList_.GetSelectedItem();
	if( (hItem != NULL) &&
		( NULL == m_ctrlTreeNewList_.GetChildItem(hItem) &&
		(m_ctrlTreeNewList_.GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED) ) )
		m_ctrlTreeNewList_.SetItemState( hItem, 0, TVIS_SELECTED );*/
	//m_ctrlTreeNewList_.SetFocus();

	m_strCurrentFile = "";
	m_strCurrentType = "";

	//������ ��ȿȭ
	CWnd *pWnd = GetDlgItem(IDC_OBJECT_PREVIEW);
	CClientDC dc(pWnd);
	CRect rect;
	pWnd->GetClientRect(&rect);
	CBrush brush, *pOldBrush;

	brush.CreateSysColorBrush(COLOR_BTNFACE);
	pOldBrush = (CBrush*)dc.SelectObject(&brush);
	dc.Rectangle(&rect);
	dc.SelectObject(pOldBrush);
	//pWnd->InvalidateRect(NULL, TRUE);

	NotifyToNewAction();
}

void CRollupZoneObjectNew::OnCancel()
{
}
void CRollupZoneObjectNew::OnOK()
{
}

void CRollupZoneObjectNew::OnBnClickedButtonEtcWaterPlaneCreate()
{
	if( !m_pControlMediator_->m_mapInfo.m_bLoaded )
		return;

	rs3::RToolSceneManager* pToolSceneManager = m_pControlMediator_->GetToolSceneManager();
	if(pToolSceneManager == NULL)
		return;

	//list view file dialog ����
	const char* szMapPath = m_pControlMediator_->m_mapInfo.m_strScenePath.c_str();
	CListViewOpenDlg dlg(szMapPath);

	dlg.SetTitle("�� ���ϱ�");
	dlg.SetFilter("Water Object ����\0*water*.elu;");

	if(dlg.DoModal() == IDOK)
	{
		CString fileName(dlg.GetFileName());
		if(fileName.IsEmpty())
			return;

		std::string listFile(fileName.GetString());
		listFile.erase(0, m_nSizePreConcForWater_);

		CString compareFileName(fileName);
		CString comparePreConc(m_strPreConcWaterFileName_.c_str());
		compareFileName.MakeLower();
		comparePreConc.MakeLower();
		if(compareFileName.Find(comparePreConc) != 0)
		{
			AfxMessageBox("�ùٸ� ������ �̸��� �ƴմϴ�.");
			return;
		}

		// water object
		if(fileName.Find("_p_") < 0)
		{
		}
		// water plane
		else
		{
			rs3::RWaterPlane* pWaterPlane = static_cast<rs3::RWaterPlane*>(pToolSceneManager->CreateSceneNode( rs3::RSID_WATERPLANE ) );
			pWaterPlane->SetNodeName( fileName.GetString() );
			pWaterPlane->Build( fileName.GetString() );
			pToolSceneManager->AddSceneNode(pWaterPlane, true);
			pToolSceneManager->AttachLogicObject( pWaterPlane );
		}

		// update ui
		m_pControlMediator_->GetViewPtr< CDlgSceneNodeList >()->UpdateTreeControl();
	}
}

void CRollupZoneObjectNew::SelectNewObjectListTab( int nIndex )
{
	m_ctlFileTab.SetCurSel( nIndex );
	UpdateFileListCtrl_( nIndex );
}

void CRollupZoneObjectNew::OnBnClickedRadioAsInstance()
{
	m_nCreateSceneType = AS_INSTANCE;
}

void CRollupZoneObjectNew::OnBnClickedRadioAsCopy()
{
	m_nCreateSceneType = AS_COPY;
}

LRESULT CRollupZoneObjectNew::CTreeCtrlNewObject::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = CTreeCtrl::WindowProc(message, wParam, lParam);

	switch(message) {
		case WM_LBUTTONDOWN:
			{
				CRect rect;
				m_pObjectNewWindow->GetClientRect(&rect);
				rect.TopLeft() += m_margin;
				m_pObjectNewWindow->InvalidateRect(&rect);
				break;
			}
	}
	return result;
}

void CRollupZoneObjectNew::NotifyToNewAction()	///< ������ ����Ǿ����� �˸���
{
	CDoActionObjectNew* pObjectNewAction = m_pControlMediator_->GetWorkSpaceMgr()->GetDoActionObjectNew();
	if (pObjectNewAction)
		pObjectNewAction->OnReferenceChanged( m_pControlMediator_, this );
}
