#include "designlab_euler.h"

#include <cmath>
#include <sstream>

#include "designlab_rotation_matrix.h"
#include "designlab_rot_converter.h"


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

	std::string EulerXYZ::ToCsvString() const
	{
		std::stringstream ss;
		ss << *this;
		return ss.str();
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

	Vector3 RotateVector3(const Vector3& vec, const EulerXYZ& rot)
	{
		const RotationMatrix3x3 rot_mat = ToRotationMatrix(rot);

		return RotateVector3(vec, rot_mat);
	}

} // namespace designlab