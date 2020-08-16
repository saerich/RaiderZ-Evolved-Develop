#pragma once

#include "RPreDefine.h"
#include "RTypes.h"
#include "MHandle.h"
#include "RResource.h"
#include <map>

using namespace std;
using namespace stdext;

namespace rs3 {

typedef map<std::string, RResource*> RRESOURCEHASHMAP;
class RSceneManager;

class RResourceContainerFunctor
{
public:
	void operator() (RRESOURCEHASHMAP& mapResources) {}
};

class RResourceManager;
class RS_API RResourceContainer
{
	friend RResourceManager;
public:
	void			Add(RResource* pResource);
	RResource*		Get(std::string ResourceName);
	bool			Remove(std::string ResourceName);
	void			Safe_Operation(RResourceContainerFunctor& Functor);
	void			DeepClear(void);
	bool			Empty(void);

private:
	RRESOURCEHASHMAP			m_mapResources;
	MCriticalSectionSpinLock	m_LockForResources;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// base resource manager
//
class RS_API RResourceManager
{
	friend class RSceneManager;
	friend class REngine;

public:

	RResourceManager();
	virtual ~RResourceManager();


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� / �Ҹ�
	//
	virtual void				Init( RSceneManager* pSceneManager ){ m_pSceneManager = pSceneManager; }
	virtual void				Destroy()							{}
	virtual void				Clear()								{}


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �޸� ���� ��å
	//
	virtual int					GetUsingVideoMemory();	///< ���� �޸�
	virtual int					GetUsingSystemMemory();	///< �ý��� �޸�


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ĳ��
	//
	void						SetCacheEnable(bool _bEnable);													///< ĳ�� Ȱ��ȭ
	void						SetCacheDuration( RResource::CACHE_PRIORITY _eCacheIndex, float _fDuration );	///< �� ĳ��ť�� �ð�. ��.

	void						UpdateCache(float _fDelta);										///< ĳ���� ������ ��å�� ���� û��(Sweep)���ش�.
	void						ClearCache();													///< ��ü ĳ���� �����ش�.

#ifndef MODIFY_LOADER
	const RRESOURCEHASHMAP&		GetResourceMap()	const {	return m_resources; }
#else
	const RRESOURCEHASHMAP&		GetResourceMap()	const {	return m_Resources.m_mapResources; }
#endif

protected:
	// resource map
#ifndef MODIFY_LOADER
	mutable RRESOURCEHASHMAP	m_resources;
	mutable RRESOURCEHASHMAP	m_resourcesCached[ RResource::RRCP_MAX ]; ///< Cache Queue
	cml2::MCriticalSection		m_resourceMapLock;
#else
	RResourceContainer			m_Resources;
	RResourceContainer			m_CachedResources[RResource::RRCP_MAX];
#endif


	// cache
	bool						m_bCacheEnable;
	float						m_cacheDuration[ RResource::RRCP_MAX ];
	float						m_cacheRemainedTime[ RResource::RRCP_MAX ];

	RSceneManager*				m_pSceneManager;


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// resource management
	//
	RResource*					GetResource(const string &strName);				///< �����Ǵ� ���ҽ� ���
	void						AddResource(RResource* pResource);				///< �����Ǵ� ���ҽ� ���ϱ�. �ߺ��˻縦 �Ѵ�
	bool						RemoveResource(const string &strName);			///< �����Ǵ� ���ҽ� ����
	int							ReleaseRef( RResource* pResource );				///< RefCount�� ����߷��ش�.

	// ĳ��
	RResource*					FindFromCacheMapAndSyncToResourceMap(const string &strName);
	bool						InsertResourceToCacheMap(RResource* pResource);

	// �����
#ifndef MODIFY_LOADER
	void						DeepClearResourceContainer( RRESOURCEHASHMAP& _refResourceContainer );
#endif
	void						RequestDeepDeletion( RResource* _pResource, std::vector<RResource*>* pDeleteQueue );
};
//----------------------------------------------------------------------------------------------------
inline int RResourceManager::ReleaseRef( RResource* pResource )
{
	// �տ��� �̹� �� ���������մϴ�.
	_ASSERT(pResource->m_nRefCount>0);
	return --pResource->m_nRefCount; 
}
//----------------------------------------------------------------------------------------------------
inline bool RResourceManager::InsertResourceToCacheMap( RResource* pResource )
{
	_ASSERT(pResource != NULL);

	// ĳ�� ������� �ʴ� ���
	// 1. ĳ�� ����� �Ŵ��� �ܿ��� ���ϴ� ���
	if ( false == m_bCacheEnable )
		return false;
	// 2. ĳ�� ����� ���ҽ��ܿ��� ���ϴ� ���
	if ( RResource::RRCP_NOT_USING == pResource->GetCachePriority() )
		return false;
	// 3. ���ҽ� �ε��� ������ ���
	if (pResource->GetState() != RRS_LOADED)
		return false;

#ifndef MODIFY_LOADER
	// ĳ�û��
	cml2::MCriticalSection::Guard guard(&m_resourceMapLock);

	RRESOURCEHASHMAP& resourceMap = m_resourcesCached[ pResource->GetCachePriority() ];
	_ASSERT( resourceMap.find(pResource->GetName()) == resourceMap.end() );
	resourceMap.insert( RRESOURCEHASHMAP::value_type( pResource->GetName(), pResource ) );
	return true;
#else
	m_CachedResources[pResource->GetCachePriority()].Add(pResource);
	return true;
#endif

}
//----------------------------------------------------------------------------------------------------
inline RResource* RResourceManager::FindFromCacheMapAndSyncToResourceMap( const string &strName )
{
#ifndef MODIFY_LOADER
	// ĳ�� Ȱ��ȭ�� �Ǿ������� ĳ�øʿ��� ã�� ĳ�øʰ� ���ҽ����� ����ȭ ���ֱ�
	if (m_bCacheEnable)
	{
		for (int i = 0; i < RResource::RRCP_MAX; ++i)
		{
			RRESOURCEHASHMAP::iterator itr_cached = m_resourcesCached[i].find(strName);
			if (itr_cached != m_resourcesCached[i].end())
			{
				// ĳ�øʿ��� �����鼭 ĳ�øʿ� �ִ� ���ҽ��� �����ֱ�
				RResource* pResource = itr_cached->second;
				m_resourcesCached[i].erase(itr_cached);

				// resouce�� �߰�
				m_resources.insert( RRESOURCEHASHMAP::value_type( pResource->GetName(), pResource ) );
				return pResource;
			}
		}
	}

	// ĳ�ð� Ȱ��ȭ �Ǿ� ���� �ʰų� ã���� ����.
	return NULL;
#else
	//���ҽ��� ĳ�����ҽ����� ����ȭ �ʿ�
	RResource* pResource = NULL;
	if (m_bCacheEnable)
	{
		for (int i = 0; i < RResource::RRCP_MAX; ++i)
		{
			pResource = m_CachedResources[i].Get(strName);
			if( NULL != pResource )
			{
				m_CachedResources[i].Remove(strName);
				break;
			}
		}

		if( pResource )
		{
			m_Resources.Add(pResource);
		}
	}

	return pResource;
#endif
}

//----------------------------------------------------------------------------------------------------
inline void RResourceManager::SetCacheDuration( RResource::CACHE_PRIORITY _eCacheIndex, float _fDuration )
{
	_ASSERT( _eCacheIndex < RResource::RRCP_MAX && _eCacheIndex != 0 );
	m_cacheDuration[ _eCacheIndex ] = _fDuration;
	m_cacheRemainedTime[ _eCacheIndex ] = _fDuration;

	_ASSERT(m_cacheDuration[ RResource::RRCP_HIGH ] >= m_cacheDuration[ RResource::RRCP_MIDDLE ]);
	_ASSERT(m_cacheDuration[ RResource::RRCP_MIDDLE ] >= m_cacheDuration[ RResource::RRCP_LOW ]);
}
//----------------------------------------------------------------------------------------------------
inline void RResourceManager::SetCacheEnable(bool _bEnable)
{
	// ��Ȱ��ȭ �Ҷ� ĳ�� �����ֱ�
	if (_bEnable == false)
		ClearCache();

	m_bCacheEnable = _bEnable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// general resource manager
//
class RS_API RGeneralResourceManager : public RResourceManager
{
public:
	DECLARE_RES_MGR_ID( RRESOURCETYPE::RR_GENERAL);
};

}
