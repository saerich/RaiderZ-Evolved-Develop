#pragma once


// AniBlendDurationDlg ��ȭ �����Դϴ�.

class AniBlendDurationDlg : public CDialog
{
	DECLARE_DYNAMIC(AniBlendDurationDlg)

public:
	AniBlendDurationDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~AniBlendDurationDlg();

	virtual void OnFinalRelease();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_EDIT_ANIBLEND_DURATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CString m_editDuration;
};
