
//! @file gait_pattern_generator_revaluation.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "gait_pattern_generator_revaluation.h"

#include <utility>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "gait_pattern_graph_tree.h"
#include "map_state.h"

namespace gaitgen {

GaitPatternGeneratorRevaluation::GaitPatternGeneratorRevaluation(
    std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_ptr,
    std::unique_ptr<IGaitPatternGenerator>&&
        gait_pattern_generator_revaluation_ptr,
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
    const std::shared_ptr<const IHexapodJointCalculator>& joint_calculator_ptr)
    : gpg_ptr_(std::move(gait_pattern_generator_ptr)),
      gpg_revaluation_ptr_(std::move(gait_pattern_generator_revaluation_ptr)),
      converter_ptr_(converter_ptr),
      joint_calculator_ptr_(joint_calculator_ptr),
      interpolated_node_creator_{converter_ptr} {
  // gpg_ptr_ は nullptrでない.
  assert(gpg_ptr_ != nullptr);

  // gpg_revaluation_ptr_ は nullptrでない.
  assert(gpg_revaluation_ptr_ != nullptr);

  // converter_ptr_ は nullptrでない.
  assert(converter_ptr_ != nullptr);

  // joint_calculator_ptr_ は nullptrでない.
  assert(joint_calculator_ptr_ != nullptr);
}

nostd::expected<RobotStateNode, std::string>
GaitPatternGeneratorRevaluation::GetNextNodeByGraphSearch(
    const RobotStateNode& current_node, const MapState& map_state,
    const RobotOperation& operation) {
  const auto result =
      gpg_ptr_->GetNextNodeByGraphSearch(current_node, map_state, operation);

  if (!result) {
    // グラフ探索に失敗した場合は終了.
    return result;
  }

  // 成功した場合は,逆運動学計算で脚軌道生成が可能であるか確認する.
  if (IsValidNode(current_node, *result)) {
    // 有効なノードである場合は,そのまま終了.
    return result;
  }

  // 逆運動学計算で脚軌道生成が不可能な場合は,再評価を行う.
  return gpg_revaluation_ptr_->GetNextNodeByGraphSearch(current_node, map_state,
                                                        operation);
}

bool GaitPatternGeneratorRevaluation::IsValidNode(
    const RobotStateNode& current_node, const RobotStateNode& next_node) const {
  // まず,next_nodeが有効なノードであるかを確認する.
  const auto joint = joint_calculator_ptr_->CalculateAllJointState(next_node);

  if (!joint_calculator_ptr_->IsValidAllJointState(next_node, joint)) {
    // 逆運動学計算に失敗した場合は無効なノードとする.
    return false;
  }

  // 次に補間ノードを生成する.
  const std::vector<RobotStateNode> interpolated_node =
      interpolated_node_creator_.CreateInterpolatedNode(current_node,
                                                        next_node);

  for (const auto& node : interpolated_node) {
    const auto joint_interpolated =
        joint_calculator_ptr_->CalculateAllJointState(node);

    if (!joint_calculator_ptr_->IsValidAllJointState(node,
                                                     joint_interpolated)) {
      // 逆運動学計算に失敗した場合は無効なノードとする.
      return false;
    }
  }

  return true;
}

int GaitPatternGeneratorRevaluation::GetExpandedNodeCount() const {
  // もしこの関数を使用したい場合は，以下の assert 文を削除すること.
  assert("この関数はまだ未実装のため，非推奨です．" && false);
  return 0;
}

std::vector<int> GaitPatternGeneratorRevaluation::GetExpandedNodeCountPerDepth()
    const {
  // もしこの関数を使用したい場合は，以下の assert 文を削除すること.
  assert("この関数はまだ未実装のため，非推奨です．" && false);
  return {};
}

}  // namespace gaitgen
