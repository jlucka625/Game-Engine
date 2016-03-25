#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(XmlParseMasterTest)
	{
	public:

#if defined(DEBUG) | defined(_DEBUG)
		TEST_METHOD_INITIALIZE(Initialize)
		{
			World::Clear();
			Sector::Clear();
			Entity::Clear();
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			World::Clear();
			Sector::Clear();
			Entity::Clear();
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif
		TEST_METHOD(TestSharedDataRTTI)
		{
			StringSharedData* stringData = new StringSharedData();

			Assert::IsFalse(stringData->Is(Foo::TypeIdClass()));
			Assert::IsTrue(stringData->Is(StringSharedData::TypeIdClass()));

			Assert::IsFalse(stringData->Is("Foo"));
			Assert::IsTrue(stringData->Is("StringSharedData"));

			Assert::IsTrue(StringSharedData::TypeName() == "StringSharedData");
			Assert::IsTrue(stringData->TypeIdInstance() == StringSharedData::TypeIdClass());

			StringSharedData* stringData2 = new StringSharedData();
			Assert::IsFalse(stringData->Equals(stringData2));

			Assert::IsTrue(stringData->QueryInterface(StringSharedData::TypeIdClass()) == stringData);
			Assert::IsFalse(stringData->QueryInterface(Foo::TypeIdClass()) == stringData);

			stringData->As<Foo>();

			delete stringData;
			delete stringData2;

			XmlParseHelperTable::ScopeSharedData* scopeData = new XmlParseHelperTable::ScopeSharedData();

			Assert::IsFalse(scopeData->Is(Foo::TypeIdClass()));
			Assert::IsTrue(scopeData->Is(XmlParseHelperTable::ScopeSharedData::TypeIdClass()));

			Assert::IsFalse(scopeData->Is("Foo"));
			Assert::IsTrue(scopeData->Is("ScopeSharedData"));

			Assert::IsTrue(XmlParseHelperTable::ScopeSharedData::TypeName() == "ScopeSharedData");
			Assert::IsTrue(scopeData->TypeIdInstance() == XmlParseHelperTable::ScopeSharedData::TypeIdClass());

			XmlParseHelperTable::ScopeSharedData* scopeData2 = new XmlParseHelperTable::ScopeSharedData();
			Assert::IsFalse(scopeData->Equals(scopeData2));

			Assert::IsTrue(scopeData->QueryInterface(XmlParseHelperTable::ScopeSharedData::TypeIdClass()) == scopeData);
			Assert::IsFalse(scopeData->QueryInterface(Foo::TypeIdClass()) == scopeData);

			scopeData->As<Foo>();

			delete scopeData;
			delete scopeData2;
			

		}

		TEST_METHOD(TestParseTableFromFile)
		{
			XmlParseHelperTable::ScopeSharedData data;
			XmlParseHelperTable helper;
			XmlParseMaster master(data);
			master.AddHelper(helper);

			master.ParseFromFile("content/table.xml");

			XmlParseMaster::SharedData* d = &master.GetSharedData();
			XmlParseHelperTable::ScopeSharedData* data2 = d->As<XmlParseHelperTable::ScopeSharedData>();
			Scope* s = data2->GetScope();

			Scope testScope;
			Scope Class1Scope;
			Class1Scope["num"] = 10;
			
			Class1Scope["numArray"] = 20;
			for (int i = 0; i < 9; i++)
				Class1Scope["numArray"].PushBack(20);

			Class1Scope["decimal"] = 2.5f;
			Class1Scope["decimalArray"] = 3.5f;
			for (int i = 0; i < 9; i++)
				Class1Scope["decimalArray"].PushBack(3.5f);

			Class1Scope["Greeting"] = "HelloWorld";
			Class1Scope["strArray"] = "Foo";
			for (int i = 0; i < 9; i++)
				Class1Scope["strArray"].PushBack("Foo");

			Class1Scope["Vec"] = glm::vec4(0.0f, 1.0f, 2.0f, 3.0f);
			Class1Scope["VecArray"] = glm::vec4(3.0f, 2.0f, 1.0f, 0.0f);
			for (int i = 0; i < 9; i++)
				Class1Scope["VecArray"].PushBack(glm::vec4(3.0f, 2.0f, 1.0f, 0.0f));
		
			Class1Scope["Mat"] = glm::mat4(glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f));
			Class1Scope["MatArray"] = glm::mat4(glm::vec4(2.0f), glm::vec4(2.0f), glm::vec4(2.0f), glm::vec4(2.0f));
			for (int i = 0; i < 9; i++)
				Class1Scope["MatArray"].PushBack(glm::mat4(glm::vec4(2.0f), glm::vec4(2.0f), glm::vec4(2.0f), glm::vec4(2.0f)));

			Scope otherScope;
			otherScope["num2"] = 10;

			Class1Scope.Adopt(otherScope, "Other");
			testScope.Adopt(Class1Scope, "Class1");

			Assert::IsTrue(*s == testScope);

			master.ResetParser();

			XmlParseMaster::SharedData* d2 = &master.GetSharedData();
			XmlParseHelperTable::ScopeSharedData* data3 = d2->As<XmlParseHelperTable::ScopeSharedData>();
			Scope* s2 = data3->GetScope();
			Assert::IsFalse(*s2 == testScope);

			master.ParseFromFile("content/table.xml");
			XmlParseMaster::SharedData* d3 = &master.GetSharedData();
			XmlParseHelperTable::ScopeSharedData* data4 = d3->As<XmlParseHelperTable::ScopeSharedData>();
			Scope* s3 = data4->GetScope();
			Assert::IsTrue(*s3 == testScope);

			XmlParseMaster* master2 = master.Clone();
			master2->ParseFromFile("content/table.xml");
			delete master2;

		}

		TEST_METHOD(TestParseFromFile)
		{
			StringSharedData data;
			XmlParseMaster master(data);
			TestHelper helper;
			master.AddHelper(helper);

			master.ParseFromFile("content/sample.xml");

			std::ifstream stream("content/sample.xml");
			std::stringstream buffer;
			buffer << stream.rdbuf();
			Assert::IsTrue(data.Depth() == 0U);
			Assert::IsTrue(master.GetSharedData().As<StringSharedData>()->GetString() == buffer.str());
			Assert::IsTrue("content/sample.xml" == master.GetFileName());
		}

		TEST_METHOD(TestParse)
		{
			StringSharedData data;
			XmlParseMaster master(data);
			TestHelper helper;
			master.AddHelper(helper);

			std::string xmlInput =
				"<data>\n"\
				"    <header length=\"4\">\n"\
				"            <item type=\"time\">16</item>\n"\
				"            <item type=\"string\">3843747</item>\n"\
				"            <item type=\"int16\">0</item>\n"\
				"            <item type=\"int16\">20</item>\n"\
				"    </header>\n"\
				"</data>";


			master.Parse(xmlInput, static_cast<std::uint32_t>(xmlInput.size()), true);
			Assert::IsTrue(data.Depth() == 0U);
			Assert::IsTrue(master.GetSharedData().As<StringSharedData>()->GetString() == xmlInput);

			XmlParseMaster* master2 = master.Clone();
			master2->Parse(xmlInput, static_cast<std::uint32_t>(xmlInput.size()), true);
			Assert::IsTrue(master2->GetSharedData().Depth() == 0U);
			Assert::IsTrue(&data != &master2->GetSharedData());
			Assert::IsTrue(&data == &master.GetSharedData());

			XmlParseMaster::SharedData* data2 = new XmlParseMaster::SharedData();
			master2->SetSharedData(*data2);
			Assert::IsTrue(data2 == &master2->GetSharedData());
			Assert::IsTrue(&data2->GetXmlParseMaster() == master2);
			delete master2;

			master.ResetParser();

			std::string xmlInput1 = "<data>\n";
			master.Parse(xmlInput1, static_cast<std::uint32_t>(xmlInput1.size()), false);
			Assert::IsTrue(master.GetSharedData().Depth() == 1U);
			Assert::IsTrue(master.GetSharedData().As<StringSharedData>()->GetString() == 
				xmlInput1);

			std::string xmlInput2 = "<header length=\"4\">\n";
			master.Parse(xmlInput2, static_cast<std::uint32_t>(xmlInput2.size()), false);
			Assert::IsTrue(master.GetSharedData().Depth() == 2U);
			Assert::IsTrue(master.GetSharedData().As<StringSharedData>()->GetString() == 
				xmlInput1 + xmlInput2);

			std::string xmlInput3 = "<item type=\"time\">16</item>\n";
			master.Parse(xmlInput3, static_cast<std::uint32_t>(xmlInput3.size()), false);
			Assert::IsTrue(master.GetSharedData().Depth() == 2U);
			Assert::IsTrue(master.GetSharedData().As<StringSharedData>()->GetString() == 
				xmlInput1 + xmlInput2 + xmlInput3);

			std::string xmlInput4 = "</header>\n";
			master.Parse(xmlInput4, static_cast<std::uint32_t>(xmlInput4.size()), false);
			Assert::IsTrue(master.GetSharedData().Depth() == 1U);
			Assert::IsTrue(master.GetSharedData().As<StringSharedData>()->GetString() == 
				xmlInput1 + xmlInput2 + xmlInput3 + xmlInput4);

			std::string xmlInput5 = "</data>";
			master.Parse(xmlInput5, static_cast<std::uint32_t>(xmlInput5.size()), true);
			Assert::IsTrue(master.GetSharedData().Depth() == 0U);
			Assert::IsTrue(master.GetSharedData().As<StringSharedData>()->GetString() == 
				xmlInput1 + xmlInput2 + xmlInput3 + xmlInput4 + xmlInput5);

			master.ResetParser();

			master.RemoveHelper(helper);
			master.Parse(xmlInput, static_cast<std::uint32_t>(xmlInput.size()), true);
			Assert::IsTrue(master.GetSharedData().Depth() == 0U);
			Assert::IsTrue(master.GetSharedData().As<StringSharedData>()->GetString() != xmlInput);

		}

		TEST_METHOD(TestParseWorldFromFile)
		{
			XmlParseHelperTable::ScopeSharedData data;
			XmlParseHelperTable helper;
			XmlParseMaster master(data);
			master.AddHelper(helper);

			master.ParseFromFile("content/world.xml");
			XmlParseMaster::SharedData* d = &master.GetSharedData();
			XmlParseHelperTable::ScopeSharedData* data2 = d->As<XmlParseHelperTable::ScopeSharedData>();
			Scope* s = data2->GetScope();
			World* world = s->Find("World")->Get<Scope*>()->As<World>();
			Assert::IsNotNull(world);

			Sector* sector = world->Find("Sectors")->Get<Scope*>(0)->As<Sector>();
			Assert::IsNotNull(sector);
			Assert::IsTrue(sector->Name() == "Scene1");
			Entity* entity = sector->Find("Entities")->Get<Scope*>(0)->As<Entity>();
			Assert::IsNotNull(entity);
			Assert::IsTrue(entity->Name() == "Monster");
			int health = entity->Find("health")->Get<int>();
			Assert::IsTrue(health == 100);
			entity = sector->Find("Entities")->Get<Scope*>(1)->As<Entity>();
			Assert::IsNotNull(entity);
			Assert::IsTrue(entity->Name() == "Player");
			std::string name = entity->Find("name")->Get<std::string>();
			Assert::IsTrue(name == "jon");

			sector = world->Find("Sectors")->Get<Scope*>(1)->As<Sector>();
			Assert::IsNotNull(sector);
			Assert::IsTrue(sector->Name() == "Scene2");
			entity = sector->Find("Entities")->Get<Scope*>(0)->As<Entity>();
			Assert::IsNotNull(entity);
			Assert::IsTrue(entity->Name() == "Cube");
			glm::vec4 position = entity->Find("Position")->Get<glm::vec4>();
			Assert::IsTrue(position == glm::vec4(0.0f, 1.0f, 2.0f, 3.0f));
			float scaleFactors = entity->Find("ScaleFactor")->Get<float>();
			Assert::IsTrue(scaleFactors == 0.1f);
		}

	private:

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState XmlParseMasterTest::sStartMemState;
#endif

}