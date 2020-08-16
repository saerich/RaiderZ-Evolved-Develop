#pragma once

#include "MXml.h"
#include "MTstring.h"

class XAnimationBlendInfo;
class XOverlayAnimationInfo;
class XAnimationHitSoundInfo;
class XReActionAnimationInfo;

#define ANIMATION_INFO_EXT							_T(".animationInfo.xml")

#define ANIMATION_INFO_XML_TAG_MAIET				"maiet"
#define ANIMATION_INFO_XML_TAG_ANIMATION			"Animation"
#define ANIMATION_INFO_XML_ATTR_ANIMATION_NAME		"name"

#define ANIMATION_BLEND_INFO_XML_TAG_TRANSITION		"AnimationTransition"
#define ANIMATION_BLEND_INFO_XML_ATTR_TARGET		"target"
#define ANIMATION_BLEND_INFO_XML_ATTR_DURATION		"duration"

#define ANIMATION_OVERLAY_XML_TAG_OVAERLAYANIMATION "OverlayAnimation"
#define ANIMATION_OVERLAY_XML_ATTR_NAME				"name"
#define ANIMATION_OVERLAY_XML_ATTR_BONE_NAM			"bonename"

#define ANIMATION_HIT_SOUND_XML_TAG_HITSOUND		"AnimationHitSound"
#define ANIMATION_HIT_SOUND_XML_ATTR_SOUND_NAME		"soundname"

#define ANIMATION_REACTION_XML_TAG_REACTIONANIMATION "ReActionAnimation"
#define ANIMATION_REACTION_XML_ATTR_REACTION_NAME	"reactionname"
#define ANIMATION_REACTION_XML_ATTR_WEIGHT			"weight"
#define ANIMATION_REACTION_XML_ATTR_SPEED			"speed"
#define ANIMATION_REACTION_XML_WEAPON_BONE_NAME		"weaponbonename"
#define ANIMATION_REACTION_XML_ATTR_DISABLE			"disable"


class XAnimationInfoLoader
{
private:
	void ParseBlendInfo(MXmlElement* pElement, MXml* pXml, tstring&  strAniName, XAnimationBlendInfo* pBlendInfo);
	void ParseOverlayInfo(MXmlElement* pElement, MXml* pXml, tstring&  strAniName, XOverlayAnimationInfo* pOverlayInfo);
	void ParseHitSoundInfo(MXmlElement* pElement, MXml* pXml, tstring&  strAniName, XAnimationHitSoundInfo* pHitSoundInfo);
	void ParseReActionInfo(MXmlElement* pElement, MXml* pXml, tstring&  strAniName, XReActionAnimationInfo* pReActionInfo);

public:
	XAnimationInfoLoader();
	virtual ~XAnimationInfoLoader();

	/**
	@brief ��ȯ ���̺��� XML�����ͷ� �ҷ��´�.
	@param animationMgr �ʱ�ȭ �� �ִϸ��̼� �Ŵ��� Ŭ����.
	@param parentXmlElement XML ���� ��� ��.
	@param szFilter ���� ��Ʈ��
	*/
	bool LoadXml(tstring strFileName, XAnimationBlendInfo* pBlendInfo, XOverlayAnimationInfo* pOverlayInfo, XAnimationHitSoundInfo* pHitSoundInfo, XReActionAnimationInfo* pReActionInfo);
};
