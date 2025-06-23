
//! @file graph_searcher_straight_move.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "graph_searcher_plane.h"

#include <functional>
#include <vector>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "leg_state.h"
#include "math_util.h"

namespace designlab {

GraphSearcherPlane::GraphSearcherPlane(
    const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
    const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr)
    : checker_ptr_(checker_ptr),
      presenter_ptr_(presenter_ptr),
      converter_ptr_(converter_ptr),
      evaluator_(InitializeEvaluator()) {}

nostd::expected<GraphSearcherPlane::return_type, std::string>
GraphSearcherPlane::SearchGraphTree(const GaitPatternGraphTree& graph,
                                    const RobotOperation& operation,
                                    const DividedMapState& divided_map_state,
                                    const int max_depth) const {
  // ターゲットモードは直進である.
  assert(operation.operation_type ==
             RobotOperationType::kStraightMovePosition ||
         operation.operation_type == RobotOperationType::kStraightMoveVector);

  if (!graph.HasRoot()) {
    // 根ノードがない場合は失敗.
    return nostd::unexpected<std::string>{
        "Failed to search graph tree: "
        "root node is not found."};
  }

  // 初期化.
  Vector3 normalized_move_direction;

  if (operation.operation_type == RobotOperationType::kStraightMovePosition) {
    normalized_move_direction =
        (operation.straight_move_position -
         graph.GetRootNode().center_of_mass_global_coord);
    normalized_move_direction.z = 0.0f;
    normalized_move_direction = normalized_move_direction.GetNormalized();
  } else {
    normalized_move_direction = operation.straight_move_vector;
    normalized_move_direction.z = 0.0f;
    normalized_move_direction = normalized_move_direction.GetNormalized();
    normalized_move_direction = normalized_move_direction.GetLength() == 0.0f
                                    ? Vector3::GetFrontVec()
                                    : normalized_move_direction;
  }

  const float target_z_value = InitTargetZValue(
      graph.GetRootNode(), divided_map_state, normalized_move_direction);

  cmdio::DebugOutputF("target_z_value = {}", target_z_value);

  GraphSearchEvaluationValue max_evaluation_value =
      evaluator_.InitializeEvaluationValue();
  int max_evaluation_value_index = -1;
  int log_depth = 0;
  HexapodMove log_move = HexapodMove::kNone;

  GraphSearchEvaluationValue candidate_evaluation_value =
      evaluator_.InitializeEvaluationValue();

  for (int i = 0; i < graph.GetGraphSize(); i++) {
    log_depth =
        log_depth < graph.GetNode(i).depth ? graph.GetNode(i).depth : log_depth;
    log_move = log_depth == graph.GetNode(i).depth ? graph.GetNode(i).next_move
                                                   : log_move;

    // 最大深さのノードのみを評価する.
    if (graph.GetNode(i).depth != max_depth) {
      continue;
    }

    // 評価値を計算する.
    candidate_evaluation_value.value.at(kTagMoveForward) =
        GetMoveForwardEvaluationValue(graph.GetNode(i), graph.GetRootNode(),
                                      normalized_move_direction);

    candidate_evaluation_value.value.at(kTagLegRot) =
        GetLegRotEvaluationValue(graph.GetNode(i), graph.GetRootNode());

    candidate_evaluation_value.value.at(kTagZDiff) =
        GetPlaneEvaluationValue(graph.GetNode(i), divided_map_state);

    // 評価値を比較する.
    if (evaluator_.LeftIsBetter(candidate_evaluation_value,
                                max_evaluation_value)) {
      // 上回っている場合は更新する.
      max_evaluation_value = candidate_evaluation_value;
      max_evaluation_value_index = i;
    }
  }

  if (graph.GetGraphSize() <= max_evaluation_value_index) {
    // 最大評価値のインデックスが範囲外の場合は失敗.
    return nostd::unexpected<std::string>{
        "Failed to search graph tree: "
        "max evaluation value index is out of range."};
  }

  if (max_evaluation_value_index < 0) {
    const std::string mes = std::format(
        "Don't find leaf node. "
        "max_depth: {}, move: {}",
        log_depth, string_util::EnumToStringRemoveTopK(log_move));

    return nostd::unexpected<std::string>{mes};
  }

  cmdio::DebugOutputF("max_evaluation_value = {}",
                      max_evaluation_value.value[kTagZDiff]);
  cmdio::DebugOutputF(
      "plane : \n{}",
      MakeRobotPlaneRect(graph.GetNode(max_evaluation_value_index)).ToString());

  return return_type{
      max_evaluation_value,
      graph.GetParentNode(max_evaluation_value_index, 1),
  };
}

nostd::expected<GraphSearcherPlane::return_type, std::string>
GraphSearcherPlane::SearchGraphTreeVector(
    const std::vector<GaitPatternGraphTree>& graph_vector,
    const RobotOperation& operation, const DividedMapState& divided_map_state,
    const int max_depth) const {
  std::vector<nostd::expected<return_type, std::string>> result_vector;

  for (const auto& graph : graph_vector) {
    const auto result =
        SearchGraphTree(graph, operation, divided_map_state, max_depth);

    result_vector.push_back(result);
  }

  // 最大評価値を持つものを探す.
  GraphSearchEvaluationValue max_evaluation_value =
      evaluator_.InitializeEvaluationValue();
  int max_evaluation_value_index = -1;

  for (int i = 0; i < result_vector.size(); i++) {
    if (!result_vector[i]) {
      // 失敗しているものは無視する.
      continue;
    }

    const auto& [evaluation_value, _] = *result_vector[i];

    // 評価値を比較する.
    if (evaluator_.LeftIsBetter(evaluation_value, max_evaluation_value)) {
      // 上回っている場合は更新する.
      max_evaluation_value = evaluation_value;
      max_evaluation_value_index = i;
    }
  }

  // インデックスが範囲外ならば失敗.
  if (max_evaluation_value_index < 0 ||
      result_vector.size() <= max_evaluation_value_index) {
    // 最大評価値のインデックスが範囲外の場合は失敗.
    return nostd::unexpected<std::string>{
        "Failed to search graph tree: "
        "max evaluation value index is out of range."};
  }

  return *result_vector[max_evaluation_value_index];
}

GraphSearchEvaluator GraphSearcherPlane::InitializeEvaluator() const {
  GraphSearchEvaluator::EvaluationMethod move_forward_method = {
      .is_lower_better = false,
      .margin = 0.0f,
  };

  GraphSearchEvaluator::EvaluationMethod leg_rot_method = {
      .is_lower_better = false,
      .margin = 0.0f,
  };

  GraphSearchEvaluator::EvaluationMethod z_diff_method = {
      .is_lower_better = true,
      .margin = 5.0f,
  };

  GraphSearchEvaluator ret({{kTagMoveForward, move_forward_method},
                            {kTagLegRot, leg_rot_method},
                            {kTagZDiff, z_diff_method}},
                           {kTagZDiff, kTagMoveForward, kTagLegRot});

  return ret;
}

float GraphSearcherPlane::InitTargetZValue(
    const RobotStateNode& node, const DividedMapState& divided_map_state,
    const Vector3& move_direction) const {
  const float move_length = 100.0f;

  const Vector3 target_position = move_direction * move_length;

  const int div = 300;
  const float min_z = -150.0f;
  const float max_z = 150.0f;

  for (int i = 0; i < div; i++) {
    const float z = min_z + (max_z - min_z) / static_cast<float>(div) *
                                static_cast<float>(i);

    Vector3 pos = node.center_of_mass_global_coord;
    pos += target_position;
    pos.z += z;

    RobotStateNode temp_node = node;
    temp_node.ChangeGlobalCenterOfMass(pos, false);

    if (!checker_ptr_->IsBodyInterferingWithGround(temp_node,
                                                   divided_map_state)) {
      return node.center_of_mass_global_coord.z + z;
    }
  }

  return node.center_of_mass_global_coord.z;
}

float GraphSearcherPlane::GetMoveForwardEvaluationValue(
    const RobotStateNode& node, const RobotStateNode& root_node,
    const Vector3& normalized_move_direction) const {
  // 正規化されていることを確認する.
  assert(math_util::IsEqual(normalized_move_direction.GetSquaredLength(), 1.f));

  const Vector3 root_to_current =
      node.center_of_mass_global_coord - root_node.center_of_mass_global_coord;

  // root_to_current の normalized_move_direction 方向の成分を取り出す.
  const float result = root_to_current.Dot(normalized_move_direction);

  return result;

  // const float margin = 7.5f;
}

float GraphSearcherPlane::GetLegRotEvaluationValue(
    const RobotStateNode& node, const RobotStateNode& root_node) const {
  float result = 0.0f;

  for (int i = 0; i < HexapodConst::kLegNum; i++) {
    if (leg_func::IsGrounded(node.leg_state, i)) {
      result +=
          (node.leg_pos[i].ProjectedXY() - root_node.leg_pos[i].ProjectedXY())
              .GetLength();
    } else {
      result += (node.leg_pos[i] - root_node.leg_pos[i]).GetLength();
    }
  }

  return result;

  // const float margin = 10.0f;
}

float GraphSearcherPlane::GetPlaneEvaluationValue(
    const RobotStateNode& node, const DividedMapState& divide_map_state) const {
  PlaneRect plane_rect = MakeRobotPlaneRect(node);
  const auto planes = divide_map_state.GetDividedMapPlane();
  std::vector<float> plane_heights;
  for (const auto& plane : planes) {
    // 平面とロボットの平面矩形の距離を求める.
    for (const auto& corner : plane_rect.corners) {
      const auto res =
          IntersectRayWithPlaneRect(corner, plane_rect.normal, plane);
      if (res) {
        const auto& [dist, _] = *res;
        // 平面との距離を格納する.
        plane_heights.push_back(dist);
      }
    }
  }

  // 平均値を結果として返す.
  if (plane_heights.empty()) {
    // 平面がない場合は0を返す.
    return 0.0f;
  }

  float sum = 0.0f;
  for (const auto& height : plane_heights) {
    sum += height;
  }
  const float average_height = sum / static_cast<float>(plane_heights.size());
  return average_height;
}

PlaneRect GraphSearcherPlane::MakeRobotPlaneRect(
    const RobotStateNode& node) const {
  // 0, 2, 3, 5 番脚の付け根の座標を取得し，
  // 平面矩形を作成する.

  PlaneRect plane_rect;
  std::vector<int> leg_indices = {0, 2, 3, 5};
  int idx = 0;
  for (const auto& leg_index : leg_indices) {
    const Vector3 leg_root_pos =
        presenter_ptr_->GetLegBasePosRobotCoordinate(leg_index);
    const Vector3 leg_root_pos_global =
        converter_ptr_->ConvertLegToGlobalCoordinate(
            leg_root_pos, leg_index, node.center_of_mass_global_coord,
            node.posture, true);

    plane_rect.corners.at(idx) = leg_root_pos_global;
    idx++;
  }

  // ロボットの姿勢から，法線を求める.
  const Vector3 normal = RotateVector3(Vector3::GetUpVec() * -1, node.posture);
  plane_rect.normal = normal;

  return plane_rect;
}

}  // namespace designlab
