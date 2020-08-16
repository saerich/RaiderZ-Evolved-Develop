#pragma once
#include "afxwin.h"
#include "afxcmn.h"


namespace rs3 {
	class RDevice;
};
// CDlgProfile ��ȭ �����Դϴ�.

class CDlgProfile : public CDialog
{
	DECLARE_DYNAMIC(CDlgProfile)

public:
	CDlgProfile(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgProfile();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_PROFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	void UpdateProfile(rs3::RDevice *pDevice);

	CListCtrl m_ctlListDP;
	CListCtrl m_ctlListUR;

	virtual BOOL OnInitDialog();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonUpdate();
	CEdit m_editURTip;
	afx_msg void OnLvnItemchangedListUsingresourceProfile(NMHDR *pNMHDR, LRESULT *pResult);
};
