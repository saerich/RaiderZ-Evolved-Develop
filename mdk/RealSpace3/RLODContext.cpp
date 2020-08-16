#include "stdafx.h"
#include "RLODContext.h"
#include "RLODProcessorBase.h"
#include "RLODProcessorDistance.h"
#include "RLODProcessorFog.h"
#include "RLODProcessorProjection.h"

namespace rs3
{

RLODContext::RLODContext( DWORD dwTypeFlags)
{
	CreateProcessors( dwTypeFlags);
}


RLODContext::~RLODContext()
{
	DeleteProcessors();
}


bool RLODContext::CheckProcessor( DWORD dwType)
{
	for( int i=0; i<(int)m_vLodProcessBase.size(); i++)
	{
		if( m_vLodProcessBase[i]->GetTypeID() == dwType)
			return true;
	}

	return false;
}


bool RLODContext::CreateProcessors( DWORD dwTypeFlags)
{
	RPFC_THISFUNC;

	if( dwTypeFlags & LODP_DISTANCE)
	{
		if(!CheckProcessor(LODP_DISTANCE))
			m_vLodProcessBase.push_back( new RLODProcessorDistance());
	}
	if( dwTypeFlags & LODP_FOG)
	{
		if(!CheckProcessor(LODP_FOG))
			m_vLodProcessBase.push_back( new RLODProcessorFog());
	}
	if( dwTypeFlags & LODP_PROJECTION)
	{
		if(!CheckProcessor(LODP_PROJECTION))
			m_vLodProcessBase.push_back( new RLODProcessorProjection());
	}

	return true;
}


bool RLODContext::DeleteProcessors()
{
	RPFC_THISFUNC;

	for( int i=0; i<(int)m_vLodProcessBase.size(); i++)
	{
		SAFE_DELETE( m_vLodProcessBase[i]);
	}
	m_vLodProcessBase.clear();

	return true;
}

RLODProcessorBase* RLODContext::GetProcessor( DWORD dwType)
{
	RPFC_THISFUNC;

	for( int i=0; i<(int)m_vLodProcessBase.size(); i++)
	{
		if( (m_vLodProcessBase[i])->GetTypeID() == dwType)
			return m_vLodProcessBase[i];
	}

	return NULL;
}


bool RLODContext::SetMetricInfo( RLODMetricInfo* pInfo)
{
	RPFC_THISFUNC;

	/// ��Ʈ�� ������ ���Ͻ� �ø����� ���ȴ�
	RLODProcessorBase* pProcessor = GetProcessor( LODP_DISTANCE);
	if( pProcessor)
	{
		((RLODProcessorDistance*)pProcessor)->SetMetricInfo( pInfo);
		return true;
	}
	else
		return false;

}


/// LOD ���� ���¸� ��ȯ�Ѵ�.
/// ��ȯ��
/// * -1 : �׸��� �ʴ´�.
/// * 0~n : ���ڰ� �������� ������������ ���� ����
int RLODContext::CalculateLOD( RCameraSceneNode* pCamera, const RSphere* pBoundingSphere)
{
	RPFC_THISFUNC;

	/// LOD ���μ����� ������ LOD ó���� ���� �ʴ´�. �� �� ���� ���¸� ��ȯ
	int nLowestLevel = 0;

	/// ���μ������� ���鼭 ���� ���� ������ ������ �����Ѵ�.
	for( int i=0; i<(int)m_vLodProcessBase.size(); i++)
	{
		int nLevel = m_vLodProcessBase[i]->CalculateLOD( pCamera, pBoundingSphere);
		if( nLevel < 0)
			return -1;
		else
			/// ���ڰ� �������� ������Ƽ�� ����Ų��. ����� ���ƾ� ��.
			nLowestLevel = max( nLowestLevel, nLevel);
	}

	return nLowestLevel;
}


}