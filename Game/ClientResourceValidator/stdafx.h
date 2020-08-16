// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#pragma warning ( disable : 4996)

#ifndef _WIN32_WINNT		// Windows XP �̻󿡼��� ����� ����� �� �ֽ��ϴ�.                   
#define _WIN32_WINNT 0x0501	// �ٸ� ������ Windows�� �µ��� ������ ������ ������ �ֽʽÿ�.
#endif						

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <string>
#include <tchar.h>
#include <atlconv.h>
#include <atlexcept.h>

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
#include <fstream>

using namespace std;

#include "MXml.h"
#include "MFileSystem.h"
#include "MDebug.h"

#include "RTypes.h"

using namespace rs3;

#include "VDef.h"
#include "VGlobal.h"
#include "VErrorMsg.h"
