// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#ifdef WIN32
//#define _WIN32_WINNT 0x0500 //XP �̻�

#include <winsock2.h>
#include <Windows.h>
#endif

#include <iostream>
#include <tchar.h>
#include <atlconv.h>
#include <atlexcept.h>

#include <winuser.h>
#include "Shlwapi.h"

#include <crtdbg.h>
#include <list>
#include <map>
#include "MDebug.h"

#include "UnitTest++.h"
#include "TestReporter.h"

#include "RSceneManager.h"
#include "RCameraSceneNode.h"
#include "RUnitTestRunner.h"

using namespace rs3;

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
