#pragma once

class CControlMediator;

// CDlgNewMiniMap ��ȭ �����Դϴ�.

class CDlgNewMiniMap : public CDialog
{
	DECLARE_DYNAMIC(CDlgNewMiniMap)

public:
	CDlgNewMiniMap(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgNewMiniMap();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_NEW_MINI_MAP };
	bool CreateMiniMap(CControlMediator* _pControlMediator);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
