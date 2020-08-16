#pragma once


// CDlgProgress ��ȭ �����Դϴ�.

class CDlgProgress : public CDialog
{
	DECLARE_DYNAMIC(CDlgProgress)

public:
	CDlgProgress(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgProgress();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_Progress;

	void SetRange(DWORD start, DWORD end);
	void SetStep(int value);
	void SetPos(DWORD pos);
	void StepIt();
	void ShowProgress();
};
