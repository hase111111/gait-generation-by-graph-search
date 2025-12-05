
//! @file boot_mode.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

namespace gaitgen {

//! @enum BootMode
//! @brief 起動モードを表す列挙型.
//! @see BootModeSelector
enum class BootMode : int {
  kSimulation = 0,        //!< シミュレーションモード.
  kViewer,                //!< ビューワーモード.
  kDisplayModel,          //!< ディスプレイモード.
  kResultViewer,          //!< リザルトビューワーモード.
  kRobotControl,          //!< ロボットコントロールモード.
  kContinuousSimulation,  //!< 連続シミュレーションモード.
  kCreateMapCsv,          //!< マップのCSV作成モード.
};

}  // namespace gaitgen
