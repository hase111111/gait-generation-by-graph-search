
//! @file      math_rotation_matrix.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_MATH_ROTATION_MATRIX_H_
#define DESIGNLAB_MATH_ROTATION_MATRIX_H_

#include <array>
#include <string>

#include "math_euler.h"
#include "math_vector3.h"


namespace designlab
{

//! @struct designlab::RotationMatrix3x3
//! @brief 3次元の回転行列を表す構造体．
//! @details
//! 回転行列については以下の参考資料を確認すること．
//! @subsubsection 参考
//! @li ３次元における回転座標変換行列
//! https://rikei-tawamure.com/entry/2019/11/04/184049
//! ( アクセス日 2024/2/29 )
//! @li 回転角度と回転行列　オイラー角とは
//! https://programming-surgeon.com/script/euler-angle/
//! ( アクセス日 2024/2/29 )
struct RotationMatrix3x3 final
{
    //! @brief 単位行列を生成する．
    RotationMatrix3x3() :
        element({ {
          { 1.0f, 0.0f, 0.0f },
          { 0.0f, 1.0f, 0.0f },
          { 0.0f, 0.0f, 1.0f }} }) {}

    //! @brief 任意の回転行列を生成する．
    RotationMatrix3x3(
      const float r11, const float r12, const float r13,
      const float r21, const float r22, const float r23,
      const float r31, const float r32, const float r33) :
        element({ {
          { r11, r12, r13 },
          { r21, r22, r23 },
          { r31, r32, r33 }} }) {}

    RotationMatrix3x3(const RotationMatrix3x3& other) = default;
    RotationMatrix3x3(RotationMatrix3x3&& other) noexcept = default;
    RotationMatrix3x3& operator =(const RotationMatrix3x3& other) = default;
    ~RotationMatrix3x3() = default;

    RotationMatrix3x3 operator* (const RotationMatrix3x3& other) const;


    //! @brief x軸周りに回転する回転行列を生成する．
    //! @param[in] angle 回転角 [rad]
    //! @return x軸周りに回転する回転行列．
    [[nodiscard]] static RotationMatrix3x3 CreateRotationMatrixX(float angle);

    //! @brief y軸周りに回転する回転行列を生成する．
    //! @param[in] angle 回転角 [rad]
    //! @return y軸周りに回転する回転行列．
    [[nodiscard]] static RotationMatrix3x3 CreateRotationMatrixY(float angle);

    //! @brief z軸周りに回転する回転行列を生成する．
    //! @param[in] angle 回転角 [rad]
    //! @return z軸周りに回転する回転行列．
    [[nodiscard]] static RotationMatrix3x3 CreateRotationMatrixZ(float angle);


    //! @brief 回転行列を文字列に変換する．
    //! @return 回転行列を表す文字列．
    [[nodiscard]] std::string ToString() const;


    //! データの並びについて．
    //! @n | R11 R12 R13 |
    //! @n | R21 R22 R23 |
    //! @n | R31 R32 R33 |
    //! @n
    //! @n R11は element[0][0]，R12は element[0][1]，R32は element[2][1]となる．
    //! @n つまり，element[ 行 - 1 ][ 列 - 1 ]となる．
    std::array<std::array<float, 3>, 3> element;
};


//! @brief 回転させたベクトルを返す．
//! @param[in] vec 回転させるベクトル．
//! @param[in] rot 回転行列．
//! @return 回転させたベクトル．
[[nodiscard]]
Vector3 RotateVector3(const Vector3& vec, const RotationMatrix3x3& rot);

}  // namespace designlab


#endif  // DESIGNLAB_MATH_ROTATION_MATRIX_H_
