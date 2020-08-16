#pragma once

#include "GTalentEffector.h"

class GTalentInfo;

class GPlayerPassiveTalent
{
public:
	// �нú� ȿ���� ����
	void GainEffect( GTalentInfo* pTalentInfo );
	// �нú� ȿ���� ����
	void LostEffect( GTalentInfo* pTalentInfo );
	// �ش� �нú� Ÿ���� ȿ���� �ް� �ִ��� ���θ� ��ȯ
	bool HasEffect(TALENT_EXTRA_PASSIVE_TYPE nPassiveType) const;
	// �ش� �нú� Ÿ���� Ư�� ȿ������ �̻��� ȿ���� �ް� �ִ��� ���θ� ��ȯ
	bool HasEffect(TALENT_EXTRA_PASSIVE_TYPE nPassiveType, int nExtraPassiveLevel) const;
	// �нú� ȿ���� ���� ��ȯ
	int  GetPassiveValue(TALENT_EXTRA_PASSIVE_TYPE nPassiveType) const;
	// ������ ��� �нú��ŷ�Ʈ�� ȿ���� ����
	void ApplyEffect( TALENT_CONDITION nCondition, GEntityActor* pThis, GEntityActor* pVictim );

	int GetRank(TALENT_EXTRA_PASSIVE_TYPE nPassiveType);
private:
	// �нú� ȿ���� �߰�
	void AddEffect(TALENT_EXTRA_PASSIVE_TYPE nEffectType, int nValue);
	// �нú� ȿ���� ����
	void RemoveEffect(TALENT_EXTRA_PASSIVE_TYPE nEffectType);
	// �ش� �ŷ�Ʈ ���ο� �´� �ŷ�Ʈ������ ��ȯ, ���ٸ� NULL ��ȯ
	GTalentInfo* GetEffect(int nTalentLine);
	// �ش� �нú� Ư������ ����, �нú� ȿ���� �������� �ʴ´ٸ� false�� ��ȯ
	bool GetEffectValue(TALENT_EXTRA_PASSIVE_TYPE nEffectType, int& nEffectValue) const;
	// �ش� �нú� Ư������ ������, �нú� ȿ���� �������� �ʴ´ٸ� false�� ��ȯ
	bool ModifyEffectValue(TALENT_EXTRA_PASSIVE_TYPE nEffectType, int nEffectValue);
	bool IsRemovableEffect(TALENT_EXTRA_PASSIVE_TYPE nEffectType);
	// ����Ÿ���� �нú� Ÿ������ ������ ��ȯ
	TALENT_EXTRA_PASSIVE_TYPE  ConvertToPassiveType(WEAPON_TYPE nType);
	void GainEffectImpl( GTalentInfo* pTalentInfo, TALENT_EXTRA_PASSIVE_TYPE nEffectType, int nExtraPassiveParam );
	void LostEffectImpl(GTalentInfo* pTalentInfo, TALENT_EXTRA_PASSIVE_TYPE nEffectType, int nExtraPassiveParam);
private:
	typedef map<TALENT_EXTRA_PASSIVE_TYPE, int> MAP_EXTRA_PASSIVE_TYPE;	// 
	typedef map<int, GTalentInfo*> MAP_TALENTS;		
	// �нú� Ư���� �����̳�, second=CustomValue
	MAP_EXTRA_PASSIVE_TYPE			m_mapPassiveValues;
	// �нú� �ŷ�Ʈ �����̳�, // first=TalentLine
	MAP_TALENTS			m_mapPassiveTalents;
	// �нú� ȿ���� �����ϱ� ���� ������
	GTalentEffector		m_TalentEffector;
};

