// SDIdocDoc.cpp : CCoreToolRS3Doc Ŭ������ ����
//

#include "stdafx.h"
//#include "CoreToolRS3.h"

#include "MProfiler.h"
//#include "RWaterSystem.h"
//#include "RTreeSystem.h"

#include "MainFrm.h"
#include "CoreToolRS3_Doc.h"

#include "C_ControlMediator.h"
#include "C_ControlMediatorHelperResource.h"
#include "C_ControlMediatorHelperAction.h"
#include "C_StatusBarHandler.h"

#include "M_ToolCmdMgr.h"
#include "M_ToolActor.h"

#include "M_ToolSceneManager.h"
#include "M_ToolSceneNodeInstance.h"

#include "C_XMLConfig.h"
#include "C_XMLUserConfig.h"
#include "C_XMLResourceList.h"

#include "V_MFCHelper.h"
#include "RXMLTemplate.h" //TXML

#include "VR_EtcMapView.h"
#include "VL_SceneNodeList.h"
#include "VL_RelaxAll.h"
#include "VR_EnvWeather.h"

#include <vector>
#include <string>

#include "WS_WorkSpaceManager.h"
#include "MFileSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCoreToolRS3Doc

IMPLEMENT_DYNCREATE(CCoreToolRS3Doc, CDocument)

BEGIN_MESSAGE_MAP(CCoreToolRS3Doc, CDocument)
	//����
	ON_COMMAND(ID_CAMERA_RESET, OnCameraReset)
	ON_COMMAND(ID_GO_TO_SELECT_CENTER, OnGoToSelectCenter)
	ON_COMMAND(ID_RESOURCE_REFRESH, OnResourceRefresh)
	ON_COMMAND(ID_RELAX_ALL, OnRelaxAll)
	ON_COMMAND(ID_WATER_PLANE_EDIT, OnWaterPlaneEdit)
	//����
	ON_COMMAND(ID_TOOLS_REF_MESH, OnSetReferenceMesh)
END_MESSAGE_MAP()


// CCoreToolRS3Doc ����/�Ҹ�

CCoreToolRS3Doc::CCoreToolRS3Doc()
: ExitCode(0)
, m_pControlMediator_(0)
, m_pMainFrame_(0)
{
}

CCoreToolRS3Doc::~CCoreToolRS3Doc()
{
}

BOOL CCoreToolRS3Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}

BOOL CCoreToolRS3Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	//�𷡽ð� ����
	HCURSOR hOld = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	//����
	MAP_EDIT_TYPE eMapEditType = OpenMapFile(lpszPathName);

	//�𷡽ð� ��
	::SetCursor(hOld);

	if( eMapEditType == MAP_EDIT_TYPE_UNKNOWN )
		return FALSE;
	return TRUE;
}

// CCoreToolRS3Doc serialization

void CCoreToolRS3Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CCoreToolRS3Doc ����

#ifdef _DEBUG
void CCoreToolRS3Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCoreToolRS3Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void CCoreToolRS3Doc::InitDocument(CControlMediator* _pControlMediator, CMainFrame* _pMainFrame)
{
	m_pControlMediator_ = _pControlMediator;
	m_pMainFrame_ = _pMainFrame;
}

void CCoreToolRS3Doc::SetToolAfterMapLoaded_(MAP_EDIT_TYPE _type, const char* _fileName, const std::string& _rWorkSpaceKeyword)
{
	if (MAP_EDIT_TYPE_EXIT_FAILD == _type || MAP_EDIT_TYPE_EXIT_SUCCESSED == _type )
		return;

	//�� ���� �޽��� Ȯ��
	rs3::RToolSceneManager* pSceneManager = m_pControlMediator_->GetToolSceneManager();
	ASSERT(pSceneManager);

	// �����޽��� ��� ���� ����
	/*rs3::RTreeSystem* pTreeSystem = pSceneManager->GetTreeSystem();
	ASSERT(pTreeSystem);*/
	//std::vector<std::string>& vecTreeErrorMessage = pTreeSystem->GetErrorMessages();
	std::vector<std::string> vecTreeErrorMessage;

	std::vector<std::string>& vecSceneManagerErrorMessage = pSceneManager->GetErrorMessage();
	if(vecTreeErrorMessage.empty() == false || vecSceneManagerErrorMessage.empty() == false)
	{
		std::string message;

		//Tree Error
		int size = vecTreeErrorMessage.size();
		for(int i = 0; i < size; ++i)
		{
			message.append(vecTreeErrorMessage[i] + "\n");
		}

		//Scene Error
		size = vecSceneManagerErrorMessage.size();
		for(int a = 0; a < size; ++a)
		{
			message.append(vecSceneManagerErrorMessage[a] + "\n");
		}

		AfxMessageBox(message.c_str());
	}

	//workspace ���� �޽��� Ȯ��
	std::deque<std::string>& deqError = m_pControlMediator_->GetWorkSpaceMgr()->GetWorkSpaceErrorMessage();
	if(deqError.empty() == false)
	{
		int size = deqError.size();
		std::string message;
		for(int i = 0; i < size; ++i)
		{
			message.append(deqError[i] + "\n");
		}
		AfxMessageBox(message.c_str());
	}

	//�ʱ�ȭ
	if(_type != MAP_EDIT_TYPE_ERROR)
	{
		m_pMainFrame_->ChangeWorkSpace(_rWorkSpaceKeyword);
		m_pMainFrame_->SetWindowText(_fileName);

		//set reference mesh
		m_pControlMediator_->GetHelperResource()->ResetRefMesh(CXMLUserConfig::GetReference().GetRefMeshName());

		//set work brush size
		int workBrushSize = CMFCHelper::GetBrushWorkAreaSize();
		CMFCHelper::SetBrushWorkAreaSize(workBrushSize);

		//set last wbState
		int wbState;
		if(CXMLUserConfig::GetReference().IsRememberWork() && CXMLUserConfig::GetReference().GetLastWork(wbState))
			m_pControlMediator_->GetWorkSpaceMgr()->TriggerWBState( (WORKBENCH_STATE::STATE)wbState, true );

		AfxGetApp()->AddToRecentFileList(_fileName);
	}

	deqError.clear();
}

MAP_EDIT_TYPE CCoreToolRS3Doc::OpenMapFile(const CString& sFile)
{
	// ���� �̸����κ��� ����ó��
	MAP_EDIT_TYPE type = MAP_EDIT_TYPE_UNKNOWN;
	if(sFile.GetLength() == 0)
		return type;

	// ���α׷��� �����ϸ鼭 ���ö� �Ҹ� �Ŷ��
	if (NULL == m_pControlMediator_)
	{
		EnterFileName = sFile;
		return MAP_EDIT_TYPE_ERROR;
	}

	// pre_open_doc message
	m_pControlMediator_->BroadCastMessage(VIEW_MESSAGE::PRE_OPEN_DOCUMENT);

	std::string strFileName = LPSTR(LPCTSTR(sFile));
	size_t nExtPos = strFileName.find_last_of('.');
	nExtPos = strFileName.find_last_of('.', nExtPos-1);
	std::string strWorkSpaceKeyword = strFileName.substr(nExtPos+1);

	mlog("trying to open File %s\n", strFileName.c_str() );

	CWorkSpaceManager* pWorkSpace = m_pControlMediator_->GetWorkSpaceMgr();
	if ( pWorkSpace->IsValidWorkSpaceKeyword(strWorkSpaceKeyword) == false )
	{
		CString message;
		message.Format("�������� �ʴ� ������ �����Դϴ�. %s ���� ���Ŀ� �����ϴ� Workspace�� �ִ��� Ȯ���Ͻʽÿ�.", strWorkSpaceKeyword.c_str());
		AfxMessageBox(message.GetString());
		return type;
	}

	// TODO: cancel �ϸ� MRU ���� ���ŵǴµ� �ذ��ϰ� �ʹ�
	if( IDCANCEL == ConfirmSaveAndClose(type, FROM_WORKSPACE, sFile, strWorkSpaceKeyword) )
		return MAP_EDIT_TYPE_UNKNOWN;

	type = m_pControlMediator_->LoadWorkSpaceFile(sFile, strWorkSpaceKeyword);
	SetToolAfterMapLoaded_(type, sFile, strWorkSpaceKeyword);

	PFC_RESET;

	m_strPathName = sFile; // CDocument::m_strPathName

	if (type == MAP_EDIT_TYPE_EXIT_SUCCESSED)
	{
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
	else if (type == MAP_EDIT_TYPE_EXIT_FAILD)
	{
		_ASSERT(ExitCode);
		*ExitCode = 1;
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
	else
	{
		// Error Message Popup
		const std::string &errorString = m_pMainFrame_->GetErrorString();
		if ( !errorString.empty())
			AfxMessageBox(errorString.c_str(), MB_ICONWARNING);

		m_pMainFrame_->ClearErrorString();
	}

	mlog("%s ������ �������ϴ�\n", strFileName.c_str() );

	return type;
}

int CCoreToolRS3Doc::ConfirmSaveAndClose(MAP_EDIT_TYPE& _maptype, LOAD_TYPE _type, const char* filename, const std::string& _rWorkSpaceKeyword)
{
	// ���� ���� ������ �ٷ� �Ѿ��
	if( m_pControlMediator_->m_mapInfo.m_bLoaded == false )
		return IDNO;

	int iResult = 0;
	if (m_pControlMediator_->GetToolCmdMgr()->IsMapChanged() == false )
	{
		iResult = IDNO;
	}
	else
	{
		iResult = MessageBox(NULL,
						"�ε�� ���� �����մϴ�. �����Ͻðڽ��ϱ�?",
						"��� - �ε�� ���� �ֽ��ϴ�.", MB_YESNOCANCEL | MB_ICONASTERISK);
	}

	switch ( iResult )
	{
	case IDCANCEL:
		return IDCANCEL;
	case IDYES:
		AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_SAVEALL);
		break;
	case IDNO:
		break;
	}

	m_pControlMediator_->BroadCastMessage(VIEW_MESSAGE::PRE_CLOSE_DOCUMENT);
	
	m_pControlMediator_->GetWorkSpaceMgr()->GetCurrentWorkSpace()->OnCloseFile();
	m_pControlMediator_->ResetResource();
	m_pControlMediator_->GetHelperResource()->DeleteTempFile();

	m_pControlMediator_->BroadCastMessage(VIEW_MESSAGE::POST_CLOSE_DOCUMENT);

	return iResult;
}

////////////// ���� �޴�
//
void CCoreToolRS3Doc::SaveMap(const char* _pSzSaveAsFileName /* = NULL */)
{
	if(m_pControlMediator_->m_mapInfo.m_bLoaded == false) return;

	//�𷡽ð� ����
	HCURSOR hOld = SetCursor(LoadCursor(NULL, IDC_WAIT));

	// �׳� save �̸� ���� �̸��� ã���ش�
	bool bSaveAs = (_pSzSaveAsFileName != NULL );
	string strDocumentFileName = bSaveAs ? _pSzSaveAsFileName : GetPathName();

	std::string strFileName = m_pControlMediator_->SaveWorkSpaceFile(strDocumentFileName.c_str(), bSaveAs);
	_ASSERT(strFileName.empty() == false);

	// Tool Command Reset
	m_pControlMediator_->GetToolCmdMgr()->ResetMapChanged();

	// �ֱ� ���Ͽ� �߰�
	AfxGetApp()->AddToRecentFileList(strFileName.c_str());

	// ������ Ÿ��Ʋ ����
	if(_pSzSaveAsFileName)
	{
		m_pMainFrame_->SetWindowText(strFileName.c_str());

		// save as �̸� document ���� �̸� ����
		m_strPathName = _pSzSaveAsFileName;
	}

	mlog("%s �� ����Ǿ����ϴ�\n", strFileName.c_str() );


	//�𷡽ð� ��
	SetCursor(hOld);
}

////////////// ���� �޴�
//
void CCoreToolRS3Doc::EditUndo()
{
	m_pControlMediator_->GetToolCmdMgr()->UndoCommand();
}

void CCoreToolRS3Doc::OnCameraReset()
{
	using namespace rs3;
	RToolCamera* pCamera = m_pControlMediator_->GetToolCamera();
	if(pCamera == NULL) return;

	const RToolCamera::LOCATION_INFO* pCamInfo
		= pCamera->GetLocation(RToolCamera::DEFAULT_CAMERA);
	pCamera->SetLocation(pCamInfo);
}

////////////// ���� �޴�
//
void CCoreToolRS3Doc::OnSetReferenceMesh()
{
	CFileDialog Dlg(TRUE);
	Dlg.GetOFN().lpstrTitle = _T("���۷��� �޽� ������ �����մϴ�.");
	Dlg.GetOFN().lpstrFilter = "elu����(*.elu)\0*.elu";
	Dlg.GetOFN().Flags |= OFN_NOCHANGEDIR | OFN_READONLY;

	if ( Dlg.DoModal()==IDOK )
	{
		POSITION pos = Dlg.GetStartPosition();
		CString szTo = Dlg.GetNextPathName(pos);

		char szOut[256] = "";
		char szFrom[256] = "";
		_fullpath( szFrom, CXMLConfig::GetInstance()->pcDataPath, 256 );
		
		//fullpath to relative path and save reset name
		PathRelativePathTo(szOut, szFrom, FILE_ATTRIBUTE_DIRECTORY, szTo, FILE_ATTRIBUTE_NORMAL);

		//reset mesh
		m_pControlMediator_->GetHelperResource()->ResetRefMesh(szTo);
		CString relPath(szOut);
		relPath.Replace("\\", "/");
		relPath.Delete(0); //ù��° ������ �����
		relPath = CXMLConfig::GetInstance()->pcDataPath + relPath;
		CXMLUserConfig::GetReference().SetRefMeshName(relPath);
	}
}

void CCoreToolRS3Doc::ExportObj(int nLevel, int nW, int nH)
{
	rs3::RToolTerrain *pTerrain = (rs3::RToolTerrain*) m_pControlMediator_->GetToolSceneManager()->GetCurrentTerrain();
	if ( pTerrain )
	{
		bool bResult = pTerrain->ExportObjFile(nLevel, nW, nH);
		ASSERT(bResult);
	}
	else
	{
		AfxMessageBox("���� �ε�Ǿ� ���� �ʽ��ϴ�.");
	}
}

void CCoreToolRS3Doc::ImportObj(const char *szFilename)
{
	rs3::RToolTerrain *pTerrain = (rs3::RToolTerrain*) m_pControlMediator_->GetToolSceneManager()->GetCurrentTerrain();
	if ( pTerrain )
	{
		std::string strErrmessage;
		bool bResult = pTerrain->ImportObjFile(szFilename,&strErrmessage);
		if ( !bResult )
			AfxMessageBox(strErrmessage.c_str());
	}
	else
	{
		AfxMessageBox("���� �ε�Ǿ� ���� �ʽ��ϴ�.");
	}
}

void CCoreToolRS3Doc::OnRelaxAll()
{
	m_pControlMediator_->GetViewPtr<CDlgRelaxAll>()->Listen( CoreMessage(VIEW_MESSAGE::ACTIVE_VIEW) );
}

void CCoreToolRS3Doc::OnResourceRefresh()
{
	m_pControlMediator_->GetHelperResource()->RefreshZone();
}

void CCoreToolRS3Doc::OnGoToSelectCenter()
{
	rs3::RToolCamera* pCamear = m_pControlMediator_->GetToolCamera();
	if( pCamear == NULL)
		return;

	pCamear->SetPositionToSelectCenter(m_pControlMediator_->GetHelperAction());
}

void CCoreToolRS3Doc::OnWaterPlaneEdit()
{
	m_pControlMediator_->GetViewPtr< CDlgSceneNodeList >()->ShowWaterEditor();
}

void SpeedTreeDiffuseScaleGen()
{

}
