// VD_DlgCutSceneDOFKey.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
//#include "CoreToolRS3.h"
#include "VD_DlgCutSceneDOFKey.h"


// CDlgCutSceneDOFKey ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgCutSceneDOFKey, CDialog)

CDlgCutSceneDOFKey::CDlgCutSceneDOFKey(float _fTime, float _fFocusDist, float _fFocusRange, float _fCocSize, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCutSceneDOFKey::IDD, pParent)
{
	m_fTime = _fTime;
	m_fFocusDist = _fFocusDist;
	m_fFocusRange = _fFocusRange;
	m_fCocSize = _fCocSize;
}

CDlgCutSceneDOFKey::~CDlgCutSceneDOFKey()
{
}

void CDlgCutSceneDOFKey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_DOFKEY_TIME	, m_fTime);
	DDX_Text(pDX, IDC_EDIT_DOFKEY_FOCUSDIST, m_fFocusDist);
	DDX_Text(pDX, IDC_EDIT_DOFKEY_FOCUSRANGE, m_fFocusRange);
	DDX_Text(pDX, IDC_EDIT_DOFKEY_COCSIZE, m_fCocSize);
}


BEGIN_MESSAGE_MAP(CDlgCutSceneDOFKey, CDialog)
END_MESSAGE_MAP()


// CDlgCutSceneDOFKey �޽��� ó�����Դϴ�.
