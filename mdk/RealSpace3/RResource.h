#pragma once

#include <list>
#include <string>
#include <map>
#include "MThread2.h"
#include "RResourceID.h"

#define DECLARE_RES_MGR_ID(x)		enum { RES_MGR_ID = x };
#define DECLARE_USING_RES_MGR(x)	typedef x T_RES_MGR;

using namespace std;

namespace rs3 {

class RResourceManager;
class RSceneManager;
class RDevice;
class RDeviceD3D;
class RHandleResourceManager;
class RResourceLoader;
class RResourceLoadingGroup;
class RBackgroundWorker;
class RWorldSceneNode;

////////////////////////////////////////////////////////////////////////////////////////////////////
// ���ҽ� ����
//
enum RRSTATE
{
	RRS_EMPTY	= 0,		///< ó�������Ǿ� ����ִ� ����
	RRS_FILLED,				///< IO�κ��� ���ҽ� �ε������� �о���� ����
	RRS_LOADED,				///< ����̽��� �� �� �ֵ��� �ε��� ����
	RRS_LOST,				///< �ε�� �Ǿ����� ����̽����� �� �� ���� ����

	RRS_WRONG				///< �߸��� ���ҽ�
};

class RResourceProvider;
class RAbstractResourceCreationInfo;
class RLinker;
class RESOURCELINKTYPE_RESOURCECREATIONINFO;
class RResourceWorkUnit;
////////////////////////////////////////////////////////////////////////////////////////////////////
//  ���ҽ� Ŭ����
//  ���۷��� ī��Ʈ �� ���̵�� ���ҽ� �Ŵ����� ���� ���ҽ� ����
//
class RS_API RResource
{
	friend RResourceManager;
	friend RDevice;
	friend RResourceLoader;
	friend RBackgroundWorker;
	friend RResourceProvider;
	friend RDeviceD3D;
	friend RResourceWorkUnit;

public:
	class RS_API LoadingController
	{
		friend RResource;

	public:
		virtual ~LoadingController();

		virtual int		CalcPriority()					{ return 0; }	// RBGWK_NORMAL
		virtual void	OnLoadingComplete(RResource*)	{}

		size_t	GetResourceNotifyCount() { return m_mapResourceNotify.size(); }
		int		GetNotifyCount(RResource* pResource )
		{
			map<RResource*,int>::iterator itr = m_mapResourceNotify.find(pResource);
			if(itr!=m_mapResourceNotify.end())
				return itr->second;
			return 0;
		}
	private:
		map<RResource*,int>		m_mapResourceNotify;	///< ���� �Ϸ������� �ް��� �ϴ� ���ҽ���, second : �������� ȸ��
		
		void AddNotify( RResource* pResource )
		{
			map<RResource*,int>::iterator itr = m_mapResourceNotify.find(pResource);
			if(itr!=m_mapResourceNotify.end())
			{
				(itr->second)++;
				return;
			}

			m_mapResourceNotify.insert( map<RResource*,int>::value_type( pResource, 1) );
		}

		void RemoveNotify( RResource* pResource	)
		{
			map<RResource*,int>::iterator itr = m_mapResourceNotify.find(pResource);
			if(itr!=m_mapResourceNotify.end())
			{
				if(itr->second == 1 )
					m_mapResourceNotify.erase( itr );
				else
					itr->second--;
				return;
			}
			_ASSERT( FALSE);
		}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������ / �Ҹ���
	//
	RResource(void);
	RResource( const char* _pSzResourceName, int nResID );
	virtual ~RResource(void);


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���ҽ� ���̵� / �̸� / ���۷��� ī��Ʈ
	//
	int							GetResID();
	const std::string&			GetName() const;
	virtual const char*			GetDebugName() { return GetName().c_str(); }

	int							AddRef();
	int							GetRefCount() const;
	void						SetRefCount(int RefCount) { m_nRefCount = RefCount; }


	//////////////////////////////////////////////////////////////////////////
	// State
	RRSTATE						GetState() const;

	/// �ε��� �����°� ����, state �� loaded �̰ų� wrong �ΰ���̴�
	bool						IsLoadingCompleted() const;

	/// �ε��� �����ߴ°�?
	bool						IsLoadingSucceeded() const;

	// TODO: �� �Լ��� �ʿ��Ѱ� ?
	void						ForceSetState( RRSTATE _eState )	{ m_state = _eState; }

	void						SetResourceLoadingGroup( RResourceLoadingGroup* pResLoadingGroup ) { m_pResLoadingGroup = pResLoadingGroup; }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ��ü ���� / �Ҹ�
	//
	virtual bool				BuildAll( bool bBackground = false, LoadingController* pController = NULL );
	virtual int					Release();		// Safe Release. RefCount�� 0�̵Ǹ� �� �ൿ���� �������־���Ѵ�.

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ڿ�
	//
	virtual int					GetUsingSystemMemory()	{ return 0; }
	virtual int					GetUsingVideoMemory()	{ return 0; }

	// ĳ��
	enum CACHE_PRIORITY
	{
		RRCP_NOT_USING	= -1,	///< ĳ�� Ȱ��ȭ ���¶� ĳ�̵��� �ʴ� ���ҽ�.
		RRCP_ALWAYS		= 0,	///< update�� �������� �ʰ� �׻� Caching �Ǿ� ����. ClearCache �� ���� ������

		RRCP_HIGH,
		RRCP_MIDDLE,
		RRCP_LOW,

		RRCP_MAX
	};

	enum CACHE_TYPE
	{
		SYSTEM_MEMORY = 0,
		VIDEO_MEMORY
	};

	void						SetCachePriority( CACHE_PRIORITY _eCachePriority );
	CACHE_PRIORITY				GetCachePriority();

	//////////////////////////////////////////////////////////////////////////
	// ��׶��� �ε� ����
	int							GetLoadingPriority();				///< ��׶��� �ε��� �켱����
	virtual void				SetLoadingPriority(int nPriority);
	RResourceLoader*			GetLoader()			{ return m_pLoader; }

	bool						IsBackgroundCreated();	///< ��׶��� �ε����� �ε��� �õ��Ǿ����� ����
	bool						IsBuildLater();			///< ���带 ResourceLoadingGroup���� ��

	void						AddLoadingController(LoadingController* pListener);
	void						RemoveLoadingController(LoadingController* pListener);

public:
	virtual bool				Invalidate()	{ return true; }	// device�� ������ ���ҽ��ϰ�� ��ġ reset�ÿ�
	virtual bool				Restore()		{ return true; }

protected:

	enum LOAD_RESULT {
		WRONG = 0,
		LOADED,
		WAITING,
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���º� ���ҽ� ó�� / ����
	//
	virtual bool				Fill()			{ return true; }	// ���ҽ��� ������ ä��� // ������ �����Ͱ� �ε��Ǵ� �κ��̴�. XML ���Ͽ��� ���� �̸��� ���� �Ǵ� ���� ������ �־� �����߳� - COMMENT OZ
	virtual bool				Erase()			{ return true; }	// ���ҽ��� ������ ���� : ���� RMesh�� ����
	virtual LOAD_RESULT			Load()			{ return LOADED; }	// ����̽��� �ε�

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �̸� / ���̵� / ���۷��� ī��Ʈ
	//
	int							m_nRefCount;
	int							m_nResID;
	CACHE_PRIORITY				m_cachePriority;
	int							m_nLoadingPriority;
	std::string					m_strName;

	RResourceLoader*			m_pLoader;

	typedef list<LoadingController*>	LOADINGCONTROLLERLIST;
	list<LoadingController*>			m_loadingControllers;

	RResourceLoadingGroup*		m_pResLoadingGroup;

//	void						RegisterNotify();			///< �Ϸ� ����
	
	bool						SendNotify1();		///< �ϳ��� �����ʿ��� ����, ������ �����ʰ� ������ false�� �����Ѵ�
	void						SendNotifyAll();	///< ��� �����ʵ鿡�� ����
	void						SendNotifyResLoadingGroup(); ///< ���ҽ� �ε� �׷쿡 �Ϸ� ����


private:
	RRSTATE						m_state;
};


//----------------------------------------------------------------------------------------------------
inline int RResource::AddRef()			{ return ++m_nRefCount; }
//----------------------------------------------------------------------------------------------------
inline int RResource::GetRefCount() const { return m_nRefCount; }
//----------------------------------------------------------------------------------------------------
inline const string& RResource::GetName() const { return m_strName; }
//----------------------------------------------------------------------------------------------------
inline int RResource::GetResID()		{ return m_nResID; }
//----------------------------------------------------------------------------------------------------
inline RRSTATE RResource::GetState() const	{ return m_state; }
//----------------------------------------------------------------------------------------------------
inline bool RResource::IsBuildLater() { return (m_pResLoadingGroup != NULL); }
//----------------------------------------------------------------------------------------------------
inline int RResource::GetLoadingPriority()	{ return m_nLoadingPriority; }
// //----------------------------------------------------------------------------------------------------
inline void RResource::SetLoadingPriority(int nPriority)	{ m_nLoadingPriority = nPriority; }
//----------------------------------------------------------------------------------------------------
inline void RResource::SetCachePriority( CACHE_PRIORITY _eCachePriority )
{
	if (GetRefCount() <= 0 )
	{
		_ASSERT( !"ĳ�����̰ų� ���۷��� ī���Ͱ� �ùٸ��� �ʽ��ϴ�." );
		return;
	}
	_ASSERT( _eCachePriority < RRCP_MAX );
	m_cachePriority = _eCachePriority;
}
//----------------------------------------------------------------------------------------------------
inline RResource::CACHE_PRIORITY RResource::GetCachePriority(){ return m_cachePriority; }
//----------------------------------------------------------------------------------------------------
inline bool RResource::IsLoadingCompleted() const { return (m_state==RRS_LOADED || m_state==RRS_WRONG); }
//----------------------------------------------------------------------------------------------------
inline bool RResource::IsLoadingSucceeded() const { return (m_state==RRS_LOADED); }
//----------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////
// �ڵ��� ���ҽ�
//
class RS_API RHandleResource : public RResource
{
	friend RHandleResourceManager;

protected:
	RHANDLE			m_handle;

public:
	RHandleResource(void);
	virtual ~RHandleResource(void);

	RHANDLE		GetHandle();
};
//----------------------------------------------------------------------------------------------------
inline RHANDLE RHandleResource::GetHandle()	{ return m_handle; }

////////////////////////////////////////////////////////////////////////////////////////////////////
// �Ϲ����� �뵵�� ��밡���� ���ҽ�
// �� Ŭ������ ��� ������ RGeneralResourceManager�� ����Ͽ� �����˴ϴ�.
//
class RGeneralResourceManager;
class RS_API RGeneralResource : public RResource
{
public:
	DECLARE_USING_RES_MGR(RGeneralResourceManager);
	RGeneralResource(const char* _pSzResourceName, RSceneManager* pSceneManager) : RResource(_pSzResourceName,RRESOURCETYPE::RR_GENERAL)
	{
	}
};


class RS_API RAbstractResourceCreationInfo
{
public:
	RAbstractResourceCreationInfo(void) : m_Name(""){}
	RAbstractResourceCreationInfo(std::string ResourceName) : m_Name(ResourceName){}

public:
	virtual int GetResourceType(void) = 0;
	RAbstractResourceCreationInfo* Clone(void);

	bool	HasResourceName(void)			{	return m_Name=="" ? false : true;	}
	bool	IsMakedResourceSharable(void)	{	return HasResourceName();	}

protected:
	virtual	RAbstractResourceCreationInfo*	AllocateCreationInfo(void) = 0;
	virtual void					OnClone(RAbstractResourceCreationInfo* pIdentifierToFill) = 0;

public:
	std::string m_Name;
};



}
