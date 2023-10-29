#include "designlab_quaternion.h"

#include "cassert_define.h"


namespace dlm = ::designlab::math_util;


designlab::Quaternion designlab::Quaternion::GetNormalized() const noexcept
{
	//ノルムが0の場合は，(0,0,0,0)を返す
	const float norm = GetNorm();
	if (norm == 0) { return { 0,0,0,0 }; }

	return *this * (1.f / norm);
}

designlab::Quaternion designlab::Quaternion::MakeByAngleAxis(float angle, const Vector3& axis)
{
	// オイラー角をクオータニオンに変換

	const float kHalfAngle = angle * 0.5f;

	return { cosf(kHalfAngle) , Vector3{ axis.x ,axis.y,axis.z }.GetNormalized() * sinf(kHalfAngle) };
}

designlab::Vector3 designlab::RotateVector3(const designlab::Vector3& vec, const designlab::Quaternion& q, const bool use_normalized_quaternions)
{
	const designlab::Quaternion p{0, vec.x, vec.y, vec.z};	// 回転させるベクトルをスカラーが0のクオータニオンに変換
	designlab::Quaternion res;	// 結果

	if (use_normalized_quaternions)
	{
		// 正規化されたクォータニオンを使う場合は，共役と逆数が等しいので，計算量を減らすことができる

		assert(dlm::IsEqual(q.GetNorm(), 1.0f));	// 正規化されたクォータニオンを使う場合は，正規化されたクォータニオンを渡す必要がある
		
		res = q * p * q.GetConjugate();	// Q * P * Q^-1 
	}
	else 
	{
		res = q * p * q.GetInverse();	// Q * P * Q^-1
	}

	return res.v;	// ベクトル成分を返す
}

designlab::Quaternion designlab::SlerpQuaternion(const Quaternion& q1, const Quaternion& q2, const float t)
{
	if(q1 == q2) { return q1; }	// クォータニオンが等しい場合は，q1を返す

	// 球面線形補間を行う
	float dot = q1.Dot(q2);		// 内積
	float theta = acosf(dot);	// 角度
	if (math_util::IsEqual(theta, 0.f)) { return q1; }	// 角度が0の場合は，q1を返す

	float sin_theta = sinf(theta);			// sinθ
	float sin_theta_inv = 1 / sin_theta;	// 1 / sinθ

	float sin_t_theta = sinf(t * theta);			// sin(tθ)
	float sin_1_t_theta = sinf((1 - t) * theta);	// sin((1-t)θ)

	return sin_1_t_theta * sin_theta_inv * q1 + sin_t_theta * sin_theta_inv * q2;	// 補間されたクォータニオンを返す
}