/// @file VD_DlgCommonFileOpen.h
#pragma once
// from MSDN Magazine -- November 2004, by Paul DiLascia
// These command IDs were obtained by spelunking the File Open dialog with Spy++.
enum {
	ODM_UNDEFINED = 0,
	ODM_VIEW_ICONS = 0x7029,
	ODM_VIEW_LIST  = 0x702b,
	ODM_VIEW_DETAIL= 0x702c,
	ODM_VIEW_THUMBS= 0x702d,
	ODM_VIEW_TILES = 0x702e,
};

//////////////////
// This class is used to subclass the "SHELLDLL_DefView" window in the file
// open dialog.
//
class CListViewShellWnd : public CWnd
{
public:
	// These are the WM_COMMAND IDs to send to the shell window to set the
	// different folder views. These were obtained using Spy++ to examine
	// which messages are sent to the shell window.
	//
	DWORD m_lastViewMode;

	CListViewShellWnd() : m_lastViewMode(0)
	{
	}

	~CListViewShellWnd()
	{
	}

	BOOL SetViewMode(int cmd);

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_DYNAMIC(CListViewShellWnd)
	DECLARE_MESSAGE_MAP()
};

/////////////////
// Customized file open dialog remembers state of folder view.
//

/** @brief ���� ���� ���̾�α� */
class CPersistOpenDlg : public CFileDialog {
public:
	CPersistOpenDlg();
	~CPersistOpenDlg();

protected:
	CListViewShellWnd m_wndListViewShell;

	virtual BOOL OnInitDialog();			 // handle init dialog
	afx_msg LRESULT OnPostInit(WPARAM wp, LPARAM lp); // the REAL initialization

	DECLARE_DYNAMIC(CPersistOpenDlg)
	DECLARE_MESSAGE_MAP()
};

/////////////
// ���ο� ������ ���̾�α׸� �߰���ų���� ���⼭����
//
/** @brief �Ϲ����� ����Ʈ �� ���Ͽ���*/
class CListViewOpenDlg : public CPersistOpenDlg {
public:
	CListViewOpenDlg(const char* _lpstr) {
		m_ofn.Flags |= OFN_NOCHANGEDIR;
		m_ofn.lpstrInitialDir = _lpstr;
	}

	void SetTitle(const char* _szTitle){ m_ofn.lpstrTitle = _szTitle; }
	void SetFilter(const char* _szFilter){ m_ofn.lpstrFilter = _szFilter; }
	void AddStyle(DWORD _dwStyle){ m_ofn.Flags |= _dwStyle ; }
	~CListViewOpenDlg(){}

protected:
	afx_msg LRESULT OnPostInit(WPARAM wp, LPARAM lp); // the REAL initialization

	DECLARE_DYNAMIC(CListViewOpenDlg)
	DECLARE_MESSAGE_MAP()
};

/** @brief xml ���Ͽ���*/
class CXMLOpenDlg : public CPersistOpenDlg {
public:
	CXMLOpenDlg(const char* _initDir, const char* _szFilter){
		m_ofn.lpstrTitle = _T("CoreToolRS - XML ������ ������ �ּ���");
		//ȭ�鿡 ���� ���͸�\0�������� / �߰��Ҷ��� \0�� �ٿ��� �տ��� �ݺ�
		m_ofn.lpstrFilter = _szFilter;
		//������ ���ٰ� �ؼ� ������ġ �ȹٲٱ�
		m_ofn.Flags |= OFN_NOCHANGEDIR;
		m_ofn.lpstrInitialDir = _initDir;
	}
	~CXMLOpenDlg(){}

protected:
	afx_msg LRESULT OnPostInit(WPARAM wp, LPARAM lp); // the REAL initialization

	DECLARE_DYNAMIC(CXMLOpenDlg)
	DECLARE_MESSAGE_MAP()
};

/** @brief bmp Ÿ�� ���Ͽ���*/
class CBMPOpenDlg : public CPersistOpenDlg {
public:
	CBMPOpenDlg(const char* _lpstr, const char *_lpfilter = "��Ʈ�� ����\0*.bmp") {
		m_ofn.lpstrTitle = _T("CoreToolRS - ��Ʈ�� ������ ������ �ּ���");
		//ȭ�鿡 ���� ���͸�\0��������
		m_ofn.lpstrFilter = _lpfilter;
		//������ ���ٰ� �ؼ� ������ġ �ȹٲٱ�
		m_ofn.Flags |= OFN_NOCHANGEDIR;
		m_ofn.lpstrInitialDir = _lpstr;
	}
	~CBMPOpenDlg(){}

protected:
	afx_msg LRESULT OnPostInit(WPARAM wp, LPARAM lp); // the REAL initialization

	DECLARE_DYNAMIC(CBMPOpenDlg)
	DECLARE_MESSAGE_MAP()
};
