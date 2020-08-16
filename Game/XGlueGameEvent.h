#ifndef _XGLUE_GAME_EVENT_H
#define _XGLUE_GAME_EVENT_H

#include "XUISystem.h"

///////////////////////////////////////////////////////////////////////////
/// �̺�Ʈ�� ���� ������ Ŭ����
class XGlueGameEvent
{
private:
	MWLua *					m_pMWLua;
public:
	XGlueGameEvent();
	~XGlueGameEvent();
	void Init(MWLua* pLua);

	bool OnLoginEvent( string strMsg, string sParam = "", int nParam1 =0, int nParam2 =0);
	bool OnCharacterEvent( string strMsg, string sParam = "", int nParam1 =0, int nParam2 =0);
	bool OnGameEvent( string strMsg, string sParam = "", int nParam1 =0, int nParam2 =0);

	// ĳ���� ���� ����
	bool OnCreatedCharacter(bool bResult, int nResult=0)	{ return OnCharacterEvent( "CREATECHARACTER", "", bResult, nResult);}

	// Enemy Info
	bool OnEnemyInfoRefresh()		{ return OnGameEvent( "ENEMYINFO", "REFRESH");	}

	// Target Info
	bool OnTargetInfoRefresh()		{ return OnGameEvent( "TARGETINFO", "REFRESH");	}
	bool OnTargetInfoHide()			{ return OnGameEvent( "TARGETINFO", "HIDE");}

	// BatleArena
	bool OnShowBattleScore()		{ return OnGameEvent( "BATTLEARENA", "SHOW");	}
	bool OnRefreshBattleScore()		{ return OnGameEvent( "BATTLEARENA", "REFRESH");}
	bool OnHideBattleScore()		{ return OnGameEvent( "BATTLEARENA", "HIDE");}

	bool OnRebirthShow()			{ return OnGameEvent( "REBIRTH", "BEGIN");		}
	bool OnBattleArenaRebirthShow()	{ return OnGameEvent( "REBIRTH", "BEGIN");		}
	bool OnRebirthHide()			{ return OnGameEvent( "REBIRTH", "END");		}

	// ����ġ
	bool OnExpRefresh()				{ return OnGameEvent( "EXPREFRESH");			}

	bool OnRefreshTalent()			{ return OnGameEvent( "TALENT", "REFRESH");		}
	bool OnRefreshSilver()			{ return OnGameEvent( "REFRESHSILVER");			}

	// Quest
	bool OnQuestAcceptShow(int nQuestID){ return OnGameEvent( "ACCEPT_QUEST", "BEGIN", nQuestID);}
	bool OnQuestAcceptHide()		{ return OnGameEvent( "ACCEPT_QUEST", "END");		}

	// Trade
	bool OnTradeShow()				{ return OnGameEvent( "TRADE", "BEGIN");	}
	bool OnTradeHide()				{ return OnGameEvent( "TRADE", "END");		}
	bool OnTradeRefresh()			{ return OnGameEvent( "TRADE", "REFRESH");	}
	bool OnTradeConfirmRefresh()	{ return OnGameEvent( "TRADE", "CONFIRMREFRESH");	}

	// MessageBox
	bool OnMessageBox(string strMsg, int nLifeTime=10000)	{ return OnGameEvent( "MESSAGEBOX", strMsg, nLifeTime);}

	// ��Ƽ
	bool OnPartyRefresh()			{ return OnGameEvent( "PARTY", "REFRESH");	}

	//����
	bool OnLootingBegin()			{ return OnGameEvent( "LOOTING", "BEGIN");	}
	bool OnLootingEnd()				{ return OnGameEvent( "LOOTING", "END");	}
	bool OnLootingRefresh()			{ return OnGameEvent( "LOOTING", "REFRESH");}
	bool OnLootingLooter()			{ return OnGameEvent( "LOOTING", "LOOTER");	}

	//������ ����Ʈ
	bool OnChallengerQuestBegin()	{ return OnGameEvent( "CHALLENGERQUEST", "BEGIN");	}
	bool OnChallengerQuestRefresh()	{ return OnGameEvent( "CHALLENGERQUEST", "REFRESH");}
	bool OnChallengerQuestEnd()		{ return OnGameEvent( "CHALLENGERQUEST", "End");	}

	// �ŷ�Ʈ �ܰ�
	bool OnActTalentStart(float fCastingTime)	{ return OnGameEvent( "ACTIONPROGRESSBAR", "CASTING", MAPT_CASTING_SPELL,fCastingTime);	}
	bool OnActTalentEnd()						{ return OnGameEvent( "ACTIONPROGRESSBAR", "END");	}

	// ���
	bool OnGuildCreate()			{ return OnGameEvent( "GUILD", "CREATE");	}
	bool OnGuildDestroy()			{ return OnGameEvent( "GUILD", "DESTROY");	}
	bool OnGuildRefresh()			{ return OnGameEvent( "GUILD", "REFRESH");	}
	bool OnGuildStorageShow()		{ return OnGameEvent( "GUILD", "STORAGESHOW");		}
	bool OnGuildStorageRefresh()	{ return OnGameEvent( "GUILD", "STORAGEREFRESH");	}

	// ������
	bool OnStorageShow()			{ return OnGameEvent( "STORAGE", "SHOW");	}
	bool OnStorageRefresh()			{ return OnGameEvent( "STORAGE", "REFRESH");}

	// Ʃ�丮��		// by tripleJ _���� Help ���� ����
	bool OnTutorialTrigger( const string& strTutorialType, int nHelpID );
	bool OnTutorialTriggerClose( const string& strTutorialType );
	bool IsShowUI( const string& strUIName );
	bool IsUpdateTutorial();
	// ui �̺�Ʈ -----
	void			OnToggleChat()												{ WCALL(m_pMWLua, "shGameEvent_OnToggleChat", WNULL); }
	bool			OnVirtualKeyDown(const char* strKeyCommand)					{ bool bRet=false; WCALL1(m_pMWLua, "shGameEvent_OnVirtualKeyDown", strKeyCommand, &bRet); return bRet; }
	void			OnChatGlobal(const char* str1, const char* str2)			{ WCALL2(m_pMWLua, "shGameEvent_OnChatGlobal", str1, str2, WNULL); }
	void			OnToggleQuest()												{ WCALL(m_pMWLua, "shGameEvent_OnToggleQuest", WNULL); }

	void			OnNoticeDialogShow(const char* strExplain)					{ WCALL1(m_pMWLua, "shGameEvent_OnNoticeDialogShow", strExplain, WNULL); }

	// ������ ����
	void			OnItemInfoWindow(int h, int l)								{ WCALL2(m_pMWLua, "shGameEvent_OnItemExplanInfo", h, l, WNULL); }
	bool			OnItemInfoWindowHide()										{ bool bRet=false; WCALL(m_pMWLua, "shGameEvent_OnItemExplanHide", &bRet); return bRet; }

	// �ŷ�Ʈ �ܰ�
	void			OnTalentLevelStart(float fCastingTime)						{ WCALL1(m_pMWLua, "shGameEvent_TalentLevelStart", fCastingTime, WNULL); }
	void			OnTalentLevelEnd()											{ WCALL(m_pMWLua, "shGameEvent_TalentLevelEnd", WNULL); }

	// Ż��Ʈ �Ʒ�
	void			OnTrainerToAllTalent(const char* strText);
	void			OnRefreshTalentTrainer(const char* strText);

	// debug
	void			OnDebugOnGameStarted()										{ WCALL(m_pMWLua, "Debug_OnGameStarted", WNULL); }

	//��Ƽ
	void			OnPartyWindowRefresh(const char* strText);
	void			OnPartyDemandRequireWindowShow()							{ WCALL(m_pMWLua, "shGameEvent_PartyDemandWindowShow", WNULL); }

	// �̴ϰ���
	// ��Ʈ ������
	void			OnHitGageStart(int nHitCount, int nCurrentCount)			{ WCALL2(m_pMWLua, "shGameEvent_OnHitGageStart", nHitCount, nCurrentCount, WNULL); }
	void			OnHitGageEnd()												{ WCALL(m_pMWLua, "shGameEvent_OnHitGageEnd", WNULL); }
	void			OnHitGageUpdate(int nCurrentHitCount)						{ WCALL1(m_pMWLua, "shGameEvent_OnHitGageUpdate", nCurrentHitCount, WNULL); }

	//Buff
	void			OnBuffRefresh()												{ WCALL(m_pMWLua, "shGameEvent_BuffRefresh", WNULL); }

	//Quest
	void			OnQuestRefresh()											{ WCALL(m_pMWLua, "shGameEvent_QuestRefresh", WNULL); }
	void			OnQuestReward(int nQuestID)									{WCALL1(m_pMWLua, "shGameEvent_QuestReward", nQuestID, WNULL); }
	//void			OnQuestShare()												{ WCALL(m_pMWLua, "shGameEvent_OnQuestShare", WNULL); }

	void			OnInteractonWindowsClose();

	void			OnDynamicTimeShow()											{ WCALL(m_pMWLua, "shGameEvent_DynamicFieldTimeShow", WNULL); }	
	void			OnDynamicTimeHide()											{ WCALL(m_pMWLua, "shGameEvent_DynamicFieldTimeHide", WNULL); }	

	//Minimap
	void			OnMiniMapInit()												{ WCALL(m_pMWLua, "shGameEvent_MiniMapInit", WNULL); }
	void			OnMiniMapMarkPosition(float fMarkPosX, float fMarkPosY)		{ WCALL2(m_pMWLua, "shGameEvent_MiniMapMarkPosition", fMarkPosX, fMarkPosY, WNULL); }
	void			OnMiniMapInfoRefresh()										{ WCALL(m_pMWLua, "shGameEvent_MiniMapInfo", WNULL); }
	
	//�ȷ�Ʈ
	void			OnPaletteRefresh()											{ WCALL(m_pMWLua, "shGameEvent_PaletteRefresh", WNULL); }

	void			OnChangeWeaponPic()											{WCALL(m_pMWLua, "shGameEvent_MyInfo_Weapon", WNULL); }

	// �ͼ� ������ ó��
	void			OnEquipBind(const char* ItemName, const char* szSlot, const char* szuid)	{WCALL3(m_pMWLua, "shGameEvent_EquipBind", ItemName, szSlot, szuid, WNULL); }
	void			OnLootItemBind(const char* ItemName, const char* szItemID, const char*  szItemCount)	{WCALL3(m_pMWLua, "shGameEvent_LootItemBind", ItemName, szItemID, szItemCount, WNULL); }

	//ä��
	void			OnShowChannelWindow( int count, int* nChannel );
	void			OnChangeChannelRequireWindowShow(int sec)					{ WCALL1(m_pMWLua, "shGameEvent_ShowChannelReqWindow", sec,WNULL); }
	void			OnChangeChannelRequireWindowHide()							{ WCALL(m_pMWLua, "shGameEvent_HideChannelReqWindow", WNULL); }

	//������
	void			OnShowBattleScore(float fWaitSecond, MUID uidWinner, int nWinnerScore, int nLoserScore, float fGameElaspedTime);
	void			OnShowBattleArenaRealTimeScore()							{ WCALL(m_pMWLua, "shGameEvent_ShowBattleArenaRealTimeScore", WNULL); }
	void			OnHideBattleArenaRealTimeScore()							{ WCALL(m_pMWLua, "shGameEvent_HideBattleArenaRealTimeScore", WNULL); }
	void			OnSetBattleArenaRealTimeScore(int nRed, int nBlue)			{ WCALL2(m_pMWLua, "shGameEvent_SetBattleArenaRealTimeScore", nRed, nBlue, WNULL); }

	// �ǵ��
	void			OnFetchFeedback( vector<string> vecTextList );

	// ���� �����ִ� â�� ������ ����
	int				GetVisibledWindow();
};


class XGlueDebugEvent
{
private:
	MWLua *					m_pMWLua;
public:
	XGlueDebugEvent() : m_pMWLua(NULL)
	{

	}
	void Init(MWLua* pLua)
	{
		m_pMWLua = pLua;
	}

	void		OnConsoleInput(const char* szCmd)								{ /* WCALL1(m_pMWLua, "shDebugEvent_OnConsoleInput", szCmd, WNULL); */ }
	void		OnLogOutput(const char* szLog)									{ /* WCALL1(m_pMWLua, "shDebugEvent_OnLogOutput", szLog, WNULL); */ }
};

#endif
