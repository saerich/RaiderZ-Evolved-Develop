/************************************************************************
  CSStringFilter

  desc : Gunz �� �ִ� MChattingFilter�� �����Ͽ� ���ְ̹� �������̽��� Raiderz �� �°� �����մϴ�.
  date : 2010-02-25
  name : ��â��
*************************************************************************/

#include "stdafx.h"
#include "CSStringFilter.h"
#include "MFileSystem.h"
#include "MLocale.h"
#include <regex>
#include "AStlUtil.h"
#include "CSDef.h"
#include <MMemoryProxy.h>

const int MAX_ABUSE_WORD_LEN = 256;

/// ��Ӿ� ���̿� ������ ���� ���ڵ�
const tstring CSStringFilter::ms_CheatingChar = _T(" `~!@#$%^&*()-_=+\\|[{]};:'\",<.>/?1");

CSStringFilter::CSStringFilter()
: m_pRoot(NULL)
{
}

CSStringFilter::~CSStringFilter()
{
	Clear();
}

void CSStringFilter::Clear()
{
	SAFE_DELETE(m_pRoot);
}

bool CSStringFilter::LoadFromFile(const TCHAR* szFileName)
{	
	if (NULL == szFileName)
		return false;

	MFile mzf;
	if ( !mzf.Open(MLocale::ConvTCHARToAnsi(szFileName).c_str())) 
		return false;

	char* buffer;	
	buffer = new char[ mzf.GetLength() + 1];
	mzf.Read( buffer, mzf.GetLength());
	buffer[ mzf.GetLength()] = NULL;
	
	tstring strTempBuf;

#ifdef _UNICODE
	strTempBuf = MLocale::ConvAnsiToUCS2(buffer);
#else
	strTempBuf = buffer;
#endif

	LoadFromStringDB((TCHAR*)strTempBuf.c_str());

	mzf.Close();
	delete [] buffer;

	return true;
}

void CSStringFilter::LoadFromStringDB( TCHAR* pStringDB)
{
	if (NULL == pStringDB) return;

#ifdef _DEBUG
	FILE* fp = _tfopen( _T("abuse_db.txt"), _T("w"));
#endif

	while ( 1)
	{
		TCHAR szText[ 256]	= {0, };

		ParseLine( pStringDB, szText);

		if ( (int)_tcslen( szText) == 0)		continue;
		if (_tcsicmp( szText, _T("END")) == 0)	break;

		AddAbuseWord( szText );

#ifdef _DEBUG
		_ftprintf( fp, _T("insert into abuselist(workd) values('%%%s%%')\n")
			, szText );
#endif

		NextLine( pStringDB);
	}

#ifdef _DEBUG
	fclose( fp );
#endif
}

void CSStringFilter::ParseLine( TCHAR*& prfBuf, TCHAR* szText)
{
	int  nTextCount = 0;

	*szText = 0;

	while ( 1)
	{
		TCHAR ch = *prfBuf++;

		if ( (ch == 0) || (ch == _T('\n')) || (ch == _T('\r')))
			break;

		*(szText + nTextCount++) = ch;
		*(szText + nTextCount) = 0;
	}
}


bool CSStringFilter::AddAbuseWord( const TCHAR* szAbuse )
{
	tstring strAbuse = szAbuse;

	if (strAbuse.size() > MAX_ABUSE_WORD_LEN ||
		strAbuse.find_first_of(ms_CheatingChar) != std::tstring::npos)
		return false;

	TCHAR buf[512] = { 0 };
	_sntprintf((TCHAR*)buf, sizeof(buf) - 1, _T("%s"), strAbuse.c_str());

	// �ܾ��� ��� ���ڸ� Ʈ�� ���·� �����Ѵ�.
	if (m_pRoot == NULL)
		m_pRoot = new CSAbuseCharNode();

	CSAbuseCharNode* pCurrent = m_pRoot;
	for (size_t i = 0; i < strAbuse.size(); i++)
	{
		pCurrent = pCurrent->AddChild(buf[i]);
	}

	// �ܾ��� ���� �Ǵ� ���ڸ� flag�� ǥ��.
	pCurrent->SetLeafNode(true);

	return true;
}

void CSStringFilter::NextLine( TCHAR*& prfBuf)
{
	for ( ; *prfBuf != _T('\n'); ++prfBuf )
		NULL;
		
	++prfBuf;
}


bool CSStringFilter::IsValidStr( const TCHAR* szSrcString, bool bCheckSpecialChar)
{
	if ( NULL == szSrcString)
		return false;
	
	TCHAR* pCopiedString = _tcsdup(szSrcString);
	tstring original = _tcslwr(pCopiedString);	///< �ҹ��ڷ� ��ȯ
	free(pCopiedString);

	for (size_t i=0; i<original.size(); i++)
	{
		if (Match(original.substr(i, original.size() - i)) > 0)
			return false;
	}

	return true;
}

tstring CSStringFilter::Filter( const TCHAR* szSrcString )
{
	TCHAR* pCopiedString = _tcsdup(szSrcString);
	tstring original = _tcslwr(pCopiedString);	///< �ҹ��ڷ� ��ȯ
	free(pCopiedString);

	std::tstring text(original);

	for (size_t i=0; i<original.size();)
	{
		size_t size = Match(original.substr(i, original.size() - i));
		if (size > 0)
		{
			text.replace(i, size, std::tstring(size, '*'));
			i += size;
		}
		else
		{
#ifdef _UNICODE
			i++;
#else
			// �ѱ��̶�� 2����Ʈ��, �ƴ϶�� 1����Ʈ�� �Ѿ��.
			i += original[i] & 0x80 ? 2 : 1;
#endif
		}
	}

	return text;
}

/************************************************************************
	PreTranslateStr

	desc	��Ʈ���� �ҹ���ȭ�ϰ� Ư�����ڸ� �����Ѵ�.
	arg		strInText	ó���� ��Ʈ��
			strOutText	ó���� ��Ʈ��
	re		true		������ �ʴ� Ư�����ڸ� �����ϰ� ����
			false		����� Ư������ ����
*************************************************************************/
bool CSStringFilter::PreTranslateStr( const string& strInText, string& strOutText)
{
	char *pStrLowercase;
	//	_strlwr_s( pStrLowercase = _strdup( strInText.c_str()), 512);			// ���ȿ�
	//	pStrLowercase = _strlwr( _strdup( strInText.c_str()));					// �񺸾ȿ�
	pStrLowercase = _strdup( strInText.c_str());
	strOutText = pStrLowercase;

	// ĳ���� �˻�
	int nPos = 0;					// ��Ʈ�� �˻� ��ġ
	bool bHaveSpcChar = false;		// ����� Ư������ ���� ����
	char ch;
	unsigned char u_ch;

	while (nPos < (int)strOutText.size())
	{
		ch = strOutText.at( nPos);
		u_ch = ch;

		// ù 2����Ʈ�� �����ڵ� �������� Ȯ��		
		if (IsDBCSLeadByteEx(MLocale::GetCodePageFromLang(LANG_KOREAN), ch) == TRUE)
		{
#ifdef LOCALE_JAPAN   // �Ϻ� ���� ������ ��� Ư�����ڵ� ���� 2����Ʈ��. 
			if ( ch == -127 && strOutText.at( nPos +1) == 64) // �ϴ� �����̽��� ���� 
			{
				bHaveSpcChar = true;
				m_strLastFilterdWord = strOutText.substr(nPos, 2);;
			}
#endif
			nPos += 2;
		}
		// �Ϲ� ASCII �������� Ȯ��
#ifdef LOCALE_JAPAN   
		else if ( ( (ch >= 'a') && (ch <= 'z') ) ||	
			( (ch >= 'A') && (ch <= 'Z') ) ||
			( (ch >= '0') && (ch <= '9') ) ||
			( (u_ch >= 0xA1 ) && (u_ch <= 0xDF ) ) )  // �Ϻ� �ݰ� �������� Ȯ��
#else 
		else if ( ( (ch >= 'a') && (ch <= 'z') ) ||	
			( (ch >= 'A') && (ch <= 'Z') ) ||
			( (ch >= '0') && (ch <= '9') )  )  
#endif
		{
			if( ( (ch >= 'A') && (ch <= 'Z') ) )  // �ҹ��� ��ȯ
			{
				strOutText[nPos] += 0x20;
			}
			nPos += 1;
		}
		else
		{
			// �����ϱ� ���� �ش� ĳ���Ͱ� ����� Ư���������� �˻�
			if ( (ch != '_') && (ch != '[') && (ch != ']') )
			{
				bHaveSpcChar = true;
			}

			// ĳ���� ����
			strOutText.erase( nPos, 1);
		}
	}

	free( pStrLowercase);

	return bHaveSpcChar;
}

wstring CSStringFilter::RemoveGameSystemCharacter(wstring strText)
{
	wregex pattern(L"\\{.*\\}|\\{|\\}");
	return regex_replace(strText, pattern, wstring(L""));
}


size_t CSStringFilter::Match(const std::tstring& text) const
{
	if (text.empty()) return 0;
	if (IsPunctutation(text[0])) return 0;
	if (m_pRoot == NULL) return 0;

	CSAbuseCharNode* pCurrent = m_pRoot;
	size_t i = 0;

	while (i < text.size())
	{
		if (IsPunctutation(text[i]))
		{
			// ���� ���ڰ� ���� ��ȣ�� ��쿡�� continue
			// ��Ӿ� ���̻��̿� ���� ��ȣ�� ���� ��츦 �˻��ϱ� ���ؼ�.
			// ex) ����!��
			i++;
			continue;
		}

		// �ڽ� ��� �߿� ���� ���ڿ� ��ġ�ϴ� ������ �̾����� ���� ã�´�.
		pCurrent = pCurrent->FindChild(text[i]);

		// �� �̻� �̾����� ��尡 ���ٴ� ���� ���ڷ� ���� ���ڵ�� ��ġ�ϴ�
		// �ܾ ��� Ʈ�� �� �������� �ʴ´ٴ� ��.
		if (pCurrent == NULL) return 0;

		// ���͸��ؾ��ϴ� �ܾ��!
		if (pCurrent->IsLeafNode()) return i + 1;	///< ��Ӿ ���ԵǾ� ���� ��쿡�� �� ��Ӿ��� ���̸� ����.

		i++;
	}

	return 0;
}

bool CSStringFilter::IsPunctutation(TCHAR nChar) const
{
	return ms_CheatingChar.find(nChar) != std::tstring::npos;
}



CSStringFilter* CSStringFilter::GetInstance()
{
	static CSStringFilter ChattingFilter;
	return &ChattingFilter;
}



//////////////////////////////////////////////////////////////////////////


CSAbuseCharNode::CSAbuseCharNode()
: m_bLeafNode(false)
{
}

CSAbuseCharNode::~CSAbuseCharNode()
{
	SAFE_DELETE_MAP(*this);
}

CSAbuseCharNode* CSAbuseCharNode::FindChild(TCHAR nChar) const
{
	const_iterator itr(find(nChar));
	return itr != end() ? itr->second : NULL;
}

CSAbuseCharNode* CSAbuseCharNode::AddChild(TCHAR nChar)
{
	// �ش��ϴ� �ڽ��� ���� ���, ���ο� ��带 �����ؼ� �߰��Ѵ�.
	iterator itr(find(nChar));
	if (itr == end()) itr = insert(value_type(nChar, new CSAbuseCharNode)).first;

	return itr->second;
}

