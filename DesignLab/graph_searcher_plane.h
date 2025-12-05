
//! @file graph_searcher_plane.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "interface_graph_searcher.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_posture_validator.h"
#include "interface_hexapod_state_presenter.h"

namespace gaitgen {

//! @class GraphSearcherPlane
//! @brief グラフ探索を行い,直進する動作を評価するクラス.
class GraphSearcherPlane final : public IGraphSearcher {
  using Tag = GraphSearchEvaluationValue::Tag;

 public:
  GraphSearcherPlane(
      const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
      const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr);

  nostd::expected<return_type, std::string> SearchGraphTree(
      const GaitPatternGraphTree& graph, const RobotOperation& operation,
      const DividedMapState& divided_map_state, int max_depth) const override;

  nostd::expected<return_type, std::string> SearchGraphTreeVector(
      const std::vector<GaitPatternGraphTree>& graph_vector,
      const RobotOperation& operation, const DividedMapState& divided_map_state,
      int max_depth) const override;

 private:
  static constexpr Tag kTagMoveForward = 0;
  static constexpr Tag kTagLegRot = 1;
  static constexpr Tag kTagStablyMargin = 2;
  static constexpr Tag kTagZDiff = 3;

  GraphSearchEvaluator InitializeEvaluator() const;

  float InitTargetZValue(const RobotStateNode& node,
                         const DividedMapState& devide_map_state,
                         const Vector3& move_direction) const;

  float GetMoveForwardEvaluationValue(
      const RobotStateNode& node, const RobotStateNode& root_node,
      const Vector3& normalized_move_direction) const;

  float GetLegRotEvaluationValue(const RobotStateNode& node,
                                 const RobotStateNode& root_node) const;

  float GetPlaneEvaluationValue(const RobotStateNode& node,
                                const DividedMapState& divide_map_state) const;

  PlaneRect MakeRobotPlaneRect(const RobotStateNode& node) const;

  const std::shared_ptr<const IHexapodPostureValidator> checker_ptr_;
  const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
  const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;

  GraphSearchEvaluator evaluator_;
};

}  // namespace gaitgen
