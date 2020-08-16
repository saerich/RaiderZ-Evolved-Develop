#include "stdafx.h"
#include "VTalentDataValidator.h"
#include "XTalentInfo.h"

map<TALENT_EVENT, CHECKTALENTEVENTHANDLER> VTalentDataValidator::m_CheckTalentEventHandlers;

VTalentDataValidator::VTalentDataValidator()
:VDataValidator()
{
	m_CheckTalentEventHandlers.insert( map<TALENT_EVENT, CHECKTALENTEVENTHANDLER>::value_type( TE_NONE,					OnCheckTalentEvent_None) );
	m_CheckTalentEventHandlers.insert( map<TALENT_EVENT, CHECKTALENTEVENTHANDLER>::value_type( TE_FIRE_EFFECT,			OnCheckTalentEvent_FireEffect) );
	m_CheckTalentEventHandlers.insert( map<TALENT_EVENT, CHECKTALENTEVENTHANDLER>::value_type( TE_LAUNCH_PROJECTILE,	OnCheckTalentEvent_Projectile) );
	m_CheckTalentEventHandlers.insert( map<TALENT_EVENT, CHECKTALENTEVENTHANDLER>::value_type( TE_EFFECT,				OnCheckTalentEvent_Effect) );
	m_CheckTalentEventHandlers.insert( map<TALENT_EVENT, CHECKTALENTEVENTHANDLER>::value_type( TE_SOUND,				OnCheckTalentEvent_Sound) );
	m_CheckTalentEventHandlers.insert( map<TALENT_EVENT, CHECKTALENTEVENTHANDLER>::value_type( TE_CAMERA,				OnCheckTalentEvent_Camera) );
	m_CheckTalentEventHandlers.insert( map<TALENT_EVENT, CHECKTALENTEVENTHANDLER>::value_type( TE_CUSTOM,				OnCheckTalentEvent_Custom) );
	m_CheckTalentEventHandlers.insert( map<TALENT_EVENT, CHECKTALENTEVENTHANDLER>::value_type( TE_DELAYED_ACT,			OnCheckTalentEvent_Delayed_Act) );
	m_CheckTalentEventHandlers.insert( map<TALENT_EVENT, CHECKTALENTEVENTHANDLER>::value_type( TE_EXTRAACTIVE_1,		OnCheckTalentEvent_ExtraActive_1) );
	m_CheckTalentEventHandlers.insert( map<TALENT_EVENT, CHECKTALENTEVENTHANDLER>::value_type( TE_EXTRAACTIVE_2,		OnCheckTalentEvent_ExtraActive_2) );
}

VTalentDataValidator::~VTalentDataValidator()
{

}

void VTalentDataValidator::Init( VValidatorInfoMgr* vInfoMgr )
{
	m_nTalentCount = vInfoMgr->m_pTalentInfoMgr->m_TalentInfoMgr.size();
}

void VTalentDataValidator::CheckStart( VValidatorInfoMgr* vInfoMgr )
{
	VTalentInfoMgr* pTalentInfoMgr = vInfoMgr->m_pTalentInfoMgr;
	for(map<int , CSTalentInfo* >::iterator it = pTalentInfoMgr->m_TalentInfoMgr.begin(); it != pTalentInfoMgr->m_TalentInfoMgr.end(); ++it)
	{
		XTalentInfo* pTalentInfo = pTalentInfoMgr->m_TalentInfoMgr.Get(it->second->m_nID);


		CheckTalentInfoXml();

		CheckTalentEvent(&pTalentInfoMgr->m_TalentInfoMgr, pTalentInfo, vInfoMgr);

		CheckTalentHitInfo(&pTalentInfoMgr->m_TalentInfoMgr, pTalentInfo, vInfoMgr);
	}
}

void VTalentDataValidator::CheckTalentInfoXml()
{

}

void VTalentDataValidator::CheckTalentEvent( XTalentInfoMgr* pTalentInfoMgr, XTalentInfo* pTalentInfo, VValidatorInfoMgr* vInfoMgr )
{
	string strCheckTalentError = (string)pTalentInfo->GetName() + "(" + TransToStringFromNumber(pTalentInfo->m_nID) + ") : ";

	if(pTalentInfo->m_Events.m_nRefID > 0)
	{
		pTalentInfo = pTalentInfoMgr->Get(pTalentInfo->m_Events.m_nRefID);
		if(pTalentInfo == NULL)
		{
			string strError = strCheckTalentError + "���۷��� (" + TransToStringFromNumber(pTalentInfo->m_Events.m_nRefID) + ") ID�� �������� �ʴ� �ŷ�Ʈ ID�Դϴ�.";
			m_TalentEventValidatorError.Add(EMT_LINK, strError);
			return;
		}
	}

	for(vector<CSTalentEventInfo>::iterator itNormal = pTalentInfo->m_Events.m_vecEvents.begin(); itNormal != pTalentInfo->m_Events.m_vecEvents.end(); ++itNormal)
	{
		CheckEventGo(&(*itNormal), vInfoMgr, &strCheckTalentError);
	}
	
	for(vector<CSTalentEventInfo>::iterator itAct = pTalentInfo->m_Events.m_vecActEvents.begin(); itAct != pTalentInfo->m_Events.m_vecActEvents.end(); ++itAct)
	{
		CheckEventGo(&(*itAct), vInfoMgr, &strCheckTalentError);
	}
}

void VTalentDataValidator::CheckEventGo( CSTalentEventInfo * pEvent, VValidatorInfoMgr* vInfoMg, string* pStrErrorPre )
{
	map<TALENT_EVENT, CHECKTALENTEVENTHANDLER>::iterator itFind = m_CheckTalentEventHandlers.find(pEvent->m_nEvent);
	if(itFind != m_CheckTalentEventHandlers.end())
	{
		itFind->second(*this, pEvent, vInfoMg, pStrErrorPre);
	}
}

void VTalentDataValidator::OnCheckTalentEvent_None( VTalentDataValidator& talentEventValidator, CSTalentEventInfo * pEvent, VValidatorInfoMgr * vInfoMgr, string* pStrErrorPre )
{

}

void VTalentDataValidator::OnCheckTalentEvent_FireEffect( VTalentDataValidator& talentEventValidator, CSTalentEventInfo * pEvent, VValidatorInfoMgr * vInfoMgr, string* pStrErrorPre )
{
	if(pEvent->m_strParam1.empty())
	{
		string strError = *pStrErrorPre + "[�߻�] �̺�Ʈ���� ����Ʈ �̸��� �����ϴ�.";
		talentEventValidator.m_TalentEventValidatorError.Add(EMT_NAME, strError);
	}
	else if(vInfoMgr->m_pEffectInfoMgr->GetEffectInfo(pEvent->m_strParam1) == NULL)
	{
		string strError = *pStrErrorPre + "[�߻�] �̺�Ʈ���� " + pEvent->m_strParam1 + " ����Ʈ�� effect.xml�� �����ϴ�.";
		talentEventValidator.m_TalentEventValidatorError.Add(EMT_LINK, strError);
	}
}

void VTalentDataValidator::OnCheckTalentEvent_Projectile( VTalentDataValidator& talentEventValidator, CSTalentEventInfo * pEvent, VValidatorInfoMgr * vInfoMgr, string* pStrErrorPre )
{
	if(pEvent->m_ProjectileInfo.m_strEffectName.empty() == false &&
		vInfoMgr->m_pEffectInfoMgr->GetEffectInfo(pEvent->m_ProjectileInfo.m_strEffectName) == NULL)
	{
		string strError = *pStrErrorPre + "[�߻�ü] �̺�Ʈ���� [�߻�ü] ����Ʈ " + pEvent->m_ProjectileInfo.m_strEffectName + " ����Ʈ�� effect.xml�� �����ϴ�.";
		talentEventValidator.m_TalentEventValidatorError.Add(EMT_LINK, strError);
	}

	if(pEvent->m_ProjectileInfo.m_strFireEffectName.empty() == false &&
		vInfoMgr->m_pEffectInfoMgr->GetEffectInfo(pEvent->m_ProjectileInfo.m_strFireEffectName) == NULL)
	{
		string strError = *pStrErrorPre + "[�߻�ü] �̺�Ʈ���� [�߻�] ����Ʈ " + pEvent->m_ProjectileInfo.m_strFireEffectName + " ����Ʈ�� effect.xml�� �����ϴ�.";
		talentEventValidator.m_TalentEventValidatorError.Add(EMT_LINK, strError);
	}

	if(pEvent->m_ProjectileInfo.m_strAttackEffectName.empty() == false &&
		vInfoMgr->m_pEffectInfoMgr->GetEffectInfo(pEvent->m_ProjectileInfo.m_strAttackEffectName) == NULL)
	{
		string strError = *pStrErrorPre + "[�߻�ü] �̺�Ʈ���� [�ǰ�] ����Ʈ " + pEvent->m_ProjectileInfo.m_strAttackEffectName + " ����Ʈ�� effect.xml�� �����ϴ�.";
		talentEventValidator.m_TalentEventValidatorError.Add(EMT_LINK, strError);
	}

	if(pEvent->m_ProjectileInfo.m_strDefenceEffectName.empty() == false &&
		vInfoMgr->m_pEffectInfoMgr->GetEffectInfo(pEvent->m_ProjectileInfo.m_strDefenceEffectName) == NULL)
	{
		string strError = *pStrErrorPre + "[�߻�ü] �̺�Ʈ���� [���] ����Ʈ " + pEvent->m_ProjectileInfo.m_strDefenceEffectName + " ����Ʈ�� effect.xml�� �����ϴ�.";
		talentEventValidator.m_TalentEventValidatorError.Add(EMT_LINK, strError);
	}

	if(pEvent->m_ProjectileInfo.m_strHitGroundEffectName.empty() == false &&
		vInfoMgr->m_pEffectInfoMgr->GetEffectInfo(pEvent->m_ProjectileInfo.m_strHitGroundEffectName) == NULL)
	{
		string strError = *pStrErrorPre + "[�߻�ü] �̺�Ʈ���� [�����ǰ�] ����Ʈ " + pEvent->m_ProjectileInfo.m_strHitGroundEffectName + " ����Ʈ�� effect.xml�� �����ϴ�.";
		talentEventValidator.m_TalentEventValidatorError.Add(EMT_LINK, strError);
	}
}

void VTalentDataValidator::OnCheckTalentEvent_Effect( VTalentDataValidator& talentEventValidator, CSTalentEventInfo * pEvent, VValidatorInfoMgr * vInfoMgr, string* pStrErrorPre )
{
	if(pEvent->m_strParam1.empty())
	{
		string strError = *pStrErrorPre + "[����Ʈ] �̺�Ʈ���� ����Ʈ �̸��� �����ϴ�.";
		talentEventValidator.m_TalentEventValidatorError.Add(EMT_NAME, strError);
	}
	else if(vInfoMgr->m_pEffectInfoMgr->GetEffectFileInfo(pEvent->m_strParam1) == NULL)
	{
		string strError = *pStrErrorPre + "[����Ʈ] �̺�Ʈ���� " + pEvent->m_strParam1 + ".effect.scene.xml ������ �����ϴ�.";
		talentEventValidator.m_TalentEventValidatorError.Add(EMT_FILE, strError);
	}
}

void VTalentDataValidator::OnCheckTalentEvent_Sound( VTalentDataValidator& talentEventValidator, CSTalentEventInfo * pEvent, VValidatorInfoMgr * vInfoMgr, string* pStrErrorPre )
{
	if(pEvent->m_strParam1.empty())
	{
		string strError = *pStrErrorPre + "[����] �̺�Ʈ���� ���� �̸��� �����ϴ�.";
		talentEventValidator.m_TalentEventValidatorError.Add(EMT_NAME, strError);
	}
	else if(vInfoMgr->m_pSoundInfoMgr->GetSampleInfo(pEvent->m_strParam1) == NULL)
	{
		string strError = *pStrErrorPre + "[����] �̺�Ʈ���� " + pEvent->m_strParam1 + " ���尡 sound.xml�� �����ϴ�.";
		talentEventValidator.m_TalentEventValidatorError.Add(EMT_LINK, strError);
	}
}

void VTalentDataValidator::OnCheckTalentEvent_Camera( VTalentDataValidator& talentEventValidator, CSTalentEventInfo * pEvent, VValidatorInfoMgr * vInfoMgr, string* pStrErrorPre )
{
	if(pEvent->m_strParam1.empty())
	{
		string strError = *pStrErrorPre + "[ī�޶�] �̺�Ʈ���� ī�޶� �̸��� �����ϴ�.";
		talentEventValidator.m_TalentEventValidatorError.Add(EMT_NAME, strError);
	}
	else if(vInfoMgr->m_pCameraInfoMgr->GetCameraFileInfo(pEvent->m_strParam1) == NULL)
	{
		string strError = *pStrErrorPre + "[ī�޶�] �̺�Ʈ���� " + pEvent->m_strParam1 + ".elu ������ �����ϴ�.";
		talentEventValidator.m_TalentEventValidatorError.Add(EMT_FILE, strError);
	}
}

void VTalentDataValidator::OnCheckTalentEvent_Custom( VTalentDataValidator& talentEventValidator, CSTalentEventInfo * pEvent, VValidatorInfoMgr * vInfoMgr, string* pStrErrorPre )
{
	if(pEvent->m_strCustomEvent.empty())
	{
		string strError = *pStrErrorPre + "[sh_custom] �̺�Ʈ���� ����Ʈ �̸��� �����ϴ�.";
		talentEventValidator.m_TalentEventValidatorError.Add(EMT_NAME, strError);
	}
	else if(vInfoMgr->m_pEffectInfoMgr->GetEffectInfo(pEvent->m_strCustomEvent) == NULL)
	{
		string strError = *pStrErrorPre + "[sh_custom] �̺�Ʈ���� " + pEvent->m_strCustomEvent + " ����Ʈ�� effect.xml�� �����ϴ�.";
		talentEventValidator.m_TalentEventValidatorError.Add(EMT_LINK, strError);
	}
}

void VTalentDataValidator::OnCheckTalentEvent_Delayed_Act( VTalentDataValidator& talentEventValidator, CSTalentEventInfo * pEvent, VValidatorInfoMgr * vInfoMgr, string* pStrErrorPre )
{
	if(pEvent->m_fTime1 <= 0.0f)
	{
		string strError = *pStrErrorPre + "[�ð���] �̺�Ʈ���� �ð� ������ 0 �Ǵ� 0���ϰ��Դϴ�.";
		talentEventValidator.m_TalentEventValidatorError.Add(EMT_NAME, strError);
	}
}

void VTalentDataValidator::OnCheckTalentEvent_ExtraActive_1( VTalentDataValidator& talentEventValidator, CSTalentEventInfo * pEvent, VValidatorInfoMgr * vInfoMgr, string* pStrErrorPre )
{

}

void VTalentDataValidator::OnCheckTalentEvent_ExtraActive_2( VTalentDataValidator& talentEventValidator, CSTalentEventInfo * pEvent, VValidatorInfoMgr * vInfoMgr, string* pStrErrorPre )
{

}

void VTalentDataValidator::CheckTalentHitInfo( XTalentInfoMgr* pTalentInfoMgr, XTalentInfo* pTalentInfo, VValidatorInfoMgr* vInfoMgr )
{
	string strCheckTalentError = (string)pTalentInfo->GetName() + "(" + TransToStringFromNumber(pTalentInfo->m_nID) + ") �ŷ�Ʈ ";

	if( pTalentInfo->m_HitInfo.m_nReferenceID > 0)
	{
		pTalentInfo = pTalentInfoMgr->Get(pTalentInfo->m_HitInfo.m_nReferenceID);
		if(pTalentInfo == NULL)
		{
			string strError = strCheckTalentError + "���۷��� (" + TransToStringFromNumber(pTalentInfo->m_HitInfo.m_nReferenceID) + ") ID�� �������� �ʴ� �ŷ�Ʈ ID�Դϴ�.";
			m_TalentHitInfoValidatorError.Add(EMT_LINK, strError);
			return;
		}
	}	
}