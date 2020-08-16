#pragma once

#include "VL_ListenableView.h"
#include "RTypes.h"

#include "afxwin.h"
#include "resource.h"
#include "WS_Field.h"

// CRollupEnvFieldInfo ��ȭ �����Դϴ�.

class CORETOOL_SOULHUNT_API CRollupEnvFieldInfo : public CDialog, public CListenableViewImpl_Rollup<CRollupEnvFieldInfo>
{
	DECLARE_DYNAMIC(CRollupEnvFieldInfo)

public:
	CRollupEnvFieldInfo(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CRollupEnvFieldInfo();

	virtual void		Listen(CoreMessage& _message);				///< Listen
	
	virtual const char*	GetViewName(){ return "�ʵ�����"; }				///< View Name

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VR_ENV_FIELDINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	MRect GetOriginalBoundingBox();
	MRect ObtainFieldSize();
	void UpdateFieldSize();
	void UpdateAggroLimitZ();
	void UpdateFixedCombobox();
	bool CheckValidation();
	CWorkSpaceField::FIELD_INFO& GetFieldInfo();

private:
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonApply();
	CString m_strMinX;
	CString m_strMaxX;
	CString m_strMinY;
	CString m_strMaxY;

	CComboBox	m_FixedTimeCombobox;

public:
	afx_msg void OnBnClickedButtonSetzonesize();
	afx_msg void OnBnClickedButtonAggrozApply();
	float m_fAggroLimitZ;
	afx_msg void OnCbnSelchangeFixedtimeCombo();
};
