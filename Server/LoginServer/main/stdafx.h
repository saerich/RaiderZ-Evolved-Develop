// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#define _CRT_SECURE_NO_DEPRECATE

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� �����Դϴ�.

#include <atlstr.h>

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

// C�� ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <time.h>


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


// cml
#include "MTypes.h"
#include "MDebug.h"
#include "MMath.h"
#include "MThread.h"
#include "MUID.h"
#include "MXml.h"
#include "MSync.h"
#include "MCrashDump.h"
#include "MFileSystem.h"
#include "MTime.h"
#include "MCodeCoverage.h"

// minet
#include "MiNet.h"
#include "MCommand.h"
#include "MCommandParameter.h"
#include "MCommandCommunicator.h"
#include "MCommandManager.h"
#include "MCommandDesc.h"
#include "MiNetHelper.h"
#include "MiNetCommon.h"

// CSCommon
#include "CCommandTable.h"
#include "CCommandResultTable.h"
#include "CStringRes.h"
#include "AStlUtil.h"

// LoginServer
#include "STypes.h"


#pragma comment(lib,"winmm.lib")
