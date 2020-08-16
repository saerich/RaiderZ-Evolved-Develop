#pragma once

class GEntityNPC;

class GNPCFirstStrike : public MTestMemPool<GNPCFirstStrike>
{
public:
	GNPCFirstStrike(GEntityNPC* pOwner);
	~GNPCFirstStrike(void);

	// ������Ʈ ó��
	void Update(float fDelta);

	// ���� ���� �������� ����
	bool IsEnable() const;

	// ���� ���� ���ɿ��� ����
	void SetEnableSpawnDelay(bool var);

private:
	GEntityNPC* m_pOwner;
	// ������ ���������� �ð�
	MRegulator m_rgrFirstEnable;
	// ������ �������� ����� ������� ����
	bool m_bEnableSpawnDelay;
	// ������ �������� �������� ����
	bool m_bSpawnDelay;
};
