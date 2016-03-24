#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLibraryDesktop
{		
	TEST_CLASS(FooTest)
	{
	public:
		
		TEST_METHOD(FooTestConstructor)
		{
			int a = 10;
			RFoo f(a);
			Assert::AreEqual(a, f.Data());
		}
	};
}