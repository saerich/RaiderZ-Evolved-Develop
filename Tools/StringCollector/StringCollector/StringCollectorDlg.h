
// StringCollectorDlg.h : ��� ����
//

#pragma once


// CStringCollectorDlg ��ȭ ����
class CStringCollectorDlg : public CDialog
{
// �����Դϴ�.
public:
	CStringCollectorDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_STRINGCOLLECTOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;


	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedSearch();
	afx_msg void OnBnClickedSearch2();
	afx_msg void OnBnClickedStart();

	DECLARE_MESSAGE_MAP()
};
