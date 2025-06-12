
//! @file map_file_exporter.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "map_file_exporter.h"

#include <fstream>

#include "cmdio_util.h"

namespace designlab {

bool MapFileExporter::ExportMap(const std::string& file_path,
                                const MapState& map_state) const noexcept {
  // ファイルを開く.
  std::ofstream ofs(file_path);

  // ファイルが開けないならば false を返す.
  if (!ofs.is_open()) {
    CmdIOUtil::ErrorOutput("ファイルを開けませんでした.");

    return false;
  }

  // ファイルを1行ずつ書き込む.
  const size_t map_point_size = map_state.GetMapPointSize();

  for (size_t i = 0; i < map_point_size; ++i) {
    ofs << map_state.GetMapPoint(i) << std::endl;
  }

  // ファイルを閉じる.
  ofs.close();

  return true;
}

}  // namespace designlab
