// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PMSCONN_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PMSCONN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef PMSCONN_EXPORTS
#define PMSCONN_API __declspec(dllexport)
#else
#define PMSCONN_API __declspec(dllimport)
#endif

#include "IPMSObject.h"
#include "PMSCodes.h"
/*
PMSInitConn : PMSConn �ʱ�ȭ�� �ϱ� ���� �Լ�, Console ����϶��� main�Լ��� ���ڸ� , Service Mode�϶��� ServiceMain �Լ��� ���ڸ� ����
@param1 : ���� ���� ����
@param2 : ���������� ������ �迭
@return value : PMSCONN_ERRORCODE ����
*/
extern PMSCONN_API	DWORD	PMSInitConn(DWORD argc, LPSTR argv[]);	// LPTSTR -> LPSTR�� �����մϴ�. �����ڵ� ������ ��ũ�� �ȵǳ׿�. [2010/6/9 praptor]
/*
PMSRunConn : PMSConn�� ���������� ���� ��Ű�� �Լ�
@param1 : Call Back������ IPMSObject ������ IPMSObject�� IPMSObject���� ���� ����
@return value : PMSCONN_ERRORCODE ����
*/
extern PMSCONN_API  DWORD	PMSRunConn(IPMSObject *pObj);
/*
PMSStopConn : PMSConn�� ���� �ϱ� ���� �Լ�
*/
extern PMSCONN_API  void	PMSStopConn();
/*
PMSSendWarningMsg : PMS�� Warning �޽����� ������ ���� �Լ�
@param1 : ��� ���� FAULT_LEVEL ����
@param2 : ��� ����
@param3 : ��� ó�� ���
@param4 : ��� �߻� ���� Serial
@param5 : ��� �߻� ī�װ�
*/
extern PMSCONN_API	BOOL	PMSSendWarningMsg(DWORD dwErrLvl, LPCSTR pszWarningMsg, LPCSTR pszTreatMsg, DWORD dwSSN, DWORD dwCategory);

/*
PMSGetConfigFileName PMS�� �Ѱ��� ���� ���� Config ������ �̸��� ���� ���� �Լ�
@return value PMS������ /CNFGFILE:xxx.xxx �� xxx.xxx���� ���� 
*/
extern PMSCONN_API	LPCSTR	PMSGetConfigFileName();		// LPCTSTR -> LPCSTR �����ڵ� ������ ��ũ�ȵǼ� �����մϴ�. [2010/6/9 praptor]
/*
PMSGetStatus PMSConn�� ���� ���¸� ���� ���� ���� �Լ�
@return value : PMSCONN_STATUS_CODE ���� ����  
*/
extern PMSCONN_API	DWORD	PMSGetStatus();
