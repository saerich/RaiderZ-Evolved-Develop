#pragma once

#include "MPhysXObject.h"

namespace physx {

//////////////////////////////////////////////////////////////////////////
//
// MPxObjClothed
//  - Cloth ���� ���� ������ �ֵ��۾�
//	- NxCloth�� ������������ �ְ� �׷��� �������� �ִ�.
//
//////////////////////////////////////////////////////////////////////////

class MPxObjClothed : public MPhysXObject
{

public:

	MPxObjClothed();
	virtual ~MPxObjClothed();

	//////////////////////////////////////////////////////////////////////////
	// Clothed Object Entity
	//
	class ClothedEntity
	{
	public:
		ClothedEntity(){ m_pPxObjClothed = NULL; }
		virtual ~ClothedEntity(){ _ASSERT(m_pPxObjClothed == NULL); }

		virtual void OnNotifySimulatedResult() = 0;

	private:
		friend				MPxObjClothed;
		MPxObjClothed*		m_pPxObjClothed;
	};

	//////////////////////////////////////////////////////////////////////////
	// Clothed Action
	//
	class ClothedAction
	{
	public:
		ClothedAction(){}
		virtual ~ClothedAction(){}
		virtual bool OnAct(float fRemainedTime) = 0;
	};


	//////////////////////////////////////////////////////////////////////////
	// Clothed Object Public Method
	//

	virtual void NotifyResultOnSimulated()
	{
		if (m_pClothedEntity)
			m_pClothedEntity->OnNotifySimulatedResult();

		UpdateAction();
	}

	virtual const MBox* GetAreaBox()
	{
		return NULL;
	}

	void			RegisterClothedEntity(ClothedEntity* pEntity);
	void			UnRegisterClothedEntity();

	void			AddAction(float fDuration, ClothedAction* pAction);
	void			ClearActionTables();

	NxClothDesc*	CreateNxClothDesc(const std::string& _rFileName, const std::string& _rContextName);

protected:

	virtual void								OnDoPreUpdateSubActorTransform() { _ASSERT(0); } //�Ⱥҷ�����
	virtual void								OnDoPostUpdateSubActorTransform(){ _ASSERT(0); } //�Ⱥҷ�����

	std::list< pair<float, ClothedAction*> >	m_listActionTables;
	void										UpdateAction();

private:
	ClothedEntity*								m_pClothedEntity;

};

} // namespace physx