#include "pch.h"

#include "../DesignLab/math_vector2.h"


namespace dl = ::designlab;


namespace
{
	// テスト用のヘルパー関数

	//! @brief 与えられた値からVector3を作成する
	dl::Vector2 MakeVec2(const float x, const float y)
	{
		dl::Vector2 vec;
		vec.x = x;
		vec.y = y;

		return vec;
	}
}


namespace designlab::test::common::math
{
	// 単項演算子 = Unary Operator

	TEST(Vector2Test, UnaryPlusOperator)
	{
		designlab::Vector2 v1 = MakeVec2(1.0f, 2.0f);
		v1 = +v1;
		EXPECT_FLOAT_EQ(v1.x, 1.0f);
		EXPECT_FLOAT_EQ(v1.y, 2.0f);

		designlab::Vector2 v2 = MakeVec2(-1.0f, -2.0f);
		v2 = +v2;
		EXPECT_FLOAT_EQ(v2.x, -1.0f);
		EXPECT_FLOAT_EQ(v2.y, -2.0f);
	}

	TEST(Vector2Test, UnaryMinusOperator)
	{
		designlab::Vector2 v1 = MakeVec2(1.0f, 2.0f);
		v1 = -v1;
		EXPECT_FLOAT_EQ(v1.x, -1.0f);
		EXPECT_FLOAT_EQ(v1.y, -2.0f);

		designlab::Vector2 v2 = MakeVec2(0.0f, -2.0f);
		v2 = -v2;
		EXPECT_FLOAT_EQ(v2.x, 0.0f);
		EXPECT_FLOAT_EQ(v2.y, 2.0f);
	}
}