#pragma once

#include "GActorObserver.h"
#include "GMonitorPortBase.h"

class GField;
class GEntityActor;

class GCombatMonitorPort : public GActorObserver, public GMonitorPortBase
{
public:
	GCombatMonitorPort(void);
	virtual ~GCombatMonitorPort(void);

	static GCombatMonitorPort& Inst();
	void	OnMsg( PCOPYDATASTRUCT pcds);

	void Prepare();
	void Start();
	void Stop();
	void Update(float fDelta);
	void Reset();
	void SetField(GField* pField);
	void AttachSubject(GEntityActor* pActor);
	void DettachSubject(GEntityActor* pActor);

	size_t GetSubjectQty() { return m_setSubjects.size(); }

private:
	void StopImpl();
	string ExtractFromActor( GEntityActor* pActor );
	void SendCommand(string strType);
	virtual void Send( string strMessage ) override;
	virtual void OnSendFailed() override;
	void SendStats( const char* szType, GEntityActor* pUser, GEntityActor* pAttacker, const char* szSourceType, int nSourceID, int nDamage=0);
	int GetActorID(GEntityActor* pActor);
private:
	virtual bool IsEnable() override;
	virtual const wchar_t* GetName() override { return L"Combat Monitor"; }
	virtual const char* GetFilePath() override { return "C:\\Works\\RaiderZ\\Develop\\Tools\\CombatMonitor\\bin\\Debug\\CombatMonitor.exe"; }
private:
	// ���� ����
	virtual void OnCombatBegin(GEntityActor* pOwner) override;
	// ���� ����
	virtual void OnCombatEnd(GEntityActor* pOwner) override;
	// �ŷ�Ʈ ����� ���۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnUseTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo ) override;
	// �ŷ�Ʈ ����� ȿ���� �ߵ��ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnActTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo ) override;
	// �ǰ� �̺�Ʈ
	virtual void OnHit( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo ) override;
	// �ǰ� ���� �̺�Ʈ (�ŷ�Ʈ ����� �� 1ȸ ȣ��)
	virtual void OnHitSuccess( GEntityActor* pOwner, GTalentInfo* pTalentInfo ) override;
	// ��� �ǰ� ���� �̺�Ʈ (��Ʈĸ������ �뵵)
	virtual void OnHitAll( GEntityActor* pOwner, GTalentInfo* pTalentInfo ) override;
	// ġ��Ÿ �ǰ� �̺�Ʈ
	virtual void OnCriHit( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo ) override;
	// ������ �̺�Ʈ
	virtual void OnMiss(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo)  override;
	// ��� �̺�Ʈ (��� ���)
	virtual void OnGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo)  override;
	// ���� ��� �̺�Ʈ
	virtual void OnPerfectGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo)  override;
	// �κ� ��� �̺�Ʈ
	virtual void OnPartialGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo)  override;
	// ���� ��� �̺�Ʈ
	virtual void OnAbsoluteGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo)  override;
	// �ŷ�Ʈ�� ��ȿ�������� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTalentImmuned(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override;
	// �������� ����� �� �̺�Ʈ
	virtual void OnDamage(GEntityActor* pOwner, MUID uidAttacker, int nDamage, EFFECT_SOURCE_TYPE nEffectSourceType, int nEffectSourceID) override;
private:
	GField*	m_pField;
	set<MUID> m_setSubjects;
	bool m_bRunning;
	bool m_bWillStop;

};
