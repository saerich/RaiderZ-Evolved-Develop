#include "StdAfx.h"
#include "RResource.h"
#include "RResourceID.h"
#include "RResourceLoader.h"

namespace rs3 {

// #define LOG_RESOURCE

RResource::RResource(void) :
	m_nRefCount(1),
	m_state(RRS_EMPTY),
	m_cachePriority(RRCP_MIDDLE),
	m_nResID( RRESOURCETYPE::RR_UNKNOWN ),
	m_pLoader(NULL),
	m_nLoadingPriority(RBGWK_NORMAL),
	m_pResLoadingGroup(NULL)
{
}

RResource::RResource(const char* _pSzResourceName, int nResID) :
	m_nRefCount(1),
	m_state(RRS_EMPTY),
	m_nResID( nResID ),
	m_cachePriority(RRCP_MIDDLE),
	m_strName( _pSzResourceName ),
	m_pLoader(NULL),
	m_nLoadingPriority(RBGWK_NORMAL),
	m_pResLoadingGroup(NULL)
{
}

//set<string>		g_setResources;

RResource::~RResource(void)
{
#ifdef LOG_RESOURCE
	mlog("RResource::~RResource %s %x %s\n", RRESOURCETYPE::GetResoruceIDString(GetResID()), this, GetName().c_str() );
#endif

	//{
	//	if (string(GetDebugName()) != string(""))
	//	{
	//		g_setResources.insert(string(GetDebugName()));
	//	}
	//}

	int nRefCount = GetRefCount();
	if(nRefCount>0)
	{
		dlog("RResource : %s %s not released ( ref count = %d )\n",RRESOURCETYPE::GetResoruceIDString( m_nResID ),m_strName.c_str(),nRefCount);
	}

// 	if( !m_loadingControllers.empty() )
// 	{
// 		// �ε� ��Ʈ�ѷ��� ���������� ��׶��� �Ϸ�� �����ް� ������ �ִ�.
// 		mlog("RResource : %s %s - %d Loading Controller still remains\n",RRESOURCETYPE::GetResoruceIDString( m_nResID ),m_strName.c_str(),m_loadingControllers.size() );
// 	}

	while( !m_loadingControllers.empty() )
	{
		RemoveLoadingController( m_loadingControllers.front() );
	}

// 	RBackgroundWorker& bg_worker = REngine::GetBackgroundWorker();
// 	bg_worker.CancelNotify( this );


	SAFE_DELETE(m_pLoader);
}

int RResource::Release()
{
	--m_nRefCount;
	if (m_nRefCount == 0)
		REngine::GetSceneManager().ReserveResourceToDelete(this);

	return m_nRefCount;
}

bool RResource::BuildAll( bool bBackground, LoadingController* pController )
{
#ifdef LOG_RESOURCE
	mlog("RResource::BuildAll %s %s %x %s\n", RRESOURCETYPE::GetResoruceIDString(GetResID()), bBackground ? "bg" : "" , this, GetName().c_str() );
#endif

	RResourceLoader thisLoader(this);
	if(pController)
		AddLoadingController( pController );

	bool ret = thisLoader.Load(bBackground);

	return ret;
}

void RResource::AddLoadingController(LoadingController* pListener)
{
	m_loadingControllers.push_back(pListener);
	pListener->AddNotify( this );
}

void RResource::RemoveLoadingController(LoadingController* pListener)
{
	// ù��° loadingController �� ã�Ƽ� �����, ���� ȸ���� ����� �Ѵ�
	list<LoadingController*>::iterator itr = find( m_loadingControllers.begin( ), m_loadingControllers.end( ), pListener );
	m_loadingControllers.erase(itr);

	pListener->RemoveNotify( this );
}

// void RResource::RegisterNotify()
// {
// #ifdef LOG_RESOURCE
// 	mlog("RResource::PostNotifyToListeners %s %x %s\n", RRESOURCETYPE::GetResoruceIDString(GetResID()), this, GetName().c_str() );
// #endif
// 
// 	RBackgroundWorker& bg_worker = REngine::GetBackgroundWorker();
// 	bg_worker.RegisterNotify( this );
// }

bool RResource::SendNotify1()
{
	SendNotifyResLoadingGroup();

	if( m_loadingControllers.empty() )
		return false;

	LoadingController* pController = m_loadingControllers.front();
	m_loadingControllers.pop_front();
	pController->RemoveNotify( this );

	pController->OnLoadingComplete( this );

	return true;
}

void RResource::SendNotifyAll()
{
	while( SendNotify1() )
	{
	}
}

void RResource::SendNotifyResLoadingGroup()
{
	if( m_pResLoadingGroup )
	{
		m_pResLoadingGroup->IncrementLoadedResource( this );
		m_pResLoadingGroup = NULL;
	}
}

bool RResource::IsBackgroundCreated()
{
	return GetLoader() && GetLoader()->IsBackgroundCreated();
}

//////////////////////////////////////////////////////////////////////////
// RHandleResource
RHandleResource::RHandleResource() : RResource() , m_handle(R_NONE)
{
}

RHandleResource::~RHandleResource()
{
}

//////////////////////////////////////////////////////////////////////////
// LoadingController
RResource::LoadingController::~LoadingController()
{
//	_ASSERT( m_resources.empty() );

	while( !m_mapResourceNotify.empty() )
	{
		RResource* pResource = (m_mapResourceNotify.begin())->first;
		pResource->RemoveLoadingController( this );
	}
}



RAbstractResourceCreationInfo* RAbstractResourceCreationInfo::Clone( void )
{
	RAbstractResourceCreationInfo* pResourceIdentifier = AllocateCreationInfo();
	pResourceIdentifier->m_Name = this->m_Name;
	OnClone(pResourceIdentifier);
	return pResourceIdentifier;
}

}
