#include "stdafx.h"
#include "RDuplicateUpdateCheckContext.h"

namespace rs3
{

//////////////////////////////////////////////////////////////////////////
// RDuplicateUpdateCheckContext
RDuplicateUpdateCheckContext::RDuplicateUpdateCheckContext() :
 m_bNeedUpdate( false )
,m_nParentUpdatedID( 0 )
,m_nUpdatedID( 0 )
{
}

RDuplicateUpdateCheckContext::~RDuplicateUpdateCheckContext()
{
}

bool RDuplicateUpdateCheckContext::IsNeedUpdate( bool bNeedUpdateHierarchy, int nParentUpdatedID )
{
	// ���� ����
	bool bNeedUpdate = m_bNeedUpdate;

	// �θ� ������Ʈ �Ǿ��ٸ� ���� ������Ʈ �ؾ� �Ѵ�
	if( bNeedUpdateHierarchy )
	{
		if( m_nParentUpdatedID != nParentUpdatedID )
		{
			bNeedUpdate = true;
			m_nParentUpdatedID = nParentUpdatedID;
		}
	}

	if( bNeedUpdate )
		m_nUpdatedID++;

	m_bNeedUpdate = false;

	return bNeedUpdate;
}

void RDuplicateUpdateCheckContext::SetForceUpdated( bool bNeedUpdateHierarchy, int nParentUpdatedID )
{
	if( bNeedUpdateHierarchy )
	{
		m_nParentUpdatedID = nParentUpdatedID;
	}

	m_nUpdatedID++;
	m_bNeedUpdate = false;
}

}