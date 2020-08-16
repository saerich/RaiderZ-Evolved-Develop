// DlgCutSceneTalentKey.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Plugin_CutScene.h"
#include "VD_DlgCutSceneTalentKey.h"
#include "VD_DlgInputCommon.h"


// CDlgCutSceneTalentKey ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgCutSceneTalentKey, CDialog)

CDlgCutSceneTalentKey::CDlgCutSceneTalentKey(float _fTime, const int _nID, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCutSceneTalentKey::IDD, pParent)
{
	CDlgInputCommon::m_isActive = true;

	m_fTime = _fTime;
	m_nID	= _nID;
}

CDlgCutSceneTalentKey::~CDlgCutSceneTalentKey()
{
	CDlgInputCommon::m_isActive = false;
}

void CDlgCutSceneTalentKey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_TALENTKEY_TIME, m_fTime);
	DDX_Text(pDX, IDC_EDIT_TALENTKEY_ID, m_nID);
}


BEGIN_MESSAGE_MAP(CDlgCutSceneTalentKey, CDialog)
END_MESSAGE_MAP()


// CDlgCutSceneTalentKey �޽��� ó�����Դϴ�.
