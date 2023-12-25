
#include "math_vector2.h"


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

Vector2 Vector2::GetNormalized() const
{
    float length = GetLength();

    if (math_util::IsEqual(length, 0.f))
    {
        return { 0, 0 };
    }

    return *this * (1.f / length);
}

std::string Vector2::ToString() const
{
    return std::string("( x : ") +
        math_util::FloatingPointNumToString(x) +
        std::string(", y : ") +
        math_util::FloatingPointNumToString(y) +
        std::string(")");
}

std::string Vector2::ToCsvString() const
{
    return math_util::FloatingPointNumToString(x) + std::string(",") +
        math_util::FloatingPointNumToString(y);
}

}  // namespace designlab
