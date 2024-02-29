
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#include "math_quaternion.h"

#include <sstream>

#include "cassert_define.h"


namespace designlab
{

Quaternion Quaternion::GetNormalized() const noexcept
{
    // ノルムが0の場合は，(0,0,0,0)を返す．
    const float norm = GetNorm();

    if (norm == 0.f)
    {
        return { 0.f, 0.f, 0.f, 0.f };
    }

    return *this * (1.f / norm);
}

Quaternion Quaternion::MakeByAngleAxis(const float angle, const Vector3& axis)
{
    // オイラー角をクオータニオンに変換．

    const float half_angle = angle * 0.5f;

    return { cosf(half_angle),
        Vector3{ axis.x, axis.y, axis.z }.GetNormalized() * sinf(half_angle) };
}

std::string Quaternion::ToString() const
{
    std::string str;
    str += "( w: " + math_util::FloatingPointNumToString(w) +
        ", x: " + math_util::FloatingPointNumToString(v.x) +
        ", y: " + math_util::FloatingPointNumToString(v.y) +
        ", z: " + math_util::FloatingPointNumToString(v.z) + " )";
    return str;
}

std::string Quaternion::ToCsvString() const
{
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

Quaternion SlerpQuaternion(const Quaternion& q1, const Quaternion& q2, const float t)
{
    assert(0 <= t && t <= 1);  // tが 0 ～ 1 の間に収まっているか確認．

    if (q1 == q2) { return q1; }  // クォータニオンが等しい場合は，q1を返す．

    // 球面線形補間を行う．
    float dot = q1.Dot(q2);  // 内積．

    if (1.0f < dot)
    {
        // 内積が1より小さい場合は，1にする．
        dot = 1.0f;
    }
    else if (dot < -1.0f)
    {
        // 内積が-1より大きい場合は，-1にする．
        dot = -1.0f;
    }

    const float theta = acosf(dot);  // 角度．

    // 角度が0の場合は，q1を返す．
    if (math_util::IsEqual(theta, 0.f)) { return q1; }

    const float sin_theta = sinf(theta);  // sin(θ)
    const float sin_theta_inv = 1 / sin_theta;  // 1 / sin(θ)

    const float sin_t_theta = sinf(t * theta);  // sin(tθ)
    const float sin_1_t_theta = sinf((1 - t) * theta);  // sin((1-t)θ)

    // 補間されたクォータニオンを返す．
    return sin_1_t_theta * sin_theta_inv * q1 + sin_t_theta * sin_theta_inv * q2;
}

}  // namespace designlab
