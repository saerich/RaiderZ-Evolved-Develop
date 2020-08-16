// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
//

#pragma once

#ifndef _WIN32_WINNT		// Windows XP 이상에서만 기능을 사용할 수 있습니다.                   
#if (_MSC_VER >= 1900)
#define _WIN32_WINNT 0x0A00	// Windows 10
#else
#define _WIN32_WINNT 0x0501	// 다른 버전의 Windows에 맞도록 적합한 값으로 변경해 주십시오.
#endif
#endif	

#define _CRT_SECURE_NO_DEPRECATE

#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일입니다.

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
#include <mmsystem.h>

// C의 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <time.h>

#include "MDebug.h"

#include <tchar.h>
#include <atlconv.h>
#include <atlexcept.h>


// 정밀한 릭체크가 필요하면 디파인합니다.
//#ifdef _DEBUG
//#define VLD_CHECK_LEAK		
//#endif
//
//#if defined(VLD_CHECK_LEAK) && defined(_DEBUG)
//#define VLD_MAX_DATA_DUMP		0xFF		// vld에서 데이터 덤프는 FF Byte만 해랑.
//#include "../../../sdk/vld/vld.h"
//#pragma comment(lib,"vldmt.lib")
//#else
//#include "MMemoryLeak.h"
//#endif
//
//#pragma comment(lib,"winmm.lib")
