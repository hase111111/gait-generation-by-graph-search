
//! @file      math_quaternion.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_MATH_QUATERNION_H_
#define DESIGNLAB_MATH_QUATERNION_H_

#include <cmath>
#include <string>

#include "cassert_define.h"
#include "math_euler.h"
#include "math_util.h"
#include "math_vector3.h"


namespace designlab
{

//! @struct designlab::Quaternion
//! @brief クォータニオンを表す構造体．
//! @details クォータニオンは，スカラー成分とベクトル成分からなる．
//! 四元数とも呼ばれる．
//! @subsubsection 参考
//! @li Zenn クォータニオン
//! https://zenn.dev/mebiusbox/books/132b654aa02124/viewer/2966c7
//! ( アクセス日 2024/2/29 )
//! @li SPORTS SENSING，運動計測概説>クォータニオン4：クォータニオンの演算
//! https://www.sports-sensing.com/brands/labss/motionmeasurement/
//! ( アクセス日 2024/2/29 )
//! @li F_ クォータニオンと回転
//! https://www.f-sp.com/entry/2017/06/30/221124
//! ( アクセス日 2024/2/29 )
struct Quaternion final
{
    //! @brief 1 + {0,0,0}で初期化する，
    constexpr Quaternion() : w(1.0f), v(0.0f, 0.0f, 0.0f) {}

    //! @brief スカラー成分とベクトル成分を指定して初期化する．
    //! ノルムが1になるように代入すること，
    //! @n 使用は非推奨，MakeByAngleAxisを使うこと．
    //! @param[in] w_ スカラー成分．
    //! @param[in] x_ ベクトル成分のx成分．
    //! @param[in] y_ ベクトル成分のy成分．
    //! @param[in] z_ ベクトル成分のz成分．
    constexpr Quaternion(const float w_,
        const float x_, const float y_, const float z_) :
        w(w_),
        v(x_, y_, z_) {}

    //! @brief スカラー成分とベクトル成分を指定して初期化する．
    //! ノルムが1になるように代入すること，
    //! @n 使用は非推奨，MakeByAngleAxisを使うこと．
    //! @param[in] w_ スカラー成分．
    //! @param[in] v_ ベクトル成分．
    constexpr Quaternion(const float w_, const Vector3& v_) : w(w_), v(v_) {}

    //! コピーコンストラクタ．
    constexpr Quaternion(const Quaternion& q) = default;

    //! ムーブコンストラクタ．
    constexpr Quaternion(Quaternion&& q) noexcept = default;

    //! コピー代入演算子．
    constexpr Quaternion& operator =(const Quaternion& q) = default;

    constexpr Quaternion operator + () const noexcept { return *this; }
    constexpr Quaternion operator - () const noexcept { return { -w, -v }; }
    constexpr Quaternion operator + (const Quaternion& q) const noexcept
    {
        return { w + q.w, v + q.v };
    }
    constexpr Quaternion operator - (const Quaternion& q) const noexcept
    {
        return { w - q.w, v - q.v };
    }
    constexpr Quaternion operator * (const Quaternion& q) const noexcept
    {
        return { w * q.w - v.Dot(q.v), w * q.v + q.w * v + v.Cross(q.v) };
    }
    constexpr Quaternion operator * (const float s) const noexcept
    {
        return { w * s, v * s };
    }
    constexpr Quaternion operator / (const float s) const
    {
        return { w / s, v / s };
    }

    bool operator == (const Quaternion& q) const noexcept
    {
        return math_util::IsEqual(w, q.w) && (v == q.v);
    }
    bool operator != (const Quaternion& q) const noexcept { return !(*this == q); }

    bool operator < (const Quaternion& q) const noexcept
    {
        return (w < q.w) || (w == q.w && v < q.v);
    }
    bool operator > (const Quaternion& other) const noexcept
    {
        return other < *this;
    }

    bool operator <= (const Quaternion& other) const noexcept
    {
        return !(*this > other);
    }

    bool operator >= (const Quaternion& other) const noexcept
    {
        return !(*this < other);
    }


    //! @brief クォータニオンの内積を返す．
    //! クォータニオンを4次元のベクトルとみなし，ベクトルの内積を求める．
    //! @param[in] other 内積を求めるクォータニオン．
    //! @return 内積．
    [[nodiscard]]
    constexpr float Dot(Quaternion other) const noexcept
    {
        return w * other.w + v.Dot(other.v);
    }

    //! @brief クォータニオンの共役を返す．
    //! 共役なクォータニオンとは，ベクトル成分の符号を反転させたもの
    //! @n q = w + xi + yj + zk とすると，
    //! qの共役は w - xi - yj - zk となる．回転は逆方向になる
    //! @return 共役クォータニオン
    [[nodiscard]]
    constexpr Quaternion GetConjugate() const noexcept { return { w, -v }; }

    //! @brief クォータニオンの長さの2乗を返す(ノルムの2乗)．
    //! @n クォータニオンの長さの2乗は，w^2 + x^2 + y^2 + z^2 で求められる．
    //! @return 長さの2乗．
    [[nodiscard]]
    constexpr float GetLengthSquared() const noexcept
    {
        return w * w + v.Dot(v);
    }

    //! @brief クォータニオンのノルムを返す．
    //! @n ノルムとは，ベクトルの大きさのこと．
    //! クォータニオンのノルムは，w^2 + x^2 + y^2 + z^2 の平方根で求められる．
    //! @return ノルム．
    [[nodiscard]]
    inline float GetNorm() const noexcept
    {
        return std::sqrt(GetLengthSquared());
    }

    //! @brief クォータニオンの逆数を返す．
    //! @n クォータニオンqの逆数q^-1は，qの共役をノルムで割ったもの．
    //! @n q^-1 = q* / |q|^2
    //! @return 逆数．
    [[nodiscard]]
    inline Quaternion GetInverse() const
    {
        return GetConjugate() * (1 / GetNorm());
    }

    //! @brief 正規化したクォータニオンを返す．
    //! @n クォータニオンの正規化とは，ノルムを1にすることを表す．
    //! @n クォータニオンqの正規化は，q / |q| で求められる．
    //! @return 正規化されたクォータニオン．
    [[nodiscard]] Quaternion GetNormalized() const noexcept;

    //! @brief 他のクォータニオンとの距離の2乗を返す．
    //! クォータニオンを4次元ベクトルとみなし，ベクトルの距離の2乗を求める．
    //! @return 距離の2乗．
    [[nodiscard]]
    constexpr float GetDistanceSquared(const Quaternion& q) const noexcept
    {
        return (*this - q).GetLengthSquared();
    }

    //! @brief 自身を正規化する．ノルムが１になる．
    inline void Normalize() noexcept { *this = GetNormalized(); }

    //! @brief 左手座標系への変換を行う．
    //! @n 言うまでもないが，機械工学では通例右手座標系を使う．
    //! しかし，dxlib は左手座標系なので，dxlib で描画するときは，
    //! この関数を使って左手座標系に変換する必要がある．
    //! @return 左手座標系に変換されたクォータニオン．
    [[nodiscard]] constexpr Quaternion ToLeftHandCoordinate() const noexcept
    {
        return { w, v.x, -v.y, v.z };
    }

    //! @brief 回転軸と回転角からクォータニオンを作成する．
    //! @n q = cos(θ/2) * w + sin(θ/2) * { v.x  + v.y  + v.z } となる．
    //! ノルムは必ず1になる．
    //! @param[in] rad_angle 回転角θ [rad]
    //! @param[in] axis 回転軸．
    //! @return 指定された回転軸周りに回転したクォータニオン．
    [[nodiscard]]
    static Quaternion MakeByAngleAxis(float rad_angle, const Vector3& axis);


    //! @brief クォータニオンを文字列に変換する．
    //! @n w, x, y, z の順で出力する．
    //! @return クォータニオンを表す文字列．
    [[nodiscard]] std::string ToString() const;

    //! @brief クォータニオンをCsv形式の文字列に変換する．カンマ区切り．
    //! @n w, x, y, z の順にカンマ区切りで出力する．
    //! @return クォータニオンを表す文字列．
    [[nodiscard]] std::string ToCsvString() const;

    float w;    //!< スカラー成分．
    Vector3 v;  //!< ベクトル成分．
};


constexpr Quaternion operator * (float s, const Quaternion& q) { return q * s; }

//! 出力ストリーム．
template <class Char>
inline std::basic_ostream<Char>& operator <<(
    std::basic_ostream<Char>& os, const Quaternion& q)
{
    os << math_util::FloatingPointNumToString(q.w) << Char(',') << q.v;

    return os;
}

//! 入力ストリーム．
template <class Char>
inline std::basic_istream<Char>& operator >>(
    std::basic_istream<Char>& is, Quaternion& q)
{
    Char unused = 0;
    return is >> unused >> q.w >> unused >> q.v;
}

//! @brief 3次元の位置ベクトルを回転させる．必ず正規化クォータニオンを用いること．
//! @param[in] vec 回転させるベクトル．
//! @param[in] q 回転させるクォータニオン．
//! @n 正規化クォータニオンならば，共役と逆数が等しいので，
//! 計算量を減らすことができる．
//! @return 回転後のベクトル．
[[nodiscard]]
constexpr Vector3 RotateVector3(const Vector3& vec, const Quaternion& q)
{
    // 回転させるベクトルをスカラーが0のクオータニオンに変換．
    const Quaternion p{ 0, vec.x, vec.y, vec.z };

    // 正規化されたクォータニオンを使う場合は，共役と逆数が等しいので，
    // 計算量を減らすことができる．

    // 正規化されたクォータニオンを使う場合は，正規化されたクォータニオンを渡す必要がある．
    assert(math_util::IsEqual(q.GetNorm(), 1.0f));

    return (q * p * q.GetConjugate()).v;  // ベクトル成分を返す．
}

//! @brief 球面線形補間を行う．
//! @param[in] q1 クォータニオン1．
//! @param[in] q2 クォータニオン2．
//! @param[in] t 補間係数．0～1の値を取る．
//! @return 補間されたクォータニオン．
Quaternion SlerpQuaternion(const Quaternion& q1, const Quaternion& q2, float t);

}  // namespace designlab


#endif  // DESIGNLAB_MATH_QUATERNION_H_
