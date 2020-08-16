#pragma once

#include "VL_ListenableView.h"
#include "afxcmn.h"
#include "afxwin.h"

#include <string>
#include <vector>

#include "WS_CutSceneXCutSceneWrapper.h"
#include "WS_EffectManager.h"

class CDlgCutSceneEditor;

class CCutSceneObjectListBox : public CListBox
{
public:
	CXCutSceneWrapper *			m_pCutSceneWrapper;

protected:
	COLORREF					GetDataStateColor(int nIndex);

public:
	CCutSceneObjectListBox():CListBox(), m_pCutSceneWrapper(NULL)	{}

	virtual void				DrawItem(LPDRAWITEMSTRUCT lpDIS);
};

// CRollupCutSceneObjectList ��ȭ �����Դϴ�.

class CRollupCutSceneObjectList : public CDialog, public CListenableViewImpl_Rollup<CRollupCutSceneObjectList>
{
	DECLARE_DYNAMIC(CRollupCutSceneObjectList)

public:
								CRollupCutSceneObjectList(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual						~CRollupCutSceneObjectList();

	// Listenable View
	virtual void				Listen(CoreMessage& _message);				///< Listen
	virtual const char*			GetViewName(){ return "CutScene Object List"; }	///< View Name

	void						ResetUI(XCutSceneContent* pContent);
	bool						SaveToXMLCurrentContent(const char* _pSzFileName);
	void						CloseCutSceneEditor();

	void						RunCutScene();
	bool						IsRunningCutScene(){ return m_bCurrentPreviewRunning; }

	void						ChangeObjectSelection(int nObjIdx, bool bTriggerShow, int nSetCurList);
	void						ChangeCurrentObjectListName(const char* _pSzNewName);

	WS_EffectManager*			GetEffectManager() { return &m_EffectManager; }

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VR_CUTSCENE_OBJ_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();
	virtual void OnCancel(){}
	virtual void OnOK(){}

	DECLARE_MESSAGE_MAP()


private:
	CXCutSceneWrapper			m_CutSceneWrapper_;
	CDlgCutSceneEditor*			m_pDlgCutSceneEditor_;
	WS_EffectManager			m_EffectManager;

	HTREEITEM					m_hClapperNameItem;
	HTREEITEM					m_hClapperDurationItem;

	bool						m_bCurrentPreviewRunning;
	float						m_fPreviewStartTime;
	float						m_fCustomElapsedTime;

	struct WEATHER_CONTEXT
	{
		int bView;
		int nTime;
		int nWeather;
		std::string strEnvSet;

		BOOL bFog;

	} m_weather;

public:
	CTreeCtrl m_ctlTreeClapper_;
	CCutSceneObjectListBox m_ctlListCutSceneObjects_;
	CButton m_ctlOpenCutSceneEditorOnSelected;

	afx_msg void OnDestroy();
	afx_msg void OnLbnSelchangeListCutsceneObj();
	afx_msg void OnBnClickedButtonCutscenePreview();
	afx_msg void OnLbnDblclkListCutsceneObj();
	afx_msg void OnBnClickedButtonCutsceneActorDel();
	afx_msg void OnBnClickedButtonCutsceneActorNew();
	afx_msg void OnBnClickedButtonOpenCutsceneEditor();
};
