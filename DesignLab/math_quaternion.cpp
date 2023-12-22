#include "math_quaternion.h"

#include <sstream>

#include "cassert_define.h"


namespace designlab
{

Quaternion Quaternion::GetNormalized() const noexcept
{
	//ノルムが0の場合は，(0,0,0,0)を返す．
	const float norm = GetNorm();
	if (norm == 0.f) { return { 0.f,0.f,0.f,0.f }; }

	return *this * (1.f / norm);
}

Quaternion Quaternion::MakeByAngleAxis(const float angle, const Vector3& axis)
{
	// オイラー角をクオータニオンに変換．

	const float half_angle = angle * 0.5f;

	return { cosf(half_angle) , Vector3{ axis.x ,axis.y,axis.z }.GetNormalized() * sinf(half_angle) };
}

std::string Quaternion::ToString() const
{
	std::string str;
	str += "( w: " + math_util::ConvertFloatToString(w) +
		", x: " + math_util::ConvertFloatToString(v.x) +
		", y: " + math_util::ConvertFloatToString(v.y) +
		", z: " + math_util::ConvertFloatToString(v.z) + " )";
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
	assert(0 <= t && t <= 1);	// tが0~1の間に収まっているか確認．

	if (q1 == q2) { return q1; }	// クォータニオンが等しい場合は，q1を返す．

	// 球面線形補間を行う．
	float dot = q1.Dot(q2);		// 内積．

	if (1.0f < dot) { dot = 1.0f; }			// 内積が1より小さい場合は，1にする．
	else if (dot < -1.0f) { dot = -1.0f; }	// 内積が-1より大きい場合は，-1にする．

	const float theta = acosf(dot);	// 角度
	if (math_util::IsEqual(theta, 0.f)) { return q1; }	// 角度が0の場合は，q1を返す．

	const float sin_theta = sinf(theta);		// sinθ
	const float sin_theta_inv = 1 / sin_theta;	// 1 / sinθ

	const float sin_t_theta = sinf(t * theta);			// sin(tθ)
	const float sin_1_t_theta = sinf((1 - t) * theta);	// sin((1-t)θ)

	return sin_1_t_theta * sin_theta_inv * q1 + sin_t_theta * sin_theta_inv * q2;	// 補間されたクォータニオンを返す．
}

}	// namespace designlab