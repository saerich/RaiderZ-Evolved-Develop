// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#ifndef _WIN32_WINNT		// Windows XP �̻󿡼��� ����� ����� �� �ֽ��ϴ�.                   
#define _WIN32_WINNT 0x0501	// �ٸ� ������ Windows�� �µ��� ������ ������ ������ �ֽʽÿ�.
#endif						

#ifdef WIN32
#include <winsock2.h>
#include <Windows.h>
#include <mmsystem.h>
#endif

#include <crtdbg.h>
#include <list>
#include <map>

#include "MDebug.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")


#include <stdio.h>
#include <tchar.h>



