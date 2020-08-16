#include "stdafx.h"

#include "MXml.h"
#include "MFileSystem.h"
#include "MDebug.h"

#include "RApplication.h"
#include "RMeshMgr.h"
#include "RConfiguration.h"

#include <string>

namespace rs3 {

RMeshMgr::RMeshMgr()
{
//	SetPath(REngine::GetConfig().GetModelPath());
}

RMeshMgr::~RMeshMgr()
{
}

void RMeshMgr::Destroy()
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

}
