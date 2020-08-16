#pragma once


// CDlgInputEnvSetImport ��ȭ �����Դϴ�.

class CORETOOL_API CDlgInputEnvSetImport : public CDialog
{
	DECLARE_DYNAMIC(CDlgInputEnvSetImport)

public:
	CDlgInputEnvSetImport(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CDlgInputEnvSetImport(int _nTime, int _nWeather);   // ǥ�� �������Դϴ�.
	virtual ~CDlgInputEnvSetImport();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_INPUT_ENV_IMPORT };

	int m_nTime;
	int m_nWeather;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	afx_msg void OnCbnSelchangeComboTimesImport();
	afx_msg void OnCbnSelchangeComboWeathersImport();

	DECLARE_MESSAGE_MAP()

private:
	CComboBox m_ctlTimesToImport_;
	CComboBox m_ctlWeathersToImport_;
};
