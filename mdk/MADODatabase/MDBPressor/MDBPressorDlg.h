// MDBPressorDlg.h : ��� ����
//

#pragma once

#include "Resource.h"
#include "afxwin.h"
#include "afxcmn.h"


class LogListBox;
class TestReportList;
class MDBPressor;

// MDBPressorDlg ��ȭ ����
class MDBPressorDlg : public CDialog
{
public:
	enum { IDD = IDD_MDBPRESSOR_DIALOG };

	MDBPressorDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	BOOL				Run();

	afx_msg void		OnClose();
	afx_msg void		OnBnClickedCheckTimer();
	afx_msg void		OnEnChangeEditTimeNTerm();
	afx_msg void		OnBnClickedBtnReload();
	afx_msg void		OnBnClickedBtnQueryStart();
	afx_msg void		OnBnClickedBtnQueryStop();

protected:
	HICON m_hIcon;

	virtual void		DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

	// ������ �޽��� �� �Լ�
	virtual BOOL		OnInitDialog();
	afx_msg void		OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void		OnPaint();
	afx_msg HCURSOR		OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()	

private:
	TestReportList*		m_pTestReport;
	MDBPressor*			m_pDBPressor;

	CString				m_strConnectionString;

	CString				m_strDataSource;
	CString				m_strCatalog;
	CString				m_strID;
	CString				m_strPassword;
	CString				m_strPoolSize;

	CString				m_strQuery;
	int					m_nExecuteCount;

	CListCtrl			m_listTestReport;

	void				InitDlgItem();
	void				LoadSettingFile();
	void				OutputEnvInfo();
	void				CheckTimerSet(BOOL bCheck);
	void				EnableTimerSet(BOOL bEnable);
	void				OutputTimerInfo();

	unsigned int		CalTimerWholeTime();
	unsigned int		CalTimerWholeTestCount();

	void				OutputEndReport();
	void				OutputTestSummaryReport();
	void				OutputTimerInfoReport();

};
