
#pragma once

// ��Ƽ �� ���� ������ �ѱ� ������ �� ������
#if defined( _PUBLISH)  &&  defined( _KOREA)
//#define XANTIHACK
#endif


class XAntiHack;
class XAntiHackModule;
class XAntiHackAuthInfo;




// XAntiHackAuthInfo
class XAntiHackAuthInfo
{
/// Member functions
public:
	XAntiHackAuthInfo()					{}
	virtual ~XAntiHackAuthInfo()		{}
};





/// class XAntiHackModule
class XAntiHackModule
{
	friend class XAntiHack;


/// Member functions
public:
	XAntiHackModule()												{}
	virtual ~XAntiHackModule()										{}

	virtual DWORD GetLastErrorCode()								{ return 0;						}
	virtual const wchar_t* GetLastErrorMsg()						{ return NULL;					}



/// Event handler
protected:
	virtual bool OnCreate()											{ return false;					}	// ����� �����ؾ� �� �� ȣ��˴ϴ�
	virtual bool OnDestroy()										{ return false;					}	// ����� �����ؾ� �� �� ȣ��˴ϴ�
	virtual void OnCreatedWindow( HWND hWnd)						{}									// �����찡 �����Ǿ��� �� ȣ��˴ϴ�
	virtual void OnDestroyedWindow()								{}									// �����찡 �����Ǿ��� �� ȣ��˴ϴ�.
	virtual bool OnPlayerLogin( const wchar_t* szID)				{ return false;					}	// �÷��̾ �α��� ���� �� ȣ��˴ϴ�
	virtual bool OnPlayerLogout( const wchar_t* szID)				{ return false;					}	// �÷��̾ �α׾ƿ� ���� �� ȣ��˴ϴ�
	virtual bool OnUpdate()											{ return false;					}	// ����� ������Ʈ �� �� ȣ��˴ϴ�.
	virtual bool OnCheckAuthority( XAntiHackAuthInfo* pAuthInfo)	{ return false;					}	// ���� Ȯ���� ��û�� ������ ȣ��˴ϴ�
	virtual bool OnCheckedAuthority( XAntiHackAuthInfo* pAuthInfo)	{ return false;					}	// ���� Ȯ���� �Ϸ�Ǿ��� ������ ȣ��˴ϴ�
	virtual void OnShowErrorMessageBox()							{}									// ���� �޽��� �ڽ��� ��� �� ȣ��˴ϴ�
};





/// class : XAntiHack
class XAntiHack
{
/// Member variables
protected:
	HWND				m_hWnd;
	XAntiHackModule*	m_pAntiHackModule;



/// Member functions
public:
	// Constructor/Destructor
	XAntiHack();
	virtual ~XAntiHack();

	// Create/Remove anti-hack module
	bool CreateModule();
	bool RemoveModule();


	// Get singleton instance
	static XAntiHack* GetInstance();


	// Show error message box
	void ShowErrorMessageBox();


	// Created/Destroyed window
	void CreatedWindow( HWND hWnd);
	void DestroyedWindow();

	// Player login/logout
	void PlayerLogin( const wchar_t* szID);
	void PlayerLogout( const wchar_t* szID);

	// Check authority
	bool CheckAuthority( XAntiHackAuthInfo* pInfo);
	bool CheckedAuthority( XAntiHackAuthInfo* pInfo);

	// Update
	void Update();
};





/// inline : Get anti-hack instance
inline XAntiHack* XGetAntiHack()
{
	return XAntiHack::GetInstance();
}
