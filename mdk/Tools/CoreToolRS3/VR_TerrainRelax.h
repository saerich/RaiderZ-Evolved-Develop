#pragma once

#include "VL_ListenableView.h"

// CRollupTerrainRelax ��ȭ �����Դϴ�.
/** @brief Terrain - Relax Rollup */
class CORETOOL_API CRollupTerrainRelax : public CDialog, public CListenableViewImpl_Rollup<CRollupTerrainRelax>
{
	DECLARE_DYNAMIC(CRollupTerrainRelax)

public:
							CRollupTerrainRelax(CWnd* pParent = NULL);	///< ǥ�� �������Դϴ�.
	virtual					~CRollupTerrainRelax();						///< �Ҹ���

	// Listenable View
	virtual void			Listen(CoreMessage& _message);			///< Listen
	virtual const char*		GetViewName(){ return "Relax"; }			///< View Name

	// Dialog
	virtual BOOL			OnInitDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VR_TERRAIN_RELAX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnCancel();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	CString m_cstrViewSize;
	float m_fEditSize;
	CSliderCtrl m_sliderCtrlSize;
	float m_fStr;
	CSliderCtrl m_sliderCtrlStr;
};
