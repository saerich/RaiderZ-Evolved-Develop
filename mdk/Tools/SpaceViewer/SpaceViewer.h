
// SpaceViewer.h : SpaceViewer ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

// CSpaceViewerApp:
// �� Ŭ������ ������ ���ؼ��� SpaceViewer.cpp�� �����Ͻʽÿ�.
//
class CSpaceViewerApp : public CWinApp
{
public:
	CSpaceViewerApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	
	virtual BOOL OnIdle(LONG lCount);
};

extern CSpaceViewerApp theApp;
