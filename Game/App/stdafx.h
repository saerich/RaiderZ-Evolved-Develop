// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable : 4996)		// sprintf_s ...
#pragma warning (disable : 4244)	// warning C4244: '=' : 'int'���� 'float'(��)�� ��ȯ�ϸ鼭 �����Ͱ� �սǵ� �� �ֽ��ϴ�.
#pragma warning (disable : 4305)	// warning C4305: '�μ�' : 'double'���� 'float'(��)�� �߸��ϴ�.
#pragma warning (disable : 4267)	// warning C4267: '=' : 'size_t'���� 'int'(��)�� ��ȯ�ϸ鼭 �����Ͱ� �սǵ� �� �ֽ��ϴ�.

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� �����Դϴ�.
#include <windows.h>
// C�� ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>

#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <queue>
#include <algorithm>
#include <cctype>
#include "MTstring.h"

using namespace std;

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <windows.h>
#include <mmsystem.h>

#include "MProfiler.h"
#include "MStatisticsProfiler.h"

#include "CSDef.h"
#include "CTransData.h"

#include "MTime.h"

#include "UnitTest++.h"
#include "MTest.h"

#include "MiNet.h"

#pragma warning( disable : 4204 )

