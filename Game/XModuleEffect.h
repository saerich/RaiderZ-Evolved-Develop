#ifndef _XMODULE_EFFECT_H
#define _XMODULE_EFFECT_H

#include "XModule.h"
#include "XHeadEffect.h"
//#include "XInteractionEffect.h"
#include "XBuffEffect.h"
#include "XNPCIConEffect.h"
#include "XTalentEffect.h"
#include "XSwordTrail.h"
#include "XModuleEffectReserve.h"
#include "XModuleActorControl.h"
#include "XProjectile.h"

#define	REBIRTH_EFFECT_NAME		L"player_rebirth"

//////////////////////////////////////////////////////////////////////////
// 1. SetRegisterUseEffectID()�� ���ؼ� ����� ����Ʈ�� ����� �ϼž� �մϴ�.
// 2. ModuleEffect�� ���ؼ� ����ϴ� ����Ʈ�� ��Ƽ� Update���� ó���ؾ� �մϴ�.
// 2-1. ��Ƽ� ó���� �ҷ��� ModuleEffect�� ó���� �ؾ� �ϴ� �͵��� ��� �̺�Ʈ�� ó���� �ؾ� �մϴ�.
// 2-2. �Լ��� Effect�� ó���ϴ� ���� �̺�Ʈ�� ó���ؾ� �մϴ�.
//////////////////////////////////////////////////////////////////////////
enum XEffectEventID
{
	XEFTEVT_UNKNOWN = 0,

	// Base
	XEFTEVT_EFFECT_MF_STUN,				///< ���� ����Ʈ
	XEFTEVT_EFFECT_END_MF_STUN,			///< ���� ���� �̺�Ʈ
	XEFTEVT_EFFECT_REBIRTH,				///< ��Ȱ ����Ʈ

	XEFTEVT_EFFECT_SWORDTRAIL,			///< �˱�
	XEFTEVT_EFFECT_END_SWORDTRAIL,		///< �˱� ����
	XEFTEVT_EFFECT_STOP_SWORDTRAIL,		///< �˱� ���

	XEFTEVT_EFFECT_NPC_ICON,			///< NPC ������
	XEFTEVT_EFFECT_PLAYER_ICON,			///< Player ������

	XEFTEVT_EFFECT_LOOTABLE,			/// ������ �Ǹ� �����ش�.
	XEFTEVT_EFFECT_LOOTABLE_DEL,		/// ���� ����Ʈ ����

	// Buff
	XEFTEVT_EFFECT_BUFF,				///< ���� ����Ʈ
	XEFTEVT_EFFECT_BUFF_END,			///< ���� ����Ʈ ����
	XEFTEVT_EFFECT_BUFF_DIE,			///< �׾����� ���� ����Ʈ ���� 
	XEFTEVT_EFFECT_BUFF_CAPTION,		///< ���� Caption
	XEFTEVT_EFFECT_BUFF_RESET,
	XEFTEVT_EFFECT_BUFF_ENCHANT,		///< ��ȭ����

	// Talent
	XEFTEVT_EFFECT_TALENT,				///< �ŷ�Ʈ ����Ʈ
	XEFTEVT_EFFECT_TALENT_DEL,			///< ���� ����Ǿ� �ִ� �ŷ�Ʈ ����Ʈ ����
	XEFTEVT_EFFECT_TALENT_EFFECT_PRE_LOADER, ///< �ŷ�Ʈ ����Ʈ �̸� �ε��ϱ�
	XEFTEVT_EFFECT_TALENT_GUARD_DISABLE, ///< ���� �Ұ����� �ŷ�Ʈ ����Ʈ 

	// Damage
	XEFTEVT_EFFECT_SHOW_HIT_DAMAGE,		///< �ǰ� ����Ʈ
	XEFTEVT_EFFECT_SHOW_DAMAGE,			///< ������ Caption
	XEFTEVT_EFFECT_SHOW_PROJECTIL_DAMAGE,///< �߻�ü�� ������ ������ ����Ʈ

	// Caption
	XEFTEVT_EFFECT_CAPTION_HEAL,		///< �� Caption
	XEFTEVT_EFFECT_CAPTION_HP_REGEN,	///< Hp Regen Caption
	XEFTEVT_EFFECT_CAPTION_GUARDDEFENSE,///< ��� Caption
	XEFTEVT_EFFECT_CAPTION_DEBUG,		///< ����׿� Caption
	XEFTEVT_EFFECT_CAPTION_EXP,			///< ��ġ Caption
	XEFTEVT_EFFECT_CAPTION_EN,			///< ���ŷ� Caption

};

enum XEffectID
{
	XEFT_ID_NONE			= 0,
	XEFT_ID_HEAD			= 1,
	XEFT_ID_BUFF,
	XEFT_ID_NPCICON,
	XEFT_ID_TALENT,
	XEFT_ID_SWORDTRAIL,
	XEFT_ID_CAPTION
};

//////////////////////////////////////////////////////////////////////////
class XModuleEffect;

class XEffectEventData
{
public:
	float					m_fRemainTime;

	XEffectEventID			m_nEffectEventID;
	int						m_nParam1;

public:
	XEffectEventData()
	{
		m_fRemainTime			= 0.0f;

		m_nEffectEventID		= XEFTEVT_UNKNOWN;
		m_nParam1				= 0;
	}
	virtual ~XEffectEventData() {}
	bool					bSecondary;

	virtual bool			OnEffectEvent(XModuleEffect * pOwner);
};

class XBuffEffectEventData : public XEffectEventData
{
public:
	BUFF_TYPE				m_eType;
	BUFF_EFFECT_TYPE		m_eEffectType;
	
	bool					m_bRemainBuffGain;
	bool					m_bIsDeBuff;

	wstring					m_strBuffName;

	int						m_nPartsSlotType;
public:
	XBuffEffectEventData():m_bRemainBuffGain(false), m_eType(BUFT_NONE), m_eEffectType(BUFF_GAIN_EFFECT), m_bIsDeBuff(false), m_nPartsSlotType(-1) {}
	virtual ~XBuffEffectEventData() {}

	virtual bool			OnEffectEvent(XModuleEffect * pOwner);
};

class XTalentEffectEventData : public XEffectEventData
{
public:
	TALENT_EFFECT_TYPE		m_nType;

	XTalentInfo *			m_pTalentInfo;
	MUID					m_uidOwner;
	MUID					m_uidAttacker;

	vec3					m_vPos;
	MMatrix					m_matMeleeEffect;

	unsigned int			m_nDamageFlags;

	bool					m_bEffectStorage;

private:
	void SetEffectData(CREATE_TALENT_EFFECT_DATA& effectData)
	{
		effectData.pTalentInfo		= m_pTalentInfo;
		effectData.pOwner			= gg.omgr->Find(m_uidOwner);
		effectData.pAttacker		= gg.omgr->Find(m_uidAttacker);
		effectData.nType			= m_nType;
		effectData.effectPos		= m_vPos;
		effectData.nDamageFlags		= m_nDamageFlags;
		effectData.bEffectStorage	= m_bEffectStorage;
		effectData.matEffect		= m_matMeleeEffect;
	}

public:
	XTalentEffectEventData() : m_pTalentInfo(NULL), m_nType(CAST_EFFECT), m_uidOwner(MUID::ZERO), m_uidAttacker(MUID::ZERO), m_vPos(0, 0, 0), m_bEffectStorage(false), m_nDamageFlags(0)
	{
		m_matMeleeEffect.MakeIdentity();
	}

	virtual ~XTalentEffectEventData() {}

	void Set(XTalentInfo * inTalentInfo, TALENT_EFFECT_TYPE inType, XObject * inObj, vec3 inpos = vec3(0.0f, 0.0f, 0.0f), XObject * inOtherObj = NULL, bool bStorage = false, unsigned int nDamageFlags = 0, MMatrix matMelee = MMatrix::IDENTITY)
	{
		m_pTalentInfo				= inTalentInfo;
		m_nType						= inType;
		if(inObj)	m_uidOwner			= inObj->GetUID();
		if(inOtherObj)	m_uidAttacker	= inOtherObj->GetUID();
		m_vPos						= inpos;
		m_bEffectStorage			= bStorage;
		m_nDamageFlags				= nDamageFlags;

		m_matMeleeEffect			= matMelee;
	}

	virtual bool			OnEffectEvent(XModuleEffect * pOwner);
};

class XHitDamageEffectEventData : public XEffectEventData
{
public:
	_DamageInfo				m_Info;
	
	MUID					m_uidAttacker;
	MUID					m_uidTarget;

	vec3					m_vOffset;

	CSProjectileInfo*		m_pProjectileInfo;

	ProjectileColJudgmentType m_eColType;

public:
	XHitDamageEffectEventData():m_uidAttacker(MUID::ZERO), m_uidTarget(MUID::ZERO), m_pProjectileInfo(NULL), m_vOffset(0, 0, 0), m_eColType(PCJT_NONE) {}
	virtual ~XHitDamageEffectEventData() {}

	virtual bool			OnEffectEvent(XModuleEffect * pOwner);
};

class XCaptionEffectEventData : public XEffectEventData
{
public:
	GUARD_TYPE				m_eGuardType;
	
	wstring					m_strMessage;
	uint32					m_color;

	UIID					m_uiidUser;

public:
	XCaptionEffectEventData():m_eGuardType(GT_INVALID), m_uiidUser(UIID_INVALID) {}
	virtual ~XCaptionEffectEventData() {}

	virtual bool			OnEffectEvent(XModuleEffect * pOwner);
};

//////////////////////////////////////////////////////////////////////////
class XNPCInfo;
class XNonPlayer;
class XTalentHitParam;
struct _DamageInfo;
class XCaptionEffect;
struct HITEFFECT_RESULT;

//////////////////////////////////////////////////////////////////////////
/// ����Ʈ ��°��� 
class XModuleEffect : public XModule, public MMemPool<XModuleEffect>
{
	DECLARE_ID(XMID_EFFECT);

	friend XEffectEventData;
	friend XBuffEffectEventData;
	friend XTalentEffectEventData;
	friend XHitDamageEffectEventData;
	friend XCaptionEffectEventData;

private:
	//--------------------------------------------------------------------------
	// ����Ʈ �޼��� ���� ����
	class XEffectEvent
	{
	private:
		vector<XEffectEventData *>	m_vecEffectEventList;
		vector<XEffectEventData *>	m_vecSendEffectEventList;
		XModuleEffect *				m_pOwner;

	public:
		XEffectEvent(XModuleEffect * pOwner);
		~XEffectEvent();

		void						OnEvent(XEffectEventData* pEventData);
		void						update(float fDelta);
		void						sendUpdate(float fDelta);
		void						push_back(XEffectEventData* pEvent);

		int							GetEffectEventIDCount(XEffectEventID nEventID);
	};

	XEffectEvent *			m_pEffectEvent;

	MUID					m_uidPlayerIcon;

protected:
	XHeadEffect *			m_pHeadEffect;
	XBuffEffect *			m_pBuffEffect;
	XNPCIConEffect *		m_pNPCIConEffect;
	XTalentEffect *			m_pTalentEffect;

	XSwordTrail	*			m_pSwordTrail;
	XCaptionEffect*			m_pCaptionEffect;

protected:
	virtual void			OnUpdate(float fDelta);
	virtual XEventResult	OnEvent(XEvent& msg);
	virtual void			OnSubscribeEvent();

	wstring					GetDamageSoundName(_DamageInfo * pInfo);

	void					OnLoadingComplete();

	void					ChangeSwordTrailAnimation();
	void					ChangeSwordTrailMotion();

	//----------------------------------------------------------------------------------------------
	// ����Ʈ ó��
	void					CreateRebirthEffect();
	void					CreateCutSceneEffect();
	void					DestroyPlayerIcon();

	void					ShowDamageCaptionEffect(_DamageInfo * pInfo, vec3 vDamagePos);
	void					ShowBuffCaptionEffect( const wchar_t* szName, bool bIsDebuff =false);
	void					ShowExpCaptionEffect(int nExp);
	void					ShowDebugCaptionEffect( const wchar_t* szMessage, uint32 nColor);

	void					CreateLootableEffect();
	void					DeleteLootableEffect();

	void					PlayDamageEffect(_DamageInfo * pInfo);
	
	// �ǰ� Effect
	void					ShowHitAndDamage(_DamageInfo * pInfo, XObject * pAttacker, const vec3& vOffset =vec3::ZERO);
	void					ShowEmptyProjrctileEffect(XObject* pTarget, XObject* pAttacker, CSProjectileInfo* pProjectileInfo, ProjectileColJudgmentType colType);
	void					ShowPartsHit(_DamageInfo* pInfo, vec3 vHitPos, HITEFFECT_RESULT& resultHitEffect);

	// �� Effect
	void					ShowHeal(int nHeal, UIID nUserUIID = UIID_INVALID);
	void					ShowHPRegen( int nHeal );

	// ���ŷ� Effect
	void					ShowEn(int nEn);

	// ��� Effect
	void					ShowGuardDefense(GUARD_TYPE eGuardType, int nDamage);
	//----------------------------------------------------------------------------------------------

public:
	XModuleEffect(XObject* pOwner=NULL);
	virtual ~XModuleEffect(void);

	void					SetRegisterUseEffectID(int nUseEffectID);


	//----------------------------------------------------------------------------------------------
	// ����Ʈ �̺�Ʈ
	void					OnEffectEvent(XEffectEventID nEffectEventID, XEffectEventData* pEffectData = NULL);
	void					OnEffectEvent(XEffectEventID nEffectEventID, int nParam1);

	//----------------------------------------------------------------------------------------------
	// Getter
	vec3					GetDamageCaptionEffectPos(vec3 vHitEffectPos);
	MMatrix					GetMeleeHitDirection(XTalentInfo* pTalentInfo);

	void					SetLootableEffectByDie();
	void					SetLootableEffectByIcon();

	void					DestroyNPCIconEffect();
	void					ReSetNPCIConEffect();

	void					ShowDamageInfo(_DamageInfo * pInfo, const MUID& uidTarget);

};

#endif // #ifndef _XMODULE_EFFECT_H