#pragma once
#include "VL_ListenableView.h"

// CRollupTerrainHeight ��ȭ �����Դϴ�.
/** @brief Terrain - Height Rollup */
class CORETOOL_API CRollupTerrainHeight : public CDialog, public CListenableViewImpl_Rollup<CRollupTerrainHeight>
{
	DECLARE_DYNAMIC(CRollupTerrainHeight)

public:
							CRollupTerrainHeight(CWnd* pParent = NULL);	///< ǥ�� �������Դϴ�.
	virtual					~CRollupTerrainHeight();					///< �Ҹ���

	// Listenable View
	virtual void			Listen(CoreMessage& _message);			///< Listen
	virtual const char*		GetViewName(){ return "���� ����"; }		///< View Name

	// Dialog
	virtual BOOL			OnInitDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VR_TERRAIN_HEIGHT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnCancel();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	float m_fEditSize;
	CString m_cstrViewSize;
	CSliderCtrl m_sliderCtrlSize;
	float m_fStr;
	CSliderCtrl m_sliderCtrlStr;
};
