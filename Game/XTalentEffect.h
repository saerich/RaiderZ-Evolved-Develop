#ifndef _XTALENT_EFFECT_H
#define	_XTALENT_EFFECT_H

#pragma once

#include "XEffectManager.h"
#include "XTalentEffectHitCapsule.h"
#include "XEffectInvoker.h"

class XTalentEffectInfo;

struct CREATE_TALENT_EFFECT_DATA
{
	XTalentInfo * pTalentInfo;

	XObject* pOwner;
	XObject* pAttacker;

	TALENT_EFFECT_TYPE nType;

	vec3 effectPos;

	unsigned int nDamageFlags;

	bool bEffectStorage;

	MMatrix matEffect;

	CREATE_TALENT_EFFECT_DATA()
	{
		pTalentInfo		= NULL;
		pOwner			= NULL;
		pAttacker		= NULL;

		nType			= CAST_EFFECT;

		effectPos		= vec3::ZERO;

		nDamageFlags	= 0;

		bEffectStorage	= false;

		matEffect.MakeIdentity();
	}
};

struct TALENT_EFFECT_INFO_DATA
{
	XTalentEffectInfo* pEffectInfo;

	XObject* pOwner;
	XObject* pAttacker;

	vec3 effectPos;
	vec3 effectDir;
	
	MMatrix matEffect; 
	
	unsigned int nDamageFlags;

	vector<EFFECT_RESULT_DATA> vecOutResultData;

	TALENT_EFFECT_INFO_DATA()
	{
		pEffectInfo			= NULL;
		pOwner				= NULL;
		pAttacker			= NULL;

		effectPos			= vec3::ZERO;
		effectDir			= vec3::AXISY;

		matEffect.MakeIdentity();
	}

	void Set(XTalentEffectInfo* pEffectAttInfo, XObject* pObj, vec3 pos, vec3 dir, MMatrix mat, XObject* pDestObj, unsigned int nFlags)
	{
		pEffectInfo = pEffectAttInfo;
		pOwner = pObj;
		pAttacker = pDestObj;
		effectPos = pos;
		effectDir = dir;
		matEffect = mat;
		nDamageFlags = nFlags;
	}
};

//////////////////////////////////////////////////////////////////////////
class XTalentEffect
{
private:
	typedef void(*TALENTEFFECTHANDLER) (TALENT_EFFECT_INFO_DATA* );
	map<TALENT_EFFECT_TYPE, TALENTEFFECTHANDLER> m_mapTalentEffectHandlers;

	vector<EFFECT_DEL>	m_vecDelEffectList;
	vector<MUID>		m_vecDelSoundList;

	XTalentEffectHitCapsule talentEffectHitCapsule;

	bool		m_bHaveAttackSound;
	bool		m_bCheckAttackSound;

private:
	void		CreateAttackBloodEffect(XObject* pObj, vec3 pos, vec3 dir, bool bCri, float fHitBloodScale, MMatrix matEffect);

	void		CreateAttackDefaultTalentEffect(CREATE_TALENT_EFFECT_DATA& creatTalentEffectData);
	void		CreateDefenseDefaultTalentEffect(XObject* pObj, vec3 pos , XObject* pOtherObj = NULL);

	vec3		GetTalentEffectDir(XTalentEffectInfo* pTalentEffectInfo, XObject* pObj_1, XObject* pObj_2);

	void		CheckAttackEffectSound(vector<EFFECT_RESULT_DATA>& vecEffectResult);

public:
	XTalentEffect();
	virtual ~XTalentEffect();

	//------------------------------------------------------------------------
	// �ŷ�Ʈ ����Ʈ
	/************************************************************************/
	/* 1. �ŷ�Ʈ ����Ʈ�� �ߺ��� �����ϴ�.                                  */
	/* 2. ������ ���Ƽ� ����Ʈ�� �����ؾ� �Ѵ�.								*/
	/* 3. uid�� �ʿ�� �ϴ� Ŭ�������� �ִ�. �׷��Ƿ� �⺻������ uid�� ���� */
	/*	  �� �� �ְ� �ؾ� �ϹǷ� vector<MUID>* vecUIDEffectStorage = NULL   */
	/*    �Ķ���Ͱ� �ִ�.													*/
	/************************************************************************/
	static void	CreateCastEffect(TALENT_EFFECT_INFO_DATA* pInfo);
	static void	CreateCastEndEffect(TALENT_EFFECT_INFO_DATA* pInfo);
	static void	CreateUseEffect(TALENT_EFFECT_INFO_DATA* pInfo);
	static void	CreateAttackEffect(TALENT_EFFECT_INFO_DATA* pInfo);
	static void	CreateMissEffect(TALENT_EFFECT_INFO_DATA* pInfo);
	static void	CreateShockEffect(TALENT_EFFECT_INFO_DATA* pInfo);
	static void	CreateDurationEffect(TALENT_EFFECT_INFO_DATA* pInfo);
	static void	CreateDefenseEffect(TALENT_EFFECT_INFO_DATA* pInfo);
	static bool	CreateEnchantHitEffect(TALENT_EFFECT_INFO_DATA* pInfo);


	void		CreateByTalentEffectType(CREATE_TALENT_EFFECT_DATA& creatTalentEffectData);
	void		CreateDefaultTalentEffect(CREATE_TALENT_EFFECT_DATA& creatTalentEffectData);		// XTalentEffectDataMgr�� NULL�̸� �⺻������ �ѷ��ִ� ����Ʈ�̴�.

	void		ClearEffect();

	void		Update(float fDelta);
};

#endif // _XTALENT_EFFECT_H
