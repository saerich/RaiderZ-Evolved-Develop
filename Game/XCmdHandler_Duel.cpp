#include "StdAfx.h"
#include "XCmdHandler_Duel.h"
#include "XModuleActorControl.h"
#include "XMyPlayer.h"
#include "XStrings.h"
#include "XInteractionInfo.h"
#include "XPost_Duel.h"
#include "XStrings.h"

XCmdHandler_Duel::XCmdHandler_Duel(MCommandCommunicator* pCC) : MCommandHandler(pCC)
{
	SetCmdHandler(MC_DUEL_QUESTION,	OnDuelQuestion);
	SetCmdHandler(MC_DUEL_CANCEL,	OnDuelCancel);
	SetCmdHandler(MC_DUEL_READY,	OnDuelReady);
	SetCmdHandler(MC_DUEL_FIGHT,	OnDuelFight);
	SetCmdHandler(MC_DUEL_FINISHED,	OnDuelFinished);
	SetCmdHandler(MC_DUEL_DEFEATED,	OnDuelDefeated);

	SetCmdHandler(MC_DUEL_PARTY_READY,		OnDuelPartyReady);
	SetCmdHandler(MC_DUEL_PARTY_FIGHT,		OnDuelPartyFight);
	SetCmdHandler(MC_DUEL_PARTY_CANCEL,		OnDuelPartyCancel);
	SetCmdHandler(MC_DUEL_PARTY_FINISHED,	OnDuelPartyFinished);

	SetCmdHandler(MC_DUEL_READY_COUNT,		OnDuelReadyCount);
}

MCommandResult XCmdHandler_Duel::OnDuelQuestion(MCommand* pCommand, MCommandHandler* pHandler)
{
	MUID uidOppnent;
	wstring strOpponentName;
	bool bCombat;
	if (pCommand->GetParameter(&uidOppnent,		0, MPT_UID)==false) return CR_ERROR;
	if (pCommand->GetParameter(strOpponentName,	1, MPT_WSTR)==false) return CR_ERROR;
	if (pCommand->GetParameter(&bCombat,		2, MPT_BOOL)==false) return CR_ERROR;

	if ( XGETCFG_GAME_REJECT_DUEL  ||  bCombat)
	{
		XObject* pPlayer = gg.omgr->Find(uidOppnent);
		if(pPlayer == NULL) return CR_FALSE;
		
		global.ui->OnSystemMsg(XGetStr(L"SMSG_DUEL_REJECT"), FSParam(strOpponentName));

		XPostDuel_QuestionRespond(false);
	}
	else
	{
		const wchar_t* szMsg = XGetStr(L"SMSG_DUEL_QUESTION");		
		global.ui->OnPresentationLower(szMsg, PRESENTATION_ICON_INFORMATION, true);

		XGetInteractionInfo().SetRequestPlayerUID(uidOppnent);
		gvar.Game.bDuelReq = true;

		gvar.Game.pMyPlayer->SafeStop();

		if (global.script)
		{
			global.script->GetGlueGameEvent().OnGameEvent( "DUEL", "CHALLENGED");
		}

		if (global.ui)
		{
			global.ui->OnGuideMsg(GUIDE_MSG_LUA_RECV_DUEL_CHALLENGED);
		}
	}

	return CR_TRUE;
}

MCommandResult XCmdHandler_Duel::OnDuelCancel(MCommand* pCommand, MCommandHandler* pHandler)
{
	MUID uidReqPlayer;
	MUID uidTarPlayer;
	if (pCommand->GetParameter(&uidReqPlayer,	0, MPT_UID)==false) return CR_ERROR;
	if (pCommand->GetParameter(&uidTarPlayer,	1, MPT_UID)==false) return CR_ERROR;

	if (global.script)
	{
		global.script->GetGlueGameEvent().OnGameEvent( "DUEL", "CANCELED");
	}

	XPlayer* pReqPlayer = gg.omgr->FindPlayer(uidReqPlayer);
	if(pReqPlayer)
	{
		pReqPlayer->SetDuel(false, MUID::ZERO);
		pReqPlayer->SetCharacterPane_Player();
	}

	XPlayer* pTarPlayer = gg.omgr->FindPlayer(uidTarPlayer);
	if(pTarPlayer)
	{
		pTarPlayer->SetDuel(false, MUID::ZERO);
		pTarPlayer->SetCharacterPane_Player();
	}

	if(XGetInteractionInfo().VSPlayerUID == MUID::ZERO)
	{
		if (pReqPlayer  &&  gvar.MyInfo.MyUID == pReqPlayer->GetUID())
		{
			global.ui->OnSystemMsg(XGetStr(L"SMSG_DUEL_CANCEL"));
		}
	}
	else
	{
		global.ui->OnSystemMsg(XGetStr(L"SMSG_DUEL_CANCEL"));

		XGetInteractionInfo().VSPlayerUID = MUID::ZERO;
	}

	return CR_TRUE;
}

MCommandResult XCmdHandler_Duel::OnDuelReady(MCommand* pCommand, MCommandHandler* pHandler)
{
	MUID uidFigher1;
	MUID uidFigher2;
	if (pCommand->GetParameter(&uidFigher1,	0, MPT_UID)==false) return CR_ERROR;
	if (pCommand->GetParameter(&uidFigher2,	1, MPT_UID)==false) return CR_ERROR;

	MUID uidVSPlayer;

	if ( uidFigher1 == XGetMyUID() )
	{
		uidVSPlayer = uidFigher2 ;
	}
	if ( uidFigher2 == XGetMyUID() )
	{
		uidVSPlayer = uidFigher1 ;
	}
	
	XNetPlayer* pNetPlayer = gg.omgr->FindNetPlayer(uidVSPlayer);
	if (pNetPlayer)
	{
		//����� �������� ���� 3�ʰ� ���ͷ��� �ȵǵ��� �ϱ����� ������ ����...
		XGetInteractionInfo().VSPlayerUID = uidVSPlayer;
		pNetPlayer->SetCharacterPane_VSNetPlayer();
	}

	return CR_TRUE;
}

MCommandResult XCmdHandler_Duel::OnDuelFight(MCommand* pCommand, MCommandHandler* pHandler)
{
	MUID uidFigher1;
	MUID uidFigher2;
	if (pCommand->GetParameter(&uidFigher1,	0, MPT_UID)==false) return CR_ERROR;
	if (pCommand->GetParameter(&uidFigher2,	1, MPT_UID)==false) return CR_ERROR;

	XPlayer* pPlayer1 = gg.omgr->FindPlayer(uidFigher1);
	if(pPlayer1)	pPlayer1->SetDuel(true, uidFigher2);

	XPlayer* pPlayer2 = gg.omgr->FindPlayer(uidFigher2);
	if(pPlayer2)	pPlayer2->SetDuel(true, uidFigher1);

	wstring str;
	if(pPlayer1 && pPlayer2)
	{
		str = FormatString(XGetStr(L"SMSG_DUEL_PVP"), FSParam(pPlayer1->GetName(), pPlayer2->GetName()));
	}

	str += XGetStr(L"SMSG_DUEL_START");
	global.ui->OnSystemMsg(str.c_str());
	global.ui->OnPresentationUpper( XGetStr( L"DUEL_BEGIN"));	

	return CR_TRUE;
}

MCommandResult XCmdHandler_Duel::OnDuelFinished(MCommand* pCommand, MCommandHandler* pHandler)
{
	MUID uidWinner;
	MUID uidLoser;
	if (pCommand->GetParameter(&uidWinner,	0, MPT_UID)==false) return CR_ERROR;
	if (pCommand->GetParameter(&uidLoser,	1, MPT_UID)==false) return CR_ERROR;

	XGetInteractionInfo().VSPlayerUID = MUID::ZERO;
	
	wstring strWinnerName = L"UnkownPlayer";
	wstring strLoserName = L"UnkownPlayer";

	XPlayer* pWinner = gg.omgr->FindPlayer( uidWinner);
	if ( pWinner)
	{
		pWinner->SetDuel(false, MUID::ZERO);
		pWinner->SetCharacterPane_Player();

		XModuleActorControl* pModuleActorControl = pWinner->GetModuleActorControl();
		if( pModuleActorControl)
		{
			pModuleActorControl->DoActionDuelGesture(L"winner");
		}

		strWinnerName = pWinner->GetName();
	}

	XPlayer* pLoser = gg.omgr->FindPlayer( uidLoser);
	if ( pLoser)
	{
		pLoser->SetDuel(false, MUID::ZERO);
		pLoser->SetCharacterPane_Player();

		strLoserName = pLoser->GetName();
	}


	// ��� �����̼� ���
/*	wstringstream stream;
	stream << strWinnerName << L"(��)�� " << strLoserName << L"���� �������� �¸��ϼ̽��ϴ�.";
	
	global.ui->OnPresentationUpper(stream.str());	
*/
	wstring strMessage = FormatString( XGetStr( L"DUEL_WINNER"), FSParam( strWinnerName, strLoserName));
	global.ui->OnPresentationUpper( strMessage);

	return CR_TRUE;
}


MCommandResult XCmdHandler_Duel::OnDuelDefeated(MCommand* pCommand, MCommandHandler* pHandler)
{
	MUID uidDefeat;
	if (pCommand->GetParameter(&uidDefeat,	0, MPT_UID)==false) return CR_ERROR;

	gvar.Game.pMyPlayer->RemoveOpponentUID(uidDefeat);

	XPlayer* pDefeater = gg.omgr->FindPlayer( uidDefeat);
	if ( pDefeater)
	{
		pDefeater->SetDuel(false, MUID::ZERO);
		pDefeater->SetCharacterPane_Player();

		XGetMyPlayer()->RemoveOpponentUID(uidDefeat);

		XModuleActorControl* pModuleActorControl = pDefeater->GetModuleActorControl();
		if( pModuleActorControl)
		{
			pModuleActorControl->DoActionDuelGesture(L"loser");
		}

		wstring str;
		str = FormatString(XGetStr(L"SMSG_DUEL_DEFEATED"), FSParam(pDefeater->GetName()));
		global.ui->OnSystemMsg(str.c_str());
		global.ui->OnPresentationUpper(str);

	}

	if(uidDefeat == XGetMyUID())
	{
		const vector<MUID>& vecTeamUIDs = gvar.Game.Duel.nTeam==1?gvar.Game.Duel.vecTeam1UIDs:gvar.Game.Duel.vecTeam2UIDs;
		for each (MUID each in vecTeamUIDs)
		{
			XPlayer* pPlayer = gg.omgr->FindPlayer(each);
			if (!pPlayer)
				continue;

			pPlayer->SetDuel(false, each);
			pPlayer->SetCharacterPane_Player();
		}
	}

	return CR_TRUE;
}

MCommandResult XCmdHandler_Duel::OnDuelPartyReady(MCommand* pCommand, MCommandHandler* pHandler)
{
	vector<MUID> vecTeam1UIDs;
	vector<MUID> vecTeam2UIDs;
	if (pCommand->GetBlob(vecTeam1UIDs,	0)==false) return CR_ERROR;
	if (pCommand->GetBlob(vecTeam2UIDs,	1)==false) return CR_ERROR;

	gvar.Game.Duel.vecTeam1UIDs = vecTeam1UIDs;
	gvar.Game.Duel.vecTeam2UIDs = vecTeam2UIDs;

	gvar.Game.Duel.nTeam = 1;
	for each (MUID each in gvar.Game.Duel.vecTeam1UIDs)
	{
		if (XGetMyUID() == each)
		{
			gvar.Game.Duel.nTeam = 0;
			break;
		}
	}
	
	const vector<MUID>& vecTeamUIDs = gvar.Game.Duel.nTeam==1?vecTeam1UIDs:vecTeam2UIDs;
	for each (MUID each in vecTeamUIDs)
	{
		XNetPlayer* pNetPlayer = gg.omgr->FindNetPlayer(each);
		if (!pNetPlayer)
			continue;

		pNetPlayer->SetCharacterPane_VSNetPlayer();
	}

	return CR_TRUE;
}

MCommandResult XCmdHandler_Duel::OnDuelPartyFight(MCommand* pCommand, MCommandHandler* pHandler)
{
	vector<MUID> vecPlayers;
	if (pCommand->GetBlob(vecPlayers,	0)==false) return CR_ERROR;
	
	// �÷��̾�� �� ���� ����
	XGetMyPlayer()->SetDuel(false, MUID::ZERO);
	for (int i = 0; i < 2; i++)
	{
		const vector<MUID>& vecTeam1UIDs = 0==i?gvar.Game.Duel.vecTeam1UIDs:gvar.Game.Duel.vecTeam2UIDs;
		const vector<MUID>& vecTeam2UIDs = 1==i?gvar.Game.Duel.vecTeam1UIDs:gvar.Game.Duel.vecTeam2UIDs;

		for each (MUID each in vecTeam1UIDs)
		{
			XPlayer* pPlayer = gg.omgr->FindPlayer(each);
			if (!pPlayer)	continue;

			pPlayer->SetOpponentUIDs(vecTeam2UIDs);
		}
	}

	//XGetMyPlayer()->SetDuel(false, MUID::ZERO);
	//const vector<MUID>& vecTeamUIDs = gvar.Game.Duel.nTeam==1?gvar.Game.Duel.vecTeam1UIDs:gvar.Game.Duel.vecTeam2UIDs;
	//for each (MUID each in vecTeamUIDs)
	//{
	//	XPlayer* pPlayer = gg.omgr->FindPlayer(each);
	//	if (!pPlayer)	continue;

	//	XGetMyPlayer()->SetDuel(true, each);
	//}

	wstring str;
	str = XGetStr(L"SMSG_DUEL_START");
	global.ui->OnSystemMsg(str.c_str());
	global.ui->OnPresentationUpper( XGetStr( L"DUEL_BEGIN"));

	return CR_TRUE;
}

MCommandResult XCmdHandler_Duel::OnDuelPartyFinished(MCommand* pCommand, MCommandHandler* pHandler)
{
	// �Ķ���� �ޱ�
	int nTeam;
	MUID uidWinTeamLeader;
	MUID uidLoseTeamLeader;
	int nWinPlayerCount;
	int nLosePlayerCount;

	if (pCommand->GetParameter(&nTeam,	0, MPT_INT)==false)				return CR_ERROR;
	if (pCommand->GetParameter(&uidWinTeamLeader, 1, MPT_UID)==false)	return CR_ERROR;
	if (pCommand->GetParameter(&uidLoseTeamLeader, 2, MPT_UID)==false)	return CR_ERROR;
	if (pCommand->GetParameter(&nWinPlayerCount, 3, MPT_INT)==false)			return CR_ERROR;
	if (pCommand->GetParameter(&nLosePlayerCount, 4, MPT_INT)==false)			return CR_ERROR;


	// ����ó��
	for (int i = 0; i < 2; i++)
	{
		const vector<MUID>& vecTeamUIDs = 0==i?gvar.Game.Duel.vecTeam1UIDs:gvar.Game.Duel.vecTeam2UIDs;

		for each (MUID each in vecTeamUIDs)
		{
			XPlayer* pPlayer = gg.omgr->FindPlayer(each);
			if (!pPlayer)	continue;

			pPlayer->SetDuel(false, MUID::ZERO);
			pPlayer->SetCharacterPane_Player();

			if (nTeam == i)
			{
				XModuleActorControl* pModuleActorControl = pPlayer->GetModuleActorControl();
				if( pModuleActorControl)
				{
					pModuleActorControl->DoActionGesture(L"winner");
				}
			}
		}
	}

	// Duel �ʱ�ȭ
	gvar.Game.Duel.Clear();


	// ��� �����̼� ���
	wstring strWinLeaderName = L"UnkownPlayer";
	XPlayer* pWinLeader = gg.omgr->FindPlayer(uidWinTeamLeader);
	if (pWinLeader != NULL)
	{
		strWinLeaderName = pWinLeader->GetName();
	}
	
	wstring strLoseLeaderName = L"UnkownPlayer";
	XPlayer* pLoseLeader = gg.omgr->FindPlayer(uidLoseTeamLeader);
	if (pLoseLeader != NULL)
	{
		strLoseLeaderName = pLoseLeader->GetName();
	}
/*	
	wstringstream stream;
	stream	<< strWinLeaderName << L"��Ƽ�� " << strLoseLeaderName << L"��Ƽ���� "
			<< nWinPlayerCount << L"��" << nLosePlayerCount << L" �������� �¸��ϼ̽��ϴ�.";

	global.ui->OnPresentationUpper(stream.str());	
*/
	wstring strMessage = FormatString( XGetStr( L"DUEL_PARTYWINNER"), FSParam( strWinLeaderName, strLoseLeaderName));
	global.ui->OnPresentationUpper( strMessage);

	return CR_TRUE;
}

MCommandResult XCmdHandler_Duel::OnDuelPartyCancel(MCommand* pCommand, MCommandHandler* pHandler)
{
	vector<MUID> vecPlayers;
	if (pCommand->GetBlob(vecPlayers,	0)==false) return CR_ERROR;

	if (global.script)
	{
		global.script->GetGlueGameEvent().OnGameEvent( "DUEL", "CANCELED");
	}

	for each (MUID each in vecPlayers)
	{
		XPlayer* pReqPlayer = gg.omgr->FindPlayer(each);
		if(!pReqPlayer)
			continue;

		pReqPlayer->SetDuel(false, MUID::ZERO);
		pReqPlayer->SetCharacterPane_Player();
	}

	gvar.Game.Duel.Clear();

	return CR_TRUE;
}

MCommandResult XCmdHandler_Duel::OnDuelReadyCount(MCommand* pCommand, MCommandHandler* pHandler)
{
	// ���� �� �ޱ�
	int nReadySecond;
	
	if (!pCommand->GetParameter(&nReadySecond, 0, MPT_INT))	return CR_ERROR;


	// �����̼����� ���
/*	wstringstream stream;
	stream << nReadySecond << L"�ʵڿ� ������ ����˴ϴ�.";
	
	global.ui->OnPresentationUpper(stream.str());
*/
	wstring strMessage = CSFormatString::Format( XGetStr( L"DUEL_BEGINAFTERSEC"), FSParam( nReadySecond));
	global.ui->OnPresentationUpper( strMessage);

	return CR_TRUE;
}
