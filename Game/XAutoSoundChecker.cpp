#include "stdafx.h"
#include "XAutoSoundChecker.h"
#include "XEffectInvoker.h"
#include "XTalentInfoMgr.h"

XAutoSoundChecker::XAutoSoundChecker()
{

}

XAutoSoundChecker::~XAutoSoundChecker()
{

}

void XAutoSoundChecker::HitSoundChecker( XObject* pObject, XTalentInfo* pTalentInfo, XObject* pDestObject /*= NULL*/ )
{
	vector<XTalentEffectInfo *> vecEffectInfoByType;
	GetTalentEffectInfo(pTalentInfo, vecEffectInfoByType);

	//////////////////////////////////////////////////////////////////////////
	// ���� ���� ��������
	vector<CHECK_EFFECT_DATA> vecCheckEffectData;
	for(vector<XTalentEffectInfo*>::iterator it = vecEffectInfoByType.begin(); it != vecEffectInfoByType.end(); it++)
	{
		if((*it)->m_nEffectType == ATTACK_EFFECT ||
			(*it)->m_nEffectType == ATTACK_SECONDARY_EFFECT)
		{
			XEffectInvoker soundCheckInvoker;
			soundCheckInvoker.GetEffect_Data(CET_SOUND, pObject, pDestObject, (*it)->m_strEffectName, (*it)->m_strBoneName, (*it)->m_strParameter, &vecCheckEffectData);
		}
	}

	// ���� ���� üũ
	bool bUseSound = false;
	if(vecCheckEffectData.size() == 1)
	{
		if(vecCheckEffectData[0].strEffectName == L"ss")
		{
			bUseSound = true;
		}
	}
	else if(vecCheckEffectData.size() > 1)
	{
		bUseSound = true;
	}

	if(bUseSound == false)
	{
		// �ƹ��͵� ���ٸ�... ����Ʈ �ǽ�....
		XEffectInvoker soundInvoker;
		soundInvoker.Invoke(pObject, wstring(L"ss"), wstring(), wstring(), NULL, pDestObject);
	}
}

void XAutoSoundChecker::GetTalentEffectInfo( XTalentInfo* pTalentInfo, vector<XTalentEffectInfo *>& vecEffectInfoByType )
{
	XTalentEffectDataMgr* pTalentEffectDataMgr = pTalentInfo->GetEffectDataMgr();

	// nType���� ����ƮInfo�� �����´�.
	if(pTalentEffectDataMgr && pTalentEffectDataMgr->IsEmpty() == false)
	{
		pTalentEffectDataMgr->Get(ATTACK_EFFECT, vecEffectInfoByType);
	}
}