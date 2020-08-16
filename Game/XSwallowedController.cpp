#include "stdafx.h"
#include "XSwallowedController.h"
#include "XModuleAction.h"
#include "XModuleEntity.h"
#include "XModuleMovable.h"
#include "XModuleTalent.h"
#include "XEffectInvoker.h"
#include "XTalentHitParam.h"
#include "XMotionHelper.h"

#include "RDebugPassRenderer.h"

XSwallowedController::XSwallowedController()
:XGrappledController()
{
	m_bDisAppear = false;
}


XSwallowedController::~XSwallowedController()
{

}

bool XSwallowedController::OnStart( _DamageRepeatInfo* pHitInfo)
{
	bool bResult = XGrappledController::OnStart(pHitInfo);

	// ó������ �������� �Դ´�.
	_ShowDamage(pHitInfo->nDamage);

	return bResult;
}


void XSwallowedController::OnFinish()
{
	XGrappledController::OnFinish();

	if(m_bDisAppear)
		OnAppear();
}

void XSwallowedController::OnDisAppear()
{
	// ���� �������.
	if(m_pPlayer == NULL)
		return;

	XModuleEntity* pModuleEntity = m_pPlayer->GetModuleEntity();
	if(pModuleEntity == NULL)
		return;

	if(pModuleEntity)
	{
		pModuleEntity->SetVisibility(0.0f);
	}

	m_bDisAppear = true;
}

void XSwallowedController::OnAppear()
{
	// ���� ��Ÿ����.
	if(m_pPlayer == NULL)
		return;

	XModuleEntity* pModuleEntity = m_pPlayer->GetModuleEntity();
	if(pModuleEntity == NULL)
		return;

	if (pModuleEntity)
	{
		pModuleEntity->SetVisibility(1.0f);
	}

	m_bDisAppear = false;
}
