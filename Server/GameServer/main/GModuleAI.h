#ifndef _GMODULE_AI_H
#define _GMODULE_AI_H

///
/// ��ƼƼ�� �ΰ����� ���
//		: �ΰ����ɿ� ���õ� ���µ�� ó���ڵ��� ������ �Ѵ�. (�������� ������ �������� ����)
//////////////////////////////////////////////////////////////////////////
#include "GModule.h"
#include "MTime.h"
#include "GAIEvent.h"
#include "GNPCAI.h"

#define SERVER_AI	L"AI"


class GEntityNPC;
class GBehaviorStateMachine;
class GJobMgr;
class GJobRunner;
class GJob;
class GColtMgr;
class GNPCAI;
class GNPCAIEventInvoker;
class GHateTable;

class GModuleAI : public GModule, public MMemPool<GModuleAI>
{
	DECLARE_ID(GMID_AI);
public:	
	GModuleAI(GEntity* pOwner = NULL, GJobRunner* pJobRunner = NULL);
	virtual ~GModuleAI();

	void					Init();
	void					Reset();	
	void					OnCombatBegin(void);
	void					OnCombatEnd(void);
	void					OnSpawnEmptyField(void);
	void					OnSpawnEmptySector(void);
	void					OnSpawnPlayerSector(void);	
	void					OnMakeMajorLodNPC(void);

	void					DropCombat()				{ m_bDropCombat = true; 	}	// lazy������ ������ �����ϰ� ���� ��ġ�� ���ư���.
	void					SetAssist(bool bVal=true)	{ m_bAssistMode = bVal; 	}
	bool					IsAssist()					{ return m_bAssistMode; 	}	

	GEntityNPC*				GetOwnerNPC() const;
	GBehaviorStateMachine*	GetBehaviorFSM(void);
	GJobMgr*				GetJobMgr(void);
	deque<GJob*>			GetMissionJobs();
	GColtMgr*				GetColt()					{ return m_pColtMgr;		}
	virtual const wchar_t*	GetClassName(void)			{ return SERVER_AI;			}	/// Ŭ���� �̸� ���
	AI_LOD_LEVEL			GetLodLevel(void) const;

	bool					IsNowInvincibility();
	bool					HasNPCSession();
protected:
	virtual void			OnInitialized();
	virtual void			OnFinalized();
	virtual void			OnSubscribeMessage();
	virtual GMessageResult	OnMessage(GMessage& msg);
	virtual void			OnUpdate(float fDelta);
	GMessageResult			OnDefaultMessage(GMessage& msg);

private:	
	void					InitStates(GJobRunner* pJobRunner);
	void					JobUpdate(float fDelta);
	void					HandleMoveTest();
	void					Logf(const wchar_t* szText, ...);
	GHateTable&				GetHateTable() const;

private:
	GJobRunner*				m_pJobRunner;
	bool					m_bAssistMode;
	GColtMgr*				m_pColtMgr;
	bool					m_bDropCombat;
	GNPCAI*					m_pNPCAI;
	GNPCAIEventInvoker*		m_pAIEventInvoker;

public:
	void OnNotify(GAIEvent aiEvent) {m_pNPCAI->OnNotify(aiEvent);	}
};

#endif
