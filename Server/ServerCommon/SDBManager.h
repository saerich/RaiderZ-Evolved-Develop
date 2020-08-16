#pragma once

#include "MDatabase.h"
#include "MDatabaseDesc.h"
#include "ServerCommonLib.h"
#include "MTstring.h"

namespace mdb
{
	class MDBRecordSet;
}

class CSLogListener;

/// DB ������
class SCOMMON_API SDBManager 
{
private:
	// �񵿱� ���� ������� �ʾ���. ���� ���� GDBTask�� �����ʸ� �־���� ��.
	CSLogListener*	m_pListener;

	void WriteDBInfo(mdb::MDatabase* pDB);

protected :
	mdb::MDatabaseDesc	m_DBDesc;
	mdb::MDatabase		m_DB;

public:
	SDBManager();
	virtual ~SDBManager();

	virtual bool Connect(mdb::MDatabaseDesc& DBDesc);
	virtual void Disconnect();
	virtual bool Execute(const wchar_t* szSQL);
	virtual bool Execute(mdb::MDBRecordSet& rs, const wchar_t* szSQL);
	virtual bool Ping();

	mdb::MDatabase* GetDatabase()	{ return &m_DB; }
	static void LogCallback( const wstring& strLog );	

	void ErrorLog(const wchar_t *pFormat, ...);
	void SetListener(CSLogListener* pListener);
};
