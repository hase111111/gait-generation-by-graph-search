
//! @file simulation_end_checker_factory.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <memory>

#include "interface_simulation_end_checker.h"
#include "simulation_setting_record.h"

namespace gaitgen {

//! @class SimulationEndCheckerFactory
//! @brief シミュレーションの終了を判定するクラスを生成するクラス.
class SimulationEndCheckerFactory final {
 public:
  //! @brief シミュレーションの終了を判定するクラスを生成する.
  //! @param[in] record シミュレーションの終了を判定する際の設定.
  //! @return シミュレーションの終了を判定するクラスのインスタンス.
  static std::unique_ptr<ISimulationEndChecker> Create(
      const SimulationSettingRecord& record);
};

}  // namespace gaitgen
