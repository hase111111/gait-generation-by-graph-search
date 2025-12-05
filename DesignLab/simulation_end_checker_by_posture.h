
//! @file simulation_end_checker_by_posture.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include "interface_simulation_end_checker.h"
#include "math_euler.h"
#include "math_quaternion.h"

namespace gaitgen {

//! @class SimulationEndCheckerByPosture
//! @brief 最終姿勢によるシミュレーション終了判定クラス.
class SimulationEndCheckerByPosture final : public ISimulationEndChecker {
 public:
  SimulationEndCheckerByPosture(const Quaternion& goal_orientation,
                                float allowable_error);

  bool IsEnd(const RobotStateNode& node) const override;

 private:
  const Quaternion goal_orientation_;  //!< 目標姿勢.
  const EulerXYZ goal_euler_;          //!< 目標姿勢.

  const float allowable_error_;  //!< 許容誤差.
};

}  // namespace gaitgen
