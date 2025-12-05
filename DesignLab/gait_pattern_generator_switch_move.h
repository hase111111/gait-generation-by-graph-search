
//! @file gait_pattern_generator_switch_move.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <memory>
#include <vector>

#include "graph_tree_creator.h"
#include "interface_gait_pattern_generator.h"
#include "robot_state_node.h"

namespace gaitgen {

//! @class GaitPatternGeneratorSwitchMove
//! @brief 複数の種類の歩容パターンの生成方法を切り替えるためのクラス.
class GaitPatternGeneratorSwitchMove final : public IGaitPatternGenerator {
 public:
  //! @param[in] gait_pattern_generator_for_straight 直進時に使用するもの.
  //! @param[in] gait_pattern_generator_for_turn_spot
  //! その場旋回時に使用するもの.
  GaitPatternGeneratorSwitchMove(std::unique_ptr<IGaitPatternGenerator>&&
                                     gait_pattern_generator_for_straight,
                                 std::unique_ptr<IGaitPatternGenerator>&&
                                     gait_pattern_generator_for_turn_spot);

  ~GaitPatternGeneratorSwitchMove() = default;

  nostd::expected<RobotStateNode, std::string> GetNextNodeByGraphSearch(
      const RobotStateNode& current_node, const MapState& map_ref,
      const RobotOperation& operation) override;

  int GetExpandedNodeCount() const override;

  std::vector<int> GetExpandedNodeCountPerDepth() const override;

 private:
  const std::unique_ptr<IGaitPatternGenerator> gpg_for_straight_ptr_;
  const std::unique_ptr<IGaitPatternGenerator> gpg_for_turn_spot_ptr_;

  RobotOperation last_operation_{};
};

}  // namespace gaitgen
