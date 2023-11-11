//! @file designlab_vector2.h
//! @brief 2次元の位置ベクトルを表す構造体

#ifndef DESIGNLAB_VECTOR2_H_
#define DESIGNLAB_VECTOR2_H_


#include <cmath>
#include <string>

#include "designlab_math_util.h"


namespace designlab
{
	//! @struct designlab::Vector2
	//! @brief 2次元の位置ベクトルを表す構造体
	//! @details 参考 : https://qiita.com/Reputeless/items/96226cfe1282a014b147
	//! @n 本当はテンプレート構造体にするべきだけど，めんどいし...型たぶんfloatのままかえないし.... 
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
			return ::designlab::math_util::IsEqual(x, other.x) && ::designlab::math_util::IsEqual(y, other.y);
		}

		constexpr bool operator!=(const Vector2& other) const { return !(*this == other); }


		//! @brief このベクトルの長さを返す
		//! @return このベクトルの長さ
		//! @note sqrtは重いので，長さの2乗を返すlengthSquare()を使うことを推奨
		float GetLength() const { return std::sqrt(GetSquaredLength()); }

		//! @brief このベクトルの長さの2乗を返す
		//! @return このベクトルの長さの2乗
		constexpr float GetSquaredLength() const noexcept { return Dot(*this); }

		//! @brief このベクトルとotherの内積を返す
		//! @param [in] other 他のベクトル
		//! @return このベクトルとotherの内積
		constexpr float Dot(const Vector2& other) const noexcept { return x * other.x + y * other.y; }

		//! @brief このベクトルとotherの外積を返す
		//! @param [in] other 他のベクトル
		//! @return このベクトルとotherの外積
		//! @note 2次元なので，外積はスカラー
		constexpr float Cross(const Vector2& other) const noexcept { return x * other.y - y * other.x; }

		//! @brief このベクトルとotherの距離を返す
		//! @param [in] other 他のベクトル
		//! @return このベクトルとotherの距離
		float GetDistanceFrom(const Vector2& other) const noexcept { return (other - *this).GetLength(); }

		//! @brief このベクトルを正規化したベクトルを返す
		//! @return 正規化されたベクトル
		Vector2 GetNormalized() const;

		//! @brief このベクトルが0ならばtrue
		//! @return このベクトルが0ならばtrue
		//! @note 誤差を考慮している
		constexpr bool IsZero() const noexcept{ return ::designlab::math_util::IsEqual(x, 0.0f) && ::designlab::math_util::IsEqual(y, 0.0f); }


		//! @brief このベクトルを文字列にして返す
		//! @n (x, y) の形式，小数点以下3桁まで
		//! @return このベクトルを文字列にしたもの
		std::string ToString() const;


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
		return os << ::designlab::math_util::ConvertFloatToString(v.x) << Char(',') << ::designlab::math_util::ConvertFloatToString(v.y);
	}

	template <class Char>
	inline std::basic_istream<Char>& operator >>(std::basic_istream<Char>& is, Vector2& v)
	{
		Char unused;
		return is >> unused >> v.x >> unused >> v.y >> unused;
	}

} // namespace designlab


#endif	// DESIGNLAB_VECTOR2_H_