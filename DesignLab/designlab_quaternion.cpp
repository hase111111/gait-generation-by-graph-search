#include "designlab_quaternion.h"

#include "cassert_define.h"


constexpr designlab::EulerXYZ designlab::Quaternion::toRotator() const
{
	// クオータニオンをXYZオイラー角に変換

	return { 0,0,0 };
}

designlab::Quaternion designlab::Quaternion::MakeByAngleAxis(float angle, const Vector3& axis)
{
	// オイラー角をクオータニオンに変換

	const float kHalfAngle = angle * 0.5f;

	return Quaternion{ cosf(kHalfAngle) , Vector3{ axis.x ,axis.y,axis.z }.Normalize() * sinf(kHalfAngle) };
}

designlab::Vector3 designlab::RotateVector3(const designlab::Vector3& vec, const designlab::Quaternion& q,const bool use_normalized_quaternions)
{
	designlab::Quaternion p{0, vec.x, vec.y, vec.z};	// 回転させるベクトルをスカラーが0のクオータニオンに変換
	designlab::Quaternion res;	// 結果

	if (use_normalized_quaternions)
	{
		// 正規化されたクォータニオンを使う場合は，共役と逆数が等しいので，計算量を減らすことができる

		assert(designlab::math_util::IsEqual(q.Norm(), 1.0f));	// 正規化されたクォータニオンを使う場合は，正規化されたクォータニオンを渡す必要がある
		
		res = q * p * q.Conjugate();	// Q * P * Q^-1 
	}
	else 
	{
		res = q * p * q.Inverse();	// Q * P * Q^-1
	}

	return res.v;	// ベクトル成分を返す
}