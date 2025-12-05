
//! @file map_creator_by_csv.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <string>

#include "interface_map_creator.h"

namespace gaitgen {

//! @class MapCreatorByCsv
//! @brief CSV ファイルからマップを生成するクラス.
class MapCreatorByCsv final : public IMapCreator {
 public:
  //! @brief コンストラクタで作成するマップ情報を与える
  //! @param[in] map_file_path マップのCSVファイルのパス.
  explicit MapCreatorByCsv(const std::string& map_file_path);

  MapState InitMap() const override;

  void UpdateMap(MapState* current_map) const override;

 private:
  std::string map_file_path_;  //!< マップの CSV ファイルのパス.
};

}  // namespace gaitgen
