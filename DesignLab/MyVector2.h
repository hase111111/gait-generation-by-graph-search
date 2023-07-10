#pragma once
#include <iostream>
#include <cmath>
#include <vector>

namespace my_vec
{
	struct SVector2 final
	{
		float x;
		float y;

		SVector2() = default;

		constexpr SVector2(float _x, float _y)
			: x(_x)
			, y(_y) {}

		float length() const { return std::sqrt(lengthSquare()); }
		constexpr float lengthSquare() const { return dot(*this); }
		constexpr float dot(const SVector2& other) const { return x * other.x + y * other.y; }
		constexpr float cross(const SVector2& other) const { return x * other.y - y * other.x; }
		float distanceFrom(const SVector2& other) const { return (other - *this).length(); }
		SVector2 normalized() const { return *this / length(); }
		constexpr bool isZero() const { return x == 0.0 && y == 0.0; }

		constexpr SVector2 operator +() const { return *this; }
		constexpr SVector2 operator -() const { return{ -x, -y }; }
		constexpr SVector2 operator +(const SVector2& other) const { return{ x + other.x, y + other.y }; }
		constexpr SVector2 operator -(const SVector2& other) const { return{ x - other.x, y - other.y }; }
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

	struct SLine2 final
	{
		SLine2() = default;

		SVector2 start;
		SVector2 end;

		//他の線分との交点を求める
SVector2 intersection(const SLine2& other) const
		{
			const auto v1 = end - start;
			const auto v2 = other.end - other.start;
			const auto v3 = other.start - start;
			const auto d = v1.cross(v2);
			if (d == 0.0f)
			{
				//平行
				return SVector2();
			}
			const auto t1 = v2.cross(v3) / d;
			const auto t2 = v1.cross(v3) / d;
			if (t1 < 0.0f || t1 > 1.0f || t2 < 0.0f || t2 > 1.0f)
			{
				//交点は線分の外
				return SVector2();
			}
			return start + v1 * t1;
		}
	};

	struct SPolygon2 final
	{
		SPolygon2() = default;

		std::vector<SVector2> vertex;

	};
}

//! @struct my_vec::SVector2
//! @brief 2次元のベクトルを表す構造体
//! @details https://qiita.com/Reputeless/items/96226cfe1282a014b147 をほぼもらってきたもの．<br>
//! 本当はテンプレート構造体にするべきだけど，めんどいし...型たぶんfloatのままかえないし.... 

//! @struct my_vec::SPolygon2
//! @brief 2次元の多角形を表す構造体
//! @details 2次元の多角形を表す構造体．
