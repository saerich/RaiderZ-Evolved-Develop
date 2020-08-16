#pragma once

#include "VL_ListenableView.h"
#include "afxwin.h"

// CRollupEntityMaterial ��ȭ �����Դϴ�.
/** @brief Entity Material Rollup */
class CRollupEntityMaterial : public CDialog, public CListenableViewImpl_Rollup<CRollupEntityMaterial>
{
	DECLARE_DYNAMIC(CRollupEntityMaterial)

public:
							CRollupEntityMaterial(CWnd* pParent = NULL);	///< ǥ�� �������Դϴ�.
	virtual					~CRollupEntityMaterial();						///< �Ҹ���

	// Listenable View
	virtual void			Listen(CoreMessage& _message);				///< Listen
	virtual const char*		GetViewName(){ return "Entity Material"; }		///< View Name

	// CDialog
	virtual BOOL			OnInitDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VR_ENTITY_MATERIAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	virtual void OnCancel(){}
	virtual void OnOK(){}

	DECLARE_MESSAGE_MAP()
};
