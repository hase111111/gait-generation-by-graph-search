
//! @file interface_simulation_end_checker.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include "robot_state_node.h"

namespace gaitgen {

//! @class ISimulationEndChecker
//! @brief シミュレーションの終了を判定するクラスのインターフェース.
class ISimulationEndChecker {
 public:
  virtual ~ISimulationEndChecker() = default;

  //! @brief シミュレーションの終了を判定する.
  //! @param[in] node ロボットの状態.
  //! @return シミュレーションを終了するなら true.
  virtual bool IsEnd(const RobotStateNode& node) const = 0;
};

}  // namespace gaitgen
