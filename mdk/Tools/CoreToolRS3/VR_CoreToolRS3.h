#pragma once

#include "VL_ListenableView.h"
#include "resource.h"

// CRollupCoreToolRS3 ��ȭ �����Դϴ�.

class CORETOOL_API CRollupCoreToolRS3 : public CDialog, public CListenableViewImpl_Rollup<CRollupCoreToolRS3>
{
	DECLARE_DYNAMIC(CRollupCoreToolRS3)

public:
	CRollupCoreToolRS3(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CRollupCoreToolRS3();

	virtual void Listen(CoreMessage& _message){}
	virtual const char* GetViewName(){ return "Hello CoreToolRS3"; }

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VR_CORE_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	virtual void OnCancel(){}
	virtual void OnOK(){}

	DECLARE_MESSAGE_MAP()
};
