
//! @file map_creator_by_csv.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "map_creator_by_csv.h"

#include <filesystem>
#include <format>
#include <fstream>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "map_file_importer.h"
#include "string_util.h"

namespace designlab {

MapCreatorByCsv::MapCreatorByCsv(const std::string& map_file_path)
    : map_file_path_(map_file_path) {
  // ファイルが存在するか確認し，なければアサート．
  const bool is_exist = std::filesystem::exists(map_file_path_);

  if (!is_exist) {
    std::string error_mes = std::format(
        "map fileが存在しません．ファイルのパスは{}です．({}のコンストラクタ)",
        map_file_path_, string_util::GetTypeName(*this));

    CmdIOUtil::Output(error_mes, OutputDetail::kError);

    assert(false);
  }
}

MapState MapCreatorByCsv::InitMap() {
  const MapFileImporter map_file_importer{};

  return map_file_importer.ImportMap(map_file_path_).value_or(MapState());
}

void MapCreatorByCsv::UpdateMap(MapState*) {
  // 何もしない．
}

}  // namespace designlab
