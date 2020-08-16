

#pragma once

#include "resource.h"
#include "CDDBDialog.h"
#include "CDDBAniBmp.h"
#include "explorer.h"
#include "ZUpdate.h"
#include "ZFileTransfer.h"
#include <string>
#include <vector>
using namespace std;



// ������ �޽���
#define	WM_FIND_PATCHINFO				(WM_USER + 1)
#define	WM_UPDATE_COMPLETE				(WM_USER + 2)
#define	WM_UPDATE_ERROR					(WM_USER + 3)


// ���� ���� ID ����
enum STATUSID
{
	STATUS_NONE			= 0,
	STATUS_READY,
	STATUS_ANALYSE_PATCHINFO,
	STATUS_CANNOT_FIND_PATCHINFO,
	STATUS_FIND_NEW_FILE,
	STATUS_UPDATING,
	STATUS_UPDATE_FAIL,
	STATUS_UPDATE_COMPLETE,
	STATUS_STARTGAME,
};


// Ÿ�̸� ID ����
enum TIMERID
{
	TIMER_UPDATE		= 0,
};

// Server info
struct _SERVER
{
	string	strIP;
	bool	bActive;

	_SERVER( const string& ip, bool active) : strIP( ip), bActive( active)	{}
};

//class PatchInfoParser;

/////////////////////////////////////////////////////////////////////////////
// Dialog
class CNewAppDlg : public CDDBDialog
{
// Variables
protected:
	void InitStringTable();
	void CheckOSVistaAndMoreVersion();
	void AdjustUISizeAndPosition();

	// ���μ��� ���� ����
	bool			m_bReservedQuit;
	
	// ���� ���μ��� ����
	char			m_szClientVersion[ 25];

	// ���� ����
	char			m_szLauncherVersion[ 25];

	// ������ ����
	int				m_nMajorVersion;

	// Alpha ������� Ȯ��
	bool			m_bIsAlpha;

	// �������� �ε��� �������� Ȯ��
	bool			m_bNavigationComplete;

	// Controls
	class CConfigedExplorer : public CExplorer
	{
	public:
		CWnd*	SetFocus();
		void	Navigate(LPCTSTR URL, VARIANT * Flags, VARIANT * TargetFrameName, VARIANT * PostData, VARIANT * Headers);
	};
	CConfigedExplorer	m_webPage;
	
	CDDBAniBmp		m_cAnimation;				// Animation bitmap control

	// Server list
	CImageList		m_ServerIconSmall;
	CImageList		m_ServerIconLarge;
	vector<_SERVER>	m_ServerList;

	// Command
	CString			m_strCommand;

public:
	// ���� ����Ʈ ������Ʈ�� �������� Ȯ��
	bool			m_bUpdateServerList;

	// ���� ���μ��� ����
	int m_nClientVersion;
	// ���� ����
	int m_nLauncherVersion;

	// ���� ������Ʈ�� �������� Ȯ��
	bool			m_bUpdateComplete;

	//{{AFX_DATA(CNewAppDlg)
	CListCtrl		m_cServerList;
	CEdit			m_cID;
	CEdit			m_cPassword;
	CDDBButton		m_cExtend;
	CDDBButton		m_cStartGame;
	CDDBButton		m_cStop;
	//}}AFX_DATA

	// Process status
	int				m_nStatus;

	// Updater
	ZUpdate			m_Update;

	// Update canceled
	bool			m_bUpdateCanceled;

	// thread
	CWinThread*		m_pThreadUpdate;
	bool			m_bThreadContinue;

// Functions
public:
	CNewAppDlg(CWnd* pParent = NULL);

	// update start UI
	void UpdateStartUI();

	// update web loading
	void UpdateWebLoading();

	// update server list
	void UpdateServerList();

	// update info window
	void UpdateInfo();

	// destroy dialog
	void OnDestroyDialog();

	// start game
	void StartGame();

	// get run information
	void GetRunInfo();

	// run external updater
	void RunExternalUpdater();

	// get file verseion
	void GetFileVersion( const char* pszFileName, char* pszVersion);

	// get OS version
	int GetOSVersion( char* pszBuff);

	// complete explorer
	void DocumentCompleteExplorer(LPDISPATCH pDisp, VARIANT* URL);

	// ���̼����� ������
	bool ShowLicenseDlg();

	protected:
		bool IsEnableStart();

		void SetExtend( int nNum =-1);

		bool GetHttpPage( const char* pszURL, string* pstrSource);
		bool ParseServerList( const string& strPage);

		static string ConvUTF8ToAnsi( const char* pszString);
		static string TransCode( const wchar_t *pwszString, int nCodePage);
		static string TransCode ( const char *pszString, int nPrevCodePage, int nCodePage);

protected:
	//{{AFX_VIRTUAL(CNewAppDlg)
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CNewAppDlg)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnExtend();
	afx_msg void OnBnClickedStartgame();
	afx_msg void OnBnClickedStop();
	afx_msg LRESULT OnFindPatchInfo( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateComplete( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateError( WPARAM wParam, LPARAM lParam);
	afx_msg void OnAbout();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnQuit();
	afx_msg void OnNMCustomdrawList( NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	DECLARE_EVENTSINK_MAP()
};