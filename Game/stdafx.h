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

#include "XAntiHack.h"				// Anti-hack module
#include "XAntiHackModuleGameGuardKor.h"

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
#include "MStatisticsProfiler.h"
#include "MCodeCoverage.h"

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
#include "MTime.h"

#include "UnitTest++.h"
#include "MTest.h"

#include "XScreenDebugger.h"
#include "XUtil.h"
#include "XGlobal.h"
#include "XGlobalVar.h"
#include "XCommandTable.h"
#include "XNetClient.h"
#include "XScript.h"
#include "XVirtualKeyDef.h"
#include "XKeyCodes.h"
#include "XKeyMapTable.h"
#include "XGameInput.h"
#include "XChatCmds.h"
#include "XHelloWorldFrame.h"
#include "XTestFrame.h"
#include "XStrCmd.h"
#include "XTimer.h"
#include "XItemData.h"
#include "XRenderHelper.h"
#include "Mint4Game.h"
#include "XConfig.h"
#include "XQuestInfo.h"
#include "XTerrain.h"
#include "XWorldEnv.h"
#include "XRealSound.h"
#include "XEvent.h"
#include "XModule.h"
#include "XActor.h"
#include "XNetPlayer.h"
#include "XUISystem.h"
#include "XCollision.h"
#include "XMath.h"
#include "XSTLUtil.h"
#include "XObjectManager.h"
#include "XMotionMgr.h"
#include "MLocale.h"
#include "CSFormatString.h"

using namespace rs3;
using namespace mint3;
using namespace minet;
//using namespace physx;


#pragma warning( disable : 4204 )

