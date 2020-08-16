#pragma once
#include "XDummyBot_Roam.h"

class XDummySpawnMgr;
class XDummyLazyCommandPoster;

class XDummyBot_Loot : public XDummyBot_Roam
{
	enum PHASE
	{
		PHASE_INIT_REPEAT,
		PHASE_INIT_DONE,
		PHASE_KILL,
		PHASE_BEGIN,
		PHASE_SHOW,
		PHASE_LOOT,
		PHASE_REFRESHITEM,
		PHASE_END,
	};

public:
	XDummyBot_Loot(XBirdDummyAgent* pAgent, XDummyAgentInfo* pAgentInfo);
	~XDummyBot_Loot(void);

	virtual MCommandResult OnCommand(XBirdDummyAgent* pAgent, MCommand* pCommand);
	static const wchar_t* GetName() { return L"Loot"; }

protected:
	virtual void OnRun(float fDelta);

private:
	vector<MUID> GetRandomLootItem();
	
	void ChangePhase(PHASE nPhase);
	bool CheckPostTime();

private:
	XDummySpawnMgr* m_pSpawnMgr;
	UIID			m_nTargetUIID;
	MUID			m_nTargetUID;
	PHASE			m_nPhase;

	vector<TD_ITEM_DROP>		m_vecTDDropItem;
	MRegulator*		m_pPhaseRegulator;
	
};

//////////////////////////////////////////////////////////////////////////
///
/// ���ۿ��� :	spawn_id �� ������ NPC �� ã�� ���� �� �����մϴ�.
///				�̹� �ʵ忡 max_spawn �� �̻��� spawn_id �� ������ NPC �� ���� ��� �������� �ʽ��ϴ�.
///				������ �� spawn_tick �ʸ��� �մϴ�.
///	
///	���ǻ��� :	�������� �ִ� ���� ���Ͻʽÿ�.
///				spawn_id �� ����Ǵ� NPC �� �������� �ƴϰ� �Ͻʽÿ�. �����Ȳ�� ������ �� �����ϴ�.
///				���� spawn_id �� ���� ��ȯ�Ǵ� ������ ���Ͻʽÿ�. ������ ���� ��å�� ���ӵǾ� �����ϴ�.
///
//////////////////////////////////////////////////////////////////////////