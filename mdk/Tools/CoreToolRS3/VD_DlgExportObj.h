#pragma once
#include "afxwin.h"


// CDlgExportObj ��ȭ �����Դϴ�.
class CDlgExportObj : public CDialog
{
	DECLARE_DYNAMIC(CDlgExportObj)

public:
	CDlgExportObj(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgExportObj();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_OBJ_EXPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_nLODLevel;
	int m_nWPart;
	int m_nHPart;
};
