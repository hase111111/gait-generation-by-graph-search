#include "pch.h"
#include "../DesignLab/MyVector.h"
#include "../DesignLab/MyVector.cpp"

using namespace dl_vec;

namespace SVectorTest
{
	TEST(SVectorFunc, LengthSquare)
	{
		SVector _v;

		_v = { 0,0,0 };
		EXPECT_EQ(_v.lengthSquare(), 0);

		_v = { 1,1,1 };
		EXPECT_EQ(_v.lengthSquare(), 3);

		_v = { 1,1,-1 };
		EXPECT_EQ(_v.lengthSquare(), 3);

		_v = { -1,-1,-1 };
		EXPECT_EQ(_v.lengthSquare(), 3);

		_v = { 0, 5, -12 };
		EXPECT_EQ(_v.lengthSquare(), 169);

		_v = { 4, -10, 5 };
		EXPECT_EQ(_v.lengthSquare(), 141);

		_v = { 100, -80, -35 };
		EXPECT_TRUE(dl_math::isEqual(_v.lengthSquare(), 17625.0f));

		_v = { 0.1f, -0.7f, 3.5f };
		EXPECT_TRUE(dl_math::isEqual(_v.lengthSquare(), 12.75f));
	}

	TEST(SVectorFunc, Length)
	{
		SVector _v;

		_v = { 0,0,0 };
		EXPECT_EQ(_v.length(), 0);

		_v = { 1, 1, 1 };
		EXPECT_FLOAT_EQ(_v.length(), 1.7320508f);

		_v = { -1, -1, -1 };
		EXPECT_FLOAT_EQ(_v.length(), 1.7320508f);

		_v = { -1, 1, -1 };
		EXPECT_FLOAT_EQ(_v.length(), 1.7320508f);

		_v = { 4, -10, 5 };
		EXPECT_TRUE(dl_math::isEqual(_v.length(), 11.8743f));

		_v = { 0, 5, -12 };
		EXPECT_TRUE(dl_math::isEqual(_v.length(), 13.0f));

		_v = { 6.7f, -33.9f, 29.5f };
		EXPECT_TRUE(dl_math::isEqual(_v.length(), 45.4351f));
	}

	TEST(SVectorFunc, Normalized)
	{
		SVector _v;

		_v = { 500, 0, 0 };
		EXPECT_EQ(_v.normalized(), SVector(1, 0, 0));

		_v = { 54.10f, 0, 0 };
		EXPECT_EQ(_v.normalized(), SVector(1, 0, 0));

		_v = { 1, 0, 0 };
		EXPECT_EQ(_v.normalized(), SVector(1, 0, 0));

		_v = { -23445, 0, 0 };
		EXPECT_EQ(_v.normalized(), SVector(-1, 0, 0));

		_v = { 5.4f, 5.4f, 5.4f };
		EXPECT_EQ(_v.normalized(), SVector(0.57735f, 0.57735f, 0.57735f));

		_v = { -94,-94,-94 };
		EXPECT_EQ(_v.normalized(), SVector(-0.57735f, -0.57735f, -0.57735f));
		EXPECT_TRUE(dl_math::isEqual(_v.normalized().length(), 1));

		_v = { 17, 17, -17 };
		EXPECT_EQ(_v.normalized(), SVector(0.57735f, 0.57735f, -0.57735f));
		EXPECT_TRUE(dl_math::isEqual(_v.normalized().length(), 1));

		_v = { 3, 2, -1 };
		EXPECT_EQ(_v.normalized(), SVector(0.801784f, 0.534522f, -0.267261f));
		EXPECT_TRUE(dl_math::isEqual(_v.normalized().length(), 1));

		_v = { 62.51f, -94.78f, 25.89f };
		EXPECT_EQ(_v.normalized(), SVector(0.536788f, -0.813898f, 0.222324f));
		EXPECT_TRUE(dl_math::isEqual(_v.normalized().length(), 1));
	}

	TEST(SVectorFunc, IsZero)
	{
		SVector _v;

		_v = { 0,0,0 };
		EXPECT_TRUE(_v.isZero());

		_v = { -10, 0, 0 };
		EXPECT_FALSE(_v.isZero());

		_v = { 512, 0, 0 };
		EXPECT_FALSE(_v.isZero());

		_v = { 0, 45, 0 };
		EXPECT_FALSE(_v.isZero());

		_v = { 0, -3, 0 };
		EXPECT_FALSE(_v.isZero());

		_v = { 0, 0, 0.5f };
		EXPECT_FALSE(_v.isZero());

		_v = { 0, 0, -0.2f };
		EXPECT_FALSE(_v.isZero());

		_v = { 0, 100, -35 };
		EXPECT_FALSE(_v.isZero());

		_v = { 67, 0, 26.8f };
		EXPECT_FALSE(_v.isZero());

		_v = { -71, 0.7f, 0 };
		EXPECT_FALSE(_v.isZero());

		_v = { 24, 67, 89 };
		EXPECT_FALSE(_v.isZero());
	}

	TEST(SVectorFunc, DistanceFrom)
	{
		SVector _v1, _v2;

		_v1 = { 0,0,0 }; _v2 = { 0,0,0 };
		EXPECT_FLOAT_EQ(_v1.distanceFrom(_v2), 0);

		_v1 = { 0,0,0 }; _v2 = { 8,7,-10 };
		EXPECT_FLOAT_EQ(_v1.distanceFrom(_v2), _v2.length());

		_v1 = { 0,0,0 }; _v2 = { 16,-30,100 };
		EXPECT_FLOAT_EQ(_v1.distanceFrom(_v2), _v2.length());

		_v1 = { 0,0,0 }; _v2 = { -4.6f, 0.4f, 34.6f };
		EXPECT_FLOAT_EQ(_v1.distanceFrom(_v2), _v2.length());
	}

}