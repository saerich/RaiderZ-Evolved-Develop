#include "StdAfx.h"
#include "XEventID.h"
#include "XModuleUI.h"
#include "XModuleEntity.h"
#include "XModuleMotion.h"
#include "XModuleAction.h"
#include "XCharacterCaption.h"
#include "XNonPlayer.h"
#include "XMyPlayer.h"
#include "XUIBalloonTextBox.h"
#include "XDebugScreenDialog.h"
#include "MMainFrame.h"
#include "XCharacter.h"
#include "RMesh.h"
#include "XGameState.h"
#include "XInteractionHelper.h"
#include "MockInteractionEffect.h"
#include "XMiniHpBar.h"
#include "XCaptionMgr.h"
#include "XHeadIcon.h"
#include "XCheckTargetableEffect.h"
#include "XGameFrameManager.h"
#include "XPlayFrame.h"
#include "XPlayer.h"


XModuleUI::XModuleUI( XObject* pOwner/*=NULL*/ )
: XModule(pOwner)
, m_bShowPartyMiniHpBar(false)
{
	m_pNameCaption				= NULL;	// character name
	m_pGuildCaption				= NULL;
	m_pMiniBar					= NULL;

	m_pDebugScreenDialog		= NULL;
	m_pBalloonTextBox			= NULL;

	m_pHeadIconPlacer			= NULL;

	m_pCheckTargetableEffect	= new XCheckTargetableEffect(pOwner);

	m_PaneRegulator.SetTickTime(0.2f);
}

XModuleUI::~XModuleUI(void)
{
	Destroy();

	SAFE_DELETE(m_pCheckTargetableEffect);
	SAFE_DELETE(m_pHeadIconPlacer);
}

void XModuleUI::Destroy()
{
	m_pDebugScreenDialog		= NULL;
	m_pBalloonTextBox			= NULL;
}

void XModuleUI::OnInitialized()
{
	XModule::OnInitialized();

	m_pModuleEntity = m_pOwner->GetModuleEntity();
	_ASSERT(m_pModuleEntity);
}

void XModuleUI::OnActivate()
{
	if( global.ui && m_pNameCaption == NULL )
	{
		XCaptionMgr* pCharacterCaptionMgr = global.ui->GetCaptionMgr();
		m_pNameCaption = pCharacterCaptionMgr->InsertCaption<XCharacterCaption>(m_pOwner->GetActor());
		_ASSERT( m_pNameCaption );

		m_pGuildCaption = pCharacterCaptionMgr->InsertCaption<XCharacterCaption>(m_pOwner->GetActor());
		_ASSERT( m_pGuildCaption );

		if(m_pMiniBar == NULL)
		{
			m_pMiniBar = pCharacterCaptionMgr->InsertCaption<XMiniHpBar>(m_pOwner->GetActor());
			_ASSERT( m_pMiniBar );
			m_pMiniBar->Create();
		}

		m_pNameCaption->SetVisible( false);
		m_pGuildCaption->SetVisible( false);

		if (m_pMiniBar && XGetMyUID() != m_pOwner->GetUID())
		{
			m_pMiniBar->SetVisible(true);
		}
	}

	if (m_pHeadIconPlacer == NULL)
	{
		m_pHeadIconPlacer = new XHeadIconPlacer(m_pNameCaption, m_pGuildCaption, m_pMiniBar);

		if(m_pOwner->GetEntityType() == ETID_PLAYER)
		{
			m_pHeadIconPlacer->SetCaptionOffset(PLAYER_ID_OFFSET_Z,
				m_pNameCaption,
				m_pGuildCaption,
				m_pMiniBar);
		}
	}
}

void XModuleUI::OnDeactivate()
{
	XCaptionMgr* pCharacterCaptionMgr = global.ui->GetCaptionMgr();

	if( m_pNameCaption != NULL )
	{
		pCharacterCaptionMgr->RemoveCaption( m_pNameCaption->GetID() );
		m_pNameCaption = NULL;
	}

	if (m_pGuildCaption != NULL)
	{
		pCharacterCaptionMgr->RemoveCaption( m_pGuildCaption->GetID() );
		m_pGuildCaption = NULL;
	}

	if(m_pMiniBar != NULL)
	{
		pCharacterCaptionMgr->RemoveCaption( m_pMiniBar->GetID() );
		m_pMiniBar = NULL;
	}

	m_pCheckTargetableEffect->Destroy();
}

void XModuleUI::OnSubscribeEvent()
{
	XModule::OnSubscribeEvent();

	Subscribe(XEVTL_MOTION_PLAYONCE_NPC_DIE);
}

void XModuleUI::SetNPCNamePane()
{
	if(m_pOwner->GetEntityType() == ETID_NPC)
	{
		SetNameColor(DIE_MONSTER_ID_COLOR);

		XNonPlayer* pThisNPC = AsNPC(GetOwner());
		XNPCInfo* pThisNPCInfo = pThisNPC->GetInfo(true);
		SetCharacterPane_NPC(pThisNPCInfo->bShowName, pThisNPCInfo->GetName(), pThisNPCInfo->GetClan(), pThisNPC->GetLevel(), pThisNPC->IsMonster());
	}
}

XEventResult XModuleUI::OnEvent( XEvent& msg )
{
	switch (msg.m_nID)
	{
	case XEVTL_MOTION_PLAYONCE_NPC_DIE:
		{
			SetNPCNamePane();

			//���ִ̾ϸ��̼��� ������ �������ͷ�Ƽ������Ʈ ǥ�ð� �����ϵ���.
			m_pCheckTargetableEffect->SetShowLootbleTargetEffect(true);
		}
		return MR_TRUE;
	}

	return MR_FALSE;
}

void XModuleUI::OnUpdate( float fDelta )
{
	PFC_THISFUNC;

	if ( global.gfmgr  &&  global.gfmgr->GetCurrFrameID() == PLAY_FRAME_ID)
	{
		// �ε�ȭ���̶��...?
		XPlayFrame* pPlayFrame = static_cast<XPlayFrame*>(global.gfmgr->GetCurrFrame());
		if(pPlayFrame->IsLoadingFrame())
		{
			// Ÿ�� ���� ����Ʈ ����
			m_pCheckTargetableEffect->DisableTargetEffect(m_pOwner);
		}
		else
		{
			UpdateCharacterPane(fDelta);

			m_pCheckTargetableEffect->Update(m_pOwner);

			UpdateNPCMiniHpBar();

			if(UpdatePlayerMiniHpBar() == false)
			{
				UpdatePartyMiniHpBar();
			}
		}
	}
}

void XModuleUI::UpdateCharacterPane(float fDelta)
{
	if(	m_pNameCaption == NULL ) return;

	if (m_pOwner->GetActor()->CheckLoadingCompletedFastly() == false) return;

	// [11/30/2007 isnara] 
	// ������ �Ÿ��� üũ�ؼ� 2.5���� �����δ� �̸��� ǥ�� �մϴ�.
	// NPC�� üũ �մϴ�. �ٸ� ĳ���ʹ� �Ÿ��� ������� ���̰� �մϴ�.
	if(m_pOwner->GetEntityType() == ETID_NPC)
	{
		XNonPlayer* pThisNPC = AsNPC(GetOwner());
		XNPCInfo* pThisNPCInfo = pThisNPC->GetInfo(true);

		UpdateNPCPaneVisible(fDelta, pThisNPCInfo);

		if(pThisNPC)
		{
			// NPC ������ Ÿ��Ʋ ��ġ ������ �ִٸ�...
			UpdateNPCNameCaption(pThisNPCInfo->strTitleBone, pThisNPCInfo->fTitleHeight);
			return;
		}
	}
	else if (m_pOwner->GetEntityType() == ETID_PLAYER)
	{
		UpdatePlayerPaneVisible(fDelta);

		UpdatePlayerNameCaption();
	}
}

void XModuleUI::UpdateNPCNameCaption(wstring strTitleBoneName, float fTitleHeight)
{
	if (m_pHeadIconPlacer)
	{
		m_pHeadIconPlacer->UpdateNPCNameCaption(m_pOwner->GetActor(),
			strTitleBoneName,
			fTitleHeight,
			m_pOwner->GetScale());
	}
}

void XModuleUI::UpdatePlayerNameCaption()
{
	if (m_pHeadIconPlacer && m_pOwner->GetEntityType() == ETID_PLAYER)
	{
		m_pHeadIconPlacer->UpdatePlayerNameCaption();
	}
}

vec3 XModuleUI::GetNamePos()
{
	return m_pNameCaption->GetPosition();
}

void XModuleUI::SetNameColor( DWORD dwColor )
{
	if(m_pNameCaption)
	{
		m_pNameCaption->SetColor(dwColor);
	}
	if(m_pGuildCaption)
	{
		m_pGuildCaption->SetColor(dwColor);
	}
}

void XModuleUI::SetCharacterPane( wstring& strName, wstring &strClan, DWORD dwColor)
{
	if (m_pNameCaption)
	{
		m_pNameCaption->SetText(strName);

		//by pok. �����ϼ��� �߰�
		m_pNameCaption->SetScale(CHARACTER_PANE_SCALE);
	}

	if (m_pGuildCaption)
	{
		if (!strClan.empty() )
		{
			wstring sz = L"<" + strClan + L">";
			m_pGuildCaption->SetText(sz);
			m_pGuildCaption->SetScale(CHARACTER_PANE_SCALE);
		}
		else
		{
			if (!m_pGuildCaption->GetText().empty())
			{
				m_pGuildCaption->SetText(L"");
			}
		}
	}

	SetNameColor(dwColor);
}

void XModuleUI::SetCharacterPane_NPC(bool bShowName, wstring strName, wstring strClan, const int& nLevel, bool bMonster)
{
	if (m_pNameCaption && bShowName)
	{
		// by pok, 6.18 �䱸���׿� ���� ����ũ�⸦ 20���� �ٿ��� (0.6 -> 0.5)
		// by pok, 6.20 ����ũ�⸦ 20���� �� �ٿ��� (0.5 -> 0.4)
		wstring strFullName = strName;
		m_pNameCaption->SetScale(CHARACTER_PANE_SCALE);
		m_pNameCaption->SetText(strFullName);

#ifdef _DEBUG
		wchar_t szUID[512];
		swprintf_s(szUID, L"%I64u", m_pOwner->GetUID().Value);
		m_pNameCaption->AppendText(L"(");
		m_pNameCaption->AppendText(szUID);
		m_pNameCaption->AppendText(L")");
#endif
	}

	if (bShowName && m_pGuildCaption && !strClan.empty())
	{
		wstring sz = L"<" + strClan + L">";
		m_pGuildCaption->SetText(sz);
		m_pGuildCaption->SetScale(CHARACTER_PANE_SCALE);
	}
}

const char DEFAULTNAME_LAYERZERO[] = "layerGame0";

float XModuleUI::GetBallonLifeTime(float fMaintainTime, wstring &szChat)
{
	float fLifeTime;
	if ( fMaintainTime > 0.1f)			fLifeTime = fMaintainTime;
	else if ( szChat.length() > 25)		fLifeTime = 10.0f;
	else								fLifeTime = 5.0f;

	return fLifeTime;
}

void XModuleUI::OnBallonText(wstring szID, wstring szChat, float fMaintainTime)
{
	if (m_pBalloonTextBox == NULL)
	{
		MWidget* pWidget = global.mint->FindWidget( DEFAULTNAME_LAYERZERO);
		if ( pWidget)
		{
			m_pBalloonTextBox = new XUIBalloonTextBox( NULL, pWidget, pWidget);
			m_pBalloonTextBox->SetOwner( m_pOwner->GetUID());
		}
	}


	if ( m_pBalloonTextBox != NULL)
	{
		float fLifeTime = GetBallonLifeTime(fMaintainTime, szChat);
		m_pBalloonTextBox->PopupBalloon( szChat, fLifeTime);

		if ( m_pOwner->GetEntityType() == ETID_NPC)
		{
			XNonPlayer* pNPC = AsNPC(m_pOwner);
			if(pNPC && pNPC->GetIConState() != NIS_NONE)
				pNPC->DestroyNPCIconEffect();
		}
	}
}

void XModuleUI::OnBallonText_Debug(wstring szID, wstring szChat)
{
	if (m_pDebugScreenDialog == NULL)
	{
		m_pDebugScreenDialog = new XDebugScreenDialog(m_pOwner, MINT_ONDEBUGSCREENPANEL, global.mint->GetMainFrame(), NULL);
	}

	m_pDebugScreenDialog->OnAir(szID, szChat);
}

bool XModuleUI::IsShowBallonText()
{
	if (m_pBalloonTextBox)
	{
		return m_pBalloonTextBox->GetShow();
	}

	return false;
}

void XModuleUI::SetCaptionVisible(bool bShow)
{
	bool bShowName = true, bShowGuild = true;


	// �ɼǿ� ���� ĸ���� ���̰ų� ��
	if ( bShow == false)
	{
		bShowName = false;
		bShowGuild = false;
	}
	else if (XGetMyUID() == m_pOwner->GetUID())
	{
		bShowName = XGETCFG_GAME_MYPLAYERNAME;
		bShowGuild = XGETCFG_GAME_MYPLAYERGUILD;
	}
	else if ( m_pOwner->GetEntityType() == ETID_PLAYER)
	{
		bShowName = XGETCFG_GAME_PLAYERNAME;
		bShowGuild = XGETCFG_GAME_PLAYERGUILD;
	}
	else if ( m_pOwner->GetEntityType() == ETID_NPC)
	{
		bShowName = XGETCFG_GAME_NPCNAME;
		bShowGuild = XGETCFG_GAME_NPCGUILD;
	}


	m_pNameCaption->SetVisible( bShowName);
	m_pGuildCaption->SetVisible( bShowGuild);

	if (m_pHeadIconPlacer)
	{
		m_pHeadIconPlacer->CheckRefresh(m_pOwner);
	}
}

bool XModuleUI::UpdateNPCMiniHpBar()
{
	if (m_pOwner->GetEntityType() != ETID_NPC)	return false;
	if (m_pNameCaption == NULL)					return false;

	XActor* pActor = m_pOwner->AsActor();

	TD_ENEMY_INFO* pEnemyInfo = gvar.Game.EnemyInfo.GetEnemyInfoByUIID(pActor->GetUIID());

	// ������ ������ NPC�ΰ�?
	bool bAttackable = true;
	bool bShowHpBar = true;
	if(pEnemyInfo)
	{
		XNonPlayer* pNPC = gg.omgr->FindNPC_UIID(pEnemyInfo->nUIID);
		if(pNPC)
		{
			bAttackable = pNPC->IsAttackable();

			XNPCInfo* pNPCInfo = pNPC->GetInfo(true);
			if(pNPCInfo)
				bShowHpBar = pNPCInfo->bShowHpBar;
		}
	}

	if(pEnemyInfo == NULL || bAttackable == false || bShowHpBar == false)
	{
		m_pMiniBar->SetActive(false);
		return false;
	}

	m_pMiniBar->SetActive(true);

	int nHpPercent = pEnemyInfo->nHPPercent;
	m_pMiniBar->UpdateHpData(nHpPercent);

	return true;
}

bool XModuleUI::UpdatePlayerMiniHpBar()
{
	if (m_pOwner->GetEntityType() != ETID_PLAYER)	return false;
	if (m_pOwner->GetUID() == XGetMyUID())			return false;
	if (m_pNameCaption == NULL)						return false;

	XPlayer* pPlayer = m_pOwner->AsPlayer();
	if (XGetMyPlayer()->IsEnemy(pPlayer) == true)
	{
		//TODO : �������� ������� hp ������ ��� ������� �Ѵ�.
		int nHpPercent = 100;

		TD_ENEMY_INFO* pEnemyInfo = gvar.Game.EnemyInfo.GetEnemyInfoByUIID(pPlayer->GetUIID());
		if (pEnemyInfo)
		{
			nHpPercent = pEnemyInfo->nHPPercent;
		}

		// ÷ ���� ���۽ÿ� �̴Ϻ��� �Ⱥ��̴� ������ �־ ��¥ ����Ÿ�� ����
		m_pMiniBar->SetActive(true);
		m_pMiniBar->UpdateHpData(nHpPercent);
		return true;
	}
	else
	{
		TD_ENEMY_INFO* pEnemyInfo = gvar.Game.EnemyInfo.GetEnemyInfoByUIID(pPlayer->GetUIID());
		if (pEnemyInfo)
		{
			int nHpPercent = pEnemyInfo->nHPPercent;
			m_pMiniBar->SetActive(true);
			m_pMiniBar->UpdateHpData(nHpPercent);
			return true;
		}
	}

	m_pMiniBar->SetActive(false);
	return false;
}

bool XModuleUI::IsShowLootbleTargetEffect() const
{
	return m_pCheckTargetableEffect->IsShowLootbleTargetEffect();
}

void XModuleUI::UpdateNPCPaneVisible( float fDelta, XNPCInfo* pThisNPCInfo )
{
	if (!m_PaneRegulator.IsReady(fDelta))		return;

	// ������ �Ÿ��� üũ�ؼ� ���� �Ÿ� �ȿ����� �̸��� ǥ���Ѵ�.
	XObject * pObject = gvar.Game.pMyPlayer;
	if(pObject)
	{
		float fLenSq = (pObject->GetPosition() - m_pOwner->GetPosition()).LengthSq();

		//��ǳ���� �߸� �̸��� �ȱ׸���.
		if (IsShowBallonText())
		{
			SetCaptionVisible(false);
		}
		else
		{
			float fShowNameDistanceSq = SHOW_NAME_DISTANCE*SHOW_NAME_DISTANCE;

			if(pThisNPCInfo->bShowName && (fLenSq <= fShowNameDistanceSq))
			{
				SetCaptionVisible(true);
			}
			else
			{
				SetCaptionVisible(false);
			}
		}
	}
}

void XModuleUI::UpdatePlayerPaneVisible( float fDelta )
{
	if (!m_PaneRegulator.IsReady(fDelta))		return;

	if (IsShowBallonText())
	{
		SetCaptionVisible(false);
	}
	else
	{
		SetCaptionVisible(true);
	}
}

void XModuleUI::UpdatePartyMiniHpBar()
{
	if (m_pOwner->GetEntityType() != ETID_PLAYER)	return;

	if (m_pOwner->GetUID() == XGetMyUID() ||
		m_pMiniBar == NULL ||
		m_bShowPartyMiniHpBar == false ||
		gvar.MyInfo.Party.IsPartyMember(m_pOwner->GetUID()) == false)
	{
		m_pMiniBar->SetActive(false);
		return;
	}

	int nHpPercent = gvar.MyInfo.Party.GetPartyMemberHP(m_pOwner->GetUID());

	m_pMiniBar->SetActive(true);
	m_pMiniBar->UpdateHpData(nHpPercent);
}