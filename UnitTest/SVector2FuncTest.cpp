#include "pch.h"
#include "../DesignLab/MyVector2.h"

using namespace my_vec;

namespace SVector2Test
{
	//length関数のテスト
	TEST(SVector2Func, Length)
	{
		//sample1
		SVector2 vec(3.0f, 4.0f);
		EXPECT_FLOAT_EQ(5.0f, vec.length());

		//sample2
		SVector2 vec2(1.0f, 1.0f);
		EXPECT_FLOAT_EQ(1.41421356f, vec2.length());

		//sample3
		SVector2 vec3(0.0f, 0.0f);
		EXPECT_FLOAT_EQ(0.0f, vec3.length());

		//sample4
		SVector2 vec4(-1.0f, -1.0f);
		EXPECT_FLOAT_EQ(1.41421356f, vec4.length());

		//sample5
		SVector2 vec5(-3.0f, -4.0f);
		EXPECT_FLOAT_EQ(5.0f, vec5.length());

		//sample6
		SVector2 vec6(3.0f, -4.0f);
		EXPECT_FLOAT_EQ(5.0f, vec6.length());
	}

	//lengthSquare関数のテスト
	TEST(SVector2Func, LengthSquare)
	{
		//sample1
		SVector2 vec(3.0f, 4.0f);
		EXPECT_FLOAT_EQ(25.0f, vec.lengthSquare());

		//sample2
		SVector2 vec2(1.0f, 1.0f);
		EXPECT_FLOAT_EQ(2.0f, vec2.lengthSquare());

		//sample3
		SVector2 vec3(0.0f, 0.0f);
		EXPECT_FLOAT_EQ(0.0f, vec3.lengthSquare());

		//sample4
		SVector2 vec4(-1.0f, -1.0f);
		EXPECT_FLOAT_EQ(2.0f, vec4.lengthSquare());

		//sample5
		SVector2 vec5(-3.0f, -4.0f);
		EXPECT_FLOAT_EQ(25.0f, vec5.lengthSquare());

		//sample6
		SVector2 vec6(3.0f, -4.0f);
		EXPECT_FLOAT_EQ(25.0f, vec6.lengthSquare());
	}

	//dot関数のテスト
	TEST(SVector2Func, Dot)
	{
		//sample1
		SVector2 vec(3.0f, 4.0f);
		SVector2 vec2(1.0f, 1.0f);
		EXPECT_FLOAT_EQ(7.0f, vec.dot(vec2));

		//sample2
		SVector2 vec3(1.0f, 1.0f);
		SVector2 vec4(1.0f, 1.0f);
		EXPECT_FLOAT_EQ(2.0f, vec3.dot(vec4));

		//sample3
		SVector2 vec5(0.0f, 0.0f);
		SVector2 vec6(0.0f, 0.0f);
		EXPECT_FLOAT_EQ(0.0f, vec5.dot(vec6));

		//sample4
		SVector2 vec7(-1.0f, -1.0f);
		SVector2 vec8(-1.0f, -1.0f);
		EXPECT_FLOAT_EQ(2.0f, vec7.dot(vec8));

		//sample5
		SVector2 vec9(-3.0f, -4.0f);
		SVector2 vec10(-3.0f, -4.0f);
		EXPECT_FLOAT_EQ(25.0f, vec9.dot(vec10));

		//sample6
		SVector2 vec11(3.0f, -4.0f);
		SVector2 vec12(3.0f, -4.0f);
		EXPECT_FLOAT_EQ(25.0f, vec11.dot(vec12));
	}

//cross関数のテスト
	TEST(SVector2Func, Cross)
	{
		//sample1
		SVector2 vec(3.0f, 4.0f);
		SVector2 vec2(1.0f, 1.0f);
		EXPECT_FLOAT_EQ(-1.0f, vec.cross(vec2));

		//sample2
		SVector2 vec3(1.0f, 1.0f);
		SVector2 vec4(1.0f, 1.0f);
		EXPECT_FLOAT_EQ(0.0f, vec3.cross(vec4));

		//sample3
		SVector2 vec5(0.0f, 0.0f);
		SVector2 vec6(0.0f, 0.0f);
		EXPECT_FLOAT_EQ(0.0f, vec5.cross(vec6));

		//sample4
		SVector2 vec7(-1.0f, -1.0f);
		SVector2 vec8(-1.0f, -1.0f);
		EXPECT_FLOAT_EQ(0.0f, vec7.cross(vec8));

		//sample5
		SVector2 vec9(-3.0f, -4.0f);
		SVector2 vec10(-3.0f, -4.0f);
		EXPECT_FLOAT_EQ(0.0f, vec9.cross(vec10));

		//sample6
		SVector2 vec11(3.0f, -4.0f);
		SVector2 vec12(3.0f, -4.0f);
		EXPECT_FLOAT_EQ(0.0f, vec11.cross(vec12));
	}

	//distanceFrom関数のテスト
	TEST(SVector2Func, DistanceFrom)
	{
		//sample1
		SVector2 vec(3.0f, 4.0f);
		SVector2 vec2(1.0f, 1.0f);
		EXPECT_FLOAT_EQ(3.60555128f, vec.distanceFrom(vec2));

		//sample2
		SVector2 vec3(1.0f, 1.0f);
		SVector2 vec4(1.0f, 1.0f);
		EXPECT_FLOAT_EQ(0.0f, vec3.distanceFrom(vec4));

		//sample3
		SVector2 vec5(0.0f, 0.0f);
		SVector2 vec6(0.0f, 0.0f);
		EXPECT_FLOAT_EQ(0.0f, vec5.distanceFrom(vec6));

		//sample4
		SVector2 vec7(-1.0f, -1.0f);
		SVector2 vec8(-1.0f, -1.0f);
		EXPECT_FLOAT_EQ(0.0f, vec7.distanceFrom(vec8));

		//sample5 - 大きな値の計算
		SVector2 vec9(30.0f, 20.0f);
		SVector2 vec10(-5.0f, -10.0f);
		EXPECT_FLOAT_EQ(46.09772228f, vec9.distanceFrom(vec10));

		//sample6 - 浮動小数の計算
		SVector2 vec11(0.3f, 0.2f);
		SVector2 vec12(-0.5f, -0.1f);
		EXPECT_FLOAT_EQ(0.854400374f, vec11.distanceFrom(vec12));
	}

	//normalized関数のテスト
	TEST(SVector2Func, Normalized)
	{
		//sample1
		SVector2 vec(3.0f, 4.0f);
		SVector2 res = vec.normalized();
		EXPECT_FLOAT_EQ(0.6f, res.x);
		EXPECT_FLOAT_EQ(0.8f, res.y);

		//sample2
		SVector2 vec2(1.0f, 1.0f);
		SVector2 res2 = vec2.normalized();
		EXPECT_FLOAT_EQ(0.707106781f, res2.x);
		EXPECT_FLOAT_EQ(0.707106781f, res2.y);

		//sample3　速さを重視しているので(0,0)を考慮していない
		//SVector2 vec3(0.0f, 0.0f);
		//SVector2 res3 = vec3.normalized();
		//EXPECT_FLOAT_EQ(0.0f, res3.x);
		//EXPECT_FLOAT_EQ(0.0f, res3.y);

		//sample4
		SVector2 vec4(-1.0f, -1.0f);
		SVector2 res4 = vec4.normalized();
		EXPECT_FLOAT_EQ(-0.707106781f, res4.x);
		EXPECT_FLOAT_EQ(-0.707106781f, res4.y);

		//sample5
		SVector2 vec5(-3.0f, -4.0f);
		SVector2 res5 = vec5.normalized();
		EXPECT_FLOAT_EQ(-0.6f, res5.x);
		EXPECT_FLOAT_EQ(-0.8f, res5.y);

		//sample6
		SVector2 vec6(3.0f, -4.0f);
		SVector2 res6 = vec6.normalized();
		EXPECT_FLOAT_EQ(0.6f, res6.x);
		EXPECT_FLOAT_EQ(-0.8f, res6.y);
	}

	//isZero関数のテスト
	TEST(SVector2Func, IsZero)
	{
		//sample1
		SVector2 vec(3.0f, 4.0f);
		EXPECT_FALSE(vec.isZero());

		//sample2
		SVector2 vec2(1.0f, 1.0f);
		EXPECT_FALSE(vec2.isZero());

		//sample3
		SVector2 vec3(0.0f, 0.0f);
		EXPECT_TRUE(vec3.isZero());

		//sample4
		SVector2 vec4(-1.0f, -1.0f);
		EXPECT_FALSE(vec4.isZero());

		//sample5
		SVector2 vec5(-3.0f, -4.0f);
		EXPECT_FALSE(vec5.isZero());

		//sample6
		SVector2 vec6(3.0f, -4.0f);
		EXPECT_FALSE(vec6.isZero());
	}
}