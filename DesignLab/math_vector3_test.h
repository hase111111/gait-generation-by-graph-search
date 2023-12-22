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

	TEST_CASE("Cross_他のベクトルを引数に渡したとき_自身×他のベクトルの外積が返るべき")
	{
		SUBCASE("(1,0,0)と(0,1,0)を引数に渡したとき_(0,0,1)が返るべき")
		{
			const auto v1 = Vector3(1, 0, 0);
			const auto v2 = Vector3(0, 1, 0);
			const auto actual = v1.Cross(v2);
			const auto expected = Vector3(0, 0, 1);
			CHECK(actual == expected);
		}

		SUBCASE("(0,1,0)と(0,0,1)を引数に渡したとき_(1,0,0)が返るべき")
		{
			const auto v1 = Vector3(0, 1, 0);
			const auto v2 = Vector3(0, 0, 1);
			const auto actual = v1.Cross(v2);
			const auto expected = Vector3(1, 0, 0);
			CHECK(actual == expected);
		}

		SUBCASE("(0,0,1)と(1,0,0)を引数に渡したとき_(0,1,0)が返るべき")
		{
			const auto v1 = Vector3(0, 0, 1);
			const auto v2 = Vector3(1, 0, 0);
			const auto actual = v1.Cross(v2);
			const auto expected = Vector3(0, 1, 0);
			CHECK(actual == expected);
		}

		SUBCASE("(0,0,1)と(0,1,0)を引数に渡したとき_(-1,0,0)が返るべき")
		{
			const auto v1 = Vector3(0, 0, 1);
			const auto v2 = Vector3(0, 1, 0);
			const auto actual = v1.Cross(v2);
			const auto expected = Vector3(-1, 0, 0);
			CHECK(actual == expected);
		}
	}
}