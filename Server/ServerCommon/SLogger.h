#pragma once

#define FILENAME_LOG				L"mlog.txt"
#define LOG_FILE_HEADER				L"mlog"

/// �α� ���� Ŭ���� 
class SLogger
{
private:
	static void AddPathSeperator( wstring& strOut );
	static wstring MakeFileName( wstring strLogPath );
public:
	static void Init(MLogLevel nLogLevel, wstring strLogPath);
};