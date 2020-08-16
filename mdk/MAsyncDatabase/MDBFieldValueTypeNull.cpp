#include "stdafx.h"
#include "MDBFieldValueTypeNull.h"
#include "MDatabaseDefine.h"


namespace mdb
{
	bool MDBFieldValueTypeNull::IsNull()
	{
		return true;
	}


	bool MDBFieldValueTypeNull::GetData(MDBRecordSet* pRecordSet)
	{
		_ASSERT(IsNull());
		return true;
	}


	void MDBFieldValueTypeNull::Reset()
	{
		SetNull();
	}
	

	__int64 MDBFieldValueTypeNull::AsInt64()
	{
		_ASSERT("�������� �ʴ� ����ȯ�Դϴ�.");
		return 0;
	}


	long MDBFieldValueTypeNull::AsLong()
	{
		_ASSERT("�������� �ʴ� ����ȯ�Դϴ�.");
		return 0;
	}


	int	MDBFieldValueTypeNull::AsInt()
	{
		_ASSERT("�������� �ʴ� ����ȯ�Դϴ�.");
		return 0;
	}


	short MDBFieldValueTypeNull::AsShort()
	{
		_ASSERT("�������� �ʴ� ����ȯ�Դϴ�.");
		return 0;
	}


	unsigned char MDBFieldValueTypeNull::AsByte()
	{
		_ASSERT("�������� �ʴ� ����ȯ�Դϴ�.");
		return 0;
	}


	bool MDBFieldValueTypeNull::AsBool()
	{
		_ASSERT("�������� �ʴ� ����ȯ�Դϴ�.");
		return false;
	}


	const string MDBFieldValueTypeNull::AsString()
	{
		_ASSERT("�������� �ʴ� ����ȯ�Դϴ�.");
		return string("NULL");
	}


	const TIMESTAMP_STRUCT MDBFieldValueTypeNull::AsTimestamp()
	{
		_ASSERT("�������� �ʴ� ����ȯ�Դϴ�.");
		TIMESTAMP_STRUCT ts;
		ts.year = 0;
		return ts;
	}

	const float MDBFieldValueTypeNull::AsFloat()
	{
		_ASSERT("�������� �ʴ� ����ȯ�Դϴ�.");
		return 0.0f;
	}

	const std::wstring MDBFieldValueTypeNull::AsWString()
	{
		return std::wstring(L"NULL");
	}

	MDBFieldValueTypeNull::MDBFieldValueTypeNull(const SQLSMALLINT nIndex) : MDBFieldValueType(nIndex)
	{

	}
}