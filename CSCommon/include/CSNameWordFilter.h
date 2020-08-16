#pragma once

class CSNameWordFilter
{
	typedef list<tstring> CSWordsList;
public:
	~CSNameWordFilter();

	void			Clear();

	bool			LoadFromFile(const TCHAR* szFileName_IllegalWords, const TCHAR* szFileName_AllowedChars);
	bool			LoadFromString(TCHAR* szString_IllegalWords, TCHAR* szString_AllowedChars);

	void			AddIllegalWord(tstring strWord);
	int				GetWordsCount()	{ return m_AllowedCharsList.size(); }

	bool			IsIllegalStr(tstring strSrcString, bool bCheckSpecialChar=false);
	bool			IsAllowedChar(tstring strSrcChar);

	static CSNameWordFilter* GetInstance();

private:
	CSNameWordFilter();

	bool			_LoadFileDB(const TCHAR* szFileName, CSWordsList& refListWords);
	void			_LoadFromStringDB(TCHAR* pStringDB, CSWordsList& refListWords);

	void			ParseLine(TCHAR*& prfBuf, TCHAR* szText);
	void			NextLine(TCHAR*& prfBuf);

	CSWordsList		m_IllegalWordsList;	///< �����ܾ� ���
	CSWordsList		m_AllowedCharsList;	///< ������ ���
};



/// �ν��Ͻ��� ����
inline CSNameWordFilter* GetNameWordFilter()
{
	return CSNameWordFilter::GetInstance();
}