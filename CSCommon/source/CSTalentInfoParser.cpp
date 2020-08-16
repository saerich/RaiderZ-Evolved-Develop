#include "stdafx.h"
#include "CSTalentInfoParser.h"
#include "CSTalentInfoDef.h"
#include "MCsvParser.h"
#include "MUtil.h"
#include "CSStrings.h"
#include "CSCommonParser.h"
#include "CSEffectInfo.h"
#include "AString.h"
#include "CSTalentInfoEnum.h"
#include "MLocale.h"
#include "CSFormatString.h"
#include "CSHelper.h"


#define COMPOSITE_ATTR(PREFIX,TXT,SUFFIX)		(string(PREFIX)+(TXT)+(SUFFIX)).c_str()

void CSTalentInfoParser::ParseEffectTiming(const string& strValue, TALENT_CONDITION& nValue)
{
	if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_NONE)						nValue = TC_NONE;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_USE_THIS_TALENT)		nValue = TC_USE_TALENT;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_FINISH_THIS_TALENT)	nValue = TC_FINISH_TALENT;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_GAIN)					nValue = TC_BUFF_GAIN;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_GONE)					nValue = TC_BUFF_GONE;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_EXPIRED)				nValue = TC_BUFF_EXPIRED;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_PERIOD)				nValue = TC_BUFF_PERIOD;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_GAIN_N_PERIOD)			nValue = TC_BUFF_GAIN_N_PERIOD;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_HIT_THIS_TALENT)		nValue = TC_HIT_TALENT;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_HIT_SUCCUESS)			nValue = TC_HIT_SUCCESS;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_HIT_ALL_THIS_TALENT)	nValue = TC_HIT_ALL_TALENT;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_HIT_ME)				nValue = TC_HIT_ME;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_MISS_ME)				nValue = TC_MISS_ME;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_GUARD_ME)				nValue = TC_GUARD_ME;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_PERFECT_GUARD_ME)		nValue = TC_PERFECT_GUARD_ME;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_CRITICAL_HIT_ME)		nValue = TC_CRITICAL_HIT_ME;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_HIT_ENEMY)				nValue = TC_HIT_ENEMY;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_MISS_ENEMY)			nValue = TC_MISS_ENEMY;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_GUARD_ENEMY)			nValue = TC_GUARD_ENEMY;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_CRITICAL_HIT_ENEMY)	nValue = TC_CRITICAL_HIT_ENEMY;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_COUNTER)				nValue = TC_COUNTER;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_RESIST)				nValue = TC_RESIST;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_BUFF_HIT)				nValue = TC_BUFF_HIT;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_BUFF_MAX_STACKED)			nValue = TC_BUFF_MAX_STACKED;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_BUFF_STACKED)			nValue = TC_BUFF_STACKED;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_HIT_ME_N)				nValue = TC_HIT_ME_N;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_CRIHIT_ME_N)			nValue = TC_CRIHIT_ME_N;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_DAMAGE)				nValue = TC_DAMAGE;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_AFFECTED)				nValue = TC_AFFECTED;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_MAGIC_ACT)				nValue = TC_MAGIC_ACT;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_MAGIC_HIT_ME)			nValue = TC_MAGIC_HIT_ME;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_MAGIC_HIT_ENEMY)		nValue = TC_MAGIC_HIT_ENEMY;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_MELEE_HIT_ME)			nValue = TC_MELEE_HIT_ME;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_MELEE_HIT_ENEMY)		nValue = TC_MELEE_HIT_ENEMY;
	else if (strValue == TALENT_XML_VALUE_BUFF_CONDITION_DO_SOMETHING)			nValue = TC_DO_SOMETHING;
}

void CSTalentInfoParser::ParseTalentEffect(MXmlElement* pElement, CSEffectInfo& outEffect, const char* pszPrefix, const char* pszSuffix)
{
	tstring strValue;

	if (_Attribute(strValue,	pElement, COMPOSITE_ATTR(pszPrefix,TALENT_XML_ATTR_EFFECT_POINT,pszSuffix)))
	{
		outEffect.m_nPoint = static_cast<CSEffectInfo::POINT>(CSStrings::StringToEffectPoint(strValue.c_str()));
	}
	if (_Attribute(strValue,	pElement, COMPOSITE_ATTR(pszPrefix,BUFF_XML_ATTR_EFFECT_TARGET,pszSuffix)))
	{
		outEffect.m_nRelation = static_cast<CSEffectInfo::RELATION>(CSStrings::StringToEffectRelation(strValue.c_str()));
	}

	if (_Attribute(&outEffect.m_fRadius,		pElement, COMPOSITE_ATTR(pszPrefix,BUFF_XML_ATTR_EFFECT_RANGE,pszSuffix)))
	{
		if (0.0f < outEffect.m_fRadius)
		{
			// ���ʹ����� ��ȯ
			outEffect.m_fRadius *= 100;
		}
	}

	_Attribute(&outEffect.m_nLimit,	pElement, COMPOSITE_ATTR(pszPrefix,BUFF_XML_ATTR_EFFECT_LIMIT,pszSuffix));	
}

void CSTalentInfoParser::ParseTalentCommon(CSTalentInfo* pTalentInfo, MXmlElement* pElement, MXml* pXml)
{
	string strValue;

	// �ŷ�Ʈ �̸��� ���ڿ����̺� Ű��
	tstring preFix;
	if (0 < pTalentInfo->m_nMode)
	{
		preFix = _T("_");
		preFix = preFix + CSStrings::WeaponType(pTalentInfo->m_nMode);
	}
	
	pTalentInfo->m_szName = CSFormatString::Format(_T("TALENT_NAME_{0}{1}"), FSParam(pTalentInfo->m_nID, preFix));
	pTalentInfo->m_szDesc = CSFormatString::Format(_T("TALENT_DESC_{0}{1}"), FSParam(pTalentInfo->m_nID, preFix));

	_ParseNPC(pElement, pTalentInfo);
	
	_Attribute(&pTalentInfo->m_bPerfectGuardOnly,		pElement, TALENT_XML_ATTR_PERFECTGUARDONLY);
	_Attribute(&pTalentInfo->m_bPartialGuardHit,		pElement, TALENT_XML_ATTR_PARTIALGUARDHIT);
	_Attribute(&pTalentInfo->m_bCancelable,				pElement, TALENT_XML_ATTR_CANCELABLE);
	_Attribute(&pTalentInfo->m_fCoolTime,				pElement, TALENT_XML_ATTR_COOLTIME);
	_Attribute(&pTalentInfo->m_bUseAiming,				pElement, TALENT_XML_ATTR_IS_AIMING);
	_Attribute(&pTalentInfo->m_nHitCapsuleGroupIndex,	pElement, TALENT_XML_ATTR_HITCAPSULEGROUPINDEX);
	_Attribute(&pTalentInfo->m_nExtraHitCapsuleGroup,	pElement, TALENT_XML_ATTR_EXTRA_HITCAPSULEGROUP);
	_Attribute(&pTalentInfo->m_nExtraHitCapsuleGroup2,	pElement, TALENT_XML_ATTR_EXTRA_HITCAPSULEGROUP2);
	_Attribute(&pTalentInfo->m_nExtraHitCapsuleGroup3,	pElement, TALENT_XML_ATTR_EXTRA_HITCAPSULEGROUP3);
	_Attribute(&pTalentInfo->m_bIgnoreMesmerize,			pElement, TALENT_XML_ATTR_IGNORE_MESMERIZE, false);
	_Attribute(&pTalentInfo->m_RequireBuff.bKeepIncludeBuff,			pElement, TALENT_XML_ATTR_KEEP_INCLUDEBUFF);
	_Attribute(&pTalentInfo->m_RequireBuff.bKeepExcludeBuff,			pElement, TALENT_XML_ATTR_KEEP_EXCLUDEBUFF);

	if (_Attribute(strValue,							pElement, TALENT_XML_ATTR_GUARD_CRASH_LEVEL))
	{
		STRING_MATCH(GUARD_CRASH_LEVEL, pTalentInfo->m_nGuardCrashLevel, strValue.c_str());
	}

	CSCSVParser csv_parser;
	csv_parser.Parse_CSV(pElement, TALENT_XML_ATTR_NPC_INCLUDE, pTalentInfo->m_RequireBuff.vecIncludeBuffID);
	csv_parser.Parse_CSV(pElement, TALENT_XML_ATTR_NPC_EXCLUDE, pTalentInfo->m_RequireBuff.vecExcludeBuffID);
	
	int nDodgeDifficulty = 230;
	if (_Attribute(&nDodgeDifficulty,				pElement, TALENT_XML_ATTR_DODGE_DIFFICULTY))
	{
		pTalentInfo->m_nDodgeDifficulty = (uint8)nDodgeDifficulty;
	}

	if (_Attribute(strValue,							pElement, TALENT_XML_ATTR_STANCE))
	{
		if (strValue == TALENT_XML_VALUE_STANCE_ALL) pTalentInfo->m_nEnabledStanceFlag = 0xFFFFFFFF;
		else if (strValue == TALENT_XML_VALUE_STANCE_NORMAL) SetBitSet(pTalentInfo->m_nEnabledStanceFlag, CS_NORMAL);
		else if (strValue == TALENT_XML_VALUE_STANCE_COMBAT) SetBitSet(pTalentInfo->m_nEnabledStanceFlag, CS_BATTLE);
	}

	if (_Attribute(strValue,							pElement, TALENT_XML_ATTR_CATEGORY))
	{
		if (strValue == TALENT_XML_VALUE_CATEGORY_COMMON) pTalentInfo->m_nCategory = TC_COMMON;
		else if (strValue == TALENT_XML_VALUE_CATEGORY_MELEE) pTalentInfo->m_nCategory = TC_MELEE;
		else if (strValue == TALENT_XML_VALUE_CATEGORY_RANGE) pTalentInfo->m_nCategory = TC_RANGE;
		else if (strValue == TALENT_XML_VALUE_CATEGORY_MAGIC) pTalentInfo->m_nCategory = TC_MAGIC;
	}

	if (_Attribute(strValue,							pElement, TALENT_XML_ATTR_STYLE))
	{
		pTalentInfo->m_nStyle = (TALENT_STYLE) CSStrings::StringToTalentStyle(MLocale::ConvAnsiToTCHAR(strValue.c_str()).c_str());
	}

	if (_Attribute(strValue,							pElement, TALENT_XML_TAG_NEED_TRAINING))
	{
		pTalentInfo->m_bNeedTraining = CSStrings::StringToBool(MLocale::ConvAnsiToTCHAR(strValue.c_str()).c_str());
	}

	// �н� �ʿ� ���� 
	_Attribute(&pTalentInfo->m_nTNeedLevel,			pElement, TALENT_XML_TAG_TNEED_LEVEL);
	_Attribute(&pTalentInfo->m_nTNeedTalent,			pElement, TALENT_XML_TAG_TNEED_TALENT);
	_Attribute(&pTalentInfo->m_nTNeedStyleTP,		pElement, TALENT_XML_TAG_TNEED_STYLETP);
	_Attribute(&pTalentInfo->m_bByLicense,			pElement, TALENT_XML_TAG_BYLICENSE);
	


	if (_Attribute(strValue,							pElement, TALENT_XML_ATTR_TALENT_TYPE))
	{
		if (strValue == TALENT_XML_VALUE_TALENTTYPE_SKILL) pTalentInfo->m_nTalentType = TT_SKILL;
		else if (strValue == TALENT_XML_VALUE_TALENTTYPE_WEAPON) pTalentInfo->m_nTalentType = TT_WEAPON;
		else if (strValue == TALENT_XML_VALUE_TALENTTYPE_ENCHANT) pTalentInfo->m_nTalentType = TT_ENCHANT;
		else if (strValue == TALENT_XML_VALUE_TALENTTYPE_ITEM) pTalentInfo->m_nTalentType = TT_ITEM;
		else if (strValue == TALENT_XML_VALUE_TALENTTYPE_GATHER) pTalentInfo->m_nTalentType = TT_GATHER;
	}

	_ParseExtraPassiveType(pElement, pTalentInfo);

	_ParseExtraActiveType(pElement, pTalentInfo);

	_ParseSkillType(pElement, pTalentInfo);

	// NPC ��� ����
	_ParseNPCChangeMode(pElement, pTalentInfo);

	// ��Ŀ��
	_ParseFocus(pElement, pTalentInfo);


	_Attribute(&pTalentInfo->m_nTalentLine,			pElement, TALENT_XML_ATTR_TALENTLINE);
	_Attribute(&pTalentInfo->m_nCooltimeLine,		pElement, TALENT_XML_ATTR_COOLTIMELINE);
	int nRank = 0;	
	_Attribute(&nRank,								pElement, TALENT_XML_ATTR_RANK);
	pTalentInfo->m_nRank = static_cast<char>(nRank);

	_Attribute(&pTalentInfo->m_fRequireMoveSpeed,	pElement, TALENT_XML_ATTR_REQUIRE_MOVESPEED);
	_Attribute(&pTalentInfo->m_bAvailableOnGuard,	pElement, TALENT_XML_ATTR_AVAILABLE_ON_GUARD);


	ParseInvokeBuff(pTalentInfo->m_Buff1, "1", pElement);
	ParseInvokeBuff(pTalentInfo->m_Buff2, "2", pElement);

	CSModifierParser::Parse_InstantModifier(pElement, pTalentInfo->m_InstantModifier);
	CSModifierParser::Parse_ActorModifier(pElement, pTalentInfo->m_ActorModifier);
	CSModifierParser::Parse_PlayerModifier(pElement, pTalentInfo->m_PlayerModifier);

	_ParseWeaponAllowed(pElement, pTalentInfo);

	_Attribute(pTalentInfo->m_strSwordTrailEffect,	pElement, TALENT_XML_TAG_SWORD_TRAIL_EFFECT);

	ParseTalentEffect(pElement, pTalentInfo->m_EffectInfo, "", "");

	_Attribute(&pTalentInfo->m_fRange,			pElement, TALENT_XML_ATTR_RANGE);
	pTalentInfo->m_fRange *= 100.0f;				// �Ÿ��� ���� �����̴�.

	if (_Attribute(strValue,				pElement, TALENT_XML_ATTR_WEAPON_REFRENCE)) 
	{
		pTalentInfo->m_WeaponReference = (WEAPON_REFRENCE)CSStrings::StringToWeapnReference(MLocale::ConvAnsiToTCHAR(strValue.c_str()).c_str());
	}
	
	// ��Ŀ��
	_ParseFinishEffect(pElement, pTalentInfo);


	_Attribute(&pTalentInfo->m_bValidation,		pElement, TALENT_XML_ATTR_VALIDATION);

	if(_Attribute(strValue,						pElement, TALENT_XML_TAG_MOVING_USE_TYPE))
	{
		if(strValue == TALENT_XML_VALUE_MOVING_USE_TYPE_NONE) pTalentInfo->m_nMovableType = MUT_NONE;
		else if (strValue == TALENT_XML_VALUE_MOVING_USE_TYPE_CASTING) pTalentInfo->m_nMovableType = MUT_CASTING;
		else if (strValue == TALENT_XML_VALUE_MOVING_USE_TYPE_USE) pTalentInfo->m_nMovableType = MUT_USE;
		else if (strValue == TALENT_XML_VALUE_MOVING_USE_TYPE_ALL) pTalentInfo->m_nMovableType = MUT_ALL;
	}

	if (_Attribute(strValue,						pElement, TALENT_XML_ATTR_TARGETING_TYPE))
	{
		if (strValue == TALENT_XML_VALUE_TARGETING_TYPE_STICKY) pTalentInfo->m_eTargetingType = TTT_STICKY;
		else if (strValue == TALENT_XML_VALUE_TARGETING_TYPE_LINE) pTalentInfo->m_eTargetingType = TTT_LINE;
		else if (strValue == TALENT_XML_VALUE_TARGETING_TYPE_GROUND) pTalentInfo->m_eTargetingType = TTT_GROUND;
	}


	// �ʱ�ȭ ��뿩��
	_Attribute(pTalentInfo->m_bUntrainable, pElement, TALENT_XML_ATTR_UNTRAINABLE);

	// ������ min, max
	_Attribute(&pTalentInfo->m_nMinDamage,		pElement, TALENT_XML_ATTR_MIN_DAMAGE);
	_Attribute(&pTalentInfo->m_nMaxDamage,		pElement, TALENT_XML_ATTR_MAX_DAMAGE);

	// �׾����� ��밡���� �ŷ�Ʈ
	_ParseUsableAtDead(pElement, pTalentInfo);	

	ParseResist(pTalentInfo->m_Resist, pElement, TALENT_XML_ATTR_RESIST_METHOD, TALENT_XML_ATTR_RESIST_TYPE, TALENT_XML_ATTR_RESIST_DIFFICULTY);
}

void CSTalentInfoParser::ParseInvokeBuff( CSBuffEnchantInfo& outBuffInvokeInfo, const char* pszSuffix, MXmlElement* pElement )
{
	string strValue;

	_Attribute(&outBuffInvokeInfo.nID,				pElement, COMPOSITE_ATTR(,TALENT_XML_ATTR_BUFF,pszSuffix));

	CSTalentInfoParser talentInfoParser;
	if (_Attribute(strValue, pElement, COMPOSITE_ATTR(,TALENT_XML_ATTR_BUFF_CONDITION,pszSuffix)))
	{
		talentInfoParser.ParseEffectTiming(strValue, outBuffInvokeInfo.nEnchantEvent);
	}

	if(_Attribute(&outBuffInvokeInfo.nRange,			pElement, COMPOSITE_ATTR(,TALENT_XML_ATTR_BUFF_EFFECT_RANGE,pszSuffix)))
	{
		outBuffInvokeInfo.nRange *= 100;
	}

	talentInfoParser.ParseTalentEffect(pElement, outBuffInvokeInfo.EffectInfo, "Buff", pszSuffix);	

	ParseResist(outBuffInvokeInfo.Resist, pElement, 
		COMPOSITE_ATTR(,TALENT_XML_ATTR_BUFF_RESIST_METHOD,pszSuffix), 
		COMPOSITE_ATTR(,TALENT_XML_ATTR_BUFF_RESIST_TYPE,pszSuffix), 
		COMPOSITE_ATTR(,TALENT_XML_ATTR_BUFF_RESIST_DIFFICULTY,pszSuffix));

	// ������� ���� ������ ������������ ����
	outBuffInvokeInfo.Resist.m_nMethod = TRM_BINARY;	
}

void CSTalentInfoParser::ParseWeaponAllowed( CSTalentInfo* pTalentInfo, const TCHAR* szValue )
{
	CSTalentInfoWeaponAllowedParser parser;
	parser.Parse(szValue, pTalentInfo->m_WeaponAllowedInfo);
}

void CSTalentInfoParser::ParseTalentServer( CSTalentInfo* pTalentInfo, MXml* pXml, MXmlElement* pElement )
{
	string strName;
	string strValue;

	bool bLoop=false;
	bool bCancelEnabled = false;
	bool bWeaponAni = false;

	_Attribute(&pTalentInfo->m_nHPCost,				pElement, TALENT_XML_ATTR_HP_COST);
	_Attribute(&pTalentInfo->m_nENCost,				pElement, TALENT_XML_ATTR_EN_COST);
	_Attribute(&pTalentInfo->m_nSTACost,				pElement, TALENT_XML_ATTR_STA_COST);
	_Attribute(pTalentInfo->m_szCastingAnimation,		pElement, TALENT_XML_ATTR_CASTING_ANI);
	_Attribute(&pTalentInfo->m_fCastingTime,			pElement, TALENT_XML_ATTR_CASTING_TIME);
	_Attribute(pTalentInfo->m_szUseAnimation,			pElement, TALENT_XML_ATTR_USE_ANI);

	_Attribute(&pTalentInfo->m_bCriticalEnable,		pElement, TALENT_XML_ATTR_CRITICAL_ENABLE);

	_Attribute(&pTalentInfo->m_Drain.m_fRate,			pElement, TALENT_XML_ATTR_DRAIN_RATE);
	if (_Attribute(strValue,			pElement, TALENT_XML_ATTR_DRAIN_TYPE))
	{
		if (!_stricmp(strValue.c_str(), "hp")) pTalentInfo->m_Drain.m_nType = TALENT_DRAIN_HP;
		else if (!_stricmp(strValue.c_str(), "en")) pTalentInfo->m_Drain.m_nType = TALENT_DRAIN_EN;
		else if (!_stricmp(strValue.c_str(), "sp")) pTalentInfo->m_Drain.m_nType = TALENT_DRAIN_SP;
	}

	if (_Attribute(strValue,			pElement, TALENT_XML_ATTR_EFFECT_TIMING))
	{
		ParseEffectTiming(strValue, pTalentInfo->m_nTiming);
	}	

	int nDamageType=0;
	_Attribute(&pTalentInfo->m_nMinHeal,		pElement, TALENT_XML_ATTR_MIN_HEAL);
	_Attribute(&pTalentInfo->m_nMaxHeal,		pElement, TALENT_XML_ATTR_MAX_HEAL);
	
	_Attribute(&pTalentInfo->m_fWeaponApplyRate,	pElement, TALENT_XML_ATTR_WEAPON_APPLY_RATE);

	_Attribute(&pTalentInfo->m_fCriticalApplyRate,	pElement, TALENT_XML_ATTR_CRITICAL_APPLY_RATE);

	if (_Attribute(strValue,				pElement, TALENT_XML_ATTR_DAMAGE_ATTRIB)) 
	{
		pTalentInfo->m_nDamageAttrib = (DAMAGE_ATTRIB)CSStrings::StringToDamageAttrib(MLocale::ConvAnsiToTCHAR(strValue.c_str()).c_str());
	}
	_Attribute(&pTalentInfo->m_nHateAdj,			pElement, TALENT_XML_ATTR_HATE_ADJ);
	_Attribute(&pTalentInfo->m_fHateFactor,		pElement, TALENT_XML_ATTR_HATE_FACTOR);

	if (_Attribute(&pTalentInfo->m_fInvincibleTimeStart,		pElement, TALENT_XML_ATTR_INVINCIBLE_TIME_START))
	{
		pTalentInfo->m_bUseInvincibleTime = true;
		_Attribute(&pTalentInfo->m_fInvincibleTimeEnd,			pElement, TALENT_XML_ATTR_INVINCIBLE_TIME_END);
	}

	if (_Attribute(&pTalentInfo->m_fSuperarmorTimeStart,		pElement, TALENT_XML_ATTR_SUPERARMOR_TIME_START))
	{
		pTalentInfo->m_bUseSuperarmorTime = true;
		_Attribute(&pTalentInfo->m_fSuperarmorTimeEnd,			pElement, TALENT_XML_ATTR_SUPERARMOR_TIME_END);
	}

	_Attribute(&pTalentInfo->m_bAvoidable,		pElement, TALENT_XML_ATTR_AVOIDABLE);
	if (_Attribute(&pTalentInfo->m_fAvoidTimeStart,		pElement, TALENT_XML_ATTR_AVOID_TIME_START))
	{
		pTalentInfo->m_bUseAvoidTime = true;
		_Attribute(&pTalentInfo->m_fAvoidTimeEnd,			pElement, TALENT_XML_ATTR_AVOID_TIME_END);
	}
	_Attribute(&pTalentInfo->m_bAvoidable,		pElement, TALENT_XML_ATTR_AVOIDABLE);

	_Attribute(pTalentInfo->m_strLuaOnStart,			pElement, TALENT_XML_ATTR_LUA_START);
	_Attribute(pTalentInfo->m_strLuaOnAct,				pElement, TALENT_XML_ATTR_LUA_ACT);
	_Attribute(pTalentInfo->m_strLuaOnDelayedAct,		pElement, TALENT_XML_ATTR_LUA_DELAYED_ACT);
	_Attribute(pTalentInfo->m_strLuaOnFinish,			pElement, TALENT_XML_ATTR_LUA_FINISH);
	_Attribute(pTalentInfo->m_strLuaOnCancel,			pElement, TALENT_XML_ATTR_LUA_CANCEL);

	// NPC��
	_Attribute(&pTalentInfo->m_bSetDirection,		pElement, TALENT_XML_ATTR_IS_SET_DIRECTION);

	strValue = "";
	_Attribute(strValue,			pElement, TALENT_XML_ATTR_FORCE_MF);
	if (strValue != "")
	{
		for (int i = 0; i < MF_SIZE; i++)
		{
			if (CSStrings::StringToMotionfactorType(MLocale::ConvAnsiToTCHAR(strValue.c_str()).c_str()) == i)
			{
				pTalentInfo->m_nForceMF = MF_STATE(i);
				break;
			}
		}
	}
	_Attribute(&pTalentInfo->m_nForceMFWeight,		pElement, TALENT_XML_ATTR_FORCE_MF_WEIGHT);


	CSMotionFactorParser::Parse(pElement, pTalentInfo->m_MotionFactorGroup);


	_Attribute(&pTalentInfo->m_nAICategories,				pElement, TALENT_XML_ATTR_CATEGORY);

	_ParseHit(pTalentInfo, pElement);


}

void CSTalentInfoParser::_ParseNPC( MXmlElement* pElement, CSTalentInfo* pTalentInfo )
{
	string strValue;
	if (_Attribute(strValue,				pElement, TALENT_XML_ATTR_NPC)) 
	{
		MCsvParser csv_parser;
		csv_parser.LoadFromStream(strValue.c_str());
		int nColCount = csv_parser.GetColCount(0);

		char szNPCID[1024];
		for (int nCol = 0; nCol < nColCount; nCol++)
		{
			if (csv_parser.GetData(nCol, 0, szNPCID, 1024))
			{
				int nNPCID = atoi(szNPCID);
				if (nNPCID > 0)
				{
					pTalentInfo->m_setNPCID.insert(nNPCID);
				}
			}
		}
	}
}

void ParseExtraPassiveType(string strValue, TALENT_EXTRA_PASSIVE_TYPE& outnValue)
{
	if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_BASE_WEAPON_ONE_HANDED) outnValue = TEPT_BASE_WEAPON_ONE_HANDED;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_BASE_WEAPON_1H_SLASH) outnValue = TEPT_BASE_WEAPON_1H_SLASH;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_BASE_WEAPON_1H_BLUNT) outnValue = TEPT_BASE_WEAPON_1H_BLUNT;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_BASE_WEAPON_1H_PIERCE) outnValue = TEPT_BASE_WEAPON_1H_PIERCE;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_BASE_WEAPON_TWO_HANDED) outnValue = TEPT_BASE_WEAPON_TWO_HANDED;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_BASE_WEAPON_STAFF) outnValue = TEPT_BASE_WEAPON_STAFF;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_BASE_WEAPON_ARCHERY) outnValue = TEPT_BASE_WEAPON_ARCHERY;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_BASE_WEAPON_2H_BLUNT) outnValue = TEPT_BASE_WEAPON_2H_BLUNT;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_BASE_WEAPON_DUAL_PIERCE) outnValue = TEPT_BASE_WEAPON_DUAL_PIERCE;		

	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_BASE_WEAPON_GUN) outnValue = TEPT_BASE_WEAPON_GUN;		
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_BASE_WEAPON_BOOK) outnValue = TEPT_BASE_WEAPON_BOOK;

	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_BASE_WEAPON_SNEAK) outnValue = TEPT_BASE_SNEAK;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_BASE_ELEMENTAL) outnValue = TEPT_BASE_ELEMENTAL;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_BASE_HOLY) outnValue = TEPT_BASE_HOLY;

	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_SPECIALIZATION_SLASH) outnValue = TEPT_SPECIALIZATION_SLASH;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_SPECIALIZATION_BLUNT) outnValue = TEPT_SPECIALIZATION_BLUNT;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_SPECIALIZATION_PIERCE) outnValue = TEPT_SPECIALIZATION_PIERCE;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_SPECIALIZATION_ELEMENTAL) outnValue = TEPT_SPECIALIZATION_ELEMENTAL;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_SPECIALIZATION_HOLY) outnValue = TEPT_SPECIALIZATION_HOLY;

	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_UTILITY_SAFEFALL) outnValue = TEPT_UTILITY_SAFEFALL;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_UTILITY_SAFEFALL2) outnValue = TEPT_UTILITY_SAFEFALL2;

	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_BASE_ARMOR_LIGHT) outnValue = TEPT_BASE_ARMOR_LIGHT;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_BASE_ARMOR_MEDIUM) outnValue = TEPT_BASE_ARMOR_MEDUIM;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_BASE_ARMOR_HEAVY) outnValue = TEPT_BASE_ARMOR_HEAVY;

	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_TALENT_SHOUT_RADIUS) outnValue = TEPT_TALENT_SHOUT_RADIUS;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_TALENT_SAZAHU) outnValue = TEPT_TALENT_SAZAHU_SPECIALIZATION;

	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_FOCUS_USE_BERSERK) outnValue = TEPT_FOCUS_USE_BERSERK;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_FOCUS_USE_COUNTER) outnValue = TEPT_FOCUS_USE_COUNTER;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_FOCUS_BERSERK_DECREASE_MAXBUBBLE) outnValue = TEPT_FOCUS_BERSERK_DECREASE_MAXBUBBLE;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_FOCUS_BERSERK_EXTENDTIME) outnValue = TEPT_FOCUS_BERSERK_EXTENDTIME;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_FOCUS_ADVENT_EXTENDTIME) outnValue = TEPT_FOCUS_ADVENT_EXTENDTIME;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_FOCUS_COUNTER_EXTENDTIME) outnValue = TEPT_FOCUS_COUNTER_EXTENDTIME;

	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_3COMBO) outnValue = TEPT_3COMBO;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_SWORD_DRAWING) outnValue = TEPT_SWORD_DRAWING;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_QUICK_RISE) outnValue = TEPT_QUICK_RISE;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_QUICK_DRAW) outnValue = TEPT_QUICK_DRAW;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_UPPERCUT) outnValue = TEPT_UPPERCUT;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_RISING_ATTACK) outnValue = TEPT_RISING_ATTACK;		
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_GUARD) outnValue = TEPT_GUARD;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_COUNTER) outnValue = TEPT_COUNTER;		
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_MAGICAL_DYNAMICS) outnValue = TEPT_MAGICAL_DYNAMICS;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_ITEM_THROWING) outnValue = TEPT_ITEM_THROWING;
	else if (strValue == TALENT_XML_VALUE_EXTRAPASSIVE_ITEM_TRAP) outnValue = TEPT_ITEM_TRAP;
}

void CSTalentInfoParser::_ParseExtraPassiveType( MXmlElement* pElement, CSTalentInfo* pTalentInfo )
{
	string strValue;
	if (_Attribute(strValue,							pElement, TALENT_XML_ATTR_EXTRA_PASSIVE_TYPE))
	{   
		ParseExtraPassiveType(strValue, pTalentInfo->m_nExtraPassive);
	}
	if (_Attribute(strValue,							pElement, TALENT_XML_ATTR_EXTRA_PASSIVE_TYPE2))
	{   
		ParseExtraPassiveType(strValue, pTalentInfo->m_nExtraPassive2);
	}

	if (_Attribute(strValue,	pElement, TALENT_XML_ATTR_EXTRA_PASSIVE_PARAM))
	{
		if (strValue == "$ID")
		{
			pTalentInfo->m_nExtraPassiveParam = pTalentInfo->m_nID;
		}
		else
		{
			pTalentInfo->m_nExtraPassiveParam = atoi(strValue.c_str());
		}
	}

	if (_Attribute(strValue,	pElement, TALENT_XML_ATTR_EXTRA_PASSIVE_PARAM2))
	{
		if (strValue == "$ID")
		{
			pTalentInfo->m_nExtraPassiveParam2 = pTalentInfo->m_nID;
		}
		else
		{
			pTalentInfo->m_nExtraPassiveParam2 = atoi(strValue.c_str());
		}
	}
}

void CSTalentInfoParser::_ParseExtraActiveType( MXmlElement* pElement, CSTalentInfo* pTalentInfo )
{
	string strValue;
	if (_Attribute(strValue,							pElement, TALENT_XML_ATTR_EXTRA_ACTIVE_TYPE))
	{
		if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_PORTAL) pTalentInfo->m_nExtraActive = TEAT_PORTAL;
		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_PORTAL_SOULBINDING) pTalentInfo->m_nExtraActive = TEAT_PORTAL_SOULBINDING;
		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_REVIVE) pTalentInfo->m_nExtraActive = TEAT_REBIRTH;
		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_TAUNT) pTalentInfo->m_nExtraActive = TEAT_TAUNT;
		
		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_BUFF_ENTITY) pTalentInfo->m_nExtraActive = TEAT_BUFF_ENTITY;

		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_SPAWN) pTalentInfo->m_nExtraActive = TEAT_SPAWN;

		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_NPC_ONLY_GRAPPLE) pTalentInfo->m_nExtraActive = TEAT_NPC_ONLY_GRAPPLE;
		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_NPC_ONLY_EAT) pTalentInfo->m_nExtraActive = TEAT_NPC_ONLY_EAT;
		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_TOGGLE_PRECISION) pTalentInfo->m_nExtraActive = TEAT_TOGGLE_PRECISION;
		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_TOGGLE_SNEAK) pTalentInfo->m_nExtraActive = TEAT_TOGGLE_SNEAK;
		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_ADVENT) pTalentInfo->m_nExtraActive = TEAT_ADVENT;		

		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_DISPEL)	pTalentInfo->m_nExtraActive = TEAT_DISPEL;
		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_CURE_POISON)	pTalentInfo->m_nExtraActive = TEAT_CURE_POISON;
		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_CURE_DISEASE)	pTalentInfo->m_nExtraActive = TEAT_CURE_DISEASE;
		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_CURE_CURSE) pTalentInfo->m_nExtraActive = TEAT_CURE_CURSE;
		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_DISPEL_MESMERIZE)	pTalentInfo->m_nExtraActive = TEAT_DISPEL_MESMERIZE;
		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_DISPEL_DEATHPENALTY)	pTalentInfo->m_nExtraActive = TEAT_DISPEL_DEATHPENALTY;
		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_DISPEL_BUFF)	pTalentInfo->m_nExtraActive = TEAT_DISPEL_BUFF;
		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_DISPEL_FOCUS)	pTalentInfo->m_nExtraActive = TEAT_DISPEL_FOCUS;
		else if (strValue == TALENT_XML_VALUE_EXTRAACTIVE_DISPEL_BPART)	pTalentInfo->m_nExtraActive = TEAT_DISPEL_BPART;
	}

	_Attribute((int*)&pTalentInfo->m_nExtraActiveParam1,	pElement, TALENT_XML_ATTR_EXTRA_ACTIVE_PARAM1);
	_Attribute((int*)&pTalentInfo->m_nExtraActiveParam2,	pElement, TALENT_XML_ATTR_EXTRA_ACTIVE_PARAM2);
}

void CSTalentInfoParser::_ParseSkillType( MXmlElement* pElement, CSTalentInfo* pTalentInfo )
{
	string strValue;
	if (_Attribute(strValue,							pElement, TALENT_XML_ATTR_SKILL_TYPE))
	{
		if (strValue == TALENT_XML_VALUE_SKILLTYPE_NONE) pTalentInfo->m_nSkillType = ST_NONE;
		else if (strValue == TALENT_XML_VALUE_SKILLTYPE_MELEE) pTalentInfo->m_nSkillType = ST_MELEE;
		else if (strValue == TALENT_XML_VALUE_SKILLTYPE_ARCHERY) pTalentInfo->m_nSkillType = ST_ARCHERY;
		else if (strValue == TALENT_XML_VALUE_SKILLTYPE_GUN) pTalentInfo->m_nSkillType = ST_GUN;
		else if (strValue == TALENT_XML_VALUE_SKILLTYPE_MAGIC) pTalentInfo->m_nSkillType = ST_MAGIC;
		else if (strValue == TALENT_XML_VALUE_SKILLTYPE_GUARD) pTalentInfo->m_nSkillType = ST_GUARD;
		else if (strValue == TALENT_XML_VALUE_SKILLTYPE_PASSIVE) pTalentInfo->m_nSkillType = ST_PASSIVE;
		else if (strValue == TALENT_XML_VALUE_SKILLTYPE_EXTRA_ACTIVE) pTalentInfo->m_nSkillType = ST_EXTRA_ACTIVE;
		else if (strValue == TALENT_XML_VALUE_SKILLTYPE_EXTRA_PASSIVE) pTalentInfo->m_nSkillType = ST_EXTRA_PASSIVE;
		else if (strValue == TALENT_XML_VALUE_SKILLTYPE_SOCIAL) pTalentInfo->m_nSkillType = ST_SOCIAL;
		else if (strValue == TALENT_XML_VALUE_SKILLTYPE_ITEM) pTalentInfo->m_nSkillType = ST_ITEM;
	}
}

void CSTalentInfoParser::_ParseNPCChangeMode( MXmlElement* pElement, CSTalentInfo* pTalentInfo )
{
	string strValue;
	if (_Attribute(strValue,	pElement, TALENT_XML_ATTR_NPC_CHANGE_MODE))
	{
		pTalentInfo->m_ChangeModeInfo.m_bChangeMode = true;
		if (strValue == TALENT_XML_VALUE_TO_MODE0) pTalentInfo->m_ChangeModeInfo.m_nModeTo = NPC_MODE_DEFAULT;
		else if (strValue == TALENT_XML_VALUE_TO_MODE1) pTalentInfo->m_ChangeModeInfo.m_nModeTo = NPC_MODE_1;
		else if (strValue == TALENT_XML_VALUE_TO_MODE2) pTalentInfo->m_ChangeModeInfo.m_nModeTo = NPC_MODE_2;
		else if (strValue == TALENT_XML_VALUE_TO_MODE3) pTalentInfo->m_ChangeModeInfo.m_nModeTo = NPC_MODE_3;
		else if (strValue == TALENT_XML_VALUE_TO_MODE4) pTalentInfo->m_ChangeModeInfo.m_nModeTo = NPC_MODE_4;
		else if (strValue == TALENT_XML_VALUE_TO_MODE5) pTalentInfo->m_ChangeModeInfo.m_nModeTo = NPC_MODE_5;
		else if (strValue == TALENT_XML_VALUE_TO_MODE6) pTalentInfo->m_ChangeModeInfo.m_nModeTo = NPC_MODE_6;
		else if (strValue == TALENT_XML_VALUE_TO_MODE7) pTalentInfo->m_ChangeModeInfo.m_nModeTo = NPC_MODE_7;
		else if (strValue == TALENT_XML_VALUE_TO_MODE8) pTalentInfo->m_ChangeModeInfo.m_nModeTo = NPC_MODE_8;
		else if (strValue == TALENT_XML_VALUE_TO_MODE9) pTalentInfo->m_ChangeModeInfo.m_nModeTo = NPC_MODE_9;
		else if (strValue == TALENT_XML_VALUE_TO_MODE10) pTalentInfo->m_ChangeModeInfo.m_nModeTo = NPC_MODE_10;
		else
		{
			pTalentInfo->m_ChangeModeInfo.m_bChangeMode = false;
		}
	}
}

void CSTalentInfoParser::_ParseFocus( MXmlElement* pElement, CSTalentInfo* pTalentInfo )
{
	string strValue;
	if (_Attribute(strValue,							pElement, TALENT_XML_ATTR_NEED_FOCUS))
	{
		if (strValue == TALENT_XML_VALUE_FOCUS_COUNTER)			pTalentInfo->m_nRequireFocus = TFT_COUNTER;
		else if (strValue == TALENT_XML_VALUE_FOCUS_BERSERK)	pTalentInfo->m_nRequireFocus = TFT_BERSERK;
		else if (strValue == TALENT_XML_VALUE_FOCUS_SNEAK)		pTalentInfo->m_nRequireFocus = TFT_SNEAK;
		else if (strValue == TALENT_XML_VALUE_FOCUS_PRECISION)	pTalentInfo->m_nRequireFocus = TFT_PRECISION;
		else if (strValue == TALENT_XML_VALUE_FOCUS_ENLIGHTEN)	pTalentInfo->m_nRequireFocus = TFT_ENLIGHTEN;
		else if (strValue == TALENT_XML_VALUE_FOCUS_ADVENT)		pTalentInfo->m_nRequireFocus = TFT_ADVENT;
	}
	if (_Attribute(strValue,							pElement, TALENT_XML_ATTR_NEXT_FOCUS))
	{
		if (strValue == TALENT_XML_VALUE_FOCUS_COUNTER)			pTalentInfo->m_nNextFocus = TFT_COUNTER;
		else if (strValue == TALENT_XML_VALUE_FOCUS_BERSERK)	pTalentInfo->m_nNextFocus = TFT_BERSERK;
		else if (strValue == TALENT_XML_VALUE_FOCUS_SNEAK)		pTalentInfo->m_nNextFocus = TFT_SNEAK;
		else if (strValue == TALENT_XML_VALUE_FOCUS_PRECISION)	pTalentInfo->m_nNextFocus = TFT_PRECISION;
		else if (strValue == TALENT_XML_VALUE_FOCUS_ENLIGHTEN)	pTalentInfo->m_nNextFocus = TFT_ENLIGHTEN;
		else if (strValue == TALENT_XML_VALUE_FOCUS_ADVENT)		pTalentInfo->m_nNextFocus = TFT_ADVENT;
	}
}

void CSTalentInfoParser::_ParseWeaponAllowed( MXmlElement* pElement, CSTalentInfo* pTalentInfo )
{
	tstring strValue;
	if (_Attribute(strValue,				pElement, TALENT_XML_ATTR_WEAPON_ALLOWED))
	{
		CSTalentInfoWeaponAllowedParser parser;
		parser.Parse(strValue.c_str(), pTalentInfo->m_WeaponAllowedInfo);
	}
}

void CSTalentInfoParser::_ParseHit( CSTalentInfo* pTalentInfo, MXmlElement* pElement )
{
	_Attribute(&pTalentInfo->m_fHitStartTime,				pElement, TALENT_XML_ATTR_HIT_START_TIME);
	_Attribute(&pTalentInfo->m_fHitDurationTime,			pElement, TALENT_XML_ATTR_HIT_DURATION_TIME);
	_Attribute(&pTalentInfo->m_fHitSize,					pElement, TALENT_XML_ATTR_HIT_SIZE);
	_Attribute(&pTalentInfo->m_vHitPosition.x,				pElement, TALENT_XML_ATTR_HIT_POSITION_X);
	_Attribute(&pTalentInfo->m_vHitPosition.y,				pElement, TALENT_XML_ATTR_HIT_POSITION_Y);
	_Attribute(&pTalentInfo->m_vHitPosition.z,				pElement, TALENT_XML_ATTR_HIT_POSITION_Z);
	_Attribute(&pTalentInfo->m_fHitDelay,					pElement, TALENT_XML_ATTR_HIT_DELAY);
	_Attribute(&pTalentInfo->m_bSeparateHit,				pElement, TALENT_XML_ATTR_SEPARATE_HIT);
	_Attribute(&pTalentInfo->m_bHitCapsulePosSync,			pElement, TALENT_XML_ATTR_HIT_POS_SYNC);

	// ���� ����
	_Attribute(&pTalentInfo->m_HitFilter.bMotionFactor[MF_BEATEN],		pElement, TALENT_XML_ATTR_HITFILTER_MF_BEATEN);
	_Attribute(&pTalentInfo->m_HitFilter.bMotionFactor[MF_KNOCKBACK],	pElement, TALENT_XML_ATTR_HITFILTER_MF_KNOCKBACK);
	_Attribute(&pTalentInfo->m_HitFilter.bMotionFactor[MF_STUN],		pElement, TALENT_XML_ATTR_HITFILTER_MF_STUN);
	_Attribute(&pTalentInfo->m_HitFilter.bMotionFactor[MF_DEFLECT],	pElement, TALENT_XML_ATTR_HITFILTER_MF_DEFLECT);	
	_Attribute(&pTalentInfo->m_HitFilter.bMotionFactor[MF_THROWUP],	pElement, TALENT_XML_ATTR_HITFILTER_MF_THROWUP);
	_Attribute(&pTalentInfo->m_HitFilter.bMotionFactor[MF_KNOCKDOWN],	pElement, TALENT_XML_ATTR_HITFILTER_MF_KNOCKDOWN);
	_Attribute(&pTalentInfo->m_HitFilter.bMotionFactor[MF_UPPERED],	pElement, TALENT_XML_ATTR_HITFILTER_MF_UPPERED);
	_Attribute(&pTalentInfo->m_HitFilter.bAltitude[AT_NORMAL],			pElement, TALENT_XML_ATTR_HITFILTER_ALTITUDE_NORMAL);
	_Attribute(&pTalentInfo->m_HitFilter.bAltitude[AT_FLOATING],		pElement, TALENT_XML_ATTR_HITFILTER_ALTITUDE_FLOATING);
	_Attribute(&pTalentInfo->m_HitFilter.bAltitude[AT_SKY],			pElement, TALENT_XML_ATTR_HITFILTER_ALTITUDE_SKY);	
}

void CSTalentInfoParser::ParseResist( GTalentResist& Resist, MXmlElement* pElement, 
									 const char* pszAttrResistMethod, const char* pszAttrResistType, const char* pszAttrResistDifficult )
{
	string strValue;
	if (_Attribute(strValue,				pElement, pszAttrResistMethod)) 
	{
		if (!_stricmp(strValue.c_str(), TALENT_XML_VALUE_RESIST_BINARY)) Resist.m_nMethod = TRM_BINARY;
		else if (!_stricmp(strValue.c_str(), TALENT_XML_VALUE_RESIST_PARTIAL)) Resist.m_nMethod = TRM_PARTIAL;
	}

	if (_Attribute(strValue,				pElement, pszAttrResistType)) 
	{
		if (!_stricmp(strValue.c_str(), TALENT_XML_VALUE_RESIST_FR)) Resist.m_nType = TRT_FR;
		else if (!_stricmp(strValue.c_str(), TALENT_XML_VALUE_RESIST_CR)) Resist.m_nType = TRT_CR;
		else if (!_stricmp(strValue.c_str(), TALENT_XML_VALUE_RESIST_PR)) Resist.m_nType = TRT_PR;
		else if (!_stricmp(strValue.c_str(), TALENT_XML_VALUE_RESIST_LR)) Resist.m_nType = TRT_LR;
		else if (!_stricmp(strValue.c_str(), TALENT_XML_VALUE_RESIST_HR)) Resist.m_nType = TRT_HR;
		else if (!_stricmp(strValue.c_str(), TALENT_XML_VALUE_RESIST_UR)) Resist.m_nType = TRT_UR;
		else if (!_stricmp(strValue.c_str(), TALENT_XML_VALUE_RESIST_CHA)) Resist.m_nType = TRT_CHA;
	}

	int nResistDIfficulty;
	if (_Attribute(&nResistDIfficulty,			pElement, pszAttrResistDifficult))
	{
		Resist.m_nDifficulty = uint8(nResistDIfficulty);
	}
}

void CSTalentInfoParser::_ParseFinishEffect( MXmlElement* pElement, CSTalentInfo* pTalentInfo )
{
	string strValue;
	if (_Attribute(strValue,							pElement, TALENT_XML_ATTR_FINISH_EFFECT))
	{
		if (strValue == TALENT_XML_VALUE_FINISH_EFFECT_NONE)
		{
			pTalentInfo->m_FinishEffect.m_nFinishType = TALENT_FINISH_NONE;
			pTalentInfo->m_FinishEffect.m_bBreakableParts = false;
		}
		else if (strValue == TALENT_XML_VALUE_FINISH_EFFECT_BPARTS)	
		{
			pTalentInfo->m_FinishEffect.m_nFinishType = TALENT_FINISH_NONE;
			pTalentInfo->m_FinishEffect.m_bBreakableParts = true;
		}
		else if (strValue == TALENT_XML_VALUE_FINISH_EFFECT_THROWUP)	
		{
			pTalentInfo->m_FinishEffect.m_nFinishType = TALENT_FINISH_THROW_UP;
			pTalentInfo->m_FinishEffect.m_bBreakableParts = false;
		}
		else if (strValue == TALENT_XML_VALUE_FINISH_EFFECT_KNOCKBACK)	
		{
			pTalentInfo->m_FinishEffect.m_nFinishType = TALENT_FINISH_KNOCK_BACK;
			pTalentInfo->m_FinishEffect.m_bBreakableParts = false;
		}
		else if (strValue == TALENT_XML_VALUE_FINISH_EFFECT_RANDOM)	
		{
			pTalentInfo->m_FinishEffect.m_nFinishType = TALENT_FINISH_RANDOM;
			pTalentInfo->m_FinishEffect.m_bBreakableParts = false;
		}
		else if (strValue == TALENT_XML_VALUE_FINISH_EFFECT_THROWUP_BPARTS)	
		{
			pTalentInfo->m_FinishEffect.m_nFinishType = TALENT_FINISH_THROW_UP;
			pTalentInfo->m_FinishEffect.m_bBreakableParts = true;
		}
		else if (strValue == TALENT_XML_VALUE_FINISH_EFFECT_KNOCKBACK_BPARTS)	
		{
			pTalentInfo->m_FinishEffect.m_nFinishType = TALENT_FINISH_KNOCK_BACK;
			pTalentInfo->m_FinishEffect.m_bBreakableParts = true;
		}
		else if (strValue == TALENT_XML_VALUE_FINISH_EFFECT_RANDOM_BPARTS)	
		{
			pTalentInfo->m_FinishEffect.m_nFinishType = TALENT_FINISH_RANDOM;
			pTalentInfo->m_FinishEffect.m_bBreakableParts = true;
		}
	}
}

void CSTalentInfoParser::_ParseUsableAtDead( MXmlElement* pElement, CSTalentInfo* pTalentInfo )
{
	// �������� ����ؼ� �ڱ⸸ �츮�� ��Ȱ�ŷ�Ʈ�� �׾����� ��밡�� �ϴ�.
	if (ST_ITEM == pTalentInfo->m_nSkillType &&
		TEAT_REBIRTH == pTalentInfo->m_nExtraActive && 
		CSEffectInfo::POINT_SELF == pTalentInfo->m_EffectInfo.m_nPoint &&
		0.0f == pTalentInfo->m_EffectInfo.m_fRadius &&
		CSEffectInfo::RELATION_ALLIED_DEAD == pTalentInfo->m_EffectInfo.m_nRelation)
	{
		pTalentInfo->m_bUsableAtDead = true;
	}
}


//////////////////////////////////////////////////////////////////////////
void CSTalentInfoWeaponAllowedParser::Parse( const TCHAR* szValue, CSTalentWeaponAllowdInfo& outWeaponAllowedInfo )
{
	MCsvParser csv_parser;
	csv_parser.LoadFromStream(MLocale::ConvTCHARToAnsi(szValue).c_str());
	int nColCount = csv_parser.GetColCount(0);

	char szWeapon[1024];
	for (int nCol = 0; nCol < nColCount; nCol++)
	{
		if (csv_parser.GetData(nCol, 0, szWeapon, 1024))
		{
			string strWeapon = szWeapon;
			string strTrimWeapon = AString::ToLower(AString::TrimBlanks(strWeapon));

			if (strTrimWeapon == "1hs")
			{
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_1HS);
			}
			else if (strTrimWeapon == "1hb")
			{
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_1HB);
			}
			else if (strTrimWeapon == "shd")
			{
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_SHD);
			}
			else if (strTrimWeapon == "2hd_h")
			{
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_2HD_H);
			}
			else if (strTrimWeapon == "2hd_l")
			{
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_2HD_L);
			}
			else if (strTrimWeapon == "1hp_s")
			{
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_1HP_S);
			}
			else if (strTrimWeapon == "1hp_l")
			{
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_1HP_L);
			}
			else if (strTrimWeapon == "dwd")
			{
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_DWD);
			}
			else if (strTrimWeapon == "bow")
			{
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_BOW);
			}
			else if (strTrimWeapon == "2hb")
			{
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_2HB);
			}
			else if (strTrimWeapon == "sta")
			{
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_STF);
			}
			else if (strTrimWeapon == "all")
			{
				outWeaponAllowedInfo.m_bAllAllowed = true;
				outWeaponAllowedInfo.m_setWeapons.clear();
				return;
			}
			else if (strTrimWeapon == "melee")
			{
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_1HS);
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_1HB);
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_2HD_H);
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_2HD_L);
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_1HP_S);
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_1HP_L);
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_DWD);
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_STF);
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_2HB);
			}
			else if (strTrimWeapon == "2hd")
			{
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_2HD_H);
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_2HD_L);
			}
			else if (strTrimWeapon == "1hp")
			{
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_1HP_S);
				outWeaponAllowedInfo.m_setWeapons.insert(TALENT_WEAPON_ALLOWED_1HP_L);
			}
		}
	}

	if (outWeaponAllowedInfo.m_setWeapons.empty() == false)
	{
		outWeaponAllowedInfo.m_bAllAllowed = false;
	}
	if (outWeaponAllowedInfo.m_setWeapons.find(TALENT_WEAPON_ALLOWED_SHD) != outWeaponAllowedInfo.m_setWeapons.end())
	{
		outWeaponAllowedInfo.m_bNeedEquipShield = true;
	}
}

bool CSTalentWeaponAllowdInfo::IsAllowed( WEAPON_TYPE nWeaponType, bool bEquipShield )
{
	if (m_bAllAllowed) return true;
	if (m_bNeedEquipShield)
	{
		if (bEquipShield == false) return false;
	}

	switch (nWeaponType)
	{
	case WEAPON_NONE:
		{

		}
		break;
	case WEAPON_1H_SLASH:
		{
			if (IsAllowed(TALENT_WEAPON_ALLOWED_1HS)) return true;
		}
		break;
	case WEAPON_1H_BLUNT:
		{
			if (IsAllowed(TALENT_WEAPON_ALLOWED_1HB)) return true;
		}
		break;
	case WEAPON_1H_PIERCE:
		{
			if (IsAllowed(TALENT_WEAPON_ALLOWED_1HP_S)) return true;
		}
		break;
	case WEAPON_TWO_HANDED:
		{
			if (IsAllowed(TALENT_WEAPON_ALLOWED_2HD_H)) return true;
		}
		break;
	case WEAPON_STAFF:
		{
			if (IsAllowed(TALENT_WEAPON_ALLOWED_STF)) return true;
		}
		break;
	case WEAPON_ARCHERY:
		{
			if (IsAllowed(TALENT_WEAPON_ALLOWED_BOW)) return true;
		}
		break;
	case WEAPON_2H_BLUNT:
		{
			if (IsAllowed(TALENT_WEAPON_ALLOWED_2HB)) return true;
		}
		break;
	case WEAPON_DUAL_WIELD:
		{
			if (IsAllowed(TALENT_WEAPON_ALLOWED_DWD)) return true;
		}
		break;
	case WEAPON_DUAL_PIERCE:
		{
//			_ASSERT(0);
		}
		break;
	default:
		{
			_ASSERT(0);
		}
	}
	return false;
}

bool CSTalentWeaponAllowdInfo::IsAllowed( TALENT_WEAPON_ALLOWED nWeapon )
{
	if (m_bAllAllowed) return true;
	if (m_setWeapons.find(nWeapon) != m_setWeapons.end()) return true;
	return false;
}




CSTalentInfoParserHelper::TalentInfoID CSTalentInfoParserHelper::ParseTalentID( MXml* pXml, MXmlElement* pElement )
{
	TalentInfoID retTalentInfoID;
	_Attribute(&retTalentInfoID.nID, pElement, TALENT_XML_ATTR_ID);

	tstring strValue;
	if (_Attribute(strValue, pElement, TALENT_XML_ATTR_MODE))
	{
		retTalentInfoID.nMode = MakeModeFromXmlValue(strValue);
	}

	return retTalentInfoID;
}

tstring CSTalentInfoParserHelper::MakeXmlValueFromMode( int nMode )
{
	tstring retValue;

	if (nMode > 0 && nMode < WEAPON_MAX)
	{
		retValue = CSStrings::WeaponType(nMode);
	}

	return retValue;
}

int CSTalentInfoParserHelper::MakeModeFromXmlValue( tstring strValue )
{
	for (int i = 0; i < WEAPON_MAX; i++)
	{
		if (!_tcsicmp(CSStrings::WeaponType(i), strValue.c_str()))
		{
			return i;
		}
	}
	return 0;
}

void CSTalentInfoParserHelper::SetXmlElement_TalentIDAndMode( MXmlElement* pElement, int nID, int nMode )
{
	_SetAttribute(pElement, TALENT_XML_ATTR_ID,				nID);
	if (nMode > 0)
	{
		string strValue;
		CSTalentInfoParserHelper helper;
		strValue = MLocale::ConvTCHARToAnsi(helper.MakeXmlValueFromMode(nMode).c_str());

		_SetAttribute(pElement, TALENT_XML_ATTR_MODE, strValue);
	}
}
