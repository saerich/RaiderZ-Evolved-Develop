// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#define _CRT_SECURE_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� �����Դϴ�.
#include <windows.h>
// C�� ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <atlconv.h>
#include <atlexcept.h>
#include <assert.h>
// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#define DIRECTINPUT_VERSION 0x0800

//#include <dinput.h>
#include <windows.h>
#include <mmsystem.h>

#include <string>
#include <vector>
#include <map>
using namespace std;

#include "MTstring.h"
#include "MDebug.h"

#include "CSMeshInfo.h"

#include "XDef.h"

#include "TECharacter.h"

//#pragma comment(lib, "../../../sdk/zlib/lib/zlib.lib")
//
//#pragma comment(lib,"winmm.lib")
//#pragma comment(lib,"dxguid.lib")

#pragma warning( disable : 4204 )