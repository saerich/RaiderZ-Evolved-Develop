// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE Ŭ�����Դϴ�.
#include <afxodlgs.h>       // MFC OLE ��ȭ ���� Ŭ�����Դϴ�.
#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC �����ͺ��̽� Ŭ�����Դϴ�.
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO �����ͺ��̽� Ŭ�����Դϴ�.
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����
