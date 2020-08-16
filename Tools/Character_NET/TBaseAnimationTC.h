#pragma once

#include "XBaseAnimationTC.h"

class TBaseAnimationTC : public XBaseAnimationTC
{
public:
	TBaseAnimationTC(TCharacter* pCharacter);
	virtual ~TBaseAnimationTC(void);

	void			InitCurrentAnimation();

	virtual void	StopAnimation();

	// �ִϸ��̼� �̺�Ʈ
	void			ClearAnimationEvent();
	void			AddAnimationEvent(ANIMATION_EVENT& ani_event);
	void			ResetAnimationEventByFrame(int nFrame);

	RPLAYSTATE		GetState();

	bool			CheckExistNodeController(string& strNodeName);

	virtual RActorNode*					GetAnimatedActorNodeTransform(string& strNodeName, RMatrix& _rOut, E_TRANSFORMSPACE eTransformSpace);
};