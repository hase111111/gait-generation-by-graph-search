#include "pch.h"

#include "../DesignLab/designlab_vector2.h"


namespace designlab::test::common::math
{
	// íPçÄââéZéq = Unary Operator

	TEST(Vector2Test, UnaryPlusOperator)
	{
		designlab::Vector2 v1(1.0f, 2.0f);
		v1 = +v1;
		EXPECT_FLOAT_EQ(v1.x, 1.0f);
		EXPECT_FLOAT_EQ(v1.y, 2.0f);

		designlab::Vector2 v2(-1.0f, -2.0f);
		v2 = +v2;
		EXPECT_FLOAT_EQ(v2.x, -1.0f);
		EXPECT_FLOAT_EQ(v2.y, -2.0f);
	}

	TEST(Vector2Test, UnaryMinusOperator)
	{
		designlab::Vector2 v1(1.0f, 2.0f);
		v1 = -v1;
		EXPECT_FLOAT_EQ(v1.x, -1.0f);
		EXPECT_FLOAT_EQ(v1.y, -2.0f);

		designlab::Vector2 v2(0.0f, -2.0f);
		v2 = -v2;
		EXPECT_FLOAT_EQ(v2.x, 0.0f);
		EXPECT_FLOAT_EQ(v2.y, 2.0f);
	}
}