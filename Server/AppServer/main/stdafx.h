// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#define _CRT_SECURE_NO_DEPRECATE

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� �����Դϴ�.

#include <atlstr.h>
#include <tchar.h>
#include <atlconv.h>
#include <atlexcept.h>

#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <string>

#include <Winsock2.h>
#include <mswsock.h>
#include <crtdbg.h>
#include <windows.h>

#include <tchar.h>
#include <atlconv.h>
#include <atlexcept.h>


// C�� ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <time.h>

#include "MDebug.h"
#include "MCommand.h"
#include "CCommandResultTable.h"


// ������ ��üũ�� �ʿ��ϸ� �������մϴ�.
//#ifdef _DEBUG
//#define VLD_CHECK_LEAK		
//#endif
//
//#if defined(VLD_CHECK_LEAK) && defined(_DEBUG)
//#define VLD_MAX_DATA_DUMP		0xFF		// vld���� ������ ������ FF Byte�� �ض�.
//#include "../../../sdk/vld/vld.h"
//#pragma comment(lib,"vldmt.lib")
//#else
#include "MMemoryLeak.h"
//#endif

#pragma comment(lib,"winmm.lib")


