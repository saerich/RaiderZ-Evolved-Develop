#pragma once

#include "VL_ListenableView.h"

#include "ext_lib/CNumEdit/NumSpinCtrl.h"
#include "ext_lib/CNumEdit/NumEdit.h"
#include "ext_lib/DIB.h"

#include "afxwin.h"
#include "resource.h"

// CEditEtcMapView ��ȭ �����Դϴ�.
/** @brief Map View Rollup */
class CORETOOL_API CRollupEtcMapView : public CDialog, public CListenableViewImpl_Rollup<CRollupEtcMapView>
{
	DECLARE_DYNAMIC(CRollupEtcMapView)

public:
						CRollupEtcMapView(CWnd* pParent = NULL);	///< ǥ�� �������Դϴ�.
	virtual				~CRollupEtcMapView();						///< �Ҹ���

	// Listenable View
	virtual void		Listen(CoreMessage& _message);			///< Listen
	virtual const char*	GetViewName(){ return "Map View"; }			///< View Name

	// CDialog
	virtual BOOL		OnInitDialog();
	LRESULT				WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	///< for draw preview

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VR_ETC_MAP_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	afx_msg void OnEnChangeEditCamSpeed();

	virtual void OnCancel(){}
	virtual void OnOK(){}

	DECLARE_MESSAGE_MAP()

public:
	void UpdateCameraControlInfo();
	void UpdateCameraLocationList();

private:
	CNumSpinCtrl m_ctrlSpinWSAD_;
	CNumSpinCtrl m_ctrlSpinWheel_;
	CNumSpinCtrl m_ctrlSpinFOV_;
	CNumSpinCtrl m_ctrlSpinFarZ_;

	CNumEdit	m_ctrEditWSAD_;
	CNumEdit	m_ctrEditWheel_;
	CNumEdit	m_ctrEditFOV_;
	CNumEdit	m_ctrEditFarZ_;

	CListBox	m_ctlListCamLocation_;
	CDib		m_miniMapDib_;
	bool		m_bTryCreateMinimap_;

public:
	afx_msg void OnBnClickedCheckCenterPivot();
	afx_msg void OnBnClickedButtonCamSave();
	afx_msg void OnBnClickedButtonCamDelete();
	afx_msg void OnLbnDblclkListCamPos();
	afx_msg void OnBnClickedButtonUpdateMinimap();
	afx_msg void OnBnClickedButtonOpenResMgr();
	afx_msg void OnBnClickedCheckViewTerrainInfo();
	afx_msg void OnBnClickedButtonProfile();
	afx_msg void OnEnChangeEditCamFov();
	afx_msg void OnEnChangeEditCamFarZ();

	CButton m_ctlCheckCenterPivot;
	CButton m_ctlVeiwCameraPivot;

	void UpdateViewCheckerButton();
	CButton m_ctlCheckTerrainInfo_;
	afx_msg void OnBnClickedButtonFovToolvalue();
	afx_msg void OnBnClickedButtonFovGamevalue();
	afx_msg void OnBnClickedButtonFarzToolvalue();
	afx_msg void OnBnClickedButtonFarzGamevalue();
	CButton m_ctlCheckKeepCurrentCamera;
	afx_msg void OnBnClicked1st3rdPerson();

	CButton m_ctl1st3rdPerson;
	afx_msg void OnBnClickedButtonTopviewNavi();
};
