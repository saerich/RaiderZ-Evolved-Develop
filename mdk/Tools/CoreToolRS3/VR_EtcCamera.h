#pragma once

#include "VL_ListenableView.h"
#include "afxwin.h"
#include <string>

// CRollupEtcCamera ��ȭ �����Դϴ�.
/** @brief Camera Editing Rollup */
class CORETOOL_API CRollupEtcCamera : public CDialog, public CListenableViewImpl_Rollup<CRollupEtcCamera>
{
	DECLARE_DYNAMIC(CRollupEtcCamera)

public:
						CRollupEtcCamera(CWnd* pParent = NULL);	///< ǥ�� �������Դϴ�.
	virtual				~CRollupEtcCamera();					///< �Ҹ���

	// Listenable View
	virtual void		Listen(CoreMessage& _message);		///< Listen
	virtual const char*	GetViewName(){ return "Camera Animation"; }	///< View Name

	void				UpdateAndRenderAddedCamera();			///< ���� ���� ������ ī�޶� ����� ��ũ��ġ�� ������

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VR_ETC_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	virtual void OnCancel(){}
	virtual void OnOK(){}

	DECLARE_MESSAGE_MAP()

private:
	CListBox	m_ctlCamListBox;
	int			m_nCameraCount;

	CButton		m_ctlViewAddedCamera;
	CButton		m_ctlCheckAniApplyToCam;
	CButton		m_ctlCheckToOrg;

	std::string	m_strCameraAnimationFile;

public:
	afx_msg void OnBnClickedButtonAddCamera();
	afx_msg void OnBnClickedButtonApplyCamAni();
	afx_msg void OnBnClickedButtonPlayCamAni();
	afx_msg void OnBnClickedButtonStopCamAni();
	afx_msg void OnBnClickedButtonAddedCamDelete();
};
