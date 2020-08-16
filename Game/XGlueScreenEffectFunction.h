#pragma once

#include "XGlueFunction.h"
#include "XScreenEffectManager.h"

/// lua���� ����ϴ� ��ũ������Ʈ API
class XGlueScreenEffectFunction : public XGlueFunction
{
public:
	virtual void DeclGlueFunc(MWLua* pLua);

	void					Add(const char* strListFileName, XSCREENEFFECT_CHARACTER_ATT charAtt, AnimationLoopType aniLoop);		// ������ ��ũ�� ����Ʈ �����ֱ�
	void					SetPos(float x, float y, float z, XSCREENEFFECT_LOCATION_TYPE scrBase);									// ������ ��ũ�� ����Ʈ ��ġ
	void					Register(const char* strScreenEffectName, 
									const char* strListFileName, 
									XSCREENEFFECT_CHARACTER_ATT charAtt,
									AnimationLoopType aniLoop);																		// ��ũ�� ����Ʈ ���
	void					SetRegPos(const char* strScreenEffectName, 
									float x, float y, float z, 
									XSCREENEFFECT_LOCATION_TYPE scrBase);															// ��ũ�� ����Ʈ ��ġ
	void					SetRegStr(const char* strScreenEffectName, const char* strContents);									// ��ũ�� ����Ʈ ���ڿ�
	void					SetChgAni(const char* strScreenEffectName, const char* strAnimation, XSCREENEFFECT_ANIMATION_END_ATT aniDel);	// ��ũ�� ����Ʈ �ִϸ��̼� ����
	void					Draw(const char* strScreenEffectName);																	// ��ũ�� ����Ʈ �׸���
	void					AniStop(const char* strScreenEffectName);
};
