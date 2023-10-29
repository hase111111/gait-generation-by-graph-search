#include "pch.h"

#include "../DesignLab/designlab_euler.h"
#include "../DesignLab/designlab_euler.cpp"


namespace dl = ::designlab;


namespace designlab::test::common::math
{
	TEST(EulerXYZTest, DefaultConstructorTest)
	{
		const dl::EulerXYZ euler;

		EXPECT_FLOAT_EQ(euler.x_angle, 0.f);
		EXPECT_FLOAT_EQ(euler.y_angle, 0.f);
		EXPECT_FLOAT_EQ(euler.z_angle, 0.f);
	}

	TEST(EulerXYZTest, ConstructorTest)
	{
		const dl::EulerXYZ euler(1.f, 2.f, 3.f);

		EXPECT_FLOAT_EQ(euler.x_angle, 1.f);
		EXPECT_FLOAT_EQ(euler.y_angle, 2.f);
		EXPECT_FLOAT_EQ(euler.z_angle, 3.f);
	}

	TEST(EulerXYZTest, CopyConstructorTest)
	{
		const dl::EulerXYZ euler(1.f, 2.f, 3.f);
		const dl::EulerXYZ euler_copy = dl::EulerXYZ(euler);

		EXPECT_FLOAT_EQ(euler_copy.x_angle, 1.f);
		EXPECT_FLOAT_EQ(euler_copy.y_angle, 2.f);
		EXPECT_FLOAT_EQ(euler_copy.z_angle, 3.f);
	}

	TEST(EulerXYZTest, MoveConstructorTest)
	{
		dl::EulerXYZ euler = dl::EulerXYZ(1.f, 2.f, 3.f);
		const dl::EulerXYZ euler_move = dl::EulerXYZ(std::move(euler));

		EXPECT_FLOAT_EQ(euler_move.x_angle, 1.f);
		EXPECT_FLOAT_EQ(euler_move.y_angle, 2.f);
		EXPECT_FLOAT_EQ(euler_move.z_angle, 3.f);
	}

}