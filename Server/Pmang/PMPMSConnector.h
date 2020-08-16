#pragma once

#include "PmLib.h"
#include "MTstring.h"

interface IPMSObject;
class PmRegionStat;

class PMANG_API PMPMSConnector
{
protected:
	class TokenPszConverter
	{
	public:
		TokenPszConverter(const wstring strCommandLine);
		~TokenPszConverter();

		DWORD Argc() const;
		LPSTR* Argv() const;
	private:
		vector<wstring> m_vecTokens;
		LPSTR* pszTokens;
	};

public:
	PMPMSConnector();
	~PMPMSConnector();

	// �������� ����
	void SetGameInfo(DWORD dwSSN, DWORD dwCategory);
	// PMS ����, ��ȯ���� PMSCONN_ERRORCODE ����
	DWORD	 Connect( DWORD argc, LPSTR argv[], IPMSObject* pCallbackObj );
	// PMS ����, ��ȯ���� PMSCONN_ERRORCODE ����
	DWORD	Connect( const wstring strCommandLine );
	// PMS �������
	void	Stop();
	// PMS�� Warning �޽����� ������
	BOOL	SendWarningMsg(DWORD dwErrLvl, LPCTSTR pszWarningMsg, LPCTSTR pszTreatMsg) const;
	// PMS�� Warning �޽����� ������
	void	SendWarningMsg() const;
	// PMS�� �Ѱ��� ���� ���� Config ������ �̸� ��ȯ
	LPCSTR	GetConfigFileName();
	// PMSConn�� ���� ���¸� ���� ���� ���� �Լ�, ��ȯ���� PMSCONN_STATUS_CODE ����
	DWORD	GetStatus();
	// ���� ��ȣ ��ȯ
	DWORD	GetSSN() const;
	// ���� ī�װ� ��ȯ
	DWORD	GetCategory() const;
	// ���� ���� ��ȯ
	PmRegionStat* GetRegionStat() const;
protected:
	// PMS �ݹ� ������Ʈ ���ϱ�
	virtual IPMSObject* GetPMSObject() = 0;
private:
	// ���� �ݹ�
	void OnConnected();
	// ������� �ݹ�
	void OnStopped();
private:
	bool		m_bConnected;
	DWORD		m_dwSSN;
	DWORD		m_dwCategory;
	PmRegionStat* m_pRegionStat;
};
