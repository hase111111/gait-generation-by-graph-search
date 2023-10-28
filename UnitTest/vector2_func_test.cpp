#include "pch.h"

#include "../DesignLab/designlab_vector2.h"


namespace dl = ::designlab;


namespace 
{
	// テスト用のヘルパー関数

	dl::Vector2 MakeVec2(const float x, const float y)
	{
		dl::Vector2 vec;
		vec.x = x;
		vec.y = y;

		return vec;
	}

	std::string ToString(const dl::Vector2& vec)
	{
		return "(" + std::to_string(vec.x) + "," + std::to_string(vec.y) + ")";
	}
}


namespace designlab::test::common::math
{
	TEST(Vector2Test, GetLengthTest)
	{
		std::vector<std::tuple<dl::Vector2, float>> testcase_list{
			{ MakeVec2(3.f, 4.f), 5.f },
			{ MakeVec2(1.f, 1.f), 1.414213562f },
			{ MakeVec2(0.f, 0.f), 0.f },
			{ MakeVec2(-1.f, -1.f), 1.414213562f },
			{ MakeVec2(-3.f, -4.f), 5.f },
			{ MakeVec2(3.f, -4.f), 5.f }
		};

		for (const auto& data : testcase_list)
		{
			const dl::Vector2 vec = std::get<0>(data);
			const float expected = std::get<1>(data);

			std::string test_discription = " GetLength 正解は : \n " + ToString(vec) + " = " + std::to_string(expected) + "\n";
			test_discription += " 実際は :" + std::to_string(vec.GetLength());

			EXPECT_FLOAT_EQ(expected, vec.GetLength()) << test_discription;
		}
	}

	TEST(Vector2Test, GetSquaredLengthTest)
	{
		std::vector<std::tuple<dl::Vector2, float>> testcase_list{
			{ MakeVec2(3.f, 4.f), 25.f },
			{ MakeVec2(1.f, 1.f), 2.f },
			{ MakeVec2(0.f, 0.f), 0.f },
			{ MakeVec2(-1.f, -1.f), 2.f },
			{ MakeVec2(-3.f, -4.f), 25.f },
			{ MakeVec2(3.f, -4.f), 25.f }
		};

		for (const auto& data : testcase_list)
		{
			const dl::Vector2 vec = std::get<0>(data);
			const float expected = std::get<1>(data);

			std::string test_discription = " GetSquaredLength 正解は : \n " + ToString(vec) + " = " + std::to_string(expected) + "\n";
			test_discription += " 実際は :" + std::to_string(vec.GetSquaredLength());

			EXPECT_FLOAT_EQ(expected, vec.GetSquaredLength()) << test_discription;
		}
	}

	TEST(Vector2Test, DotTest)
	{
		// vec1 ・ vec2 = expected の順のタプル
		std::vector<std::tuple<dl::Vector2, dl::Vector2, float>> testcase_list = {
			{ MakeVec2(3.0f, 4.0f),		MakeVec2(1.0f, 1.0f),	7.0f },
			{ MakeVec2(1.0f, 1.0f),		MakeVec2(1.0f, 1.0f),	2.0f},
			{ MakeVec2(0.0f, 0.0f),		MakeVec2(0.0f, 0.0f),	0.0f },
			{ MakeVec2(-1.0f, -1.0f),	MakeVec2(-1.0f, -1.0f),	2.0f },
			{ MakeVec2(-3.0f, -4.0f),	MakeVec2(-3.0f, -4.0f),	25.0f },
			{ MakeVec2(3.0f, -4.0f),	MakeVec2(3.0f, -4.0f),	25.0f }
		};

		for (const auto& data : testcase_list)
		{
			const dl::Vector2 vec1 = std::get<0>(data);
			const dl::Vector2 vec2 = std::get<1>(data);
			const float expected = std::get<2>(data);

			std::string test_discription = " Dot 正解は : \n " + ToString(vec1) + "・" + ToString(vec2) + " = " + std::to_string(expected) + "\n";
			test_discription += " 実際は :" + std::to_string(vec1.Dot(vec2));

			EXPECT_FLOAT_EQ(expected, vec1.Dot(vec2)) << test_discription;
		}
	}

	TEST(Vector2Test, CrossTest)
	{
		// vec1 × vec2 = expected の順のタプル
		std::vector<std::tuple<dl::Vector2, dl::Vector2, float>> testcase_list = {
			{ MakeVec2(3.f, 4.f), MakeVec2(1.f, 1.f), -1.f},
			{ MakeVec2(1.f, 1.f), MakeVec2(1.f, 1.f), 0.f},
			{ MakeVec2(0.f, 0.f), MakeVec2(0.f, 0.f), 0.f},
			{ MakeVec2(-1.f, -1.f), MakeVec2(-1.f, -1.f), 0.f},
			{ MakeVec2(-3.f, -4.f), MakeVec2(-3.f, -4.f), 0.f},
			{ MakeVec2(3.f, -4.f), MakeVec2(3.f, -4.f), 0.f}
		};

		for (const auto& data : testcase_list)
		{
			const dl::Vector2 vec1 = std::get<0>(data);
			const dl::Vector2 vec2 = std::get<1>(data);
			const float expected = std::get<2>(data);

			std::string test_discription = " Cross 正解は : \n " + ToString(vec1) + "×" + ToString(vec2) + " = " + std::to_string(expected) + "\n";
			test_discription += " 実際は :" + std::to_string(vec1.Cross(vec2));

			EXPECT_FLOAT_EQ(expected, vec1.Cross(vec2)) << test_discription;
		}
	}

	//distanceFrom関数のテスト
	TEST(Vector2Test, GetDistanceFromTest)
	{
		// vec1 と vec2 の距離 = expected の順のタプル
		std::vector<std::tuple<dl::Vector2, dl::Vector2, float>> testcase_list = {
			{ MakeVec2(3.f, 4.f), MakeVec2(1.f, 1.f), 3.60555128f },
			{ MakeVec2(1.f, 1.f), MakeVec2(1.f, 1.f), 0.0f },
			{ MakeVec2(0.f, 0.f), MakeVec2(0.f, 0.f), 0.0f },
			{ MakeVec2(-1.f, -1.f), MakeVec2(-1.f, -1.f), 0.0f },
			{ MakeVec2(-3.f, -4.f), MakeVec2(-3.f, -4.f), 0.0f },
			{ MakeVec2(3.f, -4.f), MakeVec2(3.f, -4.f), 0.0f },
			{ MakeVec2(30.f, 20.f), MakeVec2(-5.f, -10.f), 46.09772228f },
			{ MakeVec2(0.3f, 0.2f), MakeVec2(-0.5f, -0.1f), 0.854400374f }
		};

		for (const auto& data : testcase_list)
		{
			const dl::Vector2 vec1 = std::get<0>(data);
			const dl::Vector2 vec2 = std::get<1>(data);
			const float expected = std::get<2>(data);

			std::string test_discription = " GetDistanceFrom 正解は : \n " + ToString(vec1) + "と" + ToString(vec2) + "の距離 = " + std::to_string(expected) + "\n";
			test_discription += " 実際は :" + std::to_string(vec1.GetDistanceFrom(vec2));

			EXPECT_FLOAT_EQ(expected, vec1.GetDistanceFrom(vec2)) << test_discription;
		}
	}

	TEST(Vector2Test, GetNormalizedTest)
	{
		std::vector<std::tuple<dl::Vector2, dl::Vector2>> testcase_list{
			{ MakeVec2(3.f, 4.f), MakeVec2(0.6f, 0.8f) },
			{ MakeVec2(1.f, 1.f), MakeVec2(0.707106781f, 0.707106781f) },
			{ MakeVec2(-1.f, -1.f), MakeVec2(-0.707106781f, -0.707106781f) },
			{ MakeVec2(-3.f, -4.f), MakeVec2(-0.6f, -0.8f) },
			{ MakeVec2(3.f, -4.f), MakeVec2(0.6f, -0.8f) }
		};
		
		for (const auto& data : testcase_list)
		{
			const dl::Vector2 vec = std::get<0>(data);
			const dl::Vector2 expected = std::get<1>(data);

			std::string test_discription = " GetNormalized 正解は : \n " + ToString(vec) + " = " + ToString(expected) + "\n";
			test_discription += " 実際は :" + ToString(vec.GetNormalized());

			EXPECT_EQ(expected, vec.GetNormalized()) << test_discription;
		}
	}

	TEST(Vector2Test, GetNormalizedTestZeroVecTransToZeroVec) 
	{
		const auto zero_vec = MakeVec2(0.0f, 0.0f);	

		EXPECT_EQ(MakeVec2(0.0f, 0.0f).GetNormalized(), zero_vec) << "零ベクトルは正規化しても零ベクトルとなります．";

		EXPECT_EQ(MakeVec2(0.0001f, 0.00001f).GetNormalized(), zero_vec) << "実際に零でなくとも，零に十分近い場合零ベクトルとみなします．";
		EXPECT_EQ(MakeVec2(-0.0001f, -0.00001f).GetNormalized(), zero_vec) << "実際に零でなくとも，零に十分近い場合零ベクトルとみなします．";
	}

	TEST(Vector2Test, IsZeroTest)
	{
		std::vector<std::tuple<dl::Vector2, bool>> testcase_list{
			{ MakeVec2(0.0f, 0.0f), true },
			{ MakeVec2(0.00001f, -0.00001f), true },

			{ MakeVec2(1.0f, 0.0f), false },
			{ MakeVec2(0.0f, 1.0f), false },
			{ MakeVec2(3.0f, 4.0f), false },
			{ MakeVec2(-1.0f, -1.0f), false }
		};

		for (const auto& data : testcase_list)
		{
			const dl::Vector2 vec = std::get<0>(data);
			const bool expected = std::get<1>(data);

			std::string test_discription = "IsZero expected : \n " + ToString(vec) + " = " + std::to_string(expected) + "\n";
			test_discription += "Actual :" + std::to_string(vec.IsZero());

			EXPECT_EQ(expected, vec.IsZero()) << test_discription;
		}
	}

}