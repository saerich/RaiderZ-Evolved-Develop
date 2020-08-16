#pragma once

#include "CSCommonLib.h"
#include "CSModifier.h"
#include "CSTalentInfo.h"
#include "MTypes.h"
#include "MXml.h"
#include <string>
#include "MTstring.h"
#include "CSPalette.h"

using namespace::std;

enum BUFF_TYPE
{
	BUFT_NONE=0,
	BUFT_BUFF,
	BUFT_DEBUFF,
	BUFT_AURA,
	BUFT_AURA_EFFECT_BUFF,
	BUFT_AURA_EFFECT_DEBUFF,
	BUFT_ENCHANT,
	BUFT_QUEST,
	BUFT_MAX,
};

static const TCHAR* BUFF_TYPE_STR[] = 
{
	_T("NONE"),
	_T("BUFF"),
	_T("DEBUFF"),
	_T("AURA"),
	_T("AURA_EFFECT_BUFF"),
	_T("AURA_EFFECT_DEBUFF"),
	_T("ENCHANT"),
	_T("QUEST"),
};

enum BUFF_PASSIVE_EXTRA_ATTRIB_
{
	BUFPEA_NONE = 0,
	BUFPEA_DISABLE_TALENTS,
	BUFPEA_DISABLE_TALENT_MELEE,
	BUFPEA_DISABLE_TALENT_RANGE,
	BUFPEA_DISABLE_TALENT_MAGIC,
	BUFPEA_INVINCIBILITY,
	BUFPEA_ROOT,
	BUFPEA_SLEEP,
	BUFPEA_INVISIBILITYTONPC,
	BUFPEA_INVISIBILITY,
	BUFPEA_ABSOLUTE_GUARD,
	BUFPEA_DISABLE_QUESTPVP_DROP,
	BUFPEA_RIDE,
	BUFPEA_MAX
};

static const TCHAR* BUFF_PASSIVE_EXTRA_ATTRIB_STR[] = 
{
	_T("NONE"),
	_T("DISABLE_TALENTS"),
	_T("DISABLE_TALENT_MELEE"),
	_T("DISABLE_TALENT_RANGE"),
	_T("DISABLE_TALENT_MAGIC"),
	_T("INVINCIBILITY"),
	_T("ROOT"),
	_T("SLEEP"),
	_T("INVISIBILITYTONPC"),
	_T("INVISIBILITY"),
	_T("ABSOLUTE_GUARD"),
	_T("DISABLE_QUESTPVP_DROP"),
	_T("RIDE")
};

enum BUFF_ACTIVE_EXTRA_ATTRIB_
{
	BUFAEA_NONE = 0,
	BUFAEA_DIE,
	BUFAEA_DESPAWN_ONCE,
	BUFAEA_DESPAWN_ETERNAL,
	BUFAEA_DISMOUNT,
	BUFAEA_MAX
};

static const TCHAR* BUFF_ACTIVE_EXTRA_ATTRIB_STR[] = 
{
	_T("NONE"),
	_T("DIE"),
	_T("DESPAWN_ONCE"),
	_T("DESPAWN_ETERNAL"),
	_T("DISMOUNT")
};

enum BUFF_STACK_TYPE
{
	BUFFSTACK_NONE,	// ��ø���� ����
	BUFFSTACK_STACK, // ��ø�ǰ� �� �������� �ٸ� �����ֱ⸦ ����
	BUFFSTACK_STACK_UPDATE, // ��ø�ǰ� ���� ������ duration/period �ð� �ʱ�ȭ
};

static const TCHAR* BUFF_STACK_TYPE_STR[] = 
{
	_T("none"),
	_T("stack"),
	_T("stack_n_update"),
};




class CSCOMMON_API CSBuffInfo
{
public:
	int						m_nID;						// id (���)
	BUFF_TYPE				m_nType;
	BUFF_STACK_TYPE			m_nStackType;				// ����Ÿ��
	tstring					m_strName;					// ���� �̸�(���)
	int						m_nStackMaxCount;			// ���� �ִ� ����

	bool					m_bUseChangeEquipmentSlot;
	int						m_nEquipItemIDs[ITEMSLOT_MAX];				// ����� ��� ������

	bool					m_bUseChangePalette;		// �ȷ�Ʈ �������� ����
	int						m_arrPalettes[PALETTESLOT_MAX]; // ����� �ȷ�Ʈ
	
	int						m_nStackSlot;				// (���)
	int						m_nStackPower;				// (���)

	float					m_fWeaponApplyRate;			// ���� ����� ���� (1 = 100%)
	DAMAGE_ATTRIB			m_nDamageAttrib;			// ������ ���� (����)
	int						m_nMinDamage;				// �ּ� ������
	int						m_nMaxDamage;				// �ִ� ������
	int						m_nMinHeal;					// �ּ� ȸ����
	int						m_nMaxHeal;					// �ִ� ȸ����
	float					m_fPercentageHeal;			// ������ ȸ��
	BUFF_PASSIVE_EXTRA_ATTRIB_		m_nPassiveExtraAttrib;
	BUFF_PASSIVE_EXTRA_ATTRIB_		m_nPassiveExtraAttrib2;
	float							m_fPassiveExtraAttribValue;
	float							m_fPassiveExtraAttribValue2;
	BUFF_ACTIVE_EXTRA_ATTRIB_		m_nActiveExtraAttrib;

	CSInstantModifier		m_InstantModifier;
	CSActorModifier			m_ActorModifier;
	CSPlayerModifier		m_PlayerModifier;
	
	tstring					m_strEffectName;			// ����Ʈ �̸�
	tstring					m_strBoneName;				// �� �̸�

	tstring					m_strUseAniName;			// �ִϸ��̼� �̸�
	tstring					m_strAniPostfix;			// �ִϸ��̼� ���̻�

	CSRiposte				m_Riposte;		// Riposte ���� ����
	GTalentResist			m_Resist;

	bool					m_bCancelable;			// ����� ���� ������ ����� ȿ���� ������� ����	

	RequireBuff				m_RequireBuff;
	
	struct TriggeredBuff
	{
		// �� ȿ���� ����� ������ UID
		int				 nBuffID;	
		// �ߵ� ����
		TALENT_CONDITION nTiming;	
		// ȿ�� ����
		CSEffectInfo	 infoEffect;
		// �ߵ� Ȯ��
		float	    	 fRate;

		TriggeredBuff();
	} m_TriggeredBuff;

	virtual void CopyAll(CSBuffInfo* pTarget) = 0;
protected:
	bool			IsMatchPassiveExtraAttrib(BUFF_PASSIVE_EXTRA_ATTRIB_ nAttrib) const;
	bool			IsMatchActiveExtraAttrib(BUFF_ACTIVE_EXTRA_ATTRIB_ nAttrib) const;
public:
	CSBuffInfo();

	bool HasDamage();
	bool HasHealEffect();
	bool IsAuraEffectBuff();
	bool IsBuff();
	bool IsDebuff();
	bool IsQuestBuff();
	bool IsMesmerize();
	bool IsEnchant() const;
	bool IsRideBuff() const;
	bool HasProcEffect() const;
};


class CSCOMMON_API CSBuffInfoMgr : public map<int , CSBuffInfo* > 
{
private:
	CSBuffInfoMgr(CSBuffInfoMgr& other);
protected:
	void ParseBuffCommon(CSBuffInfo* pBuffInfo, MXmlElement* pElement, MXml* pXml);
public:
	CSBuffInfoMgr() {}
	virtual ~CSBuffInfoMgr() {}
};
