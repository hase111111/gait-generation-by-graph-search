#include "pch.h"

#include "../DesignLab/math_euler.h"


namespace dl = ::designlab;
namespace dlm = ::designlab::math_util;


namespace designlab::test::common::math
{
	//代入演算子 = AssignmentOperator

	TEST(EulerXYZTest, AssignmentOperatorTest)
	{
		const dl::EulerXYZ euler(1.f, 2.f, 3.f);
		dl::EulerXYZ euler_copy;
		euler_copy = euler;

		EXPECT_FLOAT_EQ(euler_copy.x_angle, 1.f);
		EXPECT_FLOAT_EQ(euler_copy.y_angle, 2.f);
		EXPECT_FLOAT_EQ(euler_copy.z_angle, 3.f);
	}

	//等号演算子 == EqualityOperator

	TEST(EulerXYZTest, EqualityOperatorTest)
	{
		const dl::EulerXYZ euler(1.f, 2.f, 3.f);
		const dl::EulerXYZ euler_copy = euler;

		EXPECT_TRUE(euler == euler_copy);
	}

	TEST(EulerXYZTest, InequalityOperatorTest)
	{
		const dl::EulerXYZ euler(1.f, 2.f, 3.f);
		const dl::EulerXYZ euler_copy = euler;

		EXPECT_FALSE(euler != euler_copy);
	}

	//乗算演算子 * MultiplicationOperator

	TEST(EulerXYZTest, MultiplicationOperatorTest)
	{
		const dl::EulerXYZ euler(1.f, 2.f, 3.f);
		const dl::EulerXYZ euler_multiplied = euler * 2.f;

		EXPECT_FLOAT_EQ(euler_multiplied.x_angle, 2.f);
		EXPECT_FLOAT_EQ(euler_multiplied.y_angle, 4.f);
		EXPECT_FLOAT_EQ(euler_multiplied.z_angle, 6.f);
	}
}