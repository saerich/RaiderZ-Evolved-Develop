
#include "stdafx.h"
#include "ZGlobal.h"
#include "NewAppDlg.h"
#include "NewAppAboutBox.h"
#include "NewAppLicenseConfirmDlg.h"
#include "NewAppUpdateConfirm.h"
#include "NewAppUpdateCompleteDlg.h"
#include "NewAppErrorDlg.h"
#include "NewAppWarnVista.h"
#include "MXml.h"
#include <math.h>
#include <Mmsystem.h>
#include "ZConfig.h"


#define STR_PROCESS_NAME					"RaiderZ"
#define STR_EXCUTE_FILENAME					"RaiderZ.exe"
#define STR_LAUNCHER_EXCUTE_FILENAME		"Launcher.exe"
#define STR_PATCHINFO_FILENAME				"RaiderZPatchInfo.txt" // RaiderZPatchInfo.json
#define STR_LAUNCHERINFO_FILENAME			"RaiderZLauncher.txt"
#define STR_ADDRESS_ALPHA					"192.168.0.6"
#define STR_DIRECTORY_ALPHA					"RaiderZPatch"
#define STR_SERVERLIST_URL					"http://icecream/soap/login_serverlist_xml.php"

// Common
#define STR_EXT_UPDATER_FILENAME			"updater.exe"


// Thread
UINT ThreadUpdate( LPVOID pParam);


// DoDataExchange
void CNewAppDlg::DoDataExchange( CDataExchange* pDX)
{
	CDDBDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewAppDlg)
	DDX_Control(pDX, IDC_SERVERLIST,	m_cServerList);
	DDX_Control(pDX, IDC_ID,			m_cID);
	DDX_Control(pDX, IDC_PASSWORD,		m_cPassword);
	DDX_Control(pDX, IDC_EXTEND,		m_cExtend);
	DDX_Control(pDX, IDC_STARTGAME, m_cStartGame);
	DDX_Control(pDX, IDC_STOP, m_cStop);
	DDX_Control(pDX, IDC_EXPLORER, m_webPage);
	DDX_Control(pDX, IDC_ANIMATION, m_cAnimation);
	//}}AFX_DATA_MAP
}


// Message map
BEGIN_MESSAGE_MAP( CNewAppDlg, CDDBDialog)
	//{{AFX_MSG_MAP(CNewAppDlg)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_EXTEND, OnBnExtend)
	ON_BN_CLICKED(IDC_STARTGAME, OnBnClickedStartgame)
	ON_BN_CLICKED(IDC_STOP, OnBnClickedStop)
	ON_BN_CLICKED(IDC_ABOUTBOX, OnAbout)
	ON_BN_CLICKED(IDC_QUIT, OnQuit)
	ON_MESSAGE(WM_FIND_PATCHINFO, OnFindPatchInfo)
	ON_MESSAGE(WM_UPDATE_COMPLETE, OnUpdateComplete)
	ON_MESSAGE(WM_UPDATE_ERROR, OnUpdateError)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SERVERLIST, &CNewAppDlg::OnNMCustomdrawList)
	ON_NOTIFY(NM_DBLCLK, IDC_SERVERLIST, &CNewAppDlg::OnNMDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/************************************************************************************
  Dialog
*************************************************************************************/
// Ŭ���� ������
CNewAppDlg::CNewAppDlg( CWnd* pParent) : CDDBDialog( IDD_DIALOG_MAINDLG, pParent)
{
	// �̰����� ������ ���̾�α��� ������ �Է��Ѵ�
	// �� �� �ڼ��� ������ ���ϸ� CDDBDialog.h ȭ���� Structure of CDDBDIALOGINFO ������ �����Ѵ�
	CDDBDLGINFO DlgInfo;
	DlgInfo.nWidth  = 750;
	DlgInfo.nHeight = 541;
	DlgInfo.IDSkinResource = IDR_BMP_MAINDLG;
	DlgInfo.IDIconResource = IDR_ICO_MAINFRAME;
	DlgInfo.IDWaveResource = IDR_WAV_PUSHBUTTON;
	DlgInfo.bEnableMoveDlg = true;
	DlgInfo.bUseAboutboxButton = true;
	DlgInfo.bUseMinimizeButton = true;
	DlgInfo.bUseQuitButton = true;
	DlgInfo.bUseTitlebar = true;
	DlgInfo.bUseBmpTitle = true;
	DlgInfo.nTextColor = RGB( 30, 30, 30);
	DlgInfo.nTextBkColor = RGB( 150, 150, 150);
#ifdef LOCALE_NHNUSA
	DlgInfo.nTextColor = RGB( 150, 150, 150);
	DlgInfo.nTextBkColor = RGB( 38, 38, 38);
#endif
	SetDialogInfo( DlgInfo);


	// ���� �ʱ�ȭ
	m_nStatus = STATUS_NONE;

	m_pThreadUpdate = NULL;
	m_bThreadContinue = false;
	
	m_nMajorVersion = 0;

	m_bUpdateCanceled = false;

	m_bReservedQuit = false;

	m_bIsAlpha = false;
	
	m_bNavigationComplete = false;

	m_bUpdateServerList = false;
	m_bUpdateComplete = false;
}


/************************************************************************************
  Ŭ���� �޼��� �ڵ鷯
*************************************************************************************/
// OnInitDialog : ���̾�α׸� �ʱ�ȭ�� �Լ�
BOOL CNewAppDlg::OnInitDialog() 
{
	CDDBDialog::OnInitDialog();

	// ���� ������ ��´�. Launcher.ini ������ �ε� �� �м��Ѵ�.
	ZConfig::Init();

	// ��Ʈ�� ���̺� �ʱ�ȭ
	InitStringTable();
	
	// �α� ���� �ʱ�ȭ
	InitLog();

	//// ������ ���� Ȯ��
	CheckOSVistaAndMoreVersion();

	AdjustUISizeAndPosition();
	
	// ���� ���� ����
	m_nStatus = STATUS_NONE;

	m_Update.Test();/// TEST�� �ϰ� �ʹٸ�

	// ������ ���� ��ġ ������ �����
	m_Update.DeleteTempDirectory();
	m_Update.DeleteCurDirFile( "CNewAppDlg::OnInitDialog()", STR_PATCHINFO_FILENAME );

	// ���� â�� ������Ʈ �Ѵ�
	UpdateInfo();
	PutLog( "[APP] Window created successfully.");
	
	// ���Ӱ� ��ó�� ���� ���� ����
	m_Update.GetCurrentLauncherAndClientExeVersion( STR_LAUNCHERINFO_FILENAME, m_nLauncherVersion, m_nClientVersion );
	CString strLog;
	strLog.Format("Current : Launcher Ver(%d), Client Ver(%d)", m_nLauncherVersion, m_nClientVersion);
	PutLog(strLog);
		
	// �ܺ� �������Ͱ� ������ �����Ѵ�
	WIN32_FIND_DATA FindData;
	HANDLE hFind = FindFirstFile( STR_EXT_UPDATER_FILENAME, &FindData);
	if ( hFind != INVALID_HANDLE_VALUE) {
		DeleteFile( STR_EXT_UPDATER_FILENAME);
	}

	// ������Ʈ Ÿ�̸� ����
	SetTimer( TIMER_UPDATE, 100, NULL);

	return true;
}


/************************************************************************************
  Ÿ�̸�
*************************************************************************************/
void CNewAppDlg::OnTimer(UINT nIDEvent)
{
	// ������Ʈ Ÿ�̸�
	if ( nIDEvent == TIMER_UPDATE)
	{
		// ���μ��� ���� ���� �������� Ȯ���Ѵ�
		if ( m_bReservedQuit)
		{
			// ���� ���� �˻�
			switch ( m_nStatus)
			{
				case STATUS_NONE :
				case STATUS_READY :
				case STATUS_CANNOT_FIND_PATCHINFO :
				case STATUS_FIND_NEW_FILE :
				case STATUS_UPDATE_FAIL :
				case STATUS_UPDATE_COMPLETE :
				case STATUS_STARTGAME :
					// ���μ��� ����
					OnDestroyDialog();
					break;
			}
		}


		// ���μ��� ���� ���� UI ����
		UpdateStartUI();


		// �������� �ε�
		if (!ZConfig::m_bHideWebPage)
			UpdateWebLoading();

		// ���� ����Ʈ �ε�
		UpdateServerList();

		// InfoBox�� ������Ʈ �Ѵ�
		UpdateInfo();


		// ���� ���� �����̸� ���� ���μ����� �����Ų��
		if ( m_nStatus == STATUS_STARTGAME)
			StartGame();
	}


	CDDBDialog::OnTimer(nIDEvent);
}

void CNewAppDlg::SetExtend( int nNum)
{
	bool bInvalid = false;

	if ( nNum == -1)
	{
		nNum = AfxGetApp()->GetProfileInt( "Server List Option", "Extend", 0);
		bInvalid = true;
	}

	if ( nNum > 1)
		nNum = 0;

	switch ( nNum)
	{
	case 0 :
		m_cServerList.ModifyStyle( LVS_ICON | LVS_SMALLICON | LVS_LIST,  LVS_REPORT);
		break;

	case 1 :
		m_cServerList.ModifyStyle( LVS_REPORT | LVS_SMALLICON | LVS_LIST,  LVS_ICON);
		break;

		/*	case 2 :
		m_cServerList.ModifyStyle( LVS_ICON | LVS_REPORT | LVS_LIST,  LVS_SMALLICON);
		break;

		case 3 :
		m_cServerList.ModifyStyle( LVS_ICON | LVS_REPORT | LVS_SMALLICON,  LVS_LIST);
		break;
		*/	}


	if ( bInvalid == false)
		AfxGetApp()->WriteProfileInt( "Server List Option", "Extend", nNum);
}


void CNewAppDlg::OnBnExtend()
{
	int nNum = AfxGetApp()->GetProfileInt( "Server List Option", "Extend", 0);
	SetExtend( ++nNum);
}

/************************************************************************************
  ������Ʈ ����
*************************************************************************************/
void CNewAppDlg::OnBnClickedStartgame()
{
	// ������Ʈ �Ϸ� �����̸� ������ �����Ų��
	if ( (ZConfig::m_bSkipUpdate == true) || (m_bUpdateComplete == true))
	{
		POSITION pos = m_cServerList.GetFirstSelectedItemPosition();
		int nSel = m_cServerList.GetNextSelectedItem( pos);
		if ( nSel < 0  ||  m_ServerList[ nSel].bActive == false)
			return;
		AfxGetApp()->WriteProfileInt( "Connect Info", "server", nSel);

		CString strID;
		m_cID.GetWindowText( strID);
		if ( strID.IsEmpty())
			return;
		AfxGetApp()->WriteProfileString( "Connect Info", "id", strID);

		CString strPassword;
		m_cPassword.GetWindowText( strPassword);
		//		if ( m_cPassword.IsEmpty())
		//			return;
		AfxGetApp()->WriteProfileString( "Connect Info", "password", strPassword);

		// ���� ������ localhost�� ��� �α��� ������ ��ġ�� �ʰ� GameServer�� �����ϱ� ���� - birdkr
		CString strLaunch = "launch";
		if (!_stricmp(m_ServerList[ nSel].strIP.c_str(), "localhost"))
		{
			strLaunch = "login";
		}

		if (ZConfig::m_bSkipSelectWorldForPWE)
			m_strCommand.Format( "%s %s %s skip_sel_world", strLaunch, strID, m_ServerList[ nSel].strIP.c_str());	///< PWE ������ �Ķ���� �ӽ÷� �߰�.
		else
			m_strCommand.Format( "%s %s %s", strLaunch, strID, m_ServerList[ nSel].strIP.c_str());


		// ���̾�α��� ���� ��ġ�� ����
		CRect rect;
		GetWindowRect( rect);
		AfxGetApp()->WriteProfileInt( "Window Position", "x", rect.left);
		AfxGetApp()->WriteProfileInt( "Window Position", "y", rect.top);

		// ���� ����
		m_nStatus = STATUS_STARTGAME;

		StartGame();
	}
	else
	{
		// ��Ʈ ����
		if ( !m_Update.IsCreated())
		{
			// ���� ����̸�...
			if ( m_bIsAlpha)
				m_Update.Create( _T( STR_ADDRESS_ALPHA), 80, STR_DIRECTORY_ALPHA, "", "");

			// ���� ����̸�...
			else
				m_Update.Create( ZConfig::m_strPatchServAddr.c_str(), 80, ZConfig::m_strPatchServDir.c_str(), "", "");
		}


		// ����� ������ ����

		// �����尡 ���� �ȳ����� ���� ������ ��� ���
		int i = 0;
		while ( m_bThreadContinue == true)
		{
			// ���� �ð� �Ŀ��� �����尡 ������� ������ ����
			if ( i > 10)
			{
				PutLog( "[APP] ERROR - The thread is not terminating.");

				m_nStatus = STATUS_READY;

				return;
			}

			Sleep( 100);

			i++;
		}


		m_bThreadContinue = true;
		m_pThreadUpdate = AfxBeginThread( ThreadUpdate, (LPVOID*)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		m_pThreadUpdate->ResumeThread();				// Run thread


		// �����찡 ��Ŀ���� �Ҿ������ �޽����� ���޴� ��찡 ����Ƿ� �������� �ٸ� ��Ʈ���� ��Ŀ�� ���ش�
		m_webPage.SetFocus();
	}
}


/************************************************************************************
  ������Ʈ ������
*************************************************************************************/
UINT ThreadUpdate( LPVOID pParam)
{
	// pDlg�� ���ؼ� NewAppDlg Ŭ������ ��� ����/�Լ�(public�� ��쿡��)��
	// ������ �� �ִ�.
	CNewAppDlg *pDlg = (CNewAppDlg*)pParam;


	// ��� ���¿������� ������Ʈ ������ �ٿ� �޴´�
	if ( (pDlg->m_nStatus == STATUS_READY) || (pDlg->m_nStatus == STATUS_CANNOT_FIND_PATCHINFO) || (pDlg->m_nStatus == STATUS_UPDATE_FAIL))
	{
		// �α� ���
		PutLog( "[APP] Download patch info file");

		// ���� ����
		pDlg->m_nStatus = STATUS_ANALYSE_PATCHINFO;


		// ��ġ ���� �̸��� ���Ѵ�
		CString strMessageSecond;
#ifdef _DEBUG
		strMessageSecond.Format( "Runtime/%s", STR_PATCHINFO_FILENAME);
#else  
		strMessageSecond.Format( "%s", STR_PATCHINFO_FILENAME);
#endif

		// ��ġ ���� ������ �ٿ� �޴´�
		bool bRetVal = pDlg->m_Update.UpdatePatchInfoFile( STR_PATCHINFO_FILENAME, strMessageSecond.GetBuffer() );


		// ��ġ ������ ���� ���ߴٸ�...
		if ( bRetVal == false)
		{
			// �α� ���
			PutLog( "[APP] ERROR - Cannot download patch info file");

			// ���� ����
			pDlg->m_nStatus = STATUS_CANNOT_FIND_PATCHINFO;

			// �޽��� ������
			pDlg->PostMessage( WM_UPDATE_ERROR, 0, 0);

			// ����
			pDlg->m_bThreadContinue = false;
			return true;
		}

		// ��ġ ������ �޾Ҵٸ�...
		// ��ġ�� ������ �ִ� ���,
		if ( pDlg->m_Update.GetUpdateInfo().GetTotalPatchFileCount() > 0)
		{
			// �α� ���
			PutLog( "[APP] Exist patch file(s)");


			// ���� ����
			pDlg->m_nStatus = STATUS_FIND_NEW_FILE;


			// �޽��� ������
			pDlg->PostMessage( WM_FIND_PATCHINFO, 0, 0);
		}
		else // ��ġ�� ������ ���� ���,
		{
			// ���� ����
			pDlg->m_bUpdateComplete = true;
			pDlg->m_nStatus = STATUS_READY;
		}

		/// ������ �޾Ҵ� ��ġ ������ ������ �����.
		pDlg->m_Update.DeletePatchFiles();
	}


	// ������Ʈ ������ Ȯ�������� ��ġ ������ �ٿ�޴´�
	else if ( pDlg->m_nStatus == STATUS_FIND_NEW_FILE)
	{
		// �α� ���
		PutLog( "[APP] Start file updating...");


		// ���� ����
		pDlg->m_nStatus = STATUS_UPDATING;


		// ������Ʈ ����
		bool bRetVal = bRetVal = pDlg->m_Update.StartUpdate();

		// ������Ʈ ������ ���...
		if ( bRetVal == false)
		{
			// �α� ���
			PutLog( "[APP] ERROR - Update fail");


			// ���� ����
			pDlg->m_nStatus = STATUS_UPDATE_FAIL;


			// �޽��� ������
			pDlg->PostMessage( WM_UPDATE_ERROR, 0, 0);
		}
		else // ������Ʈ ������ ���...
		{
			pDlg->m_Update.SetCurrentLauncherAndClientExeVersion( STR_LAUNCHERINFO_FILENAME, pDlg->m_nLauncherVersion );

			// �α� ���
			PutLog( "[APP] Update complete.");

			// ���� ����
			pDlg->m_nStatus = STATUS_UPDATE_COMPLETE;

			// �޽��� ������
			pDlg->PostMessage( WM_UPDATE_COMPLETE, 0, 0);
		}
	}


	pDlg->m_bThreadContinue = false;

	return true;
}


/************************************************************************************
  ������Ʈ�� �����Ѵ�
*************************************************************************************/
void CNewAppDlg::OnBnClickedStop()
{
	// �α� ���
	PutLog( "[APP] Update canceled...");


	// ������Ʈ ��� �÷��� ����
	m_bUpdateCanceled = true;


	// ������Ʈ ����
	m_Update.StopUpdate();


	// �����찡 ��Ŀ���� �Ҿ������ �޽����� ���޴� ��찡 ����Ƿ� �������� �ٸ� ��Ʈ���� ��Ŀ�� ���ش�
	m_webPage.SetFocus();
}


/************************************************************************************
  ���μ��� ���� ���� UI ������Ʈ
*************************************************************************************/
void CNewAppDlg::UpdateStartUI()
{
	static bool bUpdateStartUI = true;
	if ( !bUpdateStartUI)
		return;


	// ���� ������ ����ð�
	static DWORD dwTime = timeGetTime();
	DWORD currTime = timeGetTime() - dwTime;


	// ���� �ð� �ڿ� start game ��ư�� Ȱ��ȭ ��Ų��
	if ( currTime > 2000)
	{
		m_nStatus = STATUS_READY;

		if ( ZConfig::m_bSkipUpdate == true)
		{
			m_bUpdateComplete = true;

			m_cStartGame.EnableWindow( TRUE);
		}
		else
			OnBnClickedStartgame();


		bUpdateStartUI = false;
	}
}


/************************************************************************************
  ���������� �ε��Ѵ�
*************************************************************************************/
void CNewAppDlg::UpdateWebLoading()
{
	// �������� �ε��� �������� Ȯ��
	static bool bCompleteOpenWeb = false;
	if ( bCompleteOpenWeb)
		return;


	// ���������� �������� Ȯ��
	static bool bOpenWeb = false;

	// ���μ����� ����ǰ� ���� �ð� �Ŀ� ���������� ����. ���μ��� ���� ���Ŀ� CPU ������ ���� �� �����Ƿ�...
	static DWORD dwTime = timeGetTime() + 1000;
	if ( !bOpenWeb && (timeGetTime() > dwTime))
	{
		// �������� ����
		m_webPage.Navigate( _T( ZConfig::m_strWebPageURL.c_str()), NULL, NULL, NULL, NULL);
		bOpenWeb = true;
	}


	// �ε� �� ���μ��� ������ ������������ ��ٸ���. ���� 1�ʸ� �����
	static bool bLoadingComplete = false;
	if ( !bLoadingComplete && m_bNavigationComplete)
	{
		bLoadingComplete = true;
		dwTime = timeGetTime() + 1000;
	}


	// �������� �ε尡 �������� �������� ����
	if ( !bCompleteOpenWeb && bOpenWeb && bLoadingComplete && (timeGetTime() > dwTime))
	{
#define POS_WEBPAGE_X		15
#define POS_WEBPAGE_Y		30
#define SIZE_WEBPAGE_X		494
#define SIZE_WEBPAGE_Y		420

		// â�� ��ũ�� �Ǹ鼭 ������ �Ѵ�
		CRect rect;
		m_webPage.GetClientRect( &rect);

		// ����
		int height = min( SIZE_WEBPAGE_Y, rect.bottom - rect.top + 20);

		// �� ��Ʈ�� â�� ũ�⸦ �����Ѵ�
		m_webPage.MoveWindow( POS_WEBPAGE_X, POS_WEBPAGE_Y, SIZE_WEBPAGE_X, height, FALSE);

		if ( height >= SIZE_WEBPAGE_Y)
			bCompleteOpenWeb = true;

		return;
	}


	// ���� ������ �����
	int x = 480, y = 422, r = 18;
	float gap = 6.28f / 10.0f;
	for ( int i = 0;  i < 10;  i++)
	{
		float value = gap * (float)i;

		int dx = x + (int)( (float)r * sin( value));
		int dy = y + (int)( (float)r * cos( value));


		int color = (int)( 70.0f * sin( value + (double)timeGetTime() / 300.0f) + 70.0f);

		GetScreen()->PutFillCircle( dx, dy, 3, RGB( color, color, color));
	}


	// ȭ�� ����
	Rendering( x-r-10, y-r-10, r*2+20, r*2+20);
}


/************************************************************************************
���� ����Ʈ�� ������Ʈ��
*************************************************************************************/
void CNewAppDlg::UpdateServerList()
{
	GetScreen()->PutFillBox( 523, 263, 212, 15, RGB( 46, 47, 48));
	POSITION pos = m_cServerList.GetFirstSelectedItemPosition();
	int nSel = m_cServerList.GetNextSelectedItem( pos);
	if ( m_bUpdateServerList == false)
	{
		GetScreen()->PutBitmap( 528, 263, GetSkin()->GetDC(), 304, 49, 16, 13);
		GetScreen()->PutText( 547, 263, 14, "Arial", ZGetString( "STR_153"), RGB( 230, 230, 0));
	}
	else if ( nSel < 0)
	{
		GetScreen()->PutBitmap( 528, 263, GetSkin()->GetDC(), 304, 49, 16, 13);
		GetScreen()->PutText( 547, 263, 14, "Arial", ZGetString( "STR_154"), RGB( 230, 0, 0));
	}
	else if ( m_ServerList[ nSel].bActive == false)
	{
		GetScreen()->PutBitmap( 528, 263, GetSkin()->GetDC(), 304, 49, 16, 13);
		GetScreen()->PutText( 547, 263, 14, "Arial", ZGetString( "STR_155"), RGB( 230, 0, 0));
	}
	Rendering( 523, 263, 212, 15);


	CString str;
	bool bEnable = true;
	m_cID.GetWindowText( str);
	if ( str.IsEmpty()  ||  str.GetLength() < 1)	bEnable = false;
	m_cPassword.GetWindowText( str);
	//	if ( str.IsEmpty()  ||  str.GetLength() < 4)	bEnable = false;
	GetScreen()->PutFillBox( 523, 413, 200, 15, RGB( 46, 47, 48));
	if ( bEnable == false)
	{
		GetScreen()->PutBitmap( 528, 413, GetSkin()->GetDC(), 304, 49, 16, 13);
		GetScreen()->PutText( 547, 413, 14, "Arial", ZGetString( "STR_156"), RGB( 230, 0, 0));
	}
	Rendering( 523, 413, 212, 15);



	if ( m_bUpdateServerList == true)
		return;


	// ���μ����� ����ǰ� ���� �ð� �Ŀ� �۵���
	static DWORD dwTime = timeGetTime() + 50;
	if ( timeGetTime() < dwTime)
		return;


	bool bSuccess = false;

	if (ZConfig::m_bStaticLaunch)
	{
		string strPage =	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>	\
							<XML><Server name=\"";
		strPage += ZConfig::m_strStaticServerName;
		strPage += "\" ";
		strPage += "ip=\"";
		strPage += ZConfig::m_strStaticIPAddress;
		strPage += "\" ";
		strPage += "active=\"true\"/></XML>";
		if ( ParseServerList( strPage))
			bSuccess = true;
	}
	else
	{
		string strPage;
		if ( GetHttpPage( STR_SERVERLIST_URL, &strPage))
		{
			if ( ParseServerList( strPage))
				bSuccess = true;
		}
	}

	m_bUpdateServerList = true;
}


/************************************************************************************
  ��ġ ���� �߰�
*************************************************************************************/
LRESULT CNewAppDlg::OnFindPatchInfo( WPARAM wParam, LPARAM lParam)
{
	// ���� ���� ���� �׳� ����
	if ( m_bReservedQuit)
		return TRUE;


// 	// Ȯ��â ����
// 	CNewAppUpdateConfirm dlg;
// 	CString str;
// 	str.Format( /*There are %d file(s) to update. The game must go through updating process for normal operation.  It will update automatically after %%d second(s)...*/ ZGetString( "STR_104"),  m_Update.GetUpdateInfo().GetTotalPatchFileCount());
// 	dlg.SetDialog( this, /*Update Confirm*/ ZGetString( "STR_103"), str, MB_YESNO);
// 	this->SetFocus();			// ��Ŀ�� �ٽ� ȹ��
// 	int nRetVal = dlg.DoModal();
// 
// 
// 	// Yes ��ư�� ������ ������Ʈ ����
// 	if ( nRetVal == IDOK)
// 		OnBnClickedStartgame();
// 
// 	// No ��ư�� ����
// 	else
// 		m_nStatus = STATUS_UPDATE_FAIL;
	

	OnBnClickedStartgame();

	return TRUE;
}


/************************************************************************************
  ������Ʈ �Ϸ�
*************************************************************************************/
LRESULT CNewAppDlg::OnUpdateComplete( WPARAM wParam, LPARAM lParam)
{
	// ���� ���� ���� �׳� ����
	if ( m_bReservedQuit)
		return TRUE;


	// update complete box ����
	CNewAppUpdateCompleteDlg dlg;


	// ������� �ʿ��� ���
	this->SetFocus();			// ��Ŀ�� �ٽ� ȹ��
	if ( m_Update.IsReboot())
	{
		// ���̾�α� �ڽ��� ���
		dlg.SetDialog( this, /*Update Complete*/ ZGetString( "STR_105"), /*The launcher has to restart in order to finish file update.  The launcher will restart in %d second(s)...*/ ZGetString( "STR_106"), MB_OK);
		dlg.DoModal();


		// �ܺ� ������Ʈ ���μ����� �����Ѵ�
		RunExternalUpdater();


		// ���μ��� ����
		OnQuit();
	}

	// �ٷ� ������ �����ϴ� �����...
	else
	{
		// ������Ʈ �Ϸ� Ȯ��
		m_bUpdateComplete = true;


//		// Ȯ��â�� ����
// 		dlg.SetDialog( this, /*Update Complete*/ ZGetString( "STR_105"), /*The Update is successfully finished.  The game will start automatically in %d second(s)...*/ ZGetString( "STR_107"), MB_OK);
// 		dlg.DoModal();
// 
// 		// ���� ����
// 		m_nStatus = STATUS_STARTGAME;

		m_nStatus = STATUS_READY;
	}


	return TRUE;
}


/************************************************************************************
  ������Ʈ ���� �߻�
*************************************************************************************/
LRESULT CNewAppDlg::OnUpdateError( WPARAM wParam, LPARAM lParam)
{
	// ���� ���� ���� �׳� ����
	if ( m_bReservedQuit)
		return TRUE;


	// �α׷� �����
	CString strLog;
	strLog.Format( "[APP] Updater error message :\n\n%s", m_Update.GetErrorMsg()->GetBuffer());
	PutLog( strLog);

	strLog.Format( "[App] File transfer error message :\n\n%s", m_Update.GetFileTransferError());
	PutLog( strLog);


	// Error dialog ����
	CNewAppErrorDlg dlg;
	dlg.SetDialog( this, /*Error Report*/ ZGetString( "STR_108"), /*There was an error on the process.  We appologize for your inconvenience.  Please check the error report below and elimate the source or error manually.*/ ZGetString( "STR_109"), m_Update.GetErrorMsg(), MB_OK);
	this->SetFocus();			// ��Ŀ�� �ٽ� ȹ��
	int nRetVal = dlg.DoModal();


	return TRUE;
}


/************************************************************************************
  ���� ����
*************************************************************************************/
void CNewAppDlg::StartGame()
{
	static DWORD dwStartTime = timeGetTime();
	DWORD curr_time = timeGetTime() - dwStartTime;


	// 1�� �ڿ� ���� ���μ����� �����Ų��
	static bool bRunGameProcess = false;
	if ( !bRunGameProcess && (curr_time > 1000))
	{
		// ���μ��� ����
		char szCmdLine[ 4096] = { 0, };
		sprintf_s( szCmdLine, ".\\%s %s", STR_EXCUTE_FILENAME, m_strCommand);

		// ���� ���μ����� �����Ų��
		int nRet = WinExec( szCmdLine, SW_SHOW);
		CString strMsg;
		strMsg.Format("StartGame() | Result(%d)", nRet );
		PutLog( strMsg );

		bRunGameProcess = true;
		dwStartTime = timeGetTime();

		return;
	}
	else if ( !bRunGameProcess && (curr_time > 200))
	{
		ShowWindow( SW_MINIMIZE);
	}


	// ���� ���μ����� ����Ǹ� �����Ѵ�
	static bool bQuit = false;
	if ( bRunGameProcess && !bQuit && (curr_time > 5000))
	{
		// ���μ��� ����
		OnQuit();

		bQuit = true;
	}
}


/************************************************************************************
  ���� â ������Ʈ
*************************************************************************************/
void CNewAppDlg::UpdateInfo()
{
	ZUpdateUIInfo info = m_Update.GetUpdateInfo();
	CString strMessageFirst = "";
	CString strMessageSecond = "";
	float fPercent = 0.0f;


	// ���� �� �۾��� ���� �б�
	switch ( m_nStatus)
	{
		// ���� �����찡 ��������� ���� ����
		case STATUS_NONE :
		{
			// ǥ�� ����
			strMessageFirst = /*Initializing...*/ ZGetString( "STR_110");
			DWORD dwTime = (timeGetTime() / 120) % 15;
			for ( int i = 0;  i < (int)dwTime;  i++)	strMessageFirst += ".";

			break;
		}

		// ������ ��ŸƮ ��ư�� ���������� ���
		case STATUS_READY :
		{
			// ���ϸ��̼� ����
			m_cAnimation.EnableAnimate( FALSE);
			m_cAnimation.ShowScene( 0);

			// ǥ�� ����
			strMessageFirst = /*Ready to start.*/ ZGetString( "STR_111");

			// UI ����
			if ( m_bUpdateComplete == true)
			{
				m_cStartGame.EnableWindow( TRUE);
			}
			else
			{
				m_cStartGame.EnableWindow( FALSE);
			}
			m_cStop.EnableWindow( FALSE);
			break;
		}

		// ������Ʈ ���� �м���
		case STATUS_ANALYSE_PATCHINFO :
		{
			// ���ϸ��̼� ����
			m_cAnimation.EnableAnimate( TRUE);

			// ǥ�� ����
			strMessageFirst = /*Analyzing patch info*/ ZGetString( "STR_114");
			DWORD dwTime = (timeGetTime() / 120) % 15;
			for ( int i = 0;  i < (int)dwTime;  i++)	strMessageFirst += ".";

			// UI ����
			m_cStartGame.EnableWindow( FALSE);
			m_cStop.EnableWindow( FALSE);
			break;
		}

		// ������Ʈ ������ �޾ƿ��� ������ ���
		case STATUS_CANNOT_FIND_PATCHINFO :
		{
			// ���ϸ��̼� ����
			m_cAnimation.EnableAnimate( FALSE);
			m_cAnimation.ShowScene( 0);

			// ǥ�� ����
			strMessageFirst = /*Cannot find patch info.*/ ZGetString( "STR_115");

			// UI ����
			m_cStartGame.EnableWindow( TRUE);
			m_cStop.EnableWindow( FALSE);
			break;
		}

		// ������Ʈ �� ������ ���� ���
		case STATUS_FIND_NEW_FILE:
		{
			// ǥ�� ����
			strMessageFirst = /*Find new patch files.*/ ZGetString( "STR_116");

			// UI ����
			m_cStartGame.EnableWindow( FALSE);
			m_cStop.EnableWindow( FALSE);
			break;
		}

		// ���� ������Ʈ ��
		case STATUS_UPDATING :
		{
			// ǥ�� ����
			INT64 nCount = info.GetCurrPatchedFileCount() + 1;
			if ( nCount > info.GetTotalPatchFileCount())
			{
				strMessageFirst = /*Patching files*/ ZGetString( "STR_117");
				strMessageSecond = /*Please wait while the file duplication process takes place.*/ ZGetString( "STR_118");

				fPercent = 100.0f;
			}
			else
			{
				strMessageFirst.Format( "%s (%I64d / %I64d)", /*Updating files*/ ZGetString( "STR_119"), nCount, info.GetTotalPatchFileCount());


				// ���� ������Ʈ�ϴ� ������ ���� ��
				if ( (int)strlen( info.GetCurrPatchFileName()) > 2)
				{
					float _fPercent = (float)info.GetCurrDownloadSize() / (float)info.GetTotalDownloadSize() * 100.0f;
					if ( _fPercent < 0.0f)		_fPercent = 0.0f;
					if ( _fPercent > 100.0f)	_fPercent = 100.0f;

					strMessageSecond.Format( "'%s'    %.1f%%   (%I64d / %I64d byte)", info.GetCurrPatchFileName(), _fPercent, info.GetCurrDownloadSize(), info.GetTotalDownloadSize());
				}
				else
				{
					strMessageSecond = /*Read patch file info*/ ZGetString( "STR_120");
					DWORD dwTime = (timeGetTime() / 500) % 5;
					for ( int i = 0;  i < (int)dwTime;  i++)	strMessageFirst += ".";
				}
				fPercent = (float)(info.GetCurrPatchedFileSize() + info.GetCurrDownloadSize()) / (float)info.GetTotalPatchFileSize() * 100.0f + 0.5f;
			}

			DWORD dwTime = (timeGetTime() / 500) % 5;
			for ( int i = 0;  i < (int)dwTime;  i++)	strMessageFirst += ".";


			// UI ����
			m_cStartGame.EnableWindow( FALSE);
			m_cStop.EnableWindow( TRUE);
			break;
		}

		// ���� ������Ʈ ����
		case STATUS_UPDATE_FAIL :
		{
			// ���ϸ��̼� ����
			m_cAnimation.EnableAnimate( FALSE);
			m_cAnimation.ShowScene( 0);

			// ǥ�� ����
			strMessageFirst = /*Update failed.*/ ZGetString( "STR_121");

			// UI ����
			m_cStartGame.EnableWindow( TRUE);
			m_cStop.EnableWindow( FALSE);
			break;
		}

		// ���� ������Ʈ �Ϸ�
		case STATUS_UPDATE_COMPLETE :
		{
			// ���ϸ��̼� ����
			m_cAnimation.EnableAnimate( FALSE);
			m_cAnimation.ShowScene( 0);

			// ǥ�� ����
			strMessageFirst = /*Update complete.*/ ZGetString( "STR_122");
			strMessageSecond = /*Update complete.*/ ZGetString( "STR_122");
			fPercent = 100.0f;

			// UI ����
			m_cStartGame.EnableWindow( FALSE);
			m_cStop.EnableWindow( FALSE);
			break;
		}

		// ���� ����
		case STATUS_STARTGAME :
		{
			// ���ϸ��̼� ����
			m_cAnimation.EnableAnimate( FALSE);
			m_cAnimation.ShowScene( 0);


			// ǥ�� ����
			strMessageFirst = /*Starting game*/ ZGetString( "STR_123");
			DWORD dwTime = (timeGetTime() / 500) % 5;
			for ( int i = 0;  i < (int)dwTime;  i++)	strMessageFirst += ".";

			fPercent = 0.0f;

			// UI ����
			m_cStartGame.EnableWindow( FALSE);
			m_cStop.EnableWindow( FALSE);
			break;
		}
	}


	if ( strMessageSecond.GetLength() == 0)
	{
		if ( ZConfig::m_bSkipUpdate)
			strMessageSecond += /*Skip update.*/ ZGetString( "STR_112");
		strMessageSecond += "    ";

		if ( m_bIsAlpha)
			strMessageSecond += /*Alpha testing mode.*/ ZGetString( "STR_113");
		strMessageSecond += "    ";
	}



	// �ۼ�Ʈ�� 0���� �۰ų� 100�� ���� �ʵ��� ����
	if ( fPercent < 0.0f)		fPercent = 0.0f;
	if ( fPercent > 100.0f)		fPercent = 100.0f;


	// ���� �г� ����
	strMessageFirst = "     " + strMessageFirst;
	DrawPanel( 10, 460, 500, 70, strMessageFirst);


	// ���� �ٿ�޴� ���� �̸� ǥ��
#ifdef LOCALE_NHNUSA
	GetScreen()->PutText( 23, 489, 15, "Arial", strMessageSecond, GetTextColor());
#else
	GetScreen()->PutText( 20, 489, 15, "Arial", strMessageSecond, RGB( 200, 200, 200));
#endif


	// ������ �� ǥ��
	float fGausse = 489.0f * (fPercent / 100.0f) + 1.5f * (float)sin( (double)timeGetTime() / 100.0f);
	if ( (fGausse < 0.0f) || (fPercent < 0.01f))		fGausse = 0.0f;
	if ( (fGausse > 489.0f) || (fPercent > 99.99f)) 	fGausse = 489.0f;

	GetScreen()->PutBitmap( 16, 506, GetSkin()->GetDC(), 0, 86, 489, 19);
	GetScreen()->PutBitmap( 16, 506, GetSkin()->GetDC(), 0, 63, (int)fGausse, 19);


	// ��ü �ٿ�ε� �ۼ������� ǥ��
	if ( fPercent > 0.0f)
	{
		CString strPercent;
		strPercent.Format( "%.1f%%", fPercent);
#ifdef LOCALE_NHNUSA
		GetScreen()->PutText( 17, 507, 489, 19, 15, "Arial", strPercent.GetBuffer(), RGB( 50, 50, 50), TS_NORMAL, DT_CENTER);
		GetScreen()->PutText( 16, 506, 489, 19, 15, "Arial", strPercent.GetBuffer(), RGB( 200, 200, 200), TS_NORMAL, DT_CENTER);
#else
		GetScreen()->PutText( 16, 508, 489, 19, 15, "Arial", strPercent.GetBuffer(), RGB( 200, 200, 200), TS_NORMAL, DT_CENTER);
#endif
	}


	// ȭ�� ����
	Rendering( 10, 460, 500, 70);
}


/************************************************************************************
  Run external updater
*************************************************************************************/
void CNewAppDlg::RunExternalUpdater()
{
	HRSRC hRes = ::FindResource( AfxGetResourceHandle(), MAKEINTRESOURCE( IDR_UPDATER), _T( "EXE"));
	if ( hRes == NULL)
		return;

	HGLOBAL hData = ::LoadResource( AfxGetResourceHandle(), hRes);
	if ( hData == NULL)
		return;

	LPVOID lpData= LockResource( hData);

	DWORD dwSize = SizeofResource( AfxGetInstanceHandle(), hRes);
	DWORD dwWriteSize = 0;
	HANDLE hFile = ::CreateFile( STR_EXT_UPDATER_FILENAME, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( hFile != INVALID_HANDLE_VALUE)
	{
		// Make 'updater.exe' file
		::WriteFile( hFile, lpData, dwSize, &dwWriteSize, NULL);
		::CloseHandle( hFile);


		// Wait...
		Sleep( 1000);


		// Run updater
		if ( dwSize == dwWriteSize)
		{
			PutLog( "[APP] Run external updater");

			char szCmdLine[ 4096] = { 0, };

#ifdef LOCALE_NHNUSA
			sprintf( szCmdLine, ".\\%s \"%s\"", STR_EXT_UPDATER_FILENAME, GetNHNModule().GetAuth().GetAuthStr());
#else
			sprintf( szCmdLine, ".\\%s", STR_EXT_UPDATER_FILENAME);
#endif

			// Run updater
			WinExec( szCmdLine, SW_SHOW);
		}

		else
			PutLog( "[APP] Invalid updater file");
	}

	else
		PutLog( "[APP] Cannot create updater file");


	FreeResource( hData);
}


/************************************************************************************
  About box
*************************************************************************************/
void CNewAppDlg::OnAbout()
{
	// �޽��� �ۼ�
	CString strMsg;
	strMsg.Format( "%s Launcher\nCopyright (c) 2005 MAIET entertainment, Inc.\n\n%s Version : %d\nLauncher Version : %d",
		STR_PROCESS_NAME, STR_PROCESS_NAME, m_nClientVersion, m_nLauncherVersion);

	// About box ����
	CNewAppAboutBox dlg;
	dlg.SetDialog( this, "About", strMsg, MB_OK);
	this->SetFocus();			// ��Ŀ�� �ٽ� ȹ��
	dlg.DoModal();
}


/************************************************************************************
  ���� ó��
*************************************************************************************/
// OnOK : ���� Ű�� ���� �Ǵ°� ����
void CNewAppDlg::OnOK()
{
	OnBnClickedStartgame();
}

// OnCancel : ESC Ű�� ����
void CNewAppDlg::OnCancel()
{
	// ������Ʈ ���̸� ������Ʈ�� �����Ѵ�
	if ( m_nStatus == STATUS_UPDATING)
		OnBnClickedStop();

	// �ƴϸ� �ٷ� �����Ѵ�
	else
		OnQuit();
}

// OnQuit
void CNewAppDlg::OnQuit()
{
	// ���μ��� ���� ������ �Ѵ�. ������Ʈ Ÿ�̸ӿ��� �����Ų��
	m_bReservedQuit = true;

	// ������ ���� ��ġ ������ �����
	m_Update.DeleteTempDirectory();
	m_Update.DeleteCurDirFile( "CNewAppDlg::OnInitDialog()", STR_PATCHINFO_FILENAME );

	// ������Ʈ ���̸� ������Ʈ�� �����Ѵ�
	if ( m_nStatus == STATUS_UPDATING)
		OnBnClickedStop();
}

// OnDestroyDialog
void CNewAppDlg::OnDestroyDialog()
{
	// �������� ����
	m_Update.Destroy();


	// ���̾�α��� ���� ��ġ�� ����
// 	CRect rect;
// 	GetWindowRect( rect);
// 	AfxGetApp()->WriteProfileInt( "Window Position", "x", rect.left);
// 	AfxGetApp()->WriteProfileInt( "Window Position", "y", rect.top);


	// ������ ����
	CDialog::OnOK();
}


/************************************************************************************
  ���� ������ ����
*************************************************************************************/
void CNewAppDlg::GetFileVersion( const char* pszFileName, char* pszVersion)
{
	strcpy_s( pszVersion, 25, "unknown");


	DWORD dwHandle;
	DWORD dwLength = GetFileVersionInfoSize( pszFileName, &dwHandle);

	if ( !dwLength)
		return;

	std::auto_ptr<BYTE> lpBlock(new BYTE[dwLength]);
	
	if ( !GetFileVersionInfo( pszFileName, dwHandle, dwLength, lpBlock.get()))
		return;


	LPVOID pVersion = NULL;
	DWORD uLength;
	if ( !VerQueryValue( lpBlock.get(), "\\VarFileInfo\\Translation", (LPVOID*)&pVersion, (UINT*)&uLength))
		return;


	CString rVersion;
	if ( uLength == 4)
	{
		DWORD langD;
		memcpy( &langD, pVersion, 4);            

		rVersion.Format( "\\StringFileInfo\\%02X%02X%02X%02X\\ProductVersion",
							( langD & 0xff00) >> 8,
							( langD & 0xff),
							( langD & 0xff000000) >> 24,
							( langD & 0xff0000) >> 16);
	}
	else
	{
		rVersion.Format( "\\StringFileInfo\\%04X04B0\\ProductVersion", GetUserDefaultLangID());
	}


	if ( VerQueryValue( lpBlock.get(), (LPSTR)(LPCTSTR)rVersion, (LPVOID*)&pVersion, (UINT *)&uLength) == 0)
		return;


	// Success
	strcpy_s( pszVersion, 25, (char*)pVersion);
}


/************************************************************************************
  OS ������ ����
*************************************************************************************/
#define SM_SERVERR2		89
typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);

int CNewAppDlg::GetOSVersion( char* pszBuff)
{
	int nMajorID = -1;

	memset( pszBuff, 0, sizeof( pszBuff));

	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	PGNSI pGNSI;
	BOOL bOsVersionInfoEx;

	ZeroMemory( &si, sizeof( SYSTEM_INFO));
	ZeroMemory( &osvi, sizeof( OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX);
	if ( !(bOsVersionInfoEx = GetVersionEx( (OSVERSIONINFO*) &osvi)))
	{
		osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO);
		if ( !GetVersionEx( (OSVERSIONINFO*)&osvi)) 
			return -1;
	}

	pGNSI = (PGNSI)GetProcAddress( GetModuleHandle( TEXT( "kernel32.dll")), "GetNativeSystemInfo");
	if ( NULL != pGNSI)		pGNSI( &si);
	else					GetSystemInfo( &si);

	nMajorID = osvi.dwMajorVersion;

	switch ( osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
		{
			if ( osvi.wProductType == VER_NT_WORKSTATION)					strcat_s( pszBuff, 128, "Windows Vista ");
			else															strcat_s( pszBuff, 128, "Windows Server \"Longhorn\" ");
		}
		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
		{
			if ( GetSystemMetrics( SM_SERVERR2))							strcat_s( pszBuff, 128, "Microsoft Windows Server 2003 \"R2\" ");
			else if ( osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
				strcat_s( pszBuff, 128, "Microsoft Windows XP Professional x64 Edition ");
			else															strcat_s( pszBuff, 128, "Microsoft Windows Server 2003, ");
		}
		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)			strcat_s( pszBuff, 128, "Microsoft Windows XP ");
		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)			strcat_s( pszBuff, 128, "Microsoft Windows 2000 ");
		if ( osvi.dwMajorVersion <= 4)										strcat_s( pszBuff, 128, "Microsoft Windows NT ");

		if ( bOsVersionInfoEx)
		{
			if ( osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture!=PROCESSOR_ARCHITECTURE_AMD64)
			{
				if ( osvi.dwMajorVersion == 4)								strcat_s( pszBuff, 128, "Workstation 4.0 ");
				else if ( osvi.wSuiteMask & VER_SUITE_PERSONAL)				strcat_s( pszBuff, 128, "Home Edition ");
				else														strcat_s( pszBuff, 128, "Professional ");
			}

			else if ( osvi.wProductType == VER_NT_SERVER || osvi.wProductType == VER_NT_DOMAIN_CONTROLLER)
			{
				if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
				{
					if ( si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
					{
						if ( osvi.wSuiteMask & VER_SUITE_DATACENTER)		strcat_s( pszBuff, 128, "Datacenter Edition for Itanium-based Systems ");
						else if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE)	strcat_s( pszBuff, 128, "Enterprise Edition for Itanium-based Systems ");
					}
					else if ( si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
					{
						if ( osvi.wSuiteMask & VER_SUITE_DATACENTER)		strcat_s( pszBuff, 128, "Datacenter x64 Edition ");
						else if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE)	strcat_s( pszBuff, 128, "Enterprise x64 Edition ");
						else												strcat_s( pszBuff, 128, "Standard x64 Edition ");
					}
					else
					{
						if ( osvi.wSuiteMask & VER_SUITE_DATACENTER)		strcat_s( pszBuff, 128, "Datacenter Edition ");
						else if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE)	strcat_s( pszBuff, 128, "Enterprise Edition ");
						else if ( osvi.wSuiteMask & VER_SUITE_BLADE)		strcat_s( pszBuff, 128, "Web Edition ");
						else												strcat_s( pszBuff, 128, "Standard Edition ");
					}
				}
				else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
				{
					if ( osvi.wSuiteMask & VER_SUITE_DATACENTER)			strcat_s( pszBuff, 128, "Datacenter Server ");
					else if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE)		strcat_s( pszBuff, 128, "Advanced Server ");
					else													strcat_s( pszBuff, 128, "Server ");
				}
				else  // Windows NT 4.0 
				{
					if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE)			strcat_s( pszBuff, 128, "Server 4.0, Enterprise Edition ");
					else													strcat_s( pszBuff, 128, "Server 4.0 ");
				}
			}
		}

#define BUFSIZE		80
		else  
		{
			HKEY hKey;
			TCHAR szProductType[ BUFSIZE];
			DWORD dwBufLen = BUFSIZE * sizeof( TCHAR);
			LONG lRet;

			lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE, TEXT( "SYSTEM\\CurrentControlSet\\Control\\ProductOptions"), 0, KEY_QUERY_VALUE, &hKey);
			if ( lRet != ERROR_SUCCESS)
				return -1;

			lRet = RegQueryValueEx( hKey, TEXT( "ProductType"), NULL, NULL, (LPBYTE) szProductType, &dwBufLen);
			RegCloseKey( hKey);

			if ( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE * sizeof( TCHAR)))
				return -1;

			if ( lstrcmpi( TEXT( "WINNT"), szProductType) == 0)				strcat_s( pszBuff, 128, "Workstation ");
			if ( lstrcmpi( TEXT( "LANMANNT"), szProductType) == 0)			strcat_s( pszBuff, 128, "Server ");
			if ( lstrcmpi( TEXT( "SERVERNT"), szProductType) == 0)			strcat_s( pszBuff, 128, "Advanced Server ");


			char szTmp[ 128];
			sprintf_s( szTmp, "%d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion);
			strcat_s( pszBuff, 128, szTmp);
		}

		// Display service pack (if any) and build number.
		if ( osvi.dwMajorVersion == 4 && lstrcmpi( osvi.szCSDVersion, TEXT( "Service Pack 6")) == 0)
		{
			HKEY hKey;
			LONG lRet;

			// Test for SP6 versus SP6a.
			lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE, TEXT( "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"), 0, KEY_QUERY_VALUE, &hKey);
			if ( lRet == ERROR_SUCCESS)
			{
				char szTmp[ 128];
				sprintf_s( szTmp, "Service Pack 6a (Build %d)", osvi.dwBuildNumber & 0xFFFF);
				strcat_s( pszBuff, 128, szTmp);
			}
			else // Windows NT 4.0 prior to SP6a
			{
				char szTmp[ 128];
				sprintf_s( szTmp, "%s (Build %d)", osvi.szCSDVersion, osvi.dwBuildNumber & 0xFFFF);
				strcat_s( pszBuff, 128, szTmp);
			}

			RegCloseKey( hKey);
		}
		else // not Windows NT 4.0 
		{
			char szTmp[ 128];
			sprintf_s( szTmp, "%s (Build %d)", osvi.szCSDVersion, osvi.dwBuildNumber & 0xFFFF);
			strcat_s( pszBuff, 128, szTmp);
		}

		break;


		// Test for the Windows Me/98/95.
	case VER_PLATFORM_WIN32_WINDOWS:
		if ( osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			strcat_s( pszBuff, 128, "Microsoft Windows 95 ");
			if ( osvi.szCSDVersion[ 1] == 'C' || osvi.szCSDVersion[ 1] == 'B')
				strcat_s( pszBuff, 128, "OSR2");
		} 
		if ( osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			strcat_s( pszBuff, 128, "Microsoft Windows 98 ");
			if ( osvi.szCSDVersion[ 1] == 'A' || osvi.szCSDVersion[ 1] == 'B')
				strcat_s( pszBuff, 128, "SE");
		} 
		if ( osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
			strcat_s( pszBuff, 128, "Microsoft Windows Millennium Edition");
		break;


	case VER_PLATFORM_WIN32s:
		strcat_s( pszBuff, 128, "Microsoft Win32s");
		break;
	}

	return nMajorID;
}

// IsEnableStart
bool CNewAppDlg::IsEnableStart()
{
	if ( m_bUpdateComplete == false)
		return false;

	POSITION pos = m_cServerList.GetFirstSelectedItemPosition();
	int nSel = m_cServerList.GetNextSelectedItem( pos);
	if ( nSel < 0)
		return false;

	if ( m_ServerList[ nSel].bActive == false)
		return false;

	CString str;
	m_cID.GetWindowText( str);
	if ( str.IsEmpty()  ||  str.GetLength() < 1)
		return false;

	//	m_cPassword.GetWindowText( str);
	//	if ( str.IsEmpty()  ||  str.GetLength() < 4)
	//		return false;

	return true;
}


// GetHttpPage
bool CNewAppDlg::GetHttpPage( const char* pszURL, string* pstrSource)
{
	CInternetSession itnSession;
	CInternetFile* pFile = NULL;

	// Internet ������ ����.
	try
	{
		pFile = (CInternetFile*)itnSession.OpenURL( pszURL);
		if ( !pFile)
			return false;
	}
	catch( CInternetException* pEx)
	{
		// Get error message
		char szErrorMessage[ 256];
		pEx->GetErrorMessage( szErrorMessage, sizeof( szErrorMessage));


		// Close
		if ( pFile)
		{
			pFile->Close();
			delete pFile;
			itnSession.Close();
		}

		return false;
	}


	// Set read buffer size
	pFile->SetReadBufferSize( 256);


	// �������� �ҽ��� �о�´�.
	string strSource;
	while ( 1)
	{
		// ������ �о�´�.
		CString strReadData;
		if ( !pFile->ReadString( strReadData))
			break;

		strSource += strReadData;
		strSource += "\r\n";

		// ���ҽ� �������� ����
		Sleep( 1);
	}

	// Close
	pFile->Close();
	delete pFile;
	itnSession.Close();

	// Convert UTF8 to ANSI
	*pstrSource = ConvUTF8ToAnsi( strSource.c_str());

	return true;
}


// ParseServerList
bool CNewAppDlg::ParseServerList( const string& strPage)
{
	MXml xml;
	if ( xml.LoadStream( strPage.c_str()) == false)
		return false;

	MXmlHandle docHandle = xml.DocHandle();
	MXmlElement* pRoot = docHandle.FirstChild( "XML").FirstChildElement().Element();
	if ( pRoot == NULL)
		return false;

	m_cServerList.DeleteAllItems();
	m_ServerList.clear();

	int index = 0;
	for ( MXmlElement* pChild = pRoot;  pChild != NULL;  pChild = pChild->NextSiblingElement())
	{
		string tag = pChild->ValueStr();
		if ( tag == "Server")
		{
			string strName, strIP;
			bool bActive = false;

			_Attribute( strName, pChild, "name");
			_Attribute( strIP, pChild, "ip");
			_Attribute( &bActive, pChild, "active");

			m_cServerList.InsertItem( index++, strName.c_str(), bActive ? 0 : 1);

			_SERVER server( strIP, bActive);
			m_ServerList.push_back( server);
		}
	}

	int sel = AfxGetApp()->GetProfileInt( "Connect Info", "server", 0);
	if ( sel >= (int)m_ServerList.size())  sel = 0;
	else if ( m_ServerList[ sel].bActive == false)  sel = 0;
	m_cServerList.SetItemState( sel, LVIS_SELECTED, 0x000F);
	m_cServerList.EnsureVisible( sel, false);

	return true;
}


// ConvUTF8ToAnsi
string CNewAppDlg::ConvUTF8ToAnsi( const char* pszString)
{
	if ( pszString == NULL)
		return std::string( "");

	if ( (unsigned char)pszString[ 0] == 0xEF  &&  (unsigned char)pszString[ 1] == 0xBB  &&  (unsigned char)pszString[ 2] == 0xBF)
		return TransCode( pszString + 3, CP_UTF8, CP_ACP);

	string str( pszString);
	return str;
}


// TransCode
string CNewAppDlg::TransCode( const wchar_t *pwszString, int nCodePage)
{
	std::string strRet; 
	int nReqLen = (int)WideCharToMultiByte( nCodePage, 0, pwszString, -1, 0, 0, 0, 0); 

	char *pszDst = new char[ nReqLen + 1]; 
	memset( pszDst, 0x00, (int)sizeof(char) * (nReqLen + 1));

	int nLen = (int)WideCharToMultiByte( nCodePage, 0, pwszString, -1, pszDst, nReqLen, 0, 0); 
	if (nLen)
	{
		pszDst[ nLen] = 0;
		strRet = pszDst;
	}

	delete [] pszDst;
	return strRet; 
}

string CNewAppDlg::TransCode( const char *pszString, int nPrevCodePage, int nCodePage)
{
	std::string strRet; 
	int nReqLen = (int)MultiByteToWideChar( nPrevCodePage, 0, pszString, -1, 0, 0 ); 

	wchar_t* pszDst = new wchar_t[ nReqLen + 1];
	memset( pszDst, 0x00, (int)sizeof( wchar_t) * (nReqLen + 1));

	int nLen = (int)MultiByteToWideChar( nPrevCodePage, 0, pszString, -1, pszDst, nReqLen ); 
	if ( nLen)
	{ 
		pszDst[ nLen] = 0;
		strRet = TransCode( pszDst, nCodePage);
	}

	delete[] pszDst;
	return strRet; 
}


void CNewAppDlg::OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;

	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	DWORD dwRow = lplvcd->nmcd.dwItemSpec;
	switch ( lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT :
		{
			CDC *pDC = CDC::FromHandle( pNMCD->hdc);
			RECT r = pNMCD->rc;
			pDC->FillSolidRect( &r, RGB( 46, 47, 48));

			*pResult = CDRF_NOTIFYITEMDRAW;
			break;
		}

	case CDDS_ITEMPREPAINT :
		{
			*pResult = CDRF_NOTIFYSUBITEMDRAW;
			break;
		}

	case CDDS_SUBITEM | CDDS_PREPAINT | CDDS_ITEM :
		{
			if ( m_ServerList[ dwRow].bActive)	lplvcd->clrText = RGB( 180, 180, 180);
			else								lplvcd->clrText = RGB( 100, 100, 100);

			*pResult = CDRF_DODEFAULT;
			break;
		}
	}
}

void CNewAppDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	//	int itemindex = pNMListView->iItem;
	OnBnClickedStartgame();
}

// �������� �ε� �Ϸ�
BEGIN_EVENTSINK_MAP(CNewAppDlg, CDDBDialog)
	ON_EVENT(CNewAppDlg, IDC_EXPLORER, 259, DocumentCompleteExplorer, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()


void CNewAppDlg::DocumentCompleteExplorer(LPDISPATCH pDisp, VARIANT* URL)
{
	HRESULT		hr;
	LPUNKNOWN	lpUnknown;
	LPUNKNOWN	lpUnknownWB = NULL;
	LPUNKNOWN	lpUnknownDC = NULL;

	lpUnknown = m_webPage.GetControlUnknown();
	if ( lpUnknown)
	{
		// Get the pointer to the IUnknown interface of the WebBrowser 
		// control being hosted. The pointer to the IUnknown returned from 
		// GetControlUnknown is not the pointer to the IUnknown of the 
		// WebBrowser control. It's actually a pointer to the IOleObject.
		hr = lpUnknown->QueryInterface( IID_IUnknown, (LPVOID*)&lpUnknownWB);
		if ( FAILED( hr))
			return;


		// Get the pointer to the IUnknown of the object that fired this 
		// event.
		hr = pDisp->QueryInterface( IID_IUnknown, (LPVOID*)&lpUnknownDC);
		if ( FAILED( hr))
			return;


		if ( lpUnknownWB == lpUnknownDC)
		{
			// The document has finished loading.
			m_bNavigationComplete = true;
		}


		if ( lpUnknownWB)
			lpUnknownWB->Release();

		if ( lpUnknownDC)
			lpUnknownDC->Release();
	}
}


/************************************************************************************
  ���̼����� ������
*************************************************************************************/
bool CNewAppDlg::ShowLicenseDlg()
{
	// ���̾�α׸� ���
	CNewAppLicenseConfirmDlg dlg;
	dlg.SetDialog( this, /*RaiderZ License Agreement*/ZGetString( "STR_149"), "license.htm", ZGetString( "STR_150"), /*I Agree*/ZGetString( "STR_151"), /*Deny*/ZGetString( "STR_152"));

	int nRetVal = dlg.DoModal();
	if ( nRetVal == IDOK)
		return true;


	return false;
}

void CNewAppDlg::InitStringTable()
{
	HRSRC hRes = ::FindResource( AfxGetResourceHandle(), MAKEINTRESOURCE( IDR_STRING), _T( "TXT"));
	if ( hRes != NULL)
	{
		HGLOBAL hData = ::LoadResource( AfxGetResourceHandle(), hRes);
		if ( hData != NULL)
		{
			LPVOID lpData= LockResource( hData);

			// Read string table
			STLoadFromMemory( (char*)lpData);

			FreeResource( hData);
		}
	}
}

void CNewAppDlg::CheckOSVistaAndMoreVersion()
{
	// ������ ���� Ȯ��
	char szVersion[ 128];
	m_nMajorVersion = GetOSVersion( szVersion);

	CString strBuff;
	strBuff.Format( "[APP] OS version : %s", szVersion);
	PutLog( strBuff);

	// ������ ��Ÿ���� Ư�� ����(My Documents, Program Files, WINDOWS)���� ����Ǹ� ��� ����Ѵ�
	// ��Ÿ ��� �޽����� ǥ���ϴ��� Ȯ��
	if ( m_nMajorVersion < 6) { 
		return;
	}
	
	if ( AfxGetApp()->GetProfileInt( "Flag", "DoNotDisplayVistaWarnning", 0) != 0) { 
		return;
	}


	// ���� ������ ��� ��� �������� Ȯ��
	char szCurrDir[ 512],  szPath[ 512];
	GetCurrentDirectory( sizeof( szCurrDir), szCurrDir);
	bool bInvalid = false;
	LPITEMIDLIST pidl;

	// Windows ����
	memset( szPath, 0, sizeof( szPath));
	SHGetSpecialFolderLocation( NULL, CSIDL_WINDOWS, &pidl);
	SHGetPathFromIDList( pidl, szPath);
	if ( strstr( szCurrDir, szPath) != NULL) { 
		bInvalid = true;
	}

	// Program Files ����
	memset( szPath, 0, sizeof( szPath));
	ZeroMemory( szPath, sizeof( szPath));
	SHGetSpecialFolderLocation( NULL, CSIDL_PROGRAM_FILES, &pidl);
	SHGetPathFromIDList( pidl, szPath);
	if ( strstr( szCurrDir, szPath) != NULL) { 
		bInvalid = true;
	}

	// My Documents ����
	memset( szPath, 0, sizeof( szPath));
	SHGetSpecialFolderLocation( NULL, CSIDL_PERSONAL, &pidl);
	SHGetPathFromIDList( pidl, szPath);
	if ( strstr( szCurrDir, szPath) != NULL) { 
		bInvalid = true;
	}

	// ��Ÿ ���â�� ���
	if ( bInvalid)
	{
		CNewAppWarnVista dlg;
		dlg.SetDialog( /*Warning*/ ZGetString( "STR_100"), /*In Windows Vista, RaiderZ will not operate normally if installed at current directory.\n\nIf the patch fails, we advise you to uninstall RaiderZ and reinstall to its default setting.*/ ZGetString( "STR_101"), MB_OK);
		dlg.DoModal();

		if ( dlg.GetChecked()) { 
			AfxGetApp()->WriteProfileInt( "Flag", "DoNotDisplayVistaWarnning", 1);
		}
	}
}

void CNewAppDlg::AdjustUISizeAndPosition()
{
	// ���̾�α� Ÿ��Ʋ ����
	SetWindowText( /*Game Launcher*/ ZGetString( "STR_102"));


	// ���̾�α� ��ġ �̵� �� ũ�� ����
	CRect rect; 
	GetWindowRect( &rect);
	int nWidth = rect.Width(), nHeight = rect.Height();
	rect.left = AfxGetApp()->GetProfileInt( "Window Position", "x", 50);
	rect.top  = AfxGetApp()->GetProfileInt( "Window Position", "y", 50);
	rect.right  = rect.left + nWidth;
	rect.bottom = rect.top  + nHeight;
	MoveWindow( rect, true);


	// ȭ�� �׸���
	GetScreen()->PutBitmap( 42,  20,  IDR_BMP_BKIMG, 0, 0, 694, 434);


	// �������� ��Ʈ�� �ʱ�ȭ
	if ( strcmp( ZConfig::m_strWebPageURL.c_str(), "") == 0)		m_webPage.ShowWindow( SW_HIDE);
	else										m_webPage.MoveWindow( 15, 25, -1, -1);

	#define DrawCustomBox(x,y,w,h,s)  	{\
		GetScreen()->FillTiledBitmap( x, y, w, h, GetSkin()->GetDC(), 348, 20, 19, 19);\
		GetScreen()->FillTiledBitmap( x, y, w, 19, GetSkin()->GetDC(), 348, 0, 19, 19);\
		GetScreen()->FillTiledBitmap( x, y+h-19, w, 19, GetSkin()->GetDC(), 348, 40, 19, 19);\
		GetScreen()->FillTiledBitmap( x, y, 19, h, GetSkin()->GetDC(), 328, 20, 19, 19);\
		GetScreen()->FillTiledBitmap( x+w-19, y, 19, h, GetSkin()->GetDC(), 368, 20, 19, 19);\
		GetScreen()->PutBitmap( x, y, GetSkin()->GetDC(), 328, 0, 19, 19);\
		GetScreen()->PutBitmap( x+w-19, y, GetSkin()->GetDC(), 368, 0, 19, 19);\
		GetScreen()->PutBitmap( x, y+h-19, GetSkin()->GetDC(), 328, 40, 19, 19);\
		GetScreen()->PutBitmap( x+w-19, y+h-19, GetSkin()->GetDC(), 368, 40, 19, 19);\
		GetScreen()->PutText( x+4, y+2, 17, "Arial", s, RGB( 0, 0, 0), TS_BOLD);\
		GetScreen()->PutText( x+5, y+3, 17, "Arial", s, RGB( 180, 180, 180), TS_BOLD);\
	}

	// ����Ʈ �ڽ� �ʱ�ȭ
	DrawCustomBox( 522, 30, 214, 250, "Server");
	m_cServerList.MoveWindow( 527, 54, 204, 200);
	m_cServerList.ModifyStyle( 0, LVS_SINGLESEL | LVS_SHOWSELALWAYS);
	m_cServerList.SetExtendedStyle( LVS_EX_FULLROWSELECT);
	m_ServerIconSmall.Create( 16, 16, ILC_COLOR16, 2, 1);
	m_ServerIconLarge.Create( 48, 48, ILC_COLOR16, 2, 1);
	CBitmap bmpSmall;
	bmpSmall.LoadBitmap( IDR_BMP_ICONS);
	m_ServerIconSmall.Add( &bmpSmall, RGB( 255, 0, 255));
	m_cServerList.SetImageList( &m_ServerIconSmall, LVSIL_SMALL);
	CBitmap bmpLarge;
	bmpLarge.LoadBitmap( IDR_BMP_ICONS2);
	m_ServerIconLarge.Add( &bmpLarge, RGB( 255, 0, 255));
	m_cServerList.SetImageList( &m_ServerIconLarge, LVSIL_NORMAL);
	m_cServerList.InsertColumn( 0, "Server", LVCFMT_LEFT, 204);
	m_cServerList.SetTextBkColor( RGB( 46, 47, 48));
	m_cServerList.SetTextColor( RGB( 180, 180, 180));
	SetExtend();

	// ����Ʈ �ڽ� �ʱ�ȭ
	DrawCustomBox( 522, 300, 214, 130, "Login");
	GetScreen()->PutText( 538, 343, 15, "Arial", "ID", RGB( 180, 180, 180), TS_BOLD);
	GetScreen()->PutText( 538, 383, 15, "Arial", "Password", RGB( 70, 70, 70), TS_BOLD);
	GetScreen()->PutBox( 615, 340, 105, 24, RGB( 180, 180, 180));
	GetScreen()->PutBox( 615, 380, 105, 24, RGB( 70, 70, 70));
	m_cID.MoveWindow( 616, 341, 103, 22);
	m_cPassword.MoveWindow( 616, 381, 103, 22);
	m_cPassword.ShowWindow( SW_HIDE);

	CString str;
	str = AfxGetApp()->GetProfileString( "Connect Info", "id", "");
	m_cID.SetWindowText( str);

	str = AfxGetApp()->GetProfileString( "Connect Info", "password", "******");
	m_cPassword.SetWindowText( str);


	// ������ Ȯ�� ��ư �ʱ�ȭ
	m_cExtend.InitDDBButton( IDR_BMP_EXTEND, IDR_WAV_PUSHBUTTON);
	m_cExtend.MoveWindow( 718, 35);
	m_cExtend.ShowWindow( SW_SHOW);


	// ���� ��ư �ʱ�ȭ
	m_cStartGame.InitDDBButton( IDR_BMP_STARTBTN, IDR_WAV_PUSHBUTTON);
	m_cStartGame.MoveWindow( 526, 465);
	m_cStartGame.EnableWindow( FALSE);


	// ���� ��ư �ʱ�ȭ
	m_cStop.InitDDBButton( IDR_BMP_BUTTON, IDR_WAV_PUSHBUTTON);
	m_cStop.MoveWindow( 530, 510);
	m_cStop.ShowWindow( SW_HIDE);		// �Ⱥ��̰� �����


	// �ִϸ��̼� ��Ʈ�� �ʱ�ȭ
	m_cAnimation.InitDDBAniBmp( IDR_BMP_ANIMATION, 6);
	m_cAnimation.SetTimerValue( 200);
	m_cAnimation.MoveWindow( 25, 470, 10, 10);
	m_cAnimation.EnableAnimate( FALSE);
	m_cAnimation.ShowScene( 0);
}

CWnd* CNewAppDlg::CConfigedExplorer::SetFocus()
{
	if (true == ZConfig::m_bHideWebPage)
		return NULL;
	return __super::SetFocus();
}

void CNewAppDlg::CConfigedExplorer::Navigate(LPCTSTR URL, VARIANT * Flags, VARIANT * TargetFrameName, VARIANT * PostData, VARIANT * Headers)
{
	if (true == ZConfig::m_bHideWebPage)
		return;
	return __super::Navigate(URL, Flags, TargetFrameName, PostData, Headers);
}