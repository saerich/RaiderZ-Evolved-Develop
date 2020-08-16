#pragma once

class GEntityNPC;

class GNPCMinion : public MTestMemPool<GNPCMinion>
{
public:
	GNPCMinion(GEntityNPC* pOwner);
	~GNPCMinion(void);

	// ���� ��ȯ
	GEntityNPC* Summon(int nNPCID, vec3 vStartPos);

	// ��ȯ�� ��ȯ
	GEntityNPC* GetSummoner() const;

	// ���� UID ����
	void SetMasterUID(MUID uidMaster)		{ m_uidMaster = uidMaster; }
	// ������ �ִ��� ����
	bool HasMaster()		{ return m_uidMaster.IsValid(); }
	// ���� ���� ��ȯ
	size_t GetMinionCount()	{ return m_vecMinions.size(); }

	// �����ڰ� �׾����� ȣ��
	void OnDead();
	// ���ϰ� �׾����� ȣ��
	void OnDeadMinion(MUID uidMinion);
	// ������ ����� �� ȣ��
	void OnEndCombat();

private:
	// ������ �׾����� ȣ��
	void OnDeadMaster();
	// ��� ���ϸ� ����
	void KillAllMinions();
	// ��� ���ϸ� ����
	void DespawnAllMinions();
	// ������ ���� �����ϰ� ����
	void SetTargetMasterEnemy(GEntityNPC* pMinion);
	
private:
	GEntityNPC*			m_pOwner;
	vector<MUID>		m_vecMinions;
	MUID				m_uidMaster;
};
