#pragma once

#include <string>
#include <vector>
using namespace std;

#include "XActionKeyDef.h"

// by tripleJ 110414	_���� Help ���� ����

//#define HELP_DEV		// by tripleJ 110414	_���߿� Define

// 20���� ���Ͽ����� ���� Ʃ�丮�� ����� �۵��Ѵ�.
#define HELP_ENABLED_MAX_LEVEL			20

enum XHelpType
{
	HELPTYPE_POPUPHELP	= 0,
	HELPTYPE_POPUPHELP_AUTO,
	HELPTYPE_TUTORIAL,
	HELPTYPE_SPECIAL_TUTORIAL,
	HELPTYPE_MAX,
};

// Help( PopUpHelper, Tutorial )
class XHelp
{
public:
							XHelp();
							~XHelp();

			bool			InitHelp();
			void			FinishHelp();

			void			UpdateHelp( float fDeleta );

			bool			IsDestroy()						{ return m_bDestroy; }
			void			SetDestroy( bool bDestroy )		{ m_bDestroy = bDestroy; }

public:
			int				m_nHelpID;				// ID
			XHelpType		m_eHelpType;			// Help Ÿ��
			wstring			m_strImg;				// Help�� ���Ե� �̹���
			wstring			m_strImg2;				// Help�� ���Ե� �̹���2
			wstring			m_strString;			// Help ����(?) ���ڿ�
			wstring			m_strEventTrigger;		// EventTrigger
			wstring			m_strEventTriggerString;// EventTrigger Text
			wstring			m_strFinishEvent;		// Help Finish Event
			wstring			m_strFinishEventString;	// Help Finish Event String
			int				m_nLinkHelpID;			// ���� �� ������ �ٷ� ����� Help ID
			int				m_nRequiredQuest;		// ���� ����Ʈ
			int				m_nMinLevel;			// �ּ� ����
			int				m_nMaxLevel;			// �ִ� ����
			float			m_fTimer;				// �����ð�
			float			m_fFinishTime;			// ����� ������ Ÿ��

			bool			m_bDestroy;				// �ױ� ����?
};

// Ʃ�丮�� ���� �̺�Ʈ ����
enum eFinishCheckerState
{
	FINISHCHECKERSTATE_NONE,
	FINISHCHECKERSTATE_CHECKED,
	FINISHCHECKERSTATE_MAX,
};

// Ʃ�丮�� ���� �̺�Ʈ �˻� Ŭ����
class XTutorialFinishChecker
{
public:
									XTutorialFinishChecker();
	virtual							~XTutorialFinishChecker();
			
			void					SetTutorialFinishChecker( XHelp* pHelp );
			void					FinishTutorial();

			eFinishCheckerState		UpdateTutorialFinishChecker();

			bool					IsShowUI( string& strUIName );
			bool					IsUpdate();

			eFinishCheckerState		GetCheckState()										{ return m_eFinishCheckerState; }

			void					SpecialActionCmd( SPECIAL_ACTION_COMMAND eCmd )		{ m_eSpecialActionCmd = eCmd; }
			eFinishCheckerState		( XTutorialFinishChecker::*fChecker )();

protected:
			eFinishCheckerState		OnMyPlayerMove();
			eFinishCheckerState		OnNpcInteraction();
			eFinishCheckerState		OnMouseVisible();
			eFinishCheckerState		OnShowMap();
			eFinishCheckerState		OnBattleStance();
			eFinishCheckerState		OnNomalAttack();
			eFinishCheckerState		OnLooting();
			eFinishCheckerState		OnMyPlayerDodge();
			eFinishCheckerState		OnMyPlayerGuard();
			eFinishCheckerState		OnShowTalent();
			eFinishCheckerState		OnUseTalent();

			XHelp*					m_pHelp;

			SPECIAL_ACTION_COMMAND	m_eSpecialActionCmd;
			eFinishCheckerState		m_eFinishCheckerState;
};

// Ʃ�丮�� �߻� �˻� Ŭ����
class XTutorialStartChecker
{
public:
								XTutorialStartChecker();
	virtual						~XTutorialStartChecker();

	virtual	bool				InitStartChecker( XHelp* pHelp );
	virtual	void				FinishStartChecker();

	virtual	bool				UpdateStartChecker()					{ return m_bStart; }
	virtual	void				MyControlEvent( const XEvent& msg )		{}

			int					GetHelpID();

protected:	
			bool				m_bStart;
			XHelp*				m_pHelp;
};

class XKillEnemyStartChecker	: public XTutorialStartChecker
{
public:
								XKillEnemyStartChecker();
	virtual						~XKillEnemyStartChecker();

	virtual	bool				InitStartChecker( XHelp* pHelp );
	virtual	void				FinishStartChecker();

	virtual	bool				UpdateStartChecker();
	virtual	void				MyControlEvent( const XEvent& msg )		{}

protected:
			int					m_nkillEnemy;
			int					m_nKilledEpicEnemy;
			int					m_nKillCondition;
			
};

class XStateHitStartChecker		: public XTutorialStartChecker
{
public:
								XStateHitStartChecker();
	virtual						~XStateHitStartChecker();

	virtual	bool				InitStartChecker( XHelp* pHelp );
	virtual	void				FinishStartChecker();

	virtual	bool				UpdateStartChecker();
	virtual	void				MyControlEvent( const XEvent& msg );

protected:
			ACTION_STATE		m_eActionState;
			int					m_nHitCurr;
			int					m_nHitCondition;
};

// �ô� ���� �����
class XHelpHistory
{
private:
			set<int>			m_Data;								// �ô� ����
			set<int>			m_ReLoadData;						// �ٽ� ���� �ִ� ����( ���̵��� History�� ���µɰ�� )
			bool				m_bLoaded;
public:
								XHelpHistory() : m_bLoaded(false) {}
			void				InsertHistory( int nHelpID );
			void				EraseHistory( int nHelpID );
			void				InsertReloadHistory( int nHelpID );
			void				EraseReloadHistory( int nHelpID );


			void				Load();
			void				Finish();
			void				Clear();
			bool				IsExist(int nHelpID);
			bool				IsLoaded() { return m_bLoaded; }
};

class XHelpHistoryExporter
{
public:
	void Export( wstring strHistroyType, XAccountData* pAccountData, set<int>& setData );
};

class XHelpHistoryImporter
{
public:
	void Import( wstring strHistroyType, XAccountData* pAccountData, set<int>& setData);
};

/// �ð��� ������ ���� �ڵ����� ����� ���� ť
class XPopupHelpAutoQueue
{
private:
			list<XHelp*>		m_Queue;
			float				m_fQueueElapsedTime;
			float				m_fAutoCycle;
			bool				m_bAutoPopupHelp;
public:
								XPopupHelpAutoQueue();
								~XPopupHelpAutoQueue();

			void				InitAutoQueue( float fCycleTime );
			void				ClearAll();

			void				Push_Back(XHelp* pHelp);
			void				Push_Front(XHelp* pHelp);
			void				ResetTime();
			int					Update( float fDelta );
			size_t GetSize()	{ return m_Queue.size(); }
};

// HelpID
typedef map< int, XHelp* >						mapHelpData;
typedef mapHelpData::iterator					itHelpData;
// Event
typedef map< wstring, XHelp* >					mapStringTriggerHelpData;
typedef mapStringTriggerHelpData::iterator		itStringTriggerHelpData;
// StartChecker
typedef list< XTutorialStartChecker* >			listStartChecker;
typedef listStartChecker::iterator				itStartChecker;

typedef list< XHelp* >							listReserveHelp;

// Helper( PopUpHelper, Tutorial ) ���� �޴��� 
class XHelpMgr
{
public:
										XHelpMgr();
	virtual								~XHelpMgr();

			bool						LoadHelpMgr();
			bool						InitHelpMgr();
			void						FinishHelpMgr();
			void						DestroyHelpMgr();
			void						DestroyStartChecker();

			void						UpdateHelpMgr( float fDeleta );
			
			bool						IsEnabled();

			SPECIAL_ACTION_COMMAND		GetSpecialActionCmd()							{ return m_eSpecialActionCmd; }
			wstring						GetHelpImg( int nHelpID, int nImgIndex );
			wstring						GetHelpMsg( int nHelpID );
			wstring						GetEventTrigger( int nHelpID );
			float						GetHelpTimer( int nHelpID );

			bool						OnEvent( MEvent& e );
			void						SpecialActionCmd( const SPECIAL_ACTION_COMMAND& eCmd );
			void						MyControlEvent( const XEvent& msg );		// �߰��� �޼��� Ȯ�θ� �ؼ� ����

			bool						OnHelpTrigger( int nHelpID );
			bool						OnHelpTriggerCmd( int nHelpID );
			void						OnHelpTriggerClose( XHelp* pHelp = NULL );
			void						OnHelpTriggerClose( const string& strTutorialType );

			bool						OnHelpTriggerString( const wstring& strEventName );

			void						FinishedHelp( int nHelpID );
		
			void						ResetForTest();

protected:
			bool						LoadHelpData();
			void						ParseHelpData( MXmlElement* pElement, MXml* pXml );

			void						UpdateReserveHelp();
			void						UpdateStartChecker();
			void						UpdateFinishHelp();
			void						UpdatePopupHelpAuto( float fDeleta );

			bool						CheckHelpID( XHelp* pHelp );
			bool						CheckHelpType( wstring strVal, XHelp* pHelp );
			bool						InsertHelpData( XHelp* pHelp );
			void						InitEventTrigger();
			void						InitPopupHelpAuto();
			bool						SetEventChecker( XHelp* pHelp );
			void						DeleteHelpData( XHelp* pHelp );
			void						DeleteStartChecker( XTutorialStartChecker* pChecker );

			

			XHelp*						FindHelpData( int nHelpID );
			XHelp*						FindEventTriggerHelpData( const wstring& strEventName );
			bool						OnGlueEventHelpTrigger( XHelp* pHelp );
			bool						OnGlueEventPopupHelpTrigger( XHelp* pHelp );

			void						GetHelpTypeString( XHelpType eType, string& strType );

			void						SetReserveHelp( XHelp* pHelp );
			void						SetFinishHelp();

protected:
			mapHelpData					m_mapHelpData;						// Helper( PopUpHelper, Tutorial ) ����		_!!����
			mapStringTriggerHelpData	m_mapEventTriggerHelpData;			// ���� X
			listStartChecker			m_listStartChecker;					// ����
			listReserveHelp				m_listReserveHelp;
			XHelp*						m_pCurrHelp;
			XHelp*						m_pCurrPopHelp;
			XHelp*						m_pFinishDelayHelp;
			XHelpHistory				m_History;
			XPopupHelpAutoQueue			m_PopupHelpAuto;

			XTutorialFinishChecker		m_TutorialFinishChecker;

			bool						m_bEnabled;							// HelpMgr Ȱ��ȭ ����

			SPECIAL_ACTION_COMMAND		m_eSpecialActionCmd;				// �� ĳ���� Ư�� ���� ����

			float						m_fFinishDelayTime;
};
