// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
#pragma once

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <windows.h>
#include <MMSystem.h>
#include <shlwapi.h>
#include <math.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

#include "MTstring.h"

#include "RTimer.h"

#include "VDef.h"
#include "VGlobal.h"
#include "VGlobalApp.h"

#include "MSync.h"

using namespace rs3;

enum LOADING_STATE
{
	LS_NONE = -1,
	LS_MESH_INFO,
	LS_TALENT_INFO,
	LS_BUFF_INFO,
	LS_EFFECT_INFO,
	LS_SOUND_INFO,
	LS_CAMERA_INFO,
	LS_NPC_INFO,

	LS_MAX
};

enum VALIDATOR_STATE
{
	VS_NONE = -1,
	VS_MESH_INFO,

	VS_MAX
};

int CalLoagingCompletePercent(int nCurrentComplete, int nFullComplete, int nLoadingFullPercent = 100);

