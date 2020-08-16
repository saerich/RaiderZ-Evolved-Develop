#pragma once

#include "GTalentInfo.h"

class GEntityActor;

// ����: ExtraActive �ŷ�Ʈ���� ���̴� ȿ���� �����س��� Ŭ����
//			ExtraActive�� 1)�ڱ��ڽſ��� �������� 2)�ŷ�Ʈ�� ����ϴ� ������ �Ҹ���� 3)�Ϲ����� �ŷ�Ʈ ������Ƽ�� ������ �� ���� ����� ��Ƽ�� �ŷ�Ʈ�� ���Ѵ�.
class GExtraActiveTalentRunner
{
public:
	// ����Ʈ�� ��Ƽ�� �ŷ�Ʈ�� ���� ��Ŵ, �ŷ�Ʈ ���� ������ ���� �õ��ϰ� �ش��� �ȵǸ� Ư�� ����Ʈ�� �ŷ�Ʈ ����
	void Apply(GEntityActor* pUser, vec3 vPos, GTalentInfo* pTalentInfo, GEntityActor* pVictim);
private:
	// ���ݸ�� (PC����)
	void onTogglePrecision( GEntityActor* pTargetActor );
	// ���Ÿ�� (PC����)
	void onToggleSneak( GEntityActor* pTargetActor, int nSneakBuffType );
	// ���Ÿ�� (PC����)
	void onApplyAdvent( GEntityActor* pTargetActor );
	// ���� (PC����)
	void onSpawn(GEntityActor* pUser, int nNPCID, int nDespawnTime);
	// ��Ȱ
	void onRebirth(GEntityActor* pUser, GEntityActor* pTarget, int nRecoverRate);
	// ��Ż
	void onPortal(GEntityActor* pTarget, int nFieldID, int nMarkerID);
	// ��Ż ����
	void onPortal_SoulBinding(GEntityActor* pTarget);
	// ����
	void onTaunt(GEntityActor* pUser, GTalentInfo* pTalentInfo, GEntityActor* pVictim);
	// ���� ��ƼƼ ����
	void onBuffEntity(GEntityActor* pUser, vec3 vPos, int nIncludeBuffID, int nLimitQty);
	// �����ֱ��� ������ �����(100����) ����
	void onDispel(GEntityActor* pUser, GTalentInfo* pTalentInfo, GEntityActor* pVictim);
	// �����ֱ��� �� �����(200����) ����
	void onCurePoison(GEntityActor* pUser, GTalentInfo* pTalentInfo, GEntityActor* pVictim);
	// �����ֱ��� ������ �����(300����) ����
	void onCureDisease(GEntityActor* pUser, GTalentInfo* pTalentInfo, GEntityActor* pVictim);
	// �����ֱ��� ���ַ� �����(400����) ����
	void onCureCurse(GEntityActor* pUser, GTalentInfo* pTalentInfo, GEntityActor* pVictim);
	// �����ֱ��� �̵��Ұ��� �����(500����) ����
	void onDispelMesmerize(GEntityActor* pUser, GTalentInfo* pTalentInfo, GEntityActor* pVictim);
	// �����ֱ��� ��Ȱ ���Ƽ �����(600����) ����
	void onDispelDeathPenalty(GEntityActor* pUser, GTalentInfo* pTalentInfo, GEntityActor* pVictim);

	// ������� �̷ο� ���� ����
	void onDispelBuff(GEntityActor* pUser, GTalentInfo* pTalentInfo, GEntityActor* pVictim);
	// ������� ��� ��Ŀ�� ����, NPC��
	void onDispelFocus(GEntityActor* pUser, GTalentInfo* pTalentInfo, GEntityActor* pVictim);	
	// ������� ���⺯ȯ ���� ����
	void onDispelBPart(GEntityActor* pUser, GTalentInfo* pTalentInfo, GEntityActor* pVictim);
};	
