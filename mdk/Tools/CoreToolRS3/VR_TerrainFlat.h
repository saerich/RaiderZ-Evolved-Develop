#pragma once
#include "VL_ListenableView.h"

// CRollupTerrainFlat ��ȭ �����Դϴ�.
/** @brief Terrain - Flat Rollup */
class CORETOOL_API CRollupTerrainFlat : public CDialog, public CListenableViewImpl_Rollup<CRollupTerrainFlat>
{
	DECLARE_DYNAMIC(CRollupTerrainFlat)

public:
							CRollupTerrainFlat(CWnd* pParent = NULL);	///< ǥ�� �������Դϴ�.
	virtual					~CRollupTerrainFlat();						///< �Ҹ���

	// Listenable View
	virtual void			Listen(CoreMessage& _message);			///< Listen
	virtual const char*		GetViewName(){ return "�����ϰ�"; }			///< View Name

	// Dialog
	virtual BOOL			OnInitDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VR_TERRAIN_FLAT };

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
