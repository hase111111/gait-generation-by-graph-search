
//! @file simulation_end_checker_by_position.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "simulation_end_checker_by_position.h"

namespace designlab {

SimulationEndCheckerByPosition::SimulationEndCheckerByPosition(
    const Vector3& goal_position, const float allowable_error)
    : goal_position_(goal_position), allowable_error_(allowable_error) {}

bool SimulationEndCheckerByPosition::IsEnd(const RobotStateNode& node) const {
  // 位置を取得し,誤差を計算.
  const float dif =
      (node.center_of_mass_global_coord - goal_position_).GetLength();

  if (dif > allowable_error_) {
    return false;
  }

  return true;
}

}  // namespace designlab
