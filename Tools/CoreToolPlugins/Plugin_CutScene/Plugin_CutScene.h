// Plugin_CutScene.h : Plugin_CutScene DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CPlugin_CutSceneApp
// �� Ŭ������ ������ ������ Plugin_CutScene.cpp�� �����Ͻʽÿ�.
//

class CPlugin_CutSceneApp : public CWinApp
{
public:
	CPlugin_CutSceneApp();

	// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CPlugin_CutSceneApp theApp;