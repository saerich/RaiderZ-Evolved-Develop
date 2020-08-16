#include "EnumDescription.h"

using namespace std;

// ENUM : min max �� �ְ� description �� @enum[] �� �������� enum ���� ���
// ���� description "@enum[solo,red,blue]���� ������ �����մϴ�"
const char* const ENUM_ID = "@enum[";

bool EnumDescription::CreateEnumDescription(const char* szDescription, EnumDescription* pEnumDescription, const char** ppszDescription )
{
	const int nLengthID = strlen(ENUM_ID);
	if( strncmp(ENUM_ID, szDescription, nLengthID ) != 0)
		return false;

	string strDescString = szDescription + nLengthID;
	string::size_type nClosePos = strDescString.find_first_of("]");
	_ASSERT( nClosePos != string::npos);
	strDescString = strDescString.substr(0, nClosePos );

	/// @enum[] ���ŵ� ��ũ���� ����
	const char* szNakedDescription = szDescription + nLengthID + nClosePos + 1;
	*ppszDescription = szNakedDescription;

	string::size_type nStart	= 0;
	string::size_type nEnd		= 0;
	while (true)
	{
		string::size_type nEnd = strDescString.find_first_of(",", nStart);
		if (nEnd == string::npos)
		{
			pEnumDescription->m_enumStrings.push_back(strDescString.substr(nStart));
			break;
		}
		else
		{
			pEnumDescription->m_enumStrings.push_back(strDescString.substr(nStart, nEnd-nStart));
			nStart = strDescString.find_first_not_of(",", nEnd);
		}		
	}

	return true;
}

const char*	EnumDescription::GetString( unsigned int nValue ) const
{
	_ASSERT( 0<= nValue && nValue < m_enumStrings.size() );
	return m_enumStrings[nValue].c_str();
}

int EnumDescription::GetValue( const char* szString ) const
{
	int nValue = UNDEFINED;
	bool bFound = false;
	for( size_t i=0; i<m_enumStrings.size(); i++)
	{
		if( strcmp(m_enumStrings[i].c_str(), szString ) == 0)
		{
			bFound = true;
			nValue = i;
		}
	}

	_ASSERT(bFound);
	return nValue;
}
