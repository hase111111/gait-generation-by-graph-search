
//! @file toml_directory_exporter.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "toml_directory_exporter.h"

#include <filesystem>

namespace gaitgen {

const char TomlDirectoryExporter::kTomlFileDirPath[] =
    "./simulation_condition/";

const char TomlDirectoryExporter::kContinuousSimulationMapDirPath[] =
    "./simulation_condition/continuous_simulation_map/";

void TomlDirectoryExporter::Export() {
  // ディレクトリが存在しない場合は作成.
  if (!std::filesystem::exists(kTomlFileDirPath)) {
    std::filesystem::create_directory(kTomlFileDirPath);
  }

  if (!std::filesystem::exists(kContinuousSimulationMapDirPath)) {
    std::filesystem::create_directory(kContinuousSimulationMapDirPath);
  }
}

}  // namespace gaitgen
