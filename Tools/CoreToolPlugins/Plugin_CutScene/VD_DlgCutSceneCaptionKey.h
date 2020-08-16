#pragma once


// CDlgCutSceneCaptionKey ��ȭ �����Դϴ�.

class CDlgCutSceneCaptionKey : public CDialog
{
	DECLARE_DYNAMIC(CDlgCutSceneCaptionKey)

public:
	CDlgCutSceneCaptionKey(float _fStartTime, float _fDurationTime, CString& _strScript, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgCutSceneCaptionKey();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CUTSCENE_CAPTIONKEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	float	m_fStartTime;
	float	m_fDurationTime;
	CString m_strScrip;
};

// CDlgCutSceneCaptionController ��ȭ �����Դϴ�.

class CDlgCutSceneCaptionController : public CDialog
{
	DECLARE_DYNAMIC(CDlgCutSceneCaptionController)

public:
	CDlgCutSceneCaptionController(const char* _pSzFont, int _nFontSize, BOOL _bBold, BOOL _bItalic, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgCutSceneCaptionController();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CUTSCENE_CAPTION_ATTRIBUTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CString m_strFont;
	int		m_nSize;
	BOOL	m_bBold;
	BOOL	m_bItalic;
};
