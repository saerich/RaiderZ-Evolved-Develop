#include "stdafx.h"
#include "VMeshDataValidator.h"


VMeshDataValidator::VMeshDataValidator()
:VDataValidator()
{
	m_nMeshCount = 0;
}

VMeshDataValidator::~VMeshDataValidator()
{

}

void VMeshDataValidator::Init( VValidatorInfoMgr* vInfoMgr )
{
	m_nMeshCount = vInfoMgr->m_pMeshInfoMgr->m_mapMeshInfo.size();
}

void VMeshDataValidator::CheckStart( VValidatorInfoMgr* vInfoMgr )
{
	for(map<string, VMeshInfo*>::iterator it = vInfoMgr->m_pMeshInfoMgr->m_mapMeshInfo.begin(); it != vInfoMgr->m_pMeshInfoMgr->m_mapMeshInfo.end(); ++it)
	{
		// animation.xml
		CheckAnimationXMl(it->second);

		// animationevent.xml
		CheckAnimationEvent(it->second, vInfoMgr);

		// animationinfo.xml
		CheckAnimationInfo(it->second);
	}
}

void VMeshDataValidator::CheckAnimationXMl( VMeshInfo* pMeshInfo )
{
	// �ִϸ��̼� ���� ���� üũ
	VAnimationList* pAnimationList = &(pMeshInfo->m_AnimationInfoMgr.m_AnimationList);
	for(vector<ANIMATION_INFO>::iterator it = pAnimationList->m_vecAnimationList.begin(); it != pAnimationList->m_vecAnimationList.end(); ++it)
	{
		if(ExistFile(it->strAnimationFileFullName.c_str()) == false)
		{
			string strErrorMsg = pMeshInfo->m_strMeshName + " : " + it->strAnimationName + " �� ������ �������� �ʽ��ϴ�.";
			m_AnimationListValidatorError.Add(EMT_FILE, strErrorMsg);
		}
	}
}

void VMeshDataValidator::CheckAnimationEvent( VMeshInfo* pMeshInfo, VValidatorInfoMgr* vInfoMgr )
{
	// �̺�Ʈ üũ
	VAnimationEventInfo* pAnimationEvent = &(pMeshInfo->m_AnimationInfoMgr.m_AnimationEventInfo);
	for(vector<ANIMATION_EVENT_INFO>::iterator it = pAnimationEvent->m_vecAnimationEvents.begin(); it != pAnimationEvent->m_vecAnimationEvents.end(); ++it)
	{
		for(vector<RAnimationEvent*>::iterator itEvent = it->vecAnimationEvent.begin(); itEvent != it->vecAnimationEvent.end(); ++itEvent)
		{
			string strErrorPreMsg = pMeshInfo->m_strMeshName + " : " + it->strAnimationName;

			RAnimationEvent* pEvent = *itEvent;
			if(pEvent->m_strEvent == "sh_custom")
			{
				if(vInfoMgr->m_pEffectInfoMgr->GetEffectInfo(pEvent->m_strParam1) == NULL)
				{
					string strError = strErrorPreMsg + " �� [sh_custom] �̺�Ʈ���� " + pEvent->m_strParam1 + " ����Ʈ�� effect.xml�� �����ϴ�.";
					m_AnimationEventValidatorError.Add(EMT_LINK, strError);
				}
			}
			else if(pEvent->m_strEvent == "sh_sound")
			{
				if(vInfoMgr->m_pSoundInfoMgr->GetSampleInfo(pEvent->m_strParam1) == NULL)
				{
					string strError = strErrorPreMsg + " �� [sh_sound] �̺�Ʈ���� " + pEvent->m_strParam1 + " ���尡 sound.xml�� �����ϴ�.";
					m_AnimationEventValidatorError.Add(EMT_LINK, strError);
				}
			}
			else if(pEvent->m_strEvent == "sh_effect")
			{
				if(vInfoMgr->m_pEffectInfoMgr->GetEffectFileInfo(pEvent->m_strParam1) == NULL)
				{
					string strError = strErrorPreMsg + " �� [sh_effect] �̺�Ʈ���� " + pEvent->m_strParam1 + ".effect.scene.xml ������ Data/SFX ������ �����ϴ�.";
					m_AnimationEventValidatorError.Add(EMT_FILE, strError);
				}
			}
			else if(pEvent->m_strEvent == "sh_camera")
			{
				if(vInfoMgr->m_pCameraInfoMgr->GetCameraFileInfo(pEvent->m_strParam1) == NULL)
				{
					string strError = strErrorPreMsg + " �� [sh_camera] �̺�Ʈ���� " + pEvent->m_strParam1 + ".elu.ani ������ Data/SFX/Camera ������ �����ϴ�.";
					m_AnimationEventValidatorError.Add(EMT_FILE, strError);
				}
			}
		}
	}
}

void VMeshDataValidator::CheckAnimationInfo( VMeshInfo* pMeshInfo )
{
	vector<BLEND_INFO_TOOL>* pvecAnimationBlendInfo = &(pMeshInfo->m_AnimationInfoMgr.m_vecAnimationBlendInfoForTool);
	VAnimationList* pAnimationList = &(pMeshInfo->m_AnimationInfoMgr.m_AnimationList);

	string strErrorPreMsg = pMeshInfo->m_strMeshName + " : ";
	for(vector<BLEND_INFO_TOOL>::iterator it = pvecAnimationBlendInfo->begin(); it != pvecAnimationBlendInfo->end(); ++it)
	{
		if(pAnimationList->GetAnimationInfo(it->m_strSource) == NULL)
		{
			string strError = strErrorPreMsg + "source�� " + it->m_strSource + " ��(��) animation.xml�� �����ϴ�.";
			m_AnimationInfoValidatorError.Add(EMT_LINK, strError);
		}

		for(vector<string>::iterator itTarget = it->m_vecTarget.begin(); itTarget != it->m_vecTarget.end(); ++itTarget)
		{
			if(pAnimationList->GetAnimationInfo(*itTarget) == NULL)
			{
				string strError = strErrorPreMsg + it->m_strSource + " ���� �������� target�� " + *itTarget + " ��(��) animation.xml�� �����ϴ�.";
				m_AnimationInfoValidatorError.Add(EMT_LINK, strError);
			}
		}
	}
}