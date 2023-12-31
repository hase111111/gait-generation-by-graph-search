
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "robot_operator_fixed.h"


namespace designlab
{

RobotOperatorFixed::RobotOperatorFixed(const RobotOperation& operation) : operation_(operation)
{
}

RobotOperation RobotOperatorFixed::Init() const
{
    return operation_;
}

RobotOperation RobotOperatorFixed::Update([[maybe_unused]] const RobotStateNode& state)
{
    return operation_;
}

}  // namespace designlab
