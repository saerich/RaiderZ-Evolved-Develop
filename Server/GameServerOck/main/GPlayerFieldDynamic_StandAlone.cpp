#include "StdAfx.h"
#include "GPlayerFieldDynamic_StandAlone.h"
#include "GDef.h"
#include "GGlobal.h"
#include "GEntityPlayer.h"
#include "GFieldMgr.h"
#include "GPartySystem.h"
#include "GDynamicFieldMaster.h"
#include "GDynamicFieldFactory.h"
#include "SCmdRouter_Login.h"
#include "GCommandCenter.h"


GPlayerFieldDynamic_StandAlone::GPlayerFieldDynamic_StandAlone(GEntityPlayer* pOwner, GPlayerField* pPlayerField)
: GPlayerFieldDynamic(pOwner, pPlayerField)
{
}

GPlayerFieldDynamic_StandAlone::~GPlayerFieldDynamic_StandAlone()
{
}

bool GPlayerFieldDynamic_StandAlone::GateRequest(DYNAMIC_FIELD_TYPE eType, int nFieldGroupID, int nFieldID, vec3 vecDestPos, vec3 vecDestDir, bool bSingleEnter)
{
	if (DFT_NONE == eType || 0 == nFieldGroupID || 0 == nFieldID)
		return false;

	/// Ŭ���̾�Ʈ�� Gate �غ� Ŀ�ǵ� ����
	SCmdRouter_Login router(gsys.pCommandCenter);
	router.PreparingToLoad(m_pOwner->GetUID(), nFieldID);

	// ���� ������ Field Group ã��
	MUID uidDestFieldGroup = MUID::ZERO;

	uidDestFieldGroup = GetEnterableDynamicFieldGroup(eType, nFieldGroupID, bSingleEnter);
	if (uidDestFieldGroup.IsInvalid())
	{
		// ���� �Ҹ��� Field Group �� ���� ��� ����.
		GDynamicFieldFactory dynamicFieldFactory;
		GDynamicFieldMaster* pFieldGroup = dynamicFieldFactory.CreateDynamicField(eType, m_pOwner, nFieldGroupID);
		if (NULL == pFieldGroup)
			return false;

		uidDestFieldGroup = pFieldGroup->GetUID();
	}

	if (!IsValidFieldGroupWith(uidDestFieldGroup, eType, nFieldGroupID))
		return false;

	if (!GateExecute(uidDestFieldGroup, nFieldID, vecDestPos, vecDestDir))
		return false;

	return true;
}

void GPlayerFieldDynamic_StandAlone::_EndGateExecute(GDynamicFieldMaster* pFieldGroup)
{
	if (NULL == pFieldGroup) return;

	if (pFieldGroup->GetType() == DFT_TRIAL && pFieldGroup->GetPlayerQty() == 1 && gsys.pPartySystem->IsPartyMember(m_pOwner))
	{
		MUID uidParty = m_pOwner->GetPartyUID();
		gsys.pPartySystem->AddField(uidParty, m_pOwner);
	}
}
