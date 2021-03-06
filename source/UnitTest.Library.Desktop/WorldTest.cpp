#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

#pragma region Entity Foo Test Class
	class EntityFoo : public Entity
	{
		RTTI_DECLARATIONS(EntityFoo, Entity)
	public:
		EntityFoo();
		EntityFoo(const EntityFoo& rhs) = default;

		EntityFoo& operator=(const EntityFoo& rhs) = default;

		virtual ~EntityFoo() = default;

		virtual Vector<std::string>& PrescribedAttributes() const;

		virtual void Update(WorldState& worldState);

		static void Clear();

		std::uint32_t GetFrameCount();

	private:
		static Vector<std::string> sPerscribedFooAttributes;
		std::uint32_t mFrameCount;
	};

	RTTI_DEFINITIONS(EntityFoo)
	Vector<std::string> EntityFoo::sPerscribedFooAttributes;

	EntityFoo::EntityFoo() : 
		mFrameCount(0)
	{}

	Vector<std::string>& EntityFoo::PrescribedAttributes() const
	{
		return sPerscribedFooAttributes;
	}

	void EntityFoo::Update(WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
		mFrameCount++;
	}

	void EntityFoo::Clear()
	{
		sPerscribedFooAttributes.Clear();
	}

	std::uint32_t EntityFoo::GetFrameCount()
	{
		return mFrameCount;
	}
#pragma endregion

	ConcreteFactory(Entity, EntityFoo)

	TEST_CLASS(WorldTest)
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
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif
		TEST_METHOD(TestAction)
		{
			ActionIfFactory f1;
			ActionCreateActionFactory f2;
			ActionDestroyActionFactory f3;
			ActionListFactory f4;
			ActionAssignFactory f5;

			XmlParseHelperTable::ScopeSharedData data;
			XmlParseHelperTable helper;
			XmlParseHelperAction actionHelper;
			XmlParseMaster master(data);
			master.AddHelper(helper);
			master.AddHelper(actionHelper);

			master.ParseFromFile("content/action.xml");
			XmlParseMaster::SharedData* d = &master.GetSharedData();
			XmlParseHelperTable::ScopeSharedData* data2 = d->As<XmlParseHelperTable::ScopeSharedData>();
			Scope* s = data2->GetScope();
			World* world = s->Find("World")->Get<Scope*>()->As<World>();
			Assert::IsNotNull(world);

			world->Update();
			world->Update();

			Sector* sector = world->Find("Sectors")->Get<Scope*>(0)->As<Sector>();
			Assert::IsNotNull(sector);
			Assert::IsTrue(sector->Name() == "Scene1");
			Entity* entity = sector->Find("Entities")->Get<Scope*>(0)->As<Entity>();
			Assert::IsNotNull(entity);
			Assert::IsTrue(entity->Name() == "Monster");

			ActionIf* actionIf = entity->Find("Actions")->Get<Scope*>(0)->As<ActionIf>();
			Assert::IsNotNull(actionIf);
			Assert::IsTrue(actionIf->Name() == "SampleIf");
			int condition = actionIf->Find("Condition")->Get<int>();
			Assert::IsTrue(condition == 0);

			ActionList* actionList = actionIf->Find("Then")->Get<Scope*>(0)->As<ActionList>();
			Assert::IsNotNull(actionList);
			Action* action = actionList->Find("Actions")->Get<Scope*>(1)->As<Action>();
			Assert::IsTrue(action->Name() == "NewList1");
			Assert::IsTrue(action->Is("ActionList"));
			action = actionList->Find("Actions")->Get<Scope*>(0)->As<Action>();
			Assert::IsTrue(action->Name() == "Destroy1");
			Assert::IsTrue(action->Is("ActionDestroyAction"));

			actionList = actionIf->Find("Else")->Get<Scope*>(0)->As<ActionList>();
			Assert::IsNotNull(actionList);
			action = actionList->Find("Actions")->Get<Scope*>(1)->As<Action>();
			Assert::IsTrue(action->Name() == "NewList2");
			Assert::IsTrue(action->Is("ActionList"));
			action = actionList->Find("Actions")->Get<Scope*>(0)->As<Action>();
			Assert::IsTrue(action->Name() == "Destroy1");
			Assert::IsTrue(action->Is("ActionDestroyAction"));

			action = entity->Find("Actions")->Get<Scope*>(1)->As<Action>();
			Assert::IsTrue(action->Is("ActionAssign"));

		}


		TEST_METHOD(WorldTestCreate)
		{
			World w;
			w.SetName("World");
			Assert::IsTrue(w.Name() == "World");
			Sector* s = w.CreateSector("Scene1");
			Assert::IsTrue(s->Name() == "Scene1");
			Assert::IsTrue(s->GetWorld() == &w);
			w.CreateSector("Scene2");
			Datum& datum = w.Sectors();
			Assert::IsTrue(datum.Get<Scope*>(1)->As<Sector>()->Name() == "Scene2");
			Assert::IsTrue(datum.Get<Scope*>(1)->As<Sector>()->GetWorld() == &w);

			s = datum.Get<Scope*>()->As<Sector>();
			Entity* e = s->CreateEntity("Entity", "Player1");
			Assert::IsTrue(e->Name() == "Player1");
			Assert::IsTrue(e->GetSector() == s);
			s->CreateEntity("Entity", "Player2");
			Datum& datum2 = s->Entities();
			Assert::IsTrue(datum2.Get<Scope*>(1)->As<Entity>()->Name() == "Player2");
			Assert::IsTrue(datum2.Get<Scope*>(1)->As<Entity>()->GetSector() == s);
		}

		TEST_METHOD(WorldTestUpdate)
		{
			World w;
			Sector* s = w.CreateSector("A");
			s->CreateEntity("Entity", "B");
			EntityFooFactory f;
			EntityFoo* ef = static_cast<EntityFoo*>(s->CreateEntity("EntityFoo", "C"));
			Assert::IsTrue(ef->GetFrameCount() == 0);
			w.Update();
			Assert::IsTrue(ef->GetFrameCount() == 1);
		}

	private:

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif

	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState WorldTest::sStartMemState;
#endif

}