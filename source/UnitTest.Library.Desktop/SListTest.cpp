#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SListTest)
	{
	private:
		static SList<int> intList;
		static SList<int*> intPtrList;
		static SList<RFoo> fooList;
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif

	public:
#if defined(DEBUG) | defined(_DEBUG)
		TEST_METHOD_INITIALIZE(SListInitialize)
		{
			intList.Clear();
			intPtrList.Clear();
			fooList.Clear();

			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);

			Assert::IsTrue(intList.IsEmpty());
			Assert::IsTrue(intPtrList.IsEmpty());
			Assert::IsTrue(fooList.IsEmpty());
		}

		TEST_METHOD_CLEANUP(SListCleanup)
		{
			intList.Clear();
			intPtrList.Clear();
			fooList.Clear();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(SListTestBegin)
		{
			int a = 10;
			RFoo f1(a);

			//int version
			//auto expression = [this] {intList.begin(); };
			//Assert::ExpectException<std::exception>(expression);
			intList.PushFront(a);
			Assert::AreEqual(*intList.begin(), intList.Front());

			//int* version
			//auto expression2 = [this] {intPtrList.begin(); };
			//Assert::ExpectException<std::exception>(expression2);
			intPtrList.PushFront(&a);
			Assert::AreEqual(*intList.begin(), intList.Front());

			//foo version
			//auto expression3 = [this] {fooList.begin(); };
			//Assert::ExpectException<std::exception>(expression3);
			fooList.PushFront(f1);
			Assert::AreEqual(*intList.begin(), intList.Front());
		}

		TEST_METHOD(SListTestEnd)
		{
			int a = 10;
			RFoo f1(a);

			//int version
			//auto expression = [this] {intList.end(); };
			//Assert::ExpectException<std::exception>(expression);
			intList.PushFront(a);
			auto expression2 = [this] {*intList.end(); };
			Assert::ExpectException<std::exception>(expression2);

			//int* version
			//auto expression3 = [this] {intPtrList.end(); };
			//Assert::ExpectException<std::exception>(expression3);
			intPtrList.PushFront(&a);
			auto expression4 = [this] {*intPtrList.end(); };
			Assert::ExpectException<std::exception>(expression4);

			//foo version
			//auto expression5 = [this] {fooList.end(); };
			//Assert::ExpectException<std::exception>(expression5);
			auto expression6 = [this] {*fooList.end(); };
			Assert::ExpectException<std::exception>(expression6);
		}

		TEST_METHOD(SListTestInsertAfter)
		{
			//int version
			intList.PushFront(10);
			SList<int>::Iterator failIt = intList.end();
			auto expression = [&] {intList.InsertAfter(20, failIt); };
		//	Assert::ExpectException<std::exception>(expression);

			SList<int> otherIntList;
			otherIntList.PushFront(10);
			SList<int>::Iterator otherIt = otherIntList.begin();
			auto expression2 = [&] {intList.InsertAfter(20, otherIt); };
			Assert::ExpectException<std::exception>(expression2);

			SList<int>::Iterator it = intList.begin();
			intList.InsertAfter(20, it);

			Assert::AreEqual(*it, 10);
			Assert::AreEqual(*(++it), 20);

			Assert::AreEqual(*it, intList.Back());


			//int* version
			int a = 10, b = 20;
			intPtrList.PushFront(&a);
			SList<int*>::Iterator failIt2 = intPtrList.end();
			auto expression3 = [&] {intPtrList.InsertAfter(&b, failIt2); };
			//Assert::ExpectException<std::exception>(expression3);

			SList<int*> otherIntPtrList;
			otherIntPtrList.PushFront(&a);
			SList<int*>::Iterator otherIt2 = otherIntPtrList.begin();
			auto expression4 = [&] {intPtrList.InsertAfter(&b, otherIt2); };
			Assert::ExpectException<std::exception>(expression4);

			SList<int*>::Iterator it2 = intPtrList.begin();
			intPtrList.InsertAfter(&b, it2);

			Assert::AreEqual(*it2, &a);
			Assert::AreEqual(*(++it2), &b);

			Assert::AreEqual(*it2, intPtrList.Back());

			//foo version
			RFoo f1(10), f2(20);
			fooList.PushFront(f1);
			SList<RFoo>::Iterator failIt3 = fooList.end();
			auto expression5 = [&] {fooList.InsertAfter(f2, failIt3); };
			//Assert::ExpectException<std::exception>(expression5);

			SList<RFoo> otherFooList;
			otherFooList.PushFront(f1);
			SList<RFoo>::Iterator otherIt3 = otherFooList.begin();
			auto expression6 = [&] {fooList.InsertAfter(f2, otherIt3); };
			Assert::ExpectException<std::exception>(expression6);

			SList<RFoo>::Iterator it3 = fooList.begin();
			fooList.InsertAfter(f2, it3);

			Assert::IsTrue(*it3 == f1);
			Assert::IsTrue(*(++it3) == f2);

			Assert::IsTrue(*it3 == fooList.Back());

		}

		TEST_METHOD(SListTestFind)
		{
			int a = 10, b = 20, c = 30, d = 40, e = 50;
			RFoo f1(a), f2(b), f3(c), f4(d), f5(50);

			//int version
			//auto expression = [&] {intList.Find(a); };
			//Assert::ExpectException<std::exception>(expression);

			intList.PushFront(a);
			SList<int>::Iterator it = intList.Find(a);
			Assert::AreEqual(*it, a);

			intList.PushBack(b);
			intList.PushFront(c);
			intList.PushFront(d);

			it = intList.Find(d);
			Assert::AreEqual(*it, d);

			auto expression2 = [&] {*intList.Find(e); };
			Assert::ExpectException<std::exception>(expression2);

			//int* version
			//auto expression3 = [&] {intPtrList.Find(&a); };
			//Assert::ExpectException<std::exception>(expression3);

			intPtrList.PushFront(&a);
			SList<int*>::Iterator it2 = intPtrList.Find(&a);
			Assert::AreEqual(*it2, &a);

			intPtrList.PushBack(&b);
			intPtrList.PushFront(&c);
			intPtrList.PushFront(&d);

			it2 = intPtrList.Find(&d);
			Assert::AreEqual(*it2, &d);

			auto expression4 = [&] {*intPtrList.Find(&e); };
			Assert::ExpectException<std::exception>(expression4);

			//foo version
			//auto expression5 = [&] {fooList.Find(f1); };
			//Assert::ExpectException<std::exception>(expression5);

			fooList.PushFront(f1);
			SList<RFoo>::Iterator it3 = fooList.Find(f1);
			Assert::IsTrue(*it3 == f1);

			fooList.PushBack(f2);
			fooList.PushFront(f3);
			fooList.PushFront(f4);

			it3 = fooList.Find(f4);
			Assert::IsTrue(*it3 == f4);

			auto expression6 = [&] {*fooList.Find(f5); };
			Assert::ExpectException<std::exception>(expression6);

		}

		TEST_METHOD(SListTestRemove)
		{
			int a = 10, b = 20, c = 30, d = 40, e = 50;
			RFoo f1(a), f2(b), f3(c), f4(d), f5(50);

			//int version
			//auto expression = [&] {intList.Remove(a); };
			//Assert::ExpectException<std::exception>(expression);

			intList.PushFront(a);
			intList.PushBack(b);
			intList.Remove(a);
			Assert::AreEqual(intList.Front(), b);

			intList.Clear();
			intList.PushBack(a);
			intList.PushBack(b);
			intList.PushBack(c);
			intList.PushBack(d);
			intList.Remove(d);
			Assert::AreEqual(intList.Back(), c);

			intList.Remove(b);
			intList.Remove(e);

			//int* version
			//auto expression2 = [&] {intPtrList.Remove(&a); };
			//Assert::ExpectException<std::exception>(expression2);

			intPtrList.PushFront(&a);
			intPtrList.PushBack(&b);
			intPtrList.Remove(&a);
			Assert::AreEqual(intPtrList.Front(), &b);

			intPtrList.Clear();
			intPtrList.PushBack(&a);
			intPtrList.PushBack(&b);
			intPtrList.PushBack(&c);
			intPtrList.PushBack(&d);
			intPtrList.Remove(&d);
			Assert::AreEqual(intPtrList.Back(), &c);

			intPtrList.Remove(&b);
			intPtrList.Remove(&e);

			//foo version
			//auto expression3 = [&] {fooList.Remove(f1); };
			//Assert::ExpectException<std::exception>(expression3);

			fooList.PushFront(f1);
			fooList.PushBack(f2);
			fooList.Remove(f1);
			Assert::IsTrue(fooList.Front() == f2);

			fooList.Clear();
			fooList.PushBack(f1);
			fooList.PushBack(f2);
			fooList.PushBack(f3);
			fooList.PushBack(f4);
			fooList.Remove(f4);
			Assert::IsTrue(fooList.Back() == f3);

			fooList.Remove(f2);
			fooList.Remove(f5);
		}

		TEST_METHOD(SListTestEmpty)
		{
			//int test
			Assert::IsTrue(intList.IsEmpty());

			//int* test
			Assert::IsTrue(intPtrList.IsEmpty());

			//foo test
			Assert::IsTrue(fooList.IsEmpty());
		}

		TEST_METHOD(SListTestFront)
		{
			int value1 = 1, value2 = 2;
			RFoo f1(value1), f2(value2);

			//Ensuring that the elements are being stored at the front of the list every time

			//int test
			intList.PushFront(value1);
			Assert::AreEqual(value1, intList.Front());
			intList.PushFront(value2);
			Assert::AreEqual(value2, intList.Front());

			//int* test
			intPtrList.PushFront(&value1);
			Assert::AreEqual(&value1, intPtrList.Front());
			intPtrList.PushFront(&value2);
			Assert::AreEqual(&value2, intPtrList.Front());

			//foo test
			fooList.PushFront(f1);
			Assert::AreEqual(f1.Data(), fooList.Front().Data());
			fooList.PushFront(f2);
			Assert::AreEqual(f2.Data(), fooList.Front().Data());
		}

		TEST_METHOD(SListTestPushFrontAndPop)
		{
			int values[3] = { 1, 2, 3 };
			RFoo f1(values[0]), f2(values[1]), f3(values[2]);
			RFoo foos[3] = { f1, f2, f3 };
			
			//Pushing to the front and then verifying order via popping

			//int test
			intList.PushFront(values[0]);
			intList.PushFront(values[1]);
			intList.PushFront(values[2]);

			for (unsigned int i = 0; i >= intList.Size(); i++)
			{
				Assert::AreEqual(intList.Front(), values[intList.Size() - 1 - i]);
				intList.PopFront();
			}

			//int* test
			intPtrList.PushFront(&values[0]);
			intPtrList.PushFront(&values[1]);
			intPtrList.PushFront(&values[2]);

			for (unsigned int i = 0; i >= intList.Size(); i++)
			{
				Assert::AreEqual(intPtrList.Front(), &values[intList.Size() - 1 - i]);
				intPtrList.PopFront();
			}

			//foo test
			fooList.PushFront(foos[0]);
			fooList.PushFront(foos[1]);
			fooList.PushFront(foos[2]);

			for (unsigned int i = 0; i >= intList.Size(); i++)
			{
				Assert::AreEqual(fooList.Front().Data(), foos[i].Data());
				fooList.PopFront();
			}
		}

		TEST_METHOD(SListTestPushBackAndPop)
		{
			int values[3] = { 1, 2, 3 };
			RFoo f1(values[0]), f2(values[1]), f3(values[2]);
			RFoo foos[3] = { f1, f2, f3 };

			//Pushing to the back and then verifying order via popping

			//int test
			intList.PushBack(values[0]);
			intList.PushBack(values[1]);
			intList.PushBack(values[2]);

			for (unsigned int i = 0; i >= intList.Size(); i++)
			{
				Assert::AreEqual(intList.Front(), values[i]);
				intList.PopFront();
			}

			//int* test
			intPtrList.PushBack(&values[0]);
			intPtrList.PushBack(&values[1]);
			intPtrList.PushBack(&values[2]);

			for (unsigned int i = 0; i >= intList.Size(); i++)
			{
				Assert::AreEqual(intPtrList.Front(), &values[i]);
				intPtrList.PopFront();
			}

			//foo test
			fooList.PushBack(foos[0]);
			fooList.PushBack(foos[1]);
			fooList.PushBack(foos[2]);

			for (unsigned int i = 0; i >= intList.Size(); i++)
			{
				Assert::AreEqual(fooList.Front().Data(), foos[i].Data());
				fooList.PopFront();
			}
		}

		TEST_METHOD(SListTestBack)
		{
			int value1 = 1, value2 = 2;
			RFoo f1(value1), f2(value2);

			//Ensuring that the elements are being stored at the back of the list every time

			//int test
			intList.PushBack(value1);
			Assert::AreEqual(value1, intList.Back());
			intList.PushBack(value2);
			Assert::AreEqual(value2, intList.Back());

			//int* test
			intPtrList.PushBack(&value1);
			Assert::AreEqual(&value1, intPtrList.Back());
			intPtrList.PushBack(&value2);
			Assert::AreEqual(&value2, intPtrList.Back());

			//foo test
			fooList.PushBack(f1);
			Assert::AreEqual(f1.Data(), fooList.Back().Data());
			fooList.PushBack(f2);
			Assert::AreEqual(f2.Data(), fooList.Back().Data());
		}

		TEST_METHOD(SListTestSize)
		{
			int value1 = 1, value2 = 2;
			RFoo f1(value1), f2(value2);

			//int test
			intList.PushFront(value1);
			intList.PushFront(value2);
			Assert::AreEqual(2U, intList.Size());

			//int* test
			intPtrList.PushBack(&value1);
			intPtrList.PushFront(&value2);
			Assert::AreEqual(2U, intPtrList.Size());

			//foo test
			fooList.PushBack(f1);
			fooList.PushBack(f2);
			Assert::AreEqual(2U, fooList.Size());
		}

		TEST_METHOD(SListTestClear)
		{
			int value1 = 1, value2 = 2;
			RFoo f1(value1), f2(value2);
			
			//int test
			intList.PushFront(value1);
			intList.PushFront(value2);
			Assert::AreEqual(2U, intList.Size());
			intList.Clear();
			Assert::AreEqual(0U, intList.Size());

			//int* test
			intPtrList.PushFront(&value1);
			intPtrList.PushFront(&value2);
			Assert::AreEqual(2U, intPtrList.Size());
			intPtrList.Clear();
			Assert::AreEqual(0U, intPtrList.Size());

			//foo test
			fooList.PushFront(f1);
			fooList.PushFront(f2);
			Assert::AreEqual(2U, fooList.Size());
			fooList.Clear();
			Assert::AreEqual(0U, fooList.Size());
		}

		TEST_METHOD(SListTestCopy)
		{
			int value1 = 1, value2 = 2, value3 = 3;
			RFoo f1(value1), f2(value2), f3(value3);

			//int test
			intList.PushFront(value1);
			intList.PushFront(value2);
			intList.PushFront(value3);

			SList<int> intList2(intList);

			Assert::AreEqual(intList.Size(), intList2.Size());
			Assert::AreEqual(intList2.Front(), intList.Front());
			Assert::AreNotEqual(&intList2.Front(), &intList.Front());
			Assert::AreEqual(intList2.Back(), intList.Back());
			Assert::AreNotEqual(&intList2.Back(), &intList.Back());

			//int* test
			intPtrList.PushFront(&value1);
			intPtrList.PushFront(&value2);
			intPtrList.PushFront(&value3);

			SList<int*> intPtrList2(intPtrList);

			Assert::AreEqual(intPtrList.Size(), intPtrList2.Size());
			Assert::AreEqual(intPtrList2.Front(), intPtrList.Front());
			Assert::AreEqual(intPtrList2.Back(), intPtrList.Back());

			//foo test
			fooList.PushFront(f1);
			fooList.PushFront(f2);
			fooList.PushFront(f3);

			SList<RFoo> fooList2(fooList);

			Assert::AreEqual(fooList.Size(), fooList2.Size());
			Assert::AreEqual(fooList2.Front().Data(), fooList.Front().Data());
			//tests deep copy of foo
			Assert::AreNotEqual(fooList2.Front().MoreData(), fooList.Front().MoreData());
			Assert::AreEqual(fooList2.Back().Data(), fooList.Back().Data());
			//tests deep copy of foo
			Assert::AreNotEqual(fooList2.Back().MoreData(), fooList.Back().MoreData());
		}

		TEST_METHOD(SListTestAssignment)
		{
			int value1 = 1, value2 = 2, value3 = 3;
			RFoo f1(value1), f2(value2), f3(value3);

			//int test
			intList.PushFront(value1);
			intList.PushFront(value2);
			intList.PushFront(value3);

			SList<int> intList2;
			intList2 = intList;

			Assert::AreEqual(intList.Size(), intList2.Size());
			Assert::AreEqual(intList2.Front(), intList.Front());
			Assert::AreNotEqual(&intList2.Front(), &intList.Front());
			Assert::AreEqual(intList2.Back(), intList.Back());
			Assert::AreNotEqual(&intList2.Back(), &intList.Back());


			//int* test
			intPtrList.PushFront(&value1);
			intPtrList.PushFront(&value2);
			intPtrList.PushFront(&value3);

			SList<int*> intPtrList2;
			intPtrList2 = intPtrList;

			Assert::AreEqual(intPtrList.Size(), intPtrList2.Size());
			Assert::AreEqual(intPtrList2.Front(), intPtrList.Front());
			Assert::AreEqual(intPtrList2.Back(), intPtrList.Back());


			//foo test
			fooList.PushFront(f1);
			fooList.PushFront(f2);
			fooList.PushFront(f3);

			SList<RFoo> fooList2;
			fooList2 = fooList;

			Assert::AreEqual(fooList.Size(), fooList2.Size());
			Assert::AreEqual(fooList2.Front().Data(), fooList.Front().Data());
			//tests deep copy of foo
			Assert::AreNotEqual(fooList2.Front().MoreData(), fooList.Front().MoreData());
			Assert::AreEqual(fooList2.Back().Data(), fooList.Back().Data());
			//tests deep copy of foo
			Assert::AreNotEqual(fooList2.Back().MoreData(), fooList.Back().MoreData());
		}
	};
	SList<int> SListTest::intList;
	SList<int*> SListTest::intPtrList;
	SList<RFoo> SListTest::fooList;

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState SListTest::sStartMemState;
#endif

}