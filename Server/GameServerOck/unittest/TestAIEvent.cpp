#include "stdafx.h"
#include "SUnitTestUtil.h"
#include "GAIEventNotifier.h"
#include "GAIEventHandler.h"
#include "GAIEventSelector.h"
#include "CSHelper.h"

SUITE(AiEvent)
{
	class TestHandler: public GAIEventHandler
	{
	public:
		virtual void OnNotify(GAIEvent aiEvent)
		{
			m_eventLastNotify = aiEvent;
		}

		bool EqualEvent(GAIEvent aiEvent)
		{
			// �ֱ� ���� �̺�Ʈ�� ������?
			return (m_eventLastNotify.eEventID == aiEvent.eEventID	&&
				m_eventLastNotify.uidField == aiEvent.uidField		&&
				m_eventLastNotify.nSectorX == aiEvent.nSectorX		&&
				m_eventLastNotify.nSectorY == aiEvent.nSectorY		&&
				m_eventLastNotify.uidNPC == aiEvent.uidNPC);
		}
	
	public:
		GAIEvent m_eventLastNotify;
	};


	class TestSelector: public GAIEventSelector
	{
	public:
		virtual bool IsSelectEvent(GAIEvent aiEvent)
		{
			// �ʵ尡 100�̸� �̺�Ʈ�� �뺸�Ѵ�.
			return aiEvent.uidField == MUID(100);
		}
	};
	
	class TestNotifier: public GAIEventNotifier
	{
	public:
		virtual void Notify(GAIEvent aiEvent)
		{
			NotifyAll(aiEvent);
		}
	};


	TEST(AiEventNotify)
	{
		TestNotifier notifier;
		TestHandler handler1;
		TestHandler handler2;
		TestHandler handler3;

		MUIDGenerator gen;
		GAIEvent event;
		event.eEventID = GAIEvent::EVENT_ID_FIELD_ENTER_PLAYER;
		event.uidField = MUID(100);
		event.nSectorX = 10;
		event.nSectorY = 20;
		event.uidNPC = gen.Generate();

		// �ڵ鷯 2�� ��� ��, �뺸
		notifier.AddEventHandler(MUID(10), &handler1);
		notifier.AddEventHandler(MUID(20), &handler2);
		notifier.Notify(event);

		CHECK(handler1.EqualEvent(event));
		CHECK(handler2.EqualEvent(event));
		CHECK(!handler3.EqualEvent(event));

		
		event.eEventID = GAIEvent::EVENT_ID_SECTOR_ENTER_PLAYER;
		event.uidField = MUID(200);
		event.nSectorX = 30;
		event.nSectorY = 40;
		event.uidNPC = gen.Generate();
		
		// 1�� ���� ��, �뺸
		notifier.RemoveEventHandler(MUID(10), &handler1);
		notifier.Notify(event);

		CHECK(!handler1.EqualEvent(event));
		CHECK(handler2.EqualEvent(event));
		CHECK(!handler3.EqualEvent(event));
	}

	TEST(AiEventSelector)
	{
		TestNotifier notifier;
		TestHandler handler;
		TestSelector selector;

		selector.RegistHandler(&handler);
		notifier.AddEventHandler(MUID(10), &selector);
		
		MUIDGenerator gen;
		GAIEvent event;
		event.eEventID = GAIEvent::EVENT_ID_FIELD_ENTER_PLAYER;
		
		// �ʵ尡 100�̸� �뺸 ���
		event.uidField = MUID(100);
		notifier.Notify(event);		
		CHECK(handler.EqualEvent(event));

		// �ʵ尡 100�� �ƴϸ�, �뺸���� �ʴ´�.
		event.uidField = MUID(200);
		notifier.Notify(event);		
		CHECK(!handler.EqualEvent(event));

		// �ٽ� 100�̸� �뺸
		event.uidField = MUID(100);
		notifier.Notify(event);		
		CHECK(handler.EqualEvent(event));
	}
}
