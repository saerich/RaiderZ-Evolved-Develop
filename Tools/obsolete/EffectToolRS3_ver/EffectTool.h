// EffectTool.h : EffectTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"       // �� ��ȣ
#include <GdiPlus.h>

#pragma comment ( lib, "gdiplus.lib")

#include "ControlView.h"
#include "RView.h"

// CEffectToolApp:
// �� Ŭ������ ������ ���ؼ��� EffectTool.cpp�� �����Ͻʽÿ�.
//

class CEffectToolApp : public CWinApp
{
public:
	CEffectToolApp();


// ������
public:
	virtual BOOL InitInstance();

// ����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CEffectToolApp theApp;
