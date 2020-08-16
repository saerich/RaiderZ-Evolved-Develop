#pragma once
#include "afxcmn.h"
#include "afxwin.h"


namespace rs3 
{
	class RToolTerrain;
};
// CRollupTerrainUserMaterial ��ȭ �����Դϴ�.
/** @brief Terrain - User Material */
class CORETOOL_API CRollupTerrainUserMaterial : public CDialog, public CListenableViewImpl_Rollup<CRollupTerrainUserMaterial>
{
	DECLARE_DYNAMIC(CRollupTerrainUserMaterial)

public:
							CRollupTerrainUserMaterial(CWnd* pParent = NULL);	///< ǥ�� �������Դϴ�.
	virtual					~CRollupTerrainUserMaterial();						///< �Ҹ���

	// Listenable View
	virtual void			Listen(CoreMessage& _message);					///< Listen
	virtual const char*		GetViewName(){ return "User Material"; }			///< View Name

	// Dialog
	virtual BOOL			OnInitDialog();


// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VR_TERRAIN_USERMATERIAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	virtual void OnCancel(){}
	virtual void OnOK(){}

	DECLARE_MESSAGE_MAP()
public:
	float m_fEditSize;
	rs3::RToolTerrain *m_pTerrain_;
	
	CSliderCtrl m_sliderCtrlSize;
	CString m_cstrViewSize;
	DWORD m_dwEditID;
	CButton m_btnView;

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedCheckView();
	afx_msg void OnEnChangeEditUsermaterialId();
};
