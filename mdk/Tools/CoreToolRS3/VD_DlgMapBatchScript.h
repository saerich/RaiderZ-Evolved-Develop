#pragma once

#include <map>
#include <vector>
#include <string>
#include "ext_lib/DIB.h"

// CDlgMapBatchScript ��ȭ �����Դϴ�.

class CDlgMapBatchScript : public CDialog
{
	DECLARE_DYNAMIC(CDlgMapBatchScript)

public:
	CDlgMapBatchScript(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMapBatchScript();

	std::vector<std::string> m_vecSelectedMap;
	std::string m_strSelectedScript;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MAP_BATCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	BOOL OnInitDialog();

	afx_msg void OnTcnSelchangeTabMapFileList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCustomListDraw( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg void OnLvnItemchangedMapFileList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSelectAll();

	DECLARE_MESSAGE_MAP()

private:

	enum E_TYPE {
		ZONE_TAB = 0,
		FIELD_TAB
	};

	CTabCtrl m_ctlMapTab_;
	CListCtrl m_ctlMapFileList_;

	CStatic	m_contentLabel_;
	CFont	m_contentFont_;
	CDib	m_miniMapDib_;

	CComboBox m_ctlScriptCombo_;

	void UpdateMapFileList_(int _nIndex);
	void UpdateScriptComboBox_(int _nIndex);
	void PaintPreviewBkgAndPutRect_(CRect& _rect);

	typedef std::map<int, std::pair<std::string, std::string> > MAP_INDEX_STRING;
	std::map<int, std::pair<std::string, std::string> > m_mapFileNameIfExist_;
public:
	afx_msg void OnBnClickedOk();
};
