#include "StdAfx.h"
#include "XTargetIndicator.h"
#include "XTargetCollisionBox.h"
#include "XMouseInteractor.h"
#include "XInteractionInfo.h"
#include "XModuleUI.h"

XTargetIndicator::XTargetIndicator(XObject* pObject)
:m_pTargetCollisionBox(new XTargetCollisionBox(pObject))
, m_pMouseInteractor(new XMouseInteractor(pObject))
, m_bShow(true)
{
	m_pTargetCollisionBox->InitTarget(false);
	m_pTargetCollisionBox->SetTargetInfo(FLT_MAX, 0, CSEffectInfo::RELATION_ALL);
}

XTargetIndicator::~XTargetIndicator(void)
{
	SAFE_DELETE(m_pMouseInteractor);
	SAFE_DELETE(m_pTargetCollisionBox);
}

void XTargetIndicator::OnUpdate()
{
	_UpdateTarget();

	_UpdateTargetInfo();

	_UpdateTargetIndicatorVisible();

	if (m_bShow == false)
	{
		m_pTargetCollisionBox->DeleteIndicatorEffect();
	}
}

void XTargetIndicator::_UpdateTarget()
{
	if (global.ui->IsMouseVisible())
	{
		XObject* pTarget = m_pMouseInteractor->OnOver(MEvent::GetMouseClientPos().x, MEvent::GetMouseClientPos().y);
		if (pTarget)
		{
			m_pTargetCollisionBox->CheckTargetObj(pTarget);
		}
		else
		{
			m_pTargetCollisionBox->DelTargetAll();
		}
	}
	else
	{
		m_pTargetCollisionBox->UpdateTarget();
	}
}

void XTargetIndicator::_UpdateTargetInfo()
{
	stTargetID* pTargetInfo = m_pTargetCollisionBox->GetTargetInfo();
	if(pTargetInfo)
	{
		XActor* pTargetActor = gg.omgr->FindActor(pTargetInfo->uidTarget);
		if(pTargetActor && pTargetActor->IsModuleEntity())
		{
			if (XGetInteractionInfo().GetSearchUIID() != pTargetActor->GetUIID())
			{
				TargetHide();

				// Ÿ���� 0.3���̻� ��� ������ ��û�ϵ���
				if ((pTargetInfo->time + 300.f) < timeGetTime())
				{
					XGetInteractionInfo().SetSearchUIID(pTargetActor->GetUIID());

					XGetInteractionInfo().stTargetActorInfo.nLockOnTime = timeGetTime();

					if (global.script) global.script->GetGlueGameEvent().OnTargetInfoRefresh();

					m_pTargetCollisionBox->SetIndicatorEffect();
				}
			}
			else
			{
				// ������ �ִ� Ÿ���� ������ 3�������̸� �ٽ� ���ſ�û
				if ((gvar.Game.EnemyInfo.GetEnemyInfoByUIID(XGetInteractionInfo().GetSearchUIID()) == NULL) &&
					(XGetInteractionInfo().stTargetActorInfo.Find_Time(XGetInteractionInfo().GetSearchUIID()) == false))
				{
					if(global.script) global.script->GetGlueGameEvent().OnTargetInfoRefresh();
				}

				if((m_bShow == true) && (m_pTargetCollisionBox->IsIndicatorEffect() == false))
					m_pTargetCollisionBox->SetIndicatorEffect();
			}
		}
	}
	else
	{
		TargetHide();

		SetShow(true);
	}
}

void XTargetIndicator::TargetHide()
{
	if (XGetInteractionInfo().GetSearchUIID() == 0) return;

	if (global.script) global.script->GetGlueGameEvent().OnTargetInfoHide();

	XGetInteractionInfo().SetSearchUIID(0);
}

void XTargetIndicator::ChangeIndicatorEffectPos()
{
	m_pTargetCollisionBox->ChangeIndicatorEffectPos();  
}

void XTargetIndicator::SetShow( bool bShow/*=true*/ )
{
	m_bShow = bShow;
}

void XTargetIndicator::Init()
{
	m_pTargetCollisionBox->InitTarget(false);
}

void XTargetIndicator::_UpdateTargetIndicatorVisible()
{
	XActor* pTargetActor = gg.omgr->FindActor_UIID(XGetInteractionInfo().GetSearchUIID());
	if(pTargetActor && pTargetActor->IsModuleEntity())
	{
		if(pTargetActor->GetModuleUI()->IsShowBallonText())
			SetShow(false);
		else
			SetShow(true);
	}
}