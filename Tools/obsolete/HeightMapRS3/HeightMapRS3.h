// HeightMapRS3.h : HeightMapRS3 ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"       // �� ��ȣ


// CHeightMapRS3App:
// �� Ŭ������ ������ ���ؼ��� HeightMapRS3.cpp�� �����Ͻʽÿ�.
//

class CHeightMapRS3App : public CWinApp
{
public:
	CHeightMapRS3App();


// ������
public:
	virtual BOOL InitInstance();

// ����

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CHeightMapRS3App theApp;
