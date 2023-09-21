#include "designlab_quaternion.h"


designlab::Vector3 designlab::rotVecByQuat(const designlab::Vector3& vec, const SQuaternion& q)
{
	designlab::SQuaternion p{0, vec.x, vec.y, vec.z};
	designlab::SQuaternion qInv = q.inverse();
	designlab::SQuaternion r = q * p * qInv;

	return Vector3{ r.v.x, r.v.y, r.v.z };
}

constexpr designlab::EulerXYZ designlab::SQuaternion::toRotator() const
{
	// クオータニオンをXYZオイラー角に変換

	return { 0,0,0 };
}

void designlab::SQuaternion::setRotAngleAndAxis(float angle, const Vector3& axis)
{
	// オイラー角をクオータニオンに変換

	const float kHalfAngle = angle * 0.5f;
	const float kSinHalfAngle = sinf(kHalfAngle);

	v.x = axis.x * kSinHalfAngle;
	v.y = axis.y * kSinHalfAngle;
	v.z = axis.z * kSinHalfAngle;
	w = cosf(kHalfAngle);

	*this = normalize();
}
