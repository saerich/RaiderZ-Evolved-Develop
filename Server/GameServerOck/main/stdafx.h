// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#define _CRT_SECURE_NO_DEPRECATE

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� �����Դϴ�.

// atl
#include <atlstr.h>

// stl
#include <limits>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <regex>

// windows
#include <Winsock2.h>
#include <mswsock.h>
#include <crtdbg.h>
#include <windows.h>
#include <MMSystem.h>
#include <iomanip>

// C�� ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <time.h>

#include <fstream>

#include "tbb/task.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/atomic.h"


// ������ ��üũ�� �ʿ��ϸ� �������մϴ�.
//#define VLD_CHECK_LEAK		


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
#include "MMath.h"
#include "MVector3.h"
#include "MBox.h"
#include "MDebug.h"
#include "MThread.h"
#include "MUID.h"
#include "MXml.h"
#include "MLua.h"
#include "MSync.h"
#include "MProfiler.h"
#include "MStatisticsProfiler.h"
#include "MCrashDump.h"
#include "lua_tinker.h"
#include "MWLua.h"
#include "MFileSystem.h"
#include "MTime.h"
#include "MMatrix.h"
#include "MCodeCoverage.h"
#include "MLocale.h"
#include "MUtil.h"

// minet
#include "MiNet.h"
#include "MCommand.h"
#include "MCommandParameter.h"
#include "MCommandCommunicator.h"
#include "MCommandManager.h"
#include "MCommandDesc.h"
#include "MiNetHelper.h"
#include "MiNetCommon.h"

#include "RCollision.h"
#include "RHeightField.h"
#include "RCollisionTree.h"
#include "MPathFinder.h"

// CSCommon
#include "CSItemData.h"
#include "CStringRes.h"

// GameServer_lib
#include "STypes.h"
#include "GMath.h"
#include "GMessageID.h"
#include "GVectorMUID.h"


#pragma comment(lib,"winmm.lib")

#ifndef _DEBUG
#define _HAS_ITERATOR_DEBUGGING 0
#define _SECURE_SCL 0
#endif