#pragma once


#include "MDBFieldValueType.h"


namespace mdb
{
	class MADB_API MDBFieldValueTypeNull : public MDBFieldValueType
	{
	public :
		MDBFieldValueTypeNull(const SQLSMALLINT nIndex);
		~MDBFieldValueTypeNull() {}

		bool					IsNull();

		__int64					AsInt64();
		long					AsLong();
		int						AsInt();
		short					AsShort();
		unsigned char			AsByte();
		bool					AsBool();
		const string			AsString();
		const std::wstring		AsWString();
		const TIMESTAMP_STRUCT	AsTimestamp();
		const float				AsFloat();

		void					Reset();
		bool					GetData(MDBRecordSet* pRecordSet);
	};
}