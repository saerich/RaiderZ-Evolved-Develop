// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#define _CRT_SECURE_NO_DEPRECATE

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� �����Դϴ�.
#include <windows.h>
// C�� ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <time.h>

// cml
#include "MTypes.h"
#include "MDebug.h"
#include "MThread.h"
#include "MUID.h"

#include "MXml.h"

#include "Tdef.h"

#include "QuestForm.h"
#include "DialogForm.h"

using namespace QuestAndDialog_NET;
using namespace System::Data::OleDb;

ref class TQuestControllerGlobal
{
public:
	static QuestForm^			m_QuestForm;
	static DataGridViewCell^	m_dgvcSelected;
	static Object^				m_dgvcPrevSelectedValue;
};

ref class TDialogControllerGlobal
{
public:
	static DialogForm^		m_DialogForm;
	static TreeNode^		m_tnPreSelectedDialogList;
};


ref class TDBManagerGlobal
{
public:
	static DataSet^ m_dsQuest;
};

ref class TDatabaseGlobal
{
public:
	static OleDbConnection^	m_DBConnection;
};





// Leak üũ Define ----------------------------------------------
// - �����ϰ� Leak üũ�� �� VLD_CHECK_LEAK�� �������մϴ�.
//#define VLD_CHECK_LEAK
// - ���� Leak üũ�� �� M_CHECK_LEAK�� �������մϴ�.
#define M_CHECK_LEAK

#ifdef _DEBUG
#ifdef VLD_CHECK_LEAK
#include "../../../sdk/vld/vld.h"
#pragma comment(lib,"vldmt.lib")
#else
#ifdef M_CHECK_LEAK
#include "MMemoryLeak.h"
#endif
#endif
#endif // _DEBUG
// Leak üũ Define ----------------------------------------------

#pragma comment(lib, "../../../sdk/zlib/lib/zlib.lib")
#pragma comment(lib,"winmm.lib")