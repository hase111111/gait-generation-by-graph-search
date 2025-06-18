﻿
//! @file interface_robot_operator.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_INTERFACE_ROBOT_OPERATOR_H_
#define DESIGNLAB_INTERFACE_ROBOT_OPERATOR_H_

#include "robot_operation.h"
#include "robot_state_node.h"

namespace designlab {

//! @class IRobotOperator
//! @brief ロボットの動作を決定する処理のインターフェース.
class IRobotOperator {
 public:
  virtual ~IRobotOperator() = default;

  //! @brief ロボットの動作を初期化する.
  //! @return ロボットの動作.
  virtual RobotOperation Init() const = 0;

  //! @brief ロボットの動作を更新する.
  //! @param[in] state ロボットの現在の状態.
  //! @return ロボットの動作.
  virtual RobotOperation Update(const RobotStateNode& state) = 0;
};

}  // namespace designlab

#endif  // DESIGNLAB_INTERFACE_ROBOT_OPERATOR_H_
