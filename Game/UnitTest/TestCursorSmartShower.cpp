#include "stdafx.h"
#include "XCursorSmartShower.h"

SUITE(CursorSmartShower)
{
	/*

	* CtrlŰ�� ������ ���콺 Ŀ���� �����ų� �������. (Toggle)
	* CtrlŰ�� ���� ������ ������ ���콺 Ŀ���� ��� �����ִ� ���°� �ȴ�.
	* �̵�Ű�� ������ ���콺 Ŀ���� �ڵ����� �������.
		�׷��� CtrlŰ�� ������ ������ ���콺 Ŀ���� ������� �ʴ´�.


	*/

	class TestCursorSmartShower : public XCursorSmartShower
	{
	protected:
		bool	m_bMouseCursorVisible;

		virtual void ShowCursor(bool bVisible)
		{
			m_bMouseCursorVisible = bVisible;
		}
	public:
		TestCursorSmartShower() : XCursorSmartShower(), m_bMouseCursorVisible(false) {}
		virtual ~TestCursorSmartShower() {}
		virtual bool IsMouseCursorVisible()
		{
			return m_bMouseCursorVisible;
		}
	};

	class FCursorSmartShower
	{
	public:
		FCursorSmartShower()
		{

		}
		~FCursorSmartShower()
		{

		}

		TestCursorSmartShower	m_Shower;
	};

	TEST_FIXTURE(FCursorSmartShower, TestCursorSmartShower_OnKeyDown)
	{
		CHECK_EQUAL(false, m_Shower.IsMouseCursorVisible());

		m_Shower.OnKeyDown();

		CHECK_EQUAL(true, m_Shower.IsMouseCursorVisible());
	}

	TEST_FIXTURE(FCursorSmartShower, TestCursorSmartShower_OnKeyDown_ReturnValue)
	{
		CHECK_EQUAL(true, m_Shower.OnKeyDown());

		// KeyUp���� Down�� �Ǹ� false�� �����Ѵ�.
		CHECK_EQUAL(false, m_Shower.OnKeyDown());
		CHECK_EQUAL(false, m_Shower.OnKeyDown());

		m_Shower.OnKeyUp();

		CHECK_EQUAL(true, m_Shower.OnKeyDown());
	}

	TEST_FIXTURE(FCursorSmartShower, TestCursorSmartShower_OnKeyDown_Repeatedly)
	{
		CHECK_EQUAL(false, m_Shower.IsMouseCursorVisible());

		// KeyDown�� �ݺ��ؼ� ����͵� KeyUp�� �ȳ�������� �ѹ��� ó���Ѵ�.
		m_Shower.OnKeyDown();
		m_Shower.OnKeyDown();
		m_Shower.OnKeyDown();
		m_Shower.OnKeyDown();

		CHECK_EQUAL(true, m_Shower.IsMouseCursorVisible());
	}

	TEST_FIXTURE(FCursorSmartShower, TestCursorSmartShower_OnKeyUp)
	{
		CHECK_EQUAL(false, m_Shower.IsMouseCursorVisible());

		m_Shower.OnKeyDown();
		m_Shower.OnKeyUp();

		CHECK_EQUAL(true, m_Shower.IsMouseCursorVisible());


		// �ѹ� �� ������ �ٽ� �����
		m_Shower.OnKeyDown();
		m_Shower.OnKeyUp();

		CHECK_EQUAL(false, m_Shower.IsMouseCursorVisible());
	}

}