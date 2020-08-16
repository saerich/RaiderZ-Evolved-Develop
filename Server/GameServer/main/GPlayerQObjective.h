#ifndef _G_PLAYER_QOBJECTIVE_H_
#define _G_PLAYER_QOBJECTIVE_H_

class GQObjectiveInfo;

/// �÷��̾��� ����Ʈ ���� ����
class GPlayerQObjective : public MTestMemPool<GPlayerQObjective>
{
private:	
	GQObjectiveInfo*	m_pQObjectiveInfo;
	int		m_nProgress;
	bool	m_bComplete;

public:
	GPlayerQObjective(GQObjectiveInfo* pQObjectiveInfo);
	GQObjectiveInfo* GetInfo();

	int GetProgress();
	void UpdateProgress(int nProgress);
	bool IsComplete();
};

#endif

