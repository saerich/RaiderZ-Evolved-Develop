#include "stdafx.h"
#include "XBPartWeaponController.h"
#include "XMyPlayer.h"
#include "XModuleTalent.h"
#include "XModuleBuff.h"
#include "XItemManager.h"
#include "XEffectInvoker.h"

XBPartWeaponController::XBPartWeaponController()
{
	m_bWork			= false;
}

void XBPartWeaponController::OnStart( XPlayer* pPlayer, int nItemID )
{
	// ���� ���·� ���� ����
	pPlayer->ChangeStance(CS_BATTLE, true);

	m_bWork = true;

	BPartWeaponEffect(nItemID, pPlayer, XCONST::BPART_GAIN_BUFF_EFFECT_NAME);
}

void XBPartWeaponController::OnEnd(XObject* pObject, int nItemID)
{
	m_bWork = false;

	BPartWeaponEffect(nItemID, pObject, XCONST::BPART_LOST_BUFF_EFFECT_NAME);
}

bool XBPartWeaponController::CheckBuffLost( XObject* pObject, int nBuffID )
{
	if(pObject->GetEntityType() != ETID_PLAYER)
		return true;

	XModuleTalent* pModuleTalent = pObject->GetModuleTalent();

	XBuffInfo * pBuffInfo = info.buff->Get(nBuffID);
	if(pModuleTalent == NULL ||
		pBuffInfo == NULL ||								// ������ ���ų�..
		m_bWork == false ||									// �۵����� �ƴ϶��...
		pModuleTalent->IsActive() == false)					// �ŷ�Ʈ �۵����� �ƴ϶��...
		return true;

	// ���� �ŷ�Ʈ �۵����ΰ̴ϴ�.
	bool bExist = false;
	if(pObject->GetEntityType() == ETID_PLAYER)
	{
		for(int i = 0; i < ITEMSLOT_MAX; ++i)
		{
			if(pBuffInfo->m_nEquipItemIDs[i] != INVALID_ID)
				bExist = true;
		}
	}

	m_setBuffID.insert(nBuffID);
	return false;
}

void XBPartWeaponController::OnUpdate( XObject* pObject )
{
	if(pObject->GetEntityType() != ETID_PLAYER)
		return;

	if(m_setBuffID.empty())
		return;

	XModuleTalent* pModuleTalent = pObject->GetModuleTalent();
	if(pModuleTalent && pModuleTalent->IsActive() == false)
	{
		XModuleBuff* pModuleBuff = pObject->GetModuleBuff();
		if(pModuleBuff)
		{
			for(set<int>::iterator it = m_setBuffID.begin(); it != m_setBuffID.end(); ++it)
				pModuleBuff->BuffLost(*it);
		}

		m_setBuffID.clear();
	}
}

void XBPartWeaponController::BPartWeaponEffect( int nItemID, XObject* pObject, wstring strEffectName )
{
	XItemData* pItemData = info.item->GetItemData(nItemID);
	if(pItemData)
	{
		// ������ �θ� ����Ʈ�� ���Ϸ��� �ص� �ִϸ��̼� ���涧���� ����Ʈ�� ������ �ʽ��ϴ�. 
		// �׷��Ƿ� �θ� �θ� ����Ʈ�� ���Դϴ�.
		wstring strParentName = pItemData->m_strParentName;
		RActorNode* pActorNode = pObject->GetActor()->GetActorNode(MLocale::ConvUTF16ToAnsi(pItemData->m_strParentName.c_str()).c_str());
		if(pActorNode && pActorNode->GetParent())
		{
			strParentName = MLocale::ConvAnsiToUTF16(pActorNode->GetParent()->GetNodeName().c_str());
		}

		XEffectInvoker effectInvoker;
		effectInvoker.Invoke(pObject, strEffectName, wstring(L""), strParentName);
	}
}

XMyBPartWeaponController::XMyBPartWeaponController()
{
}

bool XMyBPartWeaponController::UsableActionState( int nState, int nParam )
{
	if(m_bWork == false)
		return true;

	if (nState == ACTION_STATE_CHANGE_STANCE	||
		nState == ACTION_STATE_GUARD			||
		nState == ACTION_STATE_WEAPON_CHANGE	||
		nState == ACTION_STATE_SIT				)
		return false;

	// �븻 �����߿�����
	if(nState == ACTION_STATE_NORMAL_ATTACK)
	{
		TALENT_NORMAL_ATTACK_TYPE eAttackType = (TALENT_NORMAL_ATTACK_TYPE)nParam;
		if(eAttackType != NA_DODGE_FRONT &&
			eAttackType != NA_DODGE_BACK &&
			eAttackType != NA_DODGE_LEFT &&
			eAttackType != NA_DODGE_RIGHT)
			return false;
	}

	return true;
}

bool XMyBPartWeaponController::BPartsAction(XVirtualKey nVirtualKey)
{
	if (nVirtualKey == VKEY_ACTION)
	{
		XBuffAttribute attrBuff = XGetMyPlayer()->GetModuleBuff()->GetBuffAttribute();
		if ( attrBuff.IsTotemable() == true)
		{
			XPaletteItem* pItem = gvar.MyInfo.PaletteList.GetSelectedItem(0);
			if (pItem == NULL || pItem->GetItemType() == PIT_ITEM) return false;

			XGetMyPlayer()->GetModuleMyControl()->DoActionUseTalent(pItem->talentID);

			return true;
		}
	}

	return false;
}

XNetBPartWeaponController::XNetBPartWeaponController()
{
}
