#include "StdAfx.h"
#include "TalentEffectBase.h"
#include "TTalentEffectMgr.h"
#include "Xmlseral.h"

System::Void TalentEffectBase::SetTalentEffectBaseInfo( TTalentEffectMgr *	pEffectMgr )
{
	m_pEffectMgr = pEffectMgr;
	m_nEffectBaseType = TEBY_MELEE_ATTACK;

	SetPropertyTalentEffectBase();

	m_bTextChange = false;
}

bool Character_NET::TalentEffectBase::GetTalentEffectBaseInfo( TTalentEffectMgr *	pOutEffectMgr )
{
	m_pEffectMgr = NULL;
	m_nEffectBaseType = TEBY_MELEE_ATTACK;

	return m_bTextChange;
}

System::Void Character_NET::TalentEffectBase::SetEffectBaseType( string& strBuff, TALENT_EFFECT_BASE_TYPE nBaseType )
{
	if(nBaseType == TEBY_MELEE_ATTACK)
	{
		strBuff = "Melee";
	}
	else if(nBaseType == TEBY_MELEE_SECONDARY_ATTACK)
	{
		strBuff = "MeleeSecondary";
	}
	else if(nBaseType == TEBY_MAGIC_ATTACK)
	{
		strBuff = "Magic";
	}
	else if(nBaseType == TEBY_ARCHERY_ATTACK)
	{
		strBuff = "Archery";
	}
	else if(nBaseType == TEBY_GUN_ATTACK)
	{
		strBuff = "Gun";
	}
	else if(nBaseType == TEBY_DEFENSE)
	{
		strBuff = "Defense";
	}
	else if(nBaseType == TEBY_NOSHIELD_DEFENSE)
	{
		strBuff = "NoShieldDefense";
	}	
	else if(nBaseType == TEBY_MONSTER_DEFENSE)
	{
		strBuff = "MonsterDefense";
	}
}

TALENT_EFFECT_BASE_TYPE Character_NET::TalentEffectBase::GetEffectBaseType( string& strBuff )
{
	TALENT_EFFECT_BASE_TYPE eType = TEBY_MELEE_ATTACK;				// �⺻��

	if(strBuff == "Melee")
	{
		eType = TEBY_MELEE_ATTACK;	
	}
	else if(strBuff == "MeleeSecondary")
	{
		eType = TEBY_MELEE_SECONDARY_ATTACK;	
	}
	else if(strBuff == "Magic")
	{
		eType = TEBY_MAGIC_ATTACK;
	}
	else if(strBuff == "Archery")
	{
		eType = TEBY_ARCHERY_ATTACK;
	}
	else if(strBuff == "Gun")
	{
		eType = TEBY_GUN_ATTACK;
	}
	else if(strBuff == "Defense")
	{
		eType = TEBY_DEFENSE;
	}
	else if(strBuff == "NoShieldDefense")
	{
		eType = TEBY_NOSHIELD_DEFENSE;
	}
	else if(strBuff == "MonsterDefense")
	{
		eType = TEBY_MONSTER_DEFENSE;
	}

	return eType;
}

System::Void Character_NET::TalentEffectBase::SetTextImpactPositionList( string& strBuff, IMPACTPOS nImapctPos )
{
	if(nImapctPos == HIT_POS)
	{
		strBuff = "������ġ";
	}
	else if(nImapctPos == BONE_POS)
	{
		strBuff = "�� ��ġ";
	}
	else if(nImapctPos == SHOCK_GROUND_POS)
	{
		strBuff = "���� ��ġ(����)";
	}
	else //if(nImapctPos == MODEL_POS)
	{
		strBuff = "����ġ";
	}
}

IMPACTPOS Character_NET::TalentEffectBase::GetEffectImpactPositionList( string& strBuff )
{
	IMPACTPOS eImpactPos = MODEL_POS;

	if(strBuff == "����ġ")
	{
		eImpactPos = MODEL_POS;	
	}
	else if(strBuff == "������ġ")
	{
		eImpactPos = HIT_POS;
	}
	else if(strBuff == "�� ��ġ")
	{
		eImpactPos = BONE_POS;
	}	
	else if(strBuff == "���� ��ġ(����)")
	{
		eImpactPos = SHOCK_GROUND_POS;
	}

	return eImpactPos;
}

System::Void Character_NET::TalentEffectBase::SetPropertyTalentEffectBase()
{
	XmlSerializable::TALENTEFFECT_BASE^ xeffect	= (XmlSerializable::TALENTEFFECT_BASE^)(TalentEffectase_Att->SelectedObject);

	// 1. ���� �Է�
	if(xeffect == nullptr)
	{
		SetTalentEffectBaseToProperty(m_pEffectMgr->GetTalentEffectBase(m_nEffectBaseType));
	}
	// 2. ���� ����
	else
	{
		// 2-1 Ÿ�� ����
		TALENT_EFFECT_BASE_TYPE nEffectBaseType = GetEffectBaseType((string)MStringToCharPointer(xeffect->EffectBaseType));
		if(m_nEffectBaseType != nEffectBaseType)
		{
			m_nEffectBaseType = nEffectBaseType;

			// ���� �Է�
			SetTalentEffectBaseToProperty(m_pEffectMgr->GetTalentEffectBase(m_nEffectBaseType));
		}
		// 2-2 ���� �Է�
		else
		{
			SetPropertyToTalentEffectBase(m_pEffectMgr->GetTalentEffectBase(m_nEffectBaseType));
			m_bTextChange = true;
		}
	}

}

System::Void Character_NET::TalentEffectBase::SetTalentEffectBaseToProperty( XTalentEffectBase * pEffectBase )
{
	// ������Ƽ���� �Է�
	XmlSerializable::TALENTEFFECT_BASE^ xeffectAtt = gcnew XmlSerializable::TALENTEFFECT_BASE;

	if(pEffectBase == NULL)
		return;

	string strBuff;
	SetEffectBaseType(strBuff, pEffectBase->m_nEffectBaseTYpe);
	xeffectAtt->EffectBaseType	= gcnew String(strBuff.c_str());

	xeffectAtt->EffectFileName	= gcnew String(pEffectBase->m_strEffectBaseFileName.c_str());
	xeffectAtt->���̸�			= gcnew String(pEffectBase->m_strBoneName.c_str());

	SetTextEffectDirectionList(strBuff, pEffectBase->m_nEffectDirection);
	xeffectAtt->����			= gcnew String(strBuff.c_str());
	
	SetTextImpactPositionList(strBuff, pEffectBase->m_nImpactPos);
	xeffectAtt->�浹��ġ���	= gcnew String(strBuff.c_str());

	TalentEffectase_Att->SelectedObject = xeffectAtt;
}

System::Void Character_NET::TalentEffectBase::SetPropertyToTalentEffectBase( XTalentEffectBase * pEffectBase )
{
	// ����Ʈ ������ �Է�
	XmlSerializable::TALENTEFFECT_BASE^ xeffect	= (XmlSerializable::TALENTEFFECT_BASE^)(TalentEffectase_Att->SelectedObject);

	if(pEffectBase == NULL)
		return;

	pEffectBase->m_nEffectBaseTYpe			= GetEffectBaseType((string)MStringToCharPointer(xeffect->EffectBaseType));

	pEffectBase->m_strEffectBaseFileName	= string(MStringToCharPointer(xeffect->EffectFileName));
	pEffectBase->m_strBoneName				= string(MStringToCharPointer(xeffect->���̸�));

	pEffectBase->m_nEffectDirection			= GetEffectDirectionList((string)MStringToCharPointer(xeffect->����));

	pEffectBase->m_nImpactPos				= GetEffectImpactPositionList((string)MStringToCharPointer(xeffect->�浹��ġ���));
}
