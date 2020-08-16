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

#include "MDatabase.h"

#include <Winsock2.h>
#include <mswsock.h>
#include <crtdbg.h>
#include <windows.h>

// C�� ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <time.h>


#include "gmock/gmock.h"

// googlemock�� �浹���� undef
#ifdef TEST
#undef TEST		
#endif

#include "UnitTest++.h"

#include "MCommandDesc.h"
#include "MCommandParameter.h"

#include "STypes.h"

#include "MDebug.h"

using namespace minet;

// ������ ��üũ�� �ʿ��ϸ� �������մϴ�.
#ifdef _DEBUG
#define VLD_CHECK_LEAK		
#endif


#pragma comment(lib,"winmm.lib")
