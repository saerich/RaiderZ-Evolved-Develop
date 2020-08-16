#include "StdAfx.h"
#include "ReflectiveEnumProperty.h"

using namespace std;

CReflectiveEnumProperty::CReflectiveEnumProperty( const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr,
												 DWORD_PTR dwData, int min , int max, const EnumDescription* pEnumDescription )
:CMFCPropertyGridProperty(strName,varValue,lpszDescr,dwData)
{

	// ������ �°� ������ Ȯ��

	// enum �� �ִ�/�ּҰ����� �����񱳸� �Ѵ�
	int nEnumCount = max - min +1;
	_ASSERT( pEnumDescription->GetCount() == nEnumCount );

	for( size_t i=0; i<pEnumDescription->GetCount(); i++)
		AddOption( pEnumDescription->GetString(i),FALSE );

	m_min = min;
	m_max = max;

	m_pEnumDescription = pEnumDescription;
}

CReflectiveEnumProperty::~CReflectiveEnumProperty( void )
{
}

BOOL CReflectiveEnumProperty::TextToVar( const CString& strText )
{
	CMFCPropertyGridProperty::TextToVar(strText);

	// description �� �ִ� ���� 0-base
	int nValueZeroBase = m_pEnumDescription->GetValue( (const char*) strText );
	if( nValueZeroBase != EnumDescription::UNDEFINED )
	{
		int nValue = nValueZeroBase + m_min;
		m_varValue = (variant_t)nValue;
	}

	return TRUE; 
}

CString CReflectiveEnumProperty::FormatProperty()
{
	variant_t variant = GetValue();

	if (variant.vt == VT_BSTR )
		return CString( variant );

	int nValue = variant;
	if( m_min <= nValue && nValue <= m_max)
		return m_pEnumDescription->GetString(nValue - m_min);

	return CString("undefined");
}
