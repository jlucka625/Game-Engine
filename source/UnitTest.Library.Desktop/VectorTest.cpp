#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(VectorTest)
	{
	public:
#if defined(DEBUG) | defined(_DEBUG)
		TEST_METHOD_INITIALIZE(VectorTestInitialize)
		{
			intVector.Clear();
			intPtrVector.Clear();
			fooVector.Clear();

			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);

			Assert::IsTrue(intVector.IsEmpty());
			Assert::IsTrue(intPtrVector.IsEmpty());
			Assert::IsTrue(fooVector.IsEmpty());
		}

		TEST_METHOD_CLEANUP(VectorTestCleanup)
		{
			intVector.Clear();
			intPtrVector.Clear();
			fooVector.Clear();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(VectorTestSubscriptOperator)
		{
			int a = 10, b = 20;
			RFoo f1(a), f2(b);

			//int test
			auto expression = [&] {intVector[5]; };
			Assert::ExpectException<std::exception>(expression);
			intVector.PushBack(a);
			intVector.PushBack(b);
			Assert::AreEqual(intVector[1], b);

			const Vector<int> intVector2(intVector);
			Assert::AreEqual(intVector2[1], b);
			
			//int* test
			auto expression2 = [&] {intPtrVector[5]; };
			Assert::ExpectException<std::exception>(expression2);
			intPtrVector.PushBack(&a);
			intPtrVector.PushBack(&b);
			Assert::AreEqual(intPtrVector[1], &b);

			const Vector<int*> intPtrVector2(intPtrVector);
			Assert::AreEqual(intPtrVector2[1], &b);

			//foo test
			auto expression3 = [&] {fooVector[5]; };
			Assert::ExpectException<std::exception>(expression3);
			fooVector.PushBack(f1);
			fooVector.PushBack(f2);
			Assert::IsTrue(fooVector[1] == f2);

			const Vector<RFoo> fooVector2(fooVector);
			Assert::IsTrue(fooVector2[1] == f2);
		}

		TEST_METHOD(VectorTestPushPop)
		{
			int values[3] = { 1, 2, 3 };
			RFoo f1(values[0]), f2(values[1]), f3(values[2]);
			RFoo foos[3] = { f1, f2, f3 };

			//int test
			auto expression = [&] {intVector.PopBack(); };
			Assert::ExpectException<std::exception>(expression);

			intVector.PushBack(values[0]);
			intVector.PushBack(values[1]);
			intVector.PushBack(values[2]);

			for (unsigned int i = 0; i < intVector.Size(); i++)
			{
				Assert::AreEqual(intVector.Back(), values[2 - i]);
				intVector.PopBack();
			}

			//int* test
			auto expression2 = [&] {intPtrVector.PopBack(); };
			Assert::ExpectException<std::exception>(expression2);

			intPtrVector.PushBack(&values[0]);
			intPtrVector.PushBack(&values[1]);
			intPtrVector.PushBack(&values[2]);

			for (unsigned int i = 0; i < intVector.Size(); i++)
			{
				Assert::AreEqual(intPtrVector.Back(), &values[2 - i]);
				intPtrVector.PopBack();
			}

			//foo test
			auto expression3 = [&] {fooVector.PopBack(); };
			Assert::ExpectException<std::exception>(expression3);

			fooVector.PushBack(foos[0]);
			fooVector.PushBack(foos[1]);
			fooVector.PushBack(foos[2]);

			for (unsigned int i = 0; i < intVector.Size(); i++)
			{
				Assert::IsTrue(fooVector.Back() == foos[2 - i]);
				fooVector.PopBack();
			}
		}

		TEST_METHOD(VectorTestEmpty)
		{
			//int test
			Assert::IsTrue(intVector.IsEmpty());

			//int* test
			Assert::IsTrue(intPtrVector.IsEmpty());

			//foo test
			Assert::IsTrue(fooVector.IsEmpty());
		}

		TEST_METHOD(VectorTestFront)
		{
			int value1 = 1, value2 = 2;
			RFoo f1(value1), f2(value2);

			//int test
			auto expression = [&] {intVector.Front(); };
			Assert::ExpectException<std::exception>(expression);

			const Vector<int> intVector3;
			auto expression2 = [&] {intVector3.Front(); };
			Assert::ExpectException<std::exception>(expression2);

			intVector.PushBack(value1);
			Assert::AreEqual(value1, intVector.Front());
			intVector.PushBack(value2);
			Assert::AreEqual(value1, intVector.Front());

			const Vector<int> intVector2(intVector);
			Assert::AreEqual(value1, intVector2.Front());

			//int* test
			auto expression3 = [&] {intPtrVector.Front(); };
			Assert::ExpectException<std::exception>(expression3);

			const Vector<int*> intPtrVector3;
			auto expression4 = [&] {intPtrVector3.Front(); };
			Assert::ExpectException<std::exception>(expression4);

			intPtrVector.PushBack(&value1);
			Assert::AreEqual(&value1, intPtrVector.Front());
			intPtrVector.PushBack(&value2);
			Assert::AreEqual(&value1, intPtrVector.Front());

			const Vector<int*> intPtrVector2(intPtrVector);
			Assert::AreEqual(&value1, intPtrVector2.Front());

			//foo test
			auto expression5 = [&] {fooVector.Front(); };
			Assert::ExpectException<std::exception>(expression5);

			const Vector<int> fooVector3;
			auto expression6 = [&] {fooVector3.Front(); };
			Assert::ExpectException<std::exception>(expression6);

			fooVector.PushBack(f1);
			Assert::IsTrue(f1 == fooVector.Front());
			fooVector.PushBack(f2);
			Assert::IsTrue(f1 == fooVector.Front());

			const Vector<RFoo> fooVector2(fooVector);
			Assert::IsTrue(f1 == fooVector2.Front());
		}

		TEST_METHOD(VectorTestBack)
		{
			int value1 = 1, value2 = 2;
			RFoo f1(value1), f2(value2);

			//int test
			auto expression = [&] {intVector.Back(); };
			Assert::ExpectException<std::exception>(expression);

			const Vector<int> intVector3;
			auto expression2 = [&] {intVector3.Back(); };
			Assert::ExpectException<std::exception>(expression);

			intVector.PushBack(value1);
			Assert::AreEqual(value1, intVector.Back());
			intVector.PushBack(value2);
			Assert::AreEqual(value2, intVector.Back());

			const Vector<int> intVector2(intVector);
			Assert::AreEqual(value2, intVector2.Back());

			//int* test
			auto expression3 = [&] {intPtrVector.Front(); };
			Assert::ExpectException<std::exception>(expression3);

			const Vector<int*> intPtrVector3;
			auto expression4 = [&] {intPtrVector3.Front(); };
			Assert::ExpectException<std::exception>(expression4);

			intPtrVector.PushBack(&value1);
			Assert::AreEqual(&value1, intPtrVector.Back());
			intPtrVector.PushBack(&value2);
			Assert::AreEqual(&value2, intPtrVector.Back());

			const Vector<int*> intPtrVector2(intPtrVector);
			Assert::AreEqual(&value2, intPtrVector2.Back());

			//foo test
			auto expression5 = [&] {fooVector.Front(); };
			Assert::ExpectException<std::exception>(expression5);

			const Vector<int> fooVector3;
			auto expression6 = [&] {fooVector3.Front(); };
			Assert::ExpectException<std::exception>(expression6);

			fooVector.PushBack(f1);
			Assert::AreEqual(f1.Data(), fooVector.Back().Data());
			fooVector.PushBack(f2);
			Assert::AreEqual(f2.Data(), fooVector.Back().Data());

			const Vector<RFoo> fooVector2(fooVector);
			Assert::IsTrue(f2 == fooVector2.Back());
		}

		TEST_METHOD(VectorTestBegin)
		{
			int a = 10;
			RFoo f1(a);

			//int version
			auto expression = [this] {intVector.begin(); };
			Assert::ExpectException<std::exception>(expression);
			intVector.PushBack(a);
			Assert::AreEqual(*intVector.begin(), intVector.Front());

			//int* version
			auto expression2 = [this] {intPtrVector.begin(); };
			Assert::ExpectException<std::exception>(expression2);
			intPtrVector.PushBack(&a);
			Assert::AreEqual(*intVector.begin(), intVector.Front());

			//foo version
			auto expression3 = [this] {fooVector.begin(); };
			Assert::ExpectException<std::exception>(expression3);
			fooVector.PushBack(f1);
			Assert::AreEqual(*intVector.begin(), intVector.Front());
		}

		TEST_METHOD(VectorTestEnd)
		{
			int a = 10;
			RFoo f1(a);

			//int version
			auto expression = [this] {intVector.end(); };
			Assert::ExpectException<std::exception>(expression);
			intVector.PushBack(a);
			auto expression2 = [this] {*intVector.end(); };
			Assert::ExpectException<std::exception>(expression2);

			//int* version
			auto expression3 = [this] {intPtrVector.end(); };
			Assert::ExpectException<std::exception>(expression3);
			intPtrVector.PushBack(&a);
			auto expression4 = [this] {*intPtrVector.end(); };
			Assert::ExpectException<std::exception>(expression4);

			//foo version
			auto expression5 = [this] {fooVector.end(); };
			Assert::ExpectException<std::exception>(expression5);
			fooVector.PushBack(f1);
			auto expression6 = [this] {*fooVector.end(); };
			Assert::ExpectException<std::exception>(expression6);
		}

		TEST_METHOD(VectorTestAt)
		{
			int a = 10, b = 20;
			RFoo f1(a), f2(b);

			//int test
			auto expression = [&] {intVector.At(5); };
			Assert::ExpectException<std::exception>(expression);
			intVector.PushBack(a);
			intVector.PushBack(b);
			Assert::AreEqual(intVector.At(1), b);

			const Vector<int> intVector2(intVector);
			Assert::AreEqual(intVector2.At(1), b);

			//int* test
			auto expression2 = [&] {intPtrVector.At(5); };
			Assert::ExpectException<std::exception>(expression2);
			intPtrVector.PushBack(&a);
			intPtrVector.PushBack(&b);
			Assert::AreEqual(intPtrVector.At(1), &b);

			const Vector<int*> intPtrVector2(intPtrVector);
			Assert::AreEqual(intPtrVector2.At(1), &b);

			//foo test
			auto expression3 = [&] {fooVector.At(5); };
			Assert::ExpectException<std::exception>(expression3);
			fooVector.PushBack(f1);
			fooVector.PushBack(f2);
			Assert::IsTrue(fooVector.At(1) == f2);

			const Vector<RFoo> fooVector2(fooVector);
			Assert::IsTrue(fooVector2.At(1) == f2);
		}

		TEST_METHOD(VectorTestReserve)
		{
			Vector<SList<int>> v(10, true);
			v[3].PushBack(10);
			v[9].PushBack(1);
			v.Reserve(100);

			//int test
			for (int i = 0; i < 50; i++)
			{
				intVector.PushBack(i);
			}
			Assert::AreEqual(intVector.Capacity(), 50U);
			for (int i = 0; i < 50; i++)
			{
				Assert::AreEqual(intVector.At(i), i);
			}
			intVector.Reserve(100);
			Assert::AreEqual(intVector.Capacity(), 100U);
			for (int i = 0; i < 50; i++)
			{
				Assert::AreEqual(intVector.At(i), i);
			}
			intVector.Reserve(50);
			Assert::AreEqual(intVector.Capacity(), 100U);

			//int* test
			int array[50];
			for (int i = 0; i < 50; i++)
			{
				array[i] = i;
			}
			for (int i = 0; i < 50; i++)
			{
				intPtrVector.PushBack(&array[i]);
			}
			Assert::AreEqual(intPtrVector.Capacity(), 50U);
			for (int i = 0; i < 50; i++)
			{
				Assert::AreEqual(intPtrVector.At(i), &array[i]);
			}
			intPtrVector.Reserve(100);
			Assert::AreEqual(intPtrVector.Capacity(), 100U);
			for (int i = 0; i < 50; i++)
			{
				Assert::AreEqual(intPtrVector.At(i), &array[i]);
			}
			intPtrVector.Reserve(50);
			Assert::AreEqual(intPtrVector.Capacity(), 100U);

			//foo test
			RFoo *array2 = new RFoo[50];
			for (int i = 0; i < 50; i++)
			{
				fooVector.PushBack(array2[i]);
			}
			Assert::AreEqual(fooVector.Capacity(), 50U);
			for (int i = 0; i < 50; i++)
			{
				Assert::IsTrue(fooVector.At(i) == array2[i]);
			}
			fooVector.Reserve(100);
			Assert::AreEqual(fooVector.Capacity(), 100U);
			for (int i = 0; i < 50; i++)
			{
				Assert::IsTrue(fooVector.At(i) == array2[i]);
			}
			fooVector.Reserve(50);
			Assert::AreEqual(fooVector.Capacity(), 100U);
			delete[] array2;
		}

		TEST_METHOD(VectorTestFind)
		{
			int a = 10, b = 20, c = 30, d = 40, e = 50;
			RFoo f1(a), f2(b), f3(c), f4(d), f5(50);

			//int version
			auto expression = [&] {intVector.Find(a); };
			Assert::ExpectException<std::exception>(expression);

			intVector.PushBack(a);
			Vector<int>::Iterator it = intVector.Find(a);
			Assert::AreEqual(*it, a);

			intVector.PushBack(b);
			intVector.PushBack(c);
			intVector.PushBack(d);

			it = intVector.Find(d);
			Assert::AreEqual(*it, d);

			auto expression2 = [&] {*intVector.Find(e); };
			Assert::ExpectException<std::exception>(expression2);

			//int* version
			auto expression3 = [&] {intPtrVector.Find(&a); };
			Assert::ExpectException<std::exception>(expression3);

			intPtrVector.PushBack(&a);
			Vector<int*>::Iterator it2 = intPtrVector.Find(&a);
			Assert::AreEqual(*it2, &a);

			intPtrVector.PushBack(&b);
			intPtrVector.PushBack(&c);
			intPtrVector.PushBack(&d);

			it2 = intPtrVector.Find(&d);
			Assert::AreEqual(*it2, &d);

			auto expression4 = [&] {*intPtrVector.Find(&e); };
			Assert::ExpectException<std::exception>(expression4);

			//foo version
			auto expression5 = [&] {fooVector.Find(f1); };
			Assert::ExpectException<std::exception>(expression5);

			fooVector.PushBack(f1);
			Vector<RFoo>::Iterator it3 = fooVector.Find(f1);
			Assert::IsTrue(*it3 == f1);

			fooVector.PushBack(f2);
			fooVector.PushBack(f3);
			fooVector.PushBack(f4);

			it3 = fooVector.Find(f4);
			Assert::IsTrue(*it3 == f4);

			auto expression6 = [&] {*fooVector.Find(f5); };
			Assert::ExpectException<std::exception>(expression6);
		}

		TEST_METHOD(VectorTestClear)
		{
			int value1 = 1, value2 = 2;
			RFoo f1(value1), f2(value2);

			//int test
			intVector.PushBack(value1);
			intVector.PushBack(value2);
			Assert::AreEqual(2U, intVector.Size());
			intVector.Clear();
			Assert::AreEqual(0U, intVector.Size());

			//int* test
			intPtrVector.PushBack(&value1);
			intPtrVector.PushBack(&value2);
			Assert::AreEqual(2U, intPtrVector.Size());
			intPtrVector.Clear();
			Assert::AreEqual(0U, intPtrVector.Size());

			//foo test
			fooVector.PushBack(f1);
			fooVector.PushBack(f2);
			Assert::AreEqual(2U, fooVector.Size());
			fooVector.Clear();
			Assert::AreEqual(0U, fooVector.Size());
		}

		TEST_METHOD(VectorTestRemove)
		{
			int a = 10, b = 20, c = 30, d = 40, e = 50;
			RFoo f1(a), f2(b), f3(c), f4(d), f5(e);

			
			//int version
			auto expression = [&] {intVector.Remove(a); };
			Assert::ExpectException<std::exception>(expression);

			intVector.PushBack(a);
			intVector.PushBack(b);
			intVector.Remove(a);
			Assert::AreEqual(intVector.Front(), b);

			intVector.Clear();
			intVector.PushBack(a);
			intVector.PushBack(b);
			intVector.PushBack(c);
			intVector.PushBack(d);
			intVector.Remove(d);
			Assert::AreEqual(intVector.Back(), c);

			intVector.Remove(b);
			intVector.Remove(e);

			
			//int* version
			auto expression2 = [&] {intPtrVector.Remove(&a); };
			Assert::ExpectException<std::exception>(expression2);

			intPtrVector.PushBack(&a);
			intPtrVector.PushBack(&b);
			intPtrVector.Remove(&a);
			Assert::AreEqual(intPtrVector.Front(), &b);

			intPtrVector.Clear();
			intPtrVector.PushBack(&a);
			intPtrVector.PushBack(&b);
			intPtrVector.PushBack(&c);
			intPtrVector.PushBack(&d);
			intPtrVector.Remove(&d);
			Assert::AreEqual(intPtrVector.Back(), &c);

			intPtrVector.Remove(&b);
			intPtrVector.Remove(&e);

			
			//foo version
			auto expression3 = [&] {fooVector.Remove(f1); };
			Assert::ExpectException<std::exception>(expression3);

			fooVector.PushBack(f1);
			fooVector.PushBack(f2);
			fooVector.Remove(f1);
			Assert::IsTrue(fooVector.Front() == f2);

			fooVector.Clear();
			fooVector.PushBack(f1);
			fooVector.PushBack(f2);
			fooVector.PushBack(f3);
			fooVector.PushBack(f4);
			fooVector.Remove(f4);
			Assert::IsTrue(fooVector.Back() == f3);

			fooVector.Remove(f2);
			fooVector.Remove(f5);
		}

		TEST_METHOD(VectorTestDeepCopy)
		{
			int a = 1, b = 2, c = 3;
			RFoo f1(a), f2(b), f3(c);

			//int version
			intVector.PushBack(a);
			intVector.PushBack(b);
			intVector.PushBack(c);

			Vector<int> intVectorCopy(intVector);
			Assert::AreEqual(intVectorCopy.Capacity(), intVector.Capacity());
			Assert::AreEqual(intVectorCopy.Size(), intVector.Size());
			for (int i = 0; i < 3; i++)
			{
				Assert::AreNotEqual(&intVectorCopy[i], &intVector[i]);
				Assert::AreEqual(intVectorCopy[i], intVector[i]);
			}

			//int* version
			intPtrVector.PushBack(&a);
			intPtrVector.PushBack(&b);
			intPtrVector.PushBack(&c);

			Vector<int*> intPtrVectorCopy(intPtrVector);
			Assert::AreEqual(intPtrVectorCopy.Capacity(), intPtrVector.Capacity());
			Assert::AreEqual(intPtrVectorCopy.Size(), intPtrVector.Size());
			for (int i = 0; i < 3; i++)
			{
				Assert::IsFalse(&intPtrVectorCopy[i] == &intPtrVector[i]);
				Assert::AreEqual(intPtrVectorCopy[i], intPtrVector[i]);
			}

			//foo version
			fooVector.PushBack(f1);
			fooVector.PushBack(f2);
			fooVector.PushBack(f3);

			Vector<RFoo> fooVectorCopy(fooVector);
			Assert::AreEqual(fooVectorCopy.Capacity(), fooVector.Capacity());
			Assert::AreEqual(fooVectorCopy.Size(), fooVector.Size());
			for (int i = 0; i < 3; i++)
			{
				Assert::IsFalse(&fooVectorCopy[i] == &fooVector[i]);
				Assert::IsTrue(fooVectorCopy[i] == fooVector[i]);
			}
		}

		TEST_METHOD(VectorTestAssignmentOperator)
		{
			int a = 1, b = 2, c = 3;
			RFoo f1(a), f2(b), f3(c);

			//int version
			intVector.PushBack(a);
			intVector.PushBack(b);
			intVector.PushBack(c);

			Vector<int> intVectorCopy;
			intVectorCopy = intVector;
			Assert::AreEqual(intVectorCopy.Capacity(), intVector.Capacity());
			Assert::AreEqual(intVectorCopy.Size(), intVector.Size());
			for (int i = 0; i < 3; i++)
			{
				Assert::AreNotEqual(&intVectorCopy[i], &intVector[i]);
				Assert::AreEqual(intVectorCopy[i], intVector[i]);
			}

			//int* version
			intPtrVector.PushBack(&a);
			intPtrVector.PushBack(&b);
			intPtrVector.PushBack(&c);

			Vector<int*> intPtrVectorCopy;
			intPtrVectorCopy = intPtrVector;
			Assert::AreEqual(intPtrVectorCopy.Capacity(), intPtrVector.Capacity());
			Assert::AreEqual(intPtrVectorCopy.Size(), intPtrVector.Size());
			for (int i = 0; i < 3; i++)
			{
				Assert::IsFalse(&intPtrVectorCopy[i] == &intPtrVector[i]);
				Assert::AreEqual(intPtrVectorCopy[i], intPtrVector[i]);
			}

			//foo version
			fooVector.PushBack(f1);
			fooVector.PushBack(f2);
			fooVector.PushBack(f3);

			Vector<RFoo> fooVectorCopy;
			fooVectorCopy = fooVector;
			Assert::AreEqual(fooVectorCopy.Capacity(), fooVector.Capacity());
			Assert::AreEqual(fooVectorCopy.Size(), fooVector.Size());
			for (int i = 0; i < 3; i++)
			{
				Assert::IsFalse(&fooVectorCopy[i] == &fooVector[i]);
				Assert::IsTrue(fooVectorCopy[i] == fooVector[i]);
			}
		}

		TEST_METHOD(VectorTestIteratorIncrement)
		{
			int a = 1;
			RFoo f1(a);

			//int test
			Vector<int>::Iterator it;
			auto expression = [&] {it++; };
			Assert::ExpectException<std::exception>(expression);

			intVector.PushBack(a);
			Vector<int>::Iterator it2 = intVector.end();
			auto expression2 = [&] {it2++; };
			Assert::ExpectException<std::exception>(expression2);

			//int* test
			Vector<int*>::Iterator it3;
			auto expression3 = [&] {it3++; };
			Assert::ExpectException<std::exception>(expression3);

			intPtrVector.PushBack(&a);
			Vector<int*>::Iterator it4 = intPtrVector.end();
			auto expression4 = [&] {it4++; };
			Assert::ExpectException<std::exception>(expression4);

			//foo test
			Vector<RFoo>::Iterator it5;
			auto expression5 = [&] {it5++; };
			Assert::ExpectException<std::exception>(expression5);

			fooVector.PushBack(f1);
			Vector<RFoo>::Iterator it6 = fooVector.end();
			auto expression6 = [&] {it6++; };
			Assert::ExpectException<std::exception>(expression6);
		}

		TEST_METHOD(VectorTestIteratorConstDereference)
		{
			int a = 1;
			RFoo f1(a);

			//int version
			intVector.PushBack(a);

			const Vector<int> intVector2(intVector);
			Vector<int>::Iterator it = intVector2.begin();
			Assert::AreEqual(*it, a);

			//int* version
			intPtrVector.PushBack(&a);

			const Vector<int*> intPtrVector2(intPtrVector);
			Vector<int*>::Iterator it2 = intPtrVector2.begin();
			Assert::AreEqual(*it2, &a);

			//foo version
			fooVector.PushBack(f1);

			const Vector<RFoo> fooVector2(fooVector);
			Vector<RFoo>::Iterator it3 = fooVector2.begin();
			Assert::IsTrue(*it3 == f1);

		}

		TEST_METHOD(VectorTestMove)
		{
			int a = 1, b = 2;
			RFoo f1(a), f2(b);

			//int test
			intVector.PushBack(a);
			intVector.PushBack(b);
			
			Vector<int> intVector3;
			intVector3 = std::move(intVector);
			Assert::AreEqual(intVector3[0], a);
			Assert::AreEqual(intVector3[1], b);
			auto expression2 = [&] {intVector.Front(); };
			Assert::ExpectException<std::exception>(expression2);

			//int* test
			intPtrVector.PushBack(&a);
			intPtrVector.PushBack(&b);

			Vector<int*> intPtrVector3;
			intPtrVector3 = std::move(intPtrVector);
			Assert::AreEqual(intPtrVector3[0], &a);
			Assert::AreEqual(intPtrVector3[1], &b);
			auto expression4 = [&] {intPtrVector.Front(); };
			Assert::ExpectException<std::exception>(expression4);

			//foo test
			fooVector.PushBack(f1);
			fooVector.PushBack(f2);

			Vector<RFoo> fooVector3;
			fooVector3 = std::move(fooVector);
			Assert::IsTrue(fooVector3[0] == f1);
			Assert::IsTrue(fooVector3[1] == f2);
			auto expression6 = [&] {fooVector.Front(); };
			Assert::ExpectException<std::exception>(expression6);

		}

	private:
		static Vector<int> intVector;
		static Vector<int*> intPtrVector;
		static Vector<RFoo> fooVector;
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif

	};
	Vector<int> VectorTest::intVector;
	Vector<int*> VectorTest::intPtrVector;
	Vector<RFoo> VectorTest::fooVector;
#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState VectorTest::sStartMemState;
#endif

}