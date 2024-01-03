
//! @file      math_vector3_test.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_MATH_VECTOR3_TEST_H_
#define DESIGNLAB_MATH_VECTOR3_TEST_H_

#include <doctest.h>

#include <format>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "math_vector3.h"



TEST_SUITE("Vector3")
{
    using designlab::Vector3;
    using designlab::Vector2;

    TEST_CASE("DefaultConstructorTest_ShouldBeZeroVector")
    {
        const auto v = Vector3();
        const float expected = 0.0f;
        CHECK(v.x == expected);
        CHECK(v.y == expected);
        CHECK(v.z == expected);
    }

    TEST_CASE("ConstructorTest")
    {
        const auto v = Vector3(1, 2, 3);
        CHECK(v.x == 1);
        CHECK(v.y == 2);
        CHECK(v.z == 3);
    }

    TEST_CASE("CopyConstructorTest")
    {
        const auto v1 = Vector3(1, 2, 3);
        const auto v2 = Vector3(v1);
        CHECK(v2.x == v1.x);
        CHECK(v2.y == v1.y);
        CHECK(v2.z == v1.z);
    }

    TEST_CASE("MoveConstructorTest")
    {
        Vector3 v1 = Vector3(1, 2, 3);
        const Vector3 v2 = Vector3(std::move(v1));
        CHECK(v2.x == 1);
        CHECK(v2.y == 2);
        CHECK(v2.z == 3);
    }

    TEST_CASE("AssignmentOperatorTest")
    {
        auto v = Vector3(1, 2, 3);

        CHECK(v.x == 1);
        CHECK(v.y == 2);
        CHECK(v.z == 3);
    }

    TEST_CASE("ArithmeticOperatorTest")
    {
        SUBCASE("Addition")
        {
            constexpr auto v1 = Vector3(1, 2, 3);
            constexpr auto v2 = Vector3(4, 5, 6);
            constexpr auto actual = v1 + v2;
            constexpr auto expected = Vector3(5, 7, 9);

            CHECK_EQ(actual, expected);
        }

        SUBCASE("Subtraction")
        {
            constexpr auto v1 = Vector3(1, 2, 3);
            constexpr auto v2 = Vector3(4, 5, 6);
            constexpr auto actual = v1 - v2;
            constexpr auto expected = Vector3(-3, -3, -3);

            CHECK_EQ(actual, expected);
        }

        SUBCASE("Multiplication_WhenScalarIsFirst")
        {
            constexpr auto v = Vector3(1, 2, 3);
            constexpr auto s = 2.0f;
            constexpr auto actual = s * v;
            constexpr auto expected = Vector3(2, 4, 6);

            CHECK_EQ(actual, expected);
        }

        SUBCASE("Multiplication_WhenScalarIsSecond")
        {
            constexpr auto v = Vector3(1, 2, 3);
            constexpr auto s = 2.0f;
            constexpr auto actual = v * s;
            constexpr auto expected = Vector3(2, 4, 6);

            CHECK_EQ(actual, expected);
        }

        SUBCASE("Division")
        {
            constexpr auto v1 = Vector3(1, 2, 3);
            constexpr auto v2 = 2.0f;
            constexpr auto actual = v1 / v2;
            constexpr auto expected = Vector3(0.5f, 1.0f, 1.5f);

            CHECK_EQ(actual, expected);
        }
    }

    TEST_CASE("UnaryOperatorTest")
    {
        SUBCASE("Plus")
        {
            constexpr auto v = Vector3(1, 2, 3);
            constexpr auto actual = +v;
            constexpr auto expected = Vector3(1, 2, 3);

            CHECK_EQ(actual, expected);
        }

        SUBCASE("Minus")
        {
            constexpr auto v = Vector3(1, 2, 3);
            constexpr auto actual = -v;
            constexpr auto expected = Vector3(-1, -2, -3);

            CHECK_EQ(actual, expected);
        }
    }

    TEST_CASE("CrossTest")
    {
        const struct { Vector3 v1; Vector3 v2; Vector3 expected; }
        table[] = {
            {Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1)},
            {Vector3(0, 1, 0), Vector3(0, 0, 1), Vector3(1, 0, 0)},
            {Vector3(0, 0, 1), Vector3(1, 0, 0), Vector3(0, 1, 0)},
            {Vector3(0, 0, 1), Vector3(0, 1, 0), Vector3(-1, 0, 0)},
        };

        for (auto [v1, v2, expected] : table)
        {
            const auto actual = v1.Cross(v2);

            INFO(std::format("v1 = ({}, {}, {})", v1.x, v1.y, v1.z));
            INFO(std::format("v2 = ({}, {}, {})", v2.x, v2.y, v2.z));
            CHECK_EQ(actual, expected);
        }
    }

    TEST_CASE("DotTest")
    {
        const struct { Vector3 v1; Vector3 v2; float expected; }
        table[] = {
            {Vector3(1, 0, 0), Vector3(1, 0, 0), 1.0f},
            {Vector3(1, 0, 0), Vector3(0, 1, 0), 0.0f},
            {Vector3(1, 0, 0), Vector3(0, 0, 1), 0.0f},
            {Vector3(0, 1, 0), Vector3(1, 0, 0), 0.0f},
            {Vector3(0, 1, 0), Vector3(0, 1, 0), 1.0f},
            {Vector3(0, 1, 1), Vector3(0, 1, 1), 2.0f},
            {Vector3(3, 7, 5), Vector3(2, 11, 13), 148.0f},
            {Vector3(-2, 5, -7), Vector3(1, -11, 6), -99.0f},
        };

        for (auto [v1, v2, expected] : table)
        {
            const auto actual = v1.Dot(v2);

            INFO(std::format("v1 = ({}, {}, {})", v1.x, v1.y, v1.z));
            INFO(std::format("v2 = ({}, {}, {})", v2.x, v2.y, v2.z));
            CHECK(actual == doctest::Approx(expected));
        }
    }

    TEST_CASE("GetDistanceFromTest")
    {
        const struct { Vector3 v1; Vector3 v2; float expected; }
        table[] = {
            {Vector3(1, 0, 0), Vector3(0, 1, 0), 1.414214f},
            {Vector3(0, 1, 0), Vector3(0, 0, 1), 1.414214f},
            {Vector3(26.f, -30.f, 105.f), Vector3(10.f, 0, 5.f), 105.621967f},
            {Vector3(10.f, 0, 5.f), Vector3(26.f, -30.f, 105.f), 105.621967f},
            {Vector3(0.2f, 1.f, -3.f), Vector3(-4.4f, 1.4f, 31.6f), 34.906732f},
        };

        for (auto [v1, v2, expected] : table)
        {
            const auto actual = v1.GetDistanceFrom(v2);

            INFO(std::format("v1 = ({}, {}, {})", v1.x, v1.y, v1.z));
            INFO(std::format("v2 = ({}, {}, {})", v2.x, v2.y, v2.z));
            CHECK(actual == doctest::Approx(expected));
        }
    }

    TEST_CASE("GetLengthTest")
    {
        const struct { Vector3 v; float expected; }
        table[] = {
            {Vector3(1, 0, 0), 1.0f},
            {Vector3(0, 1, 0), 1.0f},
            {Vector3(0, 0, 1), 1.0f},
            {Vector3(1, 1, 1), 1.732051f},
            {Vector3(-1, -1, -1), 1.732051f},
        };

        for (auto [v, expected] : table)
        {
            const auto actual = v.GetLength();

            INFO(std::format("v = ({}, {}, {})", v.x, v.y, v.z));
            CHECK(actual == doctest::Approx(expected));
        }
    }

    TEST_CASE("GetNormalizedTest_WhenZeroVectorIsPassed_ShouldBeZeroVector")
    {
        const auto v1 = Vector3(0, 0, 0);
        const auto actual1 = v1.GetNormalized();
        const auto expected1 = Vector3(0, 0, 0);

        CHECK(actual1 == expected1);
        CHECK(actual1.GetLength() == doctest::Approx(0.0f));

        const auto v2 = Vector3::GetZeroVec();
        const auto actual2 = v2.GetNormalized();
        const auto expected2 = Vector3(0, 0, 0);

        CHECK(actual2 == expected2);
        CHECK(actual2.GetLength() == doctest::Approx(0.0f));
    }

    TEST_CASE("GetNormalizedTest_WhenNonZeroVectorIsPassed_ShouldBeNormalizedVector")
    {
        const struct { Vector3 v; Vector3 expected; }
        table[] = {
                    {Vector3(1, 0, 0), Vector3(1, 0, 0)},
                    {Vector3(0, 1, 0), Vector3(0, 1, 0)},
                    {Vector3(0, 0, 1), Vector3(0, 0, 1)},
                    {Vector3(1, 1, 1), Vector3(0.577350f, 0.577350f, 0.577350f)},
                    {Vector3(-1, -1, -1), Vector3(-0.577350f, -0.577350f, -0.577350f)},
        };

        for (auto [v, expected] : table)
        {
            const auto actual = v.GetNormalized();

            INFO(std::format("v = ({}, {}, {})", v.x, v.y, v.z));
            CHECK(actual == expected);
            CHECK(actual.GetLength() == doctest::Approx(1.0f));
        }
    }

    TEST_CASE("GetSquaredLengthTest")
    {
        const struct { Vector3 v; float expected; }
        table[] = {
                    {Vector3(1, 0, 0), 1.0f},
                    {Vector3(0, 1, 0), 1.0f},
                    {Vector3(0, 0, 1), 1.0f},
                    {Vector3(1, 1, 1), 3.0f},
                    {Vector3(-1, -1, -1), 3.0f},
        };

        for (auto [v, expected] : table)
        {
            const auto actual = v.GetSquaredLength();

            INFO(std::format("v = ({}, {}, {})", v.x, v.y, v.z));
            CHECK(actual == doctest::Approx(expected));
        }
    }

    TEST_CASE("IsZeroTest_WhenZeroVectorIsPassed_ShouldBeTrue")
    {
        const auto v1 = Vector3(0, 0, 0);

        CHECK(v1.IsZero());

        const auto v2 = Vector3::GetZeroVec();

        CHECK(v2.IsZero());
    }

    TEST_CASE("IsZeroTest_WhenNonZeroVectorIsPassed_ShouldBeFalse")
    {
        SUBCASE("When (1,0,0) is passed, should be false")
        {
            const auto v = Vector3(1, 0, 0);

            CHECK_FALSE(v.IsZero());
        }

        SUBCASE("When (0,1,0) is passed, should be false")
        {
            const auto v = Vector3(0, 1, 0);

            CHECK_FALSE(v.IsZero());
        }

        SUBCASE("When (0,0,1) is passed, should be false")
        {
            const auto v = Vector3(0, 0, 1);

            CHECK_FALSE(v.IsZero());
        }

        SUBCASE("When (1,1,1) is passed, should be false")
        {
            const auto v = Vector3(1, 1, 1);

            CHECK_FALSE(v.IsZero());
        }

        SUBCASE("When (-1,-1,-1) is passed, should be false")
        {
            const auto v = Vector3(-1, -1, -1);

            CHECK_FALSE(v.IsZero());
        }
    }

    TEST_CASE("IsZeroTest_WhenEpsilonIsPassed_ShouldBeTrue")
    {
        const auto v1 = Vector3(0.0000001f, 0.0000001f, 0.0000001f);

        CHECK(v1.IsZero());
    }

    TEST_CASE("NormalizeTest_WhenZeroVectorIsPassed_ShouldBeZeroVector")
    {
        auto v1 = Vector3(0, 0, 0);
        v1.Normalize();

        CHECK(v1 == Vector3(0, 0, 0));
        CHECK(v1.GetLength() == doctest::Approx(0.0f));

        auto v2 = Vector3::GetZeroVec();
        v2.Normalize();

        CHECK(v2 == Vector3(0, 0, 0));
        CHECK(v2.GetLength() == doctest::Approx(0.0f));
    }

    TEST_CASE("NormalizeTest_WhenNonZeroVectorIsPassed_ShouldBeNormalizedVector")
    {
        const struct { Vector3 v; Vector3 expected; }
        table[] = {
                    {Vector3(1, 0, 0), Vector3(1, 0, 0)},
                    {Vector3(0, 1, 0), Vector3(0, 1, 0)},
                    {Vector3(0, 0, 1), Vector3(0, 0, 1)},
                    {Vector3(1, 1, 1), Vector3(0.577350f, 0.577350f, 0.577350f)},
                    {Vector3(-1, -1, -1), Vector3(-0.577350f, -0.577350f, -0.577350f)},
        };

        for (auto [v, expected] : table)
        {
            auto actual = v;
            actual.Normalize();

            INFO(std::format("v = ({}, {}, {})", v.x, v.y, v.z));
            CHECK(actual == expected);
            CHECK(actual.GetLength() == doctest::Approx(1.0f));
        }
    }

    TEST_CASE("ProjectedXYTest")
    {
        const struct { Vector3 v; Vector2 expected; }
        table[] = {
                      {Vector3(0, 0, 0), Vector2(0, 0)},
                      {Vector3(1, 2, 3), Vector2(1, 2)},
                      {Vector3(-1, -2, -3), Vector2(-1, -2)},
        };

        for (auto [v, expected] : table)
        {
            const auto actual = v.ProjectedXY();

            INFO(std::format("v = ({}, {}, {})", v.x, v.y, v.z));
            CHECK(actual == expected);
        }
    }
}


#endif  // DESIGNLAB_MATH_VECTOR3_TEST_H_
