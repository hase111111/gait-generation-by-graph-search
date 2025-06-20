
//! @file gait_pattern_generator_switch_by_map.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_GAIT_PATTERN_GENERATOR_SWITCH_BY_MAP_H_
#define DESIGNLAB_GAIT_PATTERN_GENERATOR_SWITCH_BY_MAP_H_

#include <memory>
#include <vector>

#include "graph_tree_creator.h"
#include "interface_gait_pattern_generator.h"
#include "robot_state_node.h"

namespace designlab {

//! @class GaitPatternGeneratorSwitchByMap
//! @brief 複数の種類の歩容パターンの生成方法を切り替えるためのクラス.
class GaitPatternGeneratorSwitchByMap final : public IGaitPatternGenerator {
 public:
  //! @param[in] gait_pattern_generator_for_straight 直進時に使用するもの.
  //! @param[in] gait_pattern_generator_for_turn_spot
  //! その場旋回時に使用するもの.
  GaitPatternGeneratorSwitchByMap(
      std::unique_ptr<IGaitPatternGenerator>&& gpg_normal,
      std::unique_ptr<IGaitPatternGenerator>&& gpg_pitch_rot,
      std::unique_ptr<IGaitPatternGenerator>&& gpg_roll_rot);

  ~GaitPatternGeneratorSwitchByMap() = default;

  nostd::expected<RobotStateNode, std::string> GetNextNodeByGraphSearch(
      const RobotStateNode& current_node, const MapState& map_ref,
      const RobotOperation& operation) override;

 private:
  const std::unique_ptr<IGaitPatternGenerator> gpg_normal_ptr_;
  const std::unique_ptr<IGaitPatternGenerator> gpg_pitch_rot_ptr_;
  const std::unique_ptr<IGaitPatternGenerator> gpg_roll_rot_ptr_;

  bool DoPitchRotation(const RobotStateNode& current_node,
                       const MapState& map_ref,
                       const RobotOperation& operation) const;

  bool DoRollRotation(const RobotStateNode& current_node,
                      const MapState& map_ref,
                      const RobotOperation& operation) const;
};

}  // namespace designlab

#endif  // DESIGNLAB_GAIT_PATTERN_GENERATOR_SWITCH_BY_MAP_H_
