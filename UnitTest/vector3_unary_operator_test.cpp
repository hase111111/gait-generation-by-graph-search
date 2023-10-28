#include "pch.h"

#include "../DesignLab/designlab_vector3.h"


namespace dl = ::designlab;


namespace
{
	// テスト用のヘルパー関数

	//! @brief 与えられた値からVector3を作成する
	dl::Vector3 MakeVec3(const float x, const  float y, const float z)
	{
		dl::Vector3 vec;
		vec.x = x;
		vec.y = y;
		vec.z = z;

		return vec;
	}
}


namespace designlab::test::common::math
{
	// 単項演算子 = Unary Operator

	TEST(Vector3Test, UnaryPlusOperatorTest)
	{
		dl::Vector3 v1 = MakeVec3(1.0f, 2.0f, 0.5f);
		v1 = +v1;
		EXPECT_FLOAT_EQ(v1.x, 1.0f);
		EXPECT_FLOAT_EQ(v1.y, 2.0f);
		EXPECT_FLOAT_EQ(v1.z, 0.5f);

		dl::Vector3 v2 = MakeVec3(-1.0f, -2.0f, 0.0f);
		v2 = +v2;
		EXPECT_FLOAT_EQ(v2.x, -1.0f);
		EXPECT_FLOAT_EQ(v2.y, -2.0f);
		EXPECT_FLOAT_EQ(v2.z, 0.0f);
	}

	TEST(Vector3Test, UnaryMinusOperatorTest)
	{
		designlab::Vector3 v1 = MakeVec3(1.0f, 2.0f, 0.7f);
		v1 = -v1;
		EXPECT_FLOAT_EQ(v1.x, -1.0f);
		EXPECT_FLOAT_EQ(v1.y, -2.0f);
		EXPECT_FLOAT_EQ(v1.z, -0.7f);

		designlab::Vector3 v2 = MakeVec3(0.0f, -2.0f, -3.4f);
		v2 = -v2;
		EXPECT_FLOAT_EQ(v2.x, 0.0f);
		EXPECT_FLOAT_EQ(v2.y, 2.0f);
		EXPECT_FLOAT_EQ(v2.z, 3.4f);
	}
}