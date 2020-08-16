#pragma once

namespace rs3 {

class RDevice;
class RResourceManager;
class RBackgroundWorker;
class RSceneNodeFactory;

// ����: static ��ǵ��� dll �϶� ����� �������� �ʴ´�

class RS_API REngine : public MSingleton<REngine>
{

public:
	REngine(RDevice* pDevice);
	virtual ~REngine();

	//////////////////////////////////////////////////////////////////////////
	// ���� / �� / ������Ʈ / �ʱ�ȭ
	//
	bool							Create();
	void							Destroy();

	void							Update();
	void							Clear();


	//////////////////////////////////////////////////////////////////////////
	// timer(main tick)
	//
	const RTimer&					GetTimer()	{ return timer;  }		// rs3 ���� Ÿ�̸�
	void							ResetTimer(){ timer.Reset(); }		// Ÿ�̸� ����


	//////////////////////////////////////////////////////////////////////////
	// background worker
	//
	RBackgroundWorker*				GetBackgroundWorker(){ return m_pBackgroundWorker; }
	void							CreateBackgroundWorker();
	void							ClearBackgroundWorker();


	//////////////////////////////////////////////////////////////////////////
	// resource
	//
	template< typename T_ResourceMgr >
	T_ResourceMgr*					GetResourceMgr(bool _bCreateIfNotExist = true);

	template< typename T_Resource >
	T_Resource*						CreateResource( const char* _pSzResourceName );
	void							ReleaseResource( RResource* _pResource);
	void							ReserveResourceToDelete( RResource* _pResource );

private:

	//////////////////////////////////////////////////////////////////////////
	// resource
	//
	RResourceManager**				m_arrayResourceMgr;			// resource manager container
	std::vector< RResource* >		m_resourcesToDelete;		// ���� ���ҽ�
	RBackgroundWorker*				m_pBackgroundWorker;		// Background Worker

	template< typename T_ResourceID >
	RResourceManager*				GetResourceMgrFromResource(RResource* _pResource);	//���ҽ��κ��� RResourceManager������
};

//----------------------------------------------------------------------------------------------------
template< typename T_ResourceMgr >
inline T_ResourceMgr* REngine::GetResourceMgr(bool _bCreateIfNotExist)
{
	_ASSERT( m_arrayResourceMgr );
	RResourceManager* pResourceMgr = m_arrayResourceMgr[ T_ResourceMgr::RES_MGR_ID ];
	if( pResourceMgr == NULL && _bCreateIfNotExist )
	{
		pResourceMgr = new T_ResourceMgr;
		pResourceMgr->Init();

		m_arrayResourceMgr[ T_ResourceMgr::RES_MGR_ID ] = pResourceMgr;
	}
	return static_cast<T_ResourceMgr*>(pResourceMgr);
}
//----------------------------------------------------------------------------------------------------
template< typename T_Resource >
inline T_Resource* REngine::CreateResource( const char* _pSzResourceName )
{
	_ASSERT( m_arrayResourceMgr );
	RResourceManager* pResourceMgr = m_arrayResourceMgr[ T_Resource::T_RES_MGR::RES_MGR_ID ];
	if( pResourceMgr == NULL )
	{
		pResourceMgr = new T_Resource::T_RES_MGR;
		pResourceMgr->Init();

		m_arrayResourceMgr[ T_Resource::T_RES_MGR::RES_MGR_ID ] = pResourceMgr;
	}

	// �ߺ����� reference count üũ
	RResource* pResource = pResourceMgr->GetResource(_pSzResourceName);

	if(pResource)	// �̹� �ִٸ�..
		pResource->AddRef();
	else
	{
		pResource = new T_Resource( _pSzResourceName );
		pResourceMgr->AddResource( pResource );
	}

	return static_cast< T_Resource* >( pResource );
}
//----------------------------------------------------------------------------------------------------
template< typename T_ResourceID >
inline RResourceManager* REngine::GetResourceMgrFromResource(RResource* _pResource)
{
	_ASSERT( _pResource );
	int nResID = _pResource->GetResID();
	_ASSERT( nResID >= 0 && nResID < T_ResourceID::RR_MAX );
	return m_arrayResourceMgr[ nResID ];
}
//----------------------------------------------------------------------------------------------------
}
