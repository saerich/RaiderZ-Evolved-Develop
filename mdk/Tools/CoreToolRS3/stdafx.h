// stdafx.h : �� ������� �ʰ� ���� ����ϴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#include "CorePrecompiledHeader.h"

#include "IL/il.h"
#include "IL/ilu.h"

template<class T>
inline bool ToBool(T boolValue)
{
	return boolValue != 0;
}


#include <MXml.h>
#include <tchar.h>
#include "resource.h"
#include <afxdhtml.h>

#include "CoreTypes.h"
#include "CoreToolLib.h"

#include "C_ControlMediator.h"
#include "MDebug.h"

#include "RealSpace3.h"
#include "RTypes.h"

#define FILENAME_CONFIG		"config.xml"


//#define _DEBUG_RED_ALERT
