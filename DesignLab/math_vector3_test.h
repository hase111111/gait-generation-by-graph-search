#pragma once

#include <doctest.h>

#include "math_vector3.h"


TEST_SUITE("Vector3")
{
	using namespace designlab;

	TEST_CASE("コンストラクタ，デフォルトコンストラクタを呼んだ時，0ベクトルになるべき")
	{
		const auto v = Vector3();
		CHECK(v.x == 0);
		CHECK(v.y == 0);
		CHECK(v.z == 0);
	}

	TEST_CASE("Cross，他のベクトルを渡した時，自身×他のベクトルの外積が返るべき")
	{
		SUBCASE("(1,0,0)と(0,1,0)を引数に渡したとき，(0,0,1)が返るべき")
		{
			const auto v1 = Vector3(1, 0, 0);
			const auto v2 = Vector3(0, 1, 0);
			const auto actual = v1.Cross(v2);
			const auto expected = Vector3(0, 0, 1);
			CHECK(actual == expected);
		}

		SUBCASE("(0,1,0)と(0,0,1)を引数に渡したとき，(1,0,0)が返るべき")
		{
			const auto v1 = Vector3(0, 1, 0);
			const auto v2 = Vector3(0, 0, 1);
			const auto actual = v1.Cross(v2);
			const auto expected = Vector3(1, 0, 0);
			CHECK(actual == expected);
		}

		SUBCASE("(0,0,1)と(1,0,0)を引数に渡したとき，(0,1,0)が返るべき")
		{
			const auto v1 = Vector3(0, 0, 1);
			const auto v2 = Vector3(1, 0, 0);
			const auto actual = v1.Cross(v2);
			const auto expected = Vector3(0, 1, 0);
			CHECK(actual == expected);
		}

		SUBCASE("(0,0,1)と(0,1,0)を引数に渡したとき，(-1,0,0)が返るべき")
		{
			const auto v1 = Vector3(0, 0, 1);
			const auto v2 = Vector3(0, 1, 0);
			const auto actual = v1.Cross(v2);
			const auto expected = Vector3(-1, 0, 0);
			CHECK(actual == expected);
		}
	}

	TEST_CASE("Dot，他のベクトルを渡した時，自身×他のベクトルの内積が返るべき")
	{
		SUBCASE("(1,0,0)と(0,1,0)を引数に渡したとき，0が返るべき")
		{
			const auto v1 = Vector3(1, 0, 0);
			const auto v2 = Vector3(0, 1, 0);
			const auto actual = v1.Dot(v2);
			const auto expected = 0.0f;
			CHECK(actual == expected);
		}

		SUBCASE("(0,1,0)と(0,0,1)を引数に渡したとき，0が返るべき")
		{
			const auto v1 = Vector3(0, 1, 0);
			const auto v2 = Vector3(0, 0, 1);
			const auto actual = v1.Dot(v2);
			const auto expected = 0.0f;
			CHECK(actual == expected);
		}

		SUBCASE("(0,0,1)と(1,0,0)を引数に渡したとき，0が返るべき")
		{
			const auto v1 = Vector3(0, 0, 1);
			const auto v2 = Vector3(1, 0, 0);
			const auto actual = v1.Dot(v2);
			const auto expected = 0.0f;
			CHECK(actual == expected);
		}

		SUBCASE("(0,0,1)と(0,1,0)を引数に渡したとき，0が返るべき")
		{
			const auto v1 = Vector3(0, 0, 1);
			const auto v2 = Vector3(0, 1, 0);
			const auto actual = v1.Dot(v2);
			const auto expected = 0.0f;
			CHECK(actual == expected);
		}

		SUBCASE("(5,-3,10)と(2,-1,-2)を引数に渡したとき，-7が返るべき")
		{
			const auto v1 = Vector3(5, -3, 10);
			const auto v2 = Vector3(2, -1, -2);
			const auto actual = v1.Dot(v2);
			const auto expected = -7.0f;
			CHECK(actual == expected);
		}
	}

	TEST_CASE("GetDistanceFrom，他のベクトルを渡した時，そのベクトルとの距離が返るべき")
	{
		SUBCASE("(1,0,0)に(0,1,0)を渡した時，1.414214fが返るべき")
		{
			const auto v1 = Vector3(1, 0, 0);
			const auto v2 = Vector3(0, 1, 0);
			const auto actual = v1.GetDistanceFrom(v2);
			const auto expected = 1.414214f;
			CHECK(actual == doctest::Approx(expected));
		}

		SUBCASE("(0,1,0)に(0,0,1)を渡した時，1.414214fが返るべき")
		{
			const auto v1 = Vector3(0, 1, 0);
			const auto v2 = Vector3(0, 0, 1);
			const auto actual = v1.GetDistanceFrom(v2);
			const auto expected = 1.414214f;
			CHECK(actual == doctest::Approx(expected));
		}

		SUBCASE("(26, -30, 105)に(10, 0, 5)を渡した時，105.621967fが返るべき")
		{
			const auto v1 = Vector3(26.f, -30.f, 105.f);
			const auto v2 = Vector3(10.f, 0, 5.f);
			const auto actual = v1.GetDistanceFrom(v2);
			const auto expected = 105.621967f;
			CHECK(actual == doctest::Approx(expected));
		}

		SUBCASE("(10, 0, 5)に(26, -30, 105)を渡した時，105.621967fが返るべき")
		{
			const auto v1 = Vector3(10.f, 0, 5.f);
			const auto v2 = Vector3(26.f, -30.f, 105.f);
			const auto actual = v1.GetDistanceFrom(v2);
			const auto expected = 105.621967f;
			CHECK(actual == doctest::Approx(expected));
		}

		SUBCASE("(0.2, 1, -3)に(-4.4, 1.4, 31.6)を渡した時，34.906732fが返るべき")
		{
			const auto v1 = Vector3(0.2f, 1.f, -3.f);
			const auto v2 = Vector3(-4.4f, 1.4f, 31.6f);
			const auto actual = v1.GetDistanceFrom(v2);
			const auto expected = 34.906732f;
			CHECK(actual == doctest::Approx(expected));
		}
	}

	TEST_CASE("GetLength，実行時，長さが返るべき")
	{
		SUBCASE("(3,5,2)で実行時，6.164414fが返るべき")
		{
			const auto v = Vector3(3, 5, 2);
			const auto actual = v.GetLength();
			const auto expected = 6.164414f;
			CHECK(actual == expected);
		}

		SUBCASE("(0,0,0)で実行時，0が返るべき")
		{
			const auto v = Vector3(0, 0, 0);
			const auto actual = v.GetLength();
			const auto expected = 0.0f;
			CHECK(actual == expected);
		}

		SUBCASE("(-3,-5,-2)で実行時，6.164414fが返るべき")
		{
			const auto v = Vector3(-3, -5, -2);
			const auto actual = v.GetLength();
			const auto expected = 6.164414f;
			CHECK(actual == expected);
		}
	}

	TEST_CASE("GetNormalized，実行時，正規化されたベクトルを返すべき")
	{
		SUBCASE("(3,5,2)で実行時，(0.486664,0.811107,0.324443)が返るべき")
		{
			const auto actual = Vector3(3, 5, 2).GetNormalized();
			const auto expected = Vector3(0.486664f, 0.811107f, 0.324443f);

			CHECK(actual == expected);
			CHECK(actual.GetLength() == doctest::Approx(1.0f));
		}

		SUBCASE("(0,0,0)で実行時，(0,0,0)が返るべき")
		{
			const auto actual = Vector3(0, 0, 0).GetNormalized();
			const auto expected = Vector3(0, 0, 0);

			CHECK(actual == expected);
			CHECK(actual.GetLength() == doctest::Approx(0.0f));
		}

		SUBCASE("(-3,-5,-2)で実行時，(-0.486664,-0.811107,-0.324443)が返るべき")
		{
			const auto actual = Vector3(-3, -5, -2).GetNormalized();
			const auto expected = Vector3(-0.486664f, -0.811107f, -0.324443f);

			CHECK(actual == expected);
			CHECK(actual.GetLength() == doctest::Approx(1.0f));
		}
	}

	TEST_CASE("GetSquaredLength，実行時，長さの2乗が返るべき")
	{
		SUBCASE("(3,5,2)で実行時，38.0fが返るべき")
		{
			const auto v = Vector3(3, 5, 2);
			const auto actual = v.GetSquaredLength();
			const auto expected = 38.0f;
			CHECK(actual == expected);
		}

		SUBCASE("(0,0,0)で実行時，0が返るべき")
		{
			const auto v = Vector3(0, 0, 0);
			const auto actual = v.GetSquaredLength();
			const auto expected = 0.0f;
			CHECK(actual == expected);
		}

		SUBCASE("(-3,-5,-2)で実行時，38.0fが返るべき")
		{
			const auto v = Vector3(-3, -5, -2);
			const auto actual = v.GetSquaredLength();
			const auto expected = 38.0f;
			CHECK(actual == expected);
		}
	}

	TEST_CASE("IsZero，零ベクトルを渡した時，trueが返るべき")
	{
		const auto v1 = Vector3(0, 0, 0);

		CHECK(v1.IsZero());

		const auto v2 = Vector3::GetZeroVec();

		CHECK(v2.IsZero());
	}

	TEST_CASE("IsZero，零ベクトルでないベクトルを渡した時，falseが返るべき")
	{
		SUBCASE("(1,0,0)を渡した時，falseが返るべき")
		{
			const auto v = Vector3(1, 0, 0);

			CHECK_FALSE(v.IsZero());
		}

		SUBCASE("(0,1,0)を渡した時，falseが返るべき")
		{
			const auto v = Vector3(0, 1, 0);

			CHECK_FALSE(v.IsZero());
		}

		SUBCASE("(0,0,1)を渡した時，falseが返るべき")
		{
			const auto v = Vector3(0, 0, 1);

			CHECK_FALSE(v.IsZero());
		}

		SUBCASE("(1,1,1)を渡した時，falseが返るべき")
		{
			const auto v = Vector3(1, 1, 1);

			CHECK_FALSE(v.IsZero());
		}

		SUBCASE("(-1,-1,-1)を渡した時，falseが返るべき")
		{
			const auto v = Vector3(-1, -1, -1);

			CHECK_FALSE(v.IsZero());
		}
	}

	TEST_CASE("Normalize，実行時，正規化されたベクトルになるべき")
	{
		SUBCASE("(3,5,2)で実行時，(0.486664,0.811107,0.324443)が返るべき")
		{
			auto actual = Vector3(3, 5, 2);
			actual.Normalize();
			const auto expected = Vector3(0.486664f, 0.811107f, 0.324443f);

			CHECK(actual == expected);
			CHECK(actual.GetLength() == doctest::Approx(1.0f));
		}

		SUBCASE("(0,0,0)で実行時，(0,0,0)が返るべき")
		{
			auto actual = Vector3(0, 0, 0);
			actual.Normalize();
			const auto expected = Vector3(0, 0, 0);

			CHECK(actual == expected);
			CHECK(actual.GetLength() == doctest::Approx(0.0f));
		}

		SUBCASE("(-3,-5,-2)で実行時，(-0.486664,-0.811107,-0.324443)が返るべき")
		{
			auto actual = Vector3(-3, -5, -2);
			actual.Normalize();
			const auto expected = Vector3(-0.486664f, -0.811107f, -0.324443f);

			CHECK(actual == expected);
			CHECK(actual.GetLength() == doctest::Approx(1.0f));
		}
	}

	TEST_CASE("ProjectedXY，実行時，XY平面に投射されたベクトルが返るべき")
	{
		SUBCASE("(1,2,3)で実行時，(1,2)が返るべき")
		{
			const auto v = Vector3(1, 2, 3);
			const auto actual = v.ProjectedXY();
			const auto expected = Vector2(1, 2);
			CHECK(actual == expected);
		}

		SUBCASE("(0,0,0)で実行時，(0,0)が返るべき")
		{
			const auto v = Vector3(0, 0, 0);
			const auto actual = v.ProjectedXY();
			const auto expected = Vector2(0, 0);
			CHECK(actual == expected);
		}

		SUBCASE("(-1,-2,-3)で実行時，(-1,-2)が返るべき")
		{
			const auto v = Vector3(-1, -2, -3);
			const auto actual = v.ProjectedXY();
			const auto expected = Vector2(-1, -2);
			CHECK(actual == expected);
		}
	}
}