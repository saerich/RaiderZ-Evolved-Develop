// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#define _CRT_SECURE_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#ifdef WIN32
#include <winsock2.h>
#include <Windows.h>
#include <mmsystem.h>
#endif

#include <crtdbg.h>
#include <list>
#include <map>

#include "MDebug.h"
#include "MiNetCommon.h"
#include "MCommand.h"
#include "MCommandParameter.h"
#include "MiNetLogger.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")
