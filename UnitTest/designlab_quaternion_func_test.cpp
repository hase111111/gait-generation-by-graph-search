#include "pch.h"

#include "../Designlab/designlab_quaternion.h"


namespace designlab::test::common::math
{
	TEST(QuaternionTest, RotateVector3Test)
	{
		const Quaternion q = Quaternion(1.0f, 0.f, 0.f, 0.f);
		const Vector3 vec = { 100.f, 120.f, 0.f };

		const Vector3 actual = RotateVector3(vec, q, true);

		EXPECT_EQ(vec, actual);
	}
}