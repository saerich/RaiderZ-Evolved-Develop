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
using namespace std;

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <windows.h>
#include <mmsystem.h>

#include "MUID.h"
#include "MRTTI.h"
#include "MFileSystem.h"
#include "MWLua.h"
#include "MUtil.h"
#include "MDebug.h"
#include "MBlobArray.h"
#include "FileInfo.h"
#include "MMemPool.h"
#include "MProfiler.h"
#include "MCodeCoverage.h"
#include "MTime.h"

#include "CSDef.h"
#include "CTransData.h"

#include "RTypes.h"
#include "RFont.h"
#include "RRenderHelper.h"
#include "RApplication.h"
#include "RProfileInfo.h"
#include "Mint.h"
#include "Mint4R3.h"
#include "MWidget.h"

#include "UnitTest++.h"
#include "MTest.h"

#include "MiNet.h"
#include "MiNetCommon.h"
#include "MiNetCommonDef.h"
#include "MCommand.h"
#include "CCommandTable.h"

#include "AStlUtil.h"

#include "XMath.h"
#include "XGlobal.h"
#include "XGlobalVar.h"
#include "XScript.h"
#include "XUtil.h"

using namespace rs3;
using namespace mint3;
using namespace minet;
//using namespace physx;


#pragma warning( disable : 4204 )

