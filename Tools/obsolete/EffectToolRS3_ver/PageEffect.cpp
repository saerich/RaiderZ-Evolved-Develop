// Page1.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "PageEffect.h"
#include "RBaseEmitter.h"

// CPage1 ��ȭ �����Դϴ�.

inline void _ui_SetSlider(CSliderCtrl *pCtrl, int iMin, int iMax, int iPos)
{
	pCtrl->ClearTics();
	pCtrl->ClearSel();

	pCtrl->SetRangeMin(iMin);
	pCtrl->SetRangeMax(iMax);
	pCtrl->SetPos(iPos);
}

IMPLEMENT_DYNAMIC(CPage1, CPPage)
CPage1::CPage1()
: CPPage(CPage1::IDD)
, m_strEffectName(_T("MADDUCK"))
, m_bMeshLooping(FALSE)
, m_fScale(1.0f)
{
}

CPage1::~CPage1()
{
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CPPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_EMITTER2, m_EmitterList);
	DDX_Text(pDX, IDC_EFFECT_NAME, m_strEffectName);

	DDX_Check(pDX, IDC_CHECK_MESHLOOPING, m_bMeshLooping);
	DDX_Control(pDX, IDC_SLIDER_SCALE, m_ctlScale);
}


BEGIN_MESSAGE_MAP(CPage1, CPPage)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_CHECK_MESHLOOPING, OnBnClickedCheckMeshlooping)

	ON_EN_CHANGE(IDC_EFFECT_NAME, OnEnChangeEdites)

	ON_WM_HSCROLL()
	ON_LBN_SELCHANGE(IDC_LIST_EMITTER2, OnLbnSelchangeListEmitter2)
END_MESSAGE_MAP()


// CPage1 �޽��� ó�����Դϴ�.

BOOL CPage1::OnInitDialog()
{
	CPPage::OnInitDialog();

	_ui_SetSlider(&m_ctlScale, 0, 30, 10);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CPage1::InitValue()
{
	m_EmitterList.ResetContent();
}

void CPage1::SetEffectName(const char * const pName)
{
	m_strEffectName = pName;
}

void CPage1::OnBnClickedButtonLoad()
{
	SendMessageLoadEmitter();
}

void CPage1::AddListBox(const char * const pName)
{
	m_EmitterList.InsertString(0, pName);// .AddString(pName);
}

bool CPage1::IsMeshEffect(int iSel)
{
	if( iSel < 0 ) return false;

	CString str;
	m_EmitterList.GetText(iSel, str);

	int nPos = str.Find(".elu");
	if( nPos != -1) return true;

	return false;
}

void CPage1::OnBnClickedButtonDel()
{
	int iSel = m_EmitterList.GetCurSel();
	if ( iSel < 0 )
		return;

	// [9/5/2006 madduck]
	// TODO : �޽��� ���� �߰� �Ҽ� �ְ�???
	if(IsMeshEffect(iSel))
	{
		SendMessageDelMesh(iSel);
		return;
	}

	SendMessageDelEmitter(iSel);
}

void CPage1::OnBnClickedButtonAdd()
{
	// Page5���� Type�� ���ͼ� ����
	SendMessageCreateEmitter();
}

void CPage1::OnBnClickedButtonSave()
{
	int iSel = m_EmitterList.GetCurSel();
	if ( iSel < 0 )
		return;

	if(IsMeshEffect(iSel)) return;

	SendMessageSaveEmitter(iSel);
}

void CPage1::OnEnChangeEdites()
{
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	m_ctlScale.SetPos(m_fScale * 10.0f);

	SendMessageSetEffectName();
}

void CPage1::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	m_fScale = m_ctlScale.GetPos()/10.0f;

	SendMessageMeshModify();

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPage1::OnLbnSelchangeListEmitter2()
{
	int iSel = m_EmitterList.GetCurSel();
	if ( iSel < 0 )
		return;

	if(IsMeshEffect(iSel))
	{
		SendMessageSelectMesh(iSel);
		return;
	}

	SendMessageSelectEmitter(iSel);
}

void CPage1::OnBnClickedCheckMeshlooping()
{
	UpdateData(TRUE);

	SendMessageMeshModify();
}