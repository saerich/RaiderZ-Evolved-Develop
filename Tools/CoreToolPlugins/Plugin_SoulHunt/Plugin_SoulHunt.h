// Plugin_SoulHunt.h : Plugin_SoulHunt DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CPlugin_SoulHuntApp
// �� Ŭ������ ������ ������ Plugin_SoulHunt.cpp�� �����Ͻʽÿ�.
//

class CPlugin_SoulHuntApp : public CWinApp
{
public:
	CPlugin_SoulHuntApp();

	// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CPlugin_SoulHuntApp theApp;