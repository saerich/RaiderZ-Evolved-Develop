#pragma once

#include "VL_ListenableView.h"
#include "afxwin.h"
//#include "RDecalSceneNode.h"

namespace rs3{ class RDecalSceneNode;; };

// CRollupEtcDecal ��ȭ �����Դϴ�.
/** @brief Decal Pilot Rollup */
enum DECAL_TYPE
{
	DECAL_TYPE_PROJECTION = 0,
	DECAL_TYPE_POLYGON
};

class CORETOOL_API CRollupEtcDecal : public CDialog, public CListenableViewImpl_Rollup<CRollupEtcDecal>
{
	DECLARE_DYNAMIC(CRollupEtcDecal)

public:
						CRollupEtcDecal(CWnd* pParent = NULL);	///< ǥ�� �������Դϴ�.
	virtual				~CRollupEtcDecal();						///< �Ҹ���

	// Listenable View
	virtual void		Listen(CoreMessage& _message);		///< Listen
	virtual const char*	GetViewName(){ return "Decal Test"; }	///< View Name

	virtual BOOL		OnInitDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VR_ETC_DECAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	virtual void OnCancel(){}
	virtual void OnOK(){}

	DECLARE_MESSAGE_MAP()

public:
	bool			m_bCreate;	// 1 : ���� �����ΰ�, 0 : ���� �����ΰ�
	DECAL_TYPE		m_eDecalType;

	float 			m_fDecalSize_X;
	float 			m_fDecalSize_Y;
	float 			m_fDecalSize_Z;

	float			m_fDecalRotate_Z;

	CString			m_strTextureFileName;

	afx_msg void OnBnClickedDecalCreate_Projection();
	afx_msg void OnBnClickedDecalCreate_Polygon();
	afx_msg void OnBnClickedDecalClear();	// ����

	afx_msg void OnChangeDecalSizeX();
	afx_msg void OnChangeDecalSizeY();
	afx_msg void OnChangeDecalSizeZ();
	afx_msg void OnChangeDecalRotateZ();

	afx_msg void OnBnClickedTextureOpen();
};
