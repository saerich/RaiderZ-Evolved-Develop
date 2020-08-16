#pragma once

#include "CCommandResultTable.h"

class CSNameStringChecker
{
public:
	enum TYPE
	{
		TYPE_CHARACTER = 1,
		TYPE_GUILD = 2,
	};

	enum LENGTH_LIMIT
	{
		CHARACTER_MIN_HANGUL = 2,
		CHARACTER_MAX_HANGUL = 8,
		CHARACTER_MIN_ENGLISH = 3,
		CHARACTER_MAX_ENGLISH = 10,

		GUILD_MIN_HANGUL = 4,
		GUILD_MAX_HANGUL = 16,
		GUILD_MIN_ENGLISH = 6,
		GUILD_MAX_ENGLISH = 20,
	};

	enum ERROR_CODE
	{
		OK								= CR_SUCCESS,
		ERROR_LENGTH_TOO_SHORT			= CR_FAIL_SYSTEM_TOO_SHORT_NAME_LEN,			// �̸��� �ʹ� ª���ϴ�.
		ERROR_LENGTH_TOO_LARGE			= CR_FAIL_SYSTEM_TOO_LONG_NAME_LEN,				// �̸��� �ʹ� ��ϴ�.
		ERROR_CONTAIN_NUMBER_OR_SPACE	= CR_FAIL_SYSTEM_CONTAIN_NUMBER_OR_SPACE,		// �̸��� ���ڳ� ������ �� �� �����ϴ�.
		ERROR_CONTAIN_INVALID_CHARACTER = CR_FAIL_SYSTEM_CONTAIN_INVALID_CHARACTER,		// �̸��� ����� ���ڳ� Ư�����ڰ� �� �� �����ϴ�.
		ERROR_MIXING_LANGUAGE			= CR_FAIL_SYSTEM_MIXING_LANGUAGE,				// �̸��� ����� �ѱ��� ȥ���� ����� �� �����ϴ�.
		ERROR_NUMBER_ONLY				= CR_FAIL_SYSTEM_NUMBER_ONLY,					// �̸��� ���ڸ����� ������ �� �����ϴ�.
		ERROR_INVALID_WORD				= CR_FAIL_SYSTEM_NAME_INCLUDE_INVALID_STRING,	// �̸��� ����� �� ���� �ܾ ���ԵǾ��ֽ��ϴ�.
		ERROR_CONTAIN_ALPHABETLESS		= CR_FAIL_SYSTEM_CONTAIN_ALPHABETLESS,			// �̸��� Ư����ȣ�� ����� �� �����ϴ�.
	};

public:
	CSNameStringChecker(const wchar_t* pszWideWord, TYPE nType = TYPE_CHARACTER);
	virtual ~CSNameStringChecker(void);

	ERROR_CODE Check() const;
	
protected:
	bool Check_LengthShort() const;
	bool Check_LengthLarge() const;
	bool Check_NumberAndSpace() const;
	bool Check_CpHangul() const;
	bool Check_MixingLanguage() const;
	bool Check_NumberOnly() const;
	bool Check_Alphabetless() const;
	bool Check_InvalidCharacter() const;
	bool CheckAbuseWords() const;
	bool CheckGameContentsWords() const;

	LENGTH_LIMIT GetLengthLimit_MinHangul() const;
	LENGTH_LIMIT GetLengthLimit_MaxHangul() const;
	LENGTH_LIMIT GetLengthLimit_MinEnglish() const;
	LENGTH_LIMIT GetLengthLimit_MaxEnglish() const;	

protected:
	wstring m_strWord;
	TYPE	m_nType;
};
