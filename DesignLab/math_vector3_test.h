#pragma once

#include <doctest.h>

#include "math_vector3.h"


TEST_SUITE("Vector3")
{
	using namespace designlab;

	TEST_CASE("GetSquaredLength_実行時_長さの2乗が返るべき")
	{
		SUBCASE("(3,5,2)で実行時_38.0fが返るべき")
		{
			const auto v = Vector3(3, 5, 2);
			const auto actual = v.GetSquaredLength();
			const auto expected = 38.0f;
			CHECK(actual == expected);
		}

		SUBCASE("(0,0,0)で実行時_0が返るべき")
		{
			const auto v = Vector3(0, 0, 0);
			const auto actual = v.GetSquaredLength();
			const auto expected = 0.0f;
			CHECK(actual == expected);
		}

		SUBCASE("(-3,-5,-2)で実行時_38.0fが返るべき")
		{
			const auto v = Vector3(-3, -5, -2);
			const auto actual = v.GetSquaredLength();
			const auto expected = 38.0f;
			CHECK(actual == expected);
		}
	}

	TEST_CASE("GetLength_実行時_長さが返るべき")
	{
		SUBCASE("(3,5,2)で実行時_6.164414fが返るべき")
		{
			const auto v = Vector3(3, 5, 2);
			const auto actual = v.GetLength();
			const auto expected = 6.164414f;
			CHECK(actual == expected);
		}

		SUBCASE("(0,0,0)で実行時_0が返るべき")
		{
			const auto v = Vector3(0, 0, 0);
			const auto actual = v.GetLength();
			const auto expected = 0.0f;
			CHECK(actual == expected);
		}

		SUBCASE("(-3,-5,-2)で実行時_6.164414fが返るべき")
		{
			const auto v = Vector3(-3, -5, -2);
			const auto actual = v.GetLength();
			const auto expected = 6.164414f;
			CHECK(actual == expected);
		}
	}
}