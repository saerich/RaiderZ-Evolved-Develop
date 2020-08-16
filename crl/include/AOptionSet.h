#pragma once

#include "AOptionSetBase.h"
#include <string>
#include <regex>

using namespace std;

template<typename T>
class AOptionSet: public AOptionSetBase
{
public:
	AOptionSet(wstring option, wstring description, T& outValue)
		: AOptionSetBase(option, description)
		, m_outValue(outValue)
	{
	}

	// bool SetArgument() - ��������. ���ø� Ư��ȭ�� �������� ���� Ÿ�� ��� �� ���忡���� ����.

private:
	T& m_outValue;
};


// �Ҹ��� �ɼ� - �ɼǰ��� ���� �ʴ´�. �ɼ� ����ġ�� �־ true ����
template<>
class AOptionSet<bool>: public AOptionSetBase
{
public:
	AOptionSet(wstring option, wstring description, bool& outValue)
		: AOptionSetBase(option, description)
		, m_outValue(outValue)
	{
		m_outValue = false;
	}

	virtual bool SetArgument(wstring arguments, wstring splitter)
	{
		// �ɼǰ��� ���ų� true, yes, enable, allow ���� ������ true. �� �� ��� false
		wregex pattern(L" *(true|t|yes|y|enable|e|allow|a)? *", regex_constants::icase);

		match_results<wstring::const_iterator> result;
		if (!regex_match(arguments, result, pattern))
		{
			return false;
		}

		m_outValue = true;
		return true;	
	}

private:
	bool& m_outValue;
};


// ���ڿ� �ɼ� - �ɼǰ��� ��ä�� �д´�.
template<>
class AOptionSet<wstring>: public AOptionSetBase
{
public:
	AOptionSet(wstring option, wstring description, wstring& outValue)
		: AOptionSetBase(option, description)
		, m_outValue(outValue)
	{
	}

	virtual bool SetArgument(wstring arguments, wstring splitter)
	{
		m_outValue = arguments;
		return true;	
	}

private:
	wstring& m_outValue;
};


// ���� �ɼ� - ���ڷ� �д´�. ���ڰ� �ƴ� �� ������ 0.
template<>
class AOptionSet<int>: public AOptionSetBase
{
public:
	AOptionSet(wstring option, wstring description, int& outValue)
		: AOptionSetBase(option, description)
		, m_outValue(outValue)
	{
		m_outValue = 0;
	}

	virtual bool SetArgument(wstring arguments, wstring splitter)
	{
		m_outValue = _wtoi(arguments.c_str());
		return true;
	}

private:
	int& m_outValue;
};


// ���ڿ� ��� �ɼ� - ���ø��� ������ ����, �ɼǰ��� �и��� �д´�.
template<>
class AOptionSet<vector<wstring> >: public AOptionSetBase
{
public:
	AOptionSet(wstring option, wstring description, vector<wstring>& outValue)
		: AOptionSetBase(option, description)
		, m_outValue(outValue)
	{
	}

	virtual bool SetArgument(wstring arguments, wstring splitter)
	{
		return Split(arguments, splitter);
	}

	virtual bool SetSplitArgument(wstring argument)
	{
		m_outValue.push_back(argument);
		return true;
	}

private:
	vector<wstring>& m_outValue;
};
