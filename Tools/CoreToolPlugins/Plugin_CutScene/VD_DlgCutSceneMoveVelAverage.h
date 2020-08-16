#pragma once

#include <vector>

class CXCutSceneWrapper;
// CDlgCutSceneMoveVelAverage ��ȭ �����Դϴ�.

class CDlgCutSceneMoveVelAverage : public CDialog
{
	DECLARE_DYNAMIC(CDlgCutSceneMoveVelAverage)

private:
	CXCutSceneWrapper* m_pCutSceneWrapper;
	int m_nCurrentSelected;

	void SetEnableIntervalAll();
	void SetEnableIntervalPart();

public:
	CDlgCutSceneMoveVelAverage(CXCutSceneWrapper* pCutSceneWrapper, int nCurrentSelected, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgCutSceneMoveVelAverage();

	int m_nIntervalType;
	float m_fIntervalStart;
	float m_fIntervalEnd;

	float m_fAverVel;

	void CalcPart(float& _interval, int& _first, int& _last, const std::vector<float>& _times);

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CUTSCENE_MOVEVEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioCutsceneMoveIntervalAll();
	afx_msg void OnBnClickedRadioCutsceneMoveIntervalPart();
	afx_msg void OnBnClickedButtonCutsceneCalcMoveIntervalVel();
};
