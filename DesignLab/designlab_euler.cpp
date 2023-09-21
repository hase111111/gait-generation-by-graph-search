#include "designlab_euler.h"

#include <cmath>

namespace dlm = ::designlab::math_util;


namespace designlab 
{
	std::string EulerXYZ::ToString() const
	{
		std::string str;

		str += "( x-axis: " 
			+ dlm::ConvertFloatToString(x_angle) + " [rad], y-axis : " 
			+ dlm::ConvertFloatToString(y_angle) + " [rad], z-axis: "
			+ dlm::ConvertFloatToString(z_angle) + " [rad]) xyz-euler angles";

		return str;
	}

	std::string EulerXYZ::ToStringDeg() const
	{
		std::string str;

		str += "( x-axis: "
			+ dlm::ConvertFloatToString(dlm::ConvertRadToDeg(x_angle)) + " [deg], y-axis : "
			+ dlm::ConvertFloatToString(dlm::ConvertRadToDeg(y_angle)) + " [deg], z-axis: "
			+ dlm::ConvertFloatToString(dlm::ConvertRadToDeg(z_angle)) + " [deg]) xyz-euler angles";

		return str;
	}

	Vector3 rotVector(const Vector3& vec, const EulerXYZ& rot)
	{
		const float x = cos(rot.z_angle) * cos(rot.y_angle) * vec.x
			+ (cos(rot.z_angle) * sin(rot.y_angle) * sin(rot.x_angle) - sin(rot.z_angle) * cos(rot.x_angle)) * vec.y
			+ (cos(rot.z_angle) * sin(rot.y_angle) * cos(rot.x_angle) + sin(rot.z_angle) * sin(rot.x_angle)) * vec.z;

		const float y = sin(rot.z_angle) * cos(rot.y_angle) * vec.x
			+ (sin(rot.z_angle) * sin(rot.y_angle) * sin(rot.x_angle) + cos(rot.z_angle) * cos(rot.x_angle)) * vec.y
			+ (sin(rot.z_angle) * sin(rot.y_angle) * cos(rot.x_angle) - cos(rot.z_angle) * sin(rot.x_angle)) * vec.z;

		const float z = -sin(rot.y_angle) * vec.x + cos(rot.y_angle) * sin(rot.x_angle) * vec.y + cos(rot.y_angle) * cos(rot.x_angle) * vec.z;

		return Vector3{ x, y, z };
	}

} // namespace designlab