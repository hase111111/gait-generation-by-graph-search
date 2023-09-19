#include "pch.h"

#include "../DesignLab/designlab_vector3.h"
#include "../DesignLab/designlab_vector.cpp"


namespace dl_vec_test
{
	TEST(Vector3, LengthSquare)
	{
		designlab::Vector3 vec;

		vec = { 0,0,0 };
		EXPECT_EQ(vec.lengthSquare(), 0);

		vec = { 1,1,1 };
		EXPECT_EQ(vec.lengthSquare(), 3);

		vec = { 1,1,-1 };
		EXPECT_EQ(vec.lengthSquare(), 3);

		vec = { -1,-1,-1 };
		EXPECT_EQ(vec.lengthSquare(), 3);

		vec = { 0, 5, -12 };
		EXPECT_EQ(vec.lengthSquare(), 169);

		vec = { 4, -10, 5 };
		EXPECT_EQ(vec.lengthSquare(), 141);

		vec = { 100, -80, -35 };
		EXPECT_TRUE(dl_math::isEqual(vec.lengthSquare(), 17625.0f));

		vec = { 0.1f, -0.7f, 3.5f };
		EXPECT_TRUE(dl_math::isEqual(vec.lengthSquare(), 12.75f));
	}


	TEST(Vector3, Length)
	{
		designlab::Vector3 vec;

		vec = { 0,0,0 };
		EXPECT_EQ(vec.length(), 0);

		vec = { 1, 1, 1 };
		EXPECT_FLOAT_EQ(vec.length(), 1.7320508f);

		vec = { -1, -1, -1 };
		EXPECT_FLOAT_EQ(vec.length(), 1.7320508f);

		vec = { -1, 1, -1 };
		EXPECT_FLOAT_EQ(vec.length(), 1.7320508f);

		vec = { 4, -10, 5 };
		EXPECT_TRUE(dl_math::isEqual(vec.length(), 11.8743f));

		vec = { 0, 5, -12 };
		EXPECT_TRUE(dl_math::isEqual(vec.length(), 13.0f));

		vec = { 6.7f, -33.9f, 29.5f };
		EXPECT_TRUE(dl_math::isEqual(vec.length(), 45.4351f));
	}


	TEST(Vector3, Normalized)
	{
		designlab::Vector3 vec;

		vec = { 500, 0, 0 };
		EXPECT_EQ(vec.normalized(), designlab::Vector3(1, 0, 0));

		vec = { 54.10f, 0, 0 };
		EXPECT_EQ(vec.normalized(), designlab::Vector3(1, 0, 0));

		vec = { 1, 0, 0 };
		EXPECT_EQ(vec.normalized(), designlab::Vector3(1, 0, 0));

		vec = { -23445, 0, 0 };
		EXPECT_EQ(vec.normalized(), designlab::Vector3(-1, 0, 0));

		vec = { 5.4f, 5.4f, 5.4f };
		EXPECT_EQ(vec.normalized(), designlab::Vector3(0.57735f, 0.57735f, 0.57735f));

		vec = { -94,-94,-94 };
		EXPECT_EQ(vec.normalized(), designlab::Vector3(-0.57735f, -0.57735f, -0.57735f));
		EXPECT_TRUE(dl_math::isEqual(vec.normalized().length(), 1));

		vec = { 17, 17, -17 };
		EXPECT_EQ(vec.normalized(), designlab::Vector3(0.57735f, 0.57735f, -0.57735f));
		EXPECT_TRUE(dl_math::isEqual(vec.normalized().length(), 1));

		vec = { 3, 2, -1 };
		EXPECT_EQ(vec.normalized(), designlab::Vector3(0.801784f, 0.534522f, -0.267261f));
		EXPECT_TRUE(dl_math::isEqual(vec.normalized().length(), 1));

		vec = { 62.51f, -94.78f, 25.89f };
		EXPECT_EQ(vec.normalized(), designlab::Vector3(0.536788f, -0.813898f, 0.222324f));
		EXPECT_TRUE(dl_math::isEqual(vec.normalized().length(), 1));
	}

	TEST(Vector3, IsZero)
	{
		designlab::Vector3 vec;

		vec = { 0,0,0 };
		EXPECT_TRUE(vec.isZero());

		vec = { -10, 0, 0 };
		EXPECT_FALSE(vec.isZero());

		vec = { 512, 0, 0 };
		EXPECT_FALSE(vec.isZero());

		vec = { 0, 45, 0 };
		EXPECT_FALSE(vec.isZero());

		vec = { 0, -3, 0 };
		EXPECT_FALSE(vec.isZero());

		vec = { 0, 0, 0.5f };
		EXPECT_FALSE(vec.isZero());

		vec = { 0, 0, -0.2f };
		EXPECT_FALSE(vec.isZero());

		vec = { 0, 100, -35 };
		EXPECT_FALSE(vec.isZero());

		vec = { 67, 0, 26.8f };
		EXPECT_FALSE(vec.isZero());

		vec = { -71, 0.7f, 0 };
		EXPECT_FALSE(vec.isZero());

		vec = { 24, 67, 89 };
		EXPECT_FALSE(vec.isZero());
	}

	TEST(Vector3, DistanceFrom)
	{
		designlab::Vector3 vec1, vec2;

		vec1 = { 0,0,0 }; vec2 = { 0,0,0 };
		EXPECT_FLOAT_EQ(vec1.distanceFrom(vec2), 0);

		vec1 = { 0,0,0 }; vec2 = { 8,7,-10 };
		EXPECT_FLOAT_EQ(vec1.distanceFrom(vec2), vec2.length());

		vec1 = { 0,0,0 }; vec2 = { 16,-30,100 };
		EXPECT_FLOAT_EQ(vec1.distanceFrom(vec2), vec2.length());

		vec1 = { 0,0,0 }; vec2 = { -4.6f, 0.4f, 34.6f };
		EXPECT_FLOAT_EQ(vec1.distanceFrom(vec2), vec2.length());
	}

}