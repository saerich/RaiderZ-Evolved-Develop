/// @file VD_DlgShortCut.h
#pragma once

// CDlgShortCut ��ȭ �����Դϴ�.
/**
	@brief ����Ű ����

*/
class CDlgShortCut : public CDialog
{

public:
	CDlgShortCut();   // ǥ�� �������Դϴ�.
	virtual ~CDlgShortCut();
	virtual void OnOK();

	virtual BOOL OnInitDialog();	///< �����ÿ� �ʿ��� font���� ����

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SHORTCUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	CString m_strShortcutText;
};
