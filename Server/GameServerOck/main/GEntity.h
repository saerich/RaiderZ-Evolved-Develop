#ifndef _GENTITY_H
#define _GENTITY_H

#include "GModule.h"
#include "GDef.h"
#include "MReferenceCounted.h"
#include "CSChrInfo.h"

class GField;
class GEntityMgr;
class GEntityActor;
class GFieldInfo;

/// ��ƼƼ�� �ֻ��� Ŭ����
/// ��� ��ü�� �̰��� ����ؾ��Ѵ�.
class GEntity : public MReferenceCounted
{
	friend class GEntityMgr;

public:
	GEntity();
	virtual ~GEntity();
	virtual bool Create(MUID& uid);
	virtual void Destroy();
	void Update(float fDelta);	
	void Message(GMessage& msg);

	// ��⺰ ������Ʈ
	virtual void UpdateCombat(float fDelta) {}
	virtual void UpdateAI(float fDelta)		{}
	virtual void UpdateBuff(float fDelta)	{}
	virtual void UpdateMovable(float fDelta)		{}

	// EntityMgr�� ���� delete ���ּ���..��� ��..
	void DeleteMe()			{ m_bDeleteMe = true; }
	bool IsDeleteMe()		{ return m_bDeleteMe; }

	// �ʵ� ����
	void EnterToField(GField* pField, const vec3& pos, const vec3& dir=vec3(0.0f, 1.0f, 0.0f));		
	// �ʵ忡�� ������
	void LeaveFromField();		

	// do �Լ� �ø��� ���� �ʿ�. - bird
	virtual void doDie();		// TODO: actor�� ����
	virtual void OnRebirth();	// TODO: actor�� ����
	virtual bool Damage(MUID& uidAttacker, int nDamage) { return true; } 	// TODO: actor�� ����
	virtual void doAttack(MUID uidTarget, int nDamage) {}						// TODO: actor�� ����
	bool		 IsBack(const vec3& vecTargetPos);	// TODO: actor�� ����
	bool				IsAILodEnableFiled() const; // TODO: npc�� ����
	
	// setter
	virtual bool		SetPos(const vec3& pos);
	void				SetDir(const vec3& dir);
	void				SetFacingDir(const vec3& dir);
	const MUID			GetUID(void) const	{ return m_UID; }
	MUID				GetFieldUID() const;
	const ENTITY_TYPEID	GetTypeID() const	{ return m_nTypeID; }
	
	// getter
	bool				IsType(uint32 mask) const;
	vec3				GetPos() const		{ return m_vPos; }
	vec3				GetDir() const		{ return m_vDir; }
	virtual vec3		GetFacingDir() const{ return m_vFacingDir; }
	GField*				GetField() const	{ return m_pField; }
	const GFieldInfo*	GetFieldInfo() const;
	int					GetFieldID() const;
	bool				IsInField() const	{ return (m_pField != NULL); }
	bool				IsDead() const		{ return m_bDead; }
	bool				IsNPC()  const;
	bool				IsPlayer()  const;
	bool				IsActor()  const;
	bool				IsBuffEntity() const;
	bool				IsBPart() const;

	// util
	float				DistanceTo(GEntity* pEntity);

	void SetActivateUpdate(bool isActivate);
	bool				IsActivateUpdate() const;

protected:
	virtual void OnUpdate(float fDelta);
	virtual void OnMessage(GMessage& msg);
	virtual void OnEnter()		{}
	virtual void OnLeave()		{}
	virtual void OnDestroy()	{}
	virtual void OnEnterToSharedField()		{}
	virtual void OnEnterToDynamicField()	{}
	virtual void RegisterModules()			{}						///< ��� ���
	virtual void UnregisterModlues()		{}						///< ��� ����
	bool				IsActiveModule(int nID);
	GModule*			GetModule(int nID);
	

private:
	

private:
protected:
	GModuleContainer	m_Modules;
	MUID				m_UID;
	uint32				m_nType;
	ENTITY_TYPEID		m_nTypeID;
	vec3				m_vPos;
	vec3				m_vDir;
	vec3				m_vFacingDir;		// ���� �ٶ󺸴� ����
	bool				m_bCreated;

	bool				m_bDead;

	GField*				m_pField;
	bool				m_bDeleteMe;		// �̰��� true�̸� EntityMgr�� Update���� ���� delete���ش�.

	bool				m_isActivateUpdate;	///< EntityMgr���� ������Ʈ ��� ����
};

#endif
