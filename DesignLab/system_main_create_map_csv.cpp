
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

namespace gaitgen::internal {

// simulation用の地形を大量に作成するための仮の関数.
void TempMakeMaps() {
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

  struct Temp {
    Temp(const std::string& name, const SimulationMapMode mode,
         const unsigned int option, const int interval, const int hole_rate,
         const float diagonally_striped_angle)
        : name(name),
          mode(mode),
          option(option),
          interval(interval),
          hole_rate(hole_rate),
          diagonally_striped_angle(diagonally_striped_angle) {}

    std::string name;
    SimulationMapMode mode;
    unsigned int option;
    int interval;
    int hole_rate;
    float diagonally_striped_angle;
  };
  using enum SimulationMapMode;

  std::vector<Temp> temp_vec{
      {"normal_flat", kFlat, 0, 0, 0, 0.f},
      {"normal_vertical_stripe_40x", kVerticalStripe, 0, 2, 0, 0.f},
      {"normal_vertical_stripe_60x", kVerticalStripe, 0, 3, 0, 0.f},
      {"normal_vertical_stripe_80x", kVerticalStripe, 0, 4, 0, 0.f},
      {"normal_vertical_stripe_100x", kVerticalStripe, 0, 5, 0, 0.f},
      {"normal_horizontal_stripe_40x", kHorizontalStripe, 0, 2, 0, 0.f},
      {"normal_horizontal_stripe_60x", kHorizontalStripe, 0, 3, 0, 0.f},
      {"normal_horizontal_stripe_80x", kHorizontalStripe, 0, 4, 0, 0.f},
      {"normal_horizontal_stripe_100x", kHorizontalStripe, 0, 5, 0, 0.f},
      {"normal_diagonal_stripe_40x", kDiagonalStripe, 0, 2, 0, 0.f},
      {"normal_diagonal_stripe_60x", kDiagonalStripe, 0, 3, 0, 0.f},
      {"normal_diagonal_stripe_80x", kDiagonalStripe, 0, 4, 0, 0.f},
      {"normal_diagonal_stripe_100x", kDiagonalStripe, 0, 5, 0, 0.f},
      {"normal_slanted_stripe_30deg_40x", kSlantedStripes, 0, 2, 0, 30.f},
      {"normal_slanted_stripe_30deg_60x", kSlantedStripes, 0, 3, 0, 30.f},
      {"normal_slanted_stripe_30deg_80x", kSlantedStripes, 0, 4, 0, 30.f},
      {"normal_slanted_stripe_30deg_100x", kSlantedStripes, 0, 5, 0, 30.f},
      {"normal_slanted_stripe_45deg_40x", kSlantedStripes, 0, 2, 0, 45.f},
      {"normal_slanted_stripe_45deg_60x", kSlantedStripes, 0, 3, 0, 45.f},
      {"normal_slanted_stripe_45deg_80x", kSlantedStripes, 0, 4, 0, 45.f},
      {"normal_slanted_stripe_45deg_100x", kSlantedStripes, 0, 5, 0, 45.f},
      {"normal_slanted_stripe_60deg_40x", kSlantedStripes, 0, 2, 0, 60.f},
      {"normal_slanted_stripe_60deg_60x", kSlantedStripes, 0, 3, 0, 60.f},
      {"normal_slanted_stripe_60deg_80x", kSlantedStripes, 0, 4, 0, 60.f},
      {"normal_slanted_stripe_60deg_100x", kSlantedStripes, 0, 5, 0, 60.f},
      {"normal_slanted_stripe_-30deg_40x", kSlantedStripes, 0, 2, 0, -30.f},
      {"normal_slanted_stripe_-30deg_60x", kSlantedStripes, 0, 3, 0, -30.f},
      {"normal_slanted_stripe_-30deg_80x", kSlantedStripes, 0, 4, 0, -30.f},
      {"normal_slanted_stripe_-30deg_100x", kSlantedStripes, 0, 5, 0, -30.f},
      {"normal_slanted_stripe_-45deg_40x", kSlantedStripes, 0, 2, 0, -45.f},
      {"normal_slanted_stripe_-45deg_60x", kSlantedStripes, 0, 3, 0, -45.f},
      {"normal_slanted_stripe_-45deg_80x", kSlantedStripes, 0, 4, 0, -45.f},
      {"normal_slanted_stripe_-45deg_100x", kSlantedStripes, 0, 5, 0, -45.f},
      {"normal_slanted_stripe_-60deg_40x", kSlantedStripes, 0, 2, 0, -60.f},
      {"normal_slanted_stripe_-60deg_60x", kSlantedStripes, 0, 3, 0, -60.f},
      {"normal_slanted_stripe_-60deg_80x", kSlantedStripes, 0, 4, 0, -60.f},
      {"normal_slanted_stripe_-60deg_100x", kSlantedStripes, 0, 5, 0, -60.f},
      {"normal_mesh_40x", kMesh, 0, 2, 0, 0.f},
      {"normal_mesh_60x", kMesh, 0, 3, 0, 0.f},
      {"normal_mesh_80x", kMesh, 0, 4, 0, 0.f},
      {"normal_mesh_100x", kMesh, 0, 5, 0, 0.f},
      {"normal_lattice_point_40x", kLatticePoint, 0, 2, 0, 0.f},
      {"normal_lattice_point_60x", kLatticePoint, 0, 3, 0, 0.f},
      {"normal_lattice_point_80x", kLatticePoint, 0, 4, 0, 0.f},
      {"normal_lattice_point_100x", kLatticePoint, 0, 5, 0, 0.f},
      {"normal_random_10%_40x", kFlat, 1, 2, 10, 0.f},
      {"normal_random_10%_60x", kFlat, 1, 3, 10, 0.f},
      {"normal_random_10%_80x", kFlat, 1, 4, 10, 0.f},
      {"normal_random_10%_100x", kFlat, 1, 5, 10, 0.f},
      {"normal_random_20%_40x", kFlat, 1, 2, 20, 0.f},
      {"normal_random_20%_60x", kFlat, 1, 3, 20, 0.f},
      {"normal_random_20%_80x", kFlat, 1, 4, 20, 0.f},
      {"normal_random_20%_100x", kFlat, 1, 5, 20, 0.f},
      {"normal_random_30%_40x", kFlat, 1, 2, 30, 0.f},
      {"normal_random_30%_60x", kFlat, 1, 3, 30, 0.f},
      {"normal_random_30%_80x", kFlat, 1, 4, 30, 0.f},
      {"normal_random_30%_100x", kFlat, 1, 5, 30, 0.f},
      {"normal_random_40%_40x", kFlat, 1, 2, 40, 0.f},
      {"normal_random_40%_60x", kFlat, 1, 3, 40, 0.f},
      {"normal_random_40%_80x", kFlat, 1, 4, 40, 0.f},
      {"normal_random_40%_100x", kFlat, 1, 5, 40, 0.f},
      {"valley_flat", kFlat, 128, 5, 0, 0.f},
      {"valley_vertical_stripe_40x", kVerticalStripe, 128, 2, 0, 0.f},
      {"valley_vertical_stripe_60x", kVerticalStripe, 128, 3, 0, 0.f},
      {"valley_vertical_stripe_80x", kVerticalStripe, 128, 4, 0, 0.f},
      {"valley_vertical_stripe_100x", kVerticalStripe, 128, 5, 0, 0.f},
      {"valley_horizontal_stripe_40x", kHorizontalStripe, 128, 2, 0, 0.f},
      {"valley_horizontal_stripe_60x", kHorizontalStripe, 128, 3, 0, 0.f},
      {"valley_horizontal_stripe_80x", kHorizontalStripe, 128, 4, 0, 0.f},
      {"valley_horizontal_stripe_100x", kHorizontalStripe, 128, 5, 0, 0.f},
      {"valley_diagonal_stripe_40x", kDiagonalStripe, 128, 2, 0, 0.f},
      {"valley_diagonal_stripe_60x", kDiagonalStripe, 128, 3, 0, 0.f},
      {"valley_diagonal_stripe_80x", kDiagonalStripe, 128, 4, 0, 0.f},
      {"valley_diagonal_stripe_100x", kDiagonalStripe, 128, 5, 0, 0.f},
      {"valley_slanted_stripe_30deg_40x", kSlantedStripes, 128, 2, 0, 30.f},
      {"valley_slanted_stripe_30deg_60x", kSlantedStripes, 128, 3, 0, 30.f},
      {"valley_slanted_stripe_30deg_80x", kSlantedStripes, 128, 4, 0, 30.f},
      {"valley_slanted_stripe_30deg_100x", kSlantedStripes, 128, 5, 0, 30.f},
      {"valley_slanted_stripe_45deg_40x", kSlantedStripes, 128, 2, 0, 45.f},
      {"valley_slanted_stripe_45deg_60x", kSlantedStripes, 128, 3, 0, 45.f},
      {"valley_slanted_stripe_45deg_80x", kSlantedStripes, 128, 4, 0, 45.f},
      {"valley_slanted_stripe_45deg_100x", kSlantedStripes, 128, 5, 0, 45.f},
      {"valley_slanted_stripe_60deg_40x", kSlantedStripes, 128, 2, 0, 60.f},
      {"valley_slanted_stripe_60deg_60x", kSlantedStripes, 128, 3, 0, 60.f},
      {"valley_slanted_stripe_60deg_80x", kSlantedStripes, 128, 4, 0, 60.f},
      {"valley_slanted_stripe_60deg_100x", kSlantedStripes, 128, 5, 0, 60.f},
      {"valley_slanted_stripe_-30deg_40x", kSlantedStripes, 128, 2, 0, -30.f},
      {"valley_slanted_stripe_-30deg_60x", kSlantedStripes, 128, 3, 0, -30.f},
      {"valley_slanted_stripe_-30deg_80x", kSlantedStripes, 128, 4, 0, -30.f},
      {"valley_slanted_stripe_-30deg_100x", kSlantedStripes, 128, 5, 0, -30.f},
      {"valley_slanted_stripe_-45deg_40x", kSlantedStripes, 128, 2, 0, -45.f},
      {"valley_slanted_stripe_-45deg_60x", kSlantedStripes, 128, 3, 0, -45.f},
      {"valley_slanted_stripe_-45deg_80x", kSlantedStripes, 128, 4, 0, -45.f},
      {"valley_slanted_stripe_-45deg_100x", kSlantedStripes, 128, 5, 0, -45.f},
      {"valley_slanted_stripe_-60deg_40x", kSlantedStripes, 128, 2, 0, -60.f},
      {"valley_slanted_stripe_-60deg_60x", kSlantedStripes, 128, 3, 0, -60.f},
      {"valley_slanted_stripe_-60deg_80x", kSlantedStripes, 128, 4, 0, -60.f},
      {"valley_slanted_stripe_-60deg_100x", kSlantedStripes, 128, 5, 0, -60.f},
      {"valley_mesh_40x", kMesh, 128, 2, 0, 0.f},
      {"valley_mesh_60x", kMesh, 128, 3, 0, 0.f},
      {"valley_mesh_80x", kMesh, 128, 4, 0, 0.f},
      {"valley_mesh_100x", kMesh, 128, 5, 0, 0.f},
      {"valley_lattice_point_40x", kLatticePoint, 128, 2, 0, 0.f},
      {"valley_lattice_point_60x", kLatticePoint, 128, 3, 0, 0.f},
      {"valley_lattice_point_80x", kLatticePoint, 128, 4, 0, 0.f},
      {"valley_lattice_point_100x", kLatticePoint, 128, 5, 0, 0.f},
      {"valley_random_10%_40x", kFlat, 129, 2, 10, 0.f},
      {"valley_random_10%_60x", kFlat, 129, 3, 10, 0.f},
      {"valley_random_10%_80x", kFlat, 129, 4, 10, 0.f},
      {"valley_random_10%_100x", kFlat, 129, 5, 10, 0.f},
      {"valley_random_20%_40x", kFlat, 129, 2, 20, 0.f},
      {"valley_random_20%_60x", kFlat, 129, 3, 20, 0.f},
      {"valley_random_20%_80x", kFlat, 129, 4, 20, 0.f},
      {"valley_random_20%_100x", kFlat, 129, 5, 20, 0.f},
      {"valley_random_30%_40x", kFlat, 129, 2, 30, 0.f},
      {"valley_random_30%_60x", kFlat, 129, 3, 30, 0.f},
      {"valley_random_30%_80x", kFlat, 129, 4, 30, 0.f},
      {"valley_random_30%_100x", kFlat, 129, 5, 30, 0.f},
      {"valley_random_40%_40x", kFlat, 129, 2, 40, 0.f},
      {"valley_random_40%_60x", kFlat, 129, 3, 40, 0.f},
      {"valley_random_40%_80x", kFlat, 129, 4, 40, 0.f},
      {"valley_random_40%_100x", kFlat, 129, 5, 40, 0.f},
      {"step_flat", kFlat, 64, 0, 0, 0.f},
      {"step_vertical_stripe_40x", kVerticalStripe, 64, 2, 0, 0.f},
      {"step_vertical_stripe_60x", kVerticalStripe, 64, 3, 0, 0.f},
      {"step_vertical_stripe_80x", kVerticalStripe, 64, 4, 0, 0.f},
      {"step_vertical_stripe_100x", kVerticalStripe, 64, 5, 0, 0.f},
      {"step_horizontal_stripe_40x", kHorizontalStripe, 64, 2, 0, 0.f},
      {"step_horizontal_stripe_60x", kHorizontalStripe, 64, 3, 0, 0.f},
      {"step_horizontal_stripe_80x", kHorizontalStripe, 64, 4, 0, 0.f},
      {"step_horizontal_stripe_100x", kHorizontalStripe, 64, 5, 0, 0.f},
      {"step_diagonal_stripe_40x", kDiagonalStripe, 64, 2, 0, 0.f},
      {"step_diagonal_stripe_60x", kDiagonalStripe, 64, 3, 0, 0.f},
      {"step_diagonal_stripe_80x", kDiagonalStripe, 64, 4, 0, 0.f},
      {"step_diagonal_stripe_100x", kDiagonalStripe, 64, 5, 0, 0.f},
      {"step_slanted_stripe_30deg_40x", kSlantedStripes, 64, 2, 0, 30.f},
      {"step_slanted_stripe_30deg_60x", kSlantedStripes, 64, 3, 0, 30.f},
      {"step_slanted_stripe_30deg_80x", kSlantedStripes, 64, 4, 0, 30.f},
      {"step_slanted_stripe_30deg_100x", kSlantedStripes, 64, 5, 0, 30.f},
      {"step_slanted_stripe_45deg_40x", kSlantedStripes, 64, 2, 0, 45.f},
      {"step_slanted_stripe_45deg_60x", kSlantedStripes, 64, 3, 0, 45.f},
      {"step_slanted_stripe_45deg_80x", kSlantedStripes, 64, 4, 0, 45.f},
      {"step_slanted_stripe_45deg_100x", kSlantedStripes, 64, 5, 0, 45.f},
      {"step_slanted_stripe_60deg_40x", kSlantedStripes, 64, 2, 0, 60.f},
      {"step_slanted_stripe_60deg_60x", kSlantedStripes, 64, 3, 0, 60.f},
      {"step_slanted_stripe_60deg_80x", kSlantedStripes, 64, 4, 0, 60.f},
      {"step_slanted_stripe_60deg_100x", kSlantedStripes, 64, 5, 0, 60.f},
      {"step_slanted_stripe_-30deg_40x", kSlantedStripes, 64, 2, 0, -30.f},
      {"step_slanted_stripe_-30deg_60x", kSlantedStripes, 64, 3, 0, -30.f},
      {"step_slanted_stripe_-30deg_80x", kSlantedStripes, 64, 4, 0, -30.f},
      {"step_slanted_stripe_-30deg_100x", kSlantedStripes, 64, 5, 0, -30.f},
      {"step_slanted_stripe_-45deg_40x", kSlantedStripes, 64, 2, 0, -45.f},
      {"step_slanted_stripe_-45deg_60x", kSlantedStripes, 64, 3, 0, -45.f},
      {"step_slanted_stripe_-45deg_80x", kSlantedStripes, 64, 4, 0, -45.f},
      {"step_slanted_stripe_-45deg_100x", kSlantedStripes, 64, 5, 0, -45.f},
      {"step_slanted_stripe_-60deg_40x", kSlantedStripes, 64, 2, 0, -60.f},
      {"step_slanted_stripe_-60deg_60x", kSlantedStripes, 64, 3, 0, -60.f},
      {"step_slanted_stripe_-60deg_80x", kSlantedStripes, 64, 4, 0, -60.f},
      {"step_slanted_stripe_-60deg_100x", kSlantedStripes, 64, 5, 0, -60.f},
      {"step_mesh_40x", kMesh, 64, 2, 0, 0.f},
      {"step_mesh_60x", kMesh, 64, 3, 0, 0.f},
      {"step_mesh_80x", kMesh, 64, 4, 0, 0.f},
      {"step_mesh_100x", kMesh, 64, 5, 0, 0.f},
      {"step_lattice_point_40x", kLatticePoint, 64, 2, 0, 0.f},
      {"step_lattice_point_60x", kLatticePoint, 64, 3, 0, 0.f},
      {"step_lattice_point_80x", kLatticePoint, 64, 4, 0, 0.f},
      {"step_lattice_point_100x", kLatticePoint, 64, 5, 0, 0.f},
      {"step_random_10%_40x", kFlat, 65, 2, 10, 0.f},
      {"step_random_10%_60x", kFlat, 65, 3, 10, 0.f},
      {"step_random_10%_80x", kFlat, 65, 4, 10, 0.f},
      {"step_random_10%_100x", kFlat, 65, 5, 10, 0.f},
      {"step_random_20%_40x", kFlat, 65, 2, 20, 0.f},
      {"step_random_20%_60x", kFlat, 65, 3, 20, 0.f},
      {"step_random_20%_80x", kFlat, 65, 4, 20, 0.f},
      {"step_random_20%_100x", kFlat, 65, 5, 20, 0.f},
      {"step_random_30%_40x", kFlat, 65, 2, 30, 0.f},
      {"step_random_30%_60x", kFlat, 65, 3, 30, 0.f},
      {"step_random_30%_80x", kFlat, 65, 4, 30, 0.f},
      {"step_random_30%_100x", kFlat, 65, 5, 30, 0.f},
      {"step_random_40%_40x", kFlat, 65, 2, 40, 0.f},
      {"step_random_40%_60x", kFlat, 65, 3, 40, 0.f},
      {"step_random_40%_80x", kFlat, 65, 4, 40, 0.f},
      {"step_random_40%_100x", kFlat, 65, 5, 40, 0.f},
      {"rough_flat_40x", kFlat, 16, 2, 0, 0.f},
      {"rough_flat_60x", kFlat, 16, 3, 0, 0.f},
      {"rough_flat_80x", kFlat, 16, 4, 0, 0.f},
      {"rough_flat_100x", kFlat, 16, 5, 0, 0.f},
      {"rough_vertical_stripe_40x", kVerticalStripe, 16, 2, 0, 0.f},
      {"rough_vertical_stripe_60x", kVerticalStripe, 16, 3, 0, 0.f},
      {"rough_vertical_stripe_80x", kVerticalStripe, 16, 4, 0, 0.f},
      {"rough_vertical_stripe_100x", kVerticalStripe, 16, 5, 0, 0.f},
      {"rough_horizontal_stripe_40x", kHorizontalStripe, 16, 2, 0, 0.f},
      {"rough_horizontal_stripe_60x", kHorizontalStripe, 16, 3, 0, 0.f},
      {"rough_horizontal_stripe_80x", kHorizontalStripe, 16, 4, 0, 0.f},
      {"rough_horizontal_stripe_100x", kHorizontalStripe, 16, 5, 0, 0.f},
      {"rough_diagonal_stripe_40x", kDiagonalStripe, 16, 2, 0, 0.f},
      {"rough_diagonal_stripe_60x", kDiagonalStripe, 16, 3, 0, 0.f},
      {"rough_diagonal_stripe_80x", kDiagonalStripe, 16, 4, 0, 0.f},
      {"rough_diagonal_stripe_100x", kDiagonalStripe, 16, 5, 0, 0.f},
      {"rough_slanted_stripe_30deg_40x", kSlantedStripes, 16, 2, 0, 30.f},
      {"rough_slanted_stripe_30deg_60x", kSlantedStripes, 16, 3, 0, 30.f},
      {"rough_slanted_stripe_30deg_80x", kSlantedStripes, 16, 4, 0, 30.f},
      {"rough_slanted_stripe_30deg_100x", kSlantedStripes, 16, 5, 0, 30.f},
      {"rough_slanted_stripe_45deg_40x", kSlantedStripes, 16, 2, 0, 45.f},
      {"rough_slanted_stripe_45deg_60x", kSlantedStripes, 16, 3, 0, 45.f},
      {"rough_slanted_stripe_45deg_80x", kSlantedStripes, 16, 4, 0, 45.f},
      {"rough_slanted_stripe_45deg_100x", kSlantedStripes, 16, 5, 0, 45.f},
      {"rough_slanted_stripe_60deg_40x", kSlantedStripes, 16, 2, 0, 60.f},
      {"rough_slanted_stripe_60deg_60x", kSlantedStripes, 16, 3, 0, 60.f},
      {"rough_slanted_stripe_60deg_80x", kSlantedStripes, 16, 4, 0, 60.f},
      {"rough_slanted_stripe_60deg_100x", kSlantedStripes, 16, 5, 0, 60.f},
      {"rough_slanted_stripe_-30deg_40x", kSlantedStripes, 16, 2, 0, -30.f},
      {"rough_slanted_stripe_-30deg_60x", kSlantedStripes, 16, 3, 0, -30.f},
      {"rough_slanted_stripe_-30deg_80x", kSlantedStripes, 16, 4, 0, -30.f},
      {"rough_slanted_stripe_-30deg_100x", kSlantedStripes, 16, 5, 0, -30.f},
      {"rough_slanted_stripe_-45deg_40x", kSlantedStripes, 16, 2, 0, -45.f},
      {"rough_slanted_stripe_-45deg_60x", kSlantedStripes, 16, 3, 0, -45.f},
      {"rough_slanted_stripe_-45deg_80x", kSlantedStripes, 16, 4, 0, -45.f},
      {"rough_slanted_stripe_-45deg_100x", kSlantedStripes, 16, 5, 0, -45.f},
      {"rough_slanted_stripe_-60deg_40x", kSlantedStripes, 16, 2, 0, -60.f},
      {"rough_slanted_stripe_-60deg_60x", kSlantedStripes, 16, 3, 0, -60.f},
      {"rough_slanted_stripe_-60deg_80x", kSlantedStripes, 16, 4, 0, -60.f},
      {"rough_slanted_stripe_-60deg_100x", kSlantedStripes, 16, 5, 0, -60.f},
      {"rough_mesh_40x", kMesh, 16, 2, 0, 0.f},
      {"rough_mesh_60x", kMesh, 16, 3, 0, 0.f},
      {"rough_mesh_80x", kMesh, 16, 4, 0, 0.f},
      {"rough_mesh_100x", kMesh, 16, 5, 0, 0.f},
      {"rough_lattice_point_40x", kLatticePoint, 16, 2, 0, 0.f},
      {"rough_lattice_point_60x", kLatticePoint, 16, 3, 0, 0.f},
      {"rough_lattice_point_80x", kLatticePoint, 16, 4, 0, 0.f},
      {"rough_lattice_point_100x", kLatticePoint, 16, 5, 0, 0.f},
      {"rough_random_10%_40x", kFlat, 17, 2, 10, 0.f},
      {"rough_random_10%_60x", kFlat, 17, 3, 10, 0.f},
      {"rough_random_10%_80x", kFlat, 17, 4, 10, 0.f},
      {"rough_random_10%_100x", kFlat, 17, 5, 10, 0.f},
      {"rough_random_20%_40x", kFlat, 17, 2, 20, 0.f},
      {"rough_random_20%_60x", kFlat, 17, 3, 20, 0.f},
      {"rough_random_20%_80x", kFlat, 17, 4, 20, 0.f},
      {"rough_random_20%_100x", kFlat, 17, 5, 20, 0.f},
      {"rough_random_30%_40x", kFlat, 17, 2, 30, 0.f},
      {"rough_random_30%_60x", kFlat, 17, 3, 30, 0.f},
      {"rough_random_30%_80x", kFlat, 17, 4, 30, 0.f},
      {"rough_random_30%_100x", kFlat, 17, 5, 30, 0.f},
      {"rough_random_40%_40x", kFlat, 17, 2, 40, 0.f},
      {"rough_random_40%_60x", kFlat, 17, 3, 40, 0.f},
      {"rough_random_40%_80x", kFlat, 17, 4, 40, 0.f},
      {"rough_random_40%_100x", kFlat, 17, 5, 40, 0.f},
  };

  for (const auto& temp : temp_vec) {
    SimulationMapParameter param = simulation_map_parameter;
    param.mode = temp.mode;
    param.option = temp.option;
    param.stripe_interval = temp.interval;
    param.hole_rate = temp.hole_rate;
    param.diagonal_angle = temp.diagonally_striped_angle;

    const auto map_creator = MapCreatorForSimulation(param);
    const auto map_state = map_creator.InitMap();
    const auto file_path =
        TomlDirectoryExporter::kContinuousSimulationMapDirPath + temp.name +
        ".csv";
    std::ofstream output_file(file_path);
    if (!output_file.is_open()) {
      cmdio::ErrorOutput("Failed to open file for writing: " + file_path);
      continue;
    }
    for (size_t i = 0; i < map_state.GetMapPointSize(); ++i) {
      const auto point = map_state.GetMapPoint(i);
      output_file << point.x << "," << point.y << "," << point.z << "\n";
    }
    output_file.close();
    cmdio::SystemOutput("Map CSV file has been created at: " + file_path);
  }
}

}  // namespace gaitgen::internal

namespace gaitgen {

SystemMainCreateMapCsv::SystemMainCreateMapCsv(
    const std::shared_ptr<GraphicDataBroker>& broker_ptr)
    : broker_ptr_(broker_ptr) {}

void SystemMainCreateMapCsv::Main() {
  using enum OutputDetail;

  // internal::TempMakeMaps();

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

}  // namespace gaitgen
