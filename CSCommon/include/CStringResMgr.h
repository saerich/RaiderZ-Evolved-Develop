#pragma once

#include "CSCommonLib.h"
#include "CStringRes.h"
#include "MTstring.h"

struct CLangNode;

/// ���ڿ� ���ҽ� ������
class CSCOMMON_API CStringResMgr
{
public:
	typedef map<tstring, CStringRes<tstring>*>	lang_map;

public:
	CStringResMgr();
	virtual ~CStringResMgr();

	bool Init(const CLangNode& node, bool bIgnoteNonTranslationMark=false);											///< �ش� ������ xml�� ��� �д´�.
	bool Init(const CLangNode& node, const vector< std::tstring>& vFileList, bool bIgnoteNonTranslationMark=false);	///< ������ ���� ����� ��� �д´�

	virtual	void Clear(void);

	virtual const TCHAR* GetString(tstring& key);
	virtual const TCHAR* GetString(const TCHAR* key);
	virtual wstring Plural(wstring key);
	bool Exist(tstring& key);
	bool Exist(const TCHAR* key);

	bool InsertString(const tstring& key, const tstring& value);
	bool UpdateString(const tstring& key, const tstring& value);
	void DeleteString(tstring& key);

	bool SetLocale(tstring strLocale);
	void AddLocale(tstring strLocale);
	tstring GetLocale(void) const;

protected:
	virtual bool OnInit(const CLangNode& node)		{ return true;	}	

private:
	bool LoadStrTable(const CLangNode& node, const TCHAR* szFileName, bool bIgnoteNonTranslationMark);

protected:
	lang_map				m_mapStringTables;		///< �ٱ��� ���̺�	
	tstring					m_strLocale;
};
