#include "StdAfx.h"
#include "GTalentFocusMgr.h"
#include "GTalentFocus_Counter.h"
#include "GTalentFocus_Berserk.h"
#include "GTalentFocus_Precision.h"
#include "GTalentFocus_Enlighten.h"
#include "GTalentFocus_Advent.h"
#include "GTalentFocus_Sneak.h"
#include "GEntityPlayer.h"
#include "GCommandTable.h"
#include "GConst.h"
#include "GCommand.h"

GTalentFocusMgr::GTalentFocusMgr(GEntityPlayer* pOwner)
: m_pOwner(pOwner)
, m_nLastBerserkBubble(0)
{
	memset(m_pFocusTable, NULL, sizeof(m_pFocusTable));

	m_pOwner->AttachObserver(this);
}

GTalentFocusMgr::~GTalentFocusMgr(void)
{
	m_pOwner->DetachObserver(this);
}

void GTalentFocusMgr::Init()
{
	m_pFocusTable[TFT_COUNTER]		= new GTalentFocus_Counter(m_pOwner);
	m_pFocusTable[TFT_BERSERK]		= new GTalentFocus_Berserk(m_pOwner);
	m_pFocusTable[TFT_PRECISION]	= new GTalentFocus_Precision(m_pOwner);
	m_pFocusTable[TFT_ENLIGHTEN]	= new GTalentFocus_Enlighten(m_pOwner);
	m_pFocusTable[TFT_ADVENT]		= new GTalentFocus_Advent(m_pOwner);
	m_pFocusTable[TFT_SNEAK]		= new GTalentFocus_Sneak(m_pOwner);
}

void GTalentFocusMgr::Fini()
{
	for (int i=0; i<TFT_SIZE; ++i)
	{
		SAFE_DELETE(m_pFocusTable[i]);
	}
}

void GTalentFocusMgr::Update( float fDelta )
{
	PFI_BLOCK_THISFUNC(9056);

	for (int i=0; i<TFT_SIZE; ++i)
	{
		GTalentFocus* pEachFocus = m_pFocusTable[i];
		if (!pEachFocus)	continue;

		bool bExpired = pEachFocus->Update(fDelta);
		if (bExpired)
		{
			LostFocus(pEachFocus->GetType());
		}
	}

	UpdateBubbleQty();
}

void GTalentFocusMgr::UpdateBubbleQty()
{
	PFI_BLOCK_THISFUNC(9057);

	int m_nBerserkBubble = GetBerserkBubble();

	// ���� ������ �ٲ������ Ŭ���̾�Ʈ���� �˷��ش�.
	if (m_nBerserkBubble != m_nLastBerserkBubble)
		Route_UpdateBubble(TFT_BERSERK, m_nBerserkBubble, GetBerserkMaxBubble());

	m_nLastBerserkBubble = m_nBerserkBubble;
}


int GTalentFocusMgr::GetBerserkBubble()
{
	GTalentFocus_Berserk* pBerserkFocus = GetBerserkFocus();
	VALID_RET(pBerserkFocus, 0);
	return pBerserkFocus->GetHitCount();
}

int GTalentFocusMgr::GetBerserkMaxBubble()
{
	GTalentFocus_Berserk* pBerserkFocus = GetBerserkFocus();
	VALID_RET(pBerserkFocus, 0);
	return pBerserkFocus->GetMaxHitCount();
}

void GTalentFocusMgr::CheckGainFocus(TALENT_FOCUS_TYPE nFocus, int nBuffType)
{
	// �ش� ��Ŀ���� �ٲ� �� �ִ� ������ �ȵȴٸ� ����
	GTalentFocus* pFocus = GetFocus(nFocus);
	VALID(pFocus);
	if (!pFocus->CheckActivate())
		return;

	if (!pFocus->CheckEquipItem())
		return;

	GainFocus(nFocus, nBuffType);
}

void GTalentFocusMgr::GainFocus( TALENT_FOCUS_TYPE nFocus, int nBuffType )
{
	VALID(nFocus > TFT_NONE);
	VALID(nFocus < TFT_SIZE);

	GTalentFocus* pCurFocus = GetFocus(nFocus);
	VALID(pCurFocus);

	if (!pCurFocus->CheckLicense())
		return; // ���̼����� ����� ����

	if (HasFocus(nFocus) == false)
	{
		m_setActiveFocus.insert(nFocus);
	}

	pCurFocus->SetBuffType(nBuffType);
	pCurFocus->Active();

	Route_GainFocus(nFocus, pCurFocus->GetDurationTime());
}

void GTalentFocusMgr::LostFocus(TALENT_FOCUS_TYPE nFocus)
{
	VALID(nFocus > TFT_NONE);
	VALID(nFocus < TFT_SIZE);

	if (!HasFocus(nFocus))
		return; // ���־��� ��Ŀ���� ���� ��� ����

	GTalentFocus* pCurFocus = GetFocus(nFocus);
	VALID(pCurFocus);
	pCurFocus->Deactive();

	m_setActiveFocus.erase(nFocus);

	Route_LostFocus(nFocus);
}

void GTalentFocusMgr::LostAllFocus()
{
	// set �����̳��� ���Ұ� ��ȯ�߿� �����ǹǷ�, ������ �� ó��
	vector<TALENT_FOCUS_TYPE> vecFocus(m_setActiveFocus.begin(), m_setActiveFocus.end());

	for each (TALENT_FOCUS_TYPE nFocus in vecFocus)
	{
		LostFocus(nFocus);
	}
}

bool GTalentFocusMgr::HasFocus()
{
	return !m_setActiveFocus.empty();
}

bool GTalentFocusMgr::HasFocus( TALENT_FOCUS_TYPE nFocus )
{
	return (m_setActiveFocus.find(nFocus) != m_setActiveFocus.end());
}

GTalentFocus* GTalentFocusMgr::GetFocus( TALENT_FOCUS_TYPE nFocus )
{
	VALID_RET(nFocus > TFT_NONE, NULL);
	VALID_RET(nFocus < TFT_SIZE, NULL);
	GTalentFocus* pFocus = m_pFocusTable[nFocus];
	VALID_RET(pFocus, NULL);
	return pFocus;
}

GTalentFocus_Counter* GTalentFocusMgr::GetCounterFocus()
{
	return static_cast<GTalentFocus_Counter*>(GetFocus(TFT_COUNTER));
}

GTalentFocus_Berserk* GTalentFocusMgr::GetBerserkFocus()
{
	return static_cast<GTalentFocus_Berserk*>(GetFocus(TFT_BERSERK));
}

GTalentFocus_Precision* GTalentFocusMgr::GetPrecisionFocus()
{
	return static_cast<GTalentFocus_Precision*>(GetFocus(TFT_PRECISION));
}

GTalentFocus_Enlighten* GTalentFocusMgr::GetEnlightenFocus()
{
	return static_cast<GTalentFocus_Enlighten*>(GetFocus(TFT_ENLIGHTEN));
}	

GTalentFocus_Sneak* GTalentFocusMgr::GetSneakFocus()
{
	return static_cast<GTalentFocus_Sneak*>(GetFocus(TFT_SNEAK));
}

GTalentFocus_Advent* GTalentFocusMgr::GetAdventFocus()
{
	return static_cast<GTalentFocus_Advent*>(GetFocus(TFT_ADVENT));
}

void GTalentFocusMgr::Route_GainFocus(TALENT_FOCUS_TYPE nFocus, float fDurationTime)
{
	VALID(m_pOwner);

	MCommand* pNewCommand = MakeNewCommand(MC_FOCUS_GAIN, 3, 
		NEW_UID(m_pOwner->GetUID()), 
		NEW_INT(nFocus),
		NEW_FLOAT(fDurationTime));
	m_pOwner->RouteToThisCell(pNewCommand);
}

void GTalentFocusMgr::Route_LostFocus(TALENT_FOCUS_TYPE nFocus)
{
	VALID(m_pOwner);

	MCommand* pNewCommand = MakeNewCommand(MC_FOCUS_LOST, 2, 
		NEW_UID(m_pOwner->GetUID()), NEW_INT(nFocus));
	m_pOwner->RouteToThisCell(pNewCommand);
}

void GTalentFocusMgr::Route_UpdateBubble( TALENT_FOCUS_TYPE nFocus, uint8 nBubbleQty, uint8 nMaxBubbleQty )
{
	VALID(m_pOwner);

	uint8 nBubblePercentage = (uint8)(nBubbleQty*100/nMaxBubbleQty);

	MCommand* pNewCommand = MakeNewCommand(MC_FOCUS_UPDATE_BUBBLE, 2, 
		NEW_INT(nFocus), NEW_UCHAR(nBubblePercentage));
	m_pOwner->RouteToMe(pNewCommand);
}

void GTalentFocusMgr::ToggleFocus(TALENT_FOCUS_TYPE nFocus, int nBuffType)
{
	if (HasFocus(nFocus))
	{
		LostFocus(nFocus);
	}
	else
	{
		CheckGainFocus(nFocus, nBuffType);
	}
}

void GTalentFocusMgr::OnGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo)
{
	GTalentFocus_Counter* pCounterFocus = GetCounterFocus();
	pCounterFocus->OnGuard(pAttacker);
	CheckGainFocus(pCounterFocus->GetType());
}

void GTalentFocusMgr::OnAbsoluteGuard(GEntityActor* pOwner,  GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo )
{
	GTalentFocus_Counter* pCounterFocus = GetCounterFocus();
	pCounterFocus->OnAbsoluteGuard(pAttacker);
	CheckGainFocus(pCounterFocus->GetType());
}

void GTalentFocusMgr::OnHitAll( GEntityActor* pOwner, GTalentInfo* pTalentInfo )
{
	VALID(pTalentInfo);

	// �ǰ� �̺�Ʈ�� ��Ŀ�� ��� ó��
	if (pTalentInfo->m_nTiming == TC_HIT_ALL_TALENT &&
		pTalentInfo->HasNextFocus())
	{
		GainFocus(pTalentInfo->m_nNextFocus);
	}
}

void GTalentFocusMgr::OnHitEnemy( GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pAttacker, GTalentInfo* pTalentInfo )
{
	VALID(pTalentInfo);

	// �ǰ� �̺�Ʈ�� ��Ŀ�� ��� ó��
	if (pTalentInfo->m_nTiming == TC_HIT_TALENT &&
		pTalentInfo->HasNextFocus())
	{
		GainFocus(pTalentInfo->m_nNextFocus);
	}

	switch(pTalentInfo->m_nCategory)
	{
	case TC_MELEE:	OnHitMeleeTalent(pTalentInfo);		break;
	}
}

void GTalentFocusMgr::OnHitMeleeTalent(GTalentInfo* pHitTalentInfo)
{
	GTalentFocus_Berserk* pBerserkFocus = GetBerserkFocus();
	pBerserkFocus->OnHitMeleeTalent(pHitTalentInfo);
	CheckGainFocus(pBerserkFocus->GetType());
}

void GTalentFocusMgr::OnDie()
{
	LostAllFocus();
}

void GTalentFocusMgr::OnTogglePrecision()
{
	ToggleFocus(TFT_PRECISION);
}

void GTalentFocusMgr::OnToggleSneak(int nSneakBuffType)
{
	ToggleFocus(TFT_SNEAK, nSneakBuffType);
}

void GTalentFocusMgr::OnApplyAdvent()
{
	CheckGainFocus(TFT_ADVENT);
}

void GTalentFocusMgr::OnGainBuff( int nBuffID )
{
	// ���� ���� ó��
	if (!HasFocus(TFT_ENLIGHTEN))
	{
		GTalentFocus_Enlighten* pFocus = GetEnlightenFocus();
		VALID(pFocus);
		if (nBuffID != pFocus->GetFocusBuffID())
			return;

		if (!pFocus->CheckEquipItem())
			return;

		GainFocus(pFocus->GetType());
	}
}

void GTalentFocusMgr::OnLostBuff( int nBuffID )
{
	// ��Ŀ�� ���� ���� üũ
	for (int i=0; i<TFT_SIZE; i++)
	{
		if (nBuffID == m_pFocusTable[i]->GetFocusBuffID())
		{
			LostFocus((TALENT_FOCUS_TYPE)i);
			return;
		}
	}
}

void GTalentFocusMgr::OnUseTalent(GEntityActor* pUser, GTalentInfo* pTalentInfo)
{
	VALID(pTalentInfo);

	PFI_B(3700, "GTalentFocusMgr::OnUseTalent");

	// �ŷ�Ʈ ����� ���۵Ǵ� �̺�Ʈ�� ��Ŀ�� ��� ó��
	if (pTalentInfo->m_nTiming == TC_USE_TALENT &&
		pTalentInfo->HasNextFocus())
	{
		GainFocus(pTalentInfo->m_nNextFocus);
	}

	PFI_E(3700);
}

void GTalentFocusMgr::OnFinishTalent( GTalentInfo* pTalentInfo )
{
	VALID(pTalentInfo);

	// �ŷ�Ʈ��� �̺�Ʈ�� ��Ŀ�� ��� ó��
	if (pTalentInfo->m_nTiming == TC_FINISH_TALENT &&
		pTalentInfo->HasNextFocus())
	{
		GTalentFocus* pFocus = GetFocus(pTalentInfo->m_nNextFocus);
		VALID(pFocus);
		if (!pFocus->CheckEquipItem())
			return;

		GainFocus(pFocus->GetType());
	}
}

void GTalentFocusMgr::OnActTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo )
{
	VALID(pTalentInfo);

	// ��Ŀ���� ��ȸ���̸� ������Ŀ���� ���� �ŷ�Ʈ�ϰ��, ����� �����Ѵ�.
	if (pTalentInfo->HasRequireFocus() &&
		!pTalentInfo->HasNextFocus())
	{
		GTalentFocus* pFocus = GetFocus(pTalentInfo->m_nRequireFocus);
		VALID(pFocus);
		if (pFocus->IsOnce())
		{
			LostFocus(pFocus->GetType());
		}
	}
}

void GTalentFocusMgr::OnItemEquipped( GItem* pItem )
{
	CheckEquipItemForAllFocus();
}

void GTalentFocusMgr::OnItemUnequipped( GItem* pItem )
{
	CheckEquipItemForAllFocus();
}

void GTalentFocusMgr::OnSwitchingWeaponSet( SH_ITEM_SWITCH_WEAPON val )
{
	CheckEquipItemForAllFocus();
}

void GTalentFocusMgr::OnInteraction()
{
	if (true == HasFocus(TFT_SNEAK))
	{
		LostFocus(TFT_SNEAK);
	}
}

void GTalentFocusMgr::CheckEquipItemForAllFocus()
{
	if (m_setActiveFocus.empty())
		return;

	vector<TALENT_FOCUS_TYPE> vecLostFocuses;
	for each (TALENT_FOCUS_TYPE nFocus in m_setActiveFocus)
	{
		GTalentFocus* pFocus = GetFocus(nFocus);
		DCHECK(pFocus);
		if (!pFocus->CheckEquipItem())
		{
			vecLostFocuses.push_back(nFocus);
		}
	}

	for each (TALENT_FOCUS_TYPE nFocus in vecLostFocuses)
	{
		LostFocus(nFocus);
	}
}

