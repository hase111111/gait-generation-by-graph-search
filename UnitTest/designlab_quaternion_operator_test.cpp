#include "pch.h"

#include "../Designlab/math_quaternion.h"


namespace designlab::test::common::math
{
	// 代入演算子 = AssignmentOperator 
	TEST(QuaternionTest, AssignmentOperatorTest)
	{
		const Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
		const Quaternion q2 = q;
		EXPECT_EQ(q2.w, 1.0f);
		EXPECT_EQ(q2.v.x, 2.0f);
		EXPECT_EQ(q2.v.y, 3.0f);
		EXPECT_EQ(q2.v.z, 4.0f);
	}

	// 単項演算子 = UnaryOperator
	TEST(QuaternionTest, UnaryPulsOperatorTest)
	{
		const Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
		const Quaternion q2 = +q;
		EXPECT_EQ(q2.w, 1.0f);
		EXPECT_EQ(q2.v.x, 2.0f);
		EXPECT_EQ(q2.v.y, 3.0f);
		EXPECT_EQ(q2.v.z, 4.0f);
	}

	TEST(QuaternionTest, UnaryMinusOperatorTest)
	{
		const Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
		const Quaternion q2 = -q;
		EXPECT_EQ(q2.w, -1.0f);
		EXPECT_EQ(q2.v.x, -2.0f);
		EXPECT_EQ(q2.v.y, -3.0f);
		EXPECT_EQ(q2.v.z, -4.0f);
	}
}