﻿
//! @file graph_searcher_spot_turn.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_GRAPH_SEARCHER_SPOT_TURN_H_
#define DESIGNLAB_GRAPH_SEARCHER_SPOT_TURN_H_

#include <memory>
#include <string>
#include <tuple>

#include "interface_graph_searcher.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_posture_validator.h"

namespace designlab {

//! @class GraphSearcherSpotTurn
//! @brief グラフ探索を行い,直進する動作を評価するクラス.
class GraphSearcherSpotTurn final : public IGraphSearcher {
  using Tag = GraphSearchEvaluationValue::Tag;

 public:
  GraphSearcherSpotTurn(
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr);

  nostd::expected<return_type, std::string> SearchGraphTree(
      const GaitPatternGraphTree& graph, const RobotOperation& operation,
      const DividedMapState& divided_map_state, int max_depth) const override;

  nostd::expected<return_type, std::string> SearchGraphTreeVector(
      const std::vector<GaitPatternGraphTree>& graph_vector,
      const RobotOperation& operation, const DividedMapState& divided_map_state,
      int max_depth) const override;

 private:
  static constexpr Tag kTagAmountOfTurn = 0;
  static constexpr Tag kTagLegRot = 1;
  static constexpr Tag kTagZDiff = 2;

  GraphSearchEvaluator InitializeEvaluator() const;

  float InitTargetZValue(const RobotStateNode& node,
                         const DividedMapState& devide_map_state,
                         const Quaternion& target_quat) const;

  float GetAmountOfTurnEvaluationValue(const RobotStateNode& node,
                                       const Quaternion& target_quat) const;

  float GetLegRotEvaluationValue(const RobotStateNode& node,
                                 const RobotStateNode& root_node) const;

  float GetZDiffEvaluationValue(const RobotStateNode& node,
                                const float target_z_value) const;

  const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
  const std::shared_ptr<const IHexapodPostureValidator> checker_ptr_;

  GraphSearchEvaluator evaluator_;
};

}  // namespace designlab

#endif  // DESIGNLAB_GRAPH_SEARCHER_SPOT_TURN_H_
