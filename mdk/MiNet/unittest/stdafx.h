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

#include "UnitTest++.h"

#define VLD_MAX_DATA_DUMP		0xFF		// vld���� ������ ������ FF Byte�� �ض�.

#include "../../../sdk/vld/vld.h"
#pragma comment(lib,"vldmt.lib")


#pragma comment(lib,"winmm.lib")

// cml
#include "MTypes.h"
#include "MDebug.h"
#include "MMath.h"
#include "MThread.h"
#include "MUID.h"
#include "MXml.h"
#include "MSync.h"
#include "MTime.h"

// minet
#include "MiNet.h"
#include "MCommand.h"
#include "MCommandParameter.h"
#include "MCommandCommunicator.h"
#include "MCommandManager.h"
#include "MCommandDesc.h"
#include "MiNetHelper.h"
#include "MiNetCommon.h"


#include "FCommandDesc.h"

using namespace minet;

