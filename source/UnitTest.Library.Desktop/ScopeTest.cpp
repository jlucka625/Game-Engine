#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ScopeTest)
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
	
		TEST_METHOD(ScopeTestFind)
		{
			Scope sScope;
			Assert::IsNull(sScope.Find("health"));
			Datum& a = sScope.Append("health");
			a.PushBack("health");
			Assert::IsTrue(sScope.Find("health") == &a);
		}

		TEST_METHOD(ScopeTestSearch)
		{
			Scope sScope;
			Datum& a = sScope.Append("health");
			a.PushBack("health");

			Assert::IsTrue(sScope.Search("health") == &a);
			Scope* testScope = nullptr;
			sScope.Search("health", &testScope);
			Assert::IsTrue(testScope == &sScope);

			Scope& sScope2 = sScope.AppendScope("Poop");
			Scope& sScope3 = sScope2.AppendScope("Hoopla");

			Assert::IsTrue(sScope3.Search("health") == &a);
			sScope3.Search("health", &testScope);
			Assert::IsTrue(testScope == &sScope);
		}

		TEST_METHOD(ScopeTestAppend)
		{
			Scope sScope;
			Datum& a = sScope.Append("health");
			Datum& b = sScope.Append("health");
			Assert::IsTrue(a == b);
		}

		TEST_METHOD(ScopeTestAppendScope)
		{
			Scope sScope;
			Datum& a = sScope.Append("health");
			a.PushBack("health");
			Assert::ExpectException<std::exception>([&sScope]
			{
				sScope.AppendScope("health");
			});

			Scope& sScope2 = sScope.AppendScope("Poop");
			Assert::IsTrue(sScope2.GetParent() == &sScope);
			Assert::IsTrue((*sScope.Find("Poop"))[0] == sScope2);

			Scope& sScope3 = sScope.AppendScope("Poop");
			Assert::IsTrue(sScope3.GetParent() == &sScope);
			Assert::IsTrue((*sScope.Find("Poop"))[1] == sScope3);
		}

		TEST_METHOD(ScopeTestAdopt)
		{
			Scope sScope;
			Datum& a = sScope.Append("Health");
			a.PushBack("health");
			Assert::ExpectException<std::exception>([&sScope]
			{
				sScope.Adopt(sScope, "Poop");
			});

			Scope* sScope2 = new Scope();
			Datum& b = sScope2->Append("Blah");
			b.PushBack("Blah");
			sScope.Adopt(*sScope2, "Poop");
			Assert::IsTrue(sScope2->GetParent() == &sScope);

			Scope* sScope3 = new Scope();
			Datum& c = sScope3->Append("Bye");
			c.PushBack("Bye");
			sScope.Adopt(*sScope3, "Whatever");
			Assert::IsTrue(sScope3->GetParent() == &sScope);

			Assert::IsFalse(sScope.Find("Whatever")->IsEmpty());
			sScope2->Adopt(*sScope3, "Whatever");
			Assert::IsTrue(sScope3->GetParent() == sScope2);
			Assert::IsTrue(sScope.Find("Whatever")->IsEmpty());
		}

		TEST_METHOD(ScopeTestOrphan)
		{
			Scope sScope;
			Datum& a = sScope.Append("Health");
			a.PushBack(10);
			sScope.Orphan();
			Assert::IsNull(sScope.GetParent());

			Scope* sScope2 = new Scope();
			
			sScope.Adopt(*sScope2, "poop");

			sScope.Find("poop")->RemoveAt(0);
			Assert::IsFalse(sScope2->Orphan());

			delete sScope2;

			Scope* sScope3 = new Scope();
			sScope.Adopt(*sScope3, "bloop");

			Scope* sScope4 = new Scope();
			sScope3->Adopt(*sScope4, "scoop");

			Assert::IsTrue(sScope3->Orphan());
			Assert::IsNull(sScope3->GetParent());

			delete sScope3;
		}

		TEST_METHOD(ScopeTestBracket)
		{
			//string type
			Scope sScope;
			Datum& a = sScope["health"];
			Datum& b = sScope["health"];
			Assert::IsTrue(a == b);

			//int type
			Datum& c = sScope["poop"];
			c.PushBack("poop");
			Datum& d = sScope["scoop"];
			d.PushBack("scoop");
			Datum& e = sScope["bloop"];
			e.PushBack("bloop");
			Datum& f = sScope["hoop"];
			f.PushBack("hoop");

			Datum& result = sScope[3];
			Assert::IsTrue(result == "bloop");
		}

		TEST_METHOD(ScopeTestCopy)
		{
			Scope sScope;
			sScope["a"];
			sScope["b"];
			sScope["c"];

			for (int i = 0; i < 10; i++)
			{
				sScope.AppendScope("array1");
			}

			Scope* powers = new Scope();
			sScope.Adopt(*powers, "Powers");

			(*powers)["a"];
			(*powers)["b"];


			Scope sScope2 = sScope;
			//sScope2 = sScope;

			Assert::IsTrue(sScope2 == sScope);

			Datum& a = sScope2["a"];
			a.PushBack("get rek'd");

			Assert::IsTrue(sScope2 != sScope);

			Scope sScope3(sScope2);
			Assert::IsTrue(sScope2 == sScope3);

			Datum& b = sScope3["a"];
			b.PushBack("get rek'dddd");

			Assert::IsTrue(sScope2 != sScope3);
		}

		TEST_METHOD(ScopeTestFindName)
		{
			Scope sScope;
			sScope["a"];
			sScope["b"];
			sScope["c"];

			for (int i = 0; i < 10; i++)
			{
				sScope.AppendScope("array1");
			}

			Scope* powers = new Scope();
			sScope.Adopt(*powers, "Powers");

			(*powers)["a"];
			(*powers)["b"];

			Assert::IsTrue(sScope.FindName(*powers) == "Powers");
			Assert::IsTrue(sScope.FindName(sScope) == "");
		}

		TEST_METHOD(ScopeTestClear)
		{
			Scope sScope;
			sScope["a"];
			sScope["b"];
			sScope["c"];

			for (int i = 0; i < 10; i++)
			{
				sScope.AppendScope("array1");
			}

			Scope* powers = new Scope();
			sScope.Adopt(*powers, "Powers");

			(*powers)["a"];
			(*powers)["b"];

			sScope.Clear();
		}

		TEST_METHOD(ScopeTestRTTI)
		{
			Scope* s = new Scope();
			
			s->As<Foo>();
			Assert::IsTrue(s->ToString() == "RTTI");

			Assert::IsFalse(s->Is(Foo::TypeIdClass()));
			Assert::IsTrue(s->Is(Scope::TypeIdClass()));

			Assert::IsFalse(s->Is("Foo"));
			Assert::IsTrue(s->Is("Scope"));

			Assert::IsTrue(Scope::TypeName() == "Scope");
			Assert::IsTrue(s->TypeIdInstance() == Scope::TypeIdClass());

			Scope* s2 = new Scope;

			Assert::IsFalse(s->Equals(s2));

			Assert::IsTrue(s->QueryInterface(Scope::TypeIdClass()) == s);
			Assert::IsFalse(s->QueryInterface(Foo::TypeIdClass()) == s);

			delete s;
			delete s2;
		}

	private:
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};
#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState ScopeTest::sStartMemState;
#endif
}