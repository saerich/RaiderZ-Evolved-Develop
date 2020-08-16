#ifndef _XGAME_H
#define _XGAME_H

#include "XGlobal.h"
#include "XObjectManager.h"
#include "CTransData.h"
#include "XGameStateMgr.h"
#include "XGameState.h"

class XWorld;
class XNonPlayer;
class XPlayer;
class XNPCInfo;
class XMagicArea;
class XBattleArena;
class XPvPAreaLogic;

using namespace rs3;

/// ���� ���� Ŭ����(MVC�� Model�� �ش���)
class XGame
{
private:
	bool					m_bCreated;

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������Ʈ / ����Ʈ
	//
	XObjectManager			m_ObjectManager;


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������Ʈ ĳ��
	//
	unsigned int			m_nUpdateObjectCacheTime;
	bool					m_bEnableUpdateObjectCache;
	void					UpdateObjectCache();

	//////////////////////////////////////////////////////////////////////////
	// ���� ������Ʈ
	XGameStateMgr			m_GameStateMgr;
	XGameState*				m_pCurrentGameState;


	void					UpdateMyInfo( float fDelta );
	void					UpdateFeedbackObjects( float fDelta );

	void					ChangeState(GAME_PLAY_STATE eState, int nParam = 0);

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� / �Ҹ�
	//
	XGame(bool bRealGameNotTest = true);
	virtual ~XGame();

	bool					Create();
	void					Destroy();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� ������Ʈ / ������
	//
	void					Update(float fDelta);
	bool					OnEvent(MEvent& e);
	bool					OnForceEvent(MEvent& e);

	void					OnPreRender();
	void					OnRender();

	void					OnActive(bool bActive);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� ���� �ʱ�ȭ
	//
	void					ChangeField(int nNewFieldID, vec3 vMyPos, vec3 vMyDir);
	void					OnWorldLoadingComplete();

	//////////////////////////////////////////////////////////////////////////
	// ���� ������Ʈ
	//
	void					OnChangeStart(GAME_PLAY_STATE ePlayState, int nParam = 0);
	void					OnChangeEnd();
	XGameState*				GetCurrentGameState()	{ return m_pCurrentGameState; }


	//////////////////////////////////////////////////////////////////////////
	// ��Ÿ
	bool					Check18Over();


	// �׽�Ʈ�� ����� ����. - ������ �ٸ� ������ �ű濹��
	void SetShowFeedback(bool bShow);

};

#endif // _XGAME_H