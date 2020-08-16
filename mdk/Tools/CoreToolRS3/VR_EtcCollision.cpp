// VR_EtcCollision.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "VR_EtcCollision.h"

#include "C_ControlMediator.h"
#include "C_ControlMediatorHelperResource.h"
#include "C_XMLConfig.h"
#include "C_StatusBarHandler.h"

#include "RRenderHelper.h"

#include "RCollisionTreeBuilder.h"
#include "M_CollisionTree.h"

// CRollupEtcCollision ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CRollupEtcCollision, CDialog)

CRollupEtcCollision::CRollupEtcCollision(CWnd* pParent /*=NULL*/)
	: CDialog(CRollupEtcCollision::IDD, pParent)
{
}

CRollupEtcCollision::~CRollupEtcCollision()
{
}

void CRollupEtcCollision::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_CL2_RENDER, m_ctlCl2ViewBtn);
}

BEGIN_MESSAGE_MAP(CRollupEtcCollision, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_CL2_RENDER, &CRollupEtcCollision::OnBnClickedCl2ViewBtn)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_CL2_AS, &CRollupEtcCollision::OnBnClickedButtonCreateCl2As)
END_MESSAGE_MAP()


// CRollupEtcCollision �޽��� ó�����Դϴ�.

void CRollupEtcCollision::Listen(CoreMessage& _message)
{
// 	switch( _message.nID )
// 	{
// 	}
}

void CRollupEtcCollision::OnCancel()
{
}

void CRollupEtcCollision::OnOK()
{
}

void CRollupEtcCollision::Cl2ViewToggle()
{
	if ( m_pControlMediator_ )
	{
		if ( m_ctlCl2ViewBtn.GetCheck())
			m_ctlCl2ViewBtn.SetCheck(0);
		else
			m_ctlCl2ViewBtn.SetCheck(1);

		OnBnClickedCl2ViewBtn();
	}
}

void CRollupEtcCollision::SetCollisionFileName( const string& strFileName )
{
	m_strCollisionFileName = strFileName;
}

void CRollupEtcCollision::OnBnClickedCl2ViewBtn()
{
	if ( m_pControlMediator_ )
	{
		if ( m_ctlCl2ViewBtn.GetCheck())
		{
			m_collisionTree.SetRender(true);
			m_collisionTree.Load( m_strCollisionFileName.c_str() );
		}
		else
		{
			m_collisionTree.SetRender(false);
			m_collisionTree.Destroy();
		}
	}
}

void CRollupEtcCollision::OnBnClickedButtonCreateCl2As()
{
	CFileDialog dlgFile(FALSE, "cl2", "_name");
	dlgFile.m_pOFN->lpstrTitle = "Save cl2 File";
	dlgFile.m_pOFN->lpstrInitialDir= CXMLConfig::GetInstance()->strDataFullPath.c_str();
	if(dlgFile.DoModal()==IDOK)
	{
		CString strFileName = dlgFile.GetPathName();
		CreateCl2( strFileName.GetString() );
	}
}

bool CRollupEtcCollision::CreateCl2( const char* _pSzFileName )
{
	using namespace rs3;

	RCollisionTreeBuilder colTreeBuilder;

	SceneNodeCollisionCollector collector(&colTreeBuilder);

	// �̰� �³� ?
	const rs3::RSceneNode* pSceneNode = &m_pControlMediator_->GetToolSceneManager()->GetRootSceneNode();

	pSceneNode->ExclusionTraverseRefMesh(&collector);
	int num = colTreeBuilder.GetNumOfTreeToBuild();

	// elu�� ��� �浹���� ��Ͽ� ������ ���ϸ���Ʈ�� �ѹ��� ����Ѵ�
	if( colTreeBuilder.m_vecFailedList.size() )
	{
		CString strMessage;
		strMessage.Format("%d���� ������ ���������Դϴ�. �ͽ����͹��׷� �浹������ ����� �����ϴ�. �ٽ� �ͽ���Ʈ ��Ź�ؿ�.\n", colTreeBuilder.m_vecFailedList.size());

		for( size_t i = 0; i < colTreeBuilder.m_vecFailedList.size(); ++i )
		{
			string strFailedName = colTreeBuilder.m_vecFailedList[i] + "\n";
			strMessage += CString(strFailedName.c_str());
		}

		::MessageBox(NULL,strMessage.GetBuffer(),"I'm sorry.",MB_OK);
		colTreeBuilder.m_vecFailedList.clear();
	}

	CString strMessage;
	strMessage.Format("%d���� �����￡�� �浹������ �����մϴ�.", num);
	CStatusBarHandler::GetInstance()->ShowStatusBarMessage(strMessage);

	colTreeBuilder.BuildTree();
	if (false == colTreeBuilder.Save(_pSzFileName))
		return false;

	SetCollisionFileName(_pSzFileName);
	return true;
}
