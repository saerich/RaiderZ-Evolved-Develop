
// RS3DebuggerDlg.h : ��� ����
//

#pragma once

#include "RDebuggingInfo.h"
#include "RCPUAnalyzeDialog.h"
#include "RResourceAnalyzeDialog.h"


// CRS3AnalyzeDialoga ��ȭ ����
class CRS3AnalyzeDialoga : public CDialog
{
// �����Դϴ�.
public:
	CRS3AnalyzeDialoga(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	virtual ~CRS3AnalyzeDialoga(void);

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_RS3DEBUGGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	RCPUAnalyzeDialog*				m_pProfileDialog;
	RResourceAnalyzeDialog*	m_pPerformanceViewerDialog;

public:
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnBnClickedButton1();	//PerformanceViewer��ư
	afx_msg void OnBnClickedButton2();	//Profile��ư

private:
	void	PerformanceViewerDialogOnOff(void);
	
};
