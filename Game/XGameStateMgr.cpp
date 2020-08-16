#include "stdafx.h"
#include "XGameStateMgr.h"
#include "XGamePlayState.h"
#include "XCutSceneState.h"

XGameStateMgr::XGameStateMgr()
{
	m_eCurrentGamePlayState			= GPS_GAMEPLAY;
}

XGameStateMgr::~XGameStateMgr()
{

}

XGameState* XGameStateMgr::CreateGameState(bool bRealGameNotTest)
{
	XGamePlayState* pPlayState = new XGamePlayState(this, bRealGameNotTest);
	m_vecGameState.push_back(pPlayState);

	XCutSceneState* pCutSceneState = new XCutSceneState(this, bRealGameNotTest);
	m_vecGameState.push_back(pCutSceneState);

	return GetCurrentGameState();
}

void XGameStateMgr::DestroyGameState()
{
	for(vector<XGameState*>::iterator it = m_vecGameState.begin(); it != m_vecGameState.end(); it++)
	{
		(*it)->Destroy();
		SAFE_DELETE(*it);
	}

	m_vecGameState.clear();
}

XGameState* XGameStateMgr::ChangeGameStateStart( GAME_PLAY_STATE eState, int nParam /*= 0*/ )
{
	XGameState* pChangeGameState = GetGameState(eState);

	if(eState != m_eCurrentGamePlayState)
	{
		// ���濡 ���� ���� ��ü
		XGameState* pCurGameState = GetCurrentGameState();

		if(pChangeGameState->Ready(pCurGameState, nParam) == false)
		{
			// ���� ���� 
			// ���� ������Ʈ�� �ش�.
			return pCurGameState;
		}

		pCurGameState->Exit();

		pChangeGameState->LoadResource();

		m_eCurrentGamePlayState = eState;
	}

	return pChangeGameState;
}

void XGameStateMgr::ChangeGameStateEnd()
{
	// ������ �� ����...
	XGameState* pCurGameState = GetCurrentGameState();
	pCurGameState->Enter();
}

XGameState* XGameStateMgr::GetGameState( GAME_PLAY_STATE eState )
{
	for(vector<XGameState*>::iterator it = m_vecGameState.begin(); it != m_vecGameState.end(); it++)
	{
		if((*it)->GetPlayState() == eState)
		{
			return (*it);
		}
	}

	return NULL;
}

XGameState* XGameStateMgr::GetCurrentGameState()
{
	return GetGameState(m_eCurrentGamePlayState);
}
