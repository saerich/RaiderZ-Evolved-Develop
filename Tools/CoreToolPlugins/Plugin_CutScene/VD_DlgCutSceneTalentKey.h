#pragma once
#include "afxwin.h"


// CDlgCutSceneTalentKey ��ȭ �����Դϴ�.

class CDlgCutSceneTalentKey : public CDialog
{
	DECLARE_DYNAMIC(CDlgCutSceneTalentKey)

public:
	CDlgCutSceneTalentKey(float _fTime, const int _nID = 0, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgCutSceneTalentKey();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CUTSCENE_TALENTKEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	float	m_fTime;
	int		m_nID;
};
