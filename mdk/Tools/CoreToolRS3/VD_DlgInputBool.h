#pragma once


// CDlgInputBool ��ȭ �����Դϴ�.

class CORETOOL_API CDlgInputBool : public CDialog
{
	DECLARE_DYNAMIC(CDlgInputBool)

public:
	CDlgInputBool(const char* _pSzWindowName, int _nTrueChecked, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgInputBool();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_INPUT_BOOL };
	int m_nBoolValue;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CString m_cstrWindowName_;
};
