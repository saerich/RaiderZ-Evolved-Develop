#include "StdAfx.h"
#include "GMailItemTaker.h"
#include "GPlayerMailBox.h"
#include "GEntityPlayer.h"
#include "GClientMailRouter.h"
#include "GMail.h"
#include "GGlobal.h"
#include "GItemSystem.h"
#include "GItemAdder.h"
#include "GDBManager.h"
#include "GItemHolder.h"
#include "GDBTaskData.h"


GMailItemTaker::GMailItemTaker(void)
: m_pClientRouter(NULL)
{
}

GMailItemTaker::~GMailItemTaker(void)
{
}

void GMailItemTaker::SetClientRouter(GClientMailRouter* pNewClientMailRouter)
{
	m_pClientRouter = pNewClientMailRouter;
}

bool GMailItemTaker::Take(GEntityPlayer* pPlayer, MUID uidMail, int nMailSlotID)
{
	VALID_RET(pPlayer != NULL, false);

	GPlayerMailBox& refMailbox = pPlayer->GetMailbox();

	GMail* pMail = refMailbox.GetMail(uidMail.Value);
	if (pMail == NULL) 	return false;

	GItem* pItem = pMail->GetAppendedItem(nMailSlotID);
	if (pItem == NULL) return false;
	VALID_RET(pItem->m_pItemData, false);

	if (!Check(pPlayer, pMail, pItem)) return false;
	if (!Apply(pPlayer, pMail, pItem)) return false;

	return true;
}

bool GMailItemTaker::Check(GEntityPlayer* pPlayer, GMail* pMail, GItem* pItem)
{
	VALID_RET(pPlayer != NULL, false);
	VALID_RET(pMail != NULL, false);
	VALID_RET(pItem != NULL, false);

	/// �κ��丮�� ��á���� �˻�. - ÷�ξ����� ����� ��ġ�� ���� ������� ����.
	if (pPlayer->GetItemHolder()->IsFull(SLOTTYPE_INVENTORY))
		return pPlayer->FailAndRouteAdviceMsg(CR_FAIL_SYSTEM_FEW_INVENTORY);

	/// �� �ۿ� �� ���� �޽����� ItemAdder ���� üũ, �˸�.
	if (!gsys.pItemSystem->GetAdder().CheckItemAddable(pPlayer, CHECK_ADDABLE_ITEM(pItem->m_pItemData->m_nID, pItem->GetAmount())))
		return false;

	return true;
}

bool GMailItemTaker::Apply(GEntityPlayer* pPlayer, GMail* pMail, GItem* pItem)
{
	VALID_RET(pPlayer != NULL, false);
	VALID_RET(pMail != NULL, false);
	VALID_RET(pItem != NULL, false);

	/// ���� ����
	int nReservedSlotID = pPlayer->GetItemHolder()->ReserveEmptySlot(SLOTTYPE_INVENTORY, 0);
	if (nReservedSlotID == INVALID_ID)
		return false;
	
	/// ������ ���ο� Thumbnail ���������� ����� ������ ID�� ã�´�.
	int nNewMailThumbnailItemID = pMail->GetNewThumbnailItemID(pItem->m_nSlotID);
	
	GDBT_MAIL_GET_ITEM data;
	data.m_uidPlayer			= pPlayer->GetUID();
	data.m_nAID					= pPlayer->GetAID();
	data.m_nCID					= pPlayer->GetCID();
	data.m_nMailUID				= pMail->GetUID();
	data.m_nMailSlotID			= pItem->m_nSlotID;
	data.m_nIUID				= pItem->m_nIUID;
	data.m_nItemID				= pItem->m_pItemData->m_nID;
	data.m_InvenSlotID			= nReservedSlotID;
	data.m_nNewMailDefaultItemID= nNewMailThumbnailItemID;
	data.m_nCharPlayTime		= pPlayer->GetPlayerInfo()->nPlayTimeSec;
	data.m_nDeltaCharPtm		= pPlayer->GetPlayerInfo()->GetDeltaPlayTime();
	data.m_nLevel				= pPlayer->GetLevel();
	data.m_nModStackAmt			= pItem->GetAmount();
	data.m_nMoney				= pPlayer->GetMoney();

	if (!gsys.pDBManager->MailGetAppendedItem(data))
		return false;

	return true;
}

bool GMailItemTaker::DelTakenItemForDBTask(GEntityPlayer* pPlayer, int64 nMailUID, int nMailSlotID, int nNewMailThumbnailItemID)
{
	VALID_RET(pPlayer != NULL, false);

	GPlayerMailBox& refMailbox = pPlayer->GetMailbox();
	GMail* pMail = refMailbox.GetMail(nMailUID);
	if (pMail == NULL)
		return false;

	if (NULL == pMail->GetAppendedItem(nMailSlotID))
		return false;

	/// ������ ÷�ε� ������ ����.
	pMail->DelAppendedItem(nMailSlotID);

	/// ����� ������ ID ����.
	pMail->SetThumbnailItemID(nNewMailThumbnailItemID);

	/// Ŭ���̾�Ʈ�� ����.
	m_pClientRouter->TakeAppendedItemRes(pPlayer, nMailUID, nMailSlotID, nNewMailThumbnailItemID);

	return true;
}
