#pragma once


// CDlgCutSceneMoveKey ��ȭ �����Դϴ�.

class CDlgCutSceneMoveKey : public CDialog
{
	DECLARE_DYNAMIC(CDlgCutSceneMoveKey)

public:
	CDlgCutSceneMoveKey(float _fTime, const char* _pSzPos, const char* _pSzDir, bool _bIsWarp = false, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgCutSceneMoveKey();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CUTSCENE_MOVEKEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	float	m_fTime;
	CString	m_strPos;
	CString	m_strDir;
	BOOL	m_bWarpLocation;
};
