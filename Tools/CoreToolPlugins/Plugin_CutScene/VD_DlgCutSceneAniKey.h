#pragma once
#include "afxwin.h"


// CDlgCutSceneAniKey ��ȭ �����Դϴ�.

class CDlgCutSceneAniKey : public CDialog
{
	DECLARE_DYNAMIC(CDlgCutSceneAniKey)

public:
	CDlgCutSceneAniKey(float _fTime, const char* _pSzStrAnimation = NULL, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgCutSceneAniKey();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CUTSCENE_ANIKEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	float	m_fTime;
	CString m_strAnimation;
};
