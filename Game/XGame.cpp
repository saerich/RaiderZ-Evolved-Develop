#include "stdafx.h"
#include "XGame.h"
#include "XWorld.h"
#include "MockWorld.h"
#include "RCameraSceneNode.h"
#include "XNonPlayer.h"
#include "XModuleEntity.h"
#include "XModuleMyControl.h"
#include "XUtil.h"
#include "XNPCInfo.h"
#include "XEventID.h"
#include "XMyPlayer.h"
#include "XItemContainer.h"
#include "XController.h"
#include "XModuleNetControl.h"
#include "XProfiler.h"
#include "RDirectionalLightSceneNode.h"
#include "REnvironmentSet.h"
#include "XPost_Loot.h"
#include "XPost_Interaction.h"
#include "XStrings.h"
#include "XItemManager.h"
#include "XBaseApplication.h"
#include "XEffectManager.h"
#include "XResourceManager.h"
#include "XCameraManager.h"
#include "XSoulObject.h"
#include "XModuleNonControl.h"
#include "XFeedbackObject.h"
#include "XFeedbackSystem.h"

#include "XSystem.h"
#include "XMagicArea.h"
#include "XTalentInfoMgr.h"
#include "XBattleArena.h"
#include "XModuleBuff.h"
#include "XPvPAreaLogic.h"


XGame::XGame(bool bRealGameNotTest) : m_nUpdateObjectCacheTime(0), m_bEnableUpdateObjectCache(true)
{
	gg.game = this;
	gg.omgr = &m_ObjectManager;

	m_pCurrentGameState = m_GameStateMgr.CreateGameState(bRealGameNotTest);
	gg.currentgamestate = m_pCurrentGameState;
	gg.controller		= m_pCurrentGameState->GetController();
}

XGame::~XGame()
{
	Destroy();

	gvar.Game.pMyPlayer = NULL;
	gg.game = NULL;
	gg.omgr = NULL;
	gg.currentgamestate = NULL;
	gg.controller = NULL;
}

bool XGame::Create()
{
	// ���ӿ� ���̴� Ŭ�������� Create�Լ����̴�. 
	// ���� �ܴ̿� Create�� �ʿ䰡 ����.

	// ���� �����
	if (m_pCurrentGameState->Create() == false)
		return false;

	if (gg.controller)
	{
		if (gg.controller->Create() == false)
			return false;

		gg.controller->SetEnabled(true);
	}

	// by pok, ���� �ǰ��� lack ������ �˱����� �ּ�ó�� �Ǿ��ִ� MProfile2�� �ٽ� ���ϴ�.
	// �⺻������ MInitStatisticsProfile(true, false, false); ���� �س��� ���ϴ�.
	// �ڼ��� �������ϸ��� �Ϸ��� �Ҷ� �����źв��� �ɼ��� ���� ����մϴ�.
	// ���ڵ� �ɼ��� ���� �޸� ��뷮�� �ް��� ������ �� �ֽ��ϴ�.
#ifndef _PUBLISH
	MInitStatisticsProfile(true, false);

	if (global.script)		global.script->GetGlueGameEvent().OnDebugOnGameStarted();
	if (global.ui)			global.ui->SetLogCallbackToScript();
#endif

	m_bCreated = true;

	return true;
}

void XGame::Destroy()
{
	if (!m_bCreated) return;
	m_bCreated = false;

	// �� �Ŵ����� ������Ʈ �Ŵ����� �������ֱ� ���� AsyncLoading�� ����
	REngine::GetBackgroundWorker().ClearQueues();

	m_ObjectManager.Destroy();
	m_GameStateMgr.DestroyGameState();

	// ����Ʈ ����
	if(global.emgr)
		global.emgr->Destroy();

	// �� �Ŵ��� ����
	if (global.smgr)
		global.smgr->Clear();

	if (global.res)
		global.res->FiniCaching();
}

void XGame::Update(float fDelta)
{
	PFC_THISFUNC;
	PFI_BLOCK_THISFUNC(200);

	// game controller update : input / camera / target
	gg.controller->OnUpdate(fDelta);

	m_pCurrentGameState->Update(fDelta);

	PFI_B(202, "XGame::Update : m_ObjectManager.Update");
	PFC_B("XGame::Update : m_ObjectManager.Update");
	m_ObjectManager.Update(fDelta);
	PFC_E;
	PFI_E(202);

	PFI_B(203, "XGame::Update : global.emgr->OnUpdate");
	PFC_B("XGame::Update : global.emgr->OnUpdate");
	global.emgr->OnUpdate(fDelta);		//����Ʈ ������ƾ�� ������.
	PFC_E;
	PFI_E(203);

	PFI_B(204, "XGame::Update : UpdateObjectCache");
	PFC_B("XGame::Update : UpdateObjectCache");
	UpdateObjectCache();
	PFC_E;
	PFI_E(204);

	PFI_B(205, "XGame::Update : ��Ÿ");
	PFC_B("XGame::Update : ��Ÿ");

	UpdateMyInfo(fDelta);

	m_pCurrentGameState->UpdateEnvSound();

	UpdateFeedbackObjects(fDelta);
	//if (gvar.World.nDynamicFieldExpiredTime > 0)
	//{
	//	XRemainTime	time;
	//	time.Set(gvar.World.nDynamicFieldExpiredTime-global.system->GetTimer().GetNowServerTime());

	//	if (!time.bTimeover)
	//	{
	//		mlog("XGame::Update - [%s] �����ð�: %d�ð� %d�� %d�� %s\n",
	//			"���̳��� �ʵ� ��������",
	//			time.nHour, time.nMin, time.nSec, time.bTimeover?"[�ð��ʰ�]":"");
	//	}
	//}

	if (global.res)
		global.res->UpdateCaching(fDelta);

	// ĳ���Ͱ� �̵��� ��ŭ ī�޶� �̵������ش�
	gg.controller->UpdateCameraAfterGameUpdate(fDelta);

	PFC_E;
	PFI_E(205);
}

void XGame::OnPreRender()
{
	if ((!m_pCurrentGameState->GetWorld()) || (!m_pCurrentGameState->GetWorld()->GetInfo())) return;
	//REffect *pEffect = m_pWorld->GetWorldEnv()->GetCurrentSky().m_pEffect;
}

void XGame::OnRender()
{
	PFC_THISFUNC;
	PFI_BLOCK_THISFUNC(206);

	m_ObjectManager.Render();

	if(m_pCurrentGameState)
		m_pCurrentGameState->Render();
}

void XGame::UpdateObjectCache()
{
	if (!m_pCurrentGameState->GetWorld()) return;
	if (!m_bEnableUpdateObjectCache) return;

	// 3�ʸ��� ������Ʈ
	const unsigned int OBJECT_CACHE_TICK = 3000;
	unsigned int nNowTime = XGetNowTime();
	if ((nNowTime - m_nUpdateObjectCacheTime) < OBJECT_CACHE_TICK) return;
	m_nUpdateObjectCacheTime = nNowTime;

	XObject* pMyObject = m_ObjectManager.Find(XGetMyUID());
	if (pMyObject == NULL) return;

	vec3 my_pos = pMyObject->GetPosition();
	MPoint my_pt = m_pCurrentGameState->GetWorld()->GetSectorIndex(my_pos.x, my_pos.y);

	for (XObjectMap::iterator itor = m_ObjectManager.BeginItor(); itor != m_ObjectManager.EndItor();)
	{
		XObject* pObj = itor->second;
		if ( pObj == pMyObject || !pObj->IsModuleEntity() )
		{
			++itor;
			continue;
		}

		vec3 pos = pObj->GetPosition();

		MPoint tar_pt = m_pCurrentGameState->GetWorld()->GetSectorIndex(pos.x, pos.y);
		if (XWorld::IsManagedSector(tar_pt.x, tar_pt.y, my_pt.x, my_pt.y, pObj->IsHuge()?HUGE_SECTOR_SCALE:1))
		{
			// ���ͳ��� ���̶��
			++itor;
		}
		else
		{
			// ���ͳ��� ���� �ƴ϶�� �����ش�
			itor = m_ObjectManager.Erase(pObj);

			if( pMyObject->GetModuleMyControl() )
			{
				XActor* pActor = static_cast<XActor*>(pObj);
				if( pActor )
					pMyObject->GetModuleMyControl()->ClearLockOnTarget( pActor->GetUIID() );
			}

			pObj->Destroy();
			pObj->Drop();
		}
	}
}

void XGame::UpdateMyInfo( float fDelta )
{
	gvar.MyInfo.Talent.CoolTime.Update( fDelta);
}

void XGame::OnWorldLoadingComplete()
{
	m_pCurrentGameState->OnWorldLoadingComplete();
}

void XGame::ChangeField( int nNewFieldID, vec3 vMyPos, vec3 vMyDir )
{
	m_pCurrentGameState->ChangeField(nNewFieldID, vMyPos, vMyDir);
}

/*
	�ҿ� ������Ʈ ����

	pOwnerNpc = �ҿ��� ������ų NPC ������
	uidTarget = �ҿ��� ��� �� PC�� UID
	nType = �ҿ� Ÿ��(�⺻ 0)
	nWaitTime = �ҿ��� �����Ǳ� ���� ���� �ð�(ms����)
*/

void XGame::UpdateFeedbackObjects( float fDelta )
{
	if (global.feedback)
	{
		global.feedback->Update(fDelta);
	}
}

void XGame::ChangeState( GAME_PLAY_STATE eState, int nParam /*= 0*/ )
{
	m_pCurrentGameState = m_GameStateMgr.ChangeGameStateStart(eState, nParam);
	gg.currentgamestate = m_pCurrentGameState;
	gg.controller		= m_pCurrentGameState->GetController();
}

bool XGame::OnEvent( MEvent& e )
{
	if (gg.controller && gg.controller->OnEvent(e)) 
		return true;

	return false;
}

void XGame::OnActive( bool bActive )
{
	if (gg.controller) gg.controller->OnActive(bActive);
}

void XGame::OnChangeStart( GAME_PLAY_STATE ePlayState, int nParam /*= 0*/ )
{
	ChangeState(ePlayState, nParam);
}

void XGame::OnChangeEnd()
{
	m_GameStateMgr.ChangeGameStateEnd();
}

bool XGame::OnForceEvent( MEvent& e )
{
	if (gg.controller && gg.controller->OnFoceEvent(e)) 
		return true;

	return false;
}

bool XGame::Check18Over()
{
	// 2010.7.19 - Ŭ��1���� 15���� ������ �����Ѵ�. �系 �׽�Ʈ�� 15���� �Ѵ�. ��, const.lua���� �����ϸ� 18�� �ν�
	// ���Ŀ��� �⵵�� üũ
	// 2010.7.22 - �Ǹ����� ���ӽ� 18���� �����Ѵ�.
	// 2010.7.23 - ������ 18����... ���Ǹ� 18���� �޴´ٰ� �մϴ�.
	return true;

	// �Ǹ� ����
	//if(gvar.Network.PmangInfo.isLoginOnPmang)
	//{
	//	return gvar.Network.PmangInfo.isAdult;
	//}

	// �系 ����
	if(XCONST::PLAYER_AGE_LIMIT == AL_18_OVER)
		return true;

	return false;
}
