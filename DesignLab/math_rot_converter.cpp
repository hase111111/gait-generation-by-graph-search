
#include "math_rot_converter.h"

#include "math_util.h"


namespace designlab
{

Quaternion ToQuaternion(const RotationMatrix3x3& rot)
{
    Quaternion q;

    q.w = sqrt(1.0f + rot.element[0][0] + rot.element[1][1] + rot.element[2][2]) / 2.0f;

    const float w4 = (4.0f * q.w);
    q.v.x = (rot.element[2][1] - rot.element[1][2]) / w4;
    q.v.y = (rot.element[0][2] - rot.element[2][0]) / w4;
    q.v.z = (rot.element[1][0] - rot.element[0][1]) / w4;

    q.Normalize();

    return q;
}

Quaternion ToQuaternion(const EulerXYZ& e)
{
    const float cos_x = std::cos(e.x_angle / 2.0f);
    const float cos_y = std::cos(e.y_angle / 2.0f);
    const float cos_z = std::cos(e.z_angle / 2.0f);
    const float sin_x = std::sin(e.x_angle / 2.0f);
    const float sin_y = std::sin(e.y_angle / 2.0f);
    const float sin_z = std::sin(e.z_angle / 2.0f);

    Quaternion q;
    q.w = cos_x * cos_y * cos_z + sin_x * sin_y * sin_z;
    q.v.x = sin_x * cos_y * cos_z - cos_x * sin_y * sin_z;
    q.v.y = cos_x * sin_y * cos_z + sin_x * cos_y * sin_z;
    q.v.z = cos_x * cos_y * sin_z - sin_x * sin_y * cos_z;

    if (math_util::IsEqual(q.GetNorm(), 0.0f))
    {
        return Quaternion{};
    }
    else
    {
        assert(math_util::IsEqual(q.GetNorm(), 1.0f));
        return q;
    }
}

RotationMatrix3x3 ToRotationMatrix(const Quaternion& q)
{
    RotationMatrix3x3 mat;
    Quaternion q_norm = q.GetNormalized();

    const float x2 = q_norm.v.x * q_norm.v.x;
    const float y2 = q_norm.v.y * q_norm.v.y;
    const float z2 = q_norm.v.z * q_norm.v.z;
    const float xy = q_norm.v.x * q_norm.v.y;
    const float xz = q_norm.v.x * q_norm.v.z;
    const float yz = q_norm.v.y * q_norm.v.z;
    const float wx = q_norm.w * q_norm.v.x;
    const float wy = q_norm.w * q_norm.v.y;
    const float wz = q_norm.w * q_norm.v.z;

    mat.element[0][0] = 1.0f - 2.0f * (y2 + z2);
    mat.element[0][1] = 2.0f * (xy - wz);
    mat.element[0][2] = 2.0f * (xz + wy);

    mat.element[1][0] = 2.0f * (xy + wz);
    mat.element[1][1] = 1.0f - 2.0f * (x2 + z2);
    mat.element[1][2] = 2.0f * (yz - wx);

    mat.element[2][0] = 2.0f * (xz - wy);
    mat.element[2][1] = 2.0f * (yz + wx);
    mat.element[2][2] = 1.0f - 2.0f * (x2 + y2);

    return mat;
}

RotationMatrix3x3 ToRotationMatrix(const EulerXYZ& q)
{
    const auto mat_x = RotationMatrix3x3::CreateRotationMatrixX(q.x_angle);
    const auto mat_y = RotationMatrix3x3::CreateRotationMatrixY(q.y_angle);
    const auto mat_z = RotationMatrix3x3::CreateRotationMatrixZ(q.z_angle);

    // X→Y→Zの順に回転する．
    return mat_z * mat_y * mat_x;
}

EulerXYZ ToEulerXYZ(const RotationMatrix3x3& rot)
{
    float x = std::atan2(rot.element[2][1], rot.element[2][2]);
    float y = std::atan2(-rot.element[2][0],
        std::sqrt(rot.element[2][1] * rot.element[2][1] + rot.element[2][2] * rot.element[2][2]));
    float z = std::atan2(rot.element[1][0], rot.element[0][0]);

    return EulerXYZ{ x, y, z };
}

EulerXYZ ToEulerXYZ(const Quaternion& q)
{
    // クォータニオンを回転行列に変換してから，回転行列をオイラー角に変換する．
    return ToEulerXYZ(ToRotationMatrix(q));
}


}  // namespace designlab
