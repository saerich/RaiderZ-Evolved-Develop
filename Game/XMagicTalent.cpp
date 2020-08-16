#include "stdafx.h"
#include "XEventID.h"
#include "XMagicTalent.h"
#include "XTarget.h"
#include "XModuleEntity.h"
#include "XModuleMotion.h"
#include "XProjectile.h"

void XMagicTalent::OnEnterPhasePrepare()
{
	// �ִϸ��̼� ����
	ChangeTalentMotion(TALENT_PHASE_PREPARE, m_pTalentInfo->m_szCastingAnimation.c_str(), m_pTalentInfo->m_szCastingLoopAnimation.c_str());
	// ����Ʈ ����
	ChangeTalentEffect(TALENT_PHASE_PREPARE);
}

void XMagicTalent::OnEnterPhaseAct()
{
	// �ִϸ��̼� ����
	ChangeTalentMotion(TALENT_PHASE_ACT, m_pTalentInfo->m_szUseAnimation.c_str(), L"", m_pTalentInfo->m_nMovableType);
	// ����Ʈ ����
	ChangeTalentEffect(TALENT_PHASE_ACT);

	// ���� ���Ÿ� ���� Ÿ���̸� �߻�ü �߻�
	// ���� ��� �ִϸ��̼��� �־�� �Ѵ�.
	if(((m_pTalentInfo->m_nSkillType == ST_ARCHERY) || (m_pTalentInfo->m_nSkillType == ST_MAGIC)) &&
		(!m_pTalentInfo->m_szUseAnimation.empty()))
	{
		// XModuleEntity���� �̺�Ʈ �� �߻� ���

		//mlog0("TALENT ACT %d\n", m_pTalentInfo->m_nID);

		//mlog0("�߻��غ� %d(%s)\n", m_pTalentInfo->m_nID, m_pTalentInfo->m_szUseAnimation.c_str());
	}

	// ��Ÿ ó��
	// �߻� �޼��� �ڽſ��� ������
	SendMsg(XEVTL_TALENT_ACT_SPELL);
}

void XMagicTalent::OnStart()
{
	// �ŷ�Ʈ ���� �˸�
	SendMsg(XEVTL_TALENT_START);
}

void XMagicTalent::SendMsg(int nEventID)
{
	bool bSetMsg = false;
	XEvent evt;

	switch(nEventID)
	{
	case XEVTL_TALENT_START:
	case XEVTL_TALENT_ACT_SPELL:
	case XEVTL_TALENT_RECASTING:
		{
			evt.m_nID			= nEventID; 
			evt.m_pData			= m_pTalentInfo;
			bSetMsg				= true;
		}
		break;
	}

	if(bSetMsg == true)
	{
		m_pOwner->Event(evt);
	}
}

void XMagicTalent::OnReCastingTalent( int nTalentID, int nStep )
{
	// �ŷ�Ʈ ID�� ������ ��.
	// ���� ���� �ִ� �ŷ�Ʈ�� ���ƾ� ���� �ܰ谡 �´��� �� �� �ִ�.
	if(m_pTalentInfo->m_nID == nTalentID)
	{
		SendMsg(XEVTL_TALENT_RECASTING);
	}
}

bool XMagicTalent::IsNeedTarget()
{
	if(m_pTalentInfo->m_EffectInfo.m_nPoint == CSEffectInfo::POINT_TARGET)
		return true;

	return false;
}

XEventResult XMagicTalent::OnEvent( XEvent& msg )
{
	switch (msg.m_nID)
	{
	case XEVTL_UPPER_MOTION_PLAYONCE:
		{
			if(m_pTalentInfo->m_nMovableType == MUT_USE)
			{
				wchar_t* szMotionName = MOTION_NAME_IDLE;
				if (m_pOwner->GetEntityType() == ETID_NPC) szMotionName = MOTION_NAME_NPC_IDLE;

				XModuleMotion * pModuleMotion = m_pOwner->GetModuleMotion();
				if(pModuleMotion)
					pModuleMotion->ChangeMotion(szMotionName);
			}
		}
		break;
	}

	return MR_FALSE;
}
