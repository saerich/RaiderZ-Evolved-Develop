#pragma once

#include "GActorObserver.h"

class GEntityPlayer;
class GPercentDice;

class GActorInvisibility : private GActorObserver, public MTestMemPool<GActorInvisibility>
{
public:
	GActorInvisibility(GEntityActor* pOwner);
	~GActorInvisibility(void);
	
	// ������Ʈ ó��
	void OnUpdate( float fDelta );

	const vector<MUID>& GetBlinders()		{ return m_vecBlinders; }
	const vector<MUID>& GetDetectors()		{ return m_vecDetectors; }

	// ����� �ֻ��� ����
	void SetDice(GPercentDice* pDice);
	// Ž�� Ȯ�� ��ȯ
	bool CheckDetectChance(GEntityActor* pActor);
	// Ž�� Ȯ�� ��ȯ
	float CalcDetectChancePercent(GEntityActor* pActor);
	// Ž�� �Ÿ� ��ȯ
	int CalcDetectDistance(GEntityActor* pActor);
	// �������� Ȯ�� ��ȯ
	int CalcDetectLevelPercent(GEntityActor* pActor);
	// �������� Ȯ�� ��ȯ
	float CalcDetectArmorPercent();
	// �Ÿ����� Ȯ�� ��ȯ
	float CalcDetectDistancePercent(GEntityActor* pActor);
	// ���� ���� �ȿ� ���Դ°�?
	bool IsInSight(GEntityActor* pTarget);
private:
	// ã�� ���� ������Ʈ ó��
	void UpdateDetection( float fDelta );
	// ã�� ���� ó��
	void OnTryDetect();
	// ������ ���͸� ã�� ó��
	void OnDetected( const vector<MUID>& vecEffectees );
	// ������ ���͸� ��ã�� ó��
	void OnBlinded( const vector<MUID>& vecEffectees );
	// ���ο� �����͵��� ã��
	void CalcNewBlindersAndDetectors(vector<GEntityActor*>& vecBlinders, vector<GEntityActor*>& vecDetecteors);
	// ����ȭ ���̱�
	void OnShow();
	// ����ȭ �����
	void OnHide();


	// ������ ������� �̺�Ʈ
	virtual void OnGainBuff(int nBuffID) override;
	// ������ �Ҿ����� �̺�Ʈ
	virtual void OnLostBuff(int nBuffID) override;
	

private:
	GEntityActor*	m_pOwner;
	MRegulator		m_rgrDetection;
	vector<MUID>	m_vecBlinders;
	vector<MUID>	m_vecDetectors;
	GPercentDice*	m_pDice;
	float			m_fInvisibilityPower; // ���� Ư���Ӽ������� ���޵� ����ȭ���ɰ�
};
