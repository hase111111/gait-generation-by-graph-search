#include "math_euler.h"

#include <cmath>
#include <sstream>

#include "designlab_rotation_matrix.h"
#include "designlab_rot_converter.h"


namespace designlab
{

std::string EulerXYZ::ToString() const
{
	std::string str;

	str += "( x-axis: "
		+ math_util::ConvertFloatToString(x_angle) + " [rad], y-axis : "
		+ math_util::ConvertFloatToString(y_angle) + " [rad], z-axis: "
		+ math_util::ConvertFloatToString(z_angle) + " [rad]) xyz-euler angles";

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
		+ math_util::ConvertFloatToString(math_util::ConvertRadToDeg(x_angle)) + " [deg], y-axis : "
		+ math_util::ConvertFloatToString(math_util::ConvertRadToDeg(y_angle)) + " [deg], z-axis: "
		+ math_util::ConvertFloatToString(math_util::ConvertRadToDeg(z_angle)) + " [deg]) xyz-euler angles";

	return str;
}

Vector3 RotateVector3(const Vector3& vec, const EulerXYZ& rot)
{
	const RotationMatrix3x3 rot_mat = ToRotationMatrix(rot);

	return RotateVector3(vec, rot_mat);
}

} // namespace designlab