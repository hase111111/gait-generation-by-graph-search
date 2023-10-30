#include "designlab_rot_converter.h"


designlab::Quaternion designlab::ToQuaternion(const RotationMatrix3x3& rot)
{
	designlab::Quaternion q;

	q.w = sqrt(1.0f + rot.element[0][0] + rot.element[1][1] + rot.element[2][2]) / 2.0f;

	const float w4 = (4.0f * q.w);
	q.v.x = (rot.element[2][1] - rot.element[1][2]) / w4;
	q.v.y = (rot.element[0][2] - rot.element[2][0]) / w4;
	q.v.z = (rot.element[1][0] - rot.element[0][1]) / w4;

	q.Normalize();

	return q;
}

designlab::RotationMatrix3x3 designlab::ToRotationMatrix(const Quaternion& q)
{
	designlab::RotationMatrix3x3 mat;
	designlab::Quaternion q_norm = q.GetNormalized();

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

designlab::RotationMatrix3x3 designlab::ToRotationMatrix(const EulerXYZ& q)
{
	const auto mat_x = RotationMatrix3x3::CreateRotationMatrixX(q.x_angle);
	const auto mat_y = RotationMatrix3x3::CreateRotationMatrixY(q.y_angle);
	const auto mat_z = RotationMatrix3x3::CreateRotationMatrixZ(q.z_angle);

	//_ XÅ®YÅ®ZÇÃèáÇ…âÒì]Ç∑ÇÈÅD
	return mat_z * mat_y * mat_x;
}

designlab::EulerXYZ designlab::ToEulerXYZ(const RotationMatrix3x3& rot)
{
	return {
		std::atan2(rot.element[2][1], rot.element[2][2]),
		std::atan2(-rot.element[2][0], std::sqrt(rot.element[2][1] * rot.element[2][1] + rot.element[2][2] * rot.element[2][2])),
		std::atan2(rot.element[1][0], rot.element[0][0])
	};
}
