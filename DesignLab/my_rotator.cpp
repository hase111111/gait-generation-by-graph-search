#include "my_rotator.h"

#include <cmath>


my_vec::SVector my_vec::rotVector(const SVector& vec, const SRotator& rot)
{
	const float x = cos(rot.yaw) * cos(rot.pitch) * vec.x
		+ (cos(rot.yaw) * sin(rot.pitch) * sin(rot.roll) - sin(rot.yaw) * cos(rot.roll)) * vec.y
		+ (cos(rot.yaw) * sin(rot.pitch) * cos(rot.roll) + sin(rot.yaw) * sin(rot.roll)) * vec.z;

	const float y = sin(rot.yaw) * cos(rot.pitch) * vec.x
		+ (sin(rot.yaw) * sin(rot.pitch) * sin(rot.roll) + cos(rot.yaw) * cos(rot.roll)) * vec.y
		+ (sin(rot.yaw) * sin(rot.pitch) * cos(rot.roll) - cos(rot.yaw) * sin(rot.roll)) * vec.z;

	const float z = -sin(rot.pitch) * vec.x + cos(rot.pitch) * sin(rot.roll) * vec.y + cos(rot.pitch) * cos(rot.roll) * vec.z;

	return my_vec::SVector{ x, y, z };
}
