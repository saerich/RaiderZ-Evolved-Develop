#include "stdafx.h"
#include "XCursorSmartReleaser.h"
#include "XUISystem.h"
#include "XController.h"
#include "XMyPlayer.h"

void XCursorSmartReleaser::Update( float fDelta, XCursor* pCursor, XUISystem* pUISystem, XController* pController )
{
	if (IsUpdateEnabled(pCursor, pUISystem, pController) == false)
	{
		m_fElapsedTime = 0.0f;
		return;
	}

	m_fElapsedTime += fDelta;

	if (m_fElapsedTime >= CURSOR_SMART_RELEASE_TIME)
	{
		DoIt(pCursor, pUISystem);
		m_fElapsedTime = 0.0f;
	}
}

bool XCursorSmartReleaser::IsUpdateEnabled(XCursor* pCursor, XUISystem* pUISystem, XController* pController)
{
	if (pCursor->IsVisible() == false) return false;
	/*if (pUISystem->IsFocusMainInput() == false) return false;*/

	// ui â�� ������ �ߵ����� �ʴ´�.
	//if (pUISystem->GetWindowShowManager())
	//{
	//	if (pUISystem->GetWindowShowManager()->IsAllHided() == false) return false;
	//}

	// ä��â Ȱ��ȭ üũ
	if (pUISystem->IsChattingBoxFocused() == true)
	{
		return false;
	}


	// �ܼ�â Ȱ��ȭ üũ
	if (pUISystem->IsConsoleVisible() == true)
	{
		return false;
	}

	// ���콺 Ŀ�� ��� Ű ������ �ִ��� üũ
	if (pUISystem->IsMouseCursorToggleKeyDown() == true)
	{
		return false;
	}

	if (gvar.Game.pMyPlayer)
	{
		if (gvar.Game.pMyPlayer->IsDead()) return false;
	}

	// �̵� Ű üũ
	if (pController == NULL) return false;

	bool bMovingKeyPressed = false;

	if (pController->IsVirtualKeyPressed(VKEY_FORWARD)) bMovingKeyPressed = true;
	else if (pController->IsVirtualKeyPressed(VKEY_BACKWARD)) bMovingKeyPressed = true;
	else if (pController->IsVirtualKeyPressed(VKEY_LEFT)) bMovingKeyPressed = true;
	else if (pController->IsVirtualKeyPressed(VKEY_RIGHT)) bMovingKeyPressed = true;

	if (bMovingKeyPressed == false) return false;

	return true;
}

void XCursorSmartReleaser::DoIt(XCursor* pCursor, XUISystem* pUISystem)
{
	if ( global.script)		global.script->GetGlueGameEvent().OnGameEvent( "UI", "HIDE_ALL_WINDOW");

/*
	if (pUISystem->GetWindowShowManager())
	{
		if (pUISystem->GetWindowShowManager()->IsAllHided() == false)
		{
			pUISystem->GetWindowShowManager()->HideAll();
		}
	}
*/

	pCursor->Show(false);

//	pUISystem->SetFocusMainInput();
}