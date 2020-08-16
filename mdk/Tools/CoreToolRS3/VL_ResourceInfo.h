#pragma once

#include <map>
#include <string>
#include "ext_lib/DIB.h"
#include "ext_lib/TreeCtrlEx.h"
#include "VL_ListenableView.h"

#include "resource.h"

// CDlgResourceInfo ��ȭ �����Դϴ�.
class CControlMediator;
/** @brief Resource Information Dialog */
class CORETOOL_API CDlgResourceInfo : public CDialog, public CListenableViewImpl_Dialog<CDlgResourceInfo>
{
	DECLARE_DYNAMIC(CDlgResourceInfo)

public:
	enum MAP_RES_TYPE {
		RES_ALL = 0,
		RES_ZONE,
		RES_FIELD,
		RES_NPC,
		RES_TILE,
		RES_NUM
	};

							CDlgResourceInfo(CControlMediator* _pControlMediator, MAP_RES_TYPE _type, CWnd* pParent =NULL); ///< Stack ������
							CDlgResourceInfo(CWnd* pParent = NULL);		///< ǥ�� �������Դϴ�.
	virtual					~CDlgResourceInfo();						///< �Ҹ���

	// Listenable View
	virtual void			Listen(CoreMessage& _message);			///< Listen
	virtual const char*		GetViewName(){ return "Resource Info"; }	///< View Name

	// Dialog
	virtual BOOL			OnInitDialog();								///< ������ �� �����ÿ� �ʿ��� font���� ����
	virtual void			OnCancel();									///< ��ҽÿ� ���̾�α� ����
	virtual void			OnOK();										///< �����ֱ⸦ �Ϸ��ϰ� ���̾�α� ����
	
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VL_RESOURCE_INFO };

	afx_msg void OnBnClickedButtonEditQuest();
	void SetResourceType(MAP_RES_TYPE _type);
	std::string m_strSelectedTile;
	int m_nSelectedNpcID;
	int m_nSelectedFieldID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	afx_msg void OnDestroy();
	afx_msg void OnTvnSelchangedTreeMapRes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTreeMapRes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonOpenMap();
	afx_msg void OnBnClickedButtonRunMap();
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
	CTreeCtrlEx m_ctrlExTreeMapRes_;

	struct MAP_RES_INFO {
		MAP_RES_TYPE m_eResType;
		std::string m_strNameToEdit; //������ ���� �̸�
		std::string m_strNameToView; //view �������� ������ �����̸�
		int m_nID;
	};
	MAP_RES_INFO* m_pCurrentResInfo_;

	//Update TreeView, SetResourceType�ÿ� �Ҹ���.
	void UpdateTreeWithZoneList_();
	void UpdateTreeWithFieldList_();
	void UpdateTreeWithNpcList_();
	void UpdateTreeWithTileList_();
	void ResetTreeCtrl_();

	void PaintPreviewBkgAndPutRect_(CRect& _rect);
	bool DrawTileResource_();
	bool DrawMiniMap_(CRect& _refRect);
	bool DrawNpcResource_(CRect& _refRect);
	void DrawTextMessage_(const char* _pSzMessage, bool _bClearResource);
	CDib	m_miniMapDib_;

	//for modal / modaless
	bool m_bModal_;
	MAP_RES_TYPE m_eResType_;
	
};
