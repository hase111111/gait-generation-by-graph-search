﻿#pragma once

#include <doctest.h>

#include "math_util.h"


TEST_SUITE("math_util::IsEqual")
{
    using namespace designlab;

    TEST_CASE_TEMPLATE("値が等しい時，trueを返すべき", T, float, double)
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

    TEST_CASE_TEMPLATE("値が等しくない時，falseを返すべき", T, float, double)
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

TEST_SUITE("math_util::Squared")
{
    TEST_CASE_TEMPLATE("値の2乗を返すべき", T, int, float, double)
    {
        CHECK(math_util::Squared(T(0.0)) == T(0.0));
        CHECK(math_util::Squared(T(1.0)) == T(1.0));
        CHECK(math_util::Squared(T(-1.0)) == T(1.0));
        CHECK(math_util::Squared(T(5)) == T(25));
        CHECK(math_util::Squared(T(-5)) == T(25));
        CHECK(math_util::Squared(T(15)) == T(225));
        CHECK(math_util::Squared(T(1462)) == T(2137444));
    }
}

TEST_SUITE("math_util::CanMakeTriangle")
{
    TEST_CASE_TEMPLATE("3角形を作ることが可能な時，trueを返すべき", T, int, float, double)
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

    TEST_CASE_TEMPLATE("3角形を作ることが不可能な時，falseを返すべき", T, int, float, double)
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
}

TEST_SUITE("math_util::ConvertRadToDeg")
{
    TEST_CASE_TEMPLATE("0 [rad]が渡された時，0 [deg]を返すべき", T, float, double)
    {
        CHECK(math_util::ConvertRadToDeg(T(0.0)) == T(0.0));
    }

    TEST_CASE_TEMPLATE("0 [rad] ～ 3.14 [rad]の範囲の値が渡された時，0 [deg] ～ 360 [deg]の範囲で変換すべき", T, float, double)
    {
        T rad1 = T(MathConst<T>::kPi / 6);
        T expected_deg1 = T(30.0);
        CHECK(math_util::ConvertRadToDeg(rad1) == doctest::Approx(expected_deg1));

        T rad2 = T(MathConst<T>::kPi / 4);
        T expected_deg2 = T(45.0);
        CHECK(math_util::ConvertRadToDeg(rad2) == doctest::Approx(expected_deg2));

        T rad3 = T(MathConst<T>::kPi / 3);
        T expected_deg3 = T(60.0);
        CHECK(math_util::ConvertRadToDeg(rad3) == doctest::Approx(expected_deg3));

        T rad4 = T(MathConst<T>::kPi / 2);
        T expected_deg4 = T(90.0);
        CHECK(math_util::ConvertRadToDeg(rad4) == doctest::Approx(expected_deg4));

        T rad5 = T(MathConst<T>::kPi);
        T expected_deg5 = T(180.0);
        CHECK(math_util::ConvertRadToDeg(rad5) == doctest::Approx(expected_deg5));

        T rad6 = T(MathConst<T>::kPi * 2);
        T expected_deg6 = T(360.0);
        CHECK(math_util::ConvertRadToDeg(rad6) == doctest::Approx(expected_deg6));
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