
//! @file      math_euler.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_MATH_EULER_H_
#define DESIGNLAB_MATH_EULER_H_

#include <string>

#include "math_util.h"
#include "math_vector3.h"


namespace designlab
{

//! @struct EulerXYZ
//! @brief XYZオイラー角を用いた回転を表す構造体．
//! @details
//! XYZオイラー角によって回転を表現する．単位は [rad]．
//! ロール(X軸)，ピッチ(Y軸)，ヨー(Z軸)はそれぞれ右ねじの方向に回転する．
//! @n
//! @subsubsection 参考資料
//! @li https://watako-lab.com/2019/01/23/roll_pitch_yaw/ （アクセス日 2024/4/2)
//!
//! @note 計算時間がかなり掛かるので，
//! 速度を考えるならばクォータニオン(四元数)を用いた回転を推奨する．
//! @see Quaternion
struct EulerXYZ final
{
    //! @brief デフォルトコンストラクタでは 0,0,0 で初期化する．
    constexpr EulerXYZ() : x_angle(0.f), y_angle(0.f), z_angle(0.f) {}

    //! @brief rad単位で初期化する．
    //! @param[in] x X軸周りの回転．[rad]
    //! @param[in] y Y軸周りの回転．[rad]
    //! @param[in] z Z軸周りの回転．[rad]
    constexpr EulerXYZ(const float x, const float y, const float z) :
        x_angle(x),
        y_angle(y),
        z_angle(z)
    {
    }

    constexpr EulerXYZ(const EulerXYZ& other) = default;
    constexpr EulerXYZ(EulerXYZ&& other) noexcept = default;
    constexpr EulerXYZ& operator =(const EulerXYZ& other) = default;

    ~EulerXYZ() = default;

    constexpr EulerXYZ operator *(const float s) const noexcept
    {
        return { x_angle * s, y_angle * s, z_angle * s };
    }


    constexpr bool operator ==(const EulerXYZ& other) const noexcept
    {
        using math_util::IsEqual;

        return IsEqual(x_angle, other.x_angle) && IsEqual(y_angle, other.y_angle) && IsEqual(z_angle, other.z_angle);
    }

    constexpr bool operator !=(const EulerXYZ& other) const noexcept
    {
        return !(*this == other);
    }


    //! @brief オイラー角を 単位 度 [deg] で初期化する．
    //! @param[in] x X軸周りの回転．[deg]
    //! @param[in] y Y軸周りの回転．[deg]
    //! @param[in] z Z軸周りの回転．[deg]
    constexpr void SetDeg(const float x, const float y, const float z)
    {
        x_angle = math_util::ConvertDegToRad(x);
        y_angle = math_util::ConvertDegToRad(y);
        z_angle = math_util::ConvertDegToRad(z);
    }

    //! @brief オイラー角を文字列に変換する．
    //! 単位は ラジアン [rad]
    //! @return オイラー角を表す文字列．
    [[nodiscard]] std::string ToString() const;

    //! @brief オイラー角をCsv形式の文字列に変換する．カンマ区切り．
    //! 単位は ラジアン [rad]
    //! @return オイラー角を表す文字列．
    [[nodiscard]] std::string ToCsvString() const;

    //! @brief オイラー角を文字列に変換する．
    //! 単位は 度 [deg]．
    //! @return オイラー角を表す文字列．
    [[nodiscard]] std::string ToStringDeg() const;

    //! @brief オイラー角を 単位[deg] で作成する．
    //! @param[in] x X軸周りの回転．[deg]
    //! @param[in] y Y軸周りの回転．[deg]
    //! @param[in] z Z軸周りの回転．[deg]
    //! @return オイラー角．
    [[nodiscard]] static constexpr EulerXYZ MakeEulerXYZDeg(const float x, const float y, const float z)
    {
        return EulerXYZ{ math_util::ConvertDegToRad(x), math_util::ConvertDegToRad(y), math_util::ConvertDegToRad(z) };
    }

    float x_angle;  //!< X 軸周りの回転 [rad]
    float y_angle;  //!< Y 軸周りの回転 [rad]
    float z_angle;  //!< Z 軸周りの回転 [rad]
};


//! @brief 出力ストリーム．Csv形式で出力する．カンマ区切り．単位は [rad]．
template <class Char>
inline std::basic_ostream<Char>& operator <<(std::basic_ostream<Char>& os, const EulerXYZ& r)
{
    using math_util::FloatingPointNumToString;

    os << FloatingPointNumToString(r.x_angle) << Char(',') << FloatingPointNumToString(r.y_angle) << Char(',') << FloatingPointNumToString(r.z_angle);

    return os;
}

//! @brief 入力ストリーム
template <class Char>
inline std::basic_istream<Char>& operator >>(std::basic_istream<Char>& is, EulerXYZ& r)
{
    Char unused = 0;
    return is >> unused >> r.x_angle >> unused >> r.y_angle >> unused >> r.z_angle >> unused;
}


//! @brief 回転させたベクトルを返す．三角関数の処理が多く重たいので注意．
//! @param[in] vec 位置ベクトル．
//! @param[in] rot 回転ベクトル．
//! @return 回転した後の位置ベクトル．
[[nodiscard]] Vector3 RotateVector3(const Vector3& vec, const EulerXYZ& rot);

}  // namespace designlab


#endif  // DESIGNLAB_MATH_EULER_H_
