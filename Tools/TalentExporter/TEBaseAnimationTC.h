#pragma once

#include "XBaseAnimationTC.h"

class TEBaseAnimationTC : public XBaseAnimationTC
{
public:
	TEBaseAnimationTC(TECharacter* pCharacter);
	virtual ~TEBaseAnimationTC(void);

	void			InitCurrentAnimation();

	virtual void	StopAnimation();

	// �ִϸ��̼� �̺�Ʈ
	void			ClearAnimationEvent();
	void			AddAnimationEvent(ANIMATION_EVENT& ani_event);
	void			ResetAnimationEventByFrame(int nFrame);

	RPLAYSTATE		GetState();

	bool			CheckExistNodeController(string& strNodeName);

	virtual RActorNode*					GetAnimatedActorNodeTransform(tstring& strNodeName, RMatrix& _rOut, E_TRANSFORMSPACE eTransformSpace);

};