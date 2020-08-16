#ifndef _XEFFECT_MANAGER_H
#define _XEFFECT_MANAGER_H

#include "XEffect.h"
#include "XActor.h"
#include "RActorNode.h"
#include "XEffectPreLoaderResourceMgr.h"
#include "XEffectWaitActorLoadingComplete.h"
#include "XEffectPostEffect.h"
#include "XCommonData.h"
#include "XDecalEffect.h"
#include "RDecalPolygonSceneNode.h"
#include "XHitEffectSelecter.h"
#include "XSelfIlluminationEffect.h"
#include "XLightEffect.h"
#include "XNoiseFadeEffect.h"
#include "XWeaponEffect.h"
#include "XPEEdgeMgr.h"

class XCharacter;

struct stEffectReserveData
{
	XEffectBase*			pEffect;
	EFFECT_CREATE_DATA		effectData;
};

class XEffectManager
{
	friend XEffectWaitActorLoadingComplete;

private:
	XEffectPreLoaderResourceMgr effectPreLoader;
	XEffectWaitActorLoadingComplete effectWait;
	XEffectPostEffect effectPostEffect;
	RDecalPolygonSceneNode* m_pEffectDecalPolygon;
	XHitEffectSelecter	m_HitEffectSelecter;
	XPEEdgeMgr m_PEEdgeMgr;

	vector<stEffectReserveData>	m_vecEffectReserve;

private:
	void		UpdateXEffect(float fDelta);
	void		UpdataXEffectReserve();

	void		AddXEffectReserve(XEffectBase* pEffect, EFFECT_CREATE_DATA& effectCreaetData);
	void		DelXEffectReserve(MUID& uid);

	MUID		AddXEffect(XEffect* pEffect, EFFECT_CREATE_DATA& effectCreateData);
	MUID		AddXWeaponEffect(XWeaponEffect* pEffect, EFFECT_CREATE_DATA& effectCreateData);

	bool		CheckDuplicationSelfIlluminationEffect(MUID& uidOwner, wstring& strActorNodeName);
	bool		CheckWeaponEffectDummy(XCharacter* pActor, wstring& strBoneName);

	void		AttachSceneNode(RSceneNode* pEffectInst, RSceneNode* pSceneNode, RActorNode* pActorNode, const RMatrix& _rBaseFrame = RMatrix::IDENTITY, bool bIgnoreDir = false);

protected:
	vector<XEffectBase*>		m_vEffectList;

public:
	XEffectManager();
	virtual ~XEffectManager();
	virtual void		OnUpdate(float fDelta);
	virtual void		Destroy();
	virtual void		Clear();
	
	virtual XEffect*	CreateXEffect(const wstring& szEffectName);
	virtual XDecalEffect*	CreateXDecalEffect(const wstring& szEffectName, BYTE byDeclaEffectType, const vec3& vPos, const float fSizeX, const float fSizeY, const float fSizeZ);
	virtual XSelfIlluminationEffect* CreateXSelfIlluminationEffect(float fStartTime, float fStartDurationTime, float fEndTime, float fEndDurationTime, float fMin, float fMax);
	virtual XLightEffect* CreateXLightEffect();
	virtual XNoiseFadeEffect* CreateNoiseFadeEffect(float fStartDurationTime, float fEndDurationTime);
	virtual XWeaponEffect* CreateWeaponEffect(const wstring& szEffectName);
	
	virtual void		AttachXEffect(XModelEffect* pEffect, RSceneNode* pSceneNode, RActorNode* pActorNode, const RMatrix& _rBaseFrame = RMatrix::IDENTITY, bool bIgnoreDir = false);
	virtual void		AttachXDecalEffect(XDecalEffect* pDecalEffect, RSceneNode* pSceneNode, RActorNode* pActorNode);

	size_t		GetCount();	///< ���� ����Ʈ�� ���� �����Ѵ�

	// ���� ��ǥ�� ���̴� ����Ʈ
	bool		DeleteXEffect(MUID & uid, bool bImmediatlyDelete = false);				// bImmediatlyDelete�� true�̸� ��� ����, false�̸� ������ �����
	
	XEffectBase* GetEffect(MUID& uid);
	XEffect*	GetXEffect(MUID& uid);
	XModelEffect*	GetXModelEffect(MUID& uid);
	XDecalEffect* GetXDecalEffect(MUID& uid);
	XLightEffect* GetXLightEffect(MUID& uid);
	XWeaponEffect* GetXWeaponEffect(MUID& uid);

	void		SetScaleInit(XEffect* pEffect, RSceneNode* pSceneNode, RActorNode* pActorNode);
	bool		SetScaleXEffect(MUID& uid, bool bAutoScale, vec3 scale);
	bool		SetScaleXEffect(XEffect* pEffect, bool bAutoScale, vec3 scale);

	// ����Ʈ ���̱�
	MUID		AddXEffect(const wstring& szEffectName, XCharacter* pActor, RActorNode* pActorNode, 
							const RMatrix& _rBaseFrame = RMatrix::IDENTITY, 
							bool bLastFramePause = false, bool bIgnoreDir = false, bool bImmediatlyDelete = false, float fDurationTime = 0.0f,
							float fStartVisibilityTime = 0.0f, float fEndVisibilityTime = 0.0f);
	MUID		AddXEffect(EFFECT_CREATE_DATA& effectCreateData);

	// ��Į ����Ʈ
	MUID		AddXDecalEffect(EFFECT_CREATE_DATA& effectCreateData, BYTE byDeclaEffectType, const DWORD dwColor, const float fSizeX, const float fSizeY, const float fSizeZ);

	// ȿ�� ����Ʈ
	void		SetPostEffect(BYTE byPostEffectType, XObject* pObject, float fPlayTime, float fScatTime, float fSpeed, BYTE nScarFactorType, int nBlurType);

	// Illumination ����Ʈ
	MUID		AddXSelfIllumination(EFFECT_CREATE_DATA& effectCreateData, float fStartTime, float fStartDurationTime, float fEndTime, float fEndDurationTime, float fMin, float fMax, bool bLoop);

	// Light ����Ʈ
	MUID		AddXLightEffect(XObject* pObject, wstring& strBoneName, float fRadius, float fAttenuationStart, float fIntensity, const DWORD dwColor, float fDurationTime, bool bFollow, RMatrix _rWorld = RMatrix::IDENTITY);

	// Noise Fade ����Ʈ
	void		SetXNoiseFadeEffect(XObject* pObject, vector<tstring> vecMeshNodeName, float fStartDurationTime, float fEndDurationTime, DWORD dwColor1, DWORD dwColor2, float fSize1, float fSize2, bool bInverse);

	// �� ����Ʈ
	MUID		AddXWeaponEffect(EFFECT_CREATE_DATA& effectCreateData);

	// �ܰ���
	void		EnablePEEdge(MUID& uid, const RVector4& _color, float _fTickNess, float _fBlurFactor, bool _bProjection);
	void		DisablePEEdge(MUID& uid);

	// ��Ÿ...
	MUID		AddEffectWaiting(EFFECT_CREATE_DATA& effectWaitingData);
	MUID		AddWeaponEffectWaiting(EFFECT_CREATE_DATA& effectWaitingData);
	void		AddHitInfo(XTalentHitParam* pHitInfo, unsigned int nDelayTime);

	bool		SetTransform(MUID& uid, vec3& vPos, vec3& vDir, vec3& vUp, RMatrix& matOutResult, bool bTerrainAlign = false, bool bTerrainPos = false);

	void		PreLoaderXEffect(const wstring& strEffectName);

	void		ActorLoadingComplete(MUID uidObject);
	void		ActorChangeMotion(MUID uidActor);

	void		StartHitEffectSelect(XTalentHitParam* pHitParam);

};

//////////////////////////////////////////////////////////////////////////
// inline
inline size_t XEffectManager::GetCount()	{ return m_vEffectList.size(); }

#endif