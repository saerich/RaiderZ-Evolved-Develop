#ifndef XACTOR_H
#define XACTOR_H

#include "XObject.h"
#include "XDef.h"
#include "CSDef.h"

class XModuleCollision;
class XModuleEntity;
class XModuleMotion;
class XModuleAction;
class XModuleMovable;
class XModuleNetControl;
class XModuleTalent;
class XModuleEffect;
class XModuleBuff;
class XModuleUI;
class XModuleBParts;
class XModuleSound;
class XModuleBackgroundWork;

class XActor : public XObject
{
	MDeclareRTTI;
private:
	void					_RenderDebug();	

	bool					IsCurrentAnimationAlphaTestFadeEffect();
	void					CheckAlphaTestFadeEffect();

protected:
	UIID					m_nUIID;

	XModuleEntity*			m_pModuleEntity;
	XModuleMotion*			m_pModuleMotion;
	XModuleAction*			m_pModuleAction;
	XModuleMovable*			m_pModuleMovable;
	XModuleCollision*		m_pModuleCollision;
	XModuleTalent*			m_pModuleTalent;
	XModuleMessageQueue*	m_pModuleMessageQueue;
	XModuleEffect*			m_pModuleEffect;
	XModuleBuff*			m_pModuleBuff;
	XModuleUI*				m_pModuleUI;
	XModuleBackgroundWork*	m_pModuleBackgroundWork;

	bool					m_bDead;
	unsigned int			m_nDeadTime;
	float					m_fSTARegenTime;
	float					m_fSTARegenModValue; ///< ȸ����Ű�� ���� ������ �Ҽ��� 
	bool					m_bLoadingAsync;
	bool					m_bGuard;


	virtual void			RegisterModules();
	virtual void			UnregisterModules();
	virtual void			OnUpdate(float fDelta);
	virtual void			OnRender();
	virtual void			OnPostRender();

public:
	XActor(MUID uid, UIID nUIID);
	virtual ~XActor();

	void					OnDie();
	void					OnRebirth();
	bool					IsDead()					{ return m_bDead; }
	float					GetRadius();
	virtual float			GetValidInteractionDistance();
	virtual bool			IsColliable();
	virtual WEAPON_TYPE		GetCurrWeaponType() { return WEAPON_NONE;	}	// MyPlayer, NetPlayer, NonPlayer ���� �ۼ��ؾ��Ѵ�.
	virtual WEAPON_TYPE		GetEquipWeaponType(bool bLeft){ return WEAPON_NONE;	}	// MyPlayer, NetPlayer ���� �ۼ��ؾ��Ѵ�.

	virtual void			DeadProcEnterField();

	void					AddToSceneManager();
	bool					RemoveFromSceneManager();

	virtual XModuleEntity*			GetModuleEntity()		{ return m_pModuleEntity; }
	virtual XModuleMotion*			GetModuleMotion()		{ return m_pModuleMotion; }
	virtual XModuleAction*			GetModuleAction()		{ return m_pModuleAction; }
	virtual XModuleMovable*			GetModuleMovable()		{ return m_pModuleMovable; }
	virtual XModuleCollision*		GetModuleCollision()	{ return m_pModuleCollision; }
	virtual XModuleTalent*			GetModuleTalent()		{ return m_pModuleTalent; }
	virtual XModuleMessageQueue*	GetModuleMessageQueue()	{ return m_pModuleMessageQueue; }
	virtual XModuleEffect*			GetModuleEffect()		{ return m_pModuleEffect;	}
	virtual XModuleBuff*			GetModuleBuff()			{ return m_pModuleBuff;	}
	virtual XModuleBParts*			GetModuleBParts()		{ return NULL; }
	virtual XModuleSound *			GetModuleSound()		{ return NULL; }

	void						SetOpenHitGroupID(int nGroupID);

	virtual wstring				GetCurrentAnimationName();

	void							SetGuard(bool bGuard)			{ m_bGuard = bGuard;}
	bool							GetGuard()						{ return m_bGuard;	}

	const UIID				GetUIID() { return m_nUIID; }
};

// ������Ʈ�� �׾��ִ� �������� ��ȯ
bool IsActorDead(XObject* pObject);
// ������Ʈ�� �浹���� �������� ��ȯ
bool IsActorColliable(XObject* pObject);

#endif
