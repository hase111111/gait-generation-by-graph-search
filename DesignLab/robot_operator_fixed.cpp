
//! @file robot_operator_fixed.cpp

// Copyright(c) 2023 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

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
