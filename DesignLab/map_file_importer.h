
//! @file map_file_importer.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_MAP_FILE_IMPORTER_H_
#define DESIGNLAB_MAP_FILE_IMPORTER_H_

#include <string>

#include "map_state.h"
#include "my_expected.h"

namespace designlab {

//! @class MapFileImporter
//! @brief マップを csv に出力したものを読み込むクラス.
//! @details マップには脚接地可能点が羅列されているので,
//! 1行ずつ読み込み，それぞれマップに追加する.
class MapFileImporter final {
 public:
  //! @brief マップを csv に出力したものを読み込む.
  //! @param[in] file_path 読み込むファイルのパス.
  //! @return 読み込みに成功した場合は MapState を返す.
  [[nodiscard]] nostd::expected<MapState, std::string> ImportMap(
      const std::string& file_path) const noexcept;
};

}  // namespace designlab

#endif  // DESIGNLAB_MAP_FILE_IMPORTER_H_
