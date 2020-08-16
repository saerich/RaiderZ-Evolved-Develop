#pragma once

#include "RTypes.h"
#include <list>

namespace rs3 {

class RActor;
class RAnimationEvent;
class REffectSceneNode;
class RListener;

enum REVENT
{
	RE_NONE = 0,
	RE_SHADOW_CHANGE,
	RE_RENDERING_CHANGE,
	RE_ENVIRONMENT_CHANGE,
	RE_ENVIRONMENT_BLENDING,
	RE_DEVICE_LOST,
	RE_DEVICE_RESTORE,

	RE_END
};

//////////////////////////////////////////////////////////////////////////
// �̺�Ʈ �߻� ��ü
class RS_API REventBroadcaster
{
	std::list<RListener*>	m_listeners;
public:
	virtual ~REventBroadcaster();

	void	AddListener(RListener* pListener);
	void	RemoveListener(RListener* pListener);

	void	Notify();

};

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  �̺�Ʈ ���� ��ü
//
class RS_API RListener
{
public:

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� / �Ҹ�
	//
	RListener(void)
	{
	}
	virtual ~RListener(void)
	{
	}

	virtual void					Update() {}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// �ִϸ��̼� ����
	//
	virtual void					OnAnimationEvent(RActor* pActor, const RAnimationEvent* pEvent) { }
};

}
