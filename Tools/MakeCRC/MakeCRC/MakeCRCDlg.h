
// MakeCRCDlg.h : ��� ����
//

#pragma once


// CMakeCRCDlg ��ȭ ����
class CMakeCRCDlg : public CDialog
{
// �����Դϴ�.
public:
	CMakeCRCDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MAKECRC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

	void Run( const char* szPath);


// �����Դϴ�.
protected:
	HICON m_hIcon;
	CString m_strPath;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
};
