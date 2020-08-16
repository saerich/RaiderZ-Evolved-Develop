#pragma once
#include "VL_ListenableView.h"

// CRollupTerrainAverage ��ȭ �����Դϴ�.
/** @brief Terrain - Average Rollup */
class CORETOOL_API CRollupTerrainAverage : public CDialog, public CListenableViewImpl_Rollup<CRollupTerrainAverage>
{
	DECLARE_DYNAMIC(CRollupTerrainAverage)

public:
						CRollupTerrainAverage(CWnd* pParent = NULL);	///< ǥ�� �������Դϴ�.
	virtual				~CRollupTerrainAverage();						///< �Ҹ���

	// Listenable View
	virtual void		Listen(CoreMessage& _message);				///< Listen
	virtual const char*	GetViewName(){ return "��� �귯��"; }			///< View Name


	// Dialog
	virtual BOOL		OnInitDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VR_TERRAIN_AVER };

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
