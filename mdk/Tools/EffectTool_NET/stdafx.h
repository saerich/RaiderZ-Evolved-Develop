// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
#pragma once

#pragma warning ( disable : 4146)
#pragma warning ( disable : 4793)
#pragma warning ( disable : 4996)

#using <mscorlib.dll>

#include <windows.h>

#include <atlstr.h>

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

#include "MXml.h"
//#include "Global.h"

#include "RActorNode.h"
#include "RMeshNode.h"
#include "MDebug.h"
#include "MFileSystem.h"

// Leak üũ Define ----------------------------------------------
// - �����ϰ� Leak üũ�� �� VLD_CHECK_LEAK�� �������մϴ�.
//#define VLD_CHECK_LEAK
// - ���� Leak üũ�� �� M_CHECK_LEAK�� �������մϴ�.
// #define M_CHECK_LEAK
// 
// #ifdef _DEBUG
// #ifdef VLD_CHECK_LEAK
// #include "../../../sdk/vld/vld.h"
// #pragma comment(lib,"vldmt.lib")
// #else
// #ifdef M_CHECK_LEAK
// #include "MMemoryLeak.h"
// #endif
// #endif
// #endif // _DEBUG
// // Leak üũ Define ----------------------------------------------


#ifdef _DEBUG
#pragma comment(lib, "atlsd.lib")
#else
#pragma comment(lib, "atls.lib")
#endif
