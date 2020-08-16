#pragma once

#include "GActorObserver.h"
#include "CSDef.h"

class GEntityPlayer;
class GTalentFocus;
class GTalentFocus_Counter;
class GTalentFocus_Berserk;
class GTalentFocus_Precision;
class GTalentFocus_Enlighten;
class GTalentFocus_Sneak;
class GTalentFocus_Advent;
class GItem;

class GTalentFocusMgr : private GActorObserver, public MTestMemPool<GTalentFocusMgr>
{
public:
	GTalentFocusMgr(GEntityPlayer* pOwner);
	~GTalentFocusMgr();

	// �Ŵ��� �ʱ�ȭ
	void Init();
	// �Ŵ��� ����������
	void Fini();
	// ��ƽ���� ȣ���
	void Update(float fDelta);
	
	
	// �ڽ��� �׾����� ȣ��Ǵ� �̺�Ʈ
	void OnDie();
	// �������� ��۵ɶ� ȣ��Ǵ� �̺�Ʈ
	void OnTogglePrecision();
	// �������� ��۵ɶ� ȣ��Ǵ� �̺�Ʈ
	void OnToggleSneak(int nSneakBuffType);
	// ������ ����ɶ� ȣ��Ǵ� �̺�Ʈ
	void OnApplyAdvent();

	// ��Ŀ���� �ϳ��� ������ �ִ��� ���θ� ��ȯ
	bool HasFocus();
	// Ư�� ��Ŀ���� ������ �ִ��� ���θ� ��ȯ
	bool HasFocus(TALENT_FOCUS_TYPE nFocus);
	// ���� ���� ���� ��ȯ
	int GetBerserkBubble();
	// ���� �ִ� ���� ���� ��ȯ
	int GetBerserkMaxBubble();
	// �ִ� ��Ŀ�� ���� ��ȯ
	int GetMaxFocusQty();

	// ���ͷ��� ������
	void OnInteraction();

private:
	// ��� �ǰ� ���� �̺�Ʈ (��Ʈĸ������ �뵵)
	virtual void OnHitAll( GEntityActor* pOwner, GTalentInfo* pTalentInfo ) override;
	// ���� ���������� �̺�Ʈ
	virtual void OnHitEnemy(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override;
	// ��� �̺�Ʈ
	virtual void OnGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo) override;
	// ���� ��� �̺�Ʈ
	virtual void OnAbsoluteGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo) override;
	// ������ ������� �̺�Ʈ
	virtual void OnGainBuff(int nBuffID) override;
	// ������ �Ҿ����� �̺�Ʈ
	virtual void OnLostBuff(int nBuffID) override;
	// ������ ���� (Player ����)
	virtual void OnItemEquipped(GItem* pItem) override;
	// ������ �������� (Player ����)
	virtual void OnItemUnequipped(GItem* pItem) override;
	// ���� ���� (Player ����)
	virtual void OnSwitchingWeaponSet(SH_ITEM_SWITCH_WEAPON val) override;


	// �ŷ�Ʈ ����� ���۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnUseTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo ) override;
	// �ŷ�Ʈ ����� �Ϸ������ ȣ��Ǵ� �̺�Ʈ
	virtual void OnFinishTalent(GTalentInfo* pTalentInfo) override;
	// �ŷ�Ʈ ����� ȿ���� �ߵ��ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnActTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo ) override;


	// ��Ŀ���� �ٲ� �� �ִ��� üũ�غ� ��, �ٲ� ������ �Ǹ� ��Ŀ���� �ٲ۴�.
	void CheckGainFocus(TALENT_FOCUS_TYPE nFocus, int nBuffType=INVALID_BUFF_ID);
	// ��Ŀ���� ��´�.
	void GainFocus(TALENT_FOCUS_TYPE nFocus, int nBuffType=INVALID_BUFF_ID);
	// Ư�� ��Ŀ���� �Ҵ´�.
	void LostFocus(TALENT_FOCUS_TYPE nFocus);
	// ��� ��Ŀ���� �Ҵ´�.
	void LostAllFocus();
	// ���� ������ ���߽������� ȣ��
	void OnHitMeleeTalent(GTalentInfo* pHitTalentInfo);
	// �ش� ��Ŀ���� ��� ��Ŵ
	void ToggleFocus(TALENT_FOCUS_TYPE nFocus, int nBuffType=INVALID_BUFF_ID);
	// ����� ���� ���� ó��
	void UpdateBubbleQty();

	// Ư�� ��Ŀ����ü�� ��ȯ
	GTalentFocus*			GetFocus(TALENT_FOCUS_TYPE nFocus);
	// �ݰ� ��Ŀ����ü�� ��ȯ
	GTalentFocus_Counter*	GetCounterFocus();
	// ���� ��Ŀ����ü�� ��ȯ
	GTalentFocus_Berserk*	GetBerserkFocus();
	// ���� ��Ŀ����ü�� ��ȯ
	GTalentFocus_Precision*	GetPrecisionFocus();
	// ���� ��Ŀ����ü�� ��ȯ
	GTalentFocus_Enlighten*	GetEnlightenFocus();
	// ���� ��Ŀ����ü�� ��ȯ
	GTalentFocus_Sneak*		GetSneakFocus();
	// ���� ��Ŀ����ü�� ��ȯ
	GTalentFocus_Advent*	GetAdventFocus();

	// ��� Ȱ��ȭ�� ��Ŀ���� ������ ������ �´��� �˻���, �߸��ƴٸ� ��Ŀ������
	void CheckEquipItemForAllFocus();

	// ��Ŀ�� ����ٰ� Ŭ�󿡰� �˷���
	void Route_GainFocus(TALENT_FOCUS_TYPE nFocus, float fDurationTime);
	// ��Ŀ�� �Ҿ��ٰ� Ŭ�󿡰� �˷���
	void Route_LostFocus(TALENT_FOCUS_TYPE nFocus);
	// ��Ŀ�� ������ ������ ���ŵɶ� �˷���
	void Route_UpdateBubble(TALENT_FOCUS_TYPE nFocus, uint8 nBubbleQty, uint8 nMaxBubbleQty);	
private:
	// ��Ŀ���Ŵ��� ������
	GEntityPlayer*			m_pOwner;
	// Ȱ��ȭ�� ��Ŀ��Ÿ�� ���
	set<TALENT_FOCUS_TYPE>	m_setActiveFocus;
	// ��Ŀ����ü ���̺�
	GTalentFocus*			m_pFocusTable[TFT_SIZE];
	// ���� ƽ�� ���� ���� ��
	int						m_nLastBerserkBubble;
};
