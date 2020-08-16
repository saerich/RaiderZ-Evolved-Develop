#pragma once

class GEntityActor;
class GTalentInfo;
class GBuffInfo;

class GUseCostMgr
{
public:
	GUseCostMgr(void);
	~GUseCostMgr(void);

	// �ŷ�Ʈ ��� �������� ���� ��ȯ
	bool IsUseTalent(GEntityActor* pUser, GTalentInfo* pTalentInfo);
	// ���� ���� �������� ���� ��ȯ
	bool IsMaintenanceBuff(GEntityActor* pUser, GBuffInfo* pTalentInfo);
	// �ŷ�Ʈ ��� ����� ������, ������ �Ұ����ϸ� false�� ��ȯ
	bool Pay_TalentCost(GEntityActor* pUser, GTalentInfo* pTalentInfo);
	// �ŷ�Ʈ ��� ����� ������ ������
	void Pay_TalentCostForce(GEntityActor* pUser, GTalentInfo* pTalentInfo);
	//	���� ���� ����� ������, ������ �Ұ����ϸ� false�� ��ȯ
	bool Pay_BuffMaintenanceCost(GEntityActor* pUser, GBuffInfo* pBuffInfo, int nTickCounter);
};
