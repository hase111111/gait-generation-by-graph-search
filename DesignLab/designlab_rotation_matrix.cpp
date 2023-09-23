#include "designlab_rotation_matrix.h"

#include <cmath>


namespace designlab 
{
	// 回転行列について
	// https://w3e.kanazawa-it.ac.jp/math/category/gyouretu/senkeidaisu/henkan-tex.cgi?target=/math/category/gyouretu/senkeidaisu/rotation_matrix.html

	RotationMatrix3x3::RotationMatrix3x3(const EulerXYZ& euler_xyz)
	{
		const float cos_x = std::cos(euler_xyz.x_angle);
		const float sin_x = std::sin(euler_xyz.x_angle);
		const float cos_y = std::cos(euler_xyz.y_angle);
		const float sin_y = std::sin(euler_xyz.y_angle);
		const float cos_z = std::cos(euler_xyz.z_angle);
		const float sin_z = std::sin(euler_xyz.z_angle);

		element[0][0] = cos_y * cos_z;
		element[0][1] = cos_y * sin_z;
		element[0][2] = -sin_y;

		element[1][0] = sin_x * sin_y * cos_z - cos_x * sin_z;
		element[1][1] = sin_x * sin_y * sin_z + cos_x * cos_z;
		element[1][2] = sin_x * cos_y;

		element[2][0] = cos_x * sin_y * cos_z + sin_x * sin_z;
		element[2][1] = cos_x * sin_y * sin_z - sin_x * cos_z;
		element[2][2] = cos_x * cos_y;
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

}	// namespace designlab