// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

// window 98 �̻�
#define _WIN32_WINNT 0x0500
#define WINVER 0x0410

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

//#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable : 4996)		// sprintf_s ...
#pragma warning (disable : 4244)	// warning C4244: '=' : 'int'���� 'float'(��)�� ��ȯ�ϸ鼭 �����Ͱ� �սǵ� �� �ֽ��ϴ�.
#pragma warning (disable : 4305)	// warning C4305: '�μ�' : 'double'���� 'float'(��)�� �߸��ϴ�.
#pragma warning (disable : 4267)	// warning C4267: '=' : 'size_t'���� 'int'(��)�� ��ȯ�ϸ鼭 �����Ͱ� �սǵ� �� �ֽ��ϴ�.


//#define _SECURE_SCL 0
//#define _HAS_ITERATOR_DEBUGGING 0

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <cstdio>
#include <cmath>
#include <tchar.h>

#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <algorithm>

// BVH ī�޶� ����Ұų� ���ų�
#define BVH_CAMERA

#include "RPreDefine.h"

// win32 ���̽�
#include <windows.h>
#include "crtdbg.h"

// ���� ���̽�
#include "MXml.h"
#include "mempool.h"
#include "MFileSystem.h"
#include "MRTTI.h"
#include "MSingleton.h"
#include "MProfiler.h"

#include "RTypes.h"
#include "RTimer.h"
#include "RConfiguration.h"
#include "RProfileInfo.h"
#include "REngine.h"
#include "RProfileInfo.h"

#include "RResource.h"
#include "RResourceManager.h"
#include "RD3DVertexUtil.h"
#include "RTechnique.h"

#include "RSceneNode.h"
#include "RCameraSceneNode.h"
#include "RSceneManager.h"

// ���� / ��ġ
#include "RDevice.h"

// �⺻ Application
#include "RApplication.h"

//�������Ϸ�
#include "RProfiler.h"

#pragma comment(lib, "atls.lib")
