#pragma once
#include <string>

// CDlgInputMapName ��ȭ �����Դϴ�.

class CORETOOL_API CDlgInputMapName : public CDialog
{
	DECLARE_DYNAMIC(CDlgInputMapName)

public:
	CDlgInputMapName(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgInputMapName();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_INPUT_MAP_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTestDirectory();
	std::string m_strDlgCaption;
	CString m_cstrTestDirectory;
	CString m_cstrTestFile;
public:
	afx_msg void OnBnClickedOk();
};
