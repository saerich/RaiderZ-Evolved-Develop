#ifndef __MDEBUG_H
#define __MDEBUG_H

#include <Windows.h>
#include <crtdbg.h>
#include <string>
#include <vector>
#include <set>
using namespace std;

#include "MSync.h"

#define MASSERT(x) _ASSERT(x)


#ifdef _WIN32
void __cdecl MMsg(const char *pFormat,...);
#endif

DWORD MFilterException(LPEXCEPTION_POINTERS p);

void MInstallSEH();	// Compile Option�� /EHa �־����

///////////////////////////////////////////////////////////////////////////////////////////
// MDebug /////////////
enum MLogLevel
{
	MLOG_LEVEL_DEBUG			= 0,		///< ����� ����
	MLOG_LEVEL_INFORMATION		= 1,		///< ����
	MLOG_LEVEL_WARNING			= 2,		///< ���
	MLOG_LEVEL_ERROR			= 3,		///< ����
	MLOG_LEVEL_NONE
};

/// �α� ��� ���
enum MLogOutput
{
	MLOG_FILE			= 1,		///< �ؽ�Ʈ ���Ϸ� ���
	MLOG_XML			= 2,		///< xml ���Ϸ� ���
	MLOG_DEBUG			= 4,		///< �����â���� ���
	MLOG_PROG			= 8,		///< ���α׷� ��ü���� ���

	MLOG_FILED			= (MLOG_FILE | MLOG_DEBUG),
	MLOG_PROGD			= (MLOG_PROG | MLOG_DEBUG),
	MLOG_ALL			= (1|2|4|8)	///< ��ü
};

/// �ݹ� �Լ�
typedef void (MLoggerCustomOutputCallback) (const char* szText, MLogLevel nLevel);	///< Custom ���

struct MLogSysInfo
{
	string	strName;
	string	strValue;
};

/// �����忡 ������ �α� �ý���(�̱���)
class MLogger
{
private:
	char							m_szFileName[256];			///< �ؽ�Ʈ ���� �̸�
	char							m_szXmlFileName[256];		///< XML ���� �̸�
	MLogLevel						m_nLevel;					///< ��� ����
	unsigned long					m_nOutputFlags;				///< ��� ���
	bool							m_bPrintHeader;				///< ����� ������� ����
	set<string>						m_Filters;					///< �α� ���� - ���⿡ ������ ���͸� �α׸� �߰��� �����
	MLoggerCustomOutputCallback*	m_fnCustomOutputCallback;	///< ���α׷� ��ü���� ����ϱ� ���� �ݹ麯��
	char							m_szStartDate[128];			///< �α� ������ ��¥
	char							m_szStartTime[128];			///< �α� ������ �ð�
	char							m_szXslPath[256];			///< Xml�� XSL Path
	MCriticalSection				m_csLock;

	void _OutputFile(const char* szTime, const char* szText, MLogLevel nLevel);
	void _OutputXml(const char* szTime, const char* szText, MLogLevel nLevel);
	void _OutputProg(const char* szText, MLogLevel nLevel);
	void _OutputDebugString(const char* szText, MLogLevel nLevel);
	void InitFile();
	void FinalFile();
	void InitXml();
	void FinalXml();
public:
						MLogger();
						~MLogger();
	void				Init(MLogLevel nLevel=MLOG_LEVEL_DEBUG, 
							 unsigned long int nOutputFlags=MLOG_DEBUG, 
							 const char* szFileName="mlog.txt",
							 const char* szXmlFileName="mlog.xml",
							 const char* szXslPath="" );
	void				AddFilter(const char* szFilter);		// ���� �߰�
	void				Output(const char* szText, MLogLevel nLevel=MLOG_LEVEL_INFORMATION);
	void				Output(unsigned long int nOutputFlags, const char* szText, MLogLevel nLevel=MLOG_LEVEL_INFORMATION);
	void				Output(const char* szFilter, const char* szText, MLogLevel nLevel);
	void				OutputSysInfo(vector<MLogSysInfo>& vecSysInfos);	///< �ý��� ���� ���
	void				OnCrash();											///< Crash���� �� ũ���� ������ ���
	void				SetCustomOutputCallback(MLoggerCustomOutputCallback* pCallback) { m_fnCustomOutputCallback=pCallback; }
	void				SetXSLPath(const char* szPath);
	MLogLevel			GetLevel()		{ return m_nLevel; }
	void				SetPrintHeader(bool bPrintHeader)	{ m_bPrintHeader = bPrintHeader; }
};


/// �α� �ʱ�ȭ 
MLogger* MInitLog(MLogLevel nLogLevel=MLOG_LEVEL_DEBUG, 
				unsigned long int nOutputFlags=MLOG_DEBUG,
				MLoggerCustomOutputCallback* pCallback=NULL,
				const char* szFileName="mlog.txt",
				const char* szXmlFileName=NULL,
				const char* szXSLPath=NULL);
/// �α׿� ���� �߰�
void MAddLogFilter(const char* szFilter);

/// ���� �Լ��� ȣ���ϸ� MLogger�� �ܺ����� ������ ���ŵ˴ϴ�.
void MFinalizeLog();

MLogger* MGetDefaultLogger();

// �ʱ�ȭ�� ������ ��¹������ �α� ���
void __cdecl MLog0(const char* pFormat, ...);		// debug level
void __cdecl MLog1(const char* pFormat, ...);		// information level
void __cdecl MLog2(const char* pFormat, ...);		// warning level
void __cdecl MLog3(const char* pFormat, ...);		// error level

// ��¹������ ���Ƿ� ���Ͽ� �α� ���
void __cdecl MLog0(unsigned long int nOutputFlags, const char* pFormat, ...);		// debug level
void __cdecl MLog1(unsigned long int nOutputFlags, const char* pFormat, ...);		// information level
void __cdecl MLog2(unsigned long int nOutputFlags, const char* pFormat, ...);		// warning level
void __cdecl MLog3(unsigned long int nOutputFlags, const char* pFormat, ...);		// error level

// ���͸��� �� �ִ� �α� ���
void __cdecl mlog_filter(const char* szFilter, const char* pFormat, ...);

#if defined( _DEBUG ) && defined( _MLOG_DETAIL )

#define mlog	MLog1("%s(%d)", __FILE__, __LINE__); MLog1
#define mlog0	MLog0("%s(%d)", __FILE__, __LINE__); MLog0
#define mlog1	MLog1("%s(%d)", __FILE__, __LINE__); MLog1
#define mlog2	MLog2("%s(%d)", __FILE__, __LINE__); MLog2
#define mlog3	MLog3("%s(%d)", __FILE__, __LINE__); MLog3
#define mlogf	MLog1("%s(%d)", __FILE__, __LINE__); mlog_filter

#else

#define mlog	MLog1			// information level
#define mlog0	MLog0			// debug level
#define mlog1	MLog1			// information level
#define mlog2	MLog2			// warning level
#define mlog3	MLog3			// error level
#define mlogf	mlog_filter

#endif

#endif