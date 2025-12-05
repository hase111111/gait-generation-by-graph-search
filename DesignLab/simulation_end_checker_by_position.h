
//! @file simulation_end_checker_by_position.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include "interface_simulation_end_checker.h"

namespace gaitgen {

//! @class SimulationEndCheckerByPosition
//! @brief 最終位置によるシミュレーション終了判定クラス.
class SimulationEndCheckerByPosition final : public ISimulationEndChecker {
 public:
  SimulationEndCheckerByPosition(const Vector3& goal_position,
                                 float allowable_error);

  bool IsEnd(const RobotStateNode& node) const override;

 private:
  const Vector3 goal_position_;  //!< 目標位置.
  const float allowable_error_;  //!< 許容誤差.
};

}  // namespace gaitgen
