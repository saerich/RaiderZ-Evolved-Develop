#pragma once

#include ".\TreeListCtrl\CPUAnalyzeTreeListCtrl.h"
#include <list>
#include ".\graph\graphctrl.h"

// RCPUAnalyzeDialog ��ȭ �����Դϴ�.

class RCPUAnalyzeDialog : public CDialog
{
	DECLARE_DYNCREATE(RCPUAnalyzeDialog)

public:
	RCPUAnalyzeDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~RCPUAnalyzeDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_PROFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

//------------------------------------------------------------------
public:
	void	AddProfileInfo(char* pBuffer,int Size);
	void	SetProfileIDToName(char* pBuffer,int Size);
	void	SetLeftProfileManager(CProfileManager* pProfileManager);
	void	SetRightProfileManager(CProfileManager* pProfileManager);
	void	ExportToFile( const char* szFileName, CCPUAnalyzeTreeListCtrl& listCtrl );

public:
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnEnChangeEditYscale();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedCheckTopmost();
	afx_msg void OnBnClickedButtonExportLeft();
	afx_msg void OnBnClickedButtonExportRight();

private:
	CCPUAnalyzeTreeListCtrl		m_ProfileTreeListCtrlLeft;
	CCPUAnalyzeTreeListCtrl		m_ProfileTreeListCtrlRight;

	CProfileIDToName*			m_pProfileIDToName;

	bool						m_bPause;
	bool						m_bTopMost;
	
private:
	CGraphCtrl	m_GraphCtrl;
	CEdit		m_YScaleCtrl;	

};
