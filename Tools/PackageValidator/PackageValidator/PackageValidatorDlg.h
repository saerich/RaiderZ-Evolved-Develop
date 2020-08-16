
// PackageValidatorDlg.h : ��� ����
//

#pragma once


// CPackageValidatorDlg ��ȭ ����
class CPackageValidatorDlg : public CDialog
{
// �����Դϴ�.
public:
	CPackageValidatorDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PACKAGEVALIDATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

	void Validate();


// �����Դϴ�.
protected:
	HICON m_hIcon;
	int m_nSuccess;
	int m_nFail;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStart();
	DECLARE_MESSAGE_MAP()
};
