// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
#pragma once

#pragma warning ( disable : 4146)
#pragma warning ( disable : 4793)
#pragma warning ( disable : 4996)

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// C RunTime Header Files

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <windows.h>

#include "Form1.h"
#include "Global.h"
// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "TDef.h"
#include "MFileSystem.h"

#include "TConst.h"
// [11/16/2007 madduck] �ϴ� ���� �ǵ��� �߰�. 
//#include "MWLua.h"
//#include "XDef.h"

//using namespace Character_NET;
//
//ref class GlobalObjects
//{
//public:
//	static Form1^ g_pMainForm;
//	static Global^ g_pGlobal;
//};

// Leak üũ Define ----------------------------------------------
// - �����ϰ� Leak üũ�� �� VLD_CHECK_LEAK�� �������մϴ�.
//#define VLD_CHECK_LEAK
// - ���� Leak üũ�� �� M_CHECK_LEAK�� �������մϴ�.

//#define M_CHECK_LEAK
//
//#ifdef _DEBUG
//#ifdef VLD_CHECK_LEAK
//#include "../../../sdk/vld/vld.h"
//#pragma comment(lib,"vldmt.lib")
//#else
//#ifdef M_CHECK_LEAK
//#include "MMemoryLeak.h"
//#endif
//#endif
//#endif // _DEBUG
// Leak üũ Define ----------------------------------------------

