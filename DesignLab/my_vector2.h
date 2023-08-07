#pragma once

#include <iostream>
#include <cmath>
#include <vector>

#include "my_math.h"


namespace my_vec
{
	//! @struct my_vec::SVector2
	//! @date 2023/08/06
	//! @auther 長谷川
	//! @brief 2次元のベクトルを表す構造体
	//! @details 参考 : https://qiita.com/Reputeless/items/96226cfe1282a014b147
	//! @n 本当はテンプレート構造体にするべきだけど，めんどいし...型たぶんfloatのままかえないし.... 
	struct SVector2 final
	{

		SVector2() = default;

		constexpr SVector2(float x_pos, float y_pos) : x(x_pos), y(y_pos) {}


		constexpr SVector2 operator +() const { return *this; }
		constexpr SVector2 operator -() const { return{ -x, -y }; }
		constexpr SVector2 operator +(const SVector2& other) const { return{ x + other.x, y + other.y }; }
		constexpr SVector2 operator +(const float s) const { return{ x + s, y + s }; }
		constexpr SVector2 operator -(const SVector2& other) const { return{ x - other.x, y - other.y }; }
		constexpr SVector2 operator -(const float s) const { return{ x - s, y - s }; }
		constexpr SVector2 operator *(float s) const { return{ x * s, y * s }; }
		constexpr SVector2 operator /(float s) const { return{ x / s, y / s }; }

		SVector2& operator +=(const SVector2& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		SVector2& operator -=(const SVector2& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		SVector2& operator *=(float s)
		{
			x *= s;
			y *= s;
			return *this;
		}

		SVector2& operator /=(float s)
		{
			x /= s;
			y /= s;
			return *this;
		}

		constexpr bool operator==(const SVector2& other) const
		{
			return my_math::isEqual(x, other.x) && my_math::isEqual(y, other.y);
		}


		//! @brief このベクトルの長さを返す
		//! @return このベクトルの長さ
		//! @note sqrtは重いので，長さの2乗を返すlengthSquare()を使うことを推奨
		float length() const { return std::sqrt(lengthSquare()); }

		//! @brief このベクトルの長さの2乗を返す
		//! @return このベクトルの長さの2乗
		constexpr float lengthSquare() const { return dot(*this); }

		//! @brief このベクトルとotherの内積を返す
		//! @param [in] other 他のベクトル
		//! @return このベクトルとotherの内積
		constexpr float dot(const SVector2& other) const { return x * other.x + y * other.y; }

		//! @brief このベクトルとotherの外積を返す
		//! @param [in] other 他のベクトル
		//! @return このベクトルとotherの外積
		//! @note 2次元なので，外積はスカラー
		constexpr float cross(const SVector2& other) const { return x * other.y - y * other.x; }

		//! @brief このベクトルとotherの距離を返す
		//! @param [in] other 他のベクトル
		//! @return このベクトルとotherの距離
		float distanceFrom(const SVector2& other) const { return (other - *this).length(); }

		//! @brief このベクトルを正規化したベクトルを返す
		//! @return 正規化されたベクトル
		//! @note 長さが0の場合を考慮していないので注意
		SVector2 normalized() const { return *this / length(); }

		//! @brief このベクトルが0ならばtrue
		//! @return このベクトルが0ならばtrue
		//! @note 誤差を考慮している
		constexpr bool isZero() const { return my_math::isEqual(x, 0.0f) && my_math::isEqual(y, 0.0f); }


		float x;
		float y;
	};


	inline constexpr SVector2 operator *(float s, const SVector2& v)
	{
		return{ s * v.x, s * v.y };
	}


	template <class Char>
	inline std::basic_ostream<Char>& operator <<(std::basic_ostream<Char>& os, const SVector2& v)
	{
		return os << Char('(') << v.x << Char(',') << v.y << Char(')');
	}


	template <class Char>
	inline std::basic_istream<Char>& operator >>(std::basic_istream<Char>& is, SVector2& v)
	{
		Char unused;
		return is >> unused >> v.x >> unused >> v.y >> unused;
	}

} // namespace my_vec


//! @file my_vector2.h
//! @date 2023/08/06
//! @auther 長谷川
//! @brief 2次元のベクトルを表す構造体
//! @n 行数 : @lineinfo