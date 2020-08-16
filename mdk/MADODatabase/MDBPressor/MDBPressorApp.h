// MDBPressor.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// MDBPressorApp:
// �� Ŭ������ ������ ���ؼ��� MDBPressor.cpp�� �����Ͻʽÿ�.
//

class MDBPressorDlg;

class MDBPressorApp : public CWinApp
{
public:
	MDBPressorApp();

// �������Դϴ�.
	public:
	virtual BOOL	InitInstance();
	virtual int		ExitInstance();
	virtual int		Run();

// �����Դϴ�.
	void			HeartBeat();
	BOOL			IsShutdown()	{ return m_bShutdown;	}
	void			Shutdown()		{ m_bShutdown = TRUE;	}

	DECLARE_MESSAGE_MAP()

private:
	MDBPressorDlg*	m_dlg;
	BOOL			m_bShutdown;
};

extern MDBPressorApp theApp;