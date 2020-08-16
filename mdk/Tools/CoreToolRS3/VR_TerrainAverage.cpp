// V_EditTerrainAverage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "VR_TerrainAverage.h"

//#include "CoreToolRS3.h"
#include "C_ControlMediator.h"

#include "V_MFCHelper.h"

// CRollupTerrainAverage ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CRollupTerrainAverage, CDialog)
CRollupTerrainAverage::CRollupTerrainAverage(CWnd* pParent /*=NULL*/)
	: CDialog(CRollupTerrainAverage::IDD, pParent)
{
}

CRollupTerrainAverage::~CRollupTerrainAverage()
{
}

BOOL CRollupTerrainAverage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// UI Initialize
	//�۾�����
	CMFCHelper::RegisterBrushWorkArea(&m_sliderCtrlSize, &m_fEditSize, &m_cstrViewSize);
	CMFCHelper::SetMFCSlider(&m_sliderCtrlStr, 1, 100, 0);
	m_fStr = (float)m_sliderCtrlStr.GetPos();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CRollupTerrainAverage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SLIDER_AVER_AREA, m_sliderCtrlSize);
	//DDX_Text(pDX, IDC_EDIT_AVER_AREA, m_fEditSize);
	DDX_Text(pDX, IDC_EDIT_AVER_AREA, m_cstrViewSize);

	DDX_Control(pDX, IDC_SLIDER_AVER_STR, m_sliderCtrlStr);
	DDX_Text(pDX, IDC_EDIT_AVER_STR, m_fStr);
}


BEGIN_MESSAGE_MAP(CRollupTerrainAverage, CDialog)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CRollupTerrainAverage::Listen(CoreMessage& _message){
	if(_message.nID == VIEW_MESSAGE::ACTIVE_VIEW){
		m_pControlMediator_->SetCurrentWBState(WORKBENCH_STATE::TERRAIN_AVER);
	}
}

// CEditTerrainAverage �޽��� ó�����Դϴ�.

void CRollupTerrainAverage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar){

	//�۾�����
	int size = m_sliderCtrlSize.GetPos();
	CMFCHelper::SetBrushWorkAreaSize(size);

	//����
	m_fStr = (float)m_sliderCtrlStr.GetPos();
	UpdateData(FALSE);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CRollupTerrainAverage::OnCancel()
{
}
void CRollupTerrainAverage::OnOK()
{
}
