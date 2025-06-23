
//! @file gait_pattern_generator_switch_by_map.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "gait_pattern_generator_switch_by_map.h"

#include <magic_enum.hpp>
#include <string>
#include <utility>

#include "cassert_define.h"
#include "cmdio_util.h"

namespace designlab {
GaitPatternGeneratorSwitchByMap::GaitPatternGeneratorSwitchByMap(
    std::unique_ptr<IGaitPatternGenerator>&& gpg_normal,
    std::unique_ptr<IGaitPatternGenerator>&& gpg_pitch_rot,
    std::unique_ptr<IGaitPatternGenerator>&& gpg_roll_rot)
    : gpg_normal_ptr_(std::move(gpg_normal)),
      gpg_pitch_rot_ptr_(std::move(gpg_pitch_rot)),
      gpg_roll_rot_ptr_(std::move(gpg_roll_rot)) {}

nostd::expected<RobotStateNode, std::string>
GaitPatternGeneratorSwitchByMap::GetNextNodeByGraphSearch(
    const RobotStateNode& current_node, const MapState& map_ref,
    const RobotOperation& operation) {
  if (DoPitchRotation(current_node, map_ref, operation)) {
    cmdio::DebugOutputF(
        "GaitPatternGeneratorSwitchByMap: "
        "Pitch rotation is required. Operation: {}",
        magic_enum::enum_name(operation.operation_type));
    return gpg_pitch_rot_ptr_->GetNextNodeByGraphSearch(current_node, map_ref,
                                                        operation);
  } else if (DoRollRotation(current_node, map_ref, operation)) {
    cmdio::DebugOutputF(
        "GaitPatternGeneratorSwitchByMap: "
        "Roll rotation is required. Operation: {}",
        magic_enum::enum_name(operation.operation_type));
    return gpg_roll_rot_ptr_->GetNextNodeByGraphSearch(current_node, map_ref,
                                                       operation);
  } else {
    return gpg_normal_ptr_->GetNextNodeByGraphSearch(current_node, map_ref,
                                                     operation);
  }
}

bool GaitPatternGeneratorSwitchByMap::DoPitchRotation(
    const RobotStateNode&, const MapState&, const RobotOperation&) const {
  //! @todo 仮実装
  return true;
}

bool GaitPatternGeneratorSwitchByMap::DoRollRotation(
    const RobotStateNode&, const MapState&, const RobotOperation&) const {
  return false;
}

}  // namespace designlab
