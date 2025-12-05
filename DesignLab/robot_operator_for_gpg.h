
//! @file robot_operator_for_gpg.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <vector>

#include "interface_robot_operator.h"
#include "math_util.h"

namespace gaitgen {

//! @class RobotOperatorForGpg
//! @brief global path generator を行うための仮置きクラス.
class RobotOperatorForGpg final : public IRobotOperator {
 public:
  RobotOperatorForGpg();

  RobotOperation Init() const override;

  RobotOperation Update(const RobotStateNode& node) override;

 private:
  //!< 目標角度と現在の角度の許容誤差.
  static constexpr float kAllowableAngleError{math_util::ConvertDegToRad(2.0f)};

  std::vector<Vector3> global_route_;  //!< グローバルパス.
};

}  // namespace gaitgen
