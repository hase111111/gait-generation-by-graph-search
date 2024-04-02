
//! @file      math_vector3.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_MATH_VECTOR3_H_
#define DESIGNLAB_MATH_VECTOR3_H_

#include <cmath>
#include <string>

#include "math_util.h"
#include "math_vector2.h"


namespace designlab
{

//! @struct Vector3
//! @brief 3次元の位置ベクトルを表す構造体．
//! @details
//! 座標系はロボットの進行方向にXの正，ロボットの上向きにZの正，
//! 右手座標系にYをとっている．
//! @n
//! @n ヘッダファイル内に実装を書くのは個人的には避けたいのだが，
//! constexpr関数を使う場合，このようにする必要がある．
//! constexpr関数はコンパイル時に値を計算できることを，
//! コンパイラに伝えるためのものである．
//! (C言語のマクロのような動作ができる)
//! 実行速度が大切なプロジェクトであるため，このように処理を記述する．
//! @n
//! @subsubsection 参考
//! @li C++、constexprのまとめ https://qiita.com/KRiver1/items/ef7731467b5ca83850cb ( アクセス日 2024/2/29 )
//! @li AB - 3.04.構造体>細かい話>コンストラクタ https://atcoder.jp/contests/apg4b/tasks/APG4b_ab?lang=ja ( アクセス日 2024/2/29 )
//! @li 非クラス関数による演算子オーバーロード https://programming.pc-note.net/cpp/operator2.html ( アクセス日 2024/2/29 )
//! @li 衝突判定編 http://marupeke296.com/COL_main.html ( アクセス日 2024/2/29 )
struct Vector3 final
{
    //!< デフォルトコンストラクタ．(0,0,0)で初期化される．
    constexpr Vector3() : x(0.f), y(0.f), z(0.f) {}

    constexpr Vector3(const float x_pos, const float y_pos, const float z_pos) :
        x(x_pos),
        y(y_pos),
        z(z_pos)
    {
    }

    //! コピーコンストラクタ．
    constexpr Vector3(const Vector3& other) = default;

    //! ムーブコンストラクタ．
    constexpr Vector3(Vector3&& other) noexcept = default;

    //! 代入演算子．
    constexpr Vector3& operator = (const Vector3& other) = default;

    ~Vector3() = default;


    //! @brief 2つのベクトルが等しいかどうかを返す．誤差を許容する．
    constexpr bool operator == (const Vector3& v) const noexcept
    {
        if (math_util::IsEqual(v.x, x) && math_util::IsEqual(v.y, y) && math_util::IsEqual(v.z, z))
        {
            return true;
        }

        return false;
    }

    constexpr bool operator != (const Vector3& other) const noexcept
    {
        return !(*this == other);
    }

    bool operator < (const Vector3& other) const noexcept
    {
        // 長さで比較する．
        return GetLength() < other.GetLength();
    }

    bool operator > (const Vector3& other) const noexcept
    {
        return other < *this;
    }

    bool operator <= (const Vector3& other) const noexcept
    {
        return !(*this > other);
    }

    bool operator >= (const Vector3& other) const noexcept
    {
        return !(*this < other);
    }

    // 算術演算子，ベクトル同士の掛け算(内積・外積)はメンバ関数として実装する．
    constexpr Vector3 operator +() const noexcept { return *this; }

    constexpr Vector3 operator -() const noexcept { return { -x, -y, -z }; }

    constexpr Vector3 operator +(const Vector3& other) const noexcept
    {
        return { x + other.x , y + other.y, z + other.z };
    }

    constexpr Vector3 operator -(const Vector3& other) const noexcept
    {
        return { x - other.x , y - other.y, z - other.z };
    }

    constexpr Vector3 operator *(const float num) const noexcept
    {
        return { x * num, y * num, z * num };
    }

    constexpr Vector3 operator /(const float num) const
    {
        return { x / num, y / num, z / num };
    }

    Vector3& operator += (const Vector3& other) noexcept;
    Vector3& operator -= (const Vector3& other) noexcept;
    Vector3& operator *= (const float num) noexcept;
    Vector3& operator /= (const float num);


    //! @brief ベクトルの長さの2乗を返す．
    //! @return x,y,zの値を2乗にして足し合わせたスカラー値．
    //! @note sqrt(ルートの計算)が重いのでこちらを使えるなら使うべき．
    [[nodiscard]] constexpr float GetSquaredLength() const noexcept
    {
        return x * x + y * y + z * z;
    }

    //! @brief ベクトルの長さを返す．
    //! @return ベクトルの長さ．
    //! @note sqrt(ルートの計算)が重いので，
    //! GetSquaredLength でいいならそちらを使うべき．
    [[nodiscard]] inline float GetLength() const noexcept
    {
        return std::sqrt(GetSquaredLength());
    }

    //! @brief 自分・引数 の内積の結果を返す．
    //! @param[in] other もう一方のベクトル．
    //! @return 内積の結果．
    [[nodiscard]] constexpr float Dot(const Vector3& other) const noexcept
    {
        return x * other.x + y * other.y + z * other.z;
    }

    //! @brief 自分×引数 の外積の結果を返す．
    //! @param[in] other 外積の掛け算：後ろのベクトル．
    //! @return 外積の結果．
    //! このベクトル→引数へ回転する右ねじが進む方向のベクトルが出力される．
    [[nodiscard]] constexpr Vector3 Cross(const Vector3& other) const noexcept
    {
        return Vector3{ y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
    }

    //! @brief 別のベクトルと，このベクトルの距離を返す．
    //! @param[in] other 別のベクトル．
    //! @return 距離．
    [[nodiscard]] inline float GetDistanceFrom(const Vector3& other) const noexcept
    {
        return (*this - other).GetLength();
    }

    //! @brief 単位ベクトルを返す．
    //! normalizeとは，ベクトルを正規化（単位ベクトルに変換）する操作を表す．
    //! @n 絶対値が0のベクトルの場合，そのまま0ベクトルを返す．
    //! @return 正規化されたベクトル．
    [[nodiscard]] Vector3 GetNormalized() const noexcept;

    //! @brief x,y,zともに絶対値が0ならば true を返す．
    //! @return 0ならば true そうでなければ false．
    //! @note 誤差を許容する．
    [[nodiscard]] constexpr bool IsZero() const noexcept
    {
        if (math_util::IsEqual(x, 0.0f) && math_util::IsEqual(y, 0.0f) && math_util::IsEqual(z, 0.0f))
        {
            return true;
        }

        return false;
    }

    //! @brief このベクトルを正規化する．
    //! @n 絶対値が0のベクトルの場合，そのまま0ベクトルになる．
    //! @attention この関数は，このベクトルの値を変更する．
    //! GetNormalized() は値を変更しない．
    //! 間違えないようにすること．
    void Normalize() noexcept { *this = GetNormalized(); }

    //! @brief XY平面に射影したベクトルを返す．
    //! @return XY平面に射影したベクトル．
    [[nodiscard]]
    constexpr Vector2 ProjectedXY() const noexcept { return { x, y }; }

    //! @brief 正面に進む単位ベクトルを返す．
    //! 静的な関数なので，Vector3::GetFrontVec() と呼び出せる．
    //! @code
    //! Vector3 vec = Vector3::GetFrontVec();
    //! @endcode
    //! @return 正面方向の単位ベクトル，xの正方向．
    [[nodiscard]]
    constexpr static Vector3 GetFrontVec() noexcept { return { 1, 0, 0 }; }

    //! @brief 左に進む単位ベクトルを返す．
    //! 静的な関数なので，Vector3::GetLeftVec() と呼び出せる．
    //! @return 左方向の単位ベクトル，yの正方向．
    [[nodiscard]]
    constexpr static Vector3 GetLeftVec() noexcept { return { 0, 1, 0 }; }

    //! @brief 上に進む単位ベクトルを返す．
    //! 静的な関数なので，Vector3::GetUpVec() と呼び出せる．
    //! @return 上方向の単位ベクトル，zの正方向．
    [[nodiscard]]
    constexpr static Vector3 GetUpVec() noexcept { return { 0, 0, 1 }; }

    //! @brief 零ベクトルを返す．
    //! 静的な関数なので，Vector3::GetZeroVec() と呼び出せる．
    //! @return 零ベクトル．
    [[nodiscard]]
    constexpr static Vector3 GetZeroVec() noexcept { return { 0, 0, 0 }; }


    //! @brief このベクトルを文字列にして返す．
    //! @n (x, y, z) の形式，小数点以下3桁まで．
    //! @return このベクトルを文字列にしたもの．
    [[nodiscard]] std::string ToString() const;

    //! @brief このベクトルをCSV形式の文字列にして返す．
    //! @n x, y, z の形式，小数点以下3桁まで．
    //! @return このベクトルをCSV形式の文字列にしたもの．
    [[nodiscard]] std::string ToCsvString() const;


    float x;  //!< ロボットの正面方向に正．
    float y;  //!< ロボットの左向きに正．
    float z;  //!< ロボットの上向きに正．
};


//! @brief スカラーが先に来る場合の掛け算演算子．
//! @param[in] s スカラー．
//! @param[in] vec ベクトル．
//! @return スカラーとベクトルの掛け算結果．
constexpr Vector3 operator * (const float s, const Vector3& vec) noexcept
{
    return { s * vec.x, s * vec.y, s * vec.z };
}


//! @brief 出力ストリーム．
//! ToCsvString と同じ形式で出力する．
template <class Char>
std::basic_ostream<Char>& operator <<(std::basic_ostream<Char>& os, const Vector3& v)
{
    return os << math_util::FloatingPointNumToString(v.x) << Char(',') <<
        math_util::FloatingPointNumToString(v.y) << Char(',') <<
        math_util::FloatingPointNumToString(v.z);
}


//! 入力ストリーム．
template <class Char>
inline std::basic_istream<Char>& operator >>(std::basic_istream<Char>& is, Vector3& v)
{
    Char unused = 0;
    return is >> v.x >> unused >> v.y >> unused >> v.z;
}


// 正規化直行座標系となっているか確認する，
// 条件を満たさないならばコンパイルが通らない．
static_assert(math_util::IsEqual(Vector3::GetFrontVec().Dot(Vector3::GetLeftVec()), 0.f), "The dot product of FrontVec and LeftVec is not zero.");

static_assert(math_util::IsEqual(Vector3::GetFrontVec().Dot(Vector3::GetUpVec()), 0.f), "The dot product of FrontVec and UpVec is not zero.");

static_assert(math_util::IsEqual(Vector3::GetLeftVec().Dot(Vector3::GetUpVec()), 0.f), "The dot product of LeftVec and UpVec is not zero.");

static_assert(math_util::IsEqual(Vector3::GetFrontVec().GetSquaredLength(), 1.f), "FrontVec is not normalized.");

static_assert(math_util::IsEqual(Vector3::GetLeftVec().GetSquaredLength(), 1.f), "LeftVec is not normalized.");

static_assert(math_util::IsEqual(Vector3::GetUpVec().GetSquaredLength(), 1.f), "UpVec is not normalized.");

// 0ベクトルが返ってくるか確認する．
static_assert(Vector3::GetZeroVec().x == 0.f, "It is not a 0 vector.");
static_assert(Vector3::GetZeroVec().y == 0.f, "It is not a 0 vector.");
static_assert(Vector3::GetZeroVec().z == 0.f, "It is not a 0 vector.");

}  // namespace designlab


#endif  // DESIGNLAB_MATH_VECTOR3_H_
