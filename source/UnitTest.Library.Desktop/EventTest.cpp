#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	class FooBar
	{
	public:
		FooBar(int dat) : mDat(dat) {}
		int& Dat() { return mDat; };
	private:
		int mDat;
	};

	class BarSubscriber : public EventSubscriber
	{
	public:
		BarSubscriber() : wasNotified(false) {}
		virtual void Receive(const EventPublisher& e)
		{
			Event<FooBar>* evnt = e.As<Event<FooBar>>();
			if (evnt)
			{
				wasNotified = true;
			}
		}
		bool wasNotified;
	};

	class FooSubscriber : public EventSubscriber
	{
	public:
		FooSubscriber() : mMessageReceived(false), mData(0) {}

		virtual void Receive(const EventPublisher& e)
		{
			Event<RFoo>* fooEvent = e.As<Event<RFoo>>();
			if (fooEvent)
			{
				const RFoo& foo = fooEvent->Message();
				mMessageReceived = true;
				mData = const_cast<RFoo&>(foo).Data();
				Event<FooBar>::Subscribe(&b);
			}
		}

		bool Received() { return mMessageReceived; };
		int Data() { return mData; };
		BarSubscriber b;
	private:
		bool mMessageReceived;
		int mData;
	};

	class ExceptionThrower : public EventSubscriber
	{
	public:
		ExceptionThrower() = default;
		virtual void Receive(const EventPublisher& e)
		{
			UNREFERENCED_PARAMETER(e);
			throw std::exception("Recieve failed");
		}
	};

	class EventEnqueuer : public EventSubscriber
	{
	public:
		EventEnqueuer(GameTime& time) : mTime(time) {}
		virtual void Receive(const EventPublisher& e)
		{
			Event<EventQueue*>* evnt = e.As<Event<EventQueue*>>();
			if (evnt)
			{
				const EventQueue* eq = evnt->Message();
				std::shared_ptr<Event<EventQueue*>> pub =
					std::make_shared<Event<EventQueue*>>(evnt->Message());

				Milliseconds delay(0);
				const_cast<EventQueue*>(eq)->Enqueue(pub, mTime, delay);
			}
		}
	private:
		GameTime& mTime;
	};

	TEST_CLASS(EventTest)
	{
	public:

#if defined(DEBUG) | defined(_DEBUG)
		TEST_METHOD_INITIALIZE(Initialize)
		{
			Event<EventQueue*>::UnsubscribeAll();
			Event<FooBar>::UnsubscribeAll();
			Event<RFoo>::UnsubscribeAll();
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			Event<EventQueue*>::UnsubscribeAll();
			Event<FooBar>::UnsubscribeAll();
			Event<RFoo>::UnsubscribeAll();
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
			Assert::IsTrue(const_cast<RFoo&>(e2.Message()).MoreData() != const_cast<RFoo&>(e.Message()).MoreData());

			RFoo foo2(250);
			Event<RFoo> e3(foo2);
			e3 = e;
			Assert::IsTrue(e3.Message() == e.Message());
			Assert::IsTrue(const_cast<RFoo&>(e3.Message()).MoreData() != const_cast<RFoo&>(e.Message()).MoreData());
		}

		TEST_METHOD(TestEventMove)
		{
			RFoo foo(150);
			Event<RFoo> e(foo);

			Event<RFoo> eCopy(e);

			Event<RFoo> e2(std::move(e));
			Assert::IsFalse(const_cast<RFoo&>(e2.Message()).Data() == const_cast<RFoo&>(e.Message()).Data());
			Assert::IsTrue(e2.Message() == eCopy.Message());
			Assert::IsTrue(const_cast<RFoo&>(e2.Message()).MoreData() != const_cast<RFoo&>(eCopy.Message()).MoreData());

			RFoo foo2(250);
			Event<RFoo> e3(foo2);
			e3 = std::move(e2);
			Assert::IsFalse(const_cast<RFoo&>(e2.Message()).Data() == const_cast<RFoo&>(e3.Message()).Data());
			Assert::IsTrue(e3.Message() == eCopy.Message());
			Assert::IsTrue(const_cast<RFoo&>(e3.Message()).MoreData() != const_cast<RFoo&>(eCopy.Message()).MoreData());
		}

		TEST_METHOD(TestSubOnNotfiyAsync)
		{
			GameClock clock;
			GameTime gameTime;
			clock.UpdateGameTime(gameTime);

			EventQueue eq;
			Milliseconds ms(0);

			FooSubscriber f;
			RFoo rfoo1(100);
			std::shared_ptr<Event<RFoo>> ee = std::make_shared<Event<RFoo>>(rfoo1);
			ee->Subscribe(&f);
			eq.Enqueue(ee, gameTime, ms);

			for (int i = 0; i < 10; ++i)
			{
				FooSubscriber fs;
				RFoo rfoo(i);
				std::shared_ptr<Event<RFoo>> e = std::make_shared<Event<RFoo>>(rfoo);
				e->Subscribe(&fs);
				eq.Enqueue(e, gameTime, ms);
			}

			eq.Update(gameTime);
			clock.UpdateGameTime(gameTime);
			Assert::IsTrue(eq.IsEmpty());
			Assert::IsFalse(f.b.wasNotified);

			for (int i = 0; i < 10; ++i)
			{
				FooBar fb(i);
				std::shared_ptr<Event<FooBar>> e2 = std::make_shared<Event<FooBar>>(fb);
				eq.Enqueue(e2, gameTime, ms);
			}
			eq.Update(gameTime);
			Assert::IsTrue(f.b.wasNotified);
		}

		TEST_METHOD(TestEnqueueNotifyAsync)
		{
			GameClock clock;
			GameTime gameTime;
			clock.UpdateGameTime(gameTime);

			EventQueue eq;
			Milliseconds ms(0);
			
			for (int i = 0; i < 10; ++i)
			{
				EventEnqueuer eqer(gameTime);
				std::shared_ptr<Event<EventQueue*>> e = std::make_shared<Event<EventQueue*>>(&eq);
				e->Subscribe(&eqer);
				eq.Enqueue(e, gameTime, ms);
				eq.Enqueue(e, gameTime, ms);
			}	

			eq.Update(gameTime);
			Assert::IsTrue(eq.Size() == 100);
		}

		TEST_METHOD(TestExceptionNotifyAsync)
		{
			EventQueue eq;
			ExceptionThrower thrower1;
			ExceptionThrower thrower2;
			ExceptionThrower thrower3;

			RFoo foo(150);
			std::shared_ptr<Event<RFoo>> e = std::make_shared<Event<RFoo>>(foo);
			e->Subscribe(&thrower1);
			e->Subscribe(&thrower2);
			e->Subscribe(&thrower3);

			GameClock clock;
			GameTime gameTime;
			clock.UpdateGameTime(gameTime);
			Milliseconds ms(0);
			eq.Enqueue(e, gameTime, ms);

			Assert::ExpectException<std::exception>([&]
			{
				eq.Update(gameTime);
			});

			e->UnsubscribeAll();
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