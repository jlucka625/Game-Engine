#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(AttributedTest)
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
				//Assert::Fail(L"Memory Leaks!");
			}
		}
#endif
	
		TEST_METHOD(AttributedTestCopy)
		{
			Foo foo1;
			foo1.AppendAuxiliaryAttribute("wassup");
			Datum* newDatum = foo1.Find("health");
			newDatum->Set(1337, 0);

			Foo foo2(foo1);
			Assert::IsTrue(foo1 == foo2);

			Foo foo3;
			foo3.AppendAuxiliaryAttribute("hot diggity damn");
			newDatum = foo3.Find("EnemyName");
			newDatum->Set("Blob", 0);

			foo3 = foo2;
			Assert::IsTrue(foo2 == foo3);

			int val = 1060;
			foo1.AddInternalAttribute("Extra", val, 1);
			Assert::IsFalse(foo1 == foo2);

			foo2 = foo1;
			Assert::IsTrue(foo1 == foo2);
		}

		TEST_METHOD(AttributedTestMove)
		{
			Foo foo1;
			foo1.AppendAuxiliaryAttribute("wassup");
			Datum* newDatum = foo1.Find("health");
			newDatum->Set(1337, 0);

			Foo foo2(foo1);
			Assert::IsTrue(foo1 == foo2);

			Foo foo3(std::move(foo1));
			Assert::IsFalse(foo1 == foo2);
			Assert::IsTrue(foo3 == foo2);
		}

		TEST_METHOD(AttributedTestIsAttribute)
		{
			Foo foo;
			foo.Find("health");
			/*Assert::IsTrue(foo.IsAttributed("EnemyName"));
			Assert::IsTrue(foo.IsPrescribedAttribute("EnemyName"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("EnemyName"));
			foo["Blah"];
			Assert::IsTrue(foo.IsAttributed("Blah"));
			Assert::IsFalse(foo.IsPrescribedAttribute("Blah"));
			Assert::IsTrue(foo.IsAuxiliaryAttribute("Blah"));

			foo.AppendAuxiliaryAttribute("why");
			Assert::IsTrue(foo.IsAttributed("why"));
			Assert::IsFalse(foo.IsPrescribedAttribute("why"));
			Assert::IsTrue(foo.IsAuxiliaryAttribute("why"));*/

			/*Assert::ExpectException<std::exception>([&foo]
			{
				foo.AppendAuxiliaryAttribute("health");
			});*/

			/*Bar bar;
			Assert::IsTrue(bar.IsAttributed("EnemyName"));
			//Assert::IsTrue(bar.IsPrescribedAttribute("EnemyName"));
			//Assert::IsFalse(bar.IsAuxiliaryAttribute("EnemyName"));

			Assert::IsTrue(bar.IsAttributed("childAttr"));
			Assert::IsTrue(bar.IsPrescribedAttribute("childAttr"));
			Assert::IsFalse(bar.IsAuxiliaryAttribute("childAttr"));*/
		}
	
	private:
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};
#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState AttributedTest::sStartMemState;
#endif
}