
//! @file system_main_create_map_csv.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_SYSTEM_MAIN_CREATE_MAP_CSV_H_
#define DESIGNLAB_SYSTEM_MAIN_CREATE_MAP_CSV_H_

#include <memory>
#include <string>

#include "application_setting_record.h"
#include "interface_gait_pattern_generator.h"
#include "interface_robot_operator.h"
#include "interface_simulation_end_checker.h"
#include "interface_system_main.h"
#include "map_state.h"
#include "node_initializer.h"
#include "result_file_exporter.h"
#include "robot_operation.h"
#include "stopwatch.h"

namespace designlab {

//! @class SystemMainCreateMapCsv
class SystemMainCreateMapCsv final : public ISystemMain {
 public:
  SystemMainCreateMapCsv() = default;

  void Main() override;

 private:
};

}  // namespace designlab

#endif  // DESIGNLAB_SYSTEM_MAIN_CREATE_MAP_CSV_H_
