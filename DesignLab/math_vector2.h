
//! @file      math_vector2.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_MATH_VECTOR2_H_
#define DESIGNLAB_MATH_VECTOR2_H_

#include <cmath>
#include <string>

#include "math_util.h"


namespace designlab
{
//! @struct designlab::Vector2
//! @brief 2次元の位置ベクトルを表す構造体．
//! @details
//! @subsubsection 参考
//! @li Vec2 クラスを作る 
//! https://qiita.com/Reputeless/items/96226cfe1282a014b147
//! ( アクセス日 2024/2/29 )
//!
//! 本当はテンプレート構造体にするべき．
//! しかし，実用的に使用するのは2次元，3次元程度であるため，
//! このようにそれぞれ実装する．
struct Vector2 final
{
    constexpr Vector2() : x(0), y(0) {}
    constexpr Vector2(float x_pos, float y_pos) : x(x_pos), y(y_pos) {}
    constexpr Vector2(const Vector2& other) = default;
    constexpr Vector2(Vector2&& other) noexcept = default;
    constexpr Vector2& operator =(const Vector2& other) = default;

    constexpr Vector2 operator +() const { return *this; }
    constexpr Vector2 operator -() const { return{ -x, -y }; }

    constexpr Vector2 operator +(const Vector2& other) const
    {
        return{ x + other.x, y + other.y };
    }

    constexpr Vector2 operator -(const Vector2& other) const
    {
        return{ x - other.x, y - other.y };
    }

    constexpr Vector2 operator *(float s) const { return{ x * s, y * s }; }
    constexpr Vector2 operator /(float s) const { return{ x / s, y / s }; }

    Vector2& operator +=(const Vector2& other);
    Vector2& operator -=(const Vector2& other);
    Vector2& operator *=(float s);
    Vector2& operator /=(float s);

    constexpr bool operator==(const Vector2& other) const
    {
        return math_util::IsEqual(x, other.x) && math_util::IsEqual(y, other.y);
    }

    constexpr bool operator!=(const Vector2& other) const
    {
        return !(*this == other);
    }


    //! @brief このベクトルの長さの2乗を返す．
    //! @return このベクトルの長さの2乗．
    //! @note sqrt(ルートの計算)が重いのでこちらを使えるなら使うべき．
    [[nodiscard]]
    constexpr float GetSquaredLength() const noexcept { return Dot(*this); }

    //! @brief このベクトルの長さを返す．
    //! @return このベクトルの長さ．
    //! @note sqrtは重いので，長さの2乗を返す GetSquaredLength を使うことを推奨．
    [[nodiscard]]
    float GetLength() const { return std::sqrt(GetSquaredLength()); }

    //! @brief 自分・引数 の内積の結果を返す．
    //! @param[in] other 他のベクトル．
    //! @return このベクトルと other の内積．
    [[nodiscard]]
    constexpr float Dot(const Vector2& other) const noexcept
    {
        return x * other.x + y * other.y;
    }

    //! @brief 自分×引数 の外積の結果を返す．
    //! @param[in] other 他のベクトル．
    //! @return このベクトルと other の外積．
    //! @note 2次元なので，外積はスカラー．
    [[nodiscard]]
    constexpr float Cross(const Vector2& other) const noexcept
    {
        return x * other.y - y * other.x;
    }

    //! @brief このベクトルと other の距離を返す．
    //! @param[in] other 他のベクトル．
    //! @return このベクトルと other の距離．
    [[nodiscard]]
    float GetDistanceFrom(const Vector2& other) const noexcept
    {
        return (other - *this).GetLength();
    }

    //! @brief このベクトルを正規化したベクトルを返す．
    //! @return 正規化されたベクトル．
    [[nodiscard]]
    Vector2 GetNormalized() const;

    //! @brief このベクトルが0ならば true．
    //! @return このベクトルが0ならば true．
    //! @note 誤差を考慮している．
    [[nodiscard]]
    constexpr bool IsZero() const noexcept
    {
        return math_util::IsEqual(x, 0.0f) && math_util::IsEqual(y, 0.0f);
    }

    //! @brief このベクトルを正規化する．
    //! @n 絶対値が0のベクトルの場合，そのまま0ベクトルになる．
    //! @attention この関数は，このベクトルの値を変更する．
    //! GetNormalized は値を変更しない．
    //! 間違えないようにすること．
    void Normalize() noexcept { *this = GetNormalized(); }


    //! @brief 零ベクトルを返す．
    //! 静的な関数なので Vector2::GetZeroVec() と呼ぶことができる．
    //! @return 零ベクトル．
    [[nodiscard]]
    constexpr static Vector2 GetZeroVec() noexcept { return Vector2(0.f, 0.f); }

    //! @brief このベクトルを文字列にして返す．
    //! @n (x, y) の形式，小数点以下3桁まで．
    //! @return このベクトルを文字列にしたもの．
    [[nodiscard]] std::string ToString() const;

    //! @brief このベクトルをCSV形式の文字列にして返す．
    //! @n x, y, z の形式，小数点以下3桁まで．
    //! @return このベクトルをCSV形式の文字列にしたもの．
    [[nodiscard]] std::string ToCsvString() const;


    float x;
    float y;
};


//! @brief スカラーが先に来る場合の掛け算演算子．
//! @param[in] s スカラー．
//! @param[in] v ベクトル．
//! @return s * v．
constexpr Vector2 operator *(float s, const Vector2& v)
{
    return { s * v.x, s * v.y };
}

template <class Char>
std::basic_ostream<Char>& operator <<(std::basic_ostream<Char>& os, const Vector2& v)
{
    return os << math_util::FloatingPointNumToString(v.x) <<
        Char(',') << math_util::FloatingPointNumToString(v.y);
}

template <class Char>
inline std::basic_istream<Char>& operator >>(
    std::basic_istream<Char>& is, Vector2& v)
{
    Char unused = 0;
    return is >> unused >> v.x >> unused >> v.y >> unused;
}

}  // namespace designlab


#endif  // DESIGNLAB_MATH_VECTOR2_H_
