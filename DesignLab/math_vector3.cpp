
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#include "math_vector3.h"

#include <format>
#include <sstream>

#include "math_util.h"


namespace designlab
{

Vector3 Vector3::GetNormalized() const noexcept
{
    const float GetLength = this->GetLength();

    if (math_util::IsEqual(GetLength, 0.0f))
    {
        return { 0.0f, 0.0f, 0.0f };
    }

    // 割り算は遅いので，逆数をかける．
    const float inv_length = 1.0f / GetLength;
    return *this * inv_length;
}

std::string Vector3::ToString() const
{
    return std::format("( x : {}, y : {}, z : {} )",
        math_util::FloatingPointNumToString(x),
        math_util::FloatingPointNumToString(y),
        math_util::FloatingPointNumToString(z));
}

std::string Vector3::ToCsvString() const
{
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

Vector3& Vector3::operator+=(const Vector3& other) noexcept
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& other) noexcept
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3& Vector3::operator*=(const float other) noexcept
{
    x *= other;
    y *= other;
    z *= other;
    return *this;
}

Vector3& Vector3::operator/=(const float other)
{
    x /= other;
    y /= other;
    z /= other;
    return *this;
}

}  // namespace designlab
