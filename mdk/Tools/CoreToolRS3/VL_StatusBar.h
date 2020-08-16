/// @file VL_StatusBar.h
#pragma once

#include "VL_ListenableView.h"
#include "ext_lib/InitDialogBar.h"
#include "resource.h"
#include "V_MFCAnchorSupport.h"
#include "V_EditCtrl.h"

/**
	@brief StatusBar
	����ǥ����
*/

class CORETOOL_API CCoreToolStatusBar : public CMFCStatusBar, public CListenableViewImpl_Fixed<CCoreToolStatusBar>
{
	DECLARE_DYNAMIC(CCoreToolStatusBar)

public:
							CCoreToolStatusBar(CWnd* pParent = NULL);		///< ǥ�� �������Դϴ�.
	virtual					~CCoreToolStatusBar();						///< �Ҹ���

	BOOL					Create(CWnd* pWnd);
	// Listenable View
	virtual void			Listen(CoreMessage& _message);			///< Listen
	virtual const char*		GetViewName(){ return "StatusBar"; }			///< View Name

	void					SetPaneText(LPCTSTR lpszNewText);

	enum { IDD = 0 };		// dummy for compile, TODO: ����

protected:
	virtual void	OnUpdateCmdUI(CCmdUI* pCmdUI); ///< ������ ��Ʈ���� Ȱ��ȭ �ϱ����� �Լ�
	virtual BOOL	OnInitStatusBar(); ///< Self-Drawing Button ����� �̿��ϱ� ���� �����ÿ� ȣ���ϴ� �޼ҵ�

	void			OnSelectionChanged();	///< ������ ����Ǿ�����
	void			OnEnterPosEditCtrl();	///< ��ġ ��Ʈ�ѿ� enter �Է�
	void			OnEnterScaleEditCtrl();	///< ������ ��Ʈ�ѿ� enter �Է�

	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	CCoreToolEdit			m_editControls[4];	// pos x,y,z, scale
};