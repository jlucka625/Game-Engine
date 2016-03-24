#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DatumTest)
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
		TEST_METHOD(DatumTestType)
		{
			Datum intDatum;
			Assert::IsTrue(intDatum.Type() == Datum::DatumType::Unknown);
			intDatum.SetType(Datum::DatumType::Integer);
			Assert::IsTrue(intDatum.Type() == Datum::DatumType::Integer);
			Assert::ExpectException<std::exception>([&intDatum]
			{
				intDatum.SetType(Datum::DatumType::String);
			});
			Assert::ExpectException<std::exception>([&intDatum]
			{
				intDatum.SetType(Datum::DatumType::Unknown);
			});
			Assert::ExpectException<std::exception>([&intDatum]
			{
				intDatum.SetType(Datum::DatumType::End);
			});
		}

		//Tests Reserve, PushBack, PopBack, Get, Set for Integers
		TEST_METHOD(DatumTestInteger)
		{
			Datum intDatum;
			Assert::ExpectException<std::exception>([&intDatum]
			{
				intDatum.PopBack();
			});

			Assert::ExpectException<std::exception>([&intDatum]
			{
				intDatum.Get<int>();
			});
			intDatum.PushBack(0);
			intDatum.PushBack(1);
			intDatum.PushBack(2);
			Assert::AreEqual(intDatum.Get<int>(0), 0);
			Assert::AreEqual(intDatum.Get<int>(1), 1);
			Assert::AreEqual(intDatum.Get<int>(2), 2);

			Assert::ExpectException<std::exception>([&intDatum]
			{
				intDatum.PushBack("hello");
			});

			intDatum.PopBack();
			Assert::ExpectException<std::exception>([&intDatum]
			{
				intDatum.Get<int>(2);
			});

			intDatum.PushBack(10);
			Assert::AreEqual(intDatum.Get<int>(2), 10);

			intDatum.Reserve(20);
			Assert::ExpectException<std::exception>([&intDatum]
			{
				intDatum.Get<int>(15);
			});
			Assert::ExpectException<std::exception>([&intDatum]
			{
				intDatum.Set(15, 15);
			});

			intDatum.Set(100, 0);
			Assert::AreEqual(intDatum.Get<int>(0), 100);

			intDatum.PopBack();
			intDatum.PopBack();
			intDatum.PopBack();
		}

		//Tests Reserve, PushBack, PopBack, Get, Set for Floats
		TEST_METHOD(DatumTestFloat)
		{
			Datum floatDatum;
			Assert::ExpectException<std::exception>([&floatDatum]
			{
				floatDatum.PopBack();
			});

			Assert::ExpectException<std::exception>([&floatDatum]
			{
				floatDatum.Get<float>();
			});
			floatDatum.PushBack(5.6f);
			floatDatum.PushBack(2.644f);
			floatDatum.PushBack(3.1f);
			Assert::AreEqual(floatDatum.Get<float>(0), 5.6f);
			Assert::AreEqual(floatDatum.Get<float>(1), 2.644f);
			Assert::AreEqual(floatDatum.Get<float>(2), 3.1f);

			Assert::ExpectException<std::exception>([&floatDatum]
			{
				floatDatum.PushBack("hello");
			});

			floatDatum.PopBack();
			Assert::ExpectException<std::exception>([&floatDatum]
			{
				floatDatum.Get<float>(2);
			});

			floatDatum.PushBack(10.5f);
			Assert::AreEqual(floatDatum.Get<float>(2), 10.5f);

			floatDatum.Reserve(20);
			Assert::ExpectException<std::exception>([&floatDatum]
			{
				floatDatum.Get<float>(15);
			});
			Assert::ExpectException<std::exception>([&floatDatum]
			{
				floatDatum.Set(15.6f, 15);
			});

			floatDatum.Set(100.0f, 0);
			Assert::AreEqual(floatDatum.Get<float>(0), 100.0f);

			floatDatum.PopBack();
			floatDatum.PopBack();
			floatDatum.PopBack();

		}

		//Tests Reserve, PushBack, PopBack, Get, Set for Strings
		TEST_METHOD(DatumTestString)
		{
			Datum stringDatum;
			Assert::ExpectException<std::exception>([&stringDatum]
			{
				stringDatum.PopBack();
			});

			Assert::ExpectException<std::exception>([&stringDatum]
			{
				stringDatum.Get<std::string>();
			});
			stringDatum.PushBack("hello");
			stringDatum.PushBack("goodbye");
			stringDatum.PushBack("wassup");
			Assert::IsTrue(stringDatum.Get<std::string>(0) == "hello");
			Assert::IsTrue(stringDatum.Get<std::string>(1) == "goodbye");
			Assert::IsTrue(stringDatum.Get<std::string>(2) == "wassup");

			Assert::ExpectException<std::exception>([&stringDatum]
			{
				stringDatum.PushBack(10);
			});

			stringDatum.PopBack();
			Assert::ExpectException<std::exception>([&stringDatum]
			{
				stringDatum.Get<std::string>(2);
			});

			stringDatum.PushBack("No");
			Assert::IsTrue(stringDatum.Get<std::string>(2) == "No");

			stringDatum.Reserve(20);
			Assert::ExpectException<std::exception>([&stringDatum]
			{
				stringDatum.Get<std::string>(15);
			});
			Assert::ExpectException<std::exception>([&stringDatum]
			{
				stringDatum.Set("whatevs", 15);
			});

			stringDatum.Set("yo", 0);
			Assert::IsTrue(stringDatum.Get<std::string>(0) == "yo");

			stringDatum.PopBack();
			stringDatum.PopBack();
			stringDatum.PopBack();

		}

		//Tests Reserve, PushBack, PopBack, Get, Set for Vector
		TEST_METHOD(DatumTestVector)
		{
			glm::vec4 vect1 = glm::vec4(1.0f);
			glm::vec4 vect2 = glm::vec4(2.0f);
			glm::vec4 vect3 = glm::vec4(3.0f);
			glm::vec4 vect4 = glm::vec4(4.0f);
			glm::vec4 vect5 = glm::vec4(5.0f);

			Datum vec4Datum;
			Assert::ExpectException<std::exception>([&vec4Datum]
			{
				vec4Datum.PopBack();
			});

			Assert::ExpectException<std::exception>([&vec4Datum]
			{
				vec4Datum.Get<glm::vec4>();
			});
			vec4Datum.PushBack(vect1);
			vec4Datum.PushBack(vect2);
			vec4Datum.PushBack(vect3);
			Assert::IsTrue(vec4Datum.Get<glm::vec4>(0) == vect1);
			Assert::IsTrue(vec4Datum.Get<glm::vec4>(1) == vect2);
			Assert::IsTrue(vec4Datum.Get<glm::vec4>(2) == vect3);

			Assert::ExpectException<std::exception>([&vec4Datum]
			{
				vec4Datum.PushBack("yo");
			});

			vec4Datum.PopBack();
			Assert::ExpectException<std::exception>([&vec4Datum]
			{
				vec4Datum.Get<glm::vec4>(2);
			});

			vec4Datum.PushBack(vect4);
			Assert::IsTrue(vec4Datum.Get<glm::vec4>(2) == vect4);

			vec4Datum.Reserve(20);
			Assert::ExpectException<std::exception>([&vec4Datum]
			{
				vec4Datum.Get<glm::vec4>(15);
			});
			Assert::ExpectException<std::exception>([&vec4Datum]
			{
				vec4Datum.Set(glm::vec4(), 15);
			});

			vec4Datum.Set(vect5, 0);
			Assert::IsTrue(vec4Datum.Get<glm::vec4>(0) == vect5);

			vec4Datum.PopBack();
			vec4Datum.PopBack();
			vec4Datum.PopBack();

		}

		//Tests Reserve, PushBack, PopBack, Get, Set for Matrix
		TEST_METHOD(DatumTestMatrix)
		{
			glm::mat4 matr1 = glm::mat4(1.0f);
			glm::mat4 matr2 = glm::mat4(2.0f);
			glm::mat4 matr3 = glm::mat4(3.0f);
			glm::mat4 matr4 = glm::mat4(4.0f);
			glm::mat4 matr5 = glm::mat4(5.0f);

			Datum mat4Datum;
			Assert::ExpectException<std::exception>([&mat4Datum]
			{
				mat4Datum.PopBack();
			});

			Assert::ExpectException<std::exception>([&mat4Datum]
			{
				mat4Datum.Get<glm::mat4>();
			});
			mat4Datum.PushBack(matr1);
			mat4Datum.PushBack(matr2);
			mat4Datum.PushBack(matr3);
			Assert::IsTrue(mat4Datum.Get<glm::mat4>(0) == matr1);
			Assert::IsTrue(mat4Datum.Get<glm::mat4>(1) == matr2);
			Assert::IsTrue(mat4Datum.Get<glm::mat4>(2) == matr3);

			Assert::ExpectException<std::exception>([&mat4Datum]
			{
				mat4Datum.PushBack("yo");
			});

			mat4Datum.PopBack();
			Assert::ExpectException<std::exception>([&mat4Datum]
			{
				mat4Datum.Get<glm::mat4>(2);
			});

			mat4Datum.PushBack(matr4);
			Assert::IsTrue(mat4Datum.Get<glm::mat4>(2) == matr4);

			mat4Datum.Reserve(20);
			Assert::ExpectException<std::exception>([&mat4Datum]
			{
				mat4Datum.Get<glm::mat4>(15);
			});
			Assert::ExpectException<std::exception>([&mat4Datum]
			{
				mat4Datum.Set(glm::mat4(), 15);
			});

			mat4Datum.Set(matr5, 0);
			Assert::IsTrue(mat4Datum.Get<glm::mat4>(0) == matr5);

			mat4Datum.PopBack();
			mat4Datum.PopBack();
			mat4Datum.PopBack();

		}

		//Tests Reserve, PushBack, PopBack, Get, Set for Pointer
		TEST_METHOD(DatumTestPointer)
		{
			RFoo f1(10), f2(20), f3(30), f4(40), f5(50), f6(60);
			Datum RTTIDatum;
			Assert::ExpectException<std::exception>([&RTTIDatum]
			{
				RTTIDatum.PopBack();
			});

			Assert::ExpectException<std::exception>([&RTTIDatum]
			{
				RTTIDatum.Get<RTTI*>();
			});
			RTTIDatum.PushBack(reinterpret_cast<RTTI*>(&f1));
			RTTIDatum.PushBack(reinterpret_cast<RTTI*>(&f2));
			RTTIDatum.PushBack(reinterpret_cast<RTTI*>(&f3));
			Assert::IsTrue(RTTIDatum.Get<RTTI*>(0) == reinterpret_cast<RTTI*>(&f1));
			Assert::IsTrue(RTTIDatum.Get<RTTI*>(1) == reinterpret_cast<RTTI*>(&f2));
			Assert::IsTrue(RTTIDatum.Get<RTTI*>(2) == reinterpret_cast<RTTI*>(&f3));

			Assert::ExpectException<std::exception>([&RTTIDatum]
			{
				RTTIDatum.PushBack("yo");
			});

			RTTIDatum.PopBack();
			Assert::ExpectException<std::exception>([&RTTIDatum]
			{
				RTTIDatum.Get<RTTI*>(2);
			});

			RTTIDatum.PushBack(reinterpret_cast<RTTI*>(&f4));
			Assert::IsTrue(RTTIDatum.Get<RTTI*>(2) == reinterpret_cast<RTTI*>(&f4));

			RTTIDatum.Reserve(20);
			Assert::ExpectException<std::exception>([&RTTIDatum]
			{
				RTTIDatum.Get<RTTI*>(15);
			});
			Assert::ExpectException<std::exception>([&RTTIDatum, &f5]
			{
				RTTIDatum.Set(reinterpret_cast<RTTI*>(&f5), 15);
			});

			RTTIDatum.Set(reinterpret_cast<RTTI*>(&f6), 0);
			Assert::IsTrue(RTTIDatum.Get<RTTI*>(0) == reinterpret_cast<RTTI*>(&f6));

			RTTIDatum.PopBack();
			RTTIDatum.PopBack();
			RTTIDatum.PopBack();

		}

		TEST_METHOD(DatumTestScalarAssignment)
		{
			Datum intDatum;
			intDatum = 10;
			Assert::IsTrue(intDatum.Get<int>() == 10);
			Assert::ExpectException<std::exception>([&intDatum]
			{
				intDatum = "hello";
			});
			intDatum = 50;
			Assert::IsTrue(intDatum.Get<int>() == 50);

			Datum stringDatum;
			stringDatum = "hello";
			Assert::IsTrue(stringDatum.Get<std::string>() == "hello");
			Assert::ExpectException<std::exception>([&stringDatum]
			{
				stringDatum = 10;
			});
			stringDatum = "yo";
			Assert::IsTrue(stringDatum.Get<std::string>() == "yo");

			Datum floatDatum;
			floatDatum = 10.5f;
			Assert::IsTrue(floatDatum.Get<float>() == 10.5f);
			Assert::ExpectException<std::exception>([&floatDatum]
			{
				floatDatum = "hello";
			});
			floatDatum = 50.5f;
			Assert::IsTrue(floatDatum.Get<float>() == 50.5);

			glm::vec4 vect1 = glm::vec4(1.0f);
			glm::vec4 vect2 = glm::vec4(2.0f);
			Datum vec4Datum;
			vec4Datum = vect1;
			Assert::IsTrue(vec4Datum.Get<glm::vec4>() == vect1);
			Assert::ExpectException<std::exception>([&vec4Datum]
			{
				vec4Datum = "hello";
			});
			vec4Datum = vect2;
			Assert::IsTrue(vec4Datum.Get<glm::vec4>() == vect2);

			glm::mat4 matr1 = glm::mat4(1.0f);
			glm::mat4 matr2 = glm::mat4(2.0f);
			Datum mat4Datum;
			mat4Datum = matr1;
			Assert::IsTrue(mat4Datum.Get<glm::mat4>() == matr1);
			Assert::ExpectException<std::exception>([&mat4Datum]
			{
				mat4Datum = "hello";
			});
			mat4Datum = matr2;
			Assert::IsTrue(mat4Datum.Get<glm::mat4>() == matr2);

			Foo* foos1 = new Foo[5];
			Foo* foos2 = new Foo[10];
			Datum RTTIDatum;
			RTTIDatum = reinterpret_cast<RTTI*>(foos1);
			Assert::IsTrue(RTTIDatum.Get<RTTI*>() == reinterpret_cast<RTTI*>(foos1));
			Assert::ExpectException<std::exception>([&RTTIDatum]
			{
				RTTIDatum = "hello";
			});
			RTTIDatum = reinterpret_cast<RTTI*>(foos2);
			Assert::IsTrue(RTTIDatum.Get<RTTI*>() == reinterpret_cast<RTTI*>(foos2));
			delete[] foos1;
			delete[] foos2;

		}

		TEST_METHOD(DatumTestStorageInteger)
		{
			Datum internalDatum;
			internalDatum.PushBack(5);

			int nums[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			Assert::ExpectException<std::exception>([&internalDatum, &nums]
			{
				internalDatum.SetStorage(&nums[0], 10U);
			});
			
			Datum intDatum;
			intDatum.SetStorage(&nums[0], 10U);
			Assert::IsTrue(Datum::DatumType::Integer == intDatum.Type());
			Assert::IsTrue(intDatum.Get<int>() == 0);
			Assert::IsTrue(intDatum.Get<int>(9) == 9);
			Assert::AreEqual(intDatum.Size(), 10U);
			Assert::IsTrue(intDatum.IsExternal());

			Assert::ExpectException<std::exception>([&intDatum]
			{
				intDatum.SetType(Datum::DatumType::Float);
			});
			Assert::ExpectException<std::exception>([&intDatum]
			{
				intDatum.Clear();
			});
			Assert::ExpectException<std::exception>([&intDatum]
			{
				intDatum.PushBack(10);
			});
			Assert::ExpectException<std::exception>([&intDatum]
			{
				intDatum.PopBack();
			});
			Assert::ExpectException<std::exception>([&intDatum]
			{
				intDatum.Reserve(20);
			});
		}

		TEST_METHOD(DatumTestStorageFloat)
		{
			Datum internalDatum;
			internalDatum.PushBack(5.0f);

			float nums[10] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
			Assert::ExpectException<std::exception>([&internalDatum, &nums]
			{
				internalDatum.SetStorage(&nums[0], 10U);
			});
			
			Datum floatDatum;
			floatDatum.SetStorage(&nums[0], 10U);
			Assert::IsTrue(Datum::DatumType::Float == floatDatum.Type());
			Assert::IsTrue(floatDatum.Get<float>() == 0.0f);
			Assert::IsTrue(floatDatum.Get<float>(9) == 9.0f);
			Assert::AreEqual(floatDatum.Size(), 10U);
			Assert::IsTrue(floatDatum.IsExternal());

			Assert::ExpectException<std::exception>([&floatDatum]
			{
				floatDatum.SetType(Datum::DatumType::String);
			});
			Assert::ExpectException<std::exception>([&floatDatum]
			{
				floatDatum.Clear();
			});
			Assert::ExpectException<std::exception>([&floatDatum]
			{
				floatDatum.PushBack(10.0f);
			});
			Assert::ExpectException<std::exception>([&floatDatum]
			{
				floatDatum.PopBack();
			});
			Assert::ExpectException<std::exception>([&floatDatum]
			{
				floatDatum.Reserve(20);
			});
		}

		TEST_METHOD(DatumTestStorageString)
		{
			Datum internalDatum;
			internalDatum.PushBack("yo");

			std::string strs[10] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };
			Assert::ExpectException<std::exception>([&internalDatum, &strs]
			{
				internalDatum.SetStorage(&strs[0], 10U);
			});


			Datum stringDatum;
			stringDatum.SetStorage(&strs[0], 10U);
			Assert::IsTrue(Datum::DatumType::String == stringDatum.Type());
			Assert::IsTrue(stringDatum.Get<std::string>() == "a");
			Assert::IsTrue(stringDatum.Get<std::string>(9) == "j");
			Assert::AreEqual(stringDatum.Size(), 10U);
			Assert::IsTrue(stringDatum.IsExternal());

			Assert::ExpectException<std::exception>([&stringDatum]
			{
				stringDatum.SetType(Datum::DatumType::Integer);
			});
			Assert::ExpectException<std::exception>([&stringDatum]
			{
				stringDatum.Clear();
			});
			Assert::ExpectException<std::exception>([&stringDatum]
			{
				stringDatum.PushBack("hey");
			});
			Assert::ExpectException<std::exception>([&stringDatum]
			{
				stringDatum.PopBack();
			});
			Assert::ExpectException<std::exception>([&stringDatum]
			{
				stringDatum.Reserve(20);
			});
		}

		TEST_METHOD(DatumTestStorageVector)
		{
			glm::vec4 vect(5.0f);
			Datum internalDatum;
			internalDatum.PushBack(vect);

			glm::vec4 vecs[5] = { glm::vec4(0.0f), glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(3.0f), glm::vec4(4.0f) };
			Assert::ExpectException<std::exception>([&internalDatum, &vecs]
			{
				internalDatum.SetStorage(&vecs[0], 5U);
			});


			Datum vec4Datum;
			vec4Datum.SetStorage(&vecs[0], 5U);
			Assert::IsTrue(Datum::DatumType::Vector == vec4Datum.Type());
			Assert::IsTrue(vec4Datum.Get<glm::vec4>() == vecs[0]);
			Assert::IsTrue(vec4Datum.Get<glm::vec4>(4) == vecs[4]);
			Assert::AreEqual(vec4Datum.Size(), 5U);
			Assert::IsTrue(vec4Datum.IsExternal());

			Assert::ExpectException<std::exception>([&vec4Datum]
			{
				vec4Datum.SetType(Datum::DatumType::Integer);
			});
			Assert::ExpectException<std::exception>([&vec4Datum]
			{
				vec4Datum.Clear();
			});
			Assert::ExpectException<std::exception>([&vec4Datum, &vect]
			{
				vec4Datum.PushBack(vect);
			});
			Assert::ExpectException<std::exception>([&vec4Datum]
			{
				vec4Datum.PopBack();
			});
			Assert::ExpectException<std::exception>([&vec4Datum]
			{
				vec4Datum.Reserve(20);
			});
		}
	
		TEST_METHOD(DatumTestStorageMatrix)
		{
			glm::mat4 matr(5.0f);
			Datum internalDatum;
			internalDatum.PushBack(matr);

			glm::mat4 mats[5] = { glm::mat4(0.0f), glm::mat4(1.0f), glm::mat4(2.0f), glm::mat4(3.0f), glm::mat4(4.0f) };
			Assert::ExpectException<std::exception>([&internalDatum, &mats]
			{
				internalDatum.SetStorage(&mats[0], 5U);
			});


			Datum mat4Datum;
			mat4Datum.SetStorage(&mats[0], 5U);
			Assert::IsTrue(Datum::DatumType::Matrix == mat4Datum.Type());
			Assert::IsTrue(mat4Datum.Get<glm::mat4>() == mats[0]);
			Assert::IsTrue(mat4Datum.Get<glm::mat4>(4) == mats[4]);
			Assert::AreEqual(mat4Datum.Size(), 5U);
			Assert::IsTrue(mat4Datum.IsExternal());

			Assert::ExpectException<std::exception>([&mat4Datum]
			{
				mat4Datum.SetType(Datum::DatumType::Integer);
			});
			Assert::ExpectException<std::exception>([&mat4Datum]
			{
				mat4Datum.Clear();
			});
			Assert::ExpectException<std::exception>([&mat4Datum, &matr]
			{
				mat4Datum.PushBack(matr);
			});
			Assert::ExpectException<std::exception>([&mat4Datum]
			{
				mat4Datum.PopBack();
			});
			Assert::ExpectException<std::exception>([&mat4Datum]
			{
				mat4Datum.Reserve(20);
			});
		}

		TEST_METHOD(DatumTestStoragePointer)
		{
			RFoo *f1 = new RFoo(5);
			Datum internalDatum;
			internalDatum.PushBack(f1);

			RTTI** foos = (RTTI**)std::malloc(4 * sizeof(RTTI*));
			foos[0] = new RFoo[5];
			foos[1] = new RFoo[5];
			foos[2] = new RFoo[6];
			foos[3] = new RFoo[7];

			Assert::ExpectException<std::exception>([&internalDatum, &foos]
			{
				internalDatum.SetStorage(foos, 4U);
			});

			Datum RTTIDatum;
			RTTIDatum.SetStorage(foos, 4U);
			Assert::IsTrue(Datum::DatumType::Pointer == RTTIDatum.Type());
			Assert::IsTrue(RTTIDatum.Get<RTTI*>() == foos[0]);
			Assert::IsTrue(RTTIDatum.Get<RTTI*>(3) == foos[3]);
			Assert::AreEqual(RTTIDatum.Size(), 4U);
			Assert::IsTrue(RTTIDatum.IsExternal());

			Assert::ExpectException<std::exception>([&RTTIDatum]
			{
				RTTIDatum.SetType(Datum::DatumType::Float);
			});
			Assert::ExpectException<std::exception>([&RTTIDatum]
			{
				RTTIDatum.Clear();
			});
			Assert::ExpectException<std::exception>([&RTTIDatum, &f1]
			{
				RTTIDatum.PushBack(f1);
			});
			Assert::ExpectException<std::exception>([&RTTIDatum]
			{
				RTTIDatum.PopBack();
			});
			Assert::ExpectException<std::exception>([&RTTIDatum]
			{
				RTTIDatum.Reserve(20);
			});
			delete f1;
			delete[] foos[0];
			delete[] foos[1];
			delete[] foos[2];
			delete[] foos[3];
			free(foos);
		}

		TEST_METHOD(DatumTestAssignment)
		{
			//internal to external storage
			Datum internalDatum1;
			internalDatum1.PushBack("yo");
			internalDatum1.PushBack("ho");

			int nums[3] = { 10, 1, 3 };
			Datum externalDatum1;
			externalDatum1.SetStorage(nums, 3U);

			internalDatum1 = externalDatum1;
			Assert::AreEqual(internalDatum1.Size(), 3U);
			Assert::AreEqual(internalDatum1.Capacity(), 3U);
			Assert::IsTrue(internalDatum1.Type() == Datum::DatumType::Integer);
			Assert::IsTrue(internalDatum1.IsExternal());
			Assert::IsTrue(internalDatum1.Get<int>() == 10);

			internalDatum1 = 200;
			Assert::IsTrue(internalDatum1.Get<int>() == 200);
			Assert::IsTrue(nums[0] == 200);

			//external to internal storage
			int nums2[3] = { 10, 1, 3 };
			Datum externalDatum2;
			externalDatum2.SetStorage(nums2, 3U);

			Datum internalDatum2;
			internalDatum2.PushBack("yo");
			internalDatum2.PushBack("ho");

			externalDatum2 = internalDatum2;
			Assert::AreEqual(externalDatum2.Size(), 2U);
			Assert::AreEqual(externalDatum2.Capacity(), 2U);
			Assert::IsTrue(externalDatum2.Type() == Datum::DatumType::String);
			Assert::IsFalse(externalDatum2.IsExternal());
			Assert::IsTrue(externalDatum2.Get<std::string>() == "yo");

			externalDatum2.PushBack("no");
			Assert::IsTrue(externalDatum2.Get<std::string>(2) == "no");

			//internal to internal storage
			Datum internalDatum3;
			internalDatum3.PushBack(1);
			internalDatum3.PushBack(2);

			Datum internalDatum4;
			internalDatum4.PushBack("yo");
			internalDatum4.PushBack("go");
			internalDatum4.PushBack("ho");

			internalDatum3 = internalDatum4;
			Assert::AreEqual(internalDatum3.Size(), 3U);
			Assert::AreEqual(internalDatum3.Capacity(), 3U);
			Assert::IsTrue(internalDatum3.Type() == Datum::DatumType::String);
			Assert::IsFalse(internalDatum3.IsExternal());
			Assert::IsTrue(internalDatum3.Get<std::string>() == "yo");

			internalDatum4 = "bye";
			Assert::IsTrue(internalDatum3.Get<std::string>() != "bye");

			//external to external storage
			int nums3[3] = { 10, 1, 3 };
			std::string strs[4] = { "yo", "ho", "go", "po" };
			Datum externalDatum3, externalDatum4;
			externalDatum3.SetStorage(nums3, 3U);
			externalDatum4.SetStorage(strs, 4U);

			externalDatum3 = externalDatum4;
			Assert::AreEqual(externalDatum3.Size(), 4U);
			Assert::AreEqual(externalDatum3.Capacity(), 4U);
			Assert::IsTrue(externalDatum3.Type() == Datum::DatumType::String);
			Assert::IsTrue(externalDatum3.IsExternal());
			Assert::IsTrue(externalDatum3.Get<std::string>() == "yo");

			externalDatum3 = "why?";
			Assert::IsTrue(externalDatum3.Get<std::string>() == "why?");
			Assert::IsTrue(strs[0] == "why?");
			Assert::IsTrue(externalDatum4.Get<std::string>() == "why?");
		}

		TEST_METHOD(DatumTestCopy)
		{
			Datum internalDatum1;
			internalDatum1.PushBack("yo");
			internalDatum1.PushBack("ho");

			Datum newDatum1(internalDatum1);
			Assert::AreEqual(newDatum1.Size(), 2U);
			Assert::AreEqual(newDatum1.Capacity(), 2U);
			Assert::IsTrue(newDatum1.Type() == Datum::DatumType::String);
			Assert::IsFalse(newDatum1.IsExternal());
			Assert::IsTrue(newDatum1.Get<std::string>() == "yo");
			Assert::IsTrue(newDatum1 == internalDatum1);

			int nums[3] = { 10, 1, 3 };
			Datum externalDatum1;
			externalDatum1.SetStorage(nums, 3U);

			Datum newDatum2(externalDatum1);
			Assert::AreEqual(newDatum2.Size(), 3U);
			Assert::AreEqual(newDatum2.Capacity(), 3U);
			Assert::IsTrue(newDatum2.Type() == Datum::DatumType::Integer);
			Assert::IsTrue(newDatum2.IsExternal());
			Assert::IsTrue(newDatum2.Get<int>() == 10);
			Assert::IsTrue(newDatum2 == externalDatum1);
		}

		TEST_METHOD(DatumTestSizeAndClear)
		{
			Datum intDatum;
			Assert::IsTrue(intDatum.IsEmpty());
			intDatum.PushBack(1);
			Assert::IsTrue(intDatum.Size() == 1U);
			intDatum.Reserve(10);
			Assert::IsTrue(intDatum.Capacity() == 10U);
			Assert::IsTrue(intDatum.Size() == 1U);
			intDatum.PushBack(2);
			Assert::IsTrue(intDatum.Capacity() == 10U);
			Assert::IsTrue(intDatum.Size() == 2U);
			intDatum.PopBack();
			Assert::IsTrue(intDatum.Size() == 1U);
			intDatum.Clear();
			Assert::IsTrue(intDatum.IsEmpty());
			Assert::IsTrue(intDatum.Capacity() == 0U);

			Datum floatDatum;
			Assert::IsTrue(floatDatum.IsEmpty());
			floatDatum.PushBack(1.0f);
			Assert::IsTrue(floatDatum.Size() == 1U);
			floatDatum.Reserve(10);
			Assert::IsTrue(floatDatum.Capacity() == 10U);
			Assert::IsTrue(floatDatum.Size() == 1U);
			floatDatum.PushBack(2.0f);
			Assert::IsTrue(floatDatum.Capacity() == 10U);
			Assert::IsTrue(floatDatum.Size() == 2U);
			floatDatum.PopBack();
			Assert::IsTrue(floatDatum.Size() == 1U);
			floatDatum.Clear();
			Assert::IsTrue(floatDatum.IsEmpty());
			Assert::IsTrue(floatDatum.Capacity() == 0U);

			Datum stringDatum;
			Assert::IsTrue(stringDatum.IsEmpty());
			stringDatum.PushBack("yo");
			Assert::IsTrue(stringDatum.Size() == 1U);
			stringDatum.Reserve(10);
			Assert::IsTrue(stringDatum.Capacity() == 10U);
			Assert::IsTrue(stringDatum.Size() == 1U);
			stringDatum.PushBack("yo");
			Assert::IsTrue(stringDatum.Capacity() == 10U);
			Assert::IsTrue(stringDatum.Size() == 2U);
			stringDatum.PopBack();
			Assert::IsTrue(stringDatum.Size() == 1U);
			stringDatum.Clear();
			Assert::IsTrue(stringDatum.IsEmpty());
			Assert::IsTrue(stringDatum.Capacity() == 0U);

			Datum vecDatum;
			Assert::IsTrue(vecDatum.IsEmpty());
			vecDatum.PushBack(glm::vec4());
			Assert::IsTrue(vecDatum.Size() == 1U);
			vecDatum.Reserve(10);
			Assert::IsTrue(vecDatum.Capacity() == 10U);
			Assert::IsTrue(vecDatum.Size() == 1U);
			vecDatum.PushBack(glm::vec4());
			Assert::IsTrue(vecDatum.Capacity() == 10U);
			Assert::IsTrue(vecDatum.Size() == 2U);
			vecDatum.PopBack();
			Assert::IsTrue(vecDatum.Size() == 1U);
			vecDatum.Clear();
			Assert::IsTrue(vecDatum.IsEmpty());
			Assert::IsTrue(vecDatum.Capacity() == 0U);

			Datum matDatum;
			Assert::IsTrue(matDatum.IsEmpty());
			matDatum.PushBack(glm::mat4());
			Assert::IsTrue(matDatum.Size() == 1U);
			matDatum.Reserve(10);
			Assert::IsTrue(matDatum.Capacity() == 10U);
			Assert::IsTrue(matDatum.Size() == 1U);
			matDatum.PushBack(glm::mat4());
			Assert::IsTrue(matDatum.Capacity() == 10U);
			Assert::IsTrue(matDatum.Size() == 2U);
			matDatum.PopBack();
			Assert::IsTrue(matDatum.Size() == 1U);
			matDatum.Clear();
			Assert::IsTrue(matDatum.IsEmpty());
			Assert::IsTrue(matDatum.Capacity() == 0U);

			RFoo f1(10), f2(20);
			Datum ptrDatum;
			Assert::IsTrue(ptrDatum.IsEmpty());
			ptrDatum.PushBack(&f1);
			Assert::IsTrue(ptrDatum.Size() == 1U);
			ptrDatum.Reserve(10);
			Assert::IsTrue(ptrDatum.Capacity() == 10U);
			Assert::IsTrue(ptrDatum.Size() == 1U);
			ptrDatum.PushBack(&f2);
			Assert::IsTrue(ptrDatum.Capacity() == 10U);
			Assert::IsTrue(ptrDatum.Size() == 2U);
			ptrDatum.PopBack();
			Assert::IsTrue(ptrDatum.Size() == 1U);
			ptrDatum.Clear();
			Assert::IsTrue(ptrDatum.IsEmpty());
			Assert::IsTrue(ptrDatum.Capacity() == 0U);

		}

		TEST_METHOD(DatumTestSetFromString)
		{
			Datum intDatum;
			intDatum.PushBack(1);
			intDatum.SetFromString("10", Datum::DatumType::Integer);
			Assert::IsTrue(intDatum.Get<int>() == 10);

			Datum floatDatum;
			floatDatum.PushBack(1.0f);
			floatDatum.SetFromString("10.0f", Datum::DatumType::Float);
			Assert::IsTrue(floatDatum.Get<float>() == 10.0f);

			Datum stringDatum;
			stringDatum.PushBack("yo");
			stringDatum.SetFromString("bye", Datum::DatumType::String);
			Assert::IsTrue(stringDatum.Get<std::string>() == "bye");

			Datum vecDatum;
			glm::vec4 vec(1.0f), vec1(2.0f);
			vecDatum.PushBack(vec);
			vecDatum.SetFromString("2.0 2.0 2.0 2.0", Datum::DatumType::Vector);
			Assert::IsTrue(vecDatum.Get<glm::vec4>() == vec1);

			Datum matDatum;
			glm::mat4 mat(1.0f), mat1(vec1, vec1, vec1, vec1);
			matDatum.PushBack(mat);
			matDatum.SetFromString("2.0 2.0 2.0 2.0 2.0 2.0 2.0 2.0 2.0 2.0 2.0 2.0 2.0 2.0 2.0 2.0", Datum::DatumType::Matrix);
			Assert::IsTrue(matDatum.Get<glm::mat4>() == mat1);

			Datum ptrDatum;
			RFoo f1(10);
			ptrDatum.PushBack(&f1);
			Assert::ExpectException<std::exception>([&ptrDatum]
			{
				ptrDatum.SetFromString("abc", Datum::DatumType::Pointer);
			});
		}

		TEST_METHOD(DatumTestToString)
		{
			Datum intDatum;
			intDatum.PushBack(10);
			Assert::IsTrue(intDatum.ToString(0) == "10");

			Datum floatDatum;
			floatDatum.PushBack(5.6f);
			Assert::IsTrue(floatDatum.ToString(0) == "5.600000");

			Datum stringDatum;
			stringDatum.PushBack("yo");
			Assert::IsTrue(stringDatum.ToString(0) == "yo");

			glm::vec4 vec(1.0f);
			Datum vecDatum;
			vecDatum.PushBack(vec);
			Assert::IsTrue(vecDatum.ToString(0) == "1.000000 1.000000 1.000000 1.000000");

			glm::mat4 mat(vec, vec, vec, vec);
			Datum matDatum;
			matDatum.PushBack(mat);
			Assert::IsTrue(matDatum.ToString(0) == "1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000");
		
			RFoo f1(10);
			Datum ptrDatum;
			ptrDatum.PushBack(reinterpret_cast<RTTI*>(&f1));
			Assert::IsTrue(ptrDatum.ToString(0) == "RTTI");
		}

		TEST_METHOD(DatumTestCompare)
		{
			Datum intDatum1, intDatum2;
			intDatum1 = 10;
			intDatum2 = 10;
			Assert::IsTrue(intDatum1 == intDatum2);
			Assert::IsTrue(intDatum1 == 10);
			Assert::IsTrue(intDatum2 == 10);
			intDatum2.PushBack(20);
			Assert::IsTrue(intDatum1 != intDatum2);
			intDatum2 = 20;
			Assert::IsTrue(intDatum1 != intDatum2.Get<int>());

			Datum floatDatum1, floatDatum2;
			floatDatum1 = 10.0f;
			floatDatum2 = 10.0f;
			Assert::IsTrue(floatDatum1 == floatDatum2);
			Assert::IsTrue(floatDatum1 == 10.0f);
			Assert::IsTrue(floatDatum2 == 10.0f);
			floatDatum2.PushBack(20.0f);
			Assert::IsTrue(floatDatum1 != floatDatum2);
			floatDatum2 = 20.0f;
			Assert::IsTrue(floatDatum1 != floatDatum2.Get<float>());

			Datum stringDatum1, stringDatum2;
			stringDatum1 = "yo";
			stringDatum2 = "yo";
			Assert::IsTrue(stringDatum1 == stringDatum2);
			Assert::IsTrue(stringDatum1 == "yo");
			Assert::IsTrue(stringDatum2 == "yo");
			stringDatum2.PushBack("ho");
			Assert::IsTrue(stringDatum1 != stringDatum2);
			stringDatum2 = "ho";
			Assert::IsTrue(stringDatum1 != stringDatum2.Get<std::string>());

			glm::vec4 vec1(1.0f), vec2(2.0f);
			Datum vecDatum1, vecDatum2;
			vecDatum1 = vec1;
			vecDatum2 = vec1;
			Assert::IsTrue(vecDatum1 == vecDatum2);
			Assert::IsTrue(vecDatum1 == vec1);
			Assert::IsTrue(vecDatum2 == vec1);
			vecDatum2.PushBack(vec2);
			Assert::IsTrue(vecDatum1 != vecDatum2);
			vecDatum2 = vec2;
			Assert::IsTrue(vecDatum1 != vecDatum2.Get<glm::vec4>());

			glm::mat4 mat1(1.0f), mat2(2.0f);
			Datum matDatum1, matDatum2;
			matDatum1 = mat1;
			matDatum2 = mat1;
			Assert::IsTrue(matDatum1 == matDatum2);
			Assert::IsTrue(matDatum1 == mat1);
			Assert::IsTrue(matDatum2 == mat1);
			matDatum2.PushBack(mat2);
			Assert::IsTrue(matDatum1 != matDatum2);
			matDatum2 = mat2;
			Assert::IsTrue(matDatum1 != matDatum2.Get<glm::mat4>());

			RFoo f1(10), f2(20);
			Datum ptrDatum1, ptrDatum2;
			ptrDatum1 = reinterpret_cast<RTTI*>(&f1);
			ptrDatum2 = reinterpret_cast<RTTI*>(&f1);
			Assert::IsTrue(ptrDatum1 == ptrDatum2);
			Assert::IsTrue(ptrDatum1 == reinterpret_cast<RTTI*>(&f1));
			Assert::IsTrue(ptrDatum2 == reinterpret_cast<RTTI*>(&f1));
			ptrDatum2.PushBack(reinterpret_cast<RTTI*>(&f2));
			Assert::IsTrue(ptrDatum1 != ptrDatum2);
			ptrDatum2 = reinterpret_cast<RTTI*>(&f2);
			Assert::IsTrue(ptrDatum1 != ptrDatum2.Get<RTTI*>());

		}

	private:
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState DatumTest::sStartMemState;
#endif
}