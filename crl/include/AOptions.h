#pragma once

#include "crlLib.h"
#include "AOptionSet.h"

/*
	Ŀ�ǵ���� �ļ�

	�⺻����
		app.exe -log -file filename1 filename2 -path foldername

	����
		bool            isLog;
		vector<wstring> fileList;
		wstring         foldername;


		AOptions options;		
		
		options.Add(L"log",  L"�α� ��¿���",     isLog);
		options.Add(L"file", L"������ ���ϸ��", fileList);
		options.Add(L"path", L"�����ǥ ������",   foldername);


		options.Parser(commandLine);

		if (isLog)					{ �α� ó�� }
		if (0 < fileList.size())	{ ���� ó�� }
		if (!foldername.empty())	{ ���� ó�� }
*/

class AOptionSetBase;

class CRL_API AOptions
{
public:
	AOptions();
	virtual ~AOptions();

	template<typename T>
	void Add(wstring option, wstring description, T& outValue)
	{
		m_listOptions.push_back(new AOptionSet<T>(option, description, outValue));
	}

	bool Parse(wstring commandLine);
	
	bool SetSwitch(wstring newSwitch);
	bool SetSeparator(wstring newSeparator);
	bool SetSplitter(wstring newSplitter);

private:
	bool ValidCommandLine(wstring commandLine);
	bool ParseOptions(wstring commandLine);
	
	wstring MakeEscape(wstring text);
	wstring MakeNotMatch(wstring text);
	wstring MakeMatchOption(void);

private:
	list<AOptionSetBase*> m_listOptions;
	wstring m_switch;
	wstring m_separator;
	wstring m_splitter;
};
