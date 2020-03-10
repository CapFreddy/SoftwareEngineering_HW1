#include "pch.h"
#include "CppUnitTest.h"
#include "../IntersectProject/Class.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace unittest
{
	TEST_CLASS(unittest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// original tests in blog
			string test_1 = "../IntersectProject/test/test1.txt";
			string test_2 = ",./IntersectProject/test/test2.txt";
			string test_3 = "../IntersectProject/test/test3.txt";
			string test_4 = "../IntersectProject/test/test4.txt";

			BurteForce bf;
			Assert::AreEqual(bf.getIntersection(test_1), 0);
			Assert::AreEqual(bf.getIntersection(test_2), 3);
			Assert::AreEqual(bf.getIntersection(test_3), 1);
			Assert::AreEqual(bf.getIntersection(test_4), 6);
		}

		TEST_METHOD(TestMethod2)
		{
			// lines colliding, other 3 vertical or parallel to x-axis
			string test_5 = "../IntersectProject/test/test5.txt";
			string test_6 = "../IntersectProject/test/test6.txt";


			BurteForce bf;
			Assert::AreEqual(bf.getIntersection(test_5), 10);
			Assert::AreEqual(bf.getIntersection(test_6), 7);
		}

		TEST_METHOD(TestMethod3)
		{
			// collode in one point
			string test_7 = "../IntersectProject/test/test7";
			string test_8 = "../IntersectProject/test/test8";
			string test_9 = "../IntersectProject/test/test9";
			string test_10 = "../IntersectProject/test/test10";

			BurteForce bf;
			Assert::AreEqual(bf.getIntersection(test_7), 5);
			Assert::AreEqual(bf.getIntersection(test_8), 5);
			Assert::AreEqual(bf.getIntersection(test_9), 8);
		}
	};
}
