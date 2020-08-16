#include "StdAfx.h"
#include "ObjectWrapper.h"

void CObjectWrapper::Create( CBaseObject* pBaseObject )
{
	m_objects.push_back( pBaseObject );
}

void CObjectWrapper::Create( list<CBaseObject*>& objects )
{
	if( objects.empty() ) return;

	// ���� Ÿ������ Ȯ��
 	 list<CBaseObject*>::iterator itr = objects.begin();
	CBaseObject* pFirstObject = *itr;

	++itr;
 	for( ; itr!= objects.end(); ++itr )
 	{
		// �ٸ��� �������� ó�� �Ұ�
		if( (*itr)->GetRTTI() != pFirstObject->GetRTTI() )
			return;
 	}
 
	m_objects = objects;
}


void CObjectWrapper::EnumProperties( vector<CAbstractProperty*>& o_Result )
{
	if( m_objects.empty() ) return;

	m_objects.front()->GetRTTI()->EnumProperties( o_Result );
}

float CObjectWrapper::GetMinValueFloat( CTypedProperty<float>* pProperty )
{
	return pProperty->GetMinValue( m_objects.front() );
}

float CObjectWrapper::GetMaxValueFloat( CTypedProperty<float>* pProperty )
{
	return pProperty->GetMaxValue( m_objects.front() );
}

int	CObjectWrapper::GetMinValueInt( CTypedProperty<int>* pProperty )
{
	return pProperty->GetMinValue( m_objects.front() );
}

int	CObjectWrapper::GetMaxValueInt( CTypedProperty<int>* pProperty )
{
	return pProperty->GetMaxValue( m_objects.front() );
}