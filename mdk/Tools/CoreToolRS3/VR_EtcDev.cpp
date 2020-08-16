// V_EditEtcDev.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
//#include "CoreToolRS3.h"
#include "VR_EtcDev.h"

#include "RDevice.h"
#include "RConfiguration.h"
//#include "M_CollisionTree.h"

using namespace rs3;

// CRollupEtcDev ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CRollupEtcDev, CDialog)

CRollupEtcDev::CRollupEtcDev(CWnd* pParent /*=NULL*/)
	: CDialog(CRollupEtcDev::IDD, pParent)
{

}

CRollupEtcDev::~CRollupEtcDev()
{
}

void CRollupEtcDev::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRollupEtcDev, CDialog)
END_MESSAGE_MAP()


// CRollupEtcDev �޽��� ó�����Դϴ�.

void CRollupEtcDev::Listen(CoreMessage& _message)
{
	switch( _message.nID) 
	{
		// Ȱ��
		case VIEW_MESSAGE::ACTIVE_VIEW :
		{
			m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::ETC_DEV);
		}
		break;

		// �� �ε�
		case VIEW_MESSAGE::SCENE_LOADED :
		{
		}
		break;
	}
}

BOOL CRollupEtcDev::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(FALSE);
	return TRUE;
}
