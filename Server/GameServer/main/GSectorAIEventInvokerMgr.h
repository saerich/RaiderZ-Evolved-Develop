#pragma once

#include "GFieldGrid.h"

class GSectorAIEventInvoker;

//////////////////////////////////////////////////////////////////////////
//
//	GFieldCellAIEventInvokerMgr
//
//////////////////////////////////////////////////////////////////////////

class GSectorAIEventInvokerMgr : public MTestMemPool<GSectorAIEventInvokerMgr>
{
public:
	GSectorAIEventInvokerMgr(void);
	~GSectorAIEventInvokerMgr(void);

	void Init(MUID uidField, int nSizeX, int nSizeY);
	void Clear();

	// ��ƼƼ �߰�
	void AddEntity( const MPoint& ptCell, ENTITY_TYPEID nEntityTypeID, const MUID& uid, GFieldGrid::EntitySelector* selectorEntity );
	// ��ƼƼ ����
	void RemoveEntity( const MPoint& ptCell, ENTITY_TYPEID nEntityTypeID, GFieldGrid::Sector* pCell);
	// �̺�Ʈ ���� ����
	void EventGuardBegin(const MPoint& ptCell);
	// �̺�Ʈ ���� ����
	void EventGuardEnd(const MPoint& ptCell);
private:
	vector <vector <GSectorAIEventInvoker*> >	m_vecFieldCellAIEventInvokers;
};

//////////////////////////////////////////////////////////////////////////
//
//	AIEventInvokerGuarder
//
//////////////////////////////////////////////////////////////////////////

class AIEventInvokerGuarder
{
public:
	AIEventInvokerGuarder(GSectorAIEventInvokerMgr* pOwner, MPoint ptCell);
	~AIEventInvokerGuarder();

private:
	GSectorAIEventInvokerMgr*	m_pOwner;
	MPoint	m_ptCell;
};

#define AIEVENT_INVOKER_GUARD(owner, cell)  AIEventInvokerGuarder(owner, cell)