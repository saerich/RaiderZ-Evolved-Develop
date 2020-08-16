#pragma once

class XCursor;
class XUISystem;


#define CURSOR_SMART_RELEASE_TIME  (0.01f)		// Ű ������ ���� ��� ����

// Ŀ���� ������ �� �̵�Ű�� ������ �ڵ������� Ŀ���� �����ϴ� Ŭ����
class XCursorSmartReleaser
{
private:
	float		m_fElapsedTime;
	bool		IsUpdateEnabled(XCursor* pCursor, XUISystem* pUISystem, XController* pController);
	void		DoIt(XCursor* pCursor, XUISystem* pUISystem);
public:
	XCursorSmartReleaser() : m_fElapsedTime(0.0f) {}
	void Update(float fDelta, XCursor* pCursor, XUISystem* pUISystem, XController* pController);
};