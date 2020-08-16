#pragma once

#include ".\TreeListCtrl\ResourceAnalyzeTreeListCtrl.h"
#include "afxwin.h"

// CPerformanceViewerDialog ��ȭ �����Դϴ�.

class RResourceAnalyzeDialog : public CDialog
{
	DECLARE_DYNAMIC(RResourceAnalyzeDialog)

public:
	RResourceAnalyzeDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~RResourceAnalyzeDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_PERFORMANCE };

	void UpdateProfileInfo(char* pInputBuffer,int Size);
	void UpdateTreeItemData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();
	void		 SetDefaultSize();

	DECLARE_MESSAGE_MAP()

private:
	CResourceAnalyzeTreeListCtrl m_PerformanceTreeListCtrl;
	CButton m_btnExport;
	CButton m_btnUpdate;

	bool m_bUpdated;
	bool m_bSortLess;

public:
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedExport();
	afx_msg void OnHdnItemclickMaterialList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
