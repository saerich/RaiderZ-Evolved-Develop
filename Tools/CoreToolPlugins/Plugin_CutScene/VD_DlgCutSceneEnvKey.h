#pragma once

#include "afxwin.h"

#include <string>


// CDlgCutSceneEnvKey ��ȭ �����Դϴ�.

class CControlMediator;
class CDlgCutSceneEnvKey : public CDialog
{
	DECLARE_DYNAMIC(CDlgCutSceneEnvKey)

public:
	CDlgCutSceneEnvKey(CControlMediator* _pControlMediator, float fTime, int nEnvTime, int nEnvWeather, const char* pSzSetName, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgCutSceneEnvKey();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CUTSCENE_ENVKEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CControlMediator* m_pControlMediator_;

public:
	float m_fTime;
	int m_nEnvTime;
	int m_nEnvWeather;
	std::string m_strEnvSetName;

	CComboBox m_ctlEnvSet;
	CComboBox m_ctlEnvTimes;
	CComboBox m_ctlEnvWeathers;

	void GetEnvKeyInfo(float& fTime, int& nEnvTime, int& nEnvWeather, std::string& rEnvSetName);
	afx_msg void OnDestroy();
};
