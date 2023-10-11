#include "pch.h"

#include "../DesignLab/designlab_vector2.h"


namespace dl_vec_test
{
	//length関数のテスト
	TEST(Vector2, GetLength)
	{
		//sample1
		designlab::Vector2 vec(3.0f, 4.0f);
		EXPECT_FLOAT_EQ(5.0f, vec.GetLength());

		//sample2
		designlab::Vector2 vec2(1.0f, 1.0f);
		EXPECT_FLOAT_EQ(1.41421356f, vec2.GetLength());

		//sample3
		designlab::Vector2 vec3(0.0f, 0.0f);
		EXPECT_FLOAT_EQ(0.0f, vec3.GetLength());

		//sample4
		designlab::Vector2 vec4(-1.0f, -1.0f);
		EXPECT_FLOAT_EQ(1.41421356f, vec4.GetLength());

		//sample5
		designlab::Vector2 vec5(-3.0f, -4.0f);
		EXPECT_FLOAT_EQ(5.0f, vec5.GetLength());

		//sample6
		designlab::Vector2 vec6(3.0f, -4.0f);
		EXPECT_FLOAT_EQ(5.0f, vec6.GetLength());
	}

	//lengthSquare関数のテスト
	TEST(Vector2, GetSquaredLength)
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
	TEST(Vector2, Dot)
	{
		//sample1
		designlab::Vector2 vec(3.0f, 4.0f);
		designlab::Vector2 vec2(1.0f, 1.0f);
		EXPECT_FLOAT_EQ(7.0f, vec.Dot(vec2));

		//sample2
		designlab::Vector2 vec3(1.0f, 1.0f);
		designlab::Vector2 vec4(1.0f, 1.0f);
		EXPECT_FLOAT_EQ(2.0f, vec3.Dot(vec4));

		//sample3
		designlab::Vector2 vec5(0.0f, 0.0f);
		designlab::Vector2 vec6(0.0f, 0.0f);
		EXPECT_FLOAT_EQ(0.0f, vec5.Dot(vec6));

		//sample4
		designlab::Vector2 vec7(-1.0f, -1.0f);
		designlab::Vector2 vec8(-1.0f, -1.0f);
		EXPECT_FLOAT_EQ(2.0f, vec7.Dot(vec8));

		//sample5
		designlab::Vector2 vec9(-3.0f, -4.0f);
		designlab::Vector2 vec10(-3.0f, -4.0f);
		EXPECT_FLOAT_EQ(25.0f, vec9.Dot(vec10));

		//sample6
		designlab::Vector2 vec11(3.0f, -4.0f);
		designlab::Vector2 vec12(3.0f, -4.0f);
		EXPECT_FLOAT_EQ(25.0f, vec11.Dot(vec12));
	}

	//cross関数のテスト
	TEST(Vector2, Cross)
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
	TEST(Vector2, GetDistanceFrom)
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

	//normalized関数のテスト
	TEST(Vector2, GetNormalized)
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

	//isZero関数のテスト
	TEST(Vector2, IsZero)
	{
		//sample1
		designlab::Vector2 vec(3.0f, 4.0f);
		EXPECT_FALSE(vec.IsZero());

		//sample2
		designlab::Vector2 vec2(1.0f, 1.0f);
		EXPECT_FALSE(vec2.IsZero());

		//sample3
		designlab::Vector2 vec3(0.0f, 0.0f);
		EXPECT_TRUE(vec3.IsZero());

		//sample4
		designlab::Vector2 vec4(-1.0f, -1.0f);
		EXPECT_FALSE(vec4.IsZero());

		//sample5
		designlab::Vector2 vec5(-3.0f, -4.0f);
		EXPECT_FALSE(vec5.IsZero());

		//sample6
		designlab::Vector2 vec6(3.0f, -4.0f);
		EXPECT_FALSE(vec6.IsZero());
	}

}