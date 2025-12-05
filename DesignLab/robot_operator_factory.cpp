
//! @file robot_operator_factory.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "robot_operator_factory.h"

#include "cassert_define.h"
#include "robot_operation.h"
#include "robot_operator_fixed.h"
#include "robot_operator_for_gpg.h"
#include "robot_operator_for_path.h"
#include "toml_directory_exporter.h"
#include "toml_file_importer.h"

namespace gaitgen {

std::unique_ptr<IRobotOperator> RobotOperatorFactory::Create(
    const SimulationSettingRecord& setting) {
  using enum enums::RobotOperateMode;

  if (setting.operate_mode == kForGloabalPathGenerate) {
    return std::make_unique<RobotOperatorForGpg>();
  } else if (setting.operate_mode == kFixed) {
    TomlFileImporter<RobotOperation> importer;

    const auto operation =
        importer.ImportOrUseDefault(TomlDirectoryExporter::kTomlFileDirPath +
                                    setting.fixed_operate_file_name);

    return std::make_unique<RobotOperatorFixed>(operation);
  } else if (setting.operate_mode == kForPath) {
    return std::make_unique<RobotOperatorForPath>(setting.path_points);
  } else {
    assert(false);

    return nullptr;
  }
}

}  // namespace gaitgen
