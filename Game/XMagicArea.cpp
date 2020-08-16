#include "StdAfx.h"
#include "XMagicArea.h"
#include "XModuleEntity.h"
#include "XModuleBuff.h"
#include "XEffect.h"
#include "XSystem.h"
#include "XTalentInfoMgr.h"
#include "XNPCInfo.h"
#include "XGameTransformControllerManager.h"
#include "XBuffInfo.h"

MImplementRTTI(XMagicArea, XObject);

// �������� ���̴� �ִϸ��̼��� �����Դϴ�.
#define MAGICAREA_IDLE		L"idle"
#define MAGICAREA_DESPAWN	L"die"
#define MAGICAREA_SPAWN		L"MF_KD3"
#define MAGICAREA_ATTACK	L"spell_fire"

XMagicArea::XMagicArea(MUID uid)
: XObject(uid)
, m_pNPCInfo(NULL)
, m_bModelLoadingComplete(false)
, m_bDespawn(false)
, m_bStarted(false)
, m_fRemainTime(0.0f)
, m_bUseRemainTime(false)
{
	m_nTypeID			= ETID_BUFF_ENTITY;

	m_pModuleEntity		= new XModuleEntity(this);
}

XMagicArea::~XMagicArea()
{
	SAFE_DELETE(m_pModuleEntity);
}

void XMagicArea::RegisterModules()
{
	XObject::RegisterModules();

	// Initialize ����
	if(m_pModuleEntity)
	{
		m_pModuleEntity->Initialize();
		m_Modules.AddModule(m_pModuleEntity, true);
	}
}

void XMagicArea::UnregisterModules()
{
	// Remove ����..
	if(m_pModuleEntity)
	{
		m_Modules.RemoveModule(m_pModuleEntity);
		m_pModuleEntity->Finalize();
	}

	XObject::UnregisterModules();
}

void XMagicArea::OnUpdate( float fDelta )
{
	XObject::OnUpdate(fDelta);
	CheckRemainTime(fDelta);
	m_Effect.Update(fDelta);

	// �� �ε� ���Ŀ� �ؾ��ϴ� �͵�...
	if(CheckModelLoadingComplete())
		DoModelLoadingComplete();
	
	// �ִϸ��̼� ���¿� ���� ����
	CheckAnimationState();

	// �����
	CheckDebugMagicArea();
}

bool XMagicArea::OnCreate()
{
	return true;
}

void XMagicArea::OnDestroy()
{
	m_Effect.Destroy();
}

void XMagicArea::OnRender()
{
}

void XMagicArea::Setup( vec3& vPos, int nBuffID, float fReMainTime, bool bStarted /*= false*/ )
{
	XBuffInfo* pBuffInfo = info.buff->Get(nBuffID);
	if(pBuffInfo == NULL)
		return;

	SetVisible(true);

	// ����
	SetPosition(vPos);
	m_fRemainTime = fReMainTime;

	// ����!!!! ���� �ð��� ���δ� ������ �����ϴ� ���̴�. �׷��� ��Ⱑ �Ǿ���!!!!!!!
	if(m_fRemainTime > 0.0f)
		m_bUseRemainTime = true;

	// ����Ʈ
	m_Effect.Create(this, pBuffInfo, vPos, GetDirection());

	// ��
	SetupModel(pBuffInfo);

	// �̹� ���۵� �������ΰ�?
	m_bStarted = bStarted;
}

void XMagicArea::SetupModel( XBuffInfo* pBuffInfo )
{
	int nNPCID = pBuffInfo->m_nExtraActiveModelID;
	if (nNPCID > 0)
	{
		XNPCInfo* pNPCInfo = info.npc->Get(nNPCID);
		if(pNPCInfo == NULL)
			return;

		// npc
		m_pNPCInfo = pNPCInfo;

		if (m_pModuleEntity)
		{
			m_pModuleEntity->SetScale(m_pNPCInfo->fScale);
			m_pModuleEntity->SerializeNPC(pNPCInfo);
		}
	}
}

void XMagicArea::AniSpawn()
{
	m_pModuleEntity->GetActor()->SetAnimation(MLocale::ConvUTF16ToAnsi(MAGICAREA_SPAWN).c_str());
}

void XMagicArea::AniIdle()
{
	m_pModuleEntity->GetActor()->SetAnimation(MLocale::ConvUTF16ToAnsi(MAGICAREA_IDLE).c_str());
}

void XMagicArea::AniAttack()
{
	if(m_pModuleEntity->GetActor()->SetAnimation(MLocale::ConvUTF16ToAnsi(MAGICAREA_ATTACK).c_str()) == false)
	{
		DeleteMe();
	}
}

void XMagicArea::AniDespawn()
{
	if(m_pModuleEntity->GetActor()->SetAnimation(MLocale::ConvUTF16ToAnsi(MAGICAREA_DESPAWN).c_str()) == false)
	{
		DeleteMe();
	}

	m_bDespawn = true;
}

void XMagicArea::TrapAction()
{
	AniAttack();
}

void XMagicArea::Despawn()
{
	AniDespawn();
}

bool XMagicArea::CheckModelLoadingComplete()
{
	if(m_bModelLoadingComplete == false &&
		m_pModuleEntity->IsLoadingCompleted())
	{
		m_bModelLoadingComplete = true;

		return true;
	}

	return false;
}

void XMagicArea::CheckAnimationState()
{
	// �ִϸ��̼� ���¿� ���� ����
	if(m_pModuleEntity)
	{
		// 1. �������Ŀ��� Idle �ִϸ��̼����� �����Ѵ�.
		if(m_pModuleEntity->GetAnimationController()->GetAnimationName() == MAGICAREA_SPAWN &&
			m_pModuleEntity->GetAnimationController()->IsPlayDone())
			AniIdle();

		// 2. �����̳� ������� ��� ���Ŀ��� �ڵ����� ����� �Ѵ�.
		if((m_pModuleEntity->GetAnimationController()->GetAnimationName() == MAGICAREA_DESPAWN ||
			m_pModuleEntity->GetAnimationController()->GetAnimationName() == MAGICAREA_ATTACK) &&
			m_pModuleEntity->GetAnimationController()->IsPlayDone())
			DeleteMe();
	}
}

void XMagicArea::DoModelLoadingComplete()
{
	// �ε� �Ϸ� �� ���ϴ� �͵�...
	// �׾���?
	if(m_bDespawn)
		return;

	// �̹� ���۵ȰŶ��...
	if(m_bStarted)
	{
		AniIdle();
		return;
	}

	// ����
	AniSpawn();
}

void XMagicArea::CheckDebugMagicArea()
{
	m_Effect.SetDebugMagicArea(gvar.Debug.bShowMagicArea);
}

void XMagicArea::CheckRemainTime( float fDelta )
{
	if(m_bUseRemainTime == false)
		return;

	m_fRemainTime -= fDelta;
	if(m_fRemainTime <= 0.0f && m_bDespawn == false)
	{
		// ������.
		Despawn();
	}
	else
	{
		m_Effect.CheckRemainTimeEffect(m_fRemainTime);
	}
}