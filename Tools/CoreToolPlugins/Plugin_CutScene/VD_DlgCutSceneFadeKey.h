#pragma once

#include "MTypes.h"
// CDlgCutSceneFadeKey ��ȭ �����Դϴ�.
class CControlMediator;
class XEffectInfoMgr;

class CDlgCutSceneFadeKey : public CDialog
{
	DECLARE_DYNAMIC(CDlgCutSceneFadeKey)

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CDlgCutSceneFadeKey(CControlMediator* _pControlMediator, float fTime, float fAlpha, CWnd* pParent = NULL);
	virtual ~CDlgCutSceneFadeKey();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CUTSCENE_FADEKEY };

	void GetFadeData( float& fTime, float& fAlpha);
public:
	float m_fTime;
	float m_fAlpha;
};
