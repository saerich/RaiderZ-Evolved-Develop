#include "StdAfx.h"
#include "RAnimationManager.h"

namespace rs3 {

void RAnimationManager::Destroy()
{
	Clear();
}

void RAnimationManager::Clear()
{
#ifndef MODIFY_LOADER
	// ĳ�̵� ���ҽ� �����
	for (int i = 0; i < RResource::RRCP_MAX; ++i)
		DeepClearResourceContainer( m_resourcesCached[i] );

	// ���� ������� ���ҽ� �����
	DeepClearResourceContainer( m_resources );
#else
	for (int i = 0; i < RResource::RRCP_MAX; ++i)
		m_CachedResources[i].DeepClear();
	m_Resources.DeepClear();
#endif
}

}	//namespace rs3