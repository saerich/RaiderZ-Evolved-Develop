#pragma once

#include "XResourceCache.h"

enum XResourceLoadingPolicy
{
	LOADING_NONE = 0,	// �ε� ����
	LOADING_SYNC,		// ���� �ε�
	LOADING_ASYNC		// �񵿱� �ε�
};

class XResourceRefMgr;
class XFieldInfo;
class XBaseAlwaysResourceGrabber;
class XFieldResourceGrabber;

class XResourceManager
{
private:
	XResourceCache		m_Cache;

	XBaseAlwaysResourceGrabber*		m_pAlwaysResourceGrabber;
	XFieldResourceGrabber*			m_pFieldResourceGrabber;
public:
	XResourceManager();
	virtual ~XResourceManager();

	void Init(XBaseAlwaysResourceGrabber* pAlwaysResourceGrabber);
	void Fini(RDevice* pDevice);

	//////////////////////////////////
	// Caching ///////////////////////
	void InitCaching(rs3::RSceneManager* pSceneManager, const TCHAR* szPreLoad);
	void FiniCaching();
	void UpdateCaching(float fDelta);
	void ClearCaching();
	std::vector< std::tstring >* GetNpcEffectCacheList(int _nId);

	bool IsUsingEffectCache(){return m_Cache.IsUsingEffectCache(); }
	bool IsUsingEffectCacheLog(){ return m_Cache.IsUsingEffectCacheLog(); }
	void LogEffectCache( const std::tstring& _rEffectName, bool bBgCache = false ){ m_Cache.LogEffectCache(_rEffectName, bBgCache); }
	REffectInstanceSceneNode* CreateEffectWithBGCache(const tstring& str){ return m_Cache.CreateEffectWithBGCache(str); }

	//////////////////////////////////
	// Grabber ///////////////////////
	void OnCreateResource(RResource* pResource);
	void GrabAlwaysResources();

	void GrabFieldResource(tstring& strZoneResFile);
	void DropFieldResource();

	//XResourceRefMgr*	GetRefMgr() { return m_pRefMgr; }
};