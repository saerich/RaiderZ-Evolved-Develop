// VD_DlgCutSceneAniKey.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
//#include "CoreToolRS3.h"
#include "VD_DlgCutSceneAniKey.h"

#include "VD_DlgInputCommon.h"


// CDlgCutSceneAniKey ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgCutSceneAniKey, CDialog)

CDlgCutSceneAniKey::CDlgCutSceneAniKey(float _fTime, const char* _pSzStrAnimation, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCutSceneAniKey::IDD, pParent)
{
	CDlgInputCommon::m_isActive = true;

	m_fTime = _fTime;
	if (_pSzStrAnimation)
		m_strAnimation = _pSzStrAnimation;
}

CDlgCutSceneAniKey::~CDlgCutSceneAniKey()
{
	CDlgInputCommon::m_isActive = false;
}

void CDlgCutSceneAniKey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_ANIKEY_TIME, m_fTime);
	DDX_Text(pDX, IDC_EDIT_ANIKEY_NAME, m_strAnimation);
}


BEGIN_MESSAGE_MAP(CDlgCutSceneAniKey, CDialog)
END_MESSAGE_MAP()


// CDlgCutSceneAniKey �޽��� ó�����Դϴ�.
