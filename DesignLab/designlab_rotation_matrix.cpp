#include "designlab_rotation_matrix.h"

#include <cmath>


namespace designlab 
{
	// 回転行列について
	// https://w3e.kanazawa-it.ac.jp/math/category/gyouretu/senkeidaisu/henkan-tex.cgi?target=/math/category/gyouretu/senkeidaisu/rotation_matrix.html
	// https://programming-surgeon.com/script/euler-angle/

	RotationMatrix3x3::RotationMatrix3x3(const EulerXYZ& euler_xyz)
	{
		RotationMatrix3x3 rot_x = CreateRotationMatrixX(euler_xyz.x_angle);
		RotationMatrix3x3 rot_y = CreateRotationMatrixY(euler_xyz.y_angle);
		RotationMatrix3x3 rot_z = CreateRotationMatrixZ(euler_xyz.z_angle);

		*this = rot_z * rot_y * rot_x;
	}

	RotationMatrix3x3& RotationMatrix3x3::operator*(const RotationMatrix3x3& other)
	{
		RotationMatrix3x3 res;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				res.element[i][j] = 0.0f;

				for (int k = 0; k < 3; k++)
				{
					res.element[i][j] += element[i][k] * other.element[k][j];
				}
			}
		}

		*this = res;

		return *this;
	}

	RotationMatrix3x3 RotationMatrix3x3::CreateRotationMatrixX(const float angle)
	{
		const float cos_angle = std::cos(angle);	// cosは重いので，使うときはキャッシュしておく
		const float sin_angle = std::sin(angle);	// 同上

		return RotationMatrix3x3(
			1.0f, 0.0f, 0.0f,
			0.0f, cos_angle, -sin_angle,
			0.0f, sin_angle, cos_angle
		);
	}

	RotationMatrix3x3 RotationMatrix3x3::CreateRotationMatrixY(const float angle)
	{
		const float cos_angle = std::cos(angle);	// cosは重いので，使うときはキャッシュしておく
		const float sin_angle = std::sin(angle);	// 同上

		return RotationMatrix3x3(
			cos_angle, 0.0f, sin_angle,
			0.0f, 1.0f, 0.0f,
			-sin_angle, 0.0f, cos_angle
		);
	}

	RotationMatrix3x3 RotationMatrix3x3::CreateRotationMatrixZ(const float angle)
	{
		const float cos_angle = std::cos(angle);	// cosは重いので，使うときはキャッシュしておく
		const float sin_angle = std::sin(angle);	// 同上

		return RotationMatrix3x3(
			cos_angle, -sin_angle, 0.0f,
			sin_angle, cos_angle, 0.0f,
			0.0f, 0.0f, 1.0f
		);
	}

	Vector3 rotVector(const Vector3& vec, const RotationMatrix3x3& rot)
	{
		Vector3 res;

		res.x = rot.element[0][0] * vec.x + rot.element[0][1] * vec.y + rot.element[0][2] * vec.z;
		res.y = rot.element[1][0] * vec.x + rot.element[1][1] * vec.y + rot.element[1][2] * vec.z;
		res.z = rot.element[2][0] * vec.x + rot.element[2][1] * vec.y + rot.element[2][2] * vec.z;

		return res;
	}

}	// namespace designlab