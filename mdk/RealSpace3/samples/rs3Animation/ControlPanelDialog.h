#pragma once

#include "resource.h"
// ControlPanelDialog ��ȭ �����Դϴ�.

class ControlPanelDialog : public CDialog
{
	DECLARE_DYNAMIC(ControlPanelDialog)

public:
	ControlPanelDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~ControlPanelDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
