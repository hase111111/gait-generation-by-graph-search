
//! @file      math_util_test.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_MATH_UTIL_TEST_H_
#define DESIGNLAB_MATH_UTIL_TEST_H_

#include <doctest.h>

#include <numbers>

#include "math_util.h"


TEST_SUITE("math_util::IsEqual")
{
    using designlab::math_util::IsEqual;
    using designlab::MathConst;

    TEST_CASE_TEMPLATE("値が等しい時，trueを返すべき", T, float, double)
    {
        CHECK(IsEqual(T(0.0), T(0.0)));
        CHECK(IsEqual(T(1.0), T(1.0)));
        CHECK(IsEqual(T(-1.0), T(-1.0)));
        CHECK(IsEqual(T(325.1), T(325.1)));

        SUBCASE("誤差の範囲内で等しい時，trueを返すべき")
        {
            CHECK(IsEqual(T(0.0), T(0.0) + MathConst<T>::kAllowableError / 2));
            CHECK(IsEqual(T(0.0), T(0.0) - MathConst<T>::kAllowableError / 2));
            CHECK(IsEqual(T(1.0), T(1.0) + MathConst<T>::kAllowableError / 2));
            CHECK(IsEqual(T(1.0), T(1.0) - MathConst<T>::kAllowableError / 2));
            CHECK(IsEqual(T(-1.0), T(-1.0) + MathConst<T>::kAllowableError / 2));
            CHECK(IsEqual(T(-1.0), T(-1.0) - MathConst<T>::kAllowableError / 2));
        }
    }

    TEST_CASE_TEMPLATE("値が等しくない時，falseを返すべき", T, float, double)
    {
        CHECK_FALSE(IsEqual(T(0.0), T(1.0)));
        CHECK_FALSE(IsEqual(T(1.0), T(0.0)));
        CHECK_FALSE(IsEqual(T(-1.0), T(1.0)));
        CHECK_FALSE(IsEqual(T(325.1), T(325.2)));

        SUBCASE("誤差の範囲を上回っている時，falseを返すべき")
        {
            CHECK_FALSE(IsEqual(T(0.0), T(0.0) + MathConst<T>::kAllowableError * 2));
            CHECK_FALSE(IsEqual(T(0.0), T(0.0) - MathConst<T>::kAllowableError * 2));
            CHECK_FALSE(IsEqual(T(1.0), T(1.0) + MathConst<T>::kAllowableError * 2));
            CHECK_FALSE(IsEqual(T(1.0), T(1.0) - MathConst<T>::kAllowableError * 2));
            CHECK_FALSE(IsEqual(T(-1.0), T(-1.0) + MathConst<T>::kAllowableError * 2));
            CHECK_FALSE(IsEqual(T(-1.0), T(-1.0) - MathConst<T>::kAllowableError * 2));
        }
    }
}

TEST_SUITE("math_util::Squared")
{
    using designlab::math_util::Squared;

    TEST_CASE_TEMPLATE("値の2乗を返すべき", T, int, float, double)
    {
        CHECK(Squared(T(0.0)) == T(0.0));
        CHECK(Squared(T(1.0)) == T(1.0));
        CHECK(Squared(T(-1.0)) == T(1.0));
        CHECK(Squared(T(5)) == T(25));
        CHECK(Squared(T(-5)) == T(25));
        CHECK(Squared(T(15)) == T(225));
        CHECK(Squared(T(-1462)) == T(2137444));
    }
}

TEST_SUITE("math_util::CanMakeTriangle")
{
    using designlab::math_util::CanMakeTriangle;

    TEST_CASE_TEMPLATE("3角形を作ることが可能な時，trueを返すべき", T, int, float, double)
    {
        CHECK(CanMakeTriangle(T(1.0), T(1.0), T(1.0)));
        CHECK(CanMakeTriangle(T(2.0), T(2.0), T(3.0)));
        CHECK(CanMakeTriangle(T(2.0), T(3.0), T(2.0)));
        CHECK(CanMakeTriangle(T(3.0), T(2.0), T(2.0)));
        CHECK(CanMakeTriangle(T(3.0), T(4.0), T(5.0)));
        CHECK(CanMakeTriangle(T(3.0), T(5.0), T(4.0)));
        CHECK(CanMakeTriangle(T(4.0), T(3.0), T(5.0)));
        CHECK(CanMakeTriangle(T(4.0), T(5.0), T(3.0)));
        CHECK(CanMakeTriangle(T(5.0), T(3.0), T(4.0)));
        CHECK(CanMakeTriangle(T(5.0), T(4.0), T(3.0)));
    }

    TEST_CASE_TEMPLATE("3角形を作ることが不可能な時，falseを返すべき", T, int, float, double)
    {
        CHECK_FALSE(CanMakeTriangle(T(1.0), T(1.0), T(3.0)));
        CHECK_FALSE(CanMakeTriangle(T(1.0), T(3.0), T(1.0)));
        CHECK_FALSE(CanMakeTriangle(T(3.0), T(1.0), T(1.0)));
        CHECK_FALSE(CanMakeTriangle(T(1.0), T(2.0), T(3.0)));
        CHECK_FALSE(CanMakeTriangle(T(1.0), T(3.0), T(2.0)));
        CHECK_FALSE(CanMakeTriangle(T(2.0), T(1.0), T(3.0)));
        CHECK_FALSE(CanMakeTriangle(T(2.0), T(3.0), T(1.0)));
        CHECK_FALSE(CanMakeTriangle(T(3.0), T(1.0), T(2.0)));
        CHECK_FALSE(CanMakeTriangle(T(3.0), T(2.0), T(1.0)));
        CHECK_FALSE(CanMakeTriangle(T(1.0), T(1.0), T(2.0)));
        CHECK_FALSE(CanMakeTriangle(T(1.0), T(2.0), T(1.0)));
        CHECK_FALSE(CanMakeTriangle(T(2.0), T(1.0), T(1.0)));
        CHECK_FALSE(CanMakeTriangle(T(1.0), T(1.0), T(4.0)));
        CHECK_FALSE(CanMakeTriangle(T(1.0), T(4.0), T(1.0)));
        CHECK_FALSE(CanMakeTriangle(T(4.0), T(1.0), T(1.0)));
        CHECK_FALSE(CanMakeTriangle(T(1.0), T(2.0), T(4.0)));
        CHECK_FALSE(CanMakeTriangle(T(1.0), T(4.0), T(2.0)));
        CHECK_FALSE(CanMakeTriangle(T(2.0), T(1.0), T(4.0)));
    }
}

TEST_SUITE("math_util::ConvertRadToDeg")
{
    using designlab::MathConst;
    using designlab::math_util::ConvertRadToDeg;

    TEST_CASE_TEMPLATE("0 [rad]が渡された時，0 [deg]を返すべき", T, float, double)
    {
        CHECK(ConvertRadToDeg(T(0.0)) == T(0.0));
    }

    TEST_CASE_TEMPLATE("0 [rad] ～ 3.14 [rad]の範囲の値が渡された時，0 [deg] ～ 360 [deg]の範囲で変換すべき", T, float, double)
    {
        T rad1 = T(std::numbers::pi_v<float> / 6);
        T expected_deg1 = T(30.0);
        CHECK(ConvertRadToDeg(rad1) == doctest::Approx(expected_deg1));

        T rad2 = T(std::numbers::pi_v<float> / 4);
        T expected_deg2 = T(45.0);
        CHECK(ConvertRadToDeg(rad2) == doctest::Approx(expected_deg2));

        T rad3 = T(std::numbers::pi_v<float> / 3);
        T expected_deg3 = T(60.0);
        CHECK(ConvertRadToDeg(rad3) == doctest::Approx(expected_deg3));

        T rad4 = T(std::numbers::pi_v<float> / 2);
        T expected_deg4 = T(90.0);
        CHECK(ConvertRadToDeg(rad4) == doctest::Approx(expected_deg4));

        T rad5 = T(std::numbers::pi_v<float>);
        T expected_deg5 = T(180.0);
        CHECK(ConvertRadToDeg(rad5) == doctest::Approx(expected_deg5));

        T rad6 = T(std::numbers::pi_v<float> *2);
        T expected_deg6 = T(360.0);
        CHECK(ConvertRadToDeg(rad6) == doctest::Approx(expected_deg6));
    }
}

TEST_SUITE("math_util::ConvertDegToRad")
{
    using designlab::math_util::ConvertDegToRad;

    TEST_CASE_TEMPLATE("", T, float, double)
    {
        CHECK(ConvertDegToRad(T(0.0)) == T(0.0));
        CHECK(ConvertDegToRad(T(1.0)) == doctest::Approx(T(0.0174532925)));
        CHECK(ConvertDegToRad(T(-1.0)) == doctest::Approx(T(-0.0174532925)));
        CHECK(ConvertDegToRad(T(180.0)) == doctest::Approx(T(3.1415926)));
        CHECK(ConvertDegToRad(T(-180.0)) == doctest::Approx(T(-3.1415926)));
        CHECK(ConvertDegToRad(T(360.0)) == doctest::Approx(T(6.2831853)));
        CHECK(ConvertDegToRad(T(-360.0)) == doctest::Approx(T(-6.2831853)));
        CHECK(ConvertDegToRad(T(28.647889)) == doctest::Approx(T(0.5)));
        CHECK(ConvertDegToRad(T(-28.647889)) == doctest::Approx(T(-0.5)));
        CHECK(ConvertDegToRad(T(14.323944)) == doctest::Approx(T(0.25)));
        CHECK(ConvertDegToRad(T(-14.323944)) == doctest::Approx(T(-0.25)));
        CHECK(ConvertDegToRad(T(7.161972)) == doctest::Approx(T(0.125)));
        CHECK(ConvertDegToRad(T(-7.161972)) == doctest::Approx(T(-0.125)));
        CHECK(ConvertDegToRad(T(3.580986)) == doctest::Approx(T(0.0625)));
    }
}

TEST_SUITE("math_util::LimitRangeAngleDeg")
{
    using designlab::math_util::LimitRangeAngleDeg;

    TEST_CASE_TEMPLATE("[-180 [deg], 180 [deg] )の範囲の値が渡された時，そのまま返すべき", T, float, double)
    {
        CHECK(LimitRangeAngleDeg(T(-180.0)) == T(-180.0));
        CHECK(LimitRangeAngleDeg(T(-179.0)) == T(-179.0));
        CHECK(LimitRangeAngleDeg(T(-90.0)) == T(-90.0));
        CHECK(LimitRangeAngleDeg(T(-1.0)) == T(-1.0));
        CHECK(LimitRangeAngleDeg(T(0.0)) == T(0.0));
        CHECK(LimitRangeAngleDeg(T(1.0)) == T(1.0));
        CHECK(LimitRangeAngleDeg(T(90.0)) == T(90.0));
        CHECK(LimitRangeAngleDeg(T(179.0)) == T(179.0));
    }

    TEST_CASE_TEMPLATE("-180 [deg]未満の値が渡された時，[-180 [deg], 180 [deg] )の範囲に変換して返すべき", T, float, double)
    {
        CHECK(LimitRangeAngleDeg(T(-181.0)) == T(179.0));
        CHECK(LimitRangeAngleDeg(T(-270.0)) == T(90.0));
        CHECK(LimitRangeAngleDeg(T(-360.0)) == T(0.0));
        CHECK(LimitRangeAngleDeg(T(-361.0)) == T(-1.0));
        CHECK(LimitRangeAngleDeg(T(-450.0)) == T(-90.0));
        CHECK(LimitRangeAngleDeg(T(-540.0)) == T(-180.0));
        CHECK(LimitRangeAngleDeg(T(-541.0)) == T(179.0));
        CHECK(LimitRangeAngleDeg(T(-630.0)) == T(90.0));
        CHECK(LimitRangeAngleDeg(T(-720.0)) == T(0.0));
        CHECK(LimitRangeAngleDeg(T(-721.0)) == T(-1.0));
        CHECK(LimitRangeAngleDeg(T(-810.0)) == T(-90.0));
        CHECK(LimitRangeAngleDeg(T(-900.0)) == T(-180.0));
        CHECK(LimitRangeAngleDeg(T(-901.0)) == T(179.0));
        CHECK(LimitRangeAngleDeg(T(-990.0)) == T(90.0));
        CHECK(LimitRangeAngleDeg(T(-1080.0)) == T(0.0));
    }

    TEST_CASE_TEMPLATE("180 [deg]以上の値が渡された時，[-180 [deg], 180 [deg] )の範囲に変換して返すべき", T, float, double)
    {
        CHECK(LimitRangeAngleDeg(T(180.0)) == T(-180.0));
        CHECK(LimitRangeAngleDeg(T(181.0)) == T(-179.0));
        CHECK(LimitRangeAngleDeg(T(270.0)) == T(-90.0));
        CHECK(LimitRangeAngleDeg(T(360.0)) == T(0.0));
        CHECK(LimitRangeAngleDeg(T(361.0)) == T(1.0));
        CHECK(LimitRangeAngleDeg(T(450.0)) == T(90.0));
        CHECK(LimitRangeAngleDeg(T(540.0)) == T(-180.0));
        CHECK(LimitRangeAngleDeg(T(541.0)) == T(-179.0));
        CHECK(LimitRangeAngleDeg(T(630.0)) == T(-90.0));
        CHECK(LimitRangeAngleDeg(T(720.0)) == T(0.0));
        CHECK(LimitRangeAngleDeg(T(721.0)) == T(1.0));
        CHECK(LimitRangeAngleDeg(T(810.0)) == T(90.0));
        CHECK(LimitRangeAngleDeg(T(900.0)) == T(-180.0));
        CHECK(LimitRangeAngleDeg(T(901.0)) == T(-179.0));
        CHECK(LimitRangeAngleDeg(T(990.0)) == T(-90.0));
        CHECK(LimitRangeAngleDeg(T(1080.0)) == T(0.0));
    }
}


#endif  // DESIGNLAB_MATH_UTIL_TEST_H_
