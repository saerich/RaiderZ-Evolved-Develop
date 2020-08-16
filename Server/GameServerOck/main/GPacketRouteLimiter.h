#pragma once

class GPacketRouteLimiter
{
public:
	GPacketRouteLimiter(void);
	~GPacketRouteLimiter(void);

	// ���ⷹ�� ��ȯ
	int CalcCrowdedLevel(int nPlayerQty) const;
	// ��Ŷ ���� Ƚ��
	int LimitPacketQty(int nPlayerQty) const;
	// ����� �õ�
	bool TryRoute(int nPlayerQty, int nCmdID, MUID uidSender);
	// ��ϵ� �÷��̾� ��
	int GetRegisteredPlayerQty() const;
	// ���Ͱ��� ������Ų ���Ͱ� �߰�
	void AddOwner(uint32 nFlatIdx);
	// ���Ͱ��� ������Ų ���Ͱ� ����
	void RemoveOwner(uint32 nFlatIdx);
	// ���� �������� ������ ����
	bool IsEmptyOwner() const;
	// �ش� ���Ͱ��� ������Ų �������� ����
	bool IsOwnerSector(uint32 nFlatIdx);
private:
	void InsertCircularQueue(MUID uid);
private:
	typedef map<int,int>				PacketCountMap;
	typedef map<MUID,PacketCountMap>	PlayerPacketCountMap;

	PlayerPacketCountMap m_mapCounts;
	deque<MUID>			 m_vecCircularQueue;
	set<uint32>			 m_setOwnerSector;
};
