
// RS3Analyzer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CRS3Analyzer:
// �� Ŭ������ ������ ���ؼ��� RS3Analyzer.cpp�� �����Ͻʽÿ�.
//

class CRS3Analyzer : public CWinAppEx
{
public:
	CRS3Analyzer();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CRS3Analyzer theApp;