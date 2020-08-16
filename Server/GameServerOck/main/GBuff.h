#pragma once

#include "CSBuffInfo.h"
#include "GTickRegulator.h"
#include "MTime.h"
#include "GBuffObserver.h"
#include "GBuffObserverMgr.h"

class GEntitySync;
class GTalentInfo;
class GBuffInfo;
class GBuffReleaser;
class GBuffInstantApplier;
class GBuffModifierApplier;
class GBuffTriggerApplier;
class GBuffRouter;
class GBuffObserver;
class GEFfectTimingChecker;
class GBuffStack;
class GBuffScript;
class GBuffPaletteChanger;
class GBuffEquipmentChanger;


class GBuff : public GBuffObserverMgr, public MTestMemPool<GBuff>
{
	enum DESTROY_TYPE
	{
		DESTROY_NONE=0,
		DESTROY_EXPIRED,
		DESTROY_CANCELLED,
		DESTROY_DISPELLED,
	};
public:
	GBuff(GEntitySync* pOwner, GBuffInfo* pBuffInfo, float fDurationTime, float fPeriodTime, GTalentInfo* pTalentInfo=NULL, MUID uidUser=MUID::Invalid());
	virtual ~GBuff();

public:
	// ������ �ɷ��� �� �̺�Ʈ, ��ȯ���� �����ڰ� �׾����� ����
	bool Start();
	// ������ �Ҹ�ɶ� �̺�Ʈ, ��ȯ���� �����ڰ� �׾����� ����
	bool Finish(bool bRoute);
	// ��� ������ ��� �����ϱ� ���� ƽ �̺�Ʈ,��ȯ���� ������ ����ƴ��� ����
	void PreUpdate( float fDelta );
	// ƽ �̺�Ʈ,��ȯ���� ������ ����ƴ��� ����
	bool Update(float fDelta);
	// �����ð� ����
	void UpdateBuffTime(float fDurationTime, float fPeriodTime);
	// ���� �Ҹ� �̺�Ʈ
	void OnDestry();
	// ������ �ߺ��Ǽ� �ɸ� ���
	void OnDuplicated(float fDurationTime, float fPeriodTime, MUID uidUser);
	// ������ ���õǼ� �ɸ� ���
	void OnStacked(float fDurationTime, float fPeriodTime);
	// ���� ��ҽ�Ŵ, ������ ������ ������ �ϳ� ���� (lazy�ϰ� ó��)
	void Cancel();
	// ���� ��ҽ�Ŵ, ���õ� ���� ���  (lazy�ϰ� ó��)
	void CancelForced();
	// ������ �����Ŵ (lazy�ϰ� ó��)
	void Dispel();

	// ���� ���̵�
	int GetBuffID();
	// ����� UID ��ȯ
	MUID GetUserUID();
	// ���� �ŷ�Ʈ���� ��ȯ
	GTalentInfo* GetUserTalentInfo();
	// �������� Ÿ�� ��ȯ
	DESTROY_TYPE GetDestroyType() const;
	// ������ �ֱ���ȿ���� �ִ��� ����
	bool HasPeriodEffect();
	// ������ ������ ���ӵ��� ����
	bool IsInfinite() const;
	// ������ �Ҹ�Ǵ��� ����
	bool IsGone() const;
	// ������ ���� ����
	int	GetBuffStackSlot();
	// ������ ���� �Ŀ�
	int	GetBuffStackPower();	
	// ���ӽð�
	float GetDurationTime();
	// ���� �ð�
	float GetRemainTime();
	// �ֱ����ð�
	float GetPeriodTime();
	// ���� ���� ��ȯ
	int GetStackCount() const;
	// ���� �ֱ���� ���� �ð�
	float GetRemainNextPeriodTime();
	// ������ ��ȯ
	GEntitySync* GetOwner() const { return m_pOwner; }
	// ���� ����
	GBuffInfo* GetInfo() const { return m_pInfo; }

	// ��������� ����
	bool IsDebuff() const;
	// ���ð����� �������� ����
	bool IsStackable() const;
	// ��ȭ �������� ����
	bool IsEchant() const;
	// ��� ���� �������� ����
	bool IsChangeEquipment() const;
	
private:
	// ������� ����
	void Update_Maintenance( float fDelta );
	// �ֱ�ȿ�� ó��
	bool Update_Period( float fDelta );
	// ����ð� ó��
	void Update_Expired( float fDelta );

	// ���� ������ ȿ��ó��
	bool OnGain();
	// ���� ������ ȿ��ó��
	bool OnLost(bool bRoute);

	// ���� �̺�Ʈ �˻�
	bool CheckEvent(TALENT_CONDITION nCondition);
	// �ֱ� ȿ��
	bool OnPeriod();
private:
	// �ɸ� ������ ����
	GBuffInfo*			m_pInfo;
	// ������ ���� ����
	GEntitySync*		m_pOwner;
	// ���� ���� �˸���
	MRegulator			m_rgrExpire;
	// ���� �ֱ���ȿ�� �˸���
	GTickRegulator		m_rgrPeriod;
	// ƽ�� ���� ��� �˸���
	GTickRegulator		m_rgrTickCost;
	// ������ ������ ���ӵ��� ����
	bool				m_bInifinity;
	// ���� ���� ó����
	GBuffReleaser*		m_pReleaser;
	// ���� ���� ó����
	GBuffStack*			m_pStack;
	// ���� �̺�Ʈ �˸���
	GEFfectTimingChecker*	m_pEffectTimingChecker;
	// ������ �ɸ��� ���� �ŷ�Ʈ ������
	GTalentInfo*		m_pTalentInfo;
	// ������ �ɸ��� ���� ���� UID
	MUID				m_uidUser;
	// ���� ���ȿ�� ������
	GBuffInstantApplier* m_pInstantApplyer;
	// ���� ����ȿ�� ������
	GBuffModifierApplier* m_pModifierApplyer;
	// ���� Ʈ���� ������
	GBuffTriggerApplier* m_pTriggerApplyer;
	// ���� ��Ŷ ������
	GBuffRouter*		m_pRouter;
	// ���� ����Ÿ��
	DESTROY_TYPE		m_nDestroyType;
	// ���� ��ũ��Ʈ
	GBuffScript*		m_pBuffScript;
	// �ŷ�Ʈ �ȷ�Ʈ ������
	GBuffPaletteChanger*	m_pPaletteChanger;
	// �ŷ�Ʈ ��� ������
	GBuffEquipmentChanger*	m_pEquipmentChanger;
	// Gain ȿ�� �޾Ҵ��� ����
	bool				m_bGainEffectOccured;
};
