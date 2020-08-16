// V_MapNewDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "VL_MapNew.h"

#include "CoreToolRS3_Doc.h"

#include "VL_ResourceInfo.h"

#include "C_ControlMediator.h"
#include "C_ControlMediatorHelperResource.h"

#include "M_ToolCmdMgr.h"
#include "C_XMLConfig.h"
#include "C_XMLUserConfig.h"
#include "C_XMLResourceList.h"
#include "VD_DlgCommonFileOpen.h"

#include "V_MFCHelper.h"
#include "VD_DlgInputMapName.h"

#include "WS_WorkSpaceManager.h"
#include "WS_Field.h"
#include "VD_DlgCutSceneNewMap.h"

#include "M_EntityObject.h"

// CDlgMapNew ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgMapNew, CDialog)
CDlgMapNew::CDlgMapNew(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMapNew::IDD, pParent)
{
	m_bActive_ = false;
	m_nZoneID_ = -1;
	m_nFromFieldID_ = -1;
}

CDlgMapNew::~CDlgMapNew()
{
}

BOOL CDlgMapNew::OnInitDialog(){
	CDialog::OnInitDialog();

	// ����Ÿ ����
	char szfull[256]= "";
	_fullpath( szfull, CXMLConfig::GetInstance()->pcDataPath, 256 );
	m_strDataPath_ = szfull;

	//����Ʈ ä���
	OnCbnDropdownComboZoneId();
	OnCbnDropdownComboFieldId();

	return TRUE;
}

void CDlgMapNew::EnableTemplateCtrl_(UINT _bool)
{
	CWnd::GetDlgItem(IDC_RADIO_600)->EnableWindow(_bool);
	CWnd::GetDlgItem(IDC_RADIO_800)->EnableWindow(_bool);
	CWnd::GetDlgItem(IDC_RADIO_CUSTOM)->EnableWindow(_bool);

	CWnd::GetDlgItem(IDC_RADIO_256)->EnableWindow(_bool);
	CWnd::GetDlgItem(IDC_RADIO_512)->EnableWindow(_bool);
	CWnd::GetDlgItem(IDC_RADIO_CUSTOM2)->EnableWindow(_bool);

	CWnd::GetDlgItem(IDC_EDIT_BASE_TILE)->EnableWindow(_bool);
	CWnd::GetDlgItem(IDC_BUTTON_BASE_TILE_FIND)->EnableWindow(_bool);

	//�������� Enable���θ� �����ϹǷ� ������ false��
	CWnd::GetDlgItem(IDC_EDIT_NEW_X)->EnableWindow(FALSE);
	CWnd::GetDlgItem(IDC_EDIT_NEW_Y)->EnableWindow(FALSE);
	CWnd::GetDlgItem(IDC_EDIT_NEW_XQ)->EnableWindow(FALSE);
	CWnd::GetDlgItem(IDC_EDIT_NEW_YQ)->EnableWindow(FALSE);
}
void CDlgMapNew::EnableImportSceneCtrl_(UINT _bool)
{
	CWnd::GetDlgItem(IDC_EDIT_IMPORT_SCENE)->EnableWindow(_bool);
	CWnd::GetDlgItem(IDC_BUTTON_IMPORT_SCENE_FIND)->EnableWindow(_bool);
}

void CDlgMapNew::EnableZone_(UINT _bool)
{
	EnableTemplateCtrl_(_bool);
	EnableImportSceneCtrl_(_bool);
	CWnd::GetDlgItem(IDC_RADIO_TEMPLATE)->EnableWindow(_bool);
	CWnd::GetDlgItem(IDC_RADIO_IMPORT_SCENE)->EnableWindow(_bool);
	CWnd::GetDlgItem(IDC_COMBO_ZONE_ID)->EnableWindow(_bool);
	//CWnd::GetDlgItem(IDC_BUTTON_ZONE_MANAGER)->EnableWindow(_bool);
}
void CDlgMapNew::OnOK(){
	//int a = AfxMessageBox("�޼����ڽ�", MB_YESNOCANCEL);
	ShowWindow(SW_HIDE);
	m_bActive_ = false;
}

void CDlgMapNew::OnCancel(){
	ShowWindow(SW_HIDE);
	m_bActive_ = false;
}

void CDlgMapNew::Listen(CoreMessage& _message)
{
	if(_message.nID == VIEW_MESSAGE::ACTIVE_VIEW)
	{
		m_bActive_ = true;
		if(m_enumNewType_ == ZONE)
		{
			NewZoneType_(TRUE);
			ShowWindow(SW_SHOW);

		}
		else if (m_enumNewType_ == FIELD)
		{
			NewZoneType_(FALSE);
			ShowWindow(SW_SHOW);
		}
		else
		{
			CDlgCutSceneNewMap dlg(m_pControlMediator_);
			dlg.m_strFileName =  (CXMLConfig::GetInstance()->strDataFullPath + "CutScene\\TestCutScene.cutscene.xml").c_str();
			dlg.m_strName = "Describe This CutScene";
			dlg.m_nFieldId = 0;
			dlg.m_fDuration = 1.f;

			if (dlg.DoModal() == IDOK && dlg.MakeNewCutSceneFile())
			{
				m_pControlMediator_->GetWorkSpaceMgr()->OpenMapFileWithDoc(dlg.m_strFileName.GetString());
			}

			m_bActive_ = false;
		}
	}
}

void CDlgMapNew::NewZoneType_(BOOL _bool)
{
	//ĿƮ�� ��ġ �ʱ�ȭ
	m_nZoneType_ = 0;
	m_nSizeType_ = 1; //����� ����, 800*800
	m_nDetailType_ = 1; //512 X 512

	//field ����Ʈ ä���
	OnCbnDropdownComboZoneId();
	OnCbnDropdownComboFieldId();
	m_nFieldType_ = 1; //���� �������� ������ �ʱ�
	UpdateListBox_(m_nFieldType_ - 1); //�������� �ϳ� �������� �ǹ�ȣ
	UpdateData(FALSE);

	int width=::GetSystemMetrics(SM_CXSCREEN);
	int height=::GetSystemMetrics(SM_CYSCREEN);

	CWnd* pZoneOk = GetDlgItem(IDOK_ZONE);
	CWnd* pZoneCancel = GetDlgItem(IDCANCEL_ZONE);

	int show_flag = _bool ? SW_SHOW : SW_HIDE;
	int reverse_bool = _bool ? FALSE : TRUE;

	pZoneOk->EnableWindow(_bool);
	pZoneOk->ShowWindow(show_flag);
	pZoneCancel->EnableWindow(_bool);
	pZoneCancel->ShowWindow(show_flag);
	GetDlgItem(IDOK)->EnableWindow(reverse_bool);

	if(_bool)
	{
		EnableZone_(TRUE);
		EnableImportSceneCtrl_(FALSE);
		SetWindowText("�� �� �����");
		SetWindowPos(NULL, width/2 - 185, height/2 - 210, 370, 420, SWP_NOZORDER);

		// �� ����
		GetDlgItem(IDC_STATIC_ZONE_GROUP)->SetWindowText("�� ����");
		m_ctlListBox_.ShowWindow(SW_HIDE);
	}
	else
	{
		EnableZone_(FALSE);
		SetWindowText("�� �ʵ� �����");
		SetWindowPos(NULL, width/2 - 320, height/2 -210, 640, 420, SWP_NOZORDER);

		// �⺻�� ���� �����κ��� �����̴�.
		GetDlgItem(IDC_STATIC_ZONE_GROUP)->SetWindowText("�ʵ带 ������ ��");
		m_ctlListBox_.ShowWindow(SW_SHOW);
	}
}

void CDlgMapNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_TEMPLATE, m_nZoneType_);
	DDX_Radio(pDX, IDC_RADIO_600, m_nSizeType_);
	DDX_Radio(pDX, IDC_RADIO_256, m_nDetailType_);
	DDX_Radio(pDX, IDC_NEW_ZONE, m_nFieldType_);

	DDX_Text(pDX, IDC_EDIT_NEW_X, m_strNewX_);
	DDX_Text(pDX, IDC_EDIT_NEW_Y, m_strNewY_);
	DDX_Text(pDX, IDC_EDIT_NEW_XQ, m_strNewXQ_);
	DDX_Text(pDX, IDC_EDIT_NEW_YQ, m_strNewYQ_);
	DDX_Text(pDX, IDC_EDIT_BASE_TILE, m_strNewBaseTile_);
	DDX_Text(pDX, IDC_EDIT_IMPORT_SCENE, m_strImportSceneFile_);

	DDX_Control(pDX, IDC_LIST_MAP_CANDI, m_ctlListBox_);
	DDX_Control(pDX, IDC_COMBO_ZONE_ID, m_zoneComboBox_);
	DDX_Control(pDX, IDC_COMBO_FIELD_ID, m_fieldComboBox_);
}

BEGIN_MESSAGE_MAP(CDlgMapNew, CDialog)
	ON_BN_CLICKED(IDC_RADIO_TEMPLATE, &CDlgMapNew::OnBnClickedRadioTemplate)
	ON_BN_CLICKED(IDC_RADIO_IMPORT_SCENE, &CDlgMapNew::OnBnClickedRadioImportScene)
	ON_BN_CLICKED(IDC_RADIO_600, &CDlgMapNew::OnBnClickedRadio600)
	ON_BN_CLICKED(IDC_RADIO_800, &CDlgMapNew::OnBnClickedRadio800)
	ON_BN_CLICKED(IDC_RADIO_CUSTOM, &CDlgMapNew::OnBnClickedRadioCustom)
	ON_BN_CLICKED(IDC_RADIO_256, &CDlgMapNew::OnBnClickedRadio256)
	ON_BN_CLICKED(IDC_RADIO_512, &CDlgMapNew::OnBnClickedRadio512)
	ON_BN_CLICKED(IDC_RADIO_CUSTOM2, &CDlgMapNew::OnBnClickedRadioCustom2)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_SCENE_FIND, &CDlgMapNew::OnBnClickedButtonImportSceneFind)
	ON_BN_CLICKED(IDC_BUTTON_BASE_TILE_FIND, &CDlgMapNew::OnBnClickedButtonBaseTileFind)
	ON_BN_CLICKED(IDOK_ZONE, &CDlgMapNew::OnBnClickedZoneOk)
	ON_BN_CLICKED(IDCANCEL_ZONE, &CDlgMapNew::OnBnClickedZoneCancel)
	ON_BN_CLICKED(IDOK, &CDlgMapNew::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgMapNew::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_NEW_ZONE, &CDlgMapNew::OnBnClickedNewZone)
	ON_BN_CLICKED(IDC_FROM_ZONE, &CDlgMapNew::OnBnClickedFromZone)
	ON_BN_CLICKED(IDC_FROM_FIELD, &CDlgMapNew::OnBnClickedFromField)

	//ON_BN_CLICKED(IDC_BUTTON_ZONE_MANAGER, &CDlgMapNew::OnBnClickedButtonZoneManager)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_MANAGER, &CDlgMapNew::OnBnClickedButtonFieldListEditor)

	ON_CBN_DROPDOWN(IDC_COMBO_ZONE_ID, &CDlgMapNew::OnCbnDropdownComboZoneId)
	ON_CBN_DROPDOWN(IDC_COMBO_FIELD_ID, &CDlgMapNew::OnCbnDropdownComboFieldId)
END_MESSAGE_MAP()


// CDlgMapNew �޽��� ó�����Դϴ�.

void CDlgMapNew::OnBnClickedRadioTemplate()
{
	EnableTemplateCtrl_(TRUE);
	EnableImportSceneCtrl_(FALSE);
}

void CDlgMapNew::OnBnClickedRadioImportScene()
{
	EnableTemplateCtrl_(FALSE);
	EnableImportSceneCtrl_(TRUE);
}

void CDlgMapNew::OnBnClickedRadio600()
{
	CWnd::GetDlgItem(IDC_EDIT_NEW_X)->EnableWindow(FALSE);
	CWnd::GetDlgItem(IDC_EDIT_NEW_Y)->EnableWindow(FALSE);
	UpdateData(TRUE);
}

void CDlgMapNew::OnBnClickedRadio800()
{
	CWnd::GetDlgItem(IDC_EDIT_NEW_X)->EnableWindow(FALSE);
	CWnd::GetDlgItem(IDC_EDIT_NEW_Y)->EnableWindow(FALSE);
	UpdateData(TRUE);
}

void CDlgMapNew::OnBnClickedRadioCustom()
{
	CWnd::GetDlgItem(IDC_EDIT_NEW_X)->EnableWindow(TRUE);
	CWnd::GetDlgItem(IDC_EDIT_NEW_Y)->EnableWindow(TRUE);
	UpdateData(TRUE);
}

void CDlgMapNew::OnBnClickedRadio256()
{
	CWnd::GetDlgItem(IDC_EDIT_NEW_XQ)->EnableWindow(FALSE);
	CWnd::GetDlgItem(IDC_EDIT_NEW_YQ)->EnableWindow(FALSE);
	UpdateData(TRUE);
}

void CDlgMapNew::OnBnClickedRadio512()
{
	CWnd::GetDlgItem(IDC_EDIT_NEW_XQ)->EnableWindow(FALSE);
	CWnd::GetDlgItem(IDC_EDIT_NEW_YQ)->EnableWindow(FALSE);
	UpdateData(TRUE);
}

void CDlgMapNew::OnBnClickedRadioCustom2()
{
	CWnd::GetDlgItem(IDC_EDIT_NEW_XQ)->EnableWindow(TRUE);
	CWnd::GetDlgItem(IDC_EDIT_NEW_YQ)->EnableWindow(TRUE);
	UpdateData(TRUE);
}

void CDlgMapNew::OnBnClickedButtonImportSceneFind()
{
	//�ʺ� ������Ʈ
	UpdateData(TRUE);

	// ����Ʈ�� �� ���� ��ȭâ
	CFileDialog Dlg(TRUE);
	Dlg.GetOFN().lpstrTitle = _T(".scene.xml ������ �����ϼ���");
	Dlg.GetOFN().lpstrFilter = "scene.xml����(*.scene.xml)\0*.scene.xml";
	Dlg.GetOFN().Flags |= OFN_NOCHANGEDIR | OFN_READONLY;

	if ( Dlg.DoModal()==IDOK )
	{
		// ������ �����ߴٸ� UI ������Ʈ
		m_strImportSceneFile_ = Dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CDlgMapNew::OnBnClickedButtonBaseTileFind()
{
	//�ʺ� ������Ʈ
	UpdateData(TRUE);

	CDlgResourceInfo dlg(m_pControlMediator_, CDlgResourceInfo::RES_TILE);
	if (dlg.DoModal()==IDOK )
	{
		if(dlg.m_strSelectedTile.empty())
			m_strNewBaseTile_ = "";
		else
			m_strNewBaseTile_ = dlg.m_strSelectedTile.c_str();

		//������ �����ߴٸ� UI�� ������Ʈ
		if(m_strNewBaseTile_.GetLength() != 0)
			UpdateData(FALSE);
	}
}

bool CDlgMapNew::IsValidNewZone_()
{
	// ���ο� ���� ���ø����� ���� ������ ���
	if(m_nZoneType_ == 0)
	{
		if(m_nSizeType_ == 0)
		{
			m_mapHeight_ = 60000;
			m_mapWidth_  = 60000;
		}
		else if(m_nSizeType_ == 1)
		{
			m_mapHeight_ = 80000;
			m_mapWidth_  = 80000;
		}
		else
		{
			char s[20];
			memset(s, 0, sizeof(s));
			GetDlgItemText(IDC_EDIT_NEW_X, s, sizeof(s));
			m_mapHeight_ = (float)atof(s) * 100;

			memset(s, 0, sizeof(s));
			GetDlgItemText(IDC_EDIT_NEW_Y, s, sizeof(s));
			m_mapWidth_ = (float)atof(s) * 100;

			if(m_mapWidth_ != m_mapHeight_)
			{
				AfxMessageBox("���� ũ��� ���簢�� �̾�� �մϴ�.");
				return false;
			}
		}

		if(m_nDetailType_ == 0)
		{
			m_colFace_ = 256;
			m_rowFace_ = 256;
		}
		else if(m_nDetailType_ == 1)
		{
			m_colFace_ = 512;
			m_rowFace_ = 512;
		}
		else
		{
			char s[20];
			memset(s, 0, sizeof(s));
			GetDlgItemText(IDC_EDIT_NEW_XQ, s, sizeof(s));
			m_colFace_ = (int)atof(s);

			memset(s, 0, sizeof(s));
			GetDlgItemText(IDC_EDIT_NEW_YQ, s, sizeof(s));
			m_rowFace_ = (int)atof(s);

			if(m_colFace_ != m_rowFace_)
			{
				AfxMessageBox("���� �������� ���簢�� �̾�� �մϴ�.");
				return false;
			}

			double fWidth = (double)(m_colFace_);
			double fV1 = _logb(fWidth);
			double fV2 = pow(2.0,fV1);
			if ( fV2 != fWidth )
			{
				AfxMessageBox("���� �������� 2�� �ŵ��������̿��� �մϴ�.");
				return false;
			}
		}

		//base tile ����
		CString baseTile;
		GetDlgItemText(IDC_EDIT_BASE_TILE, baseTile);
		baseTile.MakeLower();
		CString validPath(CXMLResourceList::GetReference().GetTileFolderPath());
		validPath.MakeLower();
		int test = baseTile.Find(validPath);
		if(test < 0)
		{
			AfxMessageBox("���̽� Ÿ���� �߸� �Ǿ� �ֽ��ϴ�. \n���̽� Ÿ�� ��ġ�� ������ �ִ� Ÿ�� ��ġ�� �ٸ��� �ֽ��ϴ�.");
			return false;
		}
		m_strMapBaseTile_ = (LPTSTR)(LPCTSTR)baseTile;
	}
	else // ���ο� �����κ��� ����Ʈ�Ͽ� ������ ���
	{
		if( m_strImportSceneFile_.IsEmpty() )
		{
			AfxMessageBox("����Ʈ�� ���� �������� �ʾҽ��ϴ�.");
			return false;
		}
	}

	//�������� ����
	int a = m_zoneComboBox_.GetCurSel();
	if(a < 0)
	{
		AfxMessageBox("�� ���̵� �������� �ʾҽ��ϴ�.");
		return false;
	}
	CString str_combo;
	m_zoneComboBox_.GetLBText(a, str_combo);
	if(str_combo.GetLength() == 0)
	{
		AfxMessageBox("�� ���̵� �������� �ʾҽ��ϴ�.");
		return false;
	}
	//�׽�Ʈ ���������� ���
	if(str_combo.Compare("�׽�Ʈ��")  == 0)
	{
		CDlgInputMapName dlg;
		dlg.m_strDlgCaption = "�׽�Ʈ ������ ����";
		if (IDOK == dlg.DoModal())
		{
			m_strZonePath_ = (LPTSTR)(LPCTSTR)(dlg.m_cstrTestDirectory + "\\");
			m_strZoneFile_ = (LPTSTR)(LPCTSTR)dlg.m_cstrTestFile;
			if(GetFileAttributes((m_strZonePath_ + m_strZoneFile_).c_str()) != INVALID_FILE_ATTRIBUTES)
			{
				AfxMessageBox("�̹� �����ϴ� ���� �����Ϸ� �ϰ� �ֽ��ϴ�.");
				return false;
			}
			m_nZoneID_ = -1;
			return true;
		}
		else
		{
			AfxMessageBox("�׽�Ʈ ���� ��� �ش� ��ο� ������ �����ϼž� �մϴ�.");
			return false;
		}
	}
	else
	{
		// "//"�� �������� ��ũ��
		int b = str_combo.Find("//");
		int c = str_combo.GetLength();

		std::string strID(str_combo.GetString());
		int start_id = strID.find(":");
		int end_id = strID.find(")");
		m_nZoneID_ = atoi(strID.substr(start_id+1, end_id - start_id-1).c_str());

		CString newZoneId = str_combo.Right(c-b-2);
		CString fullPath(m_strDataPath_.c_str());
		fullPath = fullPath + "zone\\";
		fullPath = fullPath + newZoneId;
		if(GetFileAttributes(fullPath) == INVALID_FILE_ATTRIBUTES)
		{
			AfxMessageBox("�ش� �� ID�� ������ �������� �ʽ��ϴ�. ������ �ٽ� �õ��ϼ���.");
			return false;
		}
		m_strZonePath_ = (LPTSTR)(LPCTSTR)(fullPath + "\\");
		newZoneId += ".zone.xml";
		m_strZoneFile_ = (LPTSTR)(LPCTSTR)(newZoneId);
		if(GetFileAttributes((m_strZonePath_ + m_strZoneFile_).c_str()) != INVALID_FILE_ATTRIBUTES)
		{
			AfxMessageBox("�̹� �����ϴ� ���� �����Ϸ� �ϰ� �ֽ��ϴ�.");
			return false;
		}
		return true;
	}
}

bool CDlgMapNew::IsValidNewMapInfo_()
{
	UpdateData(TRUE);
	
	if( m_enumNewType_ == ZONE) //zone ����
		return IsValidNewZone_();
	else //field ����
	{
		//���� �ʵ� ID
		int field_id = m_fieldComboBox_.GetCurSel();
		if(field_id < 0)
		{
			AfxMessageBox("�ʵ� ���̵� �������� �ʾҽ��ϴ�.");
			return false;
		}

		CString str_combo;
		m_fieldComboBox_.GetLBText(field_id, str_combo);
		std::string strID(str_combo.GetString());
		int start_id = strID.find(":");
		int end_id = strID.find(")");
		int newFieldID = atoi(strID.substr(start_id+1, end_id - start_id-1).c_str());

		//zone_id : field_id ���Ͽ� field ����
		//a:0 - ���ο� �����κ��� ����
		//-1,a - ���� �ʵ�� ���� ����
		// a, -1 ���� ������ ���� ����
		if(m_nFieldType_ == 0) //create with new zone
		{
			//�⺻���� �� �������� Ȯ��
			if(IsValidNewZone_() ==false)
				return false;
			if(m_nZoneID_ == -1)
			{
				AfxMessageBox("���� ���̵� �����Ǿ� ���� �ʽ��ϴ�.");
				return false;
			}
			std::string zone_name(m_strZonePath_ + m_strZoneFile_);
			m_nFromFieldID_ = 0;
		}
		//������ �͵�� ���� ���� - �� Ȥ�� �ʵ�
		else
		{
			int selected_list_index = m_ctlListBox_.GetCurSel();
			if(selected_list_index < 0 )
			{
				AfxMessageBox("����Ʈ�� �߸� ���õǾ����ϴ�.");
				return false;
			}

			//������ ������ ���� ����
			if(m_nFieldType_ == 1)
			{
				CString cstrZone;
				m_ctlListBox_.GetText(selected_list_index, cstrZone);
				std::string strZoneId(cstrZone.GetString());
				int start_id = strZoneId.find(":");
				int end_id = strZoneId.find(")");
				m_nZoneID_ = atoi(strZoneId.substr(start_id+1, end_id - start_id-1).c_str());
				m_nFromFieldID_ = -1;
			}
			//������ �ʵ�� ���� ����
			else
			{
				m_nZoneID_ = -1;

				CString cstrField;
				m_ctlListBox_.GetText(selected_list_index, cstrField);
				std::string strFieldId(cstrField.GetString());
				int start_id = strFieldId.find(":");
				int end_id = strFieldId.find(")");
				m_nFromFieldID_ = atoi(strFieldId.substr(start_id+1, end_id - start_id-1).c_str());
			}
		}

		CString fullPath(m_strDataPath_.c_str());
		fullPath = fullPath + "field\\";
		if(GetFileAttributes(fullPath) == INVALID_FILE_ATTRIBUTES)
		{
			AfxMessageBox("�ش� �ʵ��� ������ �������� �ʽ��ϴ�. ������ �ٽ� �õ��ϼ���.");
			return false;
		}
		CString newFieldName;
		newFieldName.Format("%05d", newFieldID);
		newFieldName = fullPath + newFieldName + ".field.xml";
		if(GetFileAttributes(newFieldName) != INVALID_FILE_ATTRIBUTES)
		{
			AfxMessageBox("�����ϴ� �ʵ带 �ٽ� �����Ϸ� �ϰ� �ֽ��ϴ�.");
			return false;
		}
		m_strFieldName_ = newFieldName.GetString();
		return true;
	}
}

//PutNewZoneInfoForCreation_()�� ���� �� ������ �ʿ��� ���� ���.
//NewField�� ID�� ���� ������ map_info�� ä���.
void CDlgMapNew::PutNewZoneInfoForCreation_()
{
	// ���ο� �������� ����
	if( m_nZoneType_ == 0 )
	{
		// �ƿ����� ����
		m_pControlMediator_->m_mapInfo.m_height = m_mapHeight_;
		m_pControlMediator_->m_mapInfo.m_width = m_mapWidth_;
		m_pControlMediator_->m_mapInfo.m_colFace = m_colFace_;
		m_pControlMediator_->m_mapInfo.m_rowFace = m_rowFace_;
		m_pControlMediator_->m_mapInfo.m_strTerrainBaseTile = m_strMapBaseTile_;
		m_pControlMediator_->m_mapInfo.m_strImportSceneFile.clear();
	}
	else // ����Ʈ�� ���� ���� : Indoor
	{
		// �ε��� ����
		m_pControlMediator_->m_mapInfo.m_strImportSceneFile = m_strImportSceneFile_.GetString();
	}

	m_pControlMediator_->m_mapInfo.m_eMapEditType = MAP_EDIT_TYPE_SCENE;
	m_pControlMediator_->m_mapInfo.m_strSceneFile = m_strZoneFile_;
	m_pControlMediator_->m_mapInfo.m_strScenePath = m_strZonePath_;

	// init logic object
	m_pControlMediator_->GetToolLogicObjectMgr()->InitLogicObjectManager<CEntityObject>();
}

bool CDlgMapNew::IsCreateAtLoaded_()
{
	int iResult;
	if (m_pControlMediator_->GetToolCmdMgr()->IsMapChanged() == false )
	{
		iResult = IDNO;
	}
	else
	{
		iResult = MessageBox("�ε�� ���� �����մϴ�. �����Ͻðڽ��ϱ�?",
									"��� - �ε�� ���� �ֽ��ϴ�.",
									MB_YESNOCANCEL | MB_ICONASTERISK);
	}
	
	switch ( iResult )
	{
	case IDCANCEL:
		return false;
	case IDYES:
	{
		AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_SAVEALL);

		// reset resource
		m_pControlMediator_->ResetResource();
		m_pControlMediator_->GetHelperResource()->DeleteTempFile();

		// put information
		PutNewZoneInfoForCreation_();
		if(m_enumNewType_ == ZONE)
		{
			m_pControlMediator_->CreateZone();
		}
		else
		{
			CWorkSpaceField* pField = m_pControlMediator_->GetWorkSpacePtr<CWorkSpaceField>();
			pField->SetFieldCreationInfo(m_nZoneID_, m_nFromFieldID_, m_strFieldName_.c_str());
			m_pControlMediator_->CreateWorkSpaceFile(pField->GetWorkSpaceKeyword(), MAP_EDIT_TYPE_LOGIC);
		}
		SetToolAfterCreationMap_(m_enumNewType_);
		return true;
	}
	case IDNO:

		m_pControlMediator_->ResetResource();
		m_pControlMediator_->GetHelperResource()->DeleteTempFile();

		PutNewZoneInfoForCreation_();
		if(m_enumNewType_ == ZONE)
		{
			m_pControlMediator_->CreateZone();
		}
		else
		{
			CWorkSpaceField* pField = m_pControlMediator_->GetWorkSpacePtr<CWorkSpaceField>();
			pField->SetFieldCreationInfo(m_nZoneID_, m_nFromFieldID_, m_strFieldName_.c_str());
			m_pControlMediator_->CreateWorkSpaceFile(pField->GetWorkSpaceKeyword(), MAP_EDIT_TYPE_LOGIC);
		}
		SetToolAfterCreationMap_(m_enumNewType_);
		return true;
	}

	AfxMessageBox("�ε����� �� ��å�� �߰��� ���õ��� �ʰ� ���ϵǾ����ϴ�.");
	return false;
}

void CDlgMapNew::OnBnClickedZoneOk()
{
	if(!IsValidNewMapInfo_())
		return;

	if(m_pControlMediator_->m_mapInfo.m_bLoaded)
	{
		if(!IsCreateAtLoaded_())
			return;
	}
	else
	{
		PutNewZoneInfoForCreation_();
		m_pControlMediator_->CreateZone();
		SetToolAfterCreationMap_(ZONE);
	}
	ShowWindow(SW_HIDE);
	m_bActive_ = false;
}

void CDlgMapNew::OnBnClickedZoneCancel()
{
	ShowWindow(SW_HIDE);
	m_bActive_ = false;
}

//�ʵ���
void CDlgMapNew::OnBnClickedOk()
{
	if(!IsValidNewMapInfo_())
		return;

	if(m_pControlMediator_->m_mapInfo.m_bLoaded)
	{
		if(!IsCreateAtLoaded_())
			return;
	}
	else
	{
		PutNewZoneInfoForCreation_();

		CWorkSpaceField* pField = m_pControlMediator_->GetWorkSpacePtr<CWorkSpaceField>();
		_ASSERT(pField);
		pField->SetFieldCreationInfo(m_nZoneID_, m_nFromFieldID_, m_strFieldName_.c_str());
		m_pControlMediator_->CreateWorkSpaceFile(pField->GetWorkSpaceKeyword(), MAP_EDIT_TYPE_LOGIC);
		SetToolAfterCreationMap_(FIELD);
	}

	//TODO indoor field�� ���鶧
	m_pControlMediator_->GetWorkSpaceMgr()->ChagneWorkSpaceWithMainFrame("field.xml", NULL);
	ShowWindow(SW_HIDE);
	m_bActive_ = false;
}

void CDlgMapNew::OnBnClickedCancel()
{
	ShowWindow(SW_HIDE);
	m_bActive_ = false;
}

void CDlgMapNew::OnBnClickedNewZone()
{
	EnableZone_(TRUE);
	GetDlgItem(IDC_STATIC_ZONE_GROUP)->SetWindowText("�� ����");
	EnableImportSceneCtrl_(FALSE);
	m_ctlListBox_.ShowWindow(SW_HIDE);
	UpdateData(TRUE);
}

void CDlgMapNew::OnBnClickedFromZone()
{
	EnableZone_(FALSE);
	GetDlgItem(IDC_STATIC_ZONE_GROUP)->SetWindowText("�ʵ带 ������ ��");
	m_ctlListBox_.ShowWindow(SW_SHOW);
	UpdateListBox_(0);
	UpdateData(TRUE);
}

void CDlgMapNew::OnBnClickedFromField()
{
	EnableZone_(FALSE);
	GetDlgItem(IDC_STATIC_ZONE_GROUP)->SetWindowText("������ �ʵ�");
	m_ctlListBox_.ShowWindow(SW_SHOW);
	UpdateListBox_(1);
	UpdateData(TRUE);
}

void CDlgMapNew::UpdateListBox_(int nIndex)
{
	m_ctlListBox_.ResetContent();
	for ( int i = 0; i<(int)m_listBoxArray_[nIndex].size(); ++i )
	{
		m_ctlListBox_.InsertString(i, m_listBoxArray_[nIndex][i].c_str());
	}
}

void CDlgMapNew::OnBnClickedButtonFieldListEditor()
{
	int nListIndex = m_nFieldType_ -1;
	if (nListIndex == 1) //�ʵ�κ��� ����
		CMFCHelper::OpenFieldListEditorWithZone();
	else
		CMFCHelper::OpenZoneListEditorWithField();

	OnCbnDropdownComboZoneId();
	OnCbnDropdownComboFieldId();

	if (nListIndex >= 0 )
		UpdateListBox_(nListIndex);
}

void CDlgMapNew::OnCbnDropdownComboZoneId()
{
	CXMLResourceList::GetReference().LoadZoneList();
	CXMLResourceList::GetReference().PutComboBox(m_zoneComboBox_, CXMLResourceList::ZONE_LIST);
	PutListStringFromComboBox_(m_listBoxArray_[0], m_zoneComboBox_);
}

void CDlgMapNew::OnCbnDropdownComboFieldId()
{
	CXMLResourceList::GetReference().LoadFieldList();
	CXMLResourceList::GetReference().PutComboBox(m_fieldComboBox_, CXMLResourceList::FIELD_LIST);
	PutListStringFromComboBox_(m_listBoxArray_[1], m_fieldComboBox_);
}

void CDlgMapNew::PutListStringFromComboBox_(std::vector<string>& _refVec, CComboBox& _refCombo)
{
	_refVec.clear();

	CString comboContent;
	int combo_num = _refCombo.GetCount() - 1; //�޺��ڽ����� �׽�Ʈ �������� �� �ִ�.
	for(int a = 0; a < combo_num; ++a)
	{
		_refCombo.GetLBText(a, comboContent);
		_refVec.push_back((LPTSTR)(LPCTSTR)comboContent);
	}
}

void CDlgMapNew::SetToolAfterCreationMap_(MAP_NEW_TYPE _type)
{
	m_pControlMediator_->GetHelperResource()->ResetRefMesh(CXMLUserConfig::GetReference().GetRefMeshName());
	if(_type == ZONE)
	{
		m_pControlMediator_->GetWorkSpaceMgr()->ChagneWorkSpaceWithMainFrame
		(
			"zone.xml",
			(m_pControlMediator_->m_mapInfo.m_strScenePath + m_pControlMediator_->m_mapInfo.m_strSceneFile).c_str()
		);
	}
	else
	{
		CWorkSpaceField* pField = m_pControlMediator_->GetWorkSpacePtr<CWorkSpaceField>();
		std::string fullPath(pField->m_fieldInfo.m_strFieldPath + pField->m_fieldInfo.m_strFieldFile);
		m_pControlMediator_->GetWorkSpaceMgr()->ChagneWorkSpaceWithMainFrame
		(
			"field.xml",
			fullPath.c_str()
		);
	}
}
