//////////////////////////////////////////////////////////////////////////
///
///	���� ġȯ ��� ���� �ڵ� : http://serious-code.net/moin.cgi/_bf_e5_bc_b3_c7_ca_c5_cd_b8_b5
///
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "CSCommonLib.h"

class CSAbuseCharNode;


class CSCOMMON_API CSStringFilter
{
public:
	~CSStringFilter();

	void					Clear();

	bool					LoadFromFile( const TCHAR* szFileName);
	void					LoadFromStringDB( TCHAR* pStringDB);

	bool					IsValidStr( const TCHAR* szSrcString, bool bCheckSpecialChar =false);
	tstring					Filter( const TCHAR* szSrcString );

	const TCHAR*			GetLastFilteredStr()	{ return m_strLastFilterdWord.c_str(); }


	static CSStringFilter*	GetInstance();

	/// ��Ʈ���� �ҹ���ȭ�ϰ� Ư�����ڸ� �����Ѵ�.
	static bool				PreTranslateStr(const string& strInText, string& strOutText);

	/// ���ӿ��� ����ϴ� Ư�� ����(�߰�ȣ ������ ���� �÷� �ڵ� ��)�� �����Ѵ�.
	static wstring			RemoveGameSystemCharacter(wstring strText);

private:
	CSStringFilter();

	CSAbuseCharNode*		m_pRoot;				///< ��Ӿ� �ֻ��� ��� - ��� ��Ӿ���� ù ���ڸ� ��� �ִ� ���.
	tstring					m_strLastFilterdWord;

	static const tstring	ms_CheatingChar;		///< ���͸��� ������ ���� ��ȣ��. ��Ӿ� ���̿� ���� ���� ���� �����ϱ� ����.

	void					ParseLine( TCHAR*& prfBuf, TCHAR* szText);
	bool					AddAbuseWord( const TCHAR* szAbuse );
	void					NextLine( TCHAR*& prfBuf);

	size_t					Match(const std::tstring& text) const;	///< ������ ù ����Ʈ���� ��Ӿ ���ԵǾ��ִ��� �˻��Ѵ�.
	bool					IsPunctutation(TCHAR nChar) const;		///< Ư�� ���ڰ� ���� ��ȣ���� �˻�.

};


//////////////////////////////////////////////////////////////////////////
///	��Ӿ� ���� �ϳ��� ���� ���.
///	�ֻ��� ��尡 ��Ӿ���� ù ���ڵ��� ��� ����̰� �ڽ� ������ ���� ���ڰ� �Ǵ� ������ ��Ӿ Ž��.
class CSCOMMON_API CSAbuseCharNode : public map<TCHAR, CSAbuseCharNode* >
{
public:
	CSAbuseCharNode();
	~CSAbuseCharNode();

	// Ư�� ���ڰ� ���� ���� �ִ��� �˻�.
	CSAbuseCharNode*	FindChild(TCHAR nChar) const;

	// ��Ӿ�� �̾��� ���� �߰�.
	CSAbuseCharNode*	AddChild(TCHAR nChar);

	bool				IsLeafNode() const		{ return m_bLeafNode; }
	void				SetLeafNode(bool value)	{ m_bLeafNode = value; } 

private:
	bool m_bLeafNode; ///< �ܾ��� �� ���� ����

};


//////////////////////////////////////////////////////////////////////////

/// �ν��Ͻ��� ����
inline CSStringFilter* GetStringFilter()
{
	return CSStringFilter::GetInstance();
}


