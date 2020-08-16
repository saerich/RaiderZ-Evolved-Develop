#include "stdafx.h"
#include "GMailItemPutDowner.h"
#include "CTransData.h"
#include "GItemHolder.h"
#include "GEntityPlayer.h"
#include "GPlayerMailReserver.h"
#include "GInteractionSystem.h"
#include "GGlobal.h"
#include "GNPCInteractor.h"
#include "GNPCInteractionInfo.h"

GMailItemPutDowner::GMailItemPutDowner()
: m_nLastFailure(CR_FAIL)
{
	// do nothing
}

GMailItemPutDowner::~GMailItemPutDowner()
{
	// do nothing
}

bool GMailItemPutDowner::Do(GEntityPlayer* pPlayer, const TD_MAIL_RESERVE_ITEM* pReserveItem)
{
	VALID_RET(pPlayer != NULL, false);
	VALID_RET(pReserveItem != NULL, false);


	// ������ Ȯ��
	int nSlotID = pReserveItem->nInventorySlotID;
	int nAmount = pReserveItem->nItemAmount;

	GItemHolder* pItemHolder =  pPlayer->GetItemHolder();
	GItem* pItem = pItemHolder->GetItem(SLOTTYPE_INVENTORY, nSlotID);

	if (pItem == NULL)
	{		
		m_nLastFailure = CR_FAIL_MAIL_APPEND_ITEM_NO_INVENTORY;
		return false;
	}


	// ÷�ε� ���������� Ȯ���Ѵ�.
	if (!Check(pPlayer, pItem, nAmount))
	{
		return false;
	}


	// ÷�� ��Ͽ� �����Ѵ�.
	GPlayerMailReserver& mailReserver = pPlayer->GetMailReserver();		
	bool nResult = mailReserver.PutDownItem(nSlotID, nAmount);

	return nResult;
}

bool GMailItemPutDowner::Check(GEntityPlayer* pPlayer, const GItem* pItem, int nSubAmount)
{
	VALID_RET(pPlayer != NULL, false);
	VALID_RET(pItem != NULL, false);


	// ���� ���ͷ��� ���¸� ��Ȯ���Ѵ�.
	if (!CheckMailInteraction(pPlayer))
	{
		return false;
	}

	// ���� �� �ִ� ������ ���� �ִ��� Ȯ���Ѵ�.
	if (!CheckItemAmount(pPlayer, pItem, nSubAmount))
	{
		return false;
	}


	return true;
}

bool GMailItemPutDowner::CheckMailInteraction(GEntityPlayer* pPlayer)
{
	VALID_RET(pPlayer != NULL, false);

	// ��ü��� ���ͷ��� ���̾�� �Ѵ�.
	GNPCInteractor& interactor = gsys.pInteractionSystem->GetNPCInteractor();

	if (!interactor.CheckIProgress(pPlayer, IT_MAILBOX))
	{
		m_nLastFailure = CR_FAIL_MAIL_APPEND_ITEM_NOT_INTERATION;
		return false;
	}

	return true;
}

bool GMailItemPutDowner::CheckItemAmount(GEntityPlayer* pPlayer, const GItem* pItem, int nSubAmount)
{
	VALID_RET(pPlayer != NULL, false);
	VALID_RET(pItem != NULL, false);
	VALID_RET(0 < nSubAmount, false);

	// �������� ÷�ε� ������ŭ�� ���� �� �ִ�.
	int nSlotID = pItem->m_nSlotID;	

	GPlayerMailReserver& reserver = pPlayer->GetMailReserver();
	int nReservedAmount = reserver.GetAmount(nSlotID);

	if (nReservedAmount - nSubAmount < 0)
	{
		m_nLastFailure = CR_FAIL_MAIL_APPEND_ITEM_NOT_ENOUGH_AMOUNT_DOWN;
		return false;
	}

	return true;
}

CCommandResultTable GMailItemPutDowner::GetLastFailure(void)
{
	return m_nLastFailure;
}
