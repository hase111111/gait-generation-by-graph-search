
//! @file robot_operator_factory.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <memory>

#include "interface_robot_operator.h"
#include "simulation_setting_record.h"

namespace gaitgen {

//! @class RobotOperatorFactory
//! @brief TargetUpdaterを生成するファクトリクラス.
class RobotOperatorFactory final {
 public:
  //! @brief 設定ファイルからTargetUpdaterを生成する.
  //! @param[in] setting 設定ファイルのデータ.
  //! @return  生成したTargetUpdater.
  static std::unique_ptr<IRobotOperator> Create(
      const SimulationSettingRecord& setting);
};

}  // namespace gaitgen
