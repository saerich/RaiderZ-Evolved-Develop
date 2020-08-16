#pragma once

#include "RTypes.h"
// CDlgInputTransform ��ȭ �����Դϴ�.

class CORETOOL_API CDlgInputTransform : public CDialog
{
	DECLARE_DYNAMIC(CDlgInputTransform)

public:
	CDlgInputTransform(const char* _pObjectName, const rs3::RVector3* _pPos, const rs3::RVector3* _pDir, const rs3::RVector3* _pUp, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgInputTransform();

	rs3::RVector3 GetPos(){ return rs3::RVector3(m_fPos_); }
	rs3::RVector3 GetDir(){ return rs3::RVector3(m_fDir_); }
	rs3::RVector3 GetUp(){ return rs3::RVector3(m_fUp_); }

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_INPUT_TRANSFORM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

private:
	float m_fPos_[3];
	float m_fDir_[3];
	float m_fUp_[3];

	bool m_bUsingPos_;
	bool m_bUsingDir_;
	bool m_bUsingUp_;

	CString m_cstrWindowName_;
};
