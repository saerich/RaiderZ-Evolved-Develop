#include "stdafx.h"
#include "TTalentEffectViewer.h"
#include "Xmlseral.h"
#include "TTalentEffectMgr.h"

//////////////////////////////////////////////////////////////////////////
TTalentEffectViewer::TTalentEffectViewer()
{
	m_pEffectMgr								= NULL;

	// �ŷ�Ʈ ����Ʈ �Լ� ������
	// ĳ��Ʈ
	TalentEffectFuncToProperty[0]				= &TTalentEffectViewer::SetTalentEffectCastToProperty;
	TalentEffectFuncToTalentEffectInfo[0]		= &TTalentEffectViewer::SetPropertyToTalentEffectCast;

	// ���
	TalentEffectFuncToProperty[1]				= &TTalentEffectViewer::SetTalentEffectUseToProperty;
	TalentEffectFuncToTalentEffectInfo[1]		= &TTalentEffectViewer::SetPropertyToTalentEffectUse;

	// �ǰ�
	TalentEffectFuncToProperty[2]				= &TTalentEffectViewer::SetTalentEffectAttackToProperty;
	TalentEffectFuncToTalentEffectInfo[2]		= &TTalentEffectViewer::SetPropertyToTalentEffectAttack;

	// ����
	TalentEffectFuncToProperty[3]				= &TTalentEffectViewer::SetTalentEffectMissToProperty;
	TalentEffectFuncToTalentEffectInfo[3]		= &TTalentEffectViewer::SetPropertyToTalentEffectMiss;

	// ���
	TalentEffectFuncToProperty[4]				= &TTalentEffectViewer::SetTalentEffectShockToProperty;
	TalentEffectFuncToTalentEffectInfo[4]		= &TTalentEffectViewer::SetPropertyToTalentEffectShock;

	// ����
	TalentEffectFuncToProperty[5]				= &TTalentEffectViewer::SetTalentEffectDurationToProperty;
	TalentEffectFuncToTalentEffectInfo[5]		= &TTalentEffectViewer::SetPropertyToTalentEffectDuration;

	// ���
	TalentEffectFuncToProperty[6]				= &TTalentEffectViewer::SetTalentEffectDefenseToProperty;
	TalentEffectFuncToTalentEffectInfo[6]		= &TTalentEffectViewer::SetPropertyToTalentEffectDefense;

	// ĳ��Ʈ ����
	TalentEffectFuncToProperty[7]				= &TTalentEffectViewer::SetTalentEffectCastEndToProperty;
	TalentEffectFuncToTalentEffectInfo[7]		= &TTalentEffectViewer::SetPropertyToTalentEffectCastEnd;

}

PropertyGrid^ TTalentEffectViewer::GetEffectViewerPropertGrid()
{
	return GlobalObjects::g_pMainForm->TalentEffectAttGrid;
}

void TTalentEffectViewer::SetTextEffectType( string& strBuffer, TALENT_EFFECT_TYPE nEffectType )
{
	if(nEffectType == CAST_EFFECT)
	{
		strBuffer = EFFECTYPE_STRING_CAST_EFFECT;
	}
	else if(nEffectType == USE_EFFECT)
	{
		strBuffer = EFFECTYPE_STRING_USE_EFFECT;
	}
	else if(nEffectType == ATTACK_EFFECT)
	{
		strBuffer = EFFECTYPE_STRING_ATTACK_EFFECT;
	}
	else if(nEffectType == MISS_EFFECT)
	{
		strBuffer = EFFECTYPE_STRING_MISS_EFFECT;
	}
	else if(nEffectType == SHOCK_EFFECT)
	{
		strBuffer = EFFECTYPE_STRING_SHOCK_EFFECT;
	}
	else if(nEffectType == DURATION_EFFECT)
	{
		strBuffer = EFFECTYPE_STRING_DURATION_EFFECT;
	}
	else if(nEffectType == DEFENSE_EFFECT)
	{
		strBuffer = EFFECTYPE_STRING_DEFENSE_EFFECT;
	}
	else if(nEffectType == CAST_END_EFFECT)
	{
		strBuffer = EFFECTYPE_STRING_CAST_END_EFFECT;
	}
	else
	{
		// ��.
		strBuffer = EFFECTYPE_STRING_NOT;
	}
}

void TTalentEffectViewer::SetTextImpactPositionList( string& strBuffer, IMPACTPOS nImapctPos )
{
	if(nImapctPos == HIT_POS)
	{
		strBuffer = IMPACTPOSITION_STRING_HIT_POS;
	}
	else if(nImapctPos == BONE_POS)
	{
		strBuffer = IMPACTPOSITION_STRING_BONE_POS;
	}
	else if(nImapctPos == SHOCK_GROUND_POS)
	{
		strBuffer = IMPACTPOSITION_STRING_SHOCK_GROUND_POS;
	}
	else if(nImapctPos == TALENT_HIT_CAPSULE_POS)
	{
		strBuffer = IMPACTPOSITION_STRING_TALENT_HIT_CAPSUL_POS;
	}
	else //if(nImapctPos == MODEL_POS)
	{
		strBuffer = IMPACTPOSITION_STRING_MODEL_POS;
	}
}

TALENT_EFFECT_TYPE TTalentEffectViewer::GetEffectType( string& strBuffer )
{
	TALENT_EFFECT_TYPE eType = CAST_EFFECT;				// �⺻��

	if(strBuffer == EFFECTYPE_STRING_CAST_EFFECT)
	{
		eType = CAST_EFFECT;	
	}
	else if(strBuffer == EFFECTYPE_STRING_USE_EFFECT)
	{
		eType = USE_EFFECT;
	}
	else if(strBuffer == EFFECTYPE_STRING_ATTACK_EFFECT)
	{
		eType = ATTACK_EFFECT;
	}
	else if(strBuffer == EFFECTYPE_STRING_MISS_EFFECT)
	{
		eType = MISS_EFFECT;
	}
	else if(strBuffer == EFFECTYPE_STRING_SHOCK_EFFECT)
	{
		eType = SHOCK_EFFECT;
	}
	else if(strBuffer ==  EFFECTYPE_STRING_DURATION_EFFECT)
	{
		eType = DURATION_EFFECT;
	}
	else if(strBuffer == EFFECTYPE_STRING_DEFENSE_EFFECT)
	{
		eType = DEFENSE_EFFECT;
	}
	else if(strBuffer == EFFECTYPE_STRING_CAST_END_EFFECT)
	{
		eType = CAST_END_EFFECT;
	}

	return eType;
}

IMPACTPOS TTalentEffectViewer::GetEffectImpactPositionList( string& strBuffer )
{
	IMPACTPOS eImpactPos = MODEL_POS;

	if(strBuffer == IMPACTPOSITION_STRING_MODEL_POS)
	{
		eImpactPos = MODEL_POS;	
	}
	else if(strBuffer == IMPACTPOSITION_STRING_HIT_POS)
	{
		eImpactPos = HIT_POS;
	}
	else if(strBuffer == IMPACTPOSITION_STRING_BONE_POS)
	{
		eImpactPos = BONE_POS;
	}	
	else if(strBuffer == IMPACTPOSITION_STRING_SHOCK_GROUND_POS)
	{
		eImpactPos = SHOCK_GROUND_POS;
	}
	else if(strBuffer == IMPACTPOSITION_STRING_TALENT_HIT_CAPSUL_POS)
	{
		eImpactPos = TALENT_HIT_CAPSULE_POS;
	}

	return eImpactPos;
}

void TTalentEffectViewer::SetEffectInfoFromPropertyGrid(XTalentEffectInfo* pEffectInfo)
{
	// ����Ʈ ����
	if(pEffectInfo == NULL)
	{
		m_eTalentEffectType = TALENT_EFFECT_TYPE_MAX;
		GlobalObjects::g_pMainForm->TalentEffectAttGrid->SelectedObject = nullptr;
		return;
	}

	// ������Ƽ���� ����
	int nEffectTypeIndex = 0;
	XmlSerializable::TALENTEFFECT^ xeffect	= (XmlSerializable::TALENTEFFECT^)(GlobalObjects::g_pMainForm->TalentEffectAttGrid->SelectedObject);

	// ������Ƽ���� Ÿ����  �����´�.
	nEffectTypeIndex = GetEffectType((string)MStringToCharPointer(xeffect->EffectType));

	if(nEffectTypeIndex > 7)
		return;

	// ����Ʈ �������� Ÿ���� �����´�.
	m_eTalentEffectType = pEffectInfo->m_nEffectType;
	int nEffectTypeFromInfo		= (int)pEffectInfo->m_nEffectType;

	// Ÿ���� �ٸ��� ��
	if(nEffectTypeFromInfo != nEffectTypeIndex)
	{
		// �ٸ���
		// �켱 �ٲ� Ÿ������ ���� ���� �Ѵ�.
		pEffectInfo->m_nEffectType	= (TALENT_EFFECT_TYPE)nEffectTypeIndex;
		(this->*TalentEffectFuncToProperty[nEffectTypeIndex])(pEffectInfo);

		pEffectInfo->Init();
	}

	(this->*TalentEffectFuncToTalentEffectInfo[nEffectTypeIndex])(pEffectInfo);
}

// ĳ����
bool TTalentEffectViewer::SetTalentEffectCastToProperty( XTalentEffectInfo * pTalentEffectInfo )
{
	// �ŷ�Ʈ ����Ʈ ������ ������Ƽ���� ����
	// TODO: pElement �� ��� �Ӽ����� �۶���Ƽ�� ǥ�����ش�.
	XmlSerializable::TALENTEFFECT^ xeffect = gcnew XmlSerializable::TALENTEFFECT;
	string strBuff;

	xeffect->TalentEffectInfoName = gcnew String(pTalentEffectInfo->m_strTalentEffectInfoName.c_str());

	SetTextEffectType(strBuff, pTalentEffectInfo->m_nEffectType);
	xeffect->EffectType		= gcnew String(strBuff.c_str());
	xeffect->EffectName		= gcnew String(pTalentEffectInfo->m_strEffectName.c_str());
	xeffect->���̸�			= gcnew String(pTalentEffectInfo->m_strBoneName.c_str());
	xeffect->Parameter		= gcnew String(pTalentEffectInfo->m_strParameter.c_str());

	// ����
	SetTextEffectDirectionList(strBuff, pTalentEffectInfo->m_nEffectDirection);
	xeffect->����			= gcnew String(strBuff.c_str());

	//����Ÿ ���ε� ?
	GlobalObjects::g_pMainForm->TalentEffectAttGrid->SelectedObject = xeffect;

	return true;
}

bool TTalentEffectViewer::SetPropertyToTalentEffectCast( XTalentEffectInfo * pTalentEffectInfo )
{
	// ������Ƽ ������ �ŷ�Ʈ ����Ʈ �������� ����
	XmlSerializable::TALENTEFFECT^ xeffect	= (XmlSerializable::TALENTEFFECT^)(GlobalObjects::g_pMainForm->TalentEffectAttGrid->SelectedObject);
	if(xeffect == nullptr)
		return false;

	pTalentEffectInfo->m_strTalentEffectInfoName = string(MStringToCharPointer(xeffect->TalentEffectInfoName));
	pTalentEffectInfo->m_nEffectType		= GetEffectType((string)MStringToCharPointer(xeffect->EffectType));

	pTalentEffectInfo->m_strEffectName		= string(MStringToCharPointer(xeffect->EffectName));
	pTalentEffectInfo->m_strBoneName		= string(MStringToCharPointer(xeffect->���̸�));
	pTalentEffectInfo->m_strParameter		= string(MStringToCharPointer(xeffect->Parameter));

	// ����
	pTalentEffectInfo->m_nEffectDirection =  GetEffectDirectionList((string)MStringToCharPointer(xeffect->����));

	return true;
}

// ���
bool TTalentEffectViewer::SetTalentEffectUseToProperty( XTalentEffectInfo * pTalentEffectInfo )
{
	SetTalentEffectAttackToProperty(pTalentEffectInfo);
	return true;
}

bool TTalentEffectViewer::SetPropertyToTalentEffectUse( XTalentEffectInfo * pTalentEffectInfo )
{
	SetPropertyToTalentEffectAttack(pTalentEffectInfo);
	return true;
}

// �ǰ�
bool TTalentEffectViewer::SetTalentEffectAttackToProperty( XTalentEffectInfo * pTalentEffectInfo )
{
	// �ŷ�Ʈ ����Ʈ ������ ������Ƽ���� ����
	// TODO: pElement �� ��� �Ӽ����� �۶���Ƽ�� ǥ�����ش�.
	XmlSerializable::TALENTEFFECT_EX^ xeffect = gcnew XmlSerializable::TALENTEFFECT_EX;
	string strBuff;

	xeffect->TalentEffectInfoName = gcnew String(pTalentEffectInfo->m_strTalentEffectInfoName.c_str());

	SetTextEffectType(strBuff, pTalentEffectInfo->m_nEffectType);
	xeffect->EffectType		= gcnew String(strBuff.c_str());
	xeffect->EffectName		= gcnew String(pTalentEffectInfo->m_strEffectName.c_str());
	xeffect->���̸�			= gcnew String(pTalentEffectInfo->m_strBoneName.c_str());
	xeffect->Parameter		= gcnew String(pTalentEffectInfo->m_strParameter.c_str());

	SetTextImpactPositionList(strBuff, (IMPACTPOS)pTalentEffectInfo->m_nImpactPos);
	xeffect->�浹��ġ���	= gcnew String(strBuff.c_str());

	// ����
	SetTextEffectDirectionList(strBuff, pTalentEffectInfo->m_nEffectDirection);
	xeffect->����			= gcnew String(strBuff.c_str());

	//����Ÿ ���ε� ?
	GlobalObjects::g_pMainForm->TalentEffectAttGrid->SelectedObject = xeffect;

	return true;
}

bool TTalentEffectViewer::SetPropertyToTalentEffectAttack( XTalentEffectInfo * pTalentEffectInfo )
{
	XmlSerializable::TALENTEFFECT_EX^ xeffect	= (XmlSerializable::TALENTEFFECT_EX^)(GlobalObjects::g_pMainForm->TalentEffectAttGrid->SelectedObject);
	if(xeffect == nullptr)
		return false;

	string strBuff;

	pTalentEffectInfo->m_strTalentEffectInfoName = string(MStringToCharPointer(xeffect->TalentEffectInfoName));
	pTalentEffectInfo->m_nEffectType		= GetEffectType((string)MStringToCharPointer(xeffect->EffectType));

	pTalentEffectInfo->m_strEffectName		= string(MStringToCharPointer(xeffect->EffectName));
	pTalentEffectInfo->m_strBoneName		= string(MStringToCharPointer(xeffect->���̸�));
	pTalentEffectInfo->m_strParameter		= string(MStringToCharPointer(xeffect->Parameter));

	pTalentEffectInfo->m_nImpactPos			= GetEffectImpactPositionList((string)MStringToCharPointer(xeffect->�浹��ġ���));

	// ����Ʈ Ÿ���� ����, ����̶��...
	if(	pTalentEffectInfo->m_nEffectType == SHOCK_EFFECT)
	{
		// �浹 ��ġ�� ���� �浹�� �����Ѵ�.
		pTalentEffectInfo->m_nImpactPos = SHOCK_GROUND_POS;
		SetTextImpactPositionList(strBuff, (IMPACTPOS)pTalentEffectInfo->m_nImpactPos);
		xeffect->�浹��ġ���	= gcnew String(strBuff.c_str());
	}

	//if( pTalentEffectInfo->m_nImpactPos == HIT_POS && pTalentEffectInfo->m_strBoneName.empty())
	//{
	//	pTalentEffectInfo->m_strBoneName = "Bip01";
	//	SetTalentEffectAttackToProperty(pTalentEffectInfo);
	//}

	// ����
	pTalentEffectInfo->m_nEffectDirection =  GetEffectDirectionList((string)MStringToCharPointer(xeffect->����));

	return true;
}

// ����
bool TTalentEffectViewer::SetTalentEffectMissToProperty( XTalentEffectInfo * pTalentEffectInfo )
{
	SetTalentEffectAttackToProperty(pTalentEffectInfo);

	return true;
}

bool TTalentEffectViewer::SetPropertyToTalentEffectMiss( XTalentEffectInfo * pTalentEffectInfo )
{
	SetPropertyToTalentEffectAttack(pTalentEffectInfo);

	return true;
}

// ���
bool TTalentEffectViewer::SetTalentEffectShockToProperty( XTalentEffectInfo * pTalentEffectInfo )
{
	// �ŷ�Ʈ ����Ʈ ������ ������Ƽ���� ����
	SetTalentEffectAttackToProperty(pTalentEffectInfo);

	return true;
}

bool TTalentEffectViewer::SetPropertyToTalentEffectShock( XTalentEffectInfo * pTalentEffectInfo )
{
	SetPropertyToTalentEffectAttack(pTalentEffectInfo);

	return true;
}

// ����
bool TTalentEffectViewer::SetTalentEffectDurationToProperty( XTalentEffectInfo * pTalentEffectInfo )
{
	SetTalentEffectCastToProperty(pTalentEffectInfo);

	return true;
}

bool TTalentEffectViewer::SetPropertyToTalentEffectDuration( XTalentEffectInfo * pTalentEffectInfo )
{
	SetPropertyToTalentEffectCast(pTalentEffectInfo);

	return true;
}

// ���
bool TTalentEffectViewer::SetTalentEffectDefenseToProperty( XTalentEffectInfo * pTalentEffectInfo )
{
	SetTalentEffectAttackToProperty(pTalentEffectInfo);

	return true;
}

bool TTalentEffectViewer::SetPropertyToTalentEffectDefense( XTalentEffectInfo * pTalentEffectInfo )
{
	SetPropertyToTalentEffectAttack(pTalentEffectInfo);

	return true;
}

void TTalentEffectViewer::SetPropertyGridFromEffectInfo( XTalentEffectInfo* pEffectInfo )
{
	// ����Ʈ ����
	if(pEffectInfo == NULL)
	{
		m_eTalentEffectType = TALENT_EFFECT_TYPE_MAX;
		GlobalObjects::g_pMainForm->TalentEffectAttGrid->SelectedObject = nullptr;
		return;
	}

	int nEffectTypeIndex = 0;

	// ����Ʈ �������� Ÿ���� �����´�.
	m_eTalentEffectType = pEffectInfo->m_nEffectType;
	nEffectTypeIndex = (int)pEffectInfo->m_nEffectType;
	if(nEffectTypeIndex > 7)
	{
		GlobalObjects::g_pMainForm->TalentEffectAttGrid->SelectedObject = nullptr;
		return;
	}

	(this->*TalentEffectFuncToProperty[nEffectTypeIndex])(pEffectInfo);

}

void TTalentEffectViewer::SetHitEffectPosDirInfoFromPropertyGrid( XTalentHitEffectPosDir* pHitEffectPosDirInfo )
{
	XmlSerializable::TALENTEFFECT_HITEFFECT_POSDIR^ xeffect	= (XmlSerializable::TALENTEFFECT_HITEFFECT_POSDIR^)(GlobalObjects::g_pMainForm->TalentEffectAttGrid->SelectedObject);
	if(xeffect == nullptr)
		return;

	pHitEffectPosDirInfo->vHitEffectPos.x	= xeffect->X;
	pHitEffectPosDirInfo->vHitEffectPos.y	= xeffect->Y;
	pHitEffectPosDirInfo->vHitEffectPos.z	= xeffect->Z;

	return;
}

void TTalentEffectViewer::SetPropertyGridFromHitEffectPosDirInfo( XTalentHitEffectPosDir* pHitEffectPosDirInfo )
{
	// �ŷ�Ʈ ����Ʈ ������ ������Ƽ���� ����
	// TODO: pElement �� ��� �Ӽ����� �۶���Ƽ�� ǥ�����ش�.
	XmlSerializable::TALENTEFFECT_HITEFFECT_POSDIR^ xeffect = gcnew XmlSerializable::TALENTEFFECT_HITEFFECT_POSDIR;

	xeffect->Init_Dir(pHitEffectPosDirInfo->vSwordTrailHitUp.x, pHitEffectPosDirInfo->vSwordTrailHitUp.y, pHitEffectPosDirInfo->vSwordTrailHitUp.z);
	xeffect->X				= pHitEffectPosDirInfo->vHitEffectPos.x;
	xeffect->Y				= pHitEffectPosDirInfo->vHitEffectPos.y;
	xeffect->Z				= pHitEffectPosDirInfo->vHitEffectPos.z;

	//����Ÿ ���ε� ?
	GlobalObjects::g_pMainForm->TalentEffectAttGrid->SelectedObject = xeffect;
}

bool TTalentEffectViewer::SetTalentEffectCastEndToProperty( XTalentEffectInfo * pTalentEffectInfo )
{
	return SetTalentEffectCastToProperty(pTalentEffectInfo);
}

bool TTalentEffectViewer::SetPropertyToTalentEffectCastEnd( XTalentEffectInfo * pTalentEffectInfo )
{
	return SetPropertyToTalentEffectCast(pTalentEffectInfo);
}