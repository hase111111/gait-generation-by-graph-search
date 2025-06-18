
//! @file robot_operator_fixed.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_ROBOT_OPERATOR_FIXED_H_
#define DESIGNLAB_ROBOT_OPERATOR_FIXED_H_

#include "interface_robot_operator.h"

namespace designlab {

//! @class RobotOperatorFixed
//! @brief 命令の更新をせず,初期化時の命令を送り続けるクラス.
class RobotOperatorFixed : public IRobotOperator {
 public:
  RobotOperatorFixed() = delete;

  //! @brief コンストラクタで指定した命令を常に返す.
  //! @param[in] operation ロボットへ送る命令.
  explicit RobotOperatorFixed(const RobotOperation& operation);

  RobotOperation Init() const override;

  RobotOperation Update(const RobotStateNode& state) override;

 private:
  const RobotOperation operation_;
};

}  // namespace designlab

#endif  // DESIGNLAB_ROBOT_OPERATOR_FIXED_H_
