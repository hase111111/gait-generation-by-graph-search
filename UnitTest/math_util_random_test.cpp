#include "pch.h"

#include "../DesignLab/designlab_math_util.h"


namespace dlm = designlab::math_util;


namespace designlab::test::common::math
{
	TEST(MathUtilTest, GenerateRandomNumberTestNumInRangeCaseOfDouble)
	{
		// 乱数を返すメソッドのテスト．
		// 乱数なので，テストはあまり意味がない気がする...よい方法はなんじゃらほい．
		// とりあえず，試行回数を増やして，確認する方針でいきます．

		constexpr int kTestNum = 10000;	//試行回数
		constexpr double kMin = -10.45;
		constexpr double kMax = 234.47;

		for (int i = 0; i < kTestNum; ++i)
		{
			const double _res = dlm::GenerateRandomNumber(kMin, kMax);

			// LT = Less Than <
			// GT = Greater Than >

			EXPECT_LT(kMin, _res) << "最小値を下回っています．";
			EXPECT_GT(kMax, _res) << "最大値を上回っています．";
		}
	}

	TEST(MathUtilTest, GenerateRandomNumberTestNumInRangeCaseOfFloat)
	{
		// 乱数を返すメソッドのテスト．
		// 乱数なので，テストはあまり意味がない気がする...よい方法はなんじゃらほい．
		// とりあえず，試行回数を増やして，確認する方針でいきます．

		constexpr int kTestNum = 10000;	//試行回数
		constexpr float kMin = -10.45f;
		constexpr float kMax = 234.47f;

		for (int i = 0; i < kTestNum; ++i)
		{
			const float _res = dlm::GenerateRandomNumber(kMin, kMax);

			// LT = Less Than <
			// GT = Greater Than >

			EXPECT_LT(kMin, _res) << "最小値を下回っています．";
			EXPECT_GT(kMax, _res) << "最大値を上回っています．";
		}
	}

	TEST(MathUtilTest, GenerateRandomNumberTestNumInRangeCaseOfInt)
	{
		// 乱数を返すメソッドのテスト．
		// 乱数なので，テストはあまり意味がない気がする...よい方法はなんじゃらほい．
		// とりあえず，試行回数を増やして，確認する方針でいきます．

		constexpr int kTestNum = 10000;	//試行回数
		constexpr int kMin = -10;
		constexpr int kMax = 234;

		for (int i = 0; i < kTestNum; ++i)
		{
			const int _res = dlm::GenerateRandomNumber(kMin, kMax);

			// LT = Less Than <
			// GT = Greater Than >

			EXPECT_LT(kMin, _res) << "最小値を下回っています．";
			EXPECT_GT(kMax, _res) << "最大値を上回っています．";
		}
	}
}