#include "stdafx.h"
#include "XMovableProcObjectChecker.h"
#include "XObject.h"
#include "XModuleCollision.h"
#include "XModuleTalent.h"
#include "XModuleMotion.h"
#include "XTalentHelper.h"

bool XMovableProcObjectChecker::CheckObjectCollision(const XMovableFactor& _rMovableFactor, XObject* pObejct)
{
	//----------------------------------------------------------------------//
	// ��� ĳ���Ͱ� Ư���� ��Ȳ�߿����� ������Ʈ �浹�� ����Ѵ�.
	bool bObjectCol = false;

	// ** �˹� ���´� ������Ʈ �浹�� ���� �ʴ´�. (NPC��)
	if(pObejct->GetEntityType() != ETID_PLAYER &&
		_rMovableFactor.KNOCK_BACK.m_bKnockback)
		return false;

	// 1. �ŷ�Ʈ �����
	// 2. �̵� ��� ���� �϶�...
	// 3. �� �ڽ��϶�...
	if(pObejct->GetEntityType() == ETID_PLAYER)				// �÷��̾��̸鼭...
	{
		if(pObejct->GetUID() != XGetMyUID())				// ��� ĳ���͸�...
		{

			// ����ŷ�Ʈ�� �ִ°�...
			bObjectCol = _rMovableFactor.GENERAL_STATE.m_bTalentActive;// �ŷ�Ʈ ������ΰ�...

			// ������ �浹 üũ�� ���°�...
			if(!bObjectCol &&								// �� ���ķ� �߰����� ��Ȳ�� �ִ´�.
				_rMovableFactor.IsMoveMF())					// �̵� ��������ΰ�...
			{
				bObjectCol = true;
			}

			// ���� ���ΰ�?
			if(pObejct->AsPlayer()->IsDuel())
				bObjectCol = true;
		}
		else
		{
			bObjectCol = true;
		}
	}

	// �ŷ�Ʈ ������̶� ���� ��ǥ������ �������� �ʾҴٸ� �浹�н�
	if(XGetMyUID() != pObejct->GetUID())
	{
		if(_rMovableFactor.INTERPOLATION.m_bWorking &&
			_rMovableFactor.INTERPOLATION.m_bFinishMove == false)
			return  false;
	}

	// �浹�� �ȴٸ�... �浹�� �⺻���� üũ
	if(bObjectCol)
	{
		XModuleCollision* pModuleCollision = pObejct->GetModuleCollision();
		if(pModuleCollision && pModuleCollision->IsBaseColliable())
		{
			XModuleTalent* pModuleTalent = pObejct->GetModuleTalent();
			if(pModuleTalent)
				return pModuleTalent->IsTalentColliable();
		}

		return false;
	}

	return bObjectCol;
}

bool XMovableProcObjectChecker::CheckCanRub( XMovableFactor& _rMovableFactor, XObject* pObejct, int nCurrentTalentID )
{
	bool bCheckRub = true;

	XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(nCurrentTalentID, pObejct);
	if(pTalentInfo)
	{
		if(pTalentInfo->m_eTalentShoveType == TST_STOP)
			bCheckRub = false;
	}

	if(_rMovableFactor.IsTalentActive() &&
		bCheckRub == false)
	{
		bCheckRub = false;
	}

	// �ŷ�Ʈ ������̶� ���� ����� �޸���� ����.
	// Player���Ը� �ش�� �����ϰ��̴�.
	XModuleMotion* pModuleMotion = pObejct->GetModuleMotion();
	if(pModuleMotion && pModuleMotion->IsCurrentMotion(MOTION_NAME_RUN))
	{
		bCheckRub = true;
	}

	return bCheckRub;
}

bool XMovableProcObjectChecker::CheckCapsuleRadiusOffset( const XMovableFactor& _rMovableFactor, XObject* pObejct )
{
	// �÷��̾�� �ش�
	if(pObejct->GetEntityType() != ETID_PLAYER)
		return false;

	// �ŷ�Ʈ ������̸�... OK
	if(_rMovableFactor.IsTalentActive())
		return true;

	return false;
}