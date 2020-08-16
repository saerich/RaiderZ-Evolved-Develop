#include "stdafx.h"
#include "XModulePost.h"
#include "XMyPlayer.h"
#include "XMovableDebugger.h"
#include "XModuleMyControl.h"
#include "XModuleMovable.h"
#include "XModuleEntity.h"
#include "XModuleTalent.h"
#include "CSItemHelper.h"

XModulePost::XModulePost( XObject* pOwner/*=NULL*/ ) : XModule(pOwner)
{

}

XModulePost::~XModulePost( void )
{

}

void XModulePost::OnUpdate( float fDelta )
{
	PFC_THISFUNC;

	XModule::OnUpdate(fDelta);
}

void XModulePost::PostMove( vec3& vPos, vec3& vDIr, uint32 nMovementFlags )
{
	__Report_Movable_Debug_PostMessage(L"MC_ACTION_MOVE_REQ", vPos);

	TD_PC_MOVE mi;
	mi.vTarPos = vPos;

	vec3 vTransDir = vDIr;
	vTransDir.z = 0.0f;
	vTransDir.Normalize();

	mi.svDir.SetX(vTransDir.x);
	mi.svDir.SetY(vTransDir.y);

	mi.nFlags = static_cast<unsigned short>(nMovementFlags);

	XPOSTCMD1(MC_ACTION_MOVE_REQ, MCommandParameterSingleBlob(&mi,sizeof(TD_PC_MOVE)));
}

void XModulePost::PostStop( vec3& vPos, vec3& vDir )
{
	m_PostChecker.SetLastPostedPosition(vPos);

	__Report_Movable_Debug_PostMessage(L"MC_ACTION_STOP_REQ", vPos);

	XPOSTCMD2(MC_ACTION_STOP_REQ, MCommandParameterVector(vPos), MCommandParameterShortVector2(vDir));
}

void XModulePost::PostMoveStop( vec3& vPos )
{
	m_PostChecker.SetLastPostedPosition(vPos);

	__Report_Movable_Debug_PostMessage(L"MC_ACTION_STOP_MOVE_REQ", vPos);

	XPOSTCMD1(MC_ACTION_STOP_MOVE_REQ, MCommandParameterVector(vPos));
}

void XModulePost::PostPosSync( vec3& vPos )
{
	__Report_Movable_Debug_PostMessage(L"MC_ACTION_POS_SYNC_REQ", vPos);

	XPOSTCMD1(MC_ACTION_POS_SYNC_REQ, MCommandParameterVector(vPos));
}

// ������ �κ� ���� ��ȹ��
// - ÷��ġ�� ������ �� �۴�. - elapsed time ������ �̻�
// - �ٷ� �����⸦ �Ϸ��� ������ ������ ���� ����
// - �Լ� ���� �� �Լ� ������ �����Ͽ� �ٷ� ������� ����.
void XModulePost::_UpdateMovementPost( float fDelta, vec3& pos, vec3& dir, bool bKeyAction, uint32 nMovementFlags )
{
	XModuleMyControl* pModuleMyControl = m_pOwner->GetModuleMyControl();
	XMyActionState* pCurrentActionState = pModuleMyControl->GetCurrentActionState();
	XModuleMovable* pModuleMovable = m_pOwner->GetModuleMovable();

	// �̵��� ���þ��� �׼����� ������ �̵� Ŀ�ǵ带 ������ �ʴ´�.
	if ((pCurrentActionState && !pCurrentActionState->IsPostMovement()) ||
		pModuleMyControl->GetMyHitController()->CheckSendMovePost() == false/* ||
		pModuleMovable->GetMovableFactor().GENERAL_STATE.m_bJumping*/)		// �����߿��� ������ �ʴ´�.
	{
		return;
	}

	XModuleTalent* pModuleTalent = m_pOwner->GetModuleTalent();

	XMovementPostChecker::CheckArgs checkArgs;
	checkArgs.bCurrentGoing = pModuleMovable->GetMovableFactor().IsGoingDirection();
	checkArgs.bCurrentSliding = pModuleMovable->GetMovableFactor().IsSliding();
	checkArgs.bCurrentTalentActive = pModuleTalent->IsActive();
	checkArgs.bKeyAction = bKeyAction;
	checkArgs.bCurrentPosSync = pModuleMyControl->IsPostPosSync();						// ��ũ�������� ������ �ϴ� ���
	checkArgs.bSendablePositionPost = pModuleMyControl->IsSendablePositionPost();

	checkArgs.Pos = pos;
	checkArgs.Dir = dir;

	XMovementPostChecker::RESULT nPostResult = m_PostChecker.Check(fDelta, checkArgs);

	switch (nPostResult)
	{
	case XMovementPostChecker::RESULT_PostMove:
		{
			PostMove(pos, dir, nMovementFlags);
		}
		break;
	case XMovementPostChecker::RESULT_PostStop:
		{
			PostStop(pos, dir);
		}
		break;
	case XMovementPostChecker::RESULT_PostMoveStop:
		{
			PostMoveStop(pos);
		}
		break;
	case XMovementPostChecker::RESULT_PostPosSync:
		{
			PostPosSync(pos);
		}
		break;
	case XMovementPostChecker::RESULT_PostRotate:
		{
			PostRotate(dir);
		}
		break;
	case XMovementPostChecker::RESULT_NONE:	{} break;
	default: { _ASSERT(0); }
	}
}

void XModulePost::UpdateMovement( float fDelta, vec3 pos, vec3 dir, bool bKeyAction, uint32 nMovementFlags )
{
	_UpdateMovementPost(fDelta, pos, dir, bKeyAction, nMovementFlags);
}

void XModulePost::PostPosSyncWhenFinishMovingMotionFactor( vec3& vPos )
{
	m_PostChecker.SetLastPostedPosition(vPos);

	__Report_Movable_Debug_PostMessage(L"MC_ACTION_MOVING_MF_POS_SYNC_REQ", vPos);

	XPOSTCMD1(MC_ACTION_MOVING_MF_POS_SYNC_REQ, MCommandParameterVector(vPos));
}

void XModulePost::PostRotate( vec3& vDir )
{
	XPOSTCMD1(MC_ACTION_ROTATE_REQ, MCommandParameterShortVector2(vDir));
}

void XModulePost::PostEquipItem(SH_ITEM_SLOT nItemSlot, MUID uidItem)
{
	// ! XItem::m_uidID ���ŵǾ�, slotID�θ� ������ �� �ֽ��ϴ�. - praptor, 2010.03.03
	// ! ������� �ʴ� �Լ��� �ǴܵǾ� ���忡�� ������, ���� �ּ�ó���մϴ�. - praptor, 2010.03.03

	//SH_ITEM_SLOT nCurrentIndexSlot = nItemSlot;
	//SH_ITEM_SLOT nProperSlot = nCurrentIndexSlot;

	//XItem* pItem = gvar.MyInfo.Inventory.GetItem(uidItem);
	//CSItemData* pItemData = NULL;
	//if (pItem) pItemData = pItem->m_pItemData;

	//XItem* pRWeapon = gvar.MyInfo.EquipmentSlot.GetCurrentWeaponSetWeaponRight();
	//XItem* pLWeapon = gvar.MyInfo.EquipmentSlot.GetCurrentWeaponSetWeaponLeft();

	//CSItemData* pRWeaponItemData = NULL;
	//CSItemData* pLWeaponItemData = NULL;
	//if (pRWeapon) pRWeaponItemData = pRWeapon->m_pItemData;
	//if (pLWeapon) pLWeaponItemData = pLWeapon->m_pItemData;

	//nProperSlot = CSItemHelper::GetProperWeaponSlot(nCurrentIndexSlot, pItemData, pRWeaponItemData, pLWeaponItemData);

	//XPOSTCMD2(MC_ITEM_EQUIP_REQ, MCommandParameterInt(pItem->m_nSlotID), MCommandParameterChar(char(nProperSlot)));
}

void XModulePost::Init( vec3& vPos, vec3& vDir )
{
	m_PostChecker.SetLastPostedPosition(vPos);
	m_PostChecker.SetLastPostedDir(vDir);
}

XMovementPostChecker::RESULT XMovementPostChecker::Check(float fDelta, const CheckArgs& args)
{
	m_fPosSyncElapsedTime += fDelta;

	RESULT nResult = _Check(fDelta, args);

	if (m_fPosSyncElapsedTime >= POST_MOVE_TICK)
	{
		m_fPosSyncElapsedTime = 0.0f;
	}

	ApplyCheckResult(nResult, args.Pos, args.Dir);

	return nResult;
}

XMovementPostChecker::RESULT XMovementPostChecker::_Check(float fDelta, const CheckArgs& args)
{
	const bool bCurrentGoing = args.bCurrentGoing;
	const bool bCurrentSliding = args.bCurrentSliding;
	const bool bKeyAction = args.bKeyAction;
	const bool bCurrentTalentActive = args.bCurrentTalentActive;
	const bool bCurrentPosSync = args.bCurrentPosSync;
	const bool bSendablePositionPost = args.bSendablePositionPost;

	const float fLastPostedPositionLengthSQ = m_vLastPostedPosition.DistanceToSq(args.Pos);
	const float POST_MOVE_DISTANCE_SQ = 2500.0f;		// (50 * 50)

	//----------------------------------------------------------------------------//
	// �Ϲ����� �̵�, ���� ��Ŷ
	if (bSendablePositionPost == true &&
		(m_fPosSyncElapsedTime >= POST_MOVE_TICK || 
		fLastPostedPositionLengthSQ > POST_MOVE_DISTANCE_SQ))	// 0.1��
	{
		if (bCurrentGoing)
		{
			// �̵����̴�.
			m_bStopPosted = false;

			return RESULT_PostMove;
		}
		else if(bCurrentPosSync == false)	// ��ũ���� �ƴϾ�� �Ѵ�.
		{
			if (!m_bStopPosted)
			{
				m_bStopPosted = true;

				return RESULT_PostMoveStop;
			}


			//----------------------------------------------------------------------------//
			// Ư�� ���¿� ���� ��Ŷ
			// ������ ������ ������ ��Ŷ

			if(fLastPostedPositionLengthSQ > 10.0f &&
				bCurrentSliding == false &&
				bCurrentTalentActive == false)
			{
				return RESULT_PostStop;
			}
		}

		// ��ũ�� ������ ���ߴ� ���� �Ÿ��� ������� �����.
		if (bCurrentPosSync ||
			fLastPostedPositionLengthSQ > POST_MOVE_DISTANCE_SQ)
		{
			float fDiffSyncPos = m_vLastPostedSyncPosition.DistanceToSq(args.Pos);
			if (bCurrentPosSync ||
				fDiffSyncPos > POST_MOVE_DISTANCE_SQ)
			{
				return RESULT_PostPosSync;
			}
		}
	}

	//----------------------------------------------------------------------------//
	// �Ϲ����� ȸ�� ��Ŷ
	if (m_fPosSyncElapsedTime >= POST_MOVE_TICK)
	{
		if (!bCurrentGoing)
		{
			float diffDir = m_vLastPostedDir.DotProduct(args.Dir);

			if (diffDir <= 0.97f)
			{
				return RESULT_PostRotate;
			}
		}
	}

	return RESULT_NONE;
}

void XMovementPostChecker::ApplyCheckResult( RESULT nResult, const vec3& pos, const vec3& dir )
{
	if (nResult == RESULT_PostMove ||
		nResult == RESULT_PostStop ||
		nResult == RESULT_PostMoveStop)
	{
		m_vLastPostedPosition = pos;
		m_vLastPostedSyncPosition = pos;
	}

	if (nResult == RESULT_PostPosSync)
	{
		m_vLastPostedSyncPosition = pos;
	}

	if (nResult == RESULT_PostMove ||
		nResult == RESULT_PostRotate)
	{
		m_vLastPostedDir = dir;
	}
}

XMovementPostChecker::XMovementPostChecker()
{
	m_vLastPostedPosition	= vec3(23452345,234523453,2345234);
	m_vLastPostedSyncPosition = m_vLastPostedPosition;
	m_vLastPostedDir		= vec3::ZERO;

	m_fPosSyncElapsedTime	= 0.0f;
	m_bStopPosted			= true;
}

