#include "stdafx.h"
#include "MXml.h"
#include "XAnimationInfoMgr.h"
#include "XAnimationInfoLoader.h"

bool XAnimationInfoMgr::LoadInfo( tstring& strMeshFileName )
{	
	// �ε��� �Ǿ� �ֳ�?
	if(m_bLoad)
		return true;

	m_bLoad = true;
	m_strFileName = strMeshFileName + ANIMATION_INFO_EXT;
	XAnimationInfoLoader infoLoader;
	return infoLoader.LoadXml(m_strFileName, &m_AnimationBlendInfo, &m_OverlayAnimationInfo, &m_AnimationHitSoundInfo, &m_ReActionAnimationInfo);
}
