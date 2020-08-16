#include "stdafx.h"
#include "AOptions.h"
#include <sstream>

AOptions::AOptions()
: m_switch(L"-")
, m_separator(L" ")
, m_splitter(L" ")
{
	// do nothing
}

AOptions::~AOptions()
{
	for each(AOptionSetBase* optionSet in m_listOptions)
	{
		delete optionSet;
	}
}

bool AOptions::Parse(wstring commandLine)
{
	if (!ValidCommandLine(commandLine))
	{
		return false;
	}

	return ParseOptions(commandLine);
}

bool AOptions::SetSwitch(wstring newSwitch)
{
	// EmtpySwitch ��� - commandLine ��ü�� �ϳ��� �ɼ����� ����ϰԵȴ�.
	m_switch = MakeEscape(newSwitch);
	return true;
}

bool AOptions::SetSeparator(wstring newSeparator)
{
	if (newSeparator.empty())	return false;

	m_separator = MakeEscape(newSeparator);

	return true;
}

bool AOptions::SetSplitter(wstring newSplitter)
{
	if (newSplitter.empty())	return false;

	m_splitter = MakeEscape(newSplitter);

	return true;
}

bool AOptions::ValidCommandLine(wstring commandLine)
{
	wstringstream maker;
	maker	<< "("
				<< "(^| +)"					// �� �ɼ��� �������� ����
				<< MakeMatchOption()	
			<< ")*"
			<< " *";						// l-trim

	wregex commandLinePattern(maker.str());

	match_results<wstring::const_iterator> commandResult;
	if (!regex_match(commandLine, commandResult, commandLinePattern))
	{
		return false;
	}

	return true;
}

bool AOptions::ParseOptions(wstring commandLine)
{
	wregex optionPattern(MakeMatchOption());

	wsregex_token_iterator begin(commandLine.begin(), commandLine.end(), optionPattern), end;
	for(wsregex_token_iterator it = begin; it != end; it++)
	{
		wstring token = *it;

		match_results<wstring::const_iterator> optionResult;
		if (!regex_match(token, optionResult, optionPattern))
		{
			return false;
		}

		wstring option = optionResult[1].str();
		wstring arguments = optionResult[2].str();

		for each(AOptionSetBase* optionSet in m_listOptions)
		{
			if (!optionSet->Match(option))	continue;				
			if (!optionSet->SetArgument(arguments, m_splitter))	return false;				
		}
	}

	return true;
}

wstring AOptions::MakeEscape(wstring text)
{
	wstring escapeText = text;

	wstring::size_type pos = 0;
	for(;;)
	{			
		// default regex ordinary character (ECMAScript)
		pos = escapeText.find_first_of(L"^$\\.*+?()[]{}|", pos);	

		if (pos == wstring::npos)
		{
			break;
		}

		escapeText.insert(pos, L"\\");
		pos += 2;
	}

	return escapeText;
}

wstring AOptions::MakeNotMatch(wstring text)
{	
	if (text.empty())
	{
		return L"";
	}

	// look-around ����
	return L"(?!" + text + L")";
}

wstring AOptions::MakeMatchOption(void)
{
	wstringstream maker;		
	maker	<< m_switch									// �ɼ� ����ġ
			<< "("
				<< "(?:"
					<< MakeNotMatch(m_switch)
					<< MakeNotMatch(m_separator)
				<< ".)+"
			<< ")"
			<< "(?:"									// �ɼǰ� ����
				<< m_separator							
				<< " *"
				<< "("									// ù �ɼǰ�
					<< "(?:"
						<< MakeNotMatch(m_switch)
						<< MakeNotMatch(m_splitter)
					<< ".)+"
					<< "(?:"							// ������ �ɼǰ���
					<< m_splitter
						<< "(?:"
							<< MakeNotMatch(m_switch)
							<< MakeNotMatch(m_splitter)
						<< ".)+"
					<< ")*"
				<< ")"
			<< ")?";

	return maker.str();
}
