#pragma once


// CDlgCutSceneDOFKey ��ȭ �����Դϴ�.

class CDlgCutSceneDOFKey : public CDialog
{
	DECLARE_DYNAMIC(CDlgCutSceneDOFKey)

public:
	CDlgCutSceneDOFKey(float _fTime, float _fFocusDist, float _fFocusRange, float _fCocSize, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgCutSceneDOFKey();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CUTSCENE_DOFKEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	float m_fTime;
	float m_fFocusDist;
	float m_fFocusRange;
	float m_fCocSize;
};
