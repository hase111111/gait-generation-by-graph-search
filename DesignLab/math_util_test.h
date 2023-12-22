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
}