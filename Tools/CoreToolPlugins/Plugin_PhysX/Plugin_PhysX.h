// Plugin_PhysX.h : Plugin_PhysX DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CPlugin_PhysXApp
// �� Ŭ������ ������ ������ Plugin_PhysX.cpp�� �����Ͻʽÿ�.
//

class CPlugin_PhysXApp : public CWinApp
{
public:
	CPlugin_PhysXApp();

	// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CPlugin_PhysXApp theApp;
