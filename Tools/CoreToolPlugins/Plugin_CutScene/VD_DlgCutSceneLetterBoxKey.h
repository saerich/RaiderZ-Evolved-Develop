#pragma once


// CDlgCutSceneLetterBoxKey ��ȭ �����Դϴ�.

class CDlgCutSceneLetterBoxKey : public CDialog
{
public:
	CDlgCutSceneLetterBoxKey(UINT nIDTemplate, CWnd* pParent = NULL) : CDialog(nIDTemplate, pParent), m_fTime(0.0f) {}   // ǥ�� �������Դϴ�.
	virtual ~CDlgCutSceneLetterBoxKey() {}

public:
	float m_fTime;
};

//class CDlgCutSceneLetterBoxStartKey : public CDlgCutSceneLetterBoxKey
//{
//	DECLARE_DYNAMIC(CDlgCutSceneLetterBoxStartKey)
//
//public:
//	CDlgCutSceneLetterBoxStartKey(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
//	virtual ~CDlgCutSceneLetterBoxStartKey();
//
//// ��ȭ ���� �������Դϴ�.
//	enum { IDD = IDD_DIALOG_CUTSCENE_LETTERBOX_START };
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
//
//	DECLARE_MESSAGE_MAP()
//};
//
//class CDlgCutSceneLetterBoxEndKey : public CDlgCutSceneLetterBoxKey
//{
//	DECLARE_DYNAMIC(CDlgCutSceneLetterBoxEndKey)
//
//public:
//	CDlgCutSceneLetterBoxEndKey(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
//	virtual ~CDlgCutSceneLetterBoxEndKey();
//
//	// ��ȭ ���� �������Դϴ�.
//	enum { IDD = IDD_DIALOG_CUTSCENE_LETTERBOX_END };
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
//
//	DECLARE_MESSAGE_MAP()
//};
//
