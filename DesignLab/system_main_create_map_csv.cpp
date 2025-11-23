
//! @file system_main_create_map_csv.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "system_main_create_map_csv.h"

#include "cmdio_util.h"
#include "simulation_map_parameter_validator.h"
#include "simulation_setting_record.h"
#include "toml_directory_exporter.h"
#include "toml_file_importer.h"

namespace designlab {

SystemMainCreateMapCsv::SystemMainCreateMapCsv(
    const std::shared_ptr<GraphicDataBroker>& broker_ptr)
    : broker_ptr_(broker_ptr) {}

void SystemMainCreateMapCsv::Main() {
  using enum OutputDetail;
  // コマンドラインにタイトルを表示する.
  cmdio::OutputTitle("Create Map CSV Mode");

  // マップを受け取る.
  const auto map_state = MakeMapCreator()->InitMap();

  // broker_ptr_を通してマップ情報を渡す.
  broker_ptr_->map_state.SetData(map_state);

  // CSVファイルに出力する.
  const auto csv_name = cmdio::InputDirName(
      "Enter the name of the CSV file to output the map. (Japanese is not "
      "recommended).");

  const auto file_path =
      TomlDirectoryExporter::kContinuousSimulationMapDirPath + csv_name +
      ".csv";

  // std を使って，ファイルを書き込む．
  std::ofstream output_file(file_path);
  if (!output_file.is_open()) {
    cmdio::ErrorOutput("Failed to open file for writing: " + file_path);
    return;
  }

  for (size_t i = 0; i < map_state.GetMapPointSize(); ++i) {
    const auto point = map_state.GetMapPoint(i);
    output_file << point.x << "," << point.y << "," << point.z << "\n";
  }
  output_file.close();

  cmdio::SystemOutput("Map CSV file has been created at: " + file_path);
}

std::unique_ptr<MapCreatorForSimulation>
SystemMainCreateMapCsv::MakeMapCreator() const {
  const auto sim_setting_record =
      TomlFileImporter<SimulationSettingRecord>{}.ImportOrUseDefault(
          "./simulation_condition/simulation_setting.toml");

  const TomlFileImporter<SimulationMapParameter>
      simulation_map_parameter_importer(
          std::make_unique<SimulationMapParameterValidator>());

  const std::string simulation_map_param_file_path =
      TomlDirectoryExporter::kTomlFileDirPath +
      sim_setting_record.simulation_map_param_file_name;

  const auto simulation_map_parameter =
      simulation_map_parameter_importer.ImportOrUseDefault(
          simulation_map_param_file_path);

  return std::make_unique<MapCreatorForSimulation>(simulation_map_parameter);
}

}  // namespace designlab
