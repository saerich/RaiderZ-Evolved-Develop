// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#ifndef _WIN32_WINNT		// Windows XP �̻󿡼��� ����� ����� �� �ֽ��ϴ�.                   
#if (_MSC_VER >= 1900)
#define _WIN32_WINNT 0x0A00	// Windows 10
#else
#define _WIN32_WINNT 0x0501	// �ٸ� ������ Windows�� �µ��� ������ ������ ������ �ֽʽÿ�.
#endif
#endif					

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#define _CRT_SECURE_NO_DEPRECATE

// atl
#include <atlstr.h>

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

#include "MMemPool.h"
#include "MDebug.h"
#include "MProfiler.h"
#include "MXml.h"
#include "MTypes.h"
#include "MUID.h"
#include "MCodeCoverage.h"

#include "STypes.h"
#include "MTstring.h"
