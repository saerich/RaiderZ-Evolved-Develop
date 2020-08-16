#include "StdAfx.h"
#include "GTestForward.h"
#include "GEntityPlayer.h"
#include "GActorObserver.h"
#include "GServer.h"
#include "GFieldMgr.h"
#include "FBasePlayer.h"
#include "GModuleBuff.h"
#include "GHateTable.h"

class MockEntityActor_ForEvent : public GEntityActor, public TestCallCounter
{
public:
	// ���� ����
	virtual void OnCombatBegin(GEntityActor* pOwner) override {
		__super::OnCombatBegin(pOwner);
		INCREASE_CALLCOUNTER; 
	}
	// ���� ����
	virtual void OnCombatEnd(GEntityActor* pOwner) override {
		__super::OnCombatEnd(pOwner);
		INCREASE_CALLCOUNTER; 
	}
	// �ڽ��� �׾����� ȣ��Ǵ� �̺�Ʈ
	virtual void OnDie() override {
		__super::OnDie();
		INCREASE_CALLCOUNTER; 
	}

	// �ڽ��� �׾����� ȣ��Ǵ� �̺�Ʈ
	virtual void OnKilled(GEntityActor* pTarget) override {
		__super::OnKilled(pTarget);
		INCREASE_CALLCOUNTER; 
	}

	// ��밡 �ڽ��� ����Ʈ���̺��� �߰��Ͽ�����
	virtual void OnAddHateTarget( GEntityActor* pEnemy ) override {
		__super::OnAddHateTarget(pEnemy);
		INCREASE_CALLCOUNTER; 
	}
	// ��밡 �ڽ��� ����Ʈ���̺��� �����Ͽ�����
	virtual void OnLostHateTarget( MUID uidEnemy ) override {
		__super::OnLostHateTarget(uidEnemy);
		INCREASE_CALLCOUNTER; 
	}	
	// �ڽ��� ����Ʈ���̺� Ÿ���� ���ŵǾ�����
	virtual void OnLostHateEntry(MUID nEnemyUID) override {
		__super::OnLostHateEntry(nEnemyUID);
		INCREASE_CALLCOUNTER; 
	}

	// �ǰ� �̺�Ʈ
	virtual void OnHit(GHitInfo infoHit, bool bRouteHit) override { 
		__super::OnHit(infoHit, bRouteHit);
		INCREASE_CALLCOUNTER; 
	}
	// ���� ���������� �̺�Ʈ
	virtual void OnHitEnemy(uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pTalentInfo) override { 
		__super::OnHitEnemy(nCombatResultFalg, pTarget, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// �������� �ڽ��� �ǰݵ��� ��
	virtual void OnMagicHit(GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override { 
		__super::OnMagicHit(pAttacker, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// �������� �ڽ��� �������� ��
	virtual void OnMagicHitEnemy(GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override { 
		__super::OnMagicHitEnemy(pAttacker, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ���ٰ������� �ڽ��� �ǰݵ��� ��
	virtual void OnMeleeHit(GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override	{
		__super::OnMeleeHit(pAttacker, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ���ٰ������� �ڽ��� �������� ��
	virtual void OnMeleeHitEnemy(GEntityActor* pTarget, GTalentInfo* pTalentInfo) override {
		__super::OnMeleeHitEnemy(pTarget, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}

	// ġ��Ÿ �ǰ� �̺�Ʈ
	virtual void OnCriHit( GEntityActor* pAttacker, GTalentInfo* pTalentInfo ) override {
		__super::OnCriHit(pAttacker, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ���� ġ��Ÿ�� ���������� �̺�Ʈ
	virtual void OnCriHitEnemy( GEntityActor* pTarget, GTalentInfo* pTalentInfo ) override {
		__super::OnCriHitEnemy(pTarget, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ������ �̺�Ʈ
	virtual void OnMiss(uint32 nCombatResultFalg, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo) override { 
		__super::OnMiss(nCombatResultFalg, pAttacker, pAttackTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ���� ������ ȸ�������� �̺�Ʈ
	virtual void OnMissEnemy(uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pAttackTalentInfo) override { 
		__super::OnMissEnemy(nCombatResultFalg, pTarget, pAttackTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ��� �̺�Ʈ
	virtual void OnGuard(GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo) override { 
		INCREASE_CALLCOUNTER; 
		__super::OnGuard(pAttacker, pAttackTalentInfo);
	}
	// ���� ������ �������� �̺�Ʈ
	virtual void OnGuardEnemy(GEntityActor* pTarget, GTalentInfo* pAttackTalentInfo) override { 
		__super::OnGuardEnemy(pTarget, pAttackTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ������ ������� �̺�Ʈ
	virtual void OnGainBuff(int nBuffID) override { 
		__super::OnGainBuff(nBuffID);
		INCREASE_CALLCOUNTER; 
	}
	// ������ �Ҿ����� �̺�Ʈ
	virtual void OnLostBuff(int nBuffID) override { 
		__super::OnLostBuff(nBuffID);
		INCREASE_CALLCOUNTER; 
	}
	// �ŷ�Ʈ ����� ���۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnUseTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo ) override {
		__super::OnUseTalent(pUser, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// �ŷ�Ʈ ����� ȿ���� �ߵ��ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnActTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo ) override {
		__super::OnActTalent(pUser, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ���� �ŷ�Ʈ ����� ȿ���� �ߵ��ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnMagicActTalent( GTalentInfo* pTalentInfo ) override {
		__super::OnMagicActTalent(pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// �ŷ�Ʈ ����� �Ϸ������ ȣ��Ǵ� �̺�Ʈ
	virtual void OnFinishTalent(GTalentInfo* pTalentInfo) override {
		__super::OnFinishTalent(pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// �ŷ�Ʈ�� ��ȿ�������� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTalentImmuned(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override {
		__super::OnTalentImmuned(pOwner, pAttacker, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// �������� ��۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTogglePrecision() override {
		__super::OnTogglePrecision();
		INCREASE_CALLCOUNTER; 
	}
	// �������� ��۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnToggleSneak(int nSneakBuffType) override {
		__super::OnToggleSneak(nSneakBuffType);
		INCREASE_CALLCOUNTER; 
	}
	// ������ ����ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnApplyAdvent() override {
		__super::OnApplyAdvent();
		INCREASE_CALLCOUNTER; 
	}
	// ������ �޾����� �̺�Ʈ
	virtual void OnRewarded(int nNPCID) override {
		__super::OnRewarded(nNPCID);
		INCREASE_CALLCOUNTER; 
	}

	void SetEntityType(ENTITY_TYPE nEntityType)
	{
		m_nType = nEntityType;
	}
};

class MockActorObserver_ForEvent : public GActorObserver, public TestCallCounter
{
public:
	// ���� ����
	virtual void OnCombatBegin(GEntityActor* pOwner) override {
		__super::OnCombatBegin(pOwner);
		INCREASE_CALLCOUNTER; 
	}
	// ���� ����
	virtual void OnCombatEnd(GEntityActor* pOwner) override {
		__super::OnCombatEnd(pOwner);
		INCREASE_CALLCOUNTER; 
	}
	// �ڽ��� �׾����� ȣ��Ǵ� �̺�Ʈ
	virtual void OnDie() override {
		__super::OnDie();
		INCREASE_CALLCOUNTER; 
	}

	// �ڽ��� �׾����� ȣ��Ǵ� �̺�Ʈ
	virtual void OnKilled(GEntityActor* pTarget) override {
		__super::OnKilled(pTarget);
		INCREASE_CALLCOUNTER; 
	}

	// ��밡 �ڽ��� ����Ʈ���̺��� �߰��Ͽ�����
	virtual void OnAddHateTarget( GEntityActor* pEnemy ) override {
		__super::OnAddHateTarget(pEnemy);
		INCREASE_CALLCOUNTER; 
	}
	// ��밡 �ڽ��� ����Ʈ���̺��� �����Ͽ�����
	virtual void OnLostHateTarget( MUID uidEnemy ) override {
		__super::OnLostHateTarget(uidEnemy);
		INCREASE_CALLCOUNTER; 
	}
	// �ڽ��� ����Ʈ���̺� Ÿ���� ���ŵǾ�����
	virtual void OnLostHateEntry(MUID nEnemyUID) override {
		__super::OnLostHateEntry(nEnemyUID);
		INCREASE_CALLCOUNTER; 
	}

	// �ǰ� �̺�Ʈ
	virtual void OnHit(GEntityActor* pOwner, GEntityActor* pVictim, GTalentInfo* pTalentInfo) override  { 
		__super::OnHit(pOwner, pVictim, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}

	// ���� ���������� �̺�Ʈ
	virtual void OnHitEnemy(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pTalentInfo) override { 
		__super::OnHitEnemy(pOwner, nCombatResultFalg, pTarget, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// �������� �ڽ��� �ǰݵ��� ��
	virtual void OnMagicHit(GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override { 
		__super::OnMagicHit(pAttacker, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// �������� �ڽ��� �������� ��
	virtual void OnMagicHitEnemy(GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override { 
		__super::OnMagicHitEnemy(pAttacker, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ���ٰ������� �ڽ��� �ǰݵ��� ��
	virtual void OnMeleeHit(GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override	{
		__super::OnMeleeHit(pAttacker, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ���ٰ������� �ڽ��� �������� ��
	virtual void OnMeleeHitEnemy(GEntityActor* pTarget, GTalentInfo* pTalentInfo) override {
		__super::OnMeleeHitEnemy(pTarget, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ġ��Ÿ �ǰ� �̺�Ʈ
	virtual void OnCriHit( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo ) override {
		__super::OnCriHit(pOwner, pAttacker, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ���� ġ��Ÿ�� ���������� �̺�Ʈ
	virtual void OnCriHitEnemy( GEntityActor* pOwner, GEntityActor* pTarget, GTalentInfo* pTalentInfo ) override {
		__super::OnCriHitEnemy(pOwner, pTarget, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ������ �̺�Ʈ
	virtual void OnMiss(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo) override { 
		__super::OnMiss(pOwner, nCombatResultFalg, pAttacker, pAttackTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ���� ������ ȸ�������� �̺�Ʈ
	virtual void OnMissEnemy(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pAttackTalentInfo) override { 
		__super::OnMissEnemy(pOwner, nCombatResultFalg, pTarget, pAttackTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ��� �̺�Ʈ
	virtual void OnGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo) override { 
		__super::OnGuard(pOwner, pAttacker, pAttackTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ���� ������ �������� �̺�Ʈ
	virtual void OnGuardEnemy(GEntityActor* pOwner, GEntityActor* pTarget, GTalentInfo* pAttackTalentInfo) override { 
		__super::OnGuardEnemy(pOwner, pTarget, pAttackTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ������ ������� �̺�Ʈ
	virtual void OnGainBuff(int nBuffID) override { 
		__super::OnGainBuff(nBuffID);
		INCREASE_CALLCOUNTER; 
	}
	// ������ �Ҿ����� �̺�Ʈ
	virtual void OnLostBuff(int nBuffID) override { 
		__super::OnLostBuff(nBuffID);
		INCREASE_CALLCOUNTER; 
	}
	// �ŷ�Ʈ ����� ���۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnUseTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo ) override {
		__super::OnUseTalent(pUser, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// �ŷ�Ʈ ����� ȿ���� �ߵ��ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnActTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo ) override {
		__super::OnActTalent(pUser, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ���� �ŷ�Ʈ ����� ȿ���� �ߵ��ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnMagicActTalent( GTalentInfo* pTalentInfo ) override {
		__super::OnMagicActTalent(pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// �ŷ�Ʈ ����� �Ϸ������ ȣ��Ǵ� �̺�Ʈ
	virtual void OnFinishTalent(GTalentInfo* pTalentInfo) override {
		__super::OnFinishTalent(pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// �ŷ�Ʈ�� ��ȿ�������� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTalentImmuned(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override {
		__super::OnTalentImmuned(pOwner, pAttacker, pTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// �������� ��۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTogglePrecision() override {
		__super::OnTogglePrecision();
		INCREASE_CALLCOUNTER; 
	}
	// �������� ��۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnToggleSneak(int nSneakBuffType) override {
		__super::OnToggleSneak(nSneakBuffType);
		INCREASE_CALLCOUNTER; 
	}
	// ������ ����ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnApplyAdvent() override {
		__super::OnApplyAdvent();
		INCREASE_CALLCOUNTER; 
	}
	// ������ �޾����� �̺�Ʈ
	virtual void OnRewarded(int nNPCID) override {
		__super::OnRewarded(nNPCID);
		INCREASE_CALLCOUNTER; 
	}
};


// - ������ �̺�Ʈ�� ����� �������� �׽�Ʈ
// - ���Ϳ� ���� ���Ϳ����� ���� �̺�Ʈ�� ����� ������ �׽�Ʈ
SUITE(EntityActor_Event)
{
	struct FEntityActor : public FBasePlayer
	{
		FEntityActor()
		{ 
			m_pNPCInfo = new GNPCInfo;
			m_pNPCInfo->nID = 1;
			gmgr.pNPCInfoMgr->Insert(m_pNPCInfo);

			m_pField = GUTHelper_Field::DefaultMockField();
			test.actor.Init_ForActor<MockEntityActor_ForEvent>(m_pField, m_pTester);

			m_pTester->AttachObserver(&m_ActorObserver);

			gsys.pServer->FrameCounterClear();
		}

		~FEntityActor() 
		{
			m_pTester->DetachObserver(&m_ActorObserver);
			m_pField->Destroy();
		}

		GTalentInfo* CreateTestTalent_BuffPassiveExtra(int nBuffID, float fDurationTime, BUFF_PASSIVE_EXTRA_ATTRIB_ eAttrib)
		{
			GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
			pBuff->m_fDuration = fDurationTime;
			pBuff->m_nPassiveExtraAttrib = eAttrib;
			GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);

			return pBuffTalent;
		}

		MockField*					m_pField;
		MockEntityActor_ForEvent*	m_pTester;
		MockActorObserver_ForEvent	m_ActorObserver;
		GNPCInfo*		m_pNPCInfo;

		DECLWRAPPER_Field;
		DECLWRAPPER_FakeSystem;
		DECLWRAPPER_SimpleCombatCalc;
		DECLWRAPPER_NPCInfoMgr;
	};

	TEST_FIXTURE(FEntityActor, OnCombatBegin)
	{
		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnCombatBegin"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnCombatBegin"));
		m_pTester->OnCombatBegin(m_pTester);	// ����: ���Ϳ����� ���� ���¸� ó���� �� ���� �̸� ����
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnCombatBegin"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnCombatBegin"));
	}
	
	TEST_FIXTURE(FEntityActor, OnCombatEnd)
	{
		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnCombatEnd"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnCombatEnd"));
		m_pTester->OnCombatEnd(m_pTester);	// ����: ���Ϳ����� ���� ���¸� ó���� �� ���� �̸� ����
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnCombatEnd"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnCombatEnd"));
	}

	TEST_FIXTURE(FEntityActor, OnDie)
	{
		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnDie"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnDie"));
		m_pTester->doDie();
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnDie"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnDie"));
	}

	TEST_FIXTURE(FEntityActor, OnAddHateTarget)
	{
		GEntityNPC* pNPC = m_pField->SpawnTestNPC(m_pNPCInfo);
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnAddHateTarget"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnAddHateTarget"));
		test.talent.UseTalent(m_pTester, p1DmgTalent, pNPC->GetUID());
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnAddHateTarget"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnAddHateTarget"));
	}

	TEST_FIXTURE(FEntityActor, OnLostHateTarget)
	{
		GEntityNPC* pNPC = m_pField->SpawnTestNPC(m_pNPCInfo);
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		test.talent.UseTalent(m_pTester, p1DmgTalent, pNPC->GetUID());
		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnLostHateTarget"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnLostHateTarget"));
		pNPC->doDie();
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnLostHateTarget"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnLostHateTarget"));
	}


	TEST_FIXTURE(FEntityActor, OnAddHateEntry)
	{
		class MockNPC : public GEntityNPC
		{
		public:			
			MOCK_METHOD1(OnAddHateEntry, void(GEntityActor* pEnemy));
		};

		MockNPC* pNPC = test.npc.NewMockNPC<MockNPC>(m_pField);
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		EXPECT_CALL(*pNPC, OnAddHateEntry(m_pTester)).Times(1);

		test.talent.UseTalent(m_pTester, p1DmgTalent, pNPC->GetUID());
	}

	TEST_FIXTURE(FEntityActor, OnAddHateEntryMember)
	{
		class MockNPC : public GEntityNPC
		{
		public:			
			MOCK_METHOD1(OnAddHateEntryMember, void(GEntityActor* pEnemyMember));
		};

		MockNPC* pNPC = test.npc.NewMockNPC<MockNPC>(m_pField);
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		GEntityPlayer* pPlayer = test.player.NewPlayer(m_pField);
		GEntityPlayer* pMember = test.player.NewPlayer(m_pField);
		test.party.CreateParty(pPlayer, pMember);

		EXPECT_CALL(*pNPC, OnAddHateEntryMember(pMember)).Times(1);

		test.talent.UseTalent(pPlayer, p1DmgTalent, pNPC->GetUID());
	}

	TEST_FIXTURE(FEntityActor, OnEmptyHateTable)
	{
		class MockNPC : public GEntityNPC
		{
		public:			
			MOCK_METHOD0(OnEmptyHateTable, void());
		};

		MockNPC* pNPC = test.npc.NewMockNPC<MockNPC>(m_pField);
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		EXPECT_CALL(*pNPC, OnEmptyHateTable()).Times(1);

		test.talent.UseTalent(m_pTester, p1DmgTalent, pNPC->GetUID());
		pNPC->GetHateTable().DropPoint(m_pTester->GetUID());
	}


	TEST_FIXTURE(FEntityActor, OnHit)
	{
		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, m_pTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnHit"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnHit"));
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pTester->GetUID());
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnHit"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnHit"));
	}

	TEST_FIXTURE(FEntityActor, OnHitEnemy)
	{
		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, m_pTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnHitEnemy"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnHitEnemy"));
		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnUseTalent"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnUseTalent"));
		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnActTalent"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnActTalent"));
		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnFinishTalent"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnFinishTalent"));
		test.talent.UseTalent(m_pTester, p1DmgTalent, pDefender->GetUID());
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnHitEnemy"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnHitEnemy"));
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnUseTalent"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnUseTalent"));
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnActTalent"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnActTalent"));
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnFinishTalent"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnFinishTalent"));
	}

	TEST_FIXTURE(FEntityActor, OnCriHit)
	{
		DECLWRAPPER_CriticalCombatCalc;

		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, m_pTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnHit"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnHit"));
		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnCriHit"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnCriHit"));
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pTester->GetUID());
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnHit"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnHit"));
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnCriHit"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnCriHit"));
	}

	TEST_FIXTURE(FEntityActor, OnCriHitEnemy)
	{
		DECLWRAPPER_CriticalCombatCalc;

		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, m_pTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnHitEnemy"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnHitEnemy"));
		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnFinishTalent"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnFinishTalent"));
		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnCriHitEnemy"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnCriHitEnemy"));
		test.talent.UseTalent(m_pTester, p1DmgTalent, pDefender->GetUID());
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnHitEnemy"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnHitEnemy"));
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnFinishTalent"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnFinishTalent"));
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnCriHitEnemy"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnCriHitEnemy"));
	}

	TEST_FIXTURE(FEntityActor, OnMiss)
	{
		DECLWRAPPER_MissCombatCalc;

		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, m_pTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnMiss"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnMiss"));
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pTester->GetUID());
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnMiss"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnMiss"));
	}

	TEST_FIXTURE(FEntityActor, OnMissEnemy)
	{
		DECLWRAPPER_MissCombatCalc;

		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, m_pTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnMissEnemy"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnMissEnemy"));
		test.talent.UseTalent(m_pTester, p1DmgTalent, pDefender->GetUID());
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnMissEnemy"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnMissEnemy"));
	}

	TEST_FIXTURE(FEntityActor, OnGuard)
	{
		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, m_pTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		test.talent.Guard(m_pTester);

		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnGuard"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnGuard"));
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pTester->GetUID());
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnGuard"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnGuard"));
	}

	TEST_FIXTURE(FEntityActor, OnGuardEnemy)
	{
		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, m_pTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		test.talent.Guard(pDefender);

		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnGuardEnemy"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnGuardEnemy"));
		test.talent.UseTalent(m_pTester, p1DmgTalent, pDefender->GetUID());
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnGuardEnemy"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnGuardEnemy"));
	}

	TEST_FIXTURE(FEntityActor, OnGainBuff)
	{
		float fDuration = 100.0f;
		GBuffInfo* pBuff=test.buff.NewBuffInfo();
		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnGainBuff"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnGainBuff"));
		test.buff.GainBuffSelf(m_pTester, pBuff, fDuration);
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnGainBuff"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnGainBuff"));
	}

	TEST_FIXTURE(FEntityActor, OnLostBuff)
	{
		float fDuration = 100.0f;
		GBuffInfo* pBuff=test.buff.NewBuffInfo();
		test.buff.GainBuffSelf(m_pTester, pBuff, fDuration);

		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnLostBuff"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnLostBuff"));
		// ���� ����
		m_pField->Update(fDuration);
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnLostBuff"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnLostBuff"));
	}

	TEST_FIXTURE(FEntityActor, OnTalentImmuned)
	{
		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, m_pTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);

		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnTalentImmuned"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnTalentImmuned"));
		GTalentInfo* pBuffTalent = CreateTestTalent_BuffPassiveExtra(123, BUFF_DURATION_INFINITY, BUFPEA_INVINCIBILITY);
		// ���� �ɸ� ���¿��� ���ݹޱ�
		test.talent.UseTalent(m_pTester, pBuffTalent, m_pTester->GetUID());
		CHECK_EQUAL(true, m_pTester->GetModuleBuff()->IsNowInvincibility());
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pTester->GetUID());
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnTalentImmuned"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnTalentImmuned"));
	}

	TEST_FIXTURE(FEntityActor, OnTogglePrecision)
	{
		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnTogglePrecision"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnTogglePrecision"));
		test.focus.TogglePrecision(m_pTester);
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnTogglePrecision"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnTogglePrecision"));
	}

	TEST_FIXTURE(FEntityActor, OnToggleSneak)
	{
		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnToggleSneak"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnToggleSneak"));
		test.focus.ToggleSneak(m_pTester);
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnToggleSneak"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnToggleSneak"));
	}

	TEST_FIXTURE(FEntityActor, OnApplyAdvent)
	{
		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnApplyAdvent"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnApplyAdvent"));
		test.focus.ToggleAdvent(m_pTester);
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnApplyAdvent"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnApplyAdvent"));
	}

	TEST_FIXTURE(FEntityActor, OnKilled)
	{
		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnKilled"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnKilled"));
		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, m_pTester->GetPos()+vec3(10,10,0));
		pAttacker->SetKiller(m_pTester->GetUID());
		pAttacker->doDie();
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnKilled"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnKilled"));
	}

	TEST_FIXTURE(FEntityActor, OnMagicActTalent)
	{
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		p1DmgTalent->m_nCategory = TC_MAGIC;

		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnMagicActTalent"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnMagicActTalent"));
		test.talent.UseTalent(m_pTester, p1DmgTalent, m_pTester->GetUID());
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnMagicActTalent"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnMagicActTalent"));
	}

	TEST_FIXTURE(FEntityActor, OnMagicHit)
	{
		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, m_pTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		p1DmgTalent->m_nCategory = TC_MAGIC;

		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnMagicHit"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnMagicHit"));
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pTester->GetUID());
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnMagicHit"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnMagicHit"));
	}

	TEST_FIXTURE(FEntityActor, OnMagicHitEnemy)
	{
		GEntityPlayer* pVictim = NewEntityPlayer(m_pField, m_pTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		p1DmgTalent->m_nCategory = TC_MAGIC;

		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnMagicHitEnemy"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnMagicHitEnemy"));
		test.talent.UseTalent(m_pTester, p1DmgTalent, pVictim->GetUID());
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnMagicHitEnemy"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnMagicHitEnemy"));
	}

	TEST_FIXTURE(FEntityActor, OnMeleeHit)
	{
		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, m_pTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		p1DmgTalent->m_nCategory = TC_MELEE;

		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnMeleeHit"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnMeleeHit"));
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pTester->GetUID());
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnMeleeHit"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnMeleeHit"));
	}

	TEST_FIXTURE(FEntityActor, OnMeleeHitEnemy)
	{
		GEntityPlayer* pVictim = NewEntityPlayer(m_pField, m_pTester->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		p1DmgTalent->m_nCategory = TC_MELEE;

		CHECK_EQUAL(0, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnMeleeHitEnemy"));
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnMeleeHitEnemy"));
		test.talent.UseTalent(m_pTester, p1DmgTalent, pVictim->GetUID());
		CHECK_EQUAL(1, m_pTester->GetCallCounter(L"MockEntityActor_ForEvent::OnMeleeHitEnemy"));
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent::OnMeleeHitEnemy"));
	}
}