
// PackageValidator.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CPackageValidatorApp:
// �� Ŭ������ ������ ���ؼ��� PackageValidator.cpp�� �����Ͻʽÿ�.
//

class CPackageValidatorApp : public CWinAppEx
{
public:
	CPackageValidatorApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CPackageValidatorApp theApp;