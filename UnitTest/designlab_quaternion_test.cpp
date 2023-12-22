#include "pch.h"

#include "../Designlab/math_quaternion.h"
#include "../Designlab/designlab_quaternion.cpp"


namespace designlab::test::common::math
{
	TEST(QuaternionTest, DefaultConstructorTest)
	{
		const Quaternion q;
		EXPECT_EQ(q.w, 1.0f);
		EXPECT_EQ(q.v.x, 0.0f);
		EXPECT_EQ(q.v.y, 0.0f);
		EXPECT_EQ(q.v.z, 0.0f);
	}

	TEST(QuaternionTest, ConstructorTest)
	{
		const Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
		EXPECT_EQ(q.w, 1.0f);
		EXPECT_EQ(q.v.x, 2.0f);
		EXPECT_EQ(q.v.y, 3.0f);
		EXPECT_EQ(q.v.z, 4.0f);
	}

	TEST(QuaternionTest, CopyConstructorTest)
	{
		const Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
		Quaternion q2(q);
		EXPECT_EQ(q2.w, 1.0f);
		EXPECT_EQ(q2.v.x, 2.0f);
		EXPECT_EQ(q2.v.y, 3.0f);
		EXPECT_EQ(q2.v.z, 4.0f);
	}

	//move constructor
	TEST(QuaternionTest, MoveConstructorTest)
	{
		Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
		const Quaternion q2(std::move(q));
		EXPECT_EQ(q2.w, 1.0f);
		EXPECT_EQ(q2.v.x, 2.0f);
		EXPECT_EQ(q2.v.y, 3.0f);
		EXPECT_EQ(q2.v.z, 4.0f);
	}

} // namespace designlab::test::common::math