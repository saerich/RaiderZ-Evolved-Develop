#pragma once

#include "XOverlayAnimationInfo.h"
#include "XAnimationBlendInfo.h"
#include "XAnimationHitSoundInfo.h"
#include "XReActionAnimationInfo.h"

class XAnimationInfoMgr
{
protected:
	tstring						m_strFileName;

	bool						m_bLoad;					// �ε��� �ѹ��̶� �õ��� �غó�?

public:
	XOverlayAnimationInfo		m_OverlayAnimationInfo;
	XAnimationBlendInfo			m_AnimationBlendInfo;
	XAnimationHitSoundInfo		m_AnimationHitSoundInfo;
	XReActionAnimationInfo		m_ReActionAnimationInfo;

public:
	XAnimationInfoMgr() : m_bLoad(false)			{}
	virtual ~XAnimationInfoMgr()					{}

	virtual	bool				LoadInfo(tstring& strMeshFileName);
};
