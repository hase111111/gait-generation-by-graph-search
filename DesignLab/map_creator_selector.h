
//! @file map_creator_selector.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <memory>

#include "interface_map_creator.h"
#include "simulation_setting_record.h"

namespace gaitgen {

//! @class MapCreatorSelector
//! @brief マップを生成するクラスを生成するクラス.
class MapCreatorSelector final {
 public:
  //! @brief マップを生成するクラスを生成する.
  //! @param[in] record マップを生成する際の設定.
  //! @return マップを生成するクラスのインスタンス.
  std::unique_ptr<IMapCreator> Select(
      const SimulationSettingRecord& record) const;
};

}  // namespace gaitgen
