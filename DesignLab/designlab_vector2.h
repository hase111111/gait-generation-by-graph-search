//! @file designlab_vector2.h
//! @brief 2次元の位置ベクトルを表す構造体．

#ifndef DESIGNLAB_VECTOR2_H_
#define DESIGNLAB_VECTOR2_H_

#include <cmath>
#include <string>

#include "designlab_math_util.h"


namespace designlab
{
//! @struct designlab::Vector2
//! @brief 2次元の位置ベクトルを表す構造体．
//! @details 参考 : https://qiita.com/Reputeless/items/96226cfe1282a014b147
//! @n 本当はテンプレート構造体にするべき，しかし，実用的に使用するのは2次元，3次元程度であるため，このように分けて作成する． 
struct Vector2 final
{
	constexpr Vector2() : x(0), y(0) {};
	constexpr Vector2(float x_pos, float y_pos) : x(x_pos), y(y_pos) {}
	constexpr Vector2(const Vector2& other) = default;
	constexpr Vector2(Vector2&& other) noexcept = default;
	constexpr Vector2& operator =(const Vector2& other) = default;

	constexpr Vector2 operator +() const { return *this; }
	constexpr Vector2 operator -() const { return{ -x, -y }; }
	constexpr Vector2 operator +(const Vector2& other) const { return{ x + other.x, y + other.y }; }
	constexpr Vector2 operator -(const Vector2& other) const { return{ x - other.x, y - other.y }; }
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

	constexpr bool operator!=(const Vector2& other) const { return !(*this == other); }


	//! @brief このベクトルの長さを返す．
	//! @return float このベクトルの長さ．
	//! @note sqrtは重いので，長さの2乗を返す GetSquaredLength を使うことを推奨．
	[[maybe_unused]] float GetLength() const { return std::sqrt(GetSquaredLength()); }

	//! @brief このベクトルの長さの2乗を返す．
	//! @return float このベクトルの長さの2乗．
	[[maybe_unused]] constexpr float GetSquaredLength() const noexcept { return Dot(*this); }

	//! @brief このベクトルとotherの内積を返す．
	//! @param [in] other 他のベクトル．
	//! @return float このベクトルとotherの内積．
	[[maybe_unused]] constexpr float Dot(const Vector2& other) const noexcept { return x * other.x + y * other.y; }

	//! @brief このベクトルとotherの外積を返す
	//! @param [in] other 他のベクトル
	//! @return float このベクトルとotherの外積
	//! @note 2次元なので，外積はスカラー
	[[maybe_unused]] constexpr float Cross(const Vector2& other) const noexcept { return x * other.y - y * other.x; }

	//! @brief このベクトルとotherの距離を返す
	//! @param [in] other 他のベクトル
	//! @return float このベクトルとotherの距離
	[[maybe_unused]] float GetDistanceFrom(const Vector2& other) const noexcept { return (other - *this).GetLength(); }

	//! @brief このベクトルを正規化したベクトルを返す．
	//! @return Vector2 正規化されたベクトル．
	Vector2 GetNormalized() const;

	//! @brief このベクトルが0ならばtrue．
	//! @return bool このベクトルが0ならばtrue．
	//! @note 誤差を考慮している
	[[maybe_unused]] constexpr bool IsZero() const noexcept { return math_util::IsEqual(x, 0.0f) && math_util::IsEqual(y, 0.0f); }


	//! @brief このベクトルを文字列にして返す．
	//! @n (x, y) の形式，小数点以下3桁まで．
	//! @return std::string このベクトルを文字列にしたもの．
	[[maybe_unused]] std::string ToString() const;


	float x;
	float y;
};


constexpr Vector2 operator *(float s, const Vector2& v)
{
	return { s * v.x, s * v.y };
}

template <class Char>
std::basic_ostream<Char>& operator <<(std::basic_ostream<Char>& os, const Vector2& v)
{
	return os << math_util::ConvertFloatToString(v.x) << Char(',') << math_util::ConvertFloatToString(v.y);
}

template <class Char>
inline std::basic_istream<Char>& operator >>(std::basic_istream<Char>& is, Vector2& v)
{
	Char unused;
	return is >> unused >> v.x >> unused >> v.y >> unused;
}

} // namespace designlab


#endif	// DESIGNLAB_VECTOR2_H_