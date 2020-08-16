#pragma once

#include "ServerCommonUnitTestLib.h"

struct TDBColumnDef
{
	string	strName;	// �÷� �̸�.
	string	strType;	// �÷� Ÿ��.
};


class SCOMMON_UNITTEST_API TDBTableDef
{
private:
	typedef vector<TDBColumnDef*> DBColumnVec;
public :
	friend TDBTableDef;

	TDBTableDef(const string& strName);
	~TDBTableDef();


	bool AddColumn(const string& strName
		, const string& strType);


	const string&	GetName()			{ return m_strName; }
	size_t			GetColumnCount()	{ return m_ColumnVec.size(); }
	TDBColumnDef*	GetColumn(const string& strName);
	TDBColumnDef*	GetColumn(const size_t nIndex);

	void Release();


	bool operator ==(TDBTableDef& Dest);
	bool operator ==(TDBTableDef* pDest);

private :
	TDBTableDef()  {}


private :
	string		m_strName;
	DBColumnVec m_ColumnVec;
};


typedef vector<TDBTableDef*> DBTableVec;