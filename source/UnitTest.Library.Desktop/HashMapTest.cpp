#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	class FooFunctor
	{
	public:
		int operator()(RFoo f, std::uint32_t size)
		{
			int key = f.Data() + *f.MoreData();
			unsigned char* bytes = reinterpret_cast<unsigned char*>(&key);
			int sum = 0;
			for (std::uint32_t i = 0; i < sizeof(key); ++i)
			{
				sum += bytes[i];
			}
			return sum % size;
		}
	};

	TEST_CLASS(HashMapTest)
	{
	public:

#if defined(DEBUG) | defined(_DEBUG)
		TEST_METHOD_INITIALIZE(Initialize)
		{
			stringKeyMap.Clear();
			charPtrKeyMap.Clear();
			intPtrKeyMap.Clear();
			intKeyMap.Clear();
			fooKeyMap.Clear();

			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			stringKeyMap.Clear();
			charPtrKeyMap.Clear();
			intPtrKeyMap.Clear();
			intKeyMap.Clear();
			fooKeyMap.Clear();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif
		TEST_METHOD(HashMapTestFind)
		{
			int a = 1, b = 2, c = 3, d = 4;
			RFoo f1(a), f2(b), f3(c), f4(d);

			//string version
			stringKeyMap.Insert(std::pair<std::string, int>("hello", 5));
			stringKeyMap.Insert(std::pair<std::string, int>("goodbye", 7));
			stringKeyMap.Insert(std::pair<std::string, int>("eyyyyyyyyyy", 3));

			HashMap<std::string, int, DefaultFunctor<std::string>>::Iterator it = stringKeyMap.Find("hello");
			Assert::AreEqual((*it).second, 5);

			HashMap<std::string, int, DefaultFunctor<std::string>>::Iterator it2 = stringKeyMap.Find("goodbye");
			Assert::AreEqual((*it2).second, 7);

			HashMap<std::string, int, DefaultFunctor<std::string>>::Iterator it3 = stringKeyMap.Find("eyyyyyyyyyy");
			Assert::AreEqual((*it3).second, 3);

			HashMap<std::string, int, DefaultFunctor<std::string>>::Iterator it4 = stringKeyMap.Find("yo");
			Assert::IsTrue(it4 == stringKeyMap.end());

			//char* version

			//special case
			char* str1 = "Nope";
			charPtrKeyMap.Insert(std::pair<char*, int>(str1, 22));
			charPtrKeyMap.Insert(std::pair<char*, int>("hello", 5));
			charPtrKeyMap.Insert(std::pair<char*, int>("goodbye", 7));
			charPtrKeyMap.Insert(std::pair<char*, int>("eyyyyyyyyyy", 3));

			HashMap<char*, int, DefaultFunctor<char*>>::Iterator itt = charPtrKeyMap.Find(str1);
			Assert::AreEqual((*itt).second, 22);

			HashMap<char*, int, DefaultFunctor<char*>>::Iterator it13 = charPtrKeyMap.Find("hello");
			Assert::AreEqual((*it13).second, 5);

			HashMap<char*, int, DefaultFunctor<char*>>::Iterator it14 = charPtrKeyMap.Find("goodbye");
			Assert::AreEqual((*it14).second, 7);

			HashMap<char*, int, DefaultFunctor<char*>>::Iterator it15 = charPtrKeyMap.Find("eyyyyyyyyyy");
			Assert::AreEqual((*it15).second, 3);

			HashMap<char*, int, DefaultFunctor<char*>>::Iterator it16 = charPtrKeyMap.Find("yo");
			Assert::IsTrue(it16 == charPtrKeyMap.end());

			//int version
			intKeyMap.Insert(std::pair<int, int>(a, 5));
			intKeyMap.Insert(std::pair<int, int>(b, 7));
			intKeyMap.Insert(std::pair<int, int>(c, 3));

			HashMap<int, int, DefaultFunctor<int>>::Iterator it5 = intKeyMap.Find(a);
			Assert::AreEqual((*it5).second, 5);

			HashMap<int, int, DefaultFunctor<int>>::Iterator it6 = intKeyMap.Find(b);
			Assert::AreEqual((*it6).second, 7);

			HashMap<int, int, DefaultFunctor<int>>::Iterator it7 = intKeyMap.Find(c);
			Assert::AreEqual((*it7).second, 3);

			HashMap<int, int, DefaultFunctor<int>>::Iterator it8 = intKeyMap.Find(10);
			Assert::IsTrue(it8 == intKeyMap.end());

			//int* version
			intPtrKeyMap.Insert(std::pair<int*, int>(&a, 5));
			intPtrKeyMap.Insert(std::pair<int*, int>(&b, 7));
			intPtrKeyMap.Insert(std::pair<int*, int>(&c, 3));

			HashMap<int*, int, DefaultFunctor<int*>>::Iterator it9 = intPtrKeyMap.Find(&a);
			Assert::AreEqual((*it9).second, 5);

			HashMap<int*, int, DefaultFunctor<int*>>::Iterator it10 = intPtrKeyMap.Find(&b);
			Assert::AreEqual((*it10).second, 7);

			HashMap<int*, int, DefaultFunctor<int*>>::Iterator it11 = intPtrKeyMap.Find(&c);
			Assert::AreEqual((*it11).second, 3);

			HashMap<int*, int, DefaultFunctor<int*>>::Iterator it12 = intPtrKeyMap.Find(&d);
			Assert::IsTrue(it12 == intPtrKeyMap.end());

			//foo test
			fooKeyMap.Insert(std::pair<RFoo, int>(f1, 5));
			fooKeyMap.Insert(std::pair<RFoo, int>(f2, 7));
			fooKeyMap.Insert(std::pair<RFoo, int>(f3, 3));

			HashMap<RFoo, int, FooFunctor>::Iterator it17 = fooKeyMap.Find(f1);
			Assert::AreEqual((*it17).second, 5);

			HashMap<RFoo, int, FooFunctor>::Iterator it18 = fooKeyMap.Find(f2);
			Assert::AreEqual((*it18).second, 7);

			HashMap<RFoo, int, FooFunctor>::Iterator it19 = fooKeyMap.Find(f3);
			Assert::AreEqual((*it19).second, 3);

			HashMap<RFoo, int, FooFunctor>::Iterator it20 = fooKeyMap.Find(f4);
			Assert::IsTrue(it20 == fooKeyMap.end());
		}

		TEST_METHOD(HashMapTestInsert)
		{
			int a = 1;
			RFoo f1(a);

			//string test
			HashMap<std::string, int, DefaultFunctor<std::string>>::Iterator it = stringKeyMap.Insert(std::pair<std::string, int>("hello", 5));
			Assert::AreEqual((*it).second, 5);
			HashMap<std::string, int, DefaultFunctor<std::string>>::Iterator it2 = stringKeyMap.Insert(std::pair<std::string, int>("hello", 1000));
			Assert::AreEqual((*it2).second, 5);

			//char* test
			HashMap<char*, int, DefaultFunctor<char*>>::Iterator it3 = charPtrKeyMap.Insert(std::pair<char*, int>("hello", 5));
			Assert::AreEqual((*it3).second, 5);
			HashMap<char*, int, DefaultFunctor<char*>>::Iterator it4 = charPtrKeyMap.Insert(std::pair<char*, int>("hello", 1000));
			Assert::AreEqual((*it4).second, 5);

			//int* test
			HashMap<int*, int, DefaultFunctor<int*>>::Iterator it5 = intPtrKeyMap.Insert(std::pair<int*, int>(&a, 5));
			Assert::AreEqual((*it5).second, 5);
			HashMap<int*, int, DefaultFunctor<int*>>::Iterator it6 = intPtrKeyMap.Insert(std::pair<int*, int>(&a, 1000));
			Assert::AreEqual((*it6).second, 5);

			//int test
			HashMap<int, int, DefaultFunctor<int>>::Iterator it7 = intKeyMap.Insert(std::pair<int, int>(a, 5));
			Assert::AreEqual((*it7).second, 5);
			HashMap<int, int, DefaultFunctor<int>>::Iterator it8 = intKeyMap.Insert(std::pair<int, int>(a, 1000));
			Assert::AreEqual((*it8).second, 5);

			//foo test
			HashMap<RFoo, int, FooFunctor>::Iterator it9 = fooKeyMap.Insert(std::pair<RFoo, int>(f1, 5));
			Assert::AreEqual((*it9).second, 5);
			HashMap<RFoo, int, FooFunctor>::Iterator it10 = fooKeyMap.Insert(std::pair<RFoo, int>(f1, 1000));
			Assert::AreEqual((*it10).second, 5);
		}

		TEST_METHOD(HashMapTestIndexOperator)
		{
			int a = 1;
			RFoo f1(a);

			//string test
			stringKeyMap["hello"] = 5;
			Assert::AreEqual(stringKeyMap["hello"], 5);

			//char* test
			charPtrKeyMap["hello"] = 5;
			Assert::AreEqual(charPtrKeyMap["hello"], 5);

			//int test
			intKeyMap[a] = 5;
			Assert::AreEqual(intKeyMap[a], 5);

			//int* test
			intPtrKeyMap[&a] = 5;
			Assert::AreEqual(intPtrKeyMap[&a], 5);

			//Foo test
			fooKeyMap[f1] = 5;
			Assert::AreEqual(fooKeyMap[f1], 5);
		}

		TEST_METHOD(HashMapTestRemove)
		{
			int a = 1;
			RFoo f1(a);

			//string version
			stringKeyMap["hello"] = 5;
			stringKeyMap.Remove("hello");
			Assert::IsTrue(stringKeyMap.Find("hello") == stringKeyMap.end());

			//char* version
			charPtrKeyMap["hello"] = 5;
			charPtrKeyMap.Remove("hello");
			Assert::IsTrue(charPtrKeyMap.Find("hello") == charPtrKeyMap.end());

			//int version
			intKeyMap[a] = 5;
			intKeyMap.Remove(a);
			Assert::IsTrue(intKeyMap.Find(a) == intKeyMap.end());

			//int version
			intPtrKeyMap[&a] = 5;
			intPtrKeyMap.Remove(&a);
			Assert::IsTrue(intPtrKeyMap.Find(&a) == intPtrKeyMap.end());

			//foo version
			fooKeyMap[f1] = 5;
			fooKeyMap.Remove(f1);
			Assert::IsTrue(fooKeyMap.Find(f1) == fooKeyMap.end());

		}

		TEST_METHOD(HashMapTestClear)
		{
			int a = 1, b = 2, c = 3;
			RFoo f1(a), f2(b), f3(c);
			//string version
			stringKeyMap["hello"] = 5;
			stringKeyMap["goodbye"] = 7;
			stringKeyMap["eyyyyyyyyy"] = 10;

			stringKeyMap.Clear();
			Assert::IsTrue(stringKeyMap.Find("hello") == stringKeyMap.end());
			Assert::IsTrue(stringKeyMap.Find("goodbye") == stringKeyMap.end());
			Assert::IsTrue(stringKeyMap.Find("eyyyyyyyyy") == stringKeyMap.end());
			Assert::AreEqual(stringKeyMap.Size(), 0U);

			//char* version
			charPtrKeyMap["hello"] = 5;
			charPtrKeyMap["goodbye"] = 7;
			charPtrKeyMap["eyyyyyyyyy"] = 10;

			charPtrKeyMap.Clear();
			Assert::IsTrue(charPtrKeyMap.Find("hello") == charPtrKeyMap.end());
			Assert::IsTrue(charPtrKeyMap.Find("goodbye") == charPtrKeyMap.end());
			Assert::IsTrue(charPtrKeyMap.Find("eyyyyyyyyy") == charPtrKeyMap.end());
			Assert::AreEqual(charPtrKeyMap.Size(), 0U);

			//int version
			intKeyMap[a] = 5;
			intKeyMap[b] = 7;
			intKeyMap[c] = 10;

			intKeyMap.Clear();
			Assert::IsTrue(intKeyMap.Find(a) == intKeyMap.end());
			Assert::IsTrue(intKeyMap.Find(b) == intKeyMap.end());
			Assert::IsTrue(intKeyMap.Find(c) == intKeyMap.end());
			Assert::AreEqual(intKeyMap.Size(), 0U);

			//int* version
			intPtrKeyMap[&a] = 5;
			intPtrKeyMap[&b] = 7;
			intPtrKeyMap[&c] = 10;

			intPtrKeyMap.Clear();
			Assert::IsTrue(intPtrKeyMap.Find(&a) == intPtrKeyMap.end());
			Assert::IsTrue(intPtrKeyMap.Find(&b) == intPtrKeyMap.end());
			Assert::IsTrue(intPtrKeyMap.Find(&c) == intPtrKeyMap.end());
			Assert::AreEqual(intPtrKeyMap.Size(), 0U);

			//foo version
			fooKeyMap[f1] = 5;
			fooKeyMap[f2] = 7;
			fooKeyMap[f3] = 10;

			fooKeyMap.Clear();
			Assert::IsTrue(fooKeyMap.Find(f1) == fooKeyMap.end());
			Assert::IsTrue(fooKeyMap.Find(f2) == fooKeyMap.end());
			Assert::IsTrue(fooKeyMap.Find(f3) == fooKeyMap.end());
			Assert::AreEqual(fooKeyMap.Size(), 0U);
		}

		TEST_METHOD(HashMapTestSize)
		{
			int a = 1, b = 2, c = 3;
			RFoo f1(a), f2(b), f3(c);

			//string version
			Assert::AreEqual(stringKeyMap.Size(), 0U);
			stringKeyMap["hello"] = 5;
			stringKeyMap["goodbye"] = 7;
			stringKeyMap["eyyyyyyyyy"] = 10;
			Assert::AreEqual(stringKeyMap.Size(), 3U);
			stringKeyMap.Remove("goodbye");
			Assert::AreEqual(stringKeyMap.Size(), 2U);

			//char* version
			Assert::AreEqual(charPtrKeyMap.Size(), 0U);
			charPtrKeyMap["hello"] = 5;
			charPtrKeyMap["goodbye"] = 7;
			charPtrKeyMap["eyyyyyyyyy"] = 10;
			Assert::AreEqual(charPtrKeyMap.Size(), 3U);
			charPtrKeyMap.Remove("goodbye");
			Assert::AreEqual(charPtrKeyMap.Size(), 2U);

			//int version
			Assert::AreEqual(intKeyMap.Size(), 0U);
			intKeyMap[a] = 5;
			intKeyMap[b] = 7;
			intKeyMap[c] = 10;
			Assert::AreEqual(intKeyMap.Size(), 3U);
			intKeyMap.Remove(b);
			Assert::AreEqual(intKeyMap.Size(), 2U);

			//int* version
			Assert::AreEqual(intPtrKeyMap.Size(), 0U);
			intPtrKeyMap[&a] = 5;
			intPtrKeyMap[&b] = 7;
			intPtrKeyMap[&c] = 10;
			Assert::AreEqual(intPtrKeyMap.Size(), 3U);
			intPtrKeyMap.Remove(&b);
			Assert::AreEqual(intPtrKeyMap.Size(), 2U);

			//foo version
			Assert::AreEqual(fooKeyMap.Size(), 0U);
			fooKeyMap[f1] = 5;
			fooKeyMap[f2] = 7;
			fooKeyMap[f3] = 10;
			Assert::AreEqual(fooKeyMap.Size(), 3U);
			fooKeyMap.Remove(f2);
			Assert::AreEqual(fooKeyMap.Size(), 2U);
		}

		TEST_METHOD(HashMapTestContainsKey)
		{
			int a = 1;
			RFoo f1(a);

			//string version
			Assert::IsFalse(stringKeyMap.ContainsKey("hello"));
			stringKeyMap["hello"] = 5;
			Assert::IsTrue(stringKeyMap.ContainsKey("hello"));

			//char* version
			Assert::IsFalse(charPtrKeyMap.ContainsKey("hello"));
			charPtrKeyMap["hello"] = 5;
			Assert::IsTrue(charPtrKeyMap.ContainsKey("hello"));

			//int version
			Assert::IsFalse(intKeyMap.ContainsKey(a));
			intKeyMap[a] = 5;
			Assert::IsTrue(intKeyMap.ContainsKey(a));

			//int* version
			Assert::IsFalse(intPtrKeyMap.ContainsKey(&a));
			intPtrKeyMap[&a] = 5;
			Assert::IsTrue(intPtrKeyMap.ContainsKey(&a));

			//foo version
			Assert::IsFalse(fooKeyMap.ContainsKey(f1));
			fooKeyMap[f1] = 5;
			Assert::IsTrue(fooKeyMap.ContainsKey(f1));
		}

		TEST_METHOD(HashMapTestBegin)
		{
			int a = 1;
			RFoo f1(a);

			//string test
			Assert::IsTrue(stringKeyMap.begin() == stringKeyMap.end());
			stringKeyMap["hello"] = 5;
			Assert::AreEqual((*stringKeyMap.begin()).second, 5);

			//char* test
			Assert::IsTrue(charPtrKeyMap.begin() == charPtrKeyMap.end());
			charPtrKeyMap["hello"] = 5;
			Assert::AreEqual((*charPtrKeyMap.begin()).second, 5);

			//int test
			Assert::IsTrue(intKeyMap.begin() == intKeyMap.end());
			intKeyMap[a] = 5;
			Assert::AreEqual((*intKeyMap.begin()).second, 5);

			//int* test
			Assert::IsTrue(intPtrKeyMap.begin() == intPtrKeyMap.end());
			intPtrKeyMap[&a] = 5;
			Assert::AreEqual((*intPtrKeyMap.begin()).second, 5);

			//foo test
			Assert::IsTrue(fooKeyMap.begin() == fooKeyMap.end());
			fooKeyMap[f1] = 5;
			Assert::AreEqual((*fooKeyMap.begin()).second, 5);
		}

		TEST_METHOD(HashMapTestIteratorIncrement)
		{
			int a = 5;
			RFoo f1(a);

			//string version
			stringKeyMap["hello"] = 5;
			HashMap<std::string, int, DefaultFunctor<std::string>>::Iterator it = stringKeyMap.begin();
			Assert::IsTrue((*it).second == 5);
			it++;
			Assert::IsTrue(it == stringKeyMap.end());

			HashMap<std::string, int, DefaultFunctor<std::string>>::Iterator it2;
			auto expression = [&] {it2++; };
			Assert::ExpectException<std::exception>(expression);

			HashMap<std::string, int, DefaultFunctor<std::string>>::Iterator it3 = stringKeyMap.end();
			auto expression2 = [&] {it3++; };
			Assert::ExpectException<std::exception>(expression2);

			//char* version
			charPtrKeyMap["hello"] = 5;
			HashMap<char*, int, DefaultFunctor<char*>>::Iterator it4 = charPtrKeyMap.begin();
			Assert::IsTrue((*it4).second == 5);
			it4++;
			Assert::IsTrue(it4 == charPtrKeyMap.end());

			HashMap<char*, int, DefaultFunctor<char*>>::Iterator it5;
			auto expression3 = [&] {it5++; };
			Assert::ExpectException<std::exception>(expression3);

			HashMap<char*, int, DefaultFunctor<char*>>::Iterator it6 = charPtrKeyMap.end();
			auto expression4 = [&] {it6++; };
			Assert::ExpectException<std::exception>(expression4);

			//int value 
			intKeyMap[a] = 5;
			HashMap<int, int, DefaultFunctor<int>>::Iterator it7 = intKeyMap.begin();
			Assert::IsTrue((*it7).second == 5);
			it7++;
			Assert::IsTrue(it7 == intKeyMap.end());

			HashMap<int, int, DefaultFunctor<int>>::Iterator it8;
			auto expression5 = [&] {it8++; };
			Assert::ExpectException<std::exception>(expression5);

			HashMap<int, int, DefaultFunctor<int>>::Iterator it9 = intKeyMap.end();
			auto expression6 = [&] {it9++; };
			Assert::ExpectException<std::exception>(expression6);

			//int* value 
			intPtrKeyMap[&a] = 5;
			HashMap<int*, int, DefaultFunctor<int*>>::Iterator it10 = intPtrKeyMap.begin();
			Assert::IsTrue((*it10).second == 5);
			it10++;
			Assert::IsTrue(it10 == intPtrKeyMap.end());

			HashMap<int*, int, DefaultFunctor<int*>>::Iterator it11;
			auto expression7 = [&] {it11++; };
			Assert::ExpectException<std::exception>(expression7);

			HashMap<int*, int, DefaultFunctor<int*>>::Iterator it12 = intPtrKeyMap.end();
			auto expression8 = [&] {it12++; };
			Assert::ExpectException<std::exception>(expression8);

			//foo value 
			fooKeyMap[f1] = 5;
			HashMap<RFoo, int, FooFunctor>::Iterator it13 = fooKeyMap.begin();
			Assert::IsTrue((*it13).second == 5);
			it13++;
			Assert::IsTrue(it13 == fooKeyMap.end());

			HashMap<RFoo, int, FooFunctor>::Iterator it14;
			auto expression9 = [&] {it14++; };
			Assert::ExpectException<std::exception>(expression9);

			HashMap<RFoo, int, FooFunctor>::Iterator it15 = fooKeyMap.end();
			auto expression10 = [&] {it15++; };
			Assert::ExpectException<std::exception>(expression10);

		}

		TEST_METHOD(HashMapTestDereference)
		{
			int a = 5;
			RFoo f1(a);

			//string version
			stringKeyMap["hello"] = 5;
			HashMap<std::string, int, DefaultFunctor<std::string>>::Iterator it = stringKeyMap.begin();
			Assert::IsTrue(it->second == 5);

			HashMap<std::string, int, DefaultFunctor<std::string>>::Iterator it2;
			auto expression = [&] {it2->first; };
			Assert::ExpectException<std::exception>(expression);

			HashMap<std::string, int, DefaultFunctor<std::string>>::Iterator it3 = stringKeyMap.end();
			auto expression2 = [&] {it3->first; };
			Assert::ExpectException<std::exception>(expression2);

			//char* version
			charPtrKeyMap["hello"] = 5;
			HashMap<char*, int, DefaultFunctor<char*>>::Iterator it4 = charPtrKeyMap.begin();
			Assert::IsTrue(it4->second == 5);

			HashMap<char*, int, DefaultFunctor<char*>>::Iterator it5;
			auto expression3 = [&] {it5->first; };
			Assert::ExpectException<std::exception>(expression3);

			HashMap<char*, int, DefaultFunctor<char*>>::Iterator it6 = charPtrKeyMap.end();
			auto expression4 = [&] {it6->first; };
			Assert::ExpectException<std::exception>(expression4);

			//int value
			intKeyMap[a] = 5;
			HashMap<int, int, DefaultFunctor<int>>::Iterator it7 = intKeyMap.begin();
			Assert::IsTrue(it7->second == 5);

			HashMap<int, int, DefaultFunctor<int>>::Iterator it8;
			auto expression5 = [&] {it8->first; };
			Assert::ExpectException<std::exception>(expression5);

			HashMap<int, int, DefaultFunctor<int>>::Iterator it9 = intKeyMap.end();
			auto expression6 = [&] {it9->first; };
			Assert::ExpectException<std::exception>(expression6);

			//int* value 
			intPtrKeyMap[&a] = 5;
			HashMap<int*, int, DefaultFunctor<int*>>::Iterator it10 = intPtrKeyMap.begin();
			Assert::IsTrue(it10->second == 5);

			HashMap<int*, int, DefaultFunctor<int*>>::Iterator it11;
			auto expression7 = [&] {it11->first; };
			Assert::ExpectException<std::exception>(expression7);

			HashMap<int*, int, DefaultFunctor<int*>>::Iterator it12 = intPtrKeyMap.end();
			auto expression8 = [&] {it12->first; };
			Assert::ExpectException<std::exception>(expression8);

			//foo value 
			fooKeyMap[f1] = 5;
			HashMap<RFoo, int, FooFunctor>::Iterator it13 = fooKeyMap.begin();
			Assert::IsTrue(it13->second == 5);

			HashMap<RFoo, int, FooFunctor>::Iterator it14;
			auto expression9 = [&] {it14->first; };
			Assert::ExpectException<std::exception>(expression9);

			HashMap<RFoo, int, FooFunctor>::Iterator it15 = fooKeyMap.end();
			auto expression10 = [&] {it15->first; };
			Assert::ExpectException<std::exception>(expression10);
		}

		TEST_METHOD(HashMapTestRehash)
		{
			int a = 1, b = 2, c = 3, d = 4, e = 5;
			RFoo f1(a), f2(b), f3(c), f4(d), f5(e);

			//int version
			HashMap<int, int, DefaultFunctor<int>> intKeyMap2(10);
			intKeyMap2[a] = 5;
			intKeyMap2[b] = 1000;
			intKeyMap2[c] = 5;
			intKeyMap2[d] = 5;
			intKeyMap2[e] = 5;
			float lf = intKeyMap2.LoadFactor();
			intKeyMap2.Rehash(20);
			float lf2 = intKeyMap2.LoadFactor();
			Assert::AreNotEqual(lf, lf2);
			Assert::AreEqual((*intKeyMap2.Find(b)).second, 1000);

			//int* version
			HashMap<int*, int, DefaultFunctor<int*>> intPtrKeyMap2(10);
			intPtrKeyMap2[&a] = 5;
			intPtrKeyMap2[&b] = 1000;
			intPtrKeyMap2[&c] = 5;
			intPtrKeyMap2[&d] = 5;
			intPtrKeyMap2[&e] = 5;
			float lf3 = intPtrKeyMap2.LoadFactor();
			intPtrKeyMap2.Rehash(20);
			float lf4 = intPtrKeyMap2.LoadFactor();
			Assert::AreNotEqual(lf3, lf4);
			Assert::AreEqual((*intPtrKeyMap2.Find(&b)).second, 1000);

			//char* version
			HashMap<char*, int, DefaultFunctor<char*>> charPtrKeyMap2(10);
			charPtrKeyMap2["hello"] = 5;
			charPtrKeyMap2["goodbye"] = 1000;
			charPtrKeyMap2["a"] = 5;
			charPtrKeyMap2["whateverrr"] = 5;
			charPtrKeyMap2["eyyyyy"] = 5;
			float lf5 = charPtrKeyMap2.LoadFactor();
			charPtrKeyMap2.Rehash(20);
			float lf6 = charPtrKeyMap2.LoadFactor();
			Assert::AreNotEqual(lf5, lf6);
			Assert::AreEqual((*charPtrKeyMap2.Find("goodbye")).second, 1000);

			//string version
			HashMap<std::string, int, DefaultFunctor<std::string>> stringKeyMap2(10);
			stringKeyMap2["hello"] = 5;
			stringKeyMap2["goodbye"] = 1000;
			stringKeyMap2["a"] = 5;
			stringKeyMap2["whateverrr"] = 5;
			stringKeyMap2["eyyyyy"] = 5;
			float lf7 = stringKeyMap2.LoadFactor();
			stringKeyMap2.Rehash(20);
			float lf8 = stringKeyMap2.LoadFactor();
			Assert::AreNotEqual(lf7, lf8);
			Assert::AreEqual((*stringKeyMap2.Find("goodbye")).second, 1000);

			//Foo version
			HashMap<RFoo, int, FooFunctor> fooKeyMap2(10);
			fooKeyMap2[f1] = 5;
			fooKeyMap2[f2] = 1000;
			fooKeyMap2[f3] = 5;
			fooKeyMap2[f4] = 5;
			fooKeyMap2[f5] = 5;
			float lf9 = fooKeyMap2.LoadFactor();
			fooKeyMap2.Rehash(20);
			float lf10 = fooKeyMap2.LoadFactor();
			Assert::AreNotEqual(lf9, lf10);
			Assert::AreEqual((*fooKeyMap2.Find(f2)).second, 1000);
		}

	private:
		static HashMap<std::string, int, DefaultFunctor<std::string>> stringKeyMap;
		static HashMap<char*, int, DefaultFunctor<char*>> charPtrKeyMap;
		static HashMap<int, int, DefaultFunctor<int>> intKeyMap;
		static HashMap<int*, int, DefaultFunctor<int*>> intPtrKeyMap;
		static HashMap<RFoo, int, FooFunctor> fooKeyMap;
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};
	HashMap<std::string, int, DefaultFunctor<std::string>> HashMapTest::stringKeyMap(10);
	HashMap<char*, int, DefaultFunctor<char*>> HashMapTest::charPtrKeyMap(10);
	HashMap<int, int, DefaultFunctor<int>> HashMapTest::intKeyMap(10);
	HashMap<int*, int, DefaultFunctor<int*>> HashMapTest::intPtrKeyMap(10);
	HashMap<RFoo, int, FooFunctor> HashMapTest::fooKeyMap(10);
#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState HashMapTest::sStartMemState;
#endif
}