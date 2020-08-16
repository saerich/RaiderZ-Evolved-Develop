#include "stdafx.h"
#include "XModuleBuff.h"
#include "XModuleEffect.h"
#include "XModuleAction.h"
#include "XModuleEntity.h"
#include "XModuleTalent.h"
#include "XNonPlayer.h"
#include "XTalentInfoMgr.h"
#include "XModuleMyControl.h"
#include "XWidgetNameDef.h"
#include "XEventID.h"
#include "XStrings.h"

//////////////////////////////////////////////////////////////////////////
void XBuffEnchant::Push( int nBuffID )
{
	m_vecBuffEnchant.push_back(nBuffID);
}

void XBuffEnchant::Pop( int nBuffID )
{
	for(vector<int>::iterator it = m_vecBuffEnchant.begin(); it != m_vecBuffEnchant.end(); ++it)
	{
		if(*it == nBuffID)
		{
			m_vecBuffEnchant.erase(it);
			return;
		}
	}
}

void XBuffEnchant::ParseBuffAttr( XBuffAttribute& buffAttribue )
{
	for(vector<int>::iterator it = m_vecBuffEnchant.begin(); it != m_vecBuffEnchant.end(); ++it)
	{
		XBuffInfo * pBuffInfo = info.buff->Get(*it);
		if(pBuffInfo)
			buffAttribue.ParseBuffAttr(pBuffInfo);
	}
}

bool XBuffEnchant::CheckEnchantBuffID( int nBuffID )
{
	for(vector<int>::iterator it = m_vecBuffEnchant.begin(); it != m_vecBuffEnchant.end(); ++it)
	{
		if(*it == nBuffID)
			return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void XBuffInstant::PushInstant( int nBuffID )
{
	map<int, int>::iterator itFind = m_mapBuffInstantCount.find(nBuffID);
	if(itFind != m_mapBuffInstantCount.end())
	{
		int nCount = itFind->second;
		++nCount;
		itFind->second = nCount;
	}
	else
	{
		m_mapBuffInstantCount.insert(map< int, int>::value_type(nBuffID, 1));
	}
}

void XBuffInstant::PopInstant( int nBuffID )
{
	map<int, int>::iterator itFind = m_mapBuffInstantCount.find(nBuffID);
	if(itFind != m_mapBuffInstantCount.end())
		m_mapBuffInstantCount.erase(itFind);

	return;
}

int XBuffInstant::GetInstantCount( int nBuffID )
{
	map<int, int>::iterator itFind = m_mapBuffInstantCount.find(nBuffID);
	if(itFind != m_mapBuffInstantCount.end())
		return itFind->second;

	return 0;
}

void XBuffInstant::ParseBuffAttr( XBuffAttribute& buffAttribue )
{
	for(map<int, int>::iterator itFind = m_mapBuffInstantCount.begin(); itFind != m_mapBuffInstantCount.end(); ++itFind)
	{
		XBuffInfo * pBuffInfo = info.buff->Get(itFind->first);
		if(pBuffInfo)
			buffAttribue.ParseBuffAttr(pBuffInfo, itFind->second);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////


XModuleBuff::XModuleBuff(XObject* pOwner /* = NULL */)
: XModule(pOwner)
{
	m_bLoadingComplete = false;
	m_eSavePaletteIndex = PALETTENUM_MAX;
}

XModuleBuff::~XModuleBuff(void)
{
	DestroyBuff();
}

void XModuleBuff::OnInitialized()
{

}

void XModuleBuff::OnSubscribeEvent()
{
	Subscribe(XEVTD_MESH_LOADING_COMPLETE);
}

XEventResult XModuleBuff::OnEvent( XEvent& msg )
{
	switch(msg.m_nID)
	{
	case XEVTD_MESH_LOADING_COMPLETE:
		{
			m_bLoadingComplete = true;
			ReSetRemainBuffList();
		}
		break;
	}

	return MR_FALSE;
}

void XModuleBuff::OnUpdate( float fDelta )
{
	PFC_THISFUNC;
}

bool _IsDeactivateBuff( XBuffInfo * pBuffInfo)
{
	if ( pBuffInfo->m_nPassiveExtraAttrib == BUFPEA_ROOT  ||  pBuffInfo->m_nPassiveExtraAttrib == BUFPEA_SLEEP  ||
		pBuffInfo->m_nPassiveExtraAttrib2 == BUFPEA_ROOT  ||  pBuffInfo->m_nPassiveExtraAttrib2 == BUFPEA_SLEEP)
		return true;

	return false;
}

void XModuleBuff::BuffGain( int nBuffID, float fLostRemained, bool bRemainBuffGain /*= false*/ )
{
	XBuffInfo * pBuffInfo = info.buff->Get(nBuffID);
	if(pBuffInfo == NULL)
		return;

	//------------------------------------------------------------------------
	// ���� ����
	CheckBuffDuplication(nBuffID);

	AddBuff(nBuffID, pBuffInfo->m_nStackType, fLostRemained);

	if (pBuffInfo->m_bUseChangePalette)
	{
		ChangePalette(pBuffInfo);
	}

	//------------------------------------------------------------------------
	// ���� ����
	SetGainBuffAnimation(pBuffInfo);

	CheckInvisibilityGain(pBuffInfo);
	
	// Modifier �� ���ϱ�
	GainMaintainEffect(pBuffInfo);

	//------------------------------------------------------------------------
	// ����Ʈ ����
	XModuleEffect * pEffect = m_pOwner->GetModuleEffect();
	if(pEffect)
	{
		XBuffEffectEventData* pEventData = new XBuffEffectEventData;
		pEventData->m_nParam1			= nBuffID;
		pEventData->m_eEffectType		= BUFF_GAIN_EFFECT;
		pEventData->m_bRemainBuffGain	= bRemainBuffGain;
		pEffect->OnEffectEvent(XEFTEVT_EFFECT_BUFF, pEventData);
	}

	//------------------------------------------------------------------------
	// UI ����
	ActivedBuffUI(pBuffInfo, fLostRemained);

	//------------------------------------------------------------------------
	// ��� ������
	if(m_pOwner->GetEntityType() == ETID_PLAYER)
	{
		for(int i = 0; i < ITEMSLOT_MAX; ++i)
		{
			int nItemID = pBuffInfo->m_nEquipItemIDs[i];
			m_pOwner->AsPlayer()->EquipOverlapped(nItemID);
		}
	}
}

void XModuleBuff::BuffLost( int nBuffID )
{
	XBuffInfo * pBuffInfo = info.buff->Get(nBuffID);
	if(pBuffInfo == NULL)
		return;

	//------------------------------------------------------------------------
	// ���� ����
	int nStackCount = GetStackBuffCount(nBuffID);
	int nInstantCount = m_BuffInstant.GetInstantCount(nBuffID);

	SubBuff(nBuffID);

	if (pBuffInfo->m_bUseChangePalette)
	{
		RestorePalette(pBuffInfo);
	}

	m_BuffInstant.PopInstant(nBuffID);

	//------------------------------------------------------------------------
	// ���� ����
	if ( _IsDeactivateBuff( pBuffInfo) == true)
	{
		XModuleMotion* pModuleMotion = m_pOwner->GetModuleMotion();
		if (pModuleMotion) pModuleMotion->RemoveIdleMemory(nBuffID);

		// �� ĳ���ʹ� ��Ȱ���� ������ �ɸ��� ������Ʈ�� ������
		if (  m_pOwner->GetUID() == XGetMyUID())
		{
			XModuleMyControl* pControl = m_pOwner->GetModuleMyControl();
			if ( pControl)
				pControl->DoActionIdleWithSpecialState();
		}

		else
			StopAnimation(pBuffInfo);
	}


	// �ִϸ��̼� ���̻�
	if (!pBuffInfo->m_strAniPostfix.empty())
	{
		XModuleMotion* pModuleMotion = m_pOwner->GetModuleMotion();
		if (pModuleMotion)
		{
			pModuleMotion->ReleaseAnimationNamePostfix();
			pModuleMotion->RefreshThisMotion();
		}
	}

	// Modifier �� ����
	// ���� ���ȸ�ŭ ����.
	LostMaintainEffect(pBuffInfo, nStackCount + nInstantCount);

	//------------------------------------------------------------------------
	// ����Ʈ ����
	XModuleEffect * pEffect = m_pOwner->GetModuleEffect();
	if(pEffect)
	{
		XBuffEffectEventData* pEventData = new XBuffEffectEventData;
		pEventData->m_nParam1			= nBuffID;
		pEffect->OnEffectEvent(XEFTEVT_EFFECT_BUFF_END, pEventData);
	}
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// ����ȭ ����
	CheckInvisibilityLost(pBuffInfo);

	// UI ����
	DeactivedBuffUI(pBuffInfo);

	//------------------------------------------------------------------------
	// ��� ������
	if(m_pOwner->GetEntityType() == ETID_PLAYER)
	{
		for(int i = 0; i < ITEMSLOT_MAX; ++i)
		{
			int nItemID = pBuffInfo->m_nEquipItemIDs[i];
			m_pOwner->AsPlayer()->UnEquipOverlapped(nItemID);
		}
	}
}

void XModuleBuff::ChangePalette( XBuffInfo * pBuffInfo )
{
	if (!pBuffInfo ||
		m_pOwner->GetType() != XOBJ_MY_PLAYER)
		return;

	m_eSavePaletteIndex = gvar.MyInfo.PaletteList.GetCurPaletteListIndex();
	gvar.MyInfo.PaletteList.SetCurPaletteListIndex(PALETTENUM_1);

	gvar.MyInfo.PaletteList.ChangePalette(pBuffInfo->m_arrPalettes);
	global.ui->PaletteUIRefresh( true, true);

	// �ŷ�Ʈ ��� �� �� �ְ� ����
	gvar.MyInfo.Talent.SetBuffTalentID(pBuffInfo->m_arrPalettes);
}

void XModuleBuff::RestorePalette( XBuffInfo * pBuffInfo )
{
	if (!pBuffInfo ||
		m_pOwner->GetType() != XOBJ_MY_PLAYER)
		return;

	gvar.MyInfo.PaletteList.RestorePalette();

	if(m_eSavePaletteIndex >= PALETTENUM_1 && m_eSavePaletteIndex < PALETTENUM_MAX)
		gvar.MyInfo.PaletteList.SetCurPaletteListIndex(m_eSavePaletteIndex);

	global.ui->PaletteUIRefresh( true, false);
	m_eSavePaletteIndex = PALETTENUM_MAX;

	// �ŷ�Ʈ ����
	gvar.MyInfo.Talent.ReSetBuffTalentID();
}

void XModuleBuff::UseAnimation( XBuffInfo * pBuffInfo )
{
	XModuleAction * pModuleAction = m_pOwner->GetModuleAction();
	if(pModuleAction == NULL)
	{
		return;
	}

	if(!pBuffInfo->m_strUseAniName.empty())
	{
		pModuleAction->StartBuffAnimation(pBuffInfo->m_nID, pBuffInfo->m_strUseAniName);
	}
}

void XModuleBuff::StopAnimation( XBuffInfo * pBuffInfo )
{	
	XModuleAction * pModuleAction = m_pOwner->GetModuleAction();
	if(pModuleAction == NULL)
	{
		return;
	}

	// �ִϸ��̼��� �־����͸� ������ �Ѵ�.
	if(!pBuffInfo->m_strUseAniName.empty())
	{
		pModuleAction->EndBuffAnimation(pBuffInfo->m_nID);
	}
}

void XModuleBuff::GainMaintainEffect( XBuffInfo * pBuffInfo)
{
	if(m_pOwner->GetUID() == XGetMyUID())
	{
		m_ModEffector.AddActorModifier(gvar.MyInfo.ChrStatus.ActorModifier, pBuffInfo->m_ActorModifier);
		m_ModEffector.AddActorModifier(gvar.MyInfo.ActorModifier_ExceptTalent, pBuffInfo->m_ActorModifier);
		m_ModEffector.AddPlayerModifier(gvar.MyInfo.PlayerModifier, pBuffInfo->m_PlayerModifier);	
		m_ModEffector.AddPlayerModifier(gvar.MyInfo.PlayerModifier_ExceptTalent, pBuffInfo->m_PlayerModifier);	
	}
}

void XModuleBuff::LostMaintainEffect( XBuffInfo * pBuffInfo, int nStack /*= 1*/ )
{
	if(m_pOwner->GetUID() == XGetMyUID())
	{
		for(int i = 0; i < nStack; ++i)
		{
			m_ModEffector.EraseActorModifier(gvar.MyInfo.ChrStatus.ActorModifier, pBuffInfo->m_ActorModifier);
			m_ModEffector.EraseActorModifier(gvar.MyInfo.ActorModifier_ExceptTalent, pBuffInfo->m_ActorModifier);
			m_ModEffector.ErasePlayerModifier(gvar.MyInfo.PlayerModifier, pBuffInfo->m_PlayerModifier);
			m_ModEffector.ErasePlayerModifier(gvar.MyInfo.PlayerModifier_ExceptTalent, pBuffInfo->m_PlayerModifier);
		}
	}
}

void XModuleBuff::GainInstantEffect(int nBuffID)
{
	XBuffInfo * pBuffInfo = info.buff->Get(nBuffID);
	if(pBuffInfo == NULL) return;

	int nGainHP = 0;
	int nGainEn = 0;

	if(m_pOwner->GetUID() == XGetMyUID())
	{
		int nHP = gvar.MyInfo.GetHP();
		int nEN = gvar.MyInfo.GetEN();
		int nSTA = gvar.MyInfo.GetSTA();

		m_ModEffector.ApplyInstantModidier(pBuffInfo->m_InstantModifier);

		// HP �ö󰥶��� ���� ����Ʈ�� ����.
		if(pBuffInfo->m_InstantModifier.nHP.IsModified())
		{
			nGainHP = gvar.MyInfo.GetHP() - nHP;
		}

		// EN �ö󰥶��� ���� ����Ʈ�� ����.
		if(pBuffInfo->m_InstantModifier.nEN.IsModified())
		{
			nGainEn = gvar.MyInfo.GetEN() - nEN;
		}
	}

	XModuleEffect * pEffect = m_pOwner->GetModuleEffect();
	if(pEffect)
	{
		XBuffEffectEventData* pEventData = new XBuffEffectEventData;
		pEventData->m_nParam1			= nBuffID;
		pEventData->m_eEffectType				= BUFF_HIT_EFFECT;
		pEffect->OnEffectEvent(XEFTEVT_EFFECT_BUFF, pEventData);

		if(nGainHP > 0)
		{
			XCaptionEffectEventData* pEventData = new XCaptionEffectEventData;
			pEventData->m_nParam1 = nGainHP;
			pEffect->OnEffectEvent(XEFTEVT_EFFECT_CAPTION_HEAL, pEventData);
		}

		if(nGainEn > 0)
		{
			XCaptionEffectEventData* pEventData = new XCaptionEffectEventData;
			pEventData->m_nParam1 = nGainEn;
			pEffect->OnEffectEvent(XEFTEVT_EFFECT_CAPTION_EN, pEventData);
		}
	}
}

void XModuleBuff::BuffHit( int nBuffID )
{
	XBuffInfo * pBuffInfo = info.buff->Get(nBuffID);
	if(pBuffInfo == NULL)
		return

	// Modifier �� ���ϱ�
	GainMaintainEffect(pBuffInfo);

	m_BuffInstant.PushInstant(nBuffID);

	XModuleEffect * pEffect = m_pOwner->GetModuleEffect();
	if(pEffect)
	{
		XBuffEffectEventData* pEventData = new XBuffEffectEventData;
		pEventData->m_nParam1			= nBuffID;
		pEventData->m_eEffectType				= BUFF_HIT_EFFECT;
		pEffect->OnEffectEvent(XEFTEVT_EFFECT_BUFF, pEventData);
	}
}

bool XModuleBuff::BuffExist( int nID )
{
	bool bExist = false;

	// Non plyer
	if ( m_pOwner->GetEntityType() == ETID_NPC)
	{
		XNonPlayer* pNonPlayer =  m_pOwner->AsNPC();
		bExist = pNonPlayer->GetBuffList().FindID( nID);
	}

	// My player
	else if ( m_pOwner->GetType() == XOBJ_MY_PLAYER )
	{
		bExist = gvar.MyInfo.BuffExist( nID) ? true : false;
	}

	// Net player
	else if ( m_pOwner->GetType() == XOBJ_NET_PLAYER )
	{
		XNetPlayer* pNetPlayer = m_pOwner->AsNetPlayer();
		bExist = pNetPlayer->GetBuffList().FindID( nID);
	}

	return bExist;
}

void XModuleBuff::AddBuff( int nBuffID, BUFF_STACK_TYPE stackType, float fLostRemained)
{
	// Non plyer
	if ( m_pOwner->GetEntityType() == ETID_NPC)
	{
		XNonPlayer* pNonPlayer = m_pOwner->AsNPC();
		if ( pNonPlayer->GetBuffList().FindID( nBuffID) == false)
			pNonPlayer->PushBuff( nBuffID);
	}

	// My player
	else if ( m_pOwner->GetType() == XOBJ_MY_PLAYER )
	{
		if ( gvar.MyInfo.BuffList.FindID( nBuffID) == false)
			gvar.MyInfo.BuffList.PushBackItem( nBuffID, stackType, fLostRemained);
	}

	// Net player
	else if ( m_pOwner->GetType() == XOBJ_NET_PLAYER )
	{
		XNetPlayer* pNetPlayer = m_pOwner->AsNetPlayer();
		if ( pNetPlayer->GetBuffList().FindID( nBuffID) == false)
			pNetPlayer->PushBuff( nBuffID);
	}
}

void XModuleBuff::SubBuff( int nBuffID )
{
	// Non plyer
	if ( m_pOwner->GetEntityType() == ETID_NPC)
	{
		XNonPlayer* pNonPlayer = m_pOwner->AsNPC();
		pNonPlayer->PopBuff( nBuffID);
	}

	// My player
	else if ( m_pOwner->GetType() == XOBJ_MY_PLAYER )
	{
		gvar.MyInfo.BuffList.PopItem( nBuffID);
	}

	// Net player
	else if ( m_pOwner->GetType() == XOBJ_NET_PLAYER )
	{
		XNetPlayer* pNetPlayer = m_pOwner->AsNetPlayer();
		pNetPlayer->PopBuff( nBuffID);
	}
}

void XModuleBuff::ClearBuff()
{
	// Non plyer
	if ( m_pOwner->GetEntityType() == ETID_NPC)
	{
	}

	// My player
	else if ( m_pOwner->GetEntityType() == ETID_PLAYER  &&  m_pOwner->GetUID() == XGetMyUID())
	{
		if ( global.script)		global.script->GetGlueGameEvent().OnGameEvent( "BUFF", "CLEAR");
	}

	// Net player
	else if ( m_pOwner->GetEntityType() == ETID_PLAYER)
	{
	}
}

bool XModuleBuff::IsFocusGain( int* pOutBuffID /*= NULL*/, int* pOutFocusType /*= NULL*/ )
{
	bool bFocus = false;

	for ( int i = 0; i < TFT_SIZE; i++)
	{
		if ( BuffExist( BUFF_FOCUS_ID[ i]) == false)
			continue;


		if ( pOutBuffID)
			*pOutBuffID = BUFF_FOCUS_ID[ i];

		if ( pOutFocusType)
			*pOutFocusType = i;

		bFocus = true;
		break;
	}

	return bFocus;
}

void XModuleBuff::ActivedBuffUI( XBuffInfo * pBuffInfo, float fLostRemained)
{
	//if ( global.script)
	//	global.script->GetGlueGameEvent().OnBuffRefresh();


	// My player
	if ( m_pOwner->GetType() == XOBJ_MY_PLAYER )
	{
		// �Ӹ� ���� ���� ǥ��
		if ( XGETCFG_GAME_SHOWBUFFNAME == true)
		{
			XModuleEffect* pModuleEffect = m_pOwner->GetModuleEffect();
			if ( pModuleEffect)
			{
				XBuffEffectEventData * pEventData = new XBuffEffectEventData;
				pEventData->m_strBuffName = pBuffInfo->GetName();
				pEventData->m_bIsDeBuff = (pBuffInfo->m_nType == BUFT_DEBUFF) ? true : false;
				pModuleEffect->OnEffectEvent(XEFTEVT_EFFECT_BUFF_CAPTION, pEventData);
			}
		}


		if ( global.script)		global.script->GetGlueGameEvent().OnGameEvent( "BUFF", "ADD", pBuffInfo->m_nID);

		// ���� ��� â�� ǥ��
// 		XUIBuffViewer* pWidget = (XUIBuffViewer*)global.mint->FindWidget( WIDGET_NAME_BUFF_VIEWER);
// 		if ( pWidget)
// 		{
// 			if ( pBuffInfo->m_nType == BUFT_BUFF)	pWidget->ActivedBuff( pBuffInfo->m_nID);
// 			else									pWidget->ActivedDebuff( pBuffInfo->m_nID);
// 		}

		// �����޼��� �и� _by tripleJ 110504
		if (pBuffInfo->m_bLogable)
		{
			wstring strMsg = FormatString(XGetStr(L"SMSG_BUFF_GAIN_MYPLAYER"), FSParam(pBuffInfo->GetName()));
			global.ui->OnBattleMsg(strMsg.c_str());
		}
	}
	else
	{
		// �����޼��� �и� _by tripleJ 110504
		if (pBuffInfo->m_bLogable)
		{
			wstring strMsg = FormatString(XGetStr(L"SMSG_BUFF_GAIN_OTHER"), FSParam(m_pOwner->GetName(), pBuffInfo->GetName()));
			global.ui->OnBattleMsg(strMsg.c_str());
		}
	}
}

void XModuleBuff::DeactivedBuffUI( XBuffInfo * pBuffInfo )
{
	//if ( global.script)
	//	global.script->GetGlueGameEvent().OnBuffRefresh();


	// My player
	if ( m_pOwner->GetType() == XOBJ_MY_PLAYER )
	{
		if ( global.script)		global.script->GetGlueGameEvent().OnGameEvent( "BUFF", "REMOVE", pBuffInfo->m_nID);

		// ���� ��� â���� ����
// 		XUIBuffViewer* pWidget = (XUIBuffViewer*)global.mint->FindWidget( WIDGET_NAME_BUFF_VIEWER);
// 		if ( pWidget)
// 		{
// 			if ( pBuffInfo->m_nType == BUFT_BUFF)	pWidget->DeactivedBuff( pBuffInfo->m_nID);
// 			else									pWidget->DeactivedDebuff( pBuffInfo->m_nID);
// 		}

		if (pBuffInfo->m_bLogable)
		{
			// �����޼��� �и� _by tripleJ 110504
			wstring strMsg = FormatString(XGetStr(L"SMSG_BUFF_LOST_MYPLAYER"), FSParam(pBuffInfo->GetName()));
			global.ui->OnBattleMsg(strMsg.c_str());
		}
	}
	else
	{
		if (pBuffInfo->m_bLogable)
		{
			// �����޼��� �и� _by tripleJ 110504
			wstring strMsg = FormatString(XGetStr(L"SMSG_BUFF_LOST_OTHER"), FSParam(m_pOwner->GetName(), pBuffInfo->GetName()));
			global.ui->OnBattleMsg(strMsg.c_str());
		}
	}

}

XBuffAttribute XModuleBuff::GetBuffAttribute()
{
	XBuffAttribute attr;

	// Non plyer
	if ( m_pOwner->GetEntityType() == ETID_NPC)
	{
		XNonPlayer* pNonPlayer =  m_pOwner->AsNPC();
		for each ( int nBuffID  in pNonPlayer->GetBuffList().m_vContains)
		{
			XBuffInfo* pBuffInfo = info.buff->Get( nBuffID);
			if ( pBuffInfo == NULL)  continue;
			attr.ParseBuffAttr( pBuffInfo);
		}
	}

	// My player
	else if ( m_pOwner->GetType() == XOBJ_MY_PLAYER )
	{
		for ( map< int, XMYBUFFLIST>::iterator itr = gvar.MyInfo.BuffList.m_vContains.begin();  itr != gvar.MyInfo.BuffList.m_vContains.end();  itr++)
		{
			XBuffInfo* pBuffInfo = info.buff->Get( (*itr).first);
			if ( pBuffInfo == NULL)  continue;

			int nStatckCount = itr->second.vecBuffItem.size();
			attr.ParseBuffAttr( pBuffInfo, nStatckCount);
		}
	}

	// Net player
	else if ( m_pOwner->GetEntityType() == ETID_PLAYER && MIsDerivedFromClass(XNetPlayer, m_pOwner))
		
	{
		int a = 0;
		XNetPlayer* pNetPlayer = m_pOwner->AsNetPlayer();
		for each ( int nBuffID  in pNetPlayer->GetBuffList().m_vContains)
		{
			XBuffInfo* pBuffInfo = info.buff->Get( nBuffID);
			if ( pBuffInfo == NULL)  continue;
			attr.ParseBuffAttr( pBuffInfo);
			if(nBuffID == 1276)
				++a;
		}
	}

	m_BuffInstant.ParseBuffAttr(attr);
	m_BuffEnchant.ParseBuffAttr(attr);

	return attr;
}

void XModuleBuff::DestroyBuff()
{
	// My player
	if ( m_pOwner->GetEntityType() == ETID_PLAYER  &&  m_pOwner->GetUID() == XGetMyUID())
	{
		if ( global.script)		global.script->GetGlueGameEvent().OnGameEvent( "BUFF", "CLEAR");
	}
}

void XModuleBuff::BuffDie()
{
	// Non plyer
	if ( m_pOwner->GetEntityType() == ETID_NPC)
	{
		XNonPlayer* pNonPlayer = m_pOwner->AsNPC();
		for each ( int nBuffID  in pNonPlayer->GetBuffList().m_vContains)
		{
			XBuffInfo* pBuffInfo = info.buff->Get( nBuffID);
			if ( pBuffInfo == NULL)  continue;

			XModuleEffect * pEffect = m_pOwner->GetModuleEffect();
			if ( pEffect)  
			{
				XBuffEffectEventData* pEventData = new XBuffEffectEventData;
				pEventData->m_nParam1			= pBuffInfo->m_nID;
				pEffect->OnEffectEvent( XEFTEVT_EFFECT_BUFF_DIE, pEventData);
			}
		}
		pNonPlayer->GetBuffList().DeleteAllItem();
	}

	// My player
	else if ( m_pOwner->GetType() == XOBJ_MY_PLAYER )
	{
		for ( map< int, XMYBUFFLIST>::iterator itr = gvar.MyInfo.BuffList.m_vContains.begin();  itr != gvar.MyInfo.BuffList.m_vContains.end();  itr++)
		{
			XBuffInfo* pBuffInfo = info.buff->Get( (*itr).first);
			if ( pBuffInfo == NULL)  continue;

			XModuleEffect * pEffect = m_pOwner->GetModuleEffect();
			if ( pEffect)  
			{
				XBuffEffectEventData* pEventData = new XBuffEffectEventData;
				pEventData->m_nParam1			= pBuffInfo->m_nID;
				pEffect->OnEffectEvent( XEFTEVT_EFFECT_BUFF_DIE, pEventData);
			}
		}
	}

	// Net player
	else if ( m_pOwner->GetType() == XOBJ_NET_PLAYER )
	{
		XNetPlayer* pNetPlayer = m_pOwner->AsNetPlayer();
		for each ( int nBuffID  in pNetPlayer->GetBuffList().m_vContains)
		{
			XBuffInfo* pBuffInfo = info.buff->Get( nBuffID);
			if ( pBuffInfo == NULL)  continue;

			XModuleEffect * pEffect = m_pOwner->GetModuleEffect();
			if ( pEffect)  
			{
				XBuffEffectEventData* pEventData = new XBuffEffectEventData;
				pEventData->m_nParam1			= pBuffInfo->m_nID;
				pEffect->OnEffectEvent( XEFTEVT_EFFECT_BUFF_DIE, pEventData);
			}
		}
		pNetPlayer->GetBuffList().DeleteAllItem();
	}	
}

void XModuleBuff::SetRemainBuffList( int nBuffID, float fRemainTime )
{
	if(nBuffID <= 0)
		return;

	m_TempRemainBuffList.insert(map<int, float>::value_type(nBuffID, fRemainTime));

	if(m_bLoadingComplete)
		ReSetRemainBuffList();

	return;
}

void XModuleBuff::ReSetRemainBuffList()
{
	// ���� ó�� ���Խ� ���� �Է�
	// �޽��� �ε��� �Ǿ�� �Ѵ�.
	for(map<int, float>::iterator itr = m_TempRemainBuffList.begin(); itr != m_TempRemainBuffList.end(); itr++)
	{
		BuffGain((*itr).first, (*itr).second, true);
	}

	m_TempRemainBuffList.clear();
}

void XModuleBuff::MyBuffAllDelete()
{
	// ������ ��� �����ϴ�.
	// �����ؼ� ���ñ� �ٶ��ϴ�.

	if (m_pOwner->GetEntityType() != ETID_PLAYER  ||  m_pOwner->GetUID() != XGetMyUID())
		return;
	
	map< int, XMYBUFFLIST> mapTempBuffList = gvar.MyInfo.BuffList.m_vContains;
	for ( map< int, XMYBUFFLIST>::iterator itr = mapTempBuffList.begin();  itr != mapTempBuffList.end();  itr++)
	{
		BuffLost(itr->first);
	}

	m_TempRemainBuffList.clear();
}

bool XModuleBuff::CheckTempRemainBuffList(int nBuffID)
{
	// �ε����̶��... �ӽ� ���� ����Ʈ�� ������ �Ǿ� �ִ���... üũ����.
	if(m_bLoadingComplete == false)
	{
		for(map<int, float>::iterator itr = m_TempRemainBuffList.begin(); itr != m_TempRemainBuffList.end(); itr++)
		{
			if((*itr).first == nBuffID )
			{
				m_TempRemainBuffList.erase(itr);
				return true;
			}
		}
	}

	return false;
}

void XModuleBuff::CheckBuffDuplication( int nBuffID )
{
	if(ExistBuffGain(nBuffID))
		BuffLost(nBuffID);	// ������ ����
}

void XModuleBuff::CheckInvisibilityGain( XBuffInfo* pBuffInfo )
{
	if (pBuffInfo->IsInvisibility() || 
		pBuffInfo->IsInvisibilityToNPC())
	{
		XModuleEntity* pModuleEntity = m_pOwner->GetModuleEntity();
		if (pModuleEntity)
		{
			pModuleEntity->SetMaxVisibility(BUFF_INVISIBILITY_ALPHA);
			pModuleEntity->StartFade(BUFF_INVISIBILITY_ALPHA, 1.0f);
		}
	}
}

void XModuleBuff::CheckInvisibilityLost( XBuffInfo* pBuffInfo )
{
	if (pBuffInfo->IsInvisibility() || 
		pBuffInfo->IsInvisibilityToNPC())
	{
		XModuleEntity* pModuleEntity = m_pOwner->GetModuleEntity();
		if (pModuleEntity)
		{
			pModuleEntity->SetMaxVisibility(1.0f);
			pModuleEntity->StartFade(1.0f, 1.0f);
		}
	}

}

void XModuleBuff::BuffIncrease( int nBuffID, float fLostRemained, bool bRemainBuffGain /*= false*/ )
{
	XBuffInfo * pBuffInfo = info.buff->Get(nBuffID);
	if(pBuffInfo == NULL)
		return;

	//------------------------------------------------------------------------
	// ���� ����
	if(ExistBuffGain(nBuffID) == false)
		return;

	// Ȥ�� MAX �����ϼ̳���?
	if(pBuffInfo->m_nStackMaxCount <= GetStackBuffCount(nBuffID))
	{
		// �տ��� �ϳ��� ���ϴ�.
		BuffInfoDecrease(nBuffID);
	}

	IncreaseBuff(nBuffID, fLostRemained);

	//------------------------------------------------------------------------
	// ���� ����
	SetGainBuffAnimation(pBuffInfo);

	// Modifier �� ���ϱ�
	GainMaintainEffect(pBuffInfo);

	//------------------------------------------------------------------------
	// ����Ʈ ����
	XModuleEffect * pEffect = m_pOwner->GetModuleEffect();
	if(pEffect)
	{
		// ����Ʈ ����
		XBuffEffectEventData* pEndEventData = new XBuffEffectEventData;
		pEndEventData->m_nParam1			= nBuffID;
		pEffect->OnEffectEvent(XEFTEVT_EFFECT_BUFF_END, pEndEventData);

		// ����Ʈ �ٽ� ����
		XBuffEffectEventData* pStartEventData = new XBuffEffectEventData;
		pStartEventData->m_nParam1			= nBuffID;
		pStartEventData->m_eEffectType		= BUFF_GAIN_EFFECT;
		pStartEventData->m_bRemainBuffGain	= bRemainBuffGain;
		pEffect->OnEffectEvent(XEFTEVT_EFFECT_BUFF, pStartEventData);
	}

	//mlog(L"���� ��ũ��(%s)\n", pBuffInfo->m_strName.c_str());
}

void XModuleBuff::BuffDecrease( int nBuffID )
{
	BuffInfoDecrease(nBuffID);

	//------------------------------------------------------------------------
	// ����Ʈ ����
	// ����Ʈ ����
	XModuleEffect * pEffect = m_pOwner->GetModuleEffect();
	if(pEffect)
	{
		XBuffEffectEventData* pEventData = new XBuffEffectEventData;
		pEventData->m_nParam1			= nBuffID;
		pEffect->OnEffectEvent(XEFTEVT_EFFECT_BUFF_END, pEventData);
	}

	//mlog(L"���� ��ũ��(%s)\n", pBuffInfo->m_strName.c_str());
}

bool XModuleBuff::ExistBuffGain( int nBuffID )
{
	// Non plyer
	if ( m_pOwner->GetEntityType() == ETID_NPC)
	{
		XNonPlayer* pNonPlayer = m_pOwner->AsNPC();
		if(pNonPlayer->GetBuffList().FindID(nBuffID) == true)
			return true;
	}
	// My player
	else if ( m_pOwner->GetType() == XOBJ_MY_PLAYER)
	{
		if(gvar.MyInfo.BuffList.FindID(nBuffID) == true)
			return true;
	}
	// Net player
	else if ( m_pOwner->GetType() == XOBJ_NET_PLAYER )
	{
		XNetPlayer* pNetPlayer = m_pOwner->AsNetPlayer();
		if(pNetPlayer->GetBuffList().FindID(nBuffID) == true)
			return true;
	}

	return false;
}

void XModuleBuff::IncreaseBuff( int nBuffID, float fLostRemained )
{
	// Non plyer
	if ( m_pOwner->GetEntityType() == ETID_NPC)
	{
		XNonPlayer* pNonPlayer = m_pOwner->AsNPC();
		if ( pNonPlayer->GetBuffList().FindID( nBuffID))
			pNonPlayer->PushBuff( nBuffID);
	}

	// My player
	else if ( m_pOwner->GetType() == XOBJ_MY_PLAYER )
	{
		if ( gvar.MyInfo.BuffList.FindID( nBuffID))
		{
			gvar.MyInfo.BuffList.PushStackItem( nBuffID, fLostRemained);
		}
	}

	// Net player
	else if ( m_pOwner->GetType() == XOBJ_NET_PLAYER )
	{
		XNetPlayer* pNetPlayer = m_pOwner->AsNetPlayer();
		if ( pNetPlayer->GetBuffList().FindID( nBuffID))
			pNetPlayer->PushBuff( nBuffID);
	}
}

void XModuleBuff::DecreaseBuff( int nBuffID )
{
	// Non plyer
	if ( m_pOwner->GetEntityType() == ETID_NPC)
	{
		XNonPlayer* pNonPlayer = m_pOwner->AsNPC();
		pNonPlayer->PopBuff( nBuffID);
	}

	// My player
	else if ( m_pOwner->GetType() == XOBJ_MY_PLAYER )
	{
		gvar.MyInfo.BuffList.PopStackItem( nBuffID);
	}

	// Net player
	else if ( m_pOwner->GetType() == XOBJ_NET_PLAYER )
	{
		XNetPlayer* pNetPlayer = m_pOwner->AsNetPlayer();
		pNetPlayer->PopBuff( nBuffID);
	}
}

int XModuleBuff::GetStackBuffCount( int nBuffID )
{
	// Non plyer
	if ( m_pOwner->GetEntityType() == ETID_NPC)
	{
		int n = 0;
		XNonPlayer* pNonPlayer =  m_pOwner->AsNPC();
		for each ( int nID  in pNonPlayer->GetBuffList().m_vContains)
		{
			if(nID == nBuffID)
				++n;
		}

		return n;
	}

	// My player
	else if ( m_pOwner->GetType() == XOBJ_MY_PLAYER )
	{
		return gvar.MyInfo.BuffList.GetItemCount(nBuffID);
	}

	// Net player
	else if ( m_pOwner->GetType() == XOBJ_NET_PLAYER )
	{
		int n = 0;
		XNetPlayer* pNetPlayer = m_pOwner->AsNetPlayer();
		for each ( int nID  in pNetPlayer->GetBuffList().m_vContains)
		{
			if(nID == nBuffID)
				++n;
		}

		return n;
	}

	return 0;
}

void XModuleBuff::SetGainBuffAnimation( XBuffInfo * pBuffInfo )
{
	if ( _IsDeactivateBuff( pBuffInfo) == true)
	{
		// �� ĳ���ʹ� ��Ȱ���� ������ �ɸ��� ������Ʈ�� ������
		XModuleActorControl* pControl = m_pOwner->GetModuleActorControl();
		if ( pControl)
			pControl->DoActionDeactivate( pBuffInfo->m_nID, pBuffInfo->m_strUseAniName.c_str());

		if (  m_pOwner->GetType() != XOBJ_MY_PLAYER &&
			!pBuffInfo->m_strUseAniName.empty())
		{
			UseAnimation( pBuffInfo);
		}
	}

	// �ִϸ��̼� ���̻�
	if (!pBuffInfo->m_strAniPostfix.empty())
	{
		// �ִϸ��̼��� �ִٸ�... �ŷ�Ʈ ���
		XModuleTalent* pModuleTalent = m_pOwner->GetModuleTalent();
		if(pModuleTalent)
		{
			if (pModuleTalent->IsActive())
				pModuleTalent->Cancel();
		}

		XModuleMotion* pModuleMotion = m_pOwner->GetModuleMotion();
		if (pModuleMotion)
		{
			pModuleMotion->SetAnimationNamePostfix(pBuffInfo->m_strAniPostfix.c_str());
			pModuleMotion->RefreshThisMotion();
		}
	}
}

void XModuleBuff::BuffInfoDecrease( int nBuffID )
{
	XBuffInfo * pBuffInfo = info.buff->Get(nBuffID);
	if(pBuffInfo == NULL)
		return;

	//------------------------------------------------------------------------
	// ���� ����
	if(ExistBuffGain(nBuffID) == false)
		return;

	int nStackCount = GetStackBuffCount(nBuffID);

	DecreaseBuff(nBuffID);

	//------------------------------------------------------------------------
	// ���� ����

	// Modifier �� ����
	if(nStackCount > 0)
		LostMaintainEffect(pBuffInfo);
}

void XModuleBuff::BuffEnchantGain( int nBuffID, bool bEffect, int nPartsSlotType )
{
	XBuffInfo * pBuffInfo = info.buff->Get(nBuffID);
	if(pBuffInfo == NULL)
		return;

	//------------------------------------------------------------------------
	// ���� ����
	// Modifier �� ���ϱ�
	GainMaintainEffect(pBuffInfo);

	m_BuffEnchant.Push(nBuffID);

	XModuleEffect * pEffect = m_pOwner->GetModuleEffect();
	if(bEffect && pEffect)
	{
		XBuffEffectEventData* pEventData = new XBuffEffectEventData;
		pEventData->m_nParam1			= nBuffID;
		pEventData->m_nPartsSlotType	= nPartsSlotType;
		pEffect->OnEffectEvent(XEFTEVT_EFFECT_BUFF_ENCHANT, pEventData);
	}
}

void XModuleBuff::BuffEnchantLost( int nBuffID, bool bEffect, int nPartsSlotType )
{
	XBuffInfo * pBuffInfo = info.buff->Get(nBuffID);
	if(pBuffInfo == NULL)
		return;

	//------------------------------------------------------------------------
	if(m_BuffEnchant.CheckEnchantBuffID(nBuffID) == false)
		return;

	m_BuffEnchant.Pop(nBuffID);

	// Modifier �� ����
	// ���� ���ȸ�ŭ ����.
	LostMaintainEffect(pBuffInfo);

	//------------------------------------------------------------------------
	// ����Ʈ ����
	XModuleEffect * pEffect = m_pOwner->GetModuleEffect();
	if(bEffect && pEffect)
	{
		XBuffEffectEventData* pEventData = new XBuffEffectEventData;
		pEventData->m_nParam1			= nBuffID;
		pEventData->m_nPartsSlotType	= nPartsSlotType;
		pEffect->OnEffectEvent(XEFTEVT_EFFECT_BUFF_END, pEventData);
	}
}

void XModuleBuff::Init_ForChangeField()
{
	// �ʵ峪, ä���̵��� ����Ʈ�� ��������� �ٽ� ����
	if(m_pOwner->GetType() != XOBJ_MY_PLAYER)
		return;

	XModuleEffect * pEffect = m_pOwner->GetModuleEffect();
	if(pEffect)
	{
		XBuffEffectEventData* pEventData = new XBuffEffectEventData;
		pEffect->OnEffectEvent(XEFTEVT_EFFECT_BUFF_RESET, pEventData);
	}
}