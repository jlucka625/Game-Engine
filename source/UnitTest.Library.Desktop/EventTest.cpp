#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	class FooSubscriber : public EventSubscriber
	{
	public:
		FooSubscriber() : mMessageReceived(false), mData(0) {}

		virtual void Receive(const EventPublisher* e)
		{
			Event<RFoo>* fooEvent = e->As<Event<RFoo>>();
			if (fooEvent)
			{
				RFoo& foo = fooEvent->Message();
				mMessageReceived = true;
				mData = foo.Data();
			}
		}

		bool Received() { return mMessageReceived; };
		int Data() { return mData; };

	private:
		bool mMessageReceived;
		int mData;
	};

	

	TEST_CLASS(EventTest)
	{
	public:

#if defined(DEBUG) | defined(_DEBUG)
		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif
		TEST_METHOD(TestEventRTTI)
		{
			RFoo f(10);
			EventPublisher* e = new Event<RFoo>(f);

			Event<RFoo>* fe = e->As<Event<RFoo>>();
			Assert::IsNotNull(fe);
			Assert::IsTrue(e->ToString() == "RTTI");
			Assert::IsTrue(fe->ToString() == "RTTI");

			Assert::IsTrue(fe->Is(Event<RFoo>::TypeIdClass()));
			Assert::IsFalse(fe->Is(Scope::TypeIdClass()));
			Assert::IsTrue(e->Is(EventPublisher::TypeIdClass()));
			Assert::IsFalse(e->Is(Scope::TypeIdClass()));

			Assert::IsTrue(fe->Is("Event<MessageT>"));
			Assert::IsFalse(fe->Is("Scope"));
			Assert::IsTrue(e->Is("EventPublisher"));
			Assert::IsFalse(e->Is("Scope"));

			Assert::IsTrue(Event<RFoo>::TypeName() == "Event<MessageT>");
			Assert::IsTrue(EventPublisher::TypeName() == "EventPublisher");
			Assert::IsTrue(fe->TypeIdInstance() == Event<RFoo>::TypeIdClass());
			//Assert::IsTrue(e->TypeIdInstance() == EventPublisher::TypeIdClass());

			EventPublisher* e2 = new Event<RFoo>(f);
			Event<RFoo>* fe2 = e2->As<Event<RFoo>>();

			Assert::IsFalse(fe->Equals(fe2));

			Assert::IsTrue(fe->QueryInterface(Event<RFoo>::TypeIdClass()) == fe);
			Assert::IsFalse(fe->QueryInterface(Scope::TypeIdClass()) == fe);
			Assert::IsTrue(e->QueryInterface(EventPublisher::TypeIdClass()) == e);
			Assert::IsFalse(e->QueryInterface(Scope::TypeIdClass()) == e);

			delete e;
			delete e2;
		}

		TEST_METHOD(TestEventCopy)
		{
			RFoo foo(150);
			FooSubscriber fooSub;
			Event<RFoo> e(foo);

			Event<RFoo> e2(e);
			Assert::IsTrue(e2.Message() == e.Message());
			Assert::IsTrue(e2.Message().MoreData() != e.Message().MoreData());

			RFoo foo2(250);
			Event<RFoo> e3(foo2);
			e3 = e;
			Assert::IsTrue(e3.Message() == e.Message());
			Assert::IsTrue(e3.Message().MoreData() != e.Message().MoreData());
		}

		TEST_METHOD(TestEventMove)
		{
			RFoo foo(150);
			Event<RFoo> e(foo);

			Event<RFoo> eCopy(e);

			Event<RFoo> e2(std::move(e));
			Assert::IsFalse(e2.Message().Data() == e.Message().Data());
			Assert::IsTrue(e2.Message() == eCopy.Message());
			Assert::IsTrue(e2.Message().MoreData() != eCopy.Message().MoreData());

			RFoo foo2(250);
			Event<RFoo> e3(foo2);
			e3 = std::move(e2);
			Assert::IsFalse(e2.Message().Data() == e3.Message().Data());
			Assert::IsTrue(e3.Message() == eCopy.Message());
			Assert::IsTrue(e3.Message().MoreData() != eCopy.Message().MoreData());
		}

		TEST_METHOD(TestEventSubscription)
		{
			RFoo foo(150);
			FooSubscriber fooSub;
			Event<RFoo> e(foo);

			e.Send();
			Assert::IsFalse(fooSub.Received());

			e.Subscribe(&fooSub);
			e.Send();
			Assert::IsTrue(fooSub.Received());

			FooSubscriber fooSub2;
			e.Subscribe(&fooSub2);
			e.Unsubscribe(&fooSub2);
			e.Send();
			Assert::IsTrue(fooSub2.Data() == 0);
			Assert::IsTrue(fooSub.Data() == 150);

			e.Subscribe(&fooSub2);
			e.UnsubscribeAll();
			e.Send();
			Assert::IsFalse(fooSub2.Received());
		}

		TEST_METHOD(TestEventQueue)
		{
			EventQueue eq;
			Assert::IsTrue(eq.IsEmpty());
			RFoo foo(150);
			FooSubscriber fooSub;
			std::shared_ptr<Event<RFoo>> e = std::make_shared<Event<RFoo>>(foo);
			e->Subscribe(&fooSub);
			
			GameClock clock;
			GameTime gameTime;
			clock.UpdateGameTime(gameTime);
			Milliseconds ms(0);
			eq.Enqueue(e, gameTime, ms);
			Assert::IsFalse(eq.IsEmpty());
			Assert::IsTrue(e->Delay() == ms);
			Assert::IsTrue(e->TimeEnqueued() == gameTime.CurrentTime());
			Assert::IsFalse(fooSub.Received());
			eq.Update(gameTime);
			Assert::IsTrue(fooSub.Received());


			clock.UpdateGameTime(gameTime);
			eq.Enqueue(e, gameTime, ms);
			Assert::IsTrue(eq.Size() == 1U);

			eq.Clear();
			Assert::IsTrue(eq.IsEmpty());

			eq.Enqueue(e, gameTime, ms);

			FooSubscriber fooSub2;
			e->Subscribe(&fooSub2);
			eq.Send(e);
			Assert::IsTrue(eq.IsEmpty());
			Assert::IsTrue(fooSub2.Received());
			e->UnsubscribeAll();
		}

	private:

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif

	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState EventTest::sStartMemState;
#endif

}