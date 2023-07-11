#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include "MyMath.h"

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

		//! @brief このベクトルを正規化したベクトルを返す
		//! @return 正規化されたベクトル
		//! @note 長さが0の場合を考慮していないので注意
		SVector2 normalized() const { return *this / length(); }

		constexpr bool isZero() const { return my_math::isEqual(x, 0.0f) && my_math::isEqual(y, 0.0f); }

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

		constexpr SLine2(const SVector2& start, const SVector2& end)
			: start(start), end(end)
		{}

		SVector2 start;
		SVector2 end;

		//! @brief 他の線分との交点を求める
		//! @param[in] other 他の線分
		//! @return 交点．交点がない場合は(0, 0)を返す．
		constexpr SVector2 getIntersection(const SLine2& other) const
		{
			const SVector2 v1 = end - start;
			const SVector2 v2 = other.end - other.start;
			const float d = v1.cross(v2);

			if (my_math::isEqual(d, 0.0f) == true)
			{
				return SVector2(0,0);	//平行
			}

			const SVector2 v3 = other.start - start;

			const float t1 = v2.cross(v3) / d;
			const float t2 = v1.cross(v3) / d;
			
			if (t1 < 0.0f - my_math::ALLOWABLE_ERROR || t1 > 1.0f + my_math::ALLOWABLE_ERROR || t2 < 0.0f - my_math::ALLOWABLE_ERROR || t2 > 1.0f + my_math::ALLOWABLE_ERROR)
			{
				return SVector2(0, 0);	//交点は線分の外
			}

			return start + v1 * t1;
		}

		//! @brief 線分が接触しているかどうか調べる関数
		//! @param[in] other 他の線分
		//!	@return 接触しているかどうか
		constexpr bool isContact(const SLine2& other) const
		{
			const auto v1 = end - start;
			const auto v2 = other.end - other.start;
			const auto v3 = other.start - start;
			const auto d = v1.cross(v2);

			if (my_math::isEqual(d, 0.0f) == true)
			{
				return false;	//平行
			}

			const auto t1 = v2.cross(v3) / d;
			const auto t2 = v1.cross(v3) / d;

			if (t1 < 0.0f + my_math::ALLOWABLE_ERROR || t1 > 1.0f - my_math::ALLOWABLE_ERROR || t2 < 0.0f + my_math::ALLOWABLE_ERROR || t2 > 1.0f - my_math::ALLOWABLE_ERROR)
			{
				return false;	//交点は線分の外
			}

			return true;
		}

		//! @brief 線分の長さを求める関数
		//! @return 線分の長さ
		inline float getLength() const
		{
			return (end - start).length();
		}


		bool operator==(const SLine2& other) const
		{
			return start == other.start && end == other.end;
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
