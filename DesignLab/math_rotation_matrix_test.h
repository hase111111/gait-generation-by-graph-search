
/// @file      math_rotation_matrix_test.h
/// @author    Hasegawa
/// @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#ifndef DESIGNLAB_MATH_ROTATION_MATRIX_TEST_H_
#define DESIGNLAB_MATH_ROTATION_MATRIX_TEST_H_

#include <doctest.h>

#include "math_rotation_matrix.h"

TEST_SUITE("RotationMatrix3x3")
{
    using designlab::RotationMatrix3x3;

    TEST_CASE("ConstructorTest_ShouldCreateIdentityMatrix")
    {
        // デフォルトコンストラクタで単位行列が生成される．
        const RotationMatrix3x3 rot_mat;

        // 単位行列の確認．
        CHECK(rot_mat.element[0][0] == doctest::Approx(1.0f));
        CHECK(rot_mat.element[0][1] == doctest::Approx(0.0f));
        CHECK(rot_mat.element[0][2] == doctest::Approx(0.0f));

        CHECK(rot_mat.element[1][0] == doctest::Approx(0.0f));
        CHECK(rot_mat.element[1][1] == doctest::Approx(1.0f));
        CHECK(rot_mat.element[1][2] == doctest::Approx(0.0f));

        CHECK(rot_mat.element[2][0] == doctest::Approx(0.0f));
        CHECK(rot_mat.element[2][1] == doctest::Approx(0.0f));
        CHECK(rot_mat.element[2][2] == doctest::Approx(1.0f));
    }

    TEST_CASE("ConstructorTest_ShouldCreateArbitraryMatrix")
    {
        // 任意の行列を生成する．
        const RotationMatrix3x3 rot_mat(1.0f, 2.0f, 3.0f,
                                        4.0f, 5.0f, 6.0f,
                                        7.0f, 8.0f, 9.0f);

        // 任意の行列の確認．
        CHECK(rot_mat.element[0][0] == doctest::Approx(1.0f));
        CHECK(rot_mat.element[0][1] == doctest::Approx(2.0f));
        CHECK(rot_mat.element[0][2] == doctest::Approx(3.0f));

        CHECK(rot_mat.element[1][0] == doctest::Approx(4.0f));
        CHECK(rot_mat.element[1][1] == doctest::Approx(5.0f));
        CHECK(rot_mat.element[1][2] == doctest::Approx(6.0f));

        CHECK(rot_mat.element[2][0] == doctest::Approx(7.0f));
        CHECK(rot_mat.element[2][1] == doctest::Approx(8.0f));
        CHECK(rot_mat.element[2][2] == doctest::Approx(9.0f));
    }

    TEST_CASE("ArithmeticOperatorTest")
    {
        SUBCASE("Multiplication")
        {
            // 任意の行列を生成する．
            const RotationMatrix3x3 rot_mat1(1.0f, 2.0f, 3.0f,
                                             4.0f, 5.0f, 6.0f,
                                             7.0f, 8.0f, 9.0f);

            // 任意の行列を生成する．
            const RotationMatrix3x3 rot_mat2(9.0f, 8.0f, 7.0f,
                                             6.0f, 5.0f, 4.0f,
                                             3.0f, 2.0f, 1.0f);

            const RotationMatrix3x3 actual = rot_mat1 * rot_mat2;

            // 任意の行列を生成する．
            const RotationMatrix3x3 expect(30.0f, 24.0f, 18.0f,
                                             84.0f, 69.0f, 54.0f,
                                             138.0f, 114.0f, 90.0f);

            // 任意の行列の確認．
            CHECK(actual.element[0][0] == doctest::Approx(expect.element[0][0]));
            CHECK(actual.element[0][1] == doctest::Approx(expect.element[0][1]));
            CHECK(actual.element[0][2] == doctest::Approx(expect.element[0][2]));

            CHECK(actual.element[1][0] == doctest::Approx(expect.element[1][0]));
            CHECK(actual.element[1][1] == doctest::Approx(expect.element[1][1]));
            CHECK(actual.element[1][2] == doctest::Approx(expect.element[1][2]));

            CHECK(actual.element[2][0] == doctest::Approx(expect.element[2][0]));
            CHECK(actual.element[2][1] == doctest::Approx(expect.element[2][1]));
            CHECK(actual.element[2][2] == doctest::Approx(expect.element[2][2]));
        }
    }

    TEST_CASE("CreateRotationMatrixXTest")
    {
        using designlab::math_util::ConvertDegToRad;

        const float angle = ConvertDegToRad(30.0f);

        const RotationMatrix3x3 rot_mat = RotationMatrix3x3::CreateRotationMatrixX(angle);

        const float cos_angle = std::cos(angle);
        const float sin_angle = std::sin(angle);

        CHECK(rot_mat.element[0][0] == doctest::Approx(1.0f));
        CHECK(rot_mat.element[0][1] == doctest::Approx(0.0f));
        CHECK(rot_mat.element[0][2] == doctest::Approx(0.0f));

        CHECK(rot_mat.element[1][0] == doctest::Approx(0.0f));
        CHECK(rot_mat.element[1][1] == doctest::Approx(cos_angle));
        CHECK(rot_mat.element[1][2] == doctest::Approx(-sin_angle));

        CHECK(rot_mat.element[2][0] == doctest::Approx(0.0f));
        CHECK(rot_mat.element[2][1] == doctest::Approx(sin_angle));
        CHECK(rot_mat.element[2][2] == doctest::Approx(cos_angle));
    }

    TEST_CASE("CreateRotationMatrixYTest")
    {
        using designlab::math_util::ConvertDegToRad;

        const float angle = ConvertDegToRad(30.0f);

        const RotationMatrix3x3 rot_mat = RotationMatrix3x3::CreateRotationMatrixY(angle);

        const float cos_angle = std::cos(angle);
        const float sin_angle = std::sin(angle);

        CHECK(rot_mat.element[0][0] == doctest::Approx(cos_angle));
        CHECK(rot_mat.element[0][1] == doctest::Approx(0.0f));
        CHECK(rot_mat.element[0][2] == doctest::Approx(sin_angle));

        CHECK(rot_mat.element[1][0] == doctest::Approx(0.0f));
        CHECK(rot_mat.element[1][1] == doctest::Approx(1.0f));
        CHECK(rot_mat.element[1][2] == doctest::Approx(0.0f));

        CHECK(rot_mat.element[2][0] == doctest::Approx(-sin_angle));
        CHECK(rot_mat.element[2][1] == doctest::Approx(0.0f));
        CHECK(rot_mat.element[2][2] == doctest::Approx(cos_angle));
    }

    TEST_CASE("CreateRotationMatrixZTest")
    {
        using designlab::math_util::ConvertDegToRad;

        const float angle = ConvertDegToRad(30.0f);

        const RotationMatrix3x3 rot_mat = RotationMatrix3x3::CreateRotationMatrixZ(angle);

        const float cos_angle = std::cos(angle);
        const float sin_angle = std::sin(angle);

        CHECK(rot_mat.element[0][0] == doctest::Approx(cos_angle));
        CHECK(rot_mat.element[0][1] == doctest::Approx(-sin_angle));
        CHECK(rot_mat.element[0][2] == doctest::Approx(0.0f));

        CHECK(rot_mat.element[1][0] == doctest::Approx(sin_angle));
        CHECK(rot_mat.element[1][1] == doctest::Approx(cos_angle));
        CHECK(rot_mat.element[1][2] == doctest::Approx(0.0f));

        CHECK(rot_mat.element[2][0] == doctest::Approx(0.0f));
        CHECK(rot_mat.element[2][1] == doctest::Approx(0.0f));
        CHECK(rot_mat.element[2][2] == doctest::Approx(1.0f));
    }
}

#endif  // DESIGNLAB_MATH_ROTATION_MATRIX_TEST_H_
