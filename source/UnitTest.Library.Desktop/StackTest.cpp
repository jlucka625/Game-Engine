#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(StackTest)
	{
	public:
		TEST_METHOD_INITIALIZE(StackTestInit)
		{
			Assert::IsTrue(intStack.IsEmpty());
			Assert::IsTrue(intPtrStack.IsEmpty());
			Assert::IsTrue(fooStack.IsEmpty());
		}
		
		TEST_METHOD_CLEANUP(StackTestCleanup)
		{
			intStack.Clear();
			intPtrStack.Clear();
			fooStack.Clear();
		}
		
		TEST_METHOD(StackTestPushPop)
		{
			int a = 1, b = 2, c = 3;
			RFoo f1(a), f2(b), f3(c);

			//int test
			intStack.Push(a);
			intStack.Push(b);
			intStack.Push(c);

			Assert::AreEqual(intStack.Top(), c);
			intStack.Pop();

			Assert::AreEqual(intStack.Top(), b);
			intStack.Pop();

			Assert::AreEqual(intStack.Top(), a);
			intStack.Pop();

			intStack.Pop();

			//int* test
			intPtrStack.Push(&a);
			intPtrStack.Push(&b);
			intPtrStack.Push(&c);

			Assert::AreEqual(intPtrStack.Top(), &c);
			intPtrStack.Pop();

			Assert::AreEqual(intPtrStack.Top(), &b);
			intPtrStack.Pop();

			Assert::AreEqual(intPtrStack.Top(), &a);
			intPtrStack.Pop();

			intPtrStack.Pop();

			//foo test
			fooStack.Push(f1);
			fooStack.Push(f2);
			fooStack.Push(f3);

			Assert::IsTrue(fooStack.Top() == f3);
			fooStack.Pop();

			Assert::IsTrue(fooStack.Top() == f2);
			fooStack.Pop();

			Assert::IsTrue(fooStack.Top() == f1);
			fooStack.Pop();

			fooStack.Pop();
		}
		
		TEST_METHOD(StackTestTop)
		{
			int a = 1;
			RFoo f1(a);

			//int test
			auto expression = [&] {intStack.Top(); };
			Assert::ExpectException<std::exception>(expression);

			const Stack<int> intStack2(intStack);
			auto expression2 = [&] {intStack2.Top(); };
			Assert::ExpectException<std::exception>(expression2);

			//int* test
			auto expression3 = [&] {intPtrStack.Top(); };
			Assert::ExpectException<std::exception>(expression3);

			const Stack<int*> intPtrStack2(intPtrStack);
			auto expression4 = [&] {intPtrStack2.Top(); };
			Assert::ExpectException<std::exception>(expression4);

			//foo test
			auto expression5 = [&] {fooStack.Top(); };
			Assert::ExpectException<std::exception>(expression5);

			const Stack<RFoo> fooStack2(fooStack);
			auto expression6 = [&] {fooStack2.Top(); };
			Assert::ExpectException<std::exception>(expression6);
		}
		
		TEST_METHOD(StackTestClear)
		{
			int a = 1, b = 2, c = 3;
			RFoo f1(a), f2(b), f3(c);

			//int test
			intStack.Clear();
			intStack.Push(a);
			intStack.Push(b);
			intStack.Push(c);
			Assert::AreEqual(intStack.Size(), 3U);
			intStack.Clear();
			intStack.Pop();
			intStack.Push(a);
			Assert::AreEqual(intStack.Size(), 1U);
			intStack.Pop();
			Assert::IsTrue(intStack.IsEmpty());

			//int* test
			intPtrStack.Clear();
			intPtrStack.Push(&a);
			intPtrStack.Push(&b);
			intPtrStack.Push(&c);
			Assert::AreEqual(intPtrStack.Size(), 3U);
			intPtrStack.Clear();
			intPtrStack.Pop();
			intPtrStack.Push(&a);
			Assert::AreEqual(intPtrStack.Size(), 1U);
			intPtrStack.Pop();
			Assert::IsTrue(intPtrStack.IsEmpty());

			//foo test
			fooStack.Clear();
			fooStack.Push(f1);
			fooStack.Push(f2);
			fooStack.Push(f3);
			Assert::AreEqual(fooStack.Size(), 3U);
			fooStack.Clear();
			fooStack.Pop();
			fooStack.Push(f1);
			Assert::AreEqual(fooStack.Size(), 1U);
			fooStack.Pop();
			Assert::IsTrue(fooStack.IsEmpty());
		}

		TEST_METHOD(StackTestCopy)
		{
			int a = 1, b = 2, c = 3;
			RFoo f1(a), f2(b), f3(b);

			//int test
			intStack.Push(a);
			intStack.Push(b);
			intStack.Push(c);
			Stack<int> intStackCopy(intStack);
			Assert::AreEqual(intStack.Size(), intStackCopy.Size());
			Assert::AreEqual(intStackCopy.Top(), c);
			intStackCopy.Pop();
			Assert::AreEqual(intStackCopy.Top(), b);
			intStackCopy.Pop();
			Assert::AreEqual(intStackCopy.Top(), a);

			//int* test
			intPtrStack.Push(&a);
			intPtrStack.Push(&b);
			intPtrStack.Push(&c);
			Stack<int*> intPtrStackCopy(intPtrStack);
			Assert::AreEqual(intPtrStack.Size(), intPtrStackCopy.Size());
			Assert::AreEqual(intPtrStackCopy.Top(), &c);
			intPtrStackCopy.Pop();
			Assert::AreEqual(intPtrStackCopy.Top(), &b);
			intPtrStackCopy.Pop();
			Assert::AreEqual(intPtrStackCopy.Top(), &a);

			//foo test
			fooStack.Push(f1);
			fooStack.Push(f2);
			fooStack.Push(f3);
			Stack<RFoo> fooStackCopy(fooStack);
			Assert::AreEqual(fooStack.Size(), fooStackCopy.Size());
			Assert::IsTrue(fooStackCopy.Top() == f3);
			Assert::IsTrue(fooStackCopy.Top().MoreData() != f3.MoreData());
			fooStackCopy.Pop();
			Assert::IsTrue(fooStackCopy.Top() == f2);
			Assert::IsTrue(fooStackCopy.Top().MoreData() != f2.MoreData());
			fooStackCopy.Pop();
			Assert::IsTrue(fooStackCopy.Top() == f1);
			Assert::IsTrue(fooStackCopy.Top().MoreData() != f1.MoreData());
		}

		TEST_METHOD(StackTestAssignmentOperator)
		{
			int a = 1, b = 2, c = 3;
			RFoo f1(a), f2(b), f3(b);

			//int test
			intStack.Push(a);
			intStack.Push(b);
			intStack.Push(c);
			Stack<int> intStackCopy;
			intStackCopy = intStack;
			Assert::AreEqual(intStack.Size(), intStackCopy.Size());
			Assert::AreEqual(intStackCopy.Top(), c);
			intStackCopy.Pop();
			Assert::AreEqual(intStackCopy.Top(), b);
			intStackCopy.Pop();
			Assert::AreEqual(intStackCopy.Top(), a);

			//int* test
			intPtrStack.Push(&a);
			intPtrStack.Push(&b);
			intPtrStack.Push(&c);
			Stack<int*> intPtrStackCopy;
			intPtrStackCopy = intPtrStack;
			Assert::AreEqual(intPtrStack.Size(), intPtrStackCopy.Size());
			Assert::AreEqual(intPtrStackCopy.Top(), &c);
			intPtrStackCopy.Pop();
			Assert::AreEqual(intPtrStackCopy.Top(), &b);
			intPtrStackCopy.Pop();
			Assert::AreEqual(intPtrStackCopy.Top(), &a);

			//foo test
			fooStack.Push(f1);
			fooStack.Push(f2);
			fooStack.Push(f3);
			Stack<RFoo> fooStackCopy;
			fooStackCopy = fooStack;
			Assert::AreEqual(fooStack.Size(), fooStackCopy.Size());
			Assert::IsTrue(fooStackCopy.Top() == f3);
			Assert::IsTrue(fooStackCopy.Top().MoreData() != f3.MoreData());
			fooStackCopy.Pop();
			Assert::IsTrue(fooStackCopy.Top() == f2);
			Assert::IsTrue(fooStackCopy.Top().MoreData() != f2.MoreData());
			fooStackCopy.Pop();
			Assert::IsTrue(fooStackCopy.Top() == f1);
			Assert::IsTrue(fooStackCopy.Top().MoreData() != f1.MoreData());
		}
	private:
		static Stack<int> intStack;
		static Stack<int*> intPtrStack;
		static Stack<RFoo> fooStack;
	};
	Stack<int> StackTest::intStack;
	Stack<int*> StackTest::intPtrStack;
	Stack<RFoo> StackTest::fooStack;
}