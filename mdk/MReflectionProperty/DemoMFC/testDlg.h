
// testDlg.h : ��� ����
//

#pragma once

#include "..\MFCProperty\ReflectiveUI.h"
#include "..\MFCProperty\ReflectivePropertyGridCtrl.h"
#include "my.h"
#include <string>

// CtestDlg ��ȭ ����
class CtestDlg : public CDialog
{
// �����Դϴ�.
public:
	CtestDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	~CtestDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	void OnPropertyChanged();

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

//#########################################################################################
public:
	void InitPropGrid(void);
	CMFCPropertyGridProperty* createGridGroup(std::string groupName);
private:
	CMFCPropertyGridCtrl			m_wndPropList;
	CReflectiveUI					m_UI;
	REBox							m_box;
	RECircle						m_circle;

	// for multi objects
	REBox							m_multiBox1;
	REBox							m_multiBox2;

	CReflectivePropertyGridCtrl*	m_pPropCtrl;
	CReflectivePropertyGridCtrl*	m_pPropCtrl2;
	CReflectivePropertyGridCtrl*	m_pPropCtrl3;
};

