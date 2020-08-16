#pragma once
#include "afxwin.h"

#define MAX_MTRL_TABLE 1000

// CModelInfoDlg ��ȭ �����Դϴ�.

class CModelInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CModelInfoDlg)

public:
	CModelInfoDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CModelInfoDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MODEL_INFO };

	void ClearListBox();
	void DataUpdate();

	void Begin();
	void End();

	void   GetSelectMtrl();
//	RMtrl* GetCurrentSelectMtrl();

	int		m_nSelCount;
//	RMtrl*  m_pMtrlTable[ MAX_MTRL_TABLE ];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ListMeshNode;
	CListBox m_ListMtrlNode;
	afx_msg void OnBnClickedButtonMeshnodeColor();
	afx_msg void OnBnClickedButtonMtrlnodeColor();
	afx_msg void OnBnClickedToonFile();
	CComboBox m_TexFilterCombo;
	CComboBox m_TexBlendCombo;
	CComboBox m_ToonFilterCombo;
	CComboBox m_ToonBlendCombo;
	afx_msg void OnCbnSelchangeComboTexFilter();
	afx_msg void OnCbnSelchangeComboTexBlend();
	afx_msg void OnCbnSelchangeComboToonFilter();
	afx_msg void OnCbnSelchangeComboToonBlend();
	afx_msg void OnLbnSelchangeListMtrlnode();
};
