#include "pch.h"

#include "../DesignLab/designlab_vector2.h"
#include "../DesignLab/designlab_vector2.cpp"


namespace dl = ::designlab;


namespace 
{
	// テスト用のヘルパー関数

	dl::Vector2 MakeVec2(float x, float y)
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
	TEST(Vector2Test, GetLength)
	{
		EXPECT_FLOAT_EQ(5.0f, dl::Vector2(3.0f, 4.0f).GetLength());
		EXPECT_FLOAT_EQ(5.0f, dl::Vector2(-3.0f, -4.0f).GetLength());
		EXPECT_FLOAT_EQ(5.0f, dl::Vector2(3.0f, -4.0f).GetLength());
		EXPECT_FLOAT_EQ(341.760149f, dl::Vector2(120.0f, 320.0f).GetLength());

		EXPECT_FLOAT_EQ(0.0f, dl::Vector2(0.0f, 0.0f).GetLength());
	}

	TEST(Vector2Test, GetSquaredLength)
	{
		//sample1
		designlab::Vector2 vec(3.0f, 4.0f);
		EXPECT_FLOAT_EQ(25.0f, vec.GetSquaredLength());

		//sample2
		designlab::Vector2 vec2(1.0f, 1.0f);
		EXPECT_FLOAT_EQ(2.0f, vec2.GetSquaredLength());

		//sample3
		designlab::Vector2 vec3(0.0f, 0.0f);
		EXPECT_FLOAT_EQ(0.0f, vec3.GetSquaredLength());

		//sample4
		designlab::Vector2 vec4(-1.0f, -1.0f);
		EXPECT_FLOAT_EQ(2.0f, vec4.GetSquaredLength());

		//sample5
		designlab::Vector2 vec5(-3.0f, -4.0f);
		EXPECT_FLOAT_EQ(25.0f, vec5.GetSquaredLength());

		//sample6
		designlab::Vector2 vec6(3.0f, -4.0f);
		EXPECT_FLOAT_EQ(25.0f, vec6.GetSquaredLength());
	}

	//dot関数のテスト
	TEST(Vector2Test, Dot)
	{
		std::vector<std::tuple<dl::Vector2, dl::Vector2, float>> testcase_list = {
			{MakeVec2(3.0f, 4.0f),		MakeVec2(1.0f, 1.0f),	7.0f },
			{MakeVec2(1.0f, 1.0f),		MakeVec2(1.0f, 1.0f),	2.0f},
			{MakeVec2(0.0f, 0.0f),		MakeVec2(0.0f, 0.0f),	0.0f },
			{MakeVec2(-1.0f, -1.0f),	MakeVec2(-1.0f, -1.0f),	2.0f },
			{MakeVec2(-3.0f, -4.0f),	MakeVec2(-3.0f, -4.0f),	25.0f },
			{MakeVec2(3.0f, -4.0f),		MakeVec2(3.0f, -4.0f),	25.0f }
		};

		for (const auto& data : testcase_list)
		{
			const dl::Vector2 vec1 = std::get<0>(data);
			const dl::Vector2 vec2 = std::get<1>(data);
			const float expected = std::get<2>(data);

			std::string test_discription = "Dot expected : \n " + ToString(vec1) + "・" + ToString(vec2) + " = " + std::to_string(expected) + "\n";
			test_discription += "Actual :" + std::to_string(vec1.Dot(vec2));

			EXPECT_FLOAT_EQ(expected, vec1.Dot(vec2)) << test_discription;
		}
	}

	//cross関数のテスト
	TEST(Vector2Test, Cross)
	{
		//sample1
		designlab::Vector2 vec(3.0f, 4.0f);
		designlab::Vector2 vec2(1.0f, 1.0f);
		EXPECT_FLOAT_EQ(-1.0f, vec.Cross(vec2));

		//sample2
		designlab::Vector2 vec3(1.0f, 1.0f);
		designlab::Vector2 vec4(1.0f, 1.0f);
		EXPECT_FLOAT_EQ(0.0f, vec3.Cross(vec4));

		//sample3
		designlab::Vector2 vec5(0.0f, 0.0f);
		designlab::Vector2 vec6(0.0f, 0.0f);
		EXPECT_FLOAT_EQ(0.0f, vec5.Cross(vec6));

		//sample4
		designlab::Vector2 vec7(-1.0f, -1.0f);
		designlab::Vector2 vec8(-1.0f, -1.0f);
		EXPECT_FLOAT_EQ(0.0f, vec7.Cross(vec8));

		//sample5
		designlab::Vector2 vec9(-3.0f, -4.0f);
		designlab::Vector2 vec10(-3.0f, -4.0f);
		EXPECT_FLOAT_EQ(0.0f, vec9.Cross(vec10));

		//sample6
		designlab::Vector2 vec11(3.0f, -4.0f);
		designlab::Vector2 vec12(3.0f, -4.0f);
		EXPECT_FLOAT_EQ(0.0f, vec11.Cross(vec12));
	}

	//distanceFrom関数のテスト
	TEST(Vector2Test, GetDistanceFrom)
	{
		//sample1
		designlab::Vector2 vec(3.0f, 4.0f);
		designlab::Vector2 vec2(1.0f, 1.0f);
		EXPECT_FLOAT_EQ(3.60555128f, vec.GetDistanceFrom(vec2));

		//sample2
		designlab::Vector2 vec3(1.0f, 1.0f);
		designlab::Vector2 vec4(1.0f, 1.0f);
		EXPECT_FLOAT_EQ(0.0f, vec3.GetDistanceFrom(vec4));

		//sample3
		designlab::Vector2 vec5(0.0f, 0.0f);
		designlab::Vector2 vec6(0.0f, 0.0f);
		EXPECT_FLOAT_EQ(0.0f, vec5.GetDistanceFrom(vec6));

		//sample4
		designlab::Vector2 vec7(-1.0f, -1.0f);
		designlab::Vector2 vec8(-1.0f, -1.0f);
		EXPECT_FLOAT_EQ(0.0f, vec7.GetDistanceFrom(vec8));

		//sample5 - 大きな値の計算
		designlab::Vector2 vec9(30.0f, 20.0f);
		designlab::Vector2 vec10(-5.0f, -10.0f);
		EXPECT_FLOAT_EQ(46.09772228f, vec9.GetDistanceFrom(vec10));

		//sample6 - 浮動小数の計算
		designlab::Vector2 vec11(0.3f, 0.2f);
		designlab::Vector2 vec12(-0.5f, -0.1f);
		EXPECT_FLOAT_EQ(0.854400374f, vec11.GetDistanceFrom(vec12));
	}

	TEST(Vector2Test, GetNormalized)
	{
		//sample1
		designlab::Vector2 vec(3.0f, 4.0f);
		designlab::Vector2 res = vec.GetNormalized();
		EXPECT_FLOAT_EQ(0.6f, res.x);
		EXPECT_FLOAT_EQ(0.8f, res.y);

		//sample2
		designlab::Vector2 vec2(1.0f, 1.0f);
		designlab::Vector2 res2 = vec2.GetNormalized();
		EXPECT_FLOAT_EQ(0.707106781f, res2.x);
		EXPECT_FLOAT_EQ(0.707106781f, res2.y);

		//sample3　
		designlab::Vector2 vec3(0.0f, 0.0f);
		designlab::Vector2 res3 = vec3.GetNormalized();
		EXPECT_FLOAT_EQ(0.0f, res3.x);
		EXPECT_FLOAT_EQ(0.0f, res3.y);

		//sample4
		designlab::Vector2 vec4(-1.0f, -1.0f);
		designlab::Vector2 res4 = vec4.GetNormalized();
		EXPECT_FLOAT_EQ(-0.707106781f, res4.x);
		EXPECT_FLOAT_EQ(-0.707106781f, res4.y);

		//sample5
		designlab::Vector2 vec5(-3.0f, -4.0f);
		designlab::Vector2 res5 = vec5.GetNormalized();
		EXPECT_FLOAT_EQ(-0.6f, res5.x);
		EXPECT_FLOAT_EQ(-0.8f, res5.y);

		//sample6
		designlab::Vector2 vec6(3.0f, -4.0f);
		designlab::Vector2 res6 = vec6.GetNormalized();
		EXPECT_FLOAT_EQ(0.6f, res6.x);
		EXPECT_FLOAT_EQ(-0.8f, res6.y);
	}

	TEST(Vector2Test, IsZero)
	{
		EXPECT_TRUE(dl::Vector2(0.0f, 0.0f).IsZero());

		EXPECT_FALSE(dl::Vector2(1.0f, 0.0f).IsZero());
		EXPECT_FALSE(dl::Vector2(0.0f, 1.0f).IsZero());
		EXPECT_FALSE(dl::Vector2(3.0f, 4.0f).IsZero());
		EXPECT_FALSE(dl::Vector2(-1.0f, -1.0f).IsZero());
	}

}