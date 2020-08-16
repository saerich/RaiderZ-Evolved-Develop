#include "StdAfx.h"
#include "NPCAnimationInfoSetting.h"

vector<SIMPLEANIMATION_INFO> g_vechfAnimationList;
vector<SIMPLEANIMATION_INFO> g_vechmAnimationList;

struct stBlendTemp
{
	string strSource;
	string strTarget;
};

struct stCopyNpcList
{
	string strNpcName;
	vector<SIMPLEANIMATION_INFO> vecCopyAniList;
};

System::Void Character_NET::NPCAnimationInfoSetting::NPCAnimationInfoSetting_Load( System::Object^ sender, System::EventArgs^ e )
{
	// npc
	MODELMAP ModelMap = g_pMainApp->m_TabModel.GetMap();

	int size = ModelMap[CT_NPC].m_Index.size();

	for( int j = 0; j < size; j++)
	{
		string subDir = PATH_NPC_TOOL + ModelMap[CT_NPC].m_Index[j] + "/" + ModelMap[CT_NPC].m_Index[j] + ".elu";

		if(ExistFile(subDir.c_str()))
		{
			String^ nodename = gcnew String(ModelMap[CT_NPC].m_Index[j].c_str());
			npc_listBox->Items->Add(nodename);
		}
	}

	// �̹� ���õ� NPC�� �ִٸ�...
	if(m_strSelectNPCName != nullptr)
	{
		int nIndex = npc_listBox->FindString(m_strSelectNPCName);
		if(nIndex != -1)
		{
			npc_listBox->SetSelected(nIndex, true);
		}
	}

	// �÷��̾� �ִϸ��̼�(.animation.xml) �ε�
	string strFilePath = PATH_PLAYER_FEMALE_TOOL + string("hf.elu.animation.xml");
	TAnimationHelper::GetAnimationInfoInXml(g_vechfAnimationList, strFilePath);
	
	strFilePath = PATH_PLAYER_MALE_TOOL + string("hm.elu.animation.xml");
	TAnimationHelper::GetAnimationInfoInXml(g_vechmAnimationList, strFilePath);
}

void Character_NET::NPCAnimationInfoSetting::SetSelectNpcName( String^ strName )
{
	m_strSelectNPCName = strName;
}

System::Void Character_NET::NPCAnimationInfoSetting::copy_button_Click( System::Object^ sender, System::EventArgs^ e )
{
	// ���� ����� ����Ʈ ����
	m_nCopyFullAniCount = 0;
	vector<stCopyNpcList> vecCopyNpcList;
	vector<string> vecResult;

	ListBox::SelectedObjectCollection^ selects = npc_listBox->SelectedItems;
	System::Collections::IEnumerator^ myEnum = selects->GetEnumerator();
	while(myEnum->MoveNext())
	{
		Object^ item = safe_cast<Object^>(myEnum->Current);

		stCopyNpcList npcList;
		npcList.strNpcName = MStringToCharPointer(item->ToString());

		m_nCopyFullAniCount += CollectionCopyNpcAnimationList(npcList.strNpcName, npcList.vecCopyAniList, vecResult);

		vecCopyNpcList.push_back(npcList);

		// ���� ����
		string strStartLog = npcList.strNpcName + " �� ���� �غ�";
		logOut_listBox->Items->Add(gcnew String(strStartLog.c_str()));
	}

	// 1�� ��� ����
	for(vector<string>::iterator itResult = vecResult.begin(); itResult != vecResult.end(); ++itResult)
	{
		logOut_listBox->Items->Add(gcnew String(itResult->c_str()));
	}


	//////////////////////////////////////////////////////////////////////////
	// �����մϴ�.
	m_fCurrentProgressBarState = 0.0f;
	m_copyProgressBar.Show();

	for(vector<stCopyNpcList>::iterator it = vecCopyNpcList.begin(); it != vecCopyNpcList.end(); ++it)
	{
		StartCopyAnimationInfo(it->strNpcName, it->vecCopyAniList);

		// 2�� ��� ����
		String^ strLog = String::Format("{0} �� �ִϸ��̼� {1}�� ���� �Ϸ�", gcnew String(it->strNpcName.c_str()), it->vecCopyAniList.size());
		logOut_listBox->Items->Add(strLog);
	}

	m_copyProgressBar.Hide();
}

int Character_NET::NPCAnimationInfoSetting::CollectionCopyNpcAnimationList(string strNpcName, vector<SIMPLEANIMATION_INFO>& vecCopyAniList, vector<string>& vecOutResult)
{	
	vecCopyAniList.clear();

	string strFilePath = PATH_NPC_TOOL + strNpcName + "/" + strNpcName + ".elu.animation.xml";
	
	vector<SIMPLEANIMATION_INFO> vecTempAniList;
	TAnimationHelper::GetAnimationInfoInXml(vecTempAniList, strFilePath);
	
	for(vector<SIMPLEANIMATION_INFO>::iterator it = vecTempAniList.begin(); it != vecTempAniList.end(); ++it)
	{
		if(it->strAnimationFileName.find("/player/hm/") != string::npos ||
			it->strAnimationFileName.find("/player/hf/") != string::npos)
		{
			vecCopyAniList.push_back(*it);
		}
		else
		{
			// ���� ���ϴ� �ִϸ��̼�
			string strOutput = it->strAnimationName + " �ִϸ��̼��� Player �ִϸ��̼��� �ƴϹǷ� ���� �� �� �����ϴ�.";
			vecOutResult.push_back(strOutput);
		}
	}

	return vecCopyAniList.size();
}

//void Character_NET::NPCAnimationInfoSetting::MoveDoNotCopyAnimationToSelectAnimation()
//{
//	int nSelectCount = animation_listBox->SelectedItems->Count;
//	if(nSelectCount == 0)
//		return;
//
//	vector<string> nTest;
//
//	ListBox::SelectedObjectCollection^ selects = animation_listBox->SelectedItems;
//	System::Collections::IEnumerator^ myEnum = selects->GetEnumerator();
//	while(myEnum->MoveNext())
//	{
//		Object^ item = safe_cast<Object^>(myEnum->Current);
//
//		doNotCopyAnimation_listBox->Items->Add(item->ToString());
//
//
//		nTest.push_back(MStringToCharPointer(item->ToString()));
//	}
//
//	for(vector<string>::iterator it = nTest.begin(); it != nTest.end(); ++it)
//	{
//		int nIndex = animation_listBox->FindString(gcnew String(it->c_str()));
//		animation_listBox->Items->RemoveAt(nIndex);
//	}
//}

void Character_NET::NPCAnimationInfoSetting::Init( TCharacter* phf, TCharacter* phm )
{
	m_phfPlayerModel = phf;
	m_phmPlayerModel = phm;
}

void Character_NET::NPCAnimationInfoSetting::StartCopyAnimationInfo( string strNPCModelName, vector<SIMPLEANIMATION_INFO>& vecCopyAnimationList )
{
	if(vecCopyAnimationList.empty())
		return;

	string strNPCAnimationFilePath = PATH_NPC_TOOL + strNPCModelName + "/" + strNPCModelName + ".elu";
	string strNPCAnimationEventFileName = strNPCAnimationFilePath + ".animationevent.xml";

	//int nCopyCount = animation_listBox->Items->Count;

	// �ִϸ��̼� ���� �ε�
	RMeshAnimationMgr	npcAnimationEventInfo;
	TAnimationInfoMgr	npcAnimationInfoMgr;
	
	LoadNpcAnimationInfo(npcAnimationEventInfo, npcAnimationInfoMgr, strNPCAnimationFilePath, strNPCAnimationEventFileName, vecCopyAnimationList);

	
	// ����
	CopyAnimationInfo(m_phmPlayerModel, g_vechmAnimationList, vecCopyAnimationList, npcAnimationEventInfo, npcAnimationInfoMgr);

	// ����
	CopyAnimationInfo(m_phfPlayerModel, g_vechfAnimationList, vecCopyAnimationList, npcAnimationEventInfo, npcAnimationInfoMgr);


	// �ִϸ��̼� ���� ����
	SaveNpcAnimationInfo(npcAnimationEventInfo, npcAnimationInfoMgr, strNPCAnimationEventFileName, vecCopyAnimationList);
}

bool Character_NET::NPCAnimationInfoSetting::LoadNpcAnimationInfo( RMeshAnimationMgr& npcAnimationEventInfo, TAnimationInfoMgr& npcAnimationInfoMgr, string& strNPCAnimationFilePath, string& strNPCAnimationEventFileName, vector<SIMPLEANIMATION_INFO>& vecCopyAnimationList )
{
	for(vector<SIMPLEANIMATION_INFO>::iterator it = vecCopyAnimationList.begin(); it != vecCopyAnimationList.end(); ++it)
	{
		RAnimation* pAni = new RAnimation(it->strAnimationName, NULL, 0);
		npcAnimationEventInfo.m_NodeHashList.push_back(pAni);
	}

	// �ִϸ��̼� �̺�Ʈ �ε�
	npcAnimationEventInfo.AddXmlDesc(strNPCAnimationEventFileName.c_str());


	// �ִϸ��̼� ��Ÿ ���� �ε�
	npcAnimationInfoMgr.LoadInfo(strNPCAnimationFilePath);

	return true;
}

void Character_NET::NPCAnimationInfoSetting::SaveNpcAnimationInfo( RMeshAnimationMgr& npcAnimationEventInfo, TAnimationInfoMgr& npcAnimationInfoMgr, string& strNPCAnimationEventFileName, vector<SIMPLEANIMATION_INFO>& vecCopyAnimationList )
{
	// �ִϸ��̼� �̺�Ʈ ����
	npcAnimationEventInfo.SaveAnimationEvent(strNPCAnimationEventFileName.c_str());


	// �ִϸ��̼� ��Ÿ ���� ����
	vector<string> vecAniList;
	for(vector<SIMPLEANIMATION_INFO>::iterator it = vecCopyAnimationList.begin(); it != vecCopyAnimationList.end(); ++it)
	{
		vecAniList.push_back(it->strAnimationName);
	}

	npcAnimationInfoMgr.SaveXml(vecAniList);
}

RAnimation* Character_NET::NPCAnimationInfoSetting::GetNpcAnimation( RAnimationHashList& npcAnimationList, string strAniName )
{
	for(RAnimationHashList_Iter it = npcAnimationList.begin(); it != npcAnimationList.end(); ++it)
	{
		if((*it)->GetAliasName() == strAniName)
			return *it;
	}

	return NULL;
}

void Character_NET::NPCAnimationInfoSetting::CopyAnimationInfo( TCharacter* pCopyModel, vector<SIMPLEANIMATION_INFO>& vecCopySource, vector<SIMPLEANIMATION_INFO>& vecTargetSource, RMeshAnimationMgr& npcAnimationEventInfo, TAnimationInfoMgr& npcAnimationInfoMgr )
{
	if(pCopyModel == NULL)
		return;

	for(vector<SIMPLEANIMATION_INFO>::iterator ithm = vecCopySource.begin(); ithm != vecCopySource.end(); ++ithm)
	{
		//if(ithm->strAnimationFileName == "ani/hm_1hs_atk_start.elu.ani")
		//{
		//	int a;
		//	a = 0;
		//}
		for(vector<SIMPLEANIMATION_INFO>::iterator it = vecTargetSource.begin(); it != vecTargetSource.end(); ++it)
		{
			if(it->strAnimationFileName.find(ithm->strAnimationFileName) != string::npos)
			{
				// ����
				CopyAnimationEvent(pCopyModel, npcAnimationEventInfo, ithm->strAnimationName, it->strAnimationName);

				// �ִϸ��̼� ���� ���� ����
				CopyAnimationBlendInfo(pCopyModel, npcAnimationInfoMgr, it->strAnimationName, vecTargetSource);

				// �ִϸ��̼� �������� ���� ����
				CopyAnimationOverlayInfo(pCopyModel, npcAnimationInfoMgr, ithm->strAnimationName, it->strAnimationName);

				// �ִϸ��̼� ���׼� ���� ����
				CopyAnimationReActionInfo(pCopyModel, npcAnimationInfoMgr, ithm->strAnimationName, it->strAnimationName);

				// �ִϸ��̼� ��Ʈ ���� ���� ����
				CopyAnimationHitSound(pCopyModel, npcAnimationInfoMgr, ithm->strAnimationName, it->strAnimationName);

				m_fCurrentProgressBarState += 0.5f;

				CalProgressBar();
			}
		}
	}
}

void Character_NET::NPCAnimationInfoSetting::CopyAnimationEvent( TCharacter* pCopyModel, RMeshAnimationMgr &npcAnimationEventInfo, string& strSourceAnimationName, string& strTargetAnimationName )
{
	RAnimation* pPlayerAni = pCopyModel->GetAnimation(strSourceAnimationName.c_str());
	RAnimation* pNpcAni = GetNpcAnimation(npcAnimationEventInfo.m_NodeHashList, strTargetAnimationName);

	if(pPlayerAni == NULL)
		return;

	if(pNpcAni == NULL)
	{
		pNpcAni = new RAnimation(strTargetAnimationName, NULL, 0);
		npcAnimationEventInfo.m_NodeHashList.push_back(pNpcAni);
	}

	// �̺�Ʈ ����
	pNpcAni->ClearEvent();
	for(vector<RAnimationEvent*>::iterator itEvent = pPlayerAni->m_animationEvents.begin(); itEvent != pPlayerAni->m_animationEvents.end(); ++itEvent)
	{
		if(DoNotCopyAnimationEvent(*itEvent))
			continue;

		RAnimationEvent* pAniEvent = new RAnimationEvent;
		pAniEvent->m_nFrame			= (*itEvent)->m_nFrame;
		pAniEvent->m_strEvent		= (*itEvent)->m_strEvent;
		pAniEvent->m_strParam1		= (*itEvent)->m_strParam1;
		pAniEvent->m_strParam2		= (*itEvent)->m_strParam2;
		pAniEvent->m_strParam3		= (*itEvent)->m_strParam3;
		pAniEvent->m_bUnconditional	= (*itEvent)->m_bUnconditional;

		// npc �˱� custom ����
		CopyAnimationSwordTrailEvent(*pAniEvent, strTargetAnimationName);

		pNpcAni->m_animationEvents.push_back(pAniEvent);
	}
}

void Character_NET::NPCAnimationInfoSetting::CopyAnimationBlendInfo( TCharacter* pCopyModel, TAnimationInfoMgr &npcAnimationInfoMgr, string& strTargetAnimationName, vector<SIMPLEANIMATION_INFO>& vecTargetSource )
{
	TAnimationBlendInfo::AnimationBlendData* pPlayerBlendData = pCopyModel->GetAnimationInfoMgr()->m_toolAnimationBlendInfo.GetAnimationBlendData();
	TAnimationBlendInfo::AnimationBlendData* pNpcBlendData = npcAnimationInfoMgr.m_toolAnimationBlendInfo.GetAnimationBlendData();

	// ����
	if(pNpcBlendData)
	{
		vector<stBlendTemp> vecDelTemp;
		for( TAnimationBlendInfo::AnimationBlendData::const_iterator itBlend = pNpcBlendData->begin(); itBlend != pNpcBlendData->end(); ++itBlend)
		{
			if(itBlend->first.first == strTargetAnimationName)
			{
				stBlendTemp stBlend;
				stBlend.strSource = itBlend->first.first;
				stBlend.strTarget = itBlend->first.second;
				vecDelTemp.push_back(stBlend);
			}
		}

		for(vector<stBlendTemp>::iterator itDel = vecDelTemp.begin(); itDel != vecDelTemp.end(); ++itDel)
		{
			npcAnimationInfoMgr.m_toolAnimationBlendInfo.DeleteAnimationTransType(itDel->strSource, itDel->strTarget);
		}
	}

	// ����
	if(pPlayerBlendData && pNpcBlendData)
	{
		for( TAnimationBlendInfo::AnimationBlendData::const_iterator itBlend = pPlayerBlendData->begin(); itBlend != pPlayerBlendData->end(); ++itBlend)
		{
			tstring strSource = itBlend->first.first;
			tstring strTarget = itBlend->first.second;

			for(vector<SIMPLEANIMATION_INFO>::iterator it = vecTargetSource.begin(); it != vecTargetSource.end(); ++it)
			{//NPC �����Ϳ� AnimationName�� ���� ��쿡�� �÷��̾��� ������ �����Ѵ�.  
				if(it->strAnimationName.compare(strTarget) == 0)
				{
					npcAnimationInfoMgr.m_toolAnimationBlendInfo.SetAnimationTransData(strSource, strTarget, itBlend->second.fDurationTime);
					break;
				}
			}
		}
	}
}

void Character_NET::NPCAnimationInfoSetting::CopyAnimationOverlayInfo( TCharacter* pCopyModel, TAnimationInfoMgr &npcAnimationInfoMgr, string& strSourceAnimationName, string& strTargetAnimationName )
{
	npcAnimationInfoMgr.m_toolOverlayAnimationInfo.EraseData(strTargetAnimationName);

	OVERLAY_ANIMATION_LIST * pOverlayAniList = pCopyModel->GetAnimationInfoMgr()->m_toolOverlayAnimationInfo.Get(strSourceAnimationName);

	if(pOverlayAniList == NULL)
		return;

	npcAnimationInfoMgr.m_toolOverlayAnimationInfo.Set(strTargetAnimationName, *pOverlayAniList);
}

void Character_NET::NPCAnimationInfoSetting::CopyAnimationReActionInfo( TCharacter* pCopyModel, TAnimationInfoMgr &npcAnimationInfoMgr, string& strSourceAnimationName, string& strTargetAnimationName )
{
	npcAnimationInfoMgr.m_toolReActionAnimationInfo.DelReActionAnimationInfo(strTargetAnimationName);

	ReAction_DATA * pReActionData = pCopyModel->GetAnimationInfoMgr()->m_toolReActionAnimationInfo.Get(strSourceAnimationName);

	if(pReActionData == NULL)
		return;

	npcAnimationInfoMgr.m_toolReActionAnimationInfo.Set(strTargetAnimationName, *pReActionData);
}

void Character_NET::NPCAnimationInfoSetting::CopyAnimationHitSound( TCharacter* pCopyModel, TAnimationInfoMgr &npcAnimationInfoMgr, string& strSourceAnimationName, string& strTargetAnimationName )
{
	tstring strHitSound = pCopyModel->GetAnimationInfoMgr()->m_AnimationHitSoundInfo.Get(strSourceAnimationName);

	if(strHitSound.empty() == true)
	{
		return;
	}

	npcAnimationInfoMgr.m_AnimationHitSoundInfo.Set(strTargetAnimationName, strHitSound);
}

void Character_NET::NPCAnimationInfoSetting::CopyAnimationSwordTrailEvent( RAnimationEvent& AniEvent, string& strTargetAnimationName )
{
	string strWeaponType;
	for (int i = 0; i < WEAPON_MAX; i++)
	{
		string strWeaponTypeTemp = CSStrings::WeaponType(WEAPON_TYPE(i));

		if(strTargetAnimationName.find(strWeaponTypeTemp) != string::npos)
		{
			strWeaponType = strWeaponTypeTemp;
			break;
		}
	}

	if(AniEvent.m_strEvent == "sh_sword_trail_start" && strWeaponType.empty() == false)
	{
		AniEvent.m_strParam1 = "dummy_" + strWeaponType + "_r";
	}
}

void Character_NET::NPCAnimationInfoSetting::CalProgressBar()
{
	float fValue = (m_fCurrentProgressBarState / (float)m_nCopyFullAniCount) * 100.0f;

	if(fValue > 100.0f)
		fValue = 100.0f;

	m_copyProgressBar.SetProGressBarValue((int)fValue);
}

bool Character_NET::NPCAnimationInfoSetting::DoNotCopyAnimationEvent( RAnimationEvent* pAniEvent )
{
	//http://dev/redmine/issues/17848 ���ؼ� �����մϴ�.
	//if(pAniEvent->m_strEvent == "sh_sound")
	//	return true;

	return false;
}
