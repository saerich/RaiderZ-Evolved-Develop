//
// PropertySystem.cpp
//
 

#include "PropertySystem.h"

list<CAbstractProperty*>* CPropertySystem::ms_pProperties	= 0;
bool					  CPropertySystem::ms_bReleased		= false;	

CPropertySystem::CPropertySystem()
{
}

CPropertySystem::~CPropertySystem()
{
}

void CPropertySystem::Release()
{
	_ASSERT( false == ms_bReleased );	//�����Ȱ��� �ٽ� ������ �� ����.

	if( 0 == ms_pProperties) return;

	for ( list<CAbstractProperty*>::iterator it = ms_pProperties->begin(); it != ms_pProperties->end(); ++it )
	{
		delete *it;
	}
	ms_pProperties->clear();	
	delete ms_pProperties;
	ms_pProperties = 0;

	ms_bReleased = true;
}