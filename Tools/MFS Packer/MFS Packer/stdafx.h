#pragma once
// stdafx.h: often used but not changed often
// Standard system include files and project related include files
// This is the containing file.
//

#pragma once

#define _CRT_SECURE_NO_DEPRECATE

#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

// Windows header file.
#include <windows.h>

// C's runtime header file.
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

//Additional headers required by the program are referenced here.
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <mmsystem.h>

#include "MProfiler.h"
#include "MStatisticsProfiler.h"

#include "MTime.h"

#pragma warning( disable : 4204 )
