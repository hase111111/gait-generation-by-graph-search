
//! @file      math_vector2_test.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_MATH_VECTOR2_TEST_H_
#define DESIGNLAB_MATH_VECTOR2_TEST_H_

#include <utility>

#include <doctest.h>

#include "math_vector2.h"


TEST_SUITE("Vector2::Constructor")
{
    using designlab::Vector2;

    TEST_CASE("デフォルトコンストラクタを呼んだ時，0ベクトルになるべき")
    {
        const auto v = Vector2();
        const float expected = 0.0f;
        CHECK(v.x == expected);
        CHECK(v.y == expected);
    }

    TEST_CASE("引数に値を渡した時，その値で初期化されるべき")
    {
        const auto v = Vector2(1, 2);
        const float expected_x = 1.0f;
        const float expected_y = 2.0f;
        CHECK(v.x == expected_x);
        CHECK(v.y == expected_y);
    }

    TEST_CASE("コピーコンストラクタを呼んだ時，コピー元と同じ値になるべき")
    {
        const auto v1 = Vector2(1, 2);
        const auto v2 = Vector2(v1);
        CHECK(v1 == v2);
    }

    TEST_CASE("ムーブコンストラクタを呼んだ時，コピー元と同じ値になるべき")
    {
        Vector2 v1 = Vector2(1, 2);
        const Vector2 v2 = Vector2(std::move(v1));
        CHECK(v1 == v2);
    }
}

TEST_SUITE("Vector2::Cross")
{
    using designlab::Vector2;

    TEST_CASE("他のベクトルを渡した時，自身×他のベクトルの外積が返るべき")
    {
        SUBCASE("(1,0)と(0,1)を引数に渡したとき，1が返るべき")
        {
            const auto v1 = Vector2(1, 0);
            const auto v2 = Vector2(0, 1);
            const auto actual = v1.Cross(v2);
            const auto expected = 1.0f;
            CHECK(actual == expected);
        }

        SUBCASE("(0,1)と(1,0)を引数に渡したとき，-1が返るべき")
        {
            const auto v1 = Vector2(0, 1);
            const auto v2 = Vector2(1, 0);
            const auto actual = v1.Cross(v2);
            const auto expected = -1.0f;
            CHECK(actual == expected);
        }

        SUBCASE("(1,1)と(1,1)を引数に渡したとき，0が返るべき")
        {
            const auto v1 = Vector2(1, 1);
            const auto v2 = Vector2(1, 1);
            const auto actual = v1.Cross(v2);
            const auto expected = 0.0f;
            CHECK(actual == expected);
        }
    }
}

TEST_SUITE("Vector2::Dot")
{
    using designlab::Vector2;

    TEST_CASE("他のベクトルを渡した時，自身×他のベクトルの内積が返るべき")
    {
        SUBCASE("(1,0)と(0,1)を引数に渡したとき，0が返るべき")
        {
            const auto v1 = Vector2(1, 0);
            const auto v2 = Vector2(0, 1);
            const auto actual = v1.Dot(v2);
            const auto expected = 0.0f;
            CHECK(actual == expected);
        }

        SUBCASE("(0,1)と(1,0)を引数に渡したとき，0が返るべき")
        {
            const auto v1 = Vector2(0, 1);
            const auto v2 = Vector2(1, 0);
            const auto actual = v1.Dot(v2);
            const auto expected = 0.0f;
            CHECK(actual == expected);
        }

        SUBCASE("(1,1)と(1,1)を引数に渡したとき，2が返るべき")
        {
            const auto v1 = Vector2(1, 1);
            const auto v2 = Vector2(1, 1);
            const auto actual = v1.Dot(v2);
            const auto expected = 2.0f;
            CHECK(actual == expected);
        }
    }
}

TEST_SUITE("Vector2::GetDistanceFrom")
{
    using designlab::Vector2;

    TEST_CASE("他のベクトルを渡した時，そのベクトルとの距離が返るべき")
    {
        SUBCASE("(1,0)と(0,1)を引数に渡したとき，1.41421356237fが返るべき")
        {
            const auto v1 = Vector2(1, 0);
            const auto v2 = Vector2(0, 1);
            const auto actual = v1.GetDistanceFrom(v2);
            const auto expected = 1.41421356237f;
            CHECK(actual == expected);
        }

        SUBCASE("(0,1)と(1,0)を引数に渡したとき，1.41421356237fが返るべき")
        {
            const auto v1 = Vector2(0, 1);
            const auto v2 = Vector2(1, 0);
            const auto actual = v1.GetDistanceFrom(v2);
            const auto expected = 1.41421356237f;
            CHECK(actual == expected);
        }

        SUBCASE("(1,1)と(1,1)を引数に渡したとき，0が返るべき")
        {
            const auto v1 = Vector2(1, 1);
            const auto v2 = Vector2(1, 1);
            const auto actual = v1.GetDistanceFrom(v2);
            const auto expected = 0.0f;
            CHECK(actual == expected);
        }
    }
}

TEST_SUITE("Vector2::GetLength")
{
    using designlab::Vector2;

    TEST_CASE("実行時，長さが返るべき")
    {
        SUBCASE("(3,5)で実行時，5.83095189485fが返るべき")
        {
            const auto v = Vector2(3, 5);
            const auto actual = v.GetLength();
            const auto expected = 5.83095189485f;
            CHECK(actual == expected);
        }

        SUBCASE("(0,0)で実行時，0が返るべき")
        {
            const auto v = Vector2(0, 0);
            const auto actual = v.GetLength();
            const auto expected = 0.0f;
            CHECK(actual == expected);
        }

        SUBCASE("(-3,-5)で実行時，5.83095189485fが返るべき")
        {
            const auto v = Vector2(-3, -5);
            const auto actual = v.GetLength();
            const auto expected = 5.83095189485f;
            CHECK(actual == expected);
        }
    }
}

TEST_SUITE("Vector2::GetNormalized")
{
    using designlab::Vector2;

    TEST_CASE("実行時，正規化されたベクトルを返すべき")
    {
        SUBCASE("(3,5)で実行時，(0.514495,0.857493)が返るべき")
        {
            const auto v = Vector2(3, 5);
            const auto actual = v.GetNormalized();
            const auto expected = Vector2(0.514495f, 0.857493f);

            CHECK(actual == expected);
            CHECK(actual.GetLength() == doctest::Approx(1.0f));
        }

        SUBCASE("(0,0)で実行時，(0,0)が返るべき")
        {
            const auto v = Vector2(0, 0);
            const auto actual = v.GetNormalized();
            const auto expected = Vector2(0, 0);

            CHECK(actual == expected);
            CHECK(actual.GetLength() == doctest::Approx(0.0f));
        }

        SUBCASE("(-3,-5)で実行時，(-0.514495,-0.857493)が返るべき")
        {
            const auto v = Vector2(-3, -5);
            const auto actual = v.GetNormalized();
            const auto expected = Vector2(-0.514495f, -0.857493f);

            CHECK(actual == expected);
            CHECK(actual.GetLength() == doctest::Approx(1.0f));
        }
    }
}

TEST_SUITE("Vector2::GetSquaredLength")
{
    using designlab::Vector2;

    TEST_CASE("実行時，長さの2乗が返るべき")
    {
        SUBCASE("(3,5)で実行時，34が返るべき")
        {
            const auto v = Vector2(3, 5);
            const auto actual = v.GetSquaredLength();
            const auto expected = 34.0f;
            CHECK(actual == expected);
        }

        SUBCASE("(0,0)で実行時，0が返るべき")
        {
            const auto v = Vector2(0, 0);
            const auto actual = v.GetSquaredLength();
            const auto expected = 0.0f;
            CHECK(actual == expected);
        }

        SUBCASE("(-3,-5)で実行時，34が返るべき")
        {
            const auto v = Vector2(-3, -5);
            const auto actual = v.GetSquaredLength();
            const auto expected = 34.0f;
            CHECK(actual == expected);
        }
    }
}

TEST_SUITE("Vector2::IsZero")
{
    using designlab::Vector2;

    TEST_CASE("零ベクトルを渡した時，trueが返るべき")
    {
        const auto v1 = Vector2(0, 0);

        CHECK(v1.IsZero());

        const auto v2 = Vector2::GetZeroVec();

        CHECK(v2.IsZero());
    }

    TEST_CASE("零ベクトルでないベクトルを渡した時，falseが返るべき")
    {
        const auto v1 = Vector2(1, 0);

        CHECK_FALSE(v1.IsZero());

        const auto v2 = Vector2(0, 1);

        CHECK_FALSE(v2.IsZero());

        const auto v3 = Vector2(1, 1);

        CHECK_FALSE(v3.IsZero());
    }
}

TEST_SUITE("Vector2::Normalize")
{
    using designlab::Vector2;

    TEST_CASE("実行時，正規化されたベクトルになるべき")
    {
        SUBCASE("(3,5)で実行時，(0.514495,0.857493)になるべき")
        {
            auto v = Vector2(3, 5);
            v.Normalize();
            const auto expected = Vector2(0.514495f, 0.857493f);

            CHECK(v == expected);
            CHECK(v.GetLength() == doctest::Approx(1.0f));
        }

        SUBCASE("(0,0)で実行時，(0,0)になるべき")
        {
            auto v = Vector2(0, 0);
            v.Normalize();
            const auto expected = Vector2(0, 0);

            CHECK(v == expected);
            CHECK(v.GetLength() == doctest::Approx(0.0f));
        }

        SUBCASE("(-3,-5)で実行時，(-0.514495,-0.857493)になるべき")
        {
            auto v = Vector2(-3, -5);
            v.Normalize();
            const auto expected = Vector2(-0.514495f, -0.857493f);

            CHECK(v == expected);
            CHECK(v.GetLength() == doctest::Approx(1.0f));
        }
    }
}

#endif  // DESIGNLAB_MATH_VECTOR2_TEST_H_
