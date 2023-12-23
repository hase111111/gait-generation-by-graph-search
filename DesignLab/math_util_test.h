#pragma once

#include <doctest.h>

#include "math_util.h"


TEST_SUITE("namespace math_util")
{
	using namespace designlab;

	TEST_CASE_TEMPLATE("IsEqual，値が等しい時，trueを返すべき", T, float, double)
	{
		CHECK(math_util::IsEqual(T(0.0), T(0.0)));
		CHECK(math_util::IsEqual(T(1.0), T(1.0)));
		CHECK(math_util::IsEqual(T(-1.0), T(-1.0)));
		CHECK(math_util::IsEqual(T(325.1), T(325.1)));

		SUBCASE("誤差の範囲内で等しい時，trueを返すべき")
		{
			CHECK(math_util::IsEqual(T(0.0), T(0.0) + MathConst<T>::kAllowableError / 2));
			CHECK(math_util::IsEqual(T(0.0), T(0.0) - MathConst<T>::kAllowableError / 2));
			CHECK(math_util::IsEqual(T(1.0), T(1.0) + MathConst<T>::kAllowableError / 2));
			CHECK(math_util::IsEqual(T(1.0), T(1.0) - MathConst<T>::kAllowableError / 2));
			CHECK(math_util::IsEqual(T(-1.0), T(-1.0) + MathConst<T>::kAllowableError / 2));
			CHECK(math_util::IsEqual(T(-1.0), T(-1.0) - MathConst<T>::kAllowableError / 2));
		}
	}

	TEST_CASE_TEMPLATE("IsEqual，値が等しくない時，falseを返すべき", T, float, double)
	{
		CHECK_FALSE(math_util::IsEqual(T(0.0), T(1.0)));
		CHECK_FALSE(math_util::IsEqual(T(1.0), T(0.0)));
		CHECK_FALSE(math_util::IsEqual(T(-1.0), T(1.0)));
		CHECK_FALSE(math_util::IsEqual(T(325.1), T(325.2)));

		SUBCASE("誤差の範囲を上回っている時，falseを返すべき")
		{
			CHECK_FALSE(math_util::IsEqual(T(0.0), T(0.0) + MathConst<T>::kAllowableError * 2));
			CHECK_FALSE(math_util::IsEqual(T(0.0), T(0.0) - MathConst<T>::kAllowableError * 2));
			CHECK_FALSE(math_util::IsEqual(T(1.0), T(1.0) + MathConst<T>::kAllowableError * 2));
			CHECK_FALSE(math_util::IsEqual(T(1.0), T(1.0) - MathConst<T>::kAllowableError * 2));
			CHECK_FALSE(math_util::IsEqual(T(-1.0), T(-1.0) + MathConst<T>::kAllowableError * 2));
			CHECK_FALSE(math_util::IsEqual(T(-1.0), T(-1.0) - MathConst<T>::kAllowableError * 2));
		}
	}

	TEST_CASE_TEMPLATE("Squared，値の2乗を返すべき", T, int, float, double)
	{
		CHECK(math_util::Squared(T(0.0)) == T(0.0));
		CHECK(math_util::Squared(T(1.0)) == T(1.0));
		CHECK(math_util::Squared(T(-1.0)) == T(1.0));
		CHECK(math_util::Squared(T(5)) == T(25));
		CHECK(math_util::Squared(T(-5)) == T(25));
		CHECK(math_util::Squared(T(15)) == T(225));
		CHECK(math_util::Squared(T(1462)) == T(2137444));
	}

	TEST_CASE_TEMPLATE("CanMakeTriangle，3角形を作ることが可能な時，trueを返すべき", T, int, float, double)
	{
		CHECK(math_util::CanMakeTriangle(T(1.0), T(1.0), T(1.0)));
		CHECK(math_util::CanMakeTriangle(T(2.0), T(2.0), T(3.0)));
		CHECK(math_util::CanMakeTriangle(T(2.0), T(3.0), T(2.0)));
		CHECK(math_util::CanMakeTriangle(T(3.0), T(2.0), T(2.0)));
		CHECK(math_util::CanMakeTriangle(T(3.0), T(4.0), T(5.0)));
		CHECK(math_util::CanMakeTriangle(T(3.0), T(5.0), T(4.0)));
		CHECK(math_util::CanMakeTriangle(T(4.0), T(3.0), T(5.0)));
		CHECK(math_util::CanMakeTriangle(T(4.0), T(5.0), T(3.0)));
		CHECK(math_util::CanMakeTriangle(T(5.0), T(3.0), T(4.0)));
		CHECK(math_util::CanMakeTriangle(T(5.0), T(4.0), T(3.0)));
	}

	TEST_CASE_TEMPLATE("CanMakeTriangle，3角形を作ることが不可能な時，falseを返すべき", T, int, float, double)
	{
		CHECK_FALSE(math_util::CanMakeTriangle(T(1.0), T(1.0), T(3.0)));
		CHECK_FALSE(math_util::CanMakeTriangle(T(1.0), T(3.0), T(1.0)));
		CHECK_FALSE(math_util::CanMakeTriangle(T(3.0), T(1.0), T(1.0)));
		CHECK_FALSE(math_util::CanMakeTriangle(T(1.0), T(2.0), T(3.0)));
		CHECK_FALSE(math_util::CanMakeTriangle(T(1.0), T(3.0), T(2.0)));
		CHECK_FALSE(math_util::CanMakeTriangle(T(2.0), T(1.0), T(3.0)));
		CHECK_FALSE(math_util::CanMakeTriangle(T(2.0), T(3.0), T(1.0)));
		CHECK_FALSE(math_util::CanMakeTriangle(T(3.0), T(1.0), T(2.0)));
		CHECK_FALSE(math_util::CanMakeTriangle(T(3.0), T(2.0), T(1.0)));
		CHECK_FALSE(math_util::CanMakeTriangle(T(1.0), T(1.0), T(2.0)));
		CHECK_FALSE(math_util::CanMakeTriangle(T(1.0), T(2.0), T(1.0)));
		CHECK_FALSE(math_util::CanMakeTriangle(T(2.0), T(1.0), T(1.0)));
		CHECK_FALSE(math_util::CanMakeTriangle(T(1.0), T(1.0), T(4.0)));
		CHECK_FALSE(math_util::CanMakeTriangle(T(1.0), T(4.0), T(1.0)));
		CHECK_FALSE(math_util::CanMakeTriangle(T(4.0), T(1.0), T(1.0)));
		CHECK_FALSE(math_util::CanMakeTriangle(T(1.0), T(2.0), T(4.0)));
		CHECK_FALSE(math_util::CanMakeTriangle(T(1.0), T(4.0), T(2.0)));
		CHECK_FALSE(math_util::CanMakeTriangle(T(2.0), T(1.0), T(4.0)));
	}

	TEST_CASE_TEMPLATE("ConvertRadToDeg", T, float, double)
	{
		CHECK(math_util::ConvertRadToDeg(T(0.0)) == T(0.0));
		CHECK(math_util::ConvertRadToDeg(T(1.0)) == doctest::Approx(T(57.2957795)));
		CHECK(math_util::ConvertRadToDeg(T(-1.0)) == doctest::Approx(T(-57.2957795)));
		CHECK(math_util::ConvertRadToDeg(T(3.1415926)) == doctest::Approx(T(180.0)));
		CHECK(math_util::ConvertRadToDeg(T(-3.141592)) == doctest::Approx(T(-180.0)));
		CHECK(math_util::ConvertRadToDeg(T(6.2831853)) == doctest::Approx(T(360.0)));
		CHECK(math_util::ConvertRadToDeg(T(-6.283185)) == doctest::Approx(T(-360.0)));
		CHECK(math_util::ConvertRadToDeg(T(0.5)) == doctest::Approx(T(28.647889)));
		CHECK(math_util::ConvertRadToDeg(T(-0.5)) == doctest::Approx(T(-28.647889)));
		CHECK(math_util::ConvertRadToDeg(T(0.25)) == doctest::Approx(T(14.323944)));
		CHECK(math_util::ConvertRadToDeg(T(-0.25)) == doctest::Approx(T(-14.323944)));
		CHECK(math_util::ConvertRadToDeg(T(0.125)) == doctest::Approx(T(7.161972)));
		CHECK(math_util::ConvertRadToDeg(T(-0.125)) == doctest::Approx(T(-7.161972)));
		CHECK(math_util::ConvertRadToDeg(T(0.0625)) == doctest::Approx(T(3.580986)));
	}

	TEST_CASE_TEMPLATE("ConvertDegToRad", T, float, double)
	{
		CHECK(math_util::ConvertDegToRad(T(0.0)) == T(0.0));
		CHECK(math_util::ConvertDegToRad(T(1.0)) == doctest::Approx(T(0.0174532925)));
		CHECK(math_util::ConvertDegToRad(T(-1.0)) == doctest::Approx(T(-0.0174532925)));
		CHECK(math_util::ConvertDegToRad(T(180.0)) == doctest::Approx(T(3.1415926)));
		CHECK(math_util::ConvertDegToRad(T(-180.0)) == doctest::Approx(T(-3.1415926)));
		CHECK(math_util::ConvertDegToRad(T(360.0)) == doctest::Approx(T(6.2831853)));
		CHECK(math_util::ConvertDegToRad(T(-360.0)) == doctest::Approx(T(-6.2831853)));
		CHECK(math_util::ConvertDegToRad(T(28.647889)) == doctest::Approx(T(0.5)));
		CHECK(math_util::ConvertDegToRad(T(-28.647889)) == doctest::Approx(T(-0.5)));
		CHECK(math_util::ConvertDegToRad(T(14.323944)) == doctest::Approx(T(0.25)));
		CHECK(math_util::ConvertDegToRad(T(-14.323944)) == doctest::Approx(T(-0.25)));
		CHECK(math_util::ConvertDegToRad(T(7.161972)) == doctest::Approx(T(0.125)));
		CHECK(math_util::ConvertDegToRad(T(-7.161972)) == doctest::Approx(T(-0.125)));
		CHECK(math_util::ConvertDegToRad(T(3.580986)) == doctest::Approx(T(0.0625)));
	}
}