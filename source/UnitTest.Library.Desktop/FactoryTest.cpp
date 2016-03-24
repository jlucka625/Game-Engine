#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

ConcreteFactory(RTTI, RFoo)

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(FactoryTest)
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
		TEST_METHOD(FactoryTestCreate)
		{
			RFooFactory factory;
			RTTI* f = factory.Create();
			Assert::IsTrue(f->Is("RFoo"));
			delete f;

			RTTI* f2 = Factory<RTTI>::Create("RFoo");
			Assert::IsTrue(f2->Is("RFoo"));
			delete f2;

			Assert::ExpectException<std::exception>([&]
			{
				Factory<RTTI>::Create("Foo");
			});
		}

		TEST_METHOD(FactoryTestFind)
		{
			Assert::IsNull(Factory<RTTI>::Find("RFoo"));
			RFooFactory* rFooFactory = new RFooFactory();
			Assert::IsNotNull(Factory<RTTI>::Find("RFoo"));
			Assert::IsTrue(rFooFactory->ClassName() == "RFoo");
			delete rFooFactory;

			rFooFactory = new RFooFactory();
			Assert::IsTrue((*Factory<RTTI>::begin()).second->ClassName() == rFooFactory->ClassName());
			Assert::ExpectException<std::exception>([&]
			{
				*Factory<RTTI>::end();
			});
			delete rFooFactory;
		}

	

	private:

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif

	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState FactoryTest::sStartMemState;
#endif

}