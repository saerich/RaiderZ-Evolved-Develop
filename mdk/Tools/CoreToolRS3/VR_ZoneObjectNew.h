#pragma once

#include "VL_ListenableView.h"
#include <string>
#include <vector>
#include "afxcmn.h"

#include "resource.h"

// CRollupZoneObjectNew ��ȭ �����Դϴ�.
/** @brief Zone Object New Rollup */
class CORETOOL_API CRollupZoneObjectNew : public CDialog, public CListenableViewImpl_Rollup<CRollupZoneObjectNew>
{
	DECLARE_DYNAMIC(CRollupZoneObjectNew)

public:
							CRollupZoneObjectNew(CWnd* pParent = NULL);		///< ǥ�� �������Դϴ�.
	virtual					~CRollupZoneObjectNew();						///< �Ҹ���

	// Listenable View
	virtual void			Listen(CoreMessage& _message);				///< Listen
	virtual const char*		GetViewName(){ return "���ο� ������Ʈ"; }		///< View Name

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VR_ZONE_OBJECT_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnCancel();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTcnSelchangeTabFile(NMHDR *pNMHDR, LRESULT *pResult);
	// control
	CTabCtrl m_ctlFileTab;

	class CTreeCtrlNewObject : public CTreeCtrl
	{
	public:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		CRollupZoneObjectNew* m_pObjectNewWindow;
		CPoint m_margin;
	} m_ctrlTreeNewList_;

	// ���� ���õ� ����/Ÿ��
	std::string m_strFullPathName;
	std::string m_strCurrentFile;
	std::string m_strCurrentType;
	MXmlElement *m_pCurrentNodeInfo;

private:
	enum E_CREATE_TYPE
	{
		AS_INSTANCE = 0,
		AS_COPY = 1
	};

	enum E_NODE_TYPE
	{
		SCENE = 0,
		EFFECT,
		TREE,
		ETC,

		TOTAL_NODETYPE
	};
	static const int E_TOTAL_FILE_NODE = ETC;

	// ���� ���
	std::vector<std::string> m_arrayFile[E_TOTAL_FILE_NODE];
	std::string m_initial[E_TOTAL_FILE_NODE];

	// ���� �����̸��� �ٷ������ ���
	// ���� ������Ʈ�� �� ������ �����ϸ�, �����̸� ��Ģ�� �ִ�.
	std::string m_strPreConcWaterFileName_;
	int m_nSizePreConcForWater_;

	// ���� ���õ� ����Ÿ��
	int m_nCreateSceneType;

	HTREEITEM AddToTreeItem(std::map<std::string, HTREEITEM >& mapFile, string& strFullName);
	void UpdateFileList_();
	void UpdateFileListCtrl_(int nIndex);
	afx_msg void OnTvnSelchangedTreeNewList(NMHDR *pNMHDR, LRESULT *pResult);

public:
	void NoneSelect();
	void SelectNewObjectListTab( int nIndex );
	bool IsCreatingAsInstance(){ return m_nCreateSceneType == AS_INSTANCE; }
	void NotifyToNewAction();	///< ������ ����Ǿ����� �˸���

public:
	afx_msg void OnBnClickedButtonEtcWaterPlaneCreate();
	afx_msg void OnBnClickedRadioAsInstance();
	afx_msg void OnBnClickedRadioAsCopy();
};
