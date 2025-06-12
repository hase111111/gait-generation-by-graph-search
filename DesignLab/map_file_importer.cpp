
//! @file map_file_importer.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "map_file_importer.h"

#include <fstream>
#include <sstream>
#include <vector>

#include "cmdio_util.h"

namespace designlab {

using nostd::expected;
using nostd::unexpected;

expected<MapState, std::string> MapFileImporter::ImportMap(
    const std::string& file_path) const noexcept {
  // ファイルを開く.
  std::ifstream ifs(file_path);

  // ファイルが開けないならば, エラーを返す.
  if (!ifs.is_open()) {
    return unexpected("Cannot open file. file name: " + file_path);
  }

  // ファイルを1行ずつ読み込み,Mapに追加する.
  std::vector<Vector3> map_point;

  std::string line;

  while (std::getline(ifs, line)) {
    std::istringstream iss(line);

    try {
      Vector3 point;
      iss >> point;

      map_point.push_back(point);
    } catch (...) {
      cmdio::Output("読み込むことができないデータがあったため無視します.",
                        OutputDetail::kWarning);
    }
  }

  return MapState(map_point);
}

}  // namespace designlab
