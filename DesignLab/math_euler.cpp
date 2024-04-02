
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#include "math_euler.h"

#include <cmath>
#include <sstream>

#include "math_rotation_matrix.h"
#include "math_rot_converter.h"


namespace designlab
{

std::string EulerXYZ::ToString() const
{
    std::string str;

    str += "( x-axis: "
        + math_util::FloatingPointNumToString(x_angle) + " [rad], y-axis : "
        + math_util::FloatingPointNumToString(y_angle) + " [rad], z-axis: "
        + math_util::FloatingPointNumToString(z_angle) + " [rad]) xyz-euler angles";

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
    using math_util::FloatingPointNumToString;
    using math_util::ConvertRadToDeg;

    std::string str;

    str += "( x-axis: " + FloatingPointNumToString(ConvertRadToDeg(x_angle)) + " [deg], y-axis : " +
        FloatingPointNumToString(ConvertRadToDeg(y_angle)) +
        " [deg], z-axis: " + FloatingPointNumToString(ConvertRadToDeg(z_angle)) +
        " [deg]) xyz-euler angles";

    return str;
}

Vector3 RotateVector3(const Vector3& vec, const EulerXYZ& rot)
{
    const RotationMatrix3x3 rot_mat = ToRotationMatrix(rot);

    return RotateVector3(vec, rot_mat);
}

}  // namespace designlab
