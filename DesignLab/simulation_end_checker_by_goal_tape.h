﻿
//! @file simulation_end_checker_by_goal_tape.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_SIMULATION_END_CHECKER_BY_GOAL_TAPE_H_
#define DESIGNLAB_SIMULATION_END_CHECKER_BY_GOAL_TAPE_H_

#include "interface_simulation_end_checker.h"
#include "robot_state_node.h"

namespace designlab {

//! @class SimulationEndCheckerByGoalTape
//! @brief ゴールテープによるシミュレーション終了判定クラス.
class SimulationEndCheckerByGoalTape final : public ISimulationEndChecker {
 public:
  explicit SimulationEndCheckerByGoalTape(float goal_tape_x);

  bool IsEnd(const RobotStateNode& node) const override;

 private:
  //! 直進のときに、Y方向にこの値だけ進めたら1シミュレーション終了.
  const float kGoalTape;
};

}  // namespace designlab

#endif  // DESIGNLAB_SIMULATION_END_CHECKER_BY_GOAL_TAPE_H_
