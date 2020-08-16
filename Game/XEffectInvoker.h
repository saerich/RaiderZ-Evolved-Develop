#ifndef _XEFFECT_INVOKER_H
#define _XEFFECT_INVOKER_H

#pragma once

#include "XEffectBaseInvoker.h"
#include "XCommonData.h"

class XEffectInvoker : public XEffectBaseInvoker
{
protected:
	XObject	*				m_pOwner;
	XObject *				m_pDestObject;

private:
	bool					CheckUseMotionBlur(BYTE nPostEffecType, float fPlayTime, float fScatTime, float fSpeed, int nDistance, BYTE nScarFactorType, BYTE nUseType);

	tstring					GetArmorTypeSoundName(ARMOR_ATTRIB nArmorType);
	tstring					GetSoundTypeSoundName(SOUND_TYPE nSoundType);

protected:
	virtual wstring			GetReserved_Param(wstring& strEffectName);
	virtual wstring			GetReserved_BoneName(wstring& strEffectName);
	virtual wstring			GetReserved_SoundName(wstring& strEffectName, wstring& strEffectparam, unsigned int nFlags);
	virtual wstring			GetReserved_ModelName(wstring& strEffectName);

	virtual MUID			SetSound(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_Sound& soundInfo, EFFECT_CREATE_RESULT_DATA& resultData);
	virtual MUID			SetModel(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_Model& modelInfo, EFFECT_CREATE_RESULT_DATA& resultData);
	virtual void			SetCamera(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_Camera& cameraInfo);
	virtual void			SetPostEffect(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_PostEffect& postInfo);
	virtual MUID			SetDecalEffect(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_DecalEffect& decalInfo);
	virtual MUID			SetIlluminationEffect(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_IlluminationEffect& illuminationInfo);
	virtual MUID			SetLightEffect(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_LightEffect& lightInfo);
	virtual void			SetAlphaTestFadeEffect(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_AlphaTestFadeEffect& fadeInfo);

	bool					CheckActorLoadingComplete_Effect_Model(EFFECT_CREATE_DATA& _effectData, MUID& uidOut);
	bool					CheckPlayCameraEffect();
	bool					CheckInWater(vec3& vPos, float fHeightMin, float fHeightMax);


	wstring					GetHitSoundName( wstring strPrefix, wstring strPostfix, XObject* pAttacker, XObject* pVictim, XTalentInfo* pAttackTalentInfo, unsigned int nDamageFlags);
	wstring					GetGuardSoundName( wstring strPrefix, XObject* pAttacker, XObject* pVictim);

public:
	XEffectInvoker();
	virtual ~XEffectInvoker(void);

	void					Invoke(XObject* pSourceObject, wstring& strEffectName, wstring& strEffectParam, wstring& strEffectBoneName, vector<EFFECT_RESULT_DATA>* vecOutData = NULL, XObject* pDestObject = NULL, EFFECT_POST_TRANSFORM* pPostData = NULL);

	void					GetEffect_Data(CUSTOM_EFFECT_TYPE eEffectType, XObject* pObject, XObject* pDestObject, wstring& strEffectName, wstring& strEffectParam, wstring& strEffectBoneName, vector<CHECK_EFFECT_DATA>* vecOutData);
};

//////////////////////////////////////////////////////////////////////////
// �����ϰ� ����Ʈ �Ϲ�ȭ�� ����ϱ� ���Ѱ�
class XEffectSinglyInvoker : public XEffectInvoker
{
protected:
	virtual MUID			SetSound(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_Sound& soundInfo, EFFECT_CREATE_RESULT_DATA& resultData);
	virtual MUID			SetModel(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_Model& modelInfo, EFFECT_CREATE_RESULT_DATA& resultData);
	virtual void			SetCamera(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_Camera& cameraInfo);
	virtual void			SetAlphaTestFadeEffect(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_AlphaTestFadeEffect& fadeInfo) { return; }

public:
	XEffectSinglyInvoker();
	virtual ~XEffectSinglyInvoker(void);

	void					Invoke(wstring& strEffectName, vec3& vPos, vec3& vDir, vec3& vUp, vector<EFFECT_RESULT_DATA>* vecOutData = NULL, EFFECT_POST_TRANSFORM* pPostData = NULL);
};

//////////////////////////////////////////////////////////////////////////
// �̸� ����Ʈ ������ �ε��ϱ� ���Ѱ�... �ַ� Effect_Model�� ����ϰ� ��
class XEffectAdvenceInvoker : public XEffectInvoker
{
protected:
	virtual MUID			SetModel(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_Model& modelInfo, EFFECT_CREATE_RESULT_DATA& resultData);

	virtual MUID			SetSound(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_Sound& soundInfo, EFFECT_CREATE_RESULT_DATA& resultData)			{ return MUID::ZERO; }
	virtual void			SetCamera(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_Camera& cameraInfo)			{}
	virtual void			SetPostEffect(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_PostEffect& postInfo) {}
	virtual MUID			SetDecalEffect(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_DecalEffect& decalInfo) { return MUID::ZERO; }
	virtual MUID			SetIlluminationEffect(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_IlluminationEffect& illuminationInfo) { return MUID::ZERO; }
	virtual MUID			SetLightEffect(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_LightEffect& lightInfo) { return MUID::ZERO; }
	virtual void			SetAlphaTestFadeEffect(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_AlphaTestFadeEffect& fadeInfo) { return; }

public:
	XEffectAdvenceInvoker();
	virtual ~XEffectAdvenceInvoker(void);

	void					Invoke(wstring& strEffectName);
};

//////////////////////////////////////////////////////////////////////////
// Į ����Ʈ�� ���Ѱ�... Į �ٿ��� ��带 ��� ã�Ƽ� ���δ�.
class XEffectWeaponInvoker : public XEffectInvoker
{
private:
	void					SetWeaponEffectInvoker(EFFECT_TRANSEMISION_DATA& _data, const float fEffectDurationTime, vector<EFFECT_RESULT_DATA>* vecOutData);
	void					SetWeaponEffect(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_Model * pEffectInfoItem_Model, vector<EFFECT_RESULT_DATA>* vecOutData);

	void					SetWeaponEffectAllDummyNode(EFFECT_TRANSEMISION_DATA& _data, XEffectInfoItem_Model * pEffectInfoItem_Model, vector<EFFECT_RESULT_DATA>* vecOutData);
protected:
	// �� �ٸ��ϴ�.
	MUID					SetModel(EFFECT_TRANSEMISION_DATA& _data, bool bFollow, bool bTerrainAlign, bool bTerrainPos, bool bWaterPos, bool bAlign, bool bAutoScale, bool bImmediatlyDelete, vec3 vScale, float fDurationTime, float fStartVisibilityTime, float fEndVisibilityTime, bool bLastFramePause);

	bool					CheckActorLoadingComplete_Effect_Weapon(EFFECT_CREATE_DATA& _effectData, MUID& uidOut);

public:
	XEffectWeaponInvoker();
	virtual ~XEffectWeaponInvoker(void);

	void					Invoke(XObject* pSourceObject, wstring& strEffectName, wstring& strBoneName, int nPartsSlotType, vector<EFFECT_RESULT_DATA>* vecOutData, XObject* pDestObject = NULL);

};

//////////////////////////////////////////////////////////////////////////
// Ŭ���̾�Ʈ������ ����Ʈ ����
class XEffectDelete
{
public:
	static bool				Del(CUSTOM_EFFECT_TYPE eType, MUID& uidEffect);
	static void				DelList(vector<EFFECT_RESULT_DATA>& effectList);
};

#endif
