#pragma once


// CDlgCutSceneBGMKey ��ȭ �����Դϴ�.

class CDlgCutSceneBGMKey : public CDialog
{
	DECLARE_DYNAMIC(CDlgCutSceneBGMKey)

public:
	CDlgCutSceneBGMKey(float _fTime, const char* _pSzFileName, float _fVolume, bool _bLoop, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgCutSceneBGMKey();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CUTSCENE_BGMKEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	float	m_fTime;
	CString m_strFileName;
	float	m_fVolume;
	BOOL	m_bLoop;
};
