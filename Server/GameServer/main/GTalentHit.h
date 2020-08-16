#pragma once

class GTalent;
class GTalentInfo;
class GEntityActor;

//////////////////////////////////////////////////////////////////////////
//
//	GTalentHit
//
//////////////////////////////////////////////////////////////////////////

class GTalentHit
{
public:
	GTalentHit();

	// �ŷ�Ʈ ����ó��
	void UpdateHit(GTalent* pTalent, float fElapsedTime);
	// �ŷ�Ʈ�� ��Ʈ�� ���Ͱ� �ִ��� ����
	bool HasVictim()			{ return m_VictimUIDSet.empty()==false; }
public:
	// �ش� �ŷ�Ʈ�� ���ظ� ������ ��
	void			OnGainDamage(GEntityActor* pTarget);
	// �ŷ�Ʈ�� ����ɶ� ȣ�� (Finish or Canceled)
	void			OnExit(GTalent* pTalent);
private:
	// ���׸�Ʈ ���� ����ó��
	void UpdateHitSeg(GTalent* pTalent, int nHitColIndex, int nAddtiveDamage, float fCheckTime);
private:
	// ����� ��Ʈ �ε���
	int				m_nHitIndex;
	// ��Ʈ�� ����UID
	set<MUID>		m_VictimUIDSet;
	// ��Ʈĸ���� ���� Ƚ��
	map<MUID, int>	m_mapHitTable;
	// ���׸�Ʈ���� ������������
	set<int>		m_setHitsegment;
};
