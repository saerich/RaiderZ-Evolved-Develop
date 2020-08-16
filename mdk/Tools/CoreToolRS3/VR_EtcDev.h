#pragma once

#include "VL_ListenableView.h"
#include "afxwin.h"

// CRollupEtcDev ��ȭ �����Դϴ�.
/** @brief Development Pilot Rollup */
class CORETOOL_API CRollupEtcDev : public CDialog, public CListenableViewImpl_Rollup<CRollupEtcDev>
{
	DECLARE_DYNAMIC(CRollupEtcDev)

public:
						CRollupEtcDev(CWnd* pParent = NULL);	///< ǥ�� �������Դϴ�.
	virtual				~CRollupEtcDev();						///< �Ҹ���

	// Listenable View
	virtual void		Listen(CoreMessage& _message);		///< Listen
	virtual const char*	GetViewName(){ return "Development"; }	///< View Name

	virtual BOOL		OnInitDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VR_ETC_DEV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	virtual void OnCancel(){}
	virtual void OnOK(){}

	DECLARE_MESSAGE_MAP()
};
