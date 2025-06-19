
//! @file gait_pattern_generator_switch_move.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "gait_pattern_generator_switch_move.h"

#include <magic_enum.hpp>
#include <string>
#include <utility>

#include "cassert_define.h"

namespace designlab {

GaitPatternGeneratorSwitchMove::GaitPatternGeneratorSwitchMove(
    std::unique_ptr<IGaitPatternGenerator>&& gpg_for_straight_ptr,
    std::unique_ptr<IGaitPatternGenerator>&& gpg_for_turn_spot_ptr)
    : gpg_for_straight_ptr_(std::move(gpg_for_straight_ptr)),
      gpg_for_turn_spot_ptr_(std::move(gpg_for_turn_spot_ptr)) {
  assert(gpg_for_straight_ptr_);
  assert(gpg_for_turn_spot_ptr_);
}

nostd::expected<RobotStateNode, std::string>
GaitPatternGeneratorSwitchMove::GetNextNodeByGraphSearch(
    const RobotStateNode& current_node, const MapState& map_state,
    const RobotOperation& operation) {
  using enum RobotOperationType;
  using unexpected = nostd::unexpected<std::string>;

  if (operation.operation_type == kStraightMoveVector ||
      operation.operation_type == kStraightMovePosition) {
    return gpg_for_straight_ptr_->GetNextNodeByGraphSearch(
        current_node, map_state, operation);
  } else if (operation.operation_type == kSpotTurnRotAxis ||
             operation.operation_type == kSpotTurnLastPosture) {
    return gpg_for_turn_spot_ptr_->GetNextNodeByGraphSearch(
        current_node, map_state, operation);
  } else {
    return unexpected{"A non-existent behavior was specified."};
  }
}

}  // namespace designlab
