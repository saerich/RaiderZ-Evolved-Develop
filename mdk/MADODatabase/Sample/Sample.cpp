
/**
*	@brief	������Ʈ�� CLR ������Ʈ���� �մϴ�.
*/


#include "stdafx.h"
#include "MADODatabase.h"
#include <map>
using namespace std;

#ifdef _DEBUG
#pragma comment(lib, "MADODatabased.lib")
#else
#pragma comment(lib, "MADODatabase.lib")
#endif

const int MAX_TEST_COUNT = 1;
int g_nExecuteCount = 0;
static unsigned int g_nJobNumber = 0;

void OnComplete(unsigned int nJobNumber, MRecordset* pRs, char* szError);

typedef map<unsigned int, char*> MAP_QUERY;
MAP_QUERY g_mapQuery;

bool AddMapQuery(unsigned int nJobNumber, char* szQuery);
char* FindMapQuery(unsigned int nJobNumber);
void DeleteMapQuery(unsigned int nJobNumber);
void ClearMapQuery();

int main()
{
	MADODatabase m_AdoNet;	// ��ü ����

	// ��ü �ʱ�ȭ (5���� ���� : �Ϸ� ���� �ݹ��Լ�)
	if (m_AdoNet.InitADODatabase("192.168.0.176", "GunzDB2", "sa", "tnseornr", OnComplete) == false)
	{
		return 0;
	}

	for (int i = 0; i < MAX_TEST_COUNT; i++)
	{
		char* szQuery = new char[MAX_QUERY_STRING_LENGTH];
		strcpy_s(szQuery, MAX_QUERY_STRING_LENGTH, "spTestGetDataType");	// spTestGetDataType

		AddMapQuery(g_nJobNumber, szQuery);				// �׻� ���� g_nJobNumber �� ������ �Ӵϴ�.(�Ϸᰡ �����ϴ°ͺ��� ���� �����ϴ� ������)

		m_AdoNet.PushQuery(szQuery, g_nJobNumber++);	// ������ ������ ��û�մϴ�. (���ڷ� �Ѱ��� Key������ �Ϸᰡ �����մϴ�.)
	}

	while (true)										// ��� �Ϸᰡ �������� ���ν����尡 ����Ǹ� �ȵǴϱ�
	{
		if (g_nExecuteCount >= MAX_TEST_COUNT)
		{
			break;
		}
		Sleep(1);
	}

	ClearMapQuery();
	return 0;
}

// �ϷḦ ���� �ݹ��Լ��Դϴ�.
void OnComplete(unsigned int nJobNumber, MRecordset* pRs, char* szError)
{
	char* pQuery = FindMapQuery( nJobNumber );
	DeleteMapQuery(nJobNumber);
	delete pQuery;

	if ((strlen(szError) != 0) || (pRs == NULL))
	{
		OutputDebugString(szError);
		return;
	}

	while (pRs->MoveNext() == true)
	{
		__int64 nInt64 = 0;
		// �÷� �̸����� ã�ƿ���
		pRs->GetInt64("type_bigint", &nInt64);					// INT64
		nInt64 = 0;
		// �÷� �ε����� ã�ƿ���
		pRs->GetInt64(0, &nInt64);

		int nInt32 = 0;
		pRs->GetInt32("type_int", &nInt32);						// INT32
		nInt32 = 0;
		pRs->GetInt32(1, &nInt32);

		short nInt16 = 0;
		pRs->GetInt16("type_smallint", &nInt16);				// INT16
		nInt16 = 0;
		pRs->GetInt16(2, &nInt16);

		unsigned char nByte = 0;
		pRs->GetByte("type_tinyint", &nByte);					// TINYINT

		bool bBool = false;
		pRs->GetBool("type_bit", &bBool);						// BIT

		char nDecimal_1[9+1] = {0, };
		pRs->GetDecimal("type_decimal_5byte", nDecimal_1);		// DECIMAL, NUMERIC

		char nDecimal_2[19+1] = {0, };
		pRs->GetDecimal("type_decimal_9byte", nDecimal_2);

		char nDecimal_3[28+1] = {0, };
		pRs->GetDecimal("type_decimal_13byte", nDecimal_3);

		char nDecimal_4[38+1] = {0, };
		pRs->GetDecimal("type_decimal_17byte", nDecimal_4);

		char nMoney[21+1] = {0, };
		pRs->GetMoney("type_money", nMoney);					// MONEY, SMALLMONEY

		char nSmallMoney[12+1] = {0, };
		pRs->GetMoney("type_smallmoney", nSmallMoney);

		float fFloat = 0.f;
		pRs->GetFloat("type_real", &fFloat);					// REAL

		double fDouble = 0.f;
		pRs->GetDouble("type_float", &fDouble);					// FLOAT, DOUBLE

		SYSTEMTIME stTime;
		memset(&stTime, 0, sizeof(SYSTEMTIME));
		pRs->GetDateTime("type_datetime", &stTime);				// DATETIME, SMALLDATETIME, DATE

		// ����. �����ͺ��̽��� ���ڿ� �ڷ����� ���̸� 24�� ����
		char szChar = '\0';
		pRs->GetAnsiChars("type_char", &szChar);				// �������� : CHAR(n)

		char szString[24+1] = {0, };
		pRs->GetAnsiChars("type_char_string", szString);

		char szVarChar = '\0';
		pRs->GetAnsiChars("type_varchar", &szVarChar);			// �������� : VARCHAR(n), TEXT

		char szVarString[24+1] = {0, };
		pRs->GetAnsiChars("type_varchar_string", szVarString);

		wchar_t szNChar = '\0';
		pRs->GetUniChars("type_nchar", &szNChar);				// �������� : NCHAR(n)

		wchar_t szNString[24+1] = {0, };
		pRs->GetUniChars("type_nchar_string", szNString);

		wchar_t szNVarChar = '\0';
		pRs->GetUniChars("type_nvarchar", &szNVarChar);			// �������� : NVARCHAR(n), NTEXT

		wchar_t szNVarString[24+1] = {0, };
		pRs->GetUniChars("type_nvarchar_string", szNVarString);

		char szBinary = '\0';
		pRs->GetBinarys("type_binary", &szBinary);				// �������� : BINARY(n)

		char szBinarys[24+1] = {0, };
		pRs->GetBinarys("type_binary_array", szBinarys);

		char szVarBinary = '\0';
		pRs->GetBinarys("type_varbinary", &szVarBinary);		// �������� : VARBINARY(n), IMAGE

		char szVarBinarys[24+1] = {0, };
		pRs->GetBinarys("type_varbinary_array", szVarBinarys);
	}

	g_nExecuteCount++;
}

bool AddMapQuery(unsigned int nJobNumber, char* szQuery)
{
	MAP_QUERY::iterator iter = g_mapQuery.find(nJobNumber);
	if (iter != g_mapQuery.end())
	{
		_ASSERT(0);
		return false;
	}
	g_mapQuery.insert(make_pair(nJobNumber, szQuery));
	return true;
}

char* FindMapQuery(unsigned int nJobNumber)
{
	MAP_QUERY::iterator iter = g_mapQuery.find(nJobNumber);
	if (iter != g_mapQuery.end())
	{
		return (char*)iter->second;
	}
	return NULL;
}

void DeleteMapQuery(unsigned int nJobNumber)
{
	MAP_QUERY::iterator iter = g_mapQuery.find(nJobNumber);
	if (iter != g_mapQuery.end())
	{
		g_mapQuery.erase(iter);
	}
}

void ClearMapQuery()
{
	g_mapQuery.clear();
}