
// StringCollector.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CStringCollectorApp:
// �� Ŭ������ ������ ���ؼ��� StringCollector.cpp�� �����Ͻʽÿ�.
//

class CStringCollectorApp : public CWinAppEx
{
public:
	CStringCollectorApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CStringCollectorApp theApp;