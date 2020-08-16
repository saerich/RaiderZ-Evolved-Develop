#pragma once


// CDlgCutSceneNewMap ��ȭ �����Դϴ�.

class CORETOOL_API CDlgCutSceneNewMap : public CDialog
{
	DECLARE_DYNAMIC(CDlgCutSceneNewMap)

public:
	CDlgCutSceneNewMap(CControlMediator* pControlMediator, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgCutSceneNewMap();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CUTSCENE_NEWMAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	CControlMediator* m_pControlMediator_;

public:
	CString		m_strFileName;
	CString		m_strName;

	int			m_nFieldId;
	float		m_fDuration;

	bool		MakeNewCutSceneFile();

	afx_msg void OnBnClickedButtonCutsceneFieldOpen();
};
