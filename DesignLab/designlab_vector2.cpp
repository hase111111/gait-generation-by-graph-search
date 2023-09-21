#include "designlab_vector2.h"


namespace dlm = ::designlab::math_util;	// í∑Ç∑Ç¨ÇÈÇÃÇ≈ math_utilÇÃï ñºÇçÏê¨


namespace designlab 
{
	Vector2& Vector2::operator +=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2& Vector2::operator -=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector2& Vector2::operator *=(float s)
	{
		x *= s;
		y *= s;
		return *this;
	}

	Vector2& Vector2::operator /=(float s)
	{
		x /= s;
		y /= s;
		return *this;
	}

	Vector2 Vector2::Normalized() const
	{
		float length = Length();

		if (dlm::IsEqual(length,0))
		{
			return { 0,0 };
		}

		return *this * (1.f / length);
	}

	std::string Vector2::ToString() const
	{
		return std::string("( x : ") + dlm::ConvertFloatToString(x) + std::string(", y : ") + dlm::ConvertFloatToString(y) + std::string(")");
	}

} // namespace designlab