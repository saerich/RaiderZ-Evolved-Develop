#pragma once


class CControlMediator;
// CDlgCutSceneActorType ��ȭ �����Դϴ�.

class CDlgCutSceneActorType : public CDialog
{
	DECLARE_DYNAMIC(CDlgCutSceneActorType)

public:
	CDlgCutSceneActorType(CControlMediator* pControlMediator, const char* _pSzActorType, const char* _pSzActorName, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgCutSceneActorType();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CUTSCENE_ACTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	CControlMediator* m_pControlMediator_;

public:
	int m_nActorType;
	int m_nNPCID;
	CString m_strNpcName;

	void SetEnablePlayer();
	void SetEnableNPC();
	afx_msg void OnBnClickedRadioCutscenePlayer();
	afx_msg void OnBnClickedRadioCutsceneNpc();
	afx_msg void OnBnClickedButtonCutsceneNpcSelect();
};
