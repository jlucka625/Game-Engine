#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ReactionTest)
	{
	public:

#if defined(DEBUG) | defined(_DEBUG)
		TEST_METHOD_INITIALIZE(Initialize)
		{
			World::Clear();
			Sector::Clear();
			Entity::Clear();
			Action::Clear();
			ActionList::Clear();
			ActionIf::Clear();
			ActionCreateAction::Clear();
			ActionDestroyAction::Clear();
			ActionAssign::Clear();
			ActionEvent::Clear();
			EventMessageAttributed::Clear();
			ReactionAttributed::Clear();
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			World::Clear();
			Sector::Clear();
			Entity::Clear();
			Action::Clear();
			ActionList::Clear();
			ActionIf::Clear();
			ActionCreateAction::Clear();
			ActionDestroyAction::Clear();
			ActionAssign::Clear();
			ActionEvent::Clear();
			EventMessageAttributed::Clear();
			ReactionAttributed::Clear();
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(TestReactionRTTI)
		{
			ActionList* al = new ActionList();

			Assert::IsTrue(al->ToString() == "RTTI");

			Assert::IsNotNull(al->As<ActionList>());

			Assert::IsTrue(al->Is("ActionList"));
			Assert::IsFalse(al->Is("RFoo"));

			Assert::IsTrue(al->Is(ActionList::TypeIdClass()));
			Assert::IsFalse(al->Is(RFoo::TypeIdClass()));

			Assert::IsTrue(ActionList::TypeName() == "ActionList");
			Assert::IsTrue(al->TypeIdInstance() == ActionList::TypeIdClass());

			Assert::IsTrue(al->TypeName() == "ActionList");

			ActionList* al2 = new ActionList();

			Assert::IsFalse(al->Equals(al2));

			Assert::IsTrue(al->QueryInterface(ActionList::TypeIdClass()) == al);
			Assert::IsFalse(al->QueryInterface(RFoo::TypeIdClass()) == al);

			delete al;
			delete al2;

			ReactionAttributed* ra = new ReactionAttributed();

			Assert::IsTrue(ra->ToString() == "RTTI");

			Assert::IsNotNull(ra->As<ReactionAttributed>());

			Assert::IsTrue(ra->Is("ReactionAttributed"));
			Assert::IsFalse(ra->Is("RFoo"));

			Assert::IsTrue(ra->Is(ReactionAttributed::TypeIdClass()));
			Assert::IsFalse(ra->Is(RFoo::TypeIdClass()));

			Assert::IsTrue(ReactionAttributed::TypeName() == "ReactionAttributed");
			Assert::IsTrue(ra->TypeIdInstance() == ReactionAttributed::TypeIdClass());

			Assert::IsTrue(ra->TypeName() == "ReactionAttributed");

			ReactionAttributed* ra2 = new ReactionAttributed();

			Assert::IsFalse(ra->Equals(ra2));

			Assert::IsTrue(ra->QueryInterface(ReactionAttributed::TypeIdClass()) == ra);
			Assert::IsFalse(ra->QueryInterface(RFoo::TypeIdClass()) == ra);

			delete ra;
			delete ra2;

			Reaction* r = new ReactionAttributed();

			Assert::IsTrue(r->ToString() == "RTTI");

			Assert::IsNotNull(r->As<ReactionAttributed>());

			Assert::IsTrue(r->Is("ReactionAttributed"));
			Assert::IsFalse(r->Is("RFoo"));

			Assert::IsTrue(r->Is(Reaction::TypeIdClass()));
			Assert::IsFalse(r->Is(RFoo::TypeIdClass()));

			Assert::IsTrue(Reaction::TypeName() == "Reaction");
			Assert::IsTrue(r->TypeIdInstance() == ReactionAttributed::TypeIdClass());

			std::string name = r->TypeName();
			Assert::IsTrue(name == "Reaction");

			Reaction* r2 = new ReactionAttributed();

			Assert::IsFalse(r->Equals(r2));

			Assert::IsTrue(r->QueryInterface(Reaction::TypeIdClass()) == r);
			Assert::IsFalse(r->QueryInterface(RFoo::TypeIdClass()) == r);

			delete r;
			delete r2;
		}

		TEST_METHOD(TestActionEventRTTI)
		{
			ActionEvent* ae = new ActionEvent();

			Assert::IsTrue(ae->ToString() == "RTTI");

			Assert::IsNotNull(ae->As<ActionEvent>());

			Assert::IsTrue(ae->Is("ActionEvent"));
			Assert::IsFalse(ae->Is("RFoo"));

			Assert::IsTrue(ae->Is(ActionEvent::TypeIdClass()));
			Assert::IsFalse(ae->Is(RFoo::TypeIdClass()));

			Assert::IsTrue(ActionEvent::TypeName() == "ActionEvent");
			Assert::IsTrue(ae->TypeIdInstance() == ActionEvent::TypeIdClass());
			
			Assert::IsTrue(ae->TypeName() == "ActionEvent");

			ActionEvent* ae2 = new ActionEvent();

			Assert::IsFalse(ae->Equals(ae2));

			Assert::IsTrue(ae->QueryInterface(ActionEvent::TypeIdClass()) == ae);
			Assert::IsFalse(ae->QueryInterface(RFoo::TypeIdClass()) == ae);

			delete ae;
			delete ae2;

			Action* a = new ActionEvent();

			Assert::IsTrue(a->ToString() == "RTTI");

			Assert::IsNotNull(a->As<ActionEvent>());

			Assert::IsTrue(a->Is("ActionEvent"));
			Assert::IsFalse(a->Is("RFoo"));

			Assert::IsTrue(a->Is(Action::TypeIdClass()));
			Assert::IsFalse(a->Is(RFoo::TypeIdClass()));

			Assert::IsTrue(Action::TypeName() == "Action");
			Assert::IsTrue(a->TypeIdInstance() == ActionEvent::TypeIdClass());

			Assert::IsTrue(a->TypeName() == "Action");

			Action* a2 = new ActionEvent();

			Assert::IsFalse(a->Equals(a2));

			Assert::IsTrue(a->QueryInterface(Action::TypeIdClass()) == a);
			Assert::IsFalse(a->QueryInterface(RFoo::TypeIdClass()) == a);

			delete a;
			delete a2;
		}

		TEST_METHOD(TestEventAttributedRTTI)
		{
			EventMessageAttributed* ae = new EventMessageAttributed();

			Assert::IsTrue(ae->ToString() == "RTTI");

			Assert::IsNotNull(ae->As<EventMessageAttributed>());

			Assert::IsTrue(ae->Is("EventMessageAttributed"));
			Assert::IsFalse(ae->Is("RFoo"));

			Assert::IsTrue(ae->Is(EventMessageAttributed::TypeIdClass()));
			Assert::IsFalse(ae->Is(RFoo::TypeIdClass()));

			Assert::IsTrue(EventMessageAttributed::TypeName() == "EventMessageAttributed");
			Assert::IsTrue(ae->TypeIdInstance() == EventMessageAttributed::TypeIdClass());

			Assert::IsTrue(ae->TypeName() == "EventMessageAttributed");

			EventMessageAttributed* ae2 = new EventMessageAttributed();

			Assert::IsFalse(ae->Equals(ae2));

			Assert::IsTrue(ae->QueryInterface(EventMessageAttributed::TypeIdClass()) == ae);
			Assert::IsFalse(ae->QueryInterface(RFoo::TypeIdClass()) == ae);

			delete ae;
			delete ae2;

			Attributed* a = new EventMessageAttributed();

			Assert::IsTrue(a->ToString() == "RTTI");

			Assert::IsNotNull(a->As<EventMessageAttributed>());

			Assert::IsTrue(a->Is("EventMessageAttributed"));
			Assert::IsFalse(a->Is("RFoo"));

			Assert::IsTrue(a->Is(Attributed::TypeIdClass()));
			Assert::IsFalse(a->Is(RFoo::TypeIdClass()));

			Assert::IsTrue(Attributed::TypeName() == "Attributed");
			Assert::IsTrue(a->TypeIdInstance() == EventMessageAttributed::TypeIdClass());

			Assert::IsTrue(a->TypeName() == "Attributed");

			Attributed* a2 = new EventMessageAttributed();

			Assert::IsFalse(a->Equals(a2));

			Assert::IsTrue(a->QueryInterface(Attributed::TypeIdClass()) == a);
			Assert::IsFalse(a->QueryInterface(RFoo::TypeIdClass()) == a);

			delete a;
			delete a2;
		}

		TEST_METHOD(TestEventMessageAttributedCopy)
		{
			EventMessageAttributed e;
			World* w = new World();
			std::string s = "Other";
			e.SetWorld(w);
			e.SetSubtype(s);

			EventMessageAttributed e2(e);

			Assert::IsTrue(e.GetWorld() == e2.GetWorld());
			Assert::IsTrue(e.GetSubtype() == e2.GetSubtype());

			for (std::uint32_t i = 0; i < e.PrescribedAttributes().Size(); ++i)
			{
				Assert::IsTrue(e.PrescribedAttributes()[i] == e2.PrescribedAttributes()[i]);
			}

			EventMessageAttributed e3;
			e3 = e;

			Assert::IsTrue(e3.GetWorld() == e2.GetWorld());
			Assert::IsTrue(e3.GetSubtype() == e2.GetSubtype());

			delete w;
		}

		TEST_METHOD(TestEventMessageAttributedMove)
		{
			EventMessageAttributed e;
			World* w = new World();
			std::string s = "Other";
			e.SetWorld(w);
			e.SetSubtype(s);

			EventMessageAttributed eCopy(e);

			EventMessageAttributed e2(std::move(e));

			Assert::IsTrue(e.GetWorld() != e2.GetWorld());

			Assert::IsTrue(eCopy.GetWorld() == e2.GetWorld());

			EventMessageAttributed e3;
			e3 = std::move(e2);

			Assert::IsTrue(e3.GetWorld() != e2.GetWorld());

			Assert::IsTrue(eCopy.GetWorld() == e3.GetWorld());

			delete w;
		}

		TEST_METHOD(TestReactionEvent)
		{
			ActionIfFactory f1;
			ActionCreateActionFactory f2;
			ActionDestroyActionFactory f3;
			ActionListFactory f4;
			ActionAssignFactory f5;
			ActionEventFactory f6;
			ReactionAttributedFactory f7;

			XmlParseHelperTable::ScopeSharedData data;
			XmlParseHelperTable helper;
			XmlParseHelperAction actionHelper;
			XmlParseMaster master(data);
			master.AddHelper(helper);
			master.AddHelper(actionHelper);

			master.ParseFromFile("content/reaction.xml");
			XmlParseMaster::SharedData* d = &master.GetSharedData();
			XmlParseHelperTable::ScopeSharedData* data2 = d->As<XmlParseHelperTable::ScopeSharedData>();
			Scope* s = data2->GetScope();
			World* world = s->Find("World")->Get<Scope*>()->As<World>();
			Assert::IsNotNull(world);

			world->Update();
			world->Update();

			Sector* sector = world->Find("Sectors")->Get<Scope*>(0)->As<Sector>();
			Entity* entity = sector->Find("Entities")->Get<Scope*>(0)->As<Entity>();
			ReactionAttributed* reaction = entity->Find("JumpReaction")->Get<Scope*>(0)->As<ReactionAttributed>();

			Datum* datum = reaction->Find("JumpHeight");
			int height = datum->Get<int>(0);
			Assert::IsTrue(height == 100);

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

	private:

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif

	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState ReactionTest::sStartMemState;
#endif

}