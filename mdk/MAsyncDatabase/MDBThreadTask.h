#pragma once


#include <string>
#include <vector>

using namespace std;

#include "MADBLib.h"

namespace mdb
{
	class MDatabase;
	

	enum MDB_THRTASK_RESULT
	{
		MDBTR_SUCESS = 0	// �۾��� �Ϸ�Ǿ��ٸ� �⺻������ ������ �����Ѵ�.(���и� �ϴ��� �� ���� �Ѱ��ش�.)
							// �� ���� ������ �Ǹ� MDBThreadTask��ü�� �����ȴ�.

		, MDBTR_MOVE = 1	// ���� OnCompleted���� �ٷ� ó������ �ʰ� ���� ������ �ϱ� ���ؼ� �������� �Ѵٸ�
							// �� ���� �����Ѵ�.
							// �׷��� MDBThreadTask��ü�� �������� �ʰ� ����Ʈ���� ���� �Ѵ�.
							// ��ü ����(����)�� ��� å���� ��ü�� ������ ������ �̵��ȴ�.
	};


	typedef vector<wstring> MDB_SQL_VEC;

	class MADB_API MDBThreadTask
	{
	public :
		friend class MAsyncDatabase;
		friend class MDBThread;

		MDBThreadTask(const size_t nReserveSQLSize = 1);
		virtual ~MDBThreadTask();


		const MDB_SQL_VEC&			GetSQLList();
		const string				GetSQL(const size_t nIndex);		
		const wstring				GetSQLW(const size_t nIndex);
		const size_t				GetQuerySize();

		virtual void				OnExecute(MDatabase& rfDB)	= 0;	///< MultiThread���� ȣ���.
																		///< �񵿱�� ó���� �۾��� �ϴ� ��.

		virtual MDB_THRTASK_RESULT	OnCompleted()				= 0;	///< MAsyncDatabase�� ���� �����忡�� ȣ���.
																		///< �Ϸ�� �۾��� �����ϴ� ��.


		MDBThreadTask&				PushSQL(const string& strSQL);
		MDBThreadTask&				PushSQLW(const wstring& strSQL);

	
	private :
		MDB_SQL_VEC					m_vecSQL;

		static const wstring		m_strNULLQuery;
	};
}
