
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#include "math_rotation_matrix.h"

#include <cmath>

#include "math_util.h"


namespace designlab
{

RotationMatrix3x3 RotationMatrix3x3::operator*(const RotationMatrix3x3& other) const
{
    RotationMatrix3x3 res;

    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            res.element[i][j] = 0.0f;

            for (size_t k = 0; k < 3; k++)
            {
                res.element[i][j] += element[i][k] * other.element[k][j];
            }
        }
    }

    return res;
}

RotationMatrix3x3 RotationMatrix3x3::CreateRotationMatrixX(const float angle)
{
    // cosは重いので，使うときはキャッシュしておく．
    const float cos_angle = std::cos(angle);
    const float sin_angle = std::sin(angle);  // 同上．

    return RotationMatrix3x3(
      1.0f, 0.0f, 0.0f,
      0.0f, cos_angle, -sin_angle,
      0.0f, sin_angle, cos_angle);
}

RotationMatrix3x3 RotationMatrix3x3::CreateRotationMatrixY(const float angle)
{
    // cosは重いので，使うときはキャッシュしておく．
    const float cos_angle = std::cos(angle);
    const float sin_angle = std::sin(angle);  // 同上．

    return RotationMatrix3x3(
      cos_angle, 0.0f, sin_angle,
      0.0f, 1.0f, 0.0f,
      -sin_angle, 0.0f, cos_angle);
}

RotationMatrix3x3 RotationMatrix3x3::CreateRotationMatrixZ(const float angle)
{
    // cosは重いので，使うときはキャッシュしておく．
    const float cos_angle = std::cos(angle);
    const float sin_angle = std::sin(angle);  // 同上．

    return RotationMatrix3x3(
      cos_angle, -sin_angle, 0.0f,
      sin_angle, cos_angle, 0.0f,
      0.0f, 0.0f, 1.0f);
}

std::string RotationMatrix3x3::ToString() const
{
    std::string res;

    res += math_util::FloatingPointNumToString(element[0][0]) + "," +
        math_util::FloatingPointNumToString(element[0][1]) + "," +
        math_util::FloatingPointNumToString(element[0][2]) + "\n";

    res += math_util::FloatingPointNumToString(element[1][0]) + "," +
        math_util::FloatingPointNumToString(element[1][1]) + "," +
        math_util::FloatingPointNumToString(element[1][2]) + "\n";

    res += math_util::FloatingPointNumToString(element[2][0]) + "," +
        math_util::FloatingPointNumToString(element[2][1]) + "," +
        math_util::FloatingPointNumToString(element[2][2]) + "\n";

    return res;
}

Vector3 RotateVector3(const Vector3& vec, const RotationMatrix3x3& rot)
{
    Vector3 res;

    res.x = rot.element[0][0] * vec.x +
        rot.element[0][1] * vec.y + rot.element[0][2] * vec.z;
    res.y = rot.element[1][0] * vec.x +
        rot.element[1][1] * vec.y + rot.element[1][2] * vec.z;
    res.z = rot.element[2][0] * vec.x +
        rot.element[2][1] * vec.y + rot.element[2][2] * vec.z;

    return res;
}

}  // namespace designlab
