
// MakeCRC.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMakeCRCApp:
// �� Ŭ������ ������ ���ؼ��� MakeCRC.cpp�� �����Ͻʽÿ�.
//

class CMakeCRCApp : public CWinAppEx
{
public:
	CMakeCRCApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMakeCRCApp theApp;