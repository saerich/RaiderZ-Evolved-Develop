// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#define _CRT_SECURE_NO_DEPRECATE

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� �����Դϴ�.

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


// ������ ��üũ�� �ʿ��ϸ� �������մϴ�.
#define VLD_CHECK_LEAK		


#if defined(VLD_CHECK_LEAK) && defined(_DEBUG)

#define VLD_MAX_DATA_DUMP		0xFF		// vld���� ������ ������ FF Byte�� �ض�.

#include "../../../sdk/vld/vld.h"
#pragma comment(lib,"vldmt.lib")
#else
#include "MMemoryLeak.h"
#endif

#include "MMemPool.h"

#include "UnitTest++.h"

// cml
#include "MTypes.h"
#include "MDebug.h"
#include "MThread.h"
#include "MUID.h"
#include "MXml.h"
#include "MLua.h"
#include "MSync.h"
#include "MProfiler.h"
#include "MCrashDump.h"
#include "MWLua.h"

// minet
#include "MiNet.h"
#include "MCommand.h"
#include "MCommandCommunicator.h"
#include "MCommandManager.h"
#include "MCommandDesc.h"


// cscommon
#include "CServerApplication.h"


// gameserver
#include "GGlobal.h"
#include "GSystem.h"
#include "GServer.h"

#pragma comment(lib,"winmm.lib")
