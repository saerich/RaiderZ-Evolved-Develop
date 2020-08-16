#pragma once

#include "VL_ListenableView.h"

namespace rs3{ class RToolTerrain; };
// CDlgRelaxAll ��ȭ �����Դϴ�.
/** @brief Relax All Dialog */
class CDlgRelaxAll : public CDialog, public CListenableViewImpl_Dialog<CDlgRelaxAll>
{
	DECLARE_DYNAMIC(CDlgRelaxAll)

public:
							CDlgRelaxAll(CWnd* pParent = NULL);		///< ǥ�� �������Դϴ�.
	virtual					~CDlgRelaxAll();						///< �Ҹ���

	// Listenable View
	virtual void			Listen(CoreMessage& _message);		///< Listen
	virtual const char*		GetViewName(){ return "Relax All"; }	///< View Name

	// Dialog
	virtual BOOL			OnInitDialog();
	virtual void			OnCancel();
	virtual void			OnOK();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VL_TERRAIN_RELAX_ALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
private:
	int m_nRelaxFactor_;
	rs3::RToolTerrain* m_pTerrain_;
};
