// V_EditEtcCamera.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
//#include "CoreToolRS3.h"
#include "VR_EtcCamera.h"

#include "RCameraSceneNode.h"
#include "RCameraAnimationController.h"

#include "VD_DlgCommonFileOpen.h"

#include "RRenderHelper.h"
#include "RDebugPassRenderer.h"
#include "RConfiguration.h"

#include "C_XMLConfig.h"

using namespace rs3;

const char* CAM_ANI_CONTROL_ID = "00_AniController";

// CRollupEtcCamera ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CRollupEtcCamera, CDialog)

CRollupEtcCamera::CRollupEtcCamera(CWnd* pParent /*=NULL*/)
	: CDialog(CRollupEtcCamera::IDD, pParent)
	, m_nCameraCount(0)
{

}

CRollupEtcCamera::~CRollupEtcCamera()
{
}

void CRollupEtcCamera::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ADDED_CAMERA, m_ctlCamListBox);
	DDX_Control(pDX, IDC_CHECK_VIEW_ADDED_CAM, m_ctlViewAddedCamera);
	DDX_Control(pDX, IDC_CHECK_CAM_ANI_APPLY, m_ctlCheckAniApplyToCam);
	DDX_Control(pDX, IDC_CHECK_CAM_ANI_TO_ORG, m_ctlCheckToOrg);
}


BEGIN_MESSAGE_MAP(CRollupEtcCamera, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_CAMERA, &CRollupEtcCamera::OnBnClickedButtonAddCamera)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_APPLY_CAM_ANI, &CRollupEtcCamera::OnBnClickedButtonApplyCamAni)
	ON_BN_CLICKED(IDC_BUTTON_PLAY_CAM_ANI, &CRollupEtcCamera::OnBnClickedButtonPlayCamAni)
	ON_BN_CLICKED(IDC_BUTTON_STOP_CAM_ANI, &CRollupEtcCamera::OnBnClickedButtonStopCamAni)
	ON_BN_CLICKED(IDC_BUTTON_ADDED_CAM_DELETE, &CRollupEtcCamera::OnBnClickedButtonAddedCamDelete)
END_MESSAGE_MAP()


// CRollupEtcCamera �޽��� ó�����Դϴ�.

void CRollupEtcCamera::Listen(CoreMessage& _message)
{
	if (_message.nID == VIEW_MESSAGE::SCENE_LOADED)
	{
		// �����Ϸ��� ������ ī�޶� ����Ʈ �ʱ�ȭ
		int nCameraListCount = m_ctlCamListBox.GetCount();
		if (nCameraListCount > 0)
		{
			for (int i = 0; i < nCameraListCount; ++i)
			{
				RCameraSceneNode* pAddedCamera = (RCameraSceneNode*)m_ctlCamListBox.GetItemDataPtr(i);
				delete pAddedCamera;
			}
			m_ctlCamListBox.ResetContent();
		}
		// TODO : ���ʿ� ���� ������ �о Added Camera List�� ������ִ� �κ��� �߰��ؾ� �մϴ�.

		// ī�޶� �ִϸ��̼� ���� �ʱ�ȭ
		m_strCameraAnimationFile.clear();
		GetDlgItem(IDC_BUTTON_OPEN_CAM_ANI)->SetWindowText("�ִϸ��̼� ���� ����");
	}
	else if (_message.nID == VIEW_MESSAGE::BEFORE_SCENE_CLEAR)
	{
		int nCameraListCount = m_ctlCamListBox.GetCount();
		for (int i = 0; i < nCameraListCount; ++i)
		{
			RCameraSceneNode* pAddedCamera = (RCameraSceneNode*)m_ctlCamListBox.GetItemDataPtr(i);
			delete pAddedCamera;
		}
		m_ctlCamListBox.ResetContent();
	}
}

void CRollupEtcCamera::OnBnClickedButtonAddCamera()
{
	CString strAddedCamName;
	strAddedCamName.Format("added_cam_%03d", m_nCameraCount);
	int nIndex = m_ctlCamListBox.AddString(strAddedCamName);

	RToolCamera* pToolCamera = m_pControlMediator_->GetToolCamera();
	_ASSERT( pToolCamera );

	RCameraSceneNode* pAddedCamera = new RCameraSceneNode;
	pAddedCamera->SetTransform( pToolCamera->GetWorldTransform() );
	m_ctlCamListBox.SetItemDataPtr(nIndex, (void*)(pAddedCamera));
	m_ctlCamListBox.SetCurSel(nIndex);
	m_nCameraCount++;
}

void CRollupEtcCamera::UpdateAndRenderAddedCamera()
{
	int nCameraListCount = m_ctlCamListBox.GetCount();
	if (nCameraListCount == 0)
		return;

	int nSelectedCam = m_ctlCamListBox.GetCurSel();
	bool bAniApply = ( nSelectedCam >= 0 && m_ctlCheckAniApplyToCam.GetCheck());
	bool bVewCam = ( m_ctlViewAddedCamera.GetCheck() == TRUE );
	if (  bAniApply || bVewCam )
	{
		for (int i = 0; i < nCameraListCount; ++i)
		{
			RCameraSceneNode* pAddedCamera = (RCameraSceneNode*)m_ctlCamListBox.GetItemDataPtr(i);
//			pAddedCamera->Update();
			RSceneNodeUpdateVisitor visitor;
			pAddedCamera->Traverse(&visitor);

			if ( nSelectedCam == i && bAniApply )
			{
				// ī�޶� �����϶��� ī�޶� ��ġ���̸� �������� �ʴ´�.
				RToolCamera* pToolCamera = m_pControlMediator_->GetToolCamera();
				_ASSERT( pToolCamera );
				pToolCamera->SetTransform( pAddedCamera->GetWorldTransform() );
			}
			else if (bVewCam)
			{
				RRenderHelper::RenderAxis(pAddedCamera->GetWorldTransform(), 20 );
				m_pControlMediator_->GetToolSceneManager()->GetPassRenderer<RDebugPassRenderer>()->AddDebugBox(pAddedCamera->GetWorldTransform(), 30, 1000);
			}
		}
	}
}

void CRollupEtcCamera::OnBnClickedButtonApplyCamAni()
{
	//list view file dialog ����
	char szEffectPath[256] = "";
	_fullpath( szEffectPath, CXMLConfig::GetInstance()->strCameraAniPath.c_str(), 256 );
	CListViewOpenDlg dlg(szEffectPath);

	dlg.SetTitle("ī�޶� �ִϸ��̼�");
	dlg.SetFilter("Camera Animation ����\0*camera*.ani;");
	if(dlg.DoModal() == IDOK)
	{
		CString fileName(dlg.GetFileName());
		if(fileName.IsEmpty())
			return;

		m_strCameraAnimationFile = fileName.GetString();
		int token_start = m_strCameraAnimationFile.rfind("\\") + 1;
		int token_size = m_strCameraAnimationFile.rfind("elu.ani") - 1 - token_start;
		GetDlgItem(IDC_BUTTON_OPEN_CAM_ANI)->SetWindowText(("���� : " + m_strCameraAnimationFile.substr(token_start, token_size)).c_str() );
	}
}

void CRollupEtcCamera::OnBnClickedButtonPlayCamAni()
{
	int nSelected = m_ctlCamListBox.GetCurSel();
	if (nSelected < 0)
	{
		AfxMessageBox("���� ���õ� ī�޶� �����ϴ�.");
		return;
	}

	if (m_strCameraAnimationFile.empty())
	{
		AfxMessageBox("���� ���õ� �ִϸ��̼��� �����ϴ�.");
		return;
	}

	bool bToOrg = ( m_ctlCheckToOrg.GetCheck() == TRUE );
	RCameraSceneNode* pAddedCamera = (RCameraSceneNode*)m_ctlCamListBox.GetItemDataPtr(nSelected);
	RCameraAnimationController* pController = new RCameraAnimationController(CAM_ANI_CONTROL_ID, (CXMLConfig::GetInstance()->strCameraAniPath + m_strCameraAnimationFile).c_str(), true, bToOrg );
	pController->ResetTimer(-1);
	pAddedCamera->AddCameraController(pController);
}

void CRollupEtcCamera::OnBnClickedButtonStopCamAni()
{
	int nSelected = m_ctlCamListBox.GetCurSel();
	if (nSelected < 0)
		return;

	RCameraSceneNode* pAddedCamera = (RCameraSceneNode*)m_ctlCamListBox.GetItemDataPtr(nSelected);
	// ������ CameraController ���� RCCT_PREUPDATE �������� ����
	RCameraSceneNodeController* pController = pAddedCamera->GetCameraController(RCCT_PREUPDATE, CAM_ANI_CONTROL_ID);
	if (pController)
		pController->ReserveToRemove();
}

void CRollupEtcCamera::OnBnClickedButtonAddedCamDelete()
{
	int nSelected = m_ctlCamListBox.GetCurSel();
	if (nSelected < 0)
		return;

	RCameraSceneNode* pAddedCamera = (RCameraSceneNode*)m_ctlCamListBox.GetItemDataPtr(nSelected);
	delete pAddedCamera;

	m_ctlCamListBox.DeleteString(nSelected);
}
