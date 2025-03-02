﻿
//! @file graph_searcher_spot_turn.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "graph_searcher_spot_turn.h"

#include <vector>

#include "cassert_define.h"
#include "graph_search_const.h"


namespace designlab
{

GraphSearcherSpotTurn::GraphSearcherSpotTurn(
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
    const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr) :
    converter_ptr_(converter_ptr),
    checker_ptr_(checker_ptr),
    evaluator_(InitializeEvaluator())
{
}

std::tuple<GraphSearchResult, GraphSearchEvaluationValue, RobotStateNode> GraphSearcherSpotTurn::SearchGraphTree(
    const GaitPatternGraphTree& graph,
    const RobotOperation& operation,
    const DividedMapState& divided_map_state,
    const int max_depth) const
{
    assert(operation.operation_type == RobotOperationType::kSpotTurnLastPosture ||
           operation.operation_type == RobotOperationType::kSpotTurnRotAxis);

    if (!graph.HasRoot())
    {
        const GraphSearchResult result = { enums::Result::kFailure, "ルートノードがありません．" };
        return { result, GraphSearchEvaluationValue{}, RobotStateNode{} };
    }

    // 初期化．
    Quaternion target_quaternion;

    if (operation.operation_type == RobotOperationType::kSpotTurnLastPosture)
    {
        target_quaternion = operation.spot_turn_last_posture;
    }
    else
    {
        // 回転軸周りに20deg回転クォータニオンを作成する．
        const auto rot_quaternion = Quaternion::MakeByAngleAxis(math_util::ConvertDegToRad(20.0f), operation.spot_turn_rot_axis);
        target_quaternion = rot_quaternion * graph.GetRootNode().posture;
    }

    const float target_z_value = InitTargetZValue(graph.GetRootNode(), divided_map_state, target_quaternion);


    GraphSearchEvaluationValue max_evaluation_value = evaluator_.InitializeEvaluationValue();
    int max_evaluation_value_index = -1;

    for (int i = 0; i < graph.GetGraphSize(); i++)
    {
        // 最大深さのノードのみを評価する．
        if (graph.GetNode(i).depth != max_depth)
        {
            continue;
        }

        // 評価値を計算する．
        GraphSearchEvaluationValue candidate_evaluation_value = evaluator_.InitializeEvaluationValue();

        candidate_evaluation_value.value.at(kTagAmountOfTurn) = GetAmountOfTurnEvaluationValue(graph.GetNode(i), target_quaternion);
        candidate_evaluation_value.value.at(kTagLegRot) = GetLegRotEvaluationValue(graph.GetNode(i), graph.GetRootNode());
        candidate_evaluation_value.value.at(kTagZDiff) = GetZDiffEvaluationValue(graph.GetNode(i), target_z_value);

        // 評価値を比較する．
        if (evaluator_.LeftIsBetter(candidate_evaluation_value, max_evaluation_value))
        {
            max_evaluation_value = candidate_evaluation_value;
            max_evaluation_value_index = i;
        }
    }

    // インデックスが範囲外ならば失敗．
    if (max_evaluation_value_index < 0 || graph.GetGraphSize() <= max_evaluation_value_index)
    {
        const GraphSearchResult result = { enums::Result::kFailure, "最大評価値のインデックスが範囲外です．" };
        return { result, GraphSearchEvaluationValue{}, RobotStateNode{} };
    }

    const GraphSearchResult result = { enums::Result::kSuccess, "" };

    return { result, max_evaluation_value, graph.GetParentNode(max_evaluation_value_index, 1), };
}

std::tuple<GraphSearchResult, GraphSearchEvaluationValue, RobotStateNode> GraphSearcherSpotTurn::SearchGraphTreeVector(
    const std::vector<GaitPatternGraphTree>& graph_vector,
    const RobotOperation& operation,
    const DividedMapState& divided_map_state,
    int max_depth) const
{
    std::vector<std::tuple<GraphSearchResult, GraphSearchEvaluationValue, RobotStateNode>> result_vector;

    for (const auto& graph : graph_vector)
    {
        const auto result = SearchGraphTree(graph, operation, divided_map_state, max_depth);

        result_vector.push_back(result);
    }

    // 最大評価値を持つものを探す．
    GraphSearchEvaluationValue max_evaluation_value = evaluator_.InitializeEvaluationValue();
    int max_evaluation_value_index = -1;

    for (int i = 0; i < result_vector.size(); i++)
    {
        const auto& [result, evaluation_value, _] = result_vector.at(i);

        if (result.result != enums::Result::kSuccess)
        {
            continue;
        }

        if (evaluator_.LeftIsBetter(evaluation_value, max_evaluation_value))
        {
            max_evaluation_value = evaluation_value;
            max_evaluation_value_index = i;
        }
    }

    // インデックスが範囲外ならば失敗．
    if (max_evaluation_value_index < 0 || max_evaluation_value_index >= result_vector.size())
    {
        const GraphSearchResult result = { enums::Result::kFailure, "最大評価値のインデックスが範囲外です．" };
        return { result, GraphSearchEvaluationValue{}, RobotStateNode{} };
    }

    return result_vector[max_evaluation_value_index];
}

GraphSearchEvaluator GraphSearcherSpotTurn::InitializeEvaluator() const
{
    GraphSearchEvaluator::EvaluationMethod amount_of_turn_method =
    {
        .is_lower_better = false,
        .margin = 0.0f,
    };

    GraphSearchEvaluator::EvaluationMethod leg_rot_method =
    {
        .is_lower_better = false,
        .margin = 50.0f,
    };

    GraphSearchEvaluator::EvaluationMethod z_diff_method =
    {
        .is_lower_better = true,
        .margin = 1.0f,
    };

    GraphSearchEvaluator ret({ {kTagAmountOfTurn, amount_of_turn_method}, {kTagLegRot, leg_rot_method}, {kTagZDiff, z_diff_method} },
                             { kTagAmountOfTurn, kTagLegRot, kTagZDiff, });

    return ret;
}

float GraphSearcherSpotTurn::InitTargetZValue(const RobotStateNode& node,
                                              const DividedMapState& divided_map_state,
                                              [[maybe_unused]] const Quaternion& target_quat) const
{
    const int div = 60;
    const float min_z = -150.0f;
    const float max_z = 150.0f;

    for (int i = 0; i < div; i++)
    {
        const float z = min_z + (max_z - min_z) / static_cast<float>(div) * static_cast<float>(i);

        Vector3 pos = node.center_of_mass_global_coord;
        pos.z += z;

        RobotStateNode temp_node = node;
        temp_node.ChangeGlobalCenterOfMass(pos, false);
        temp_node.ChangePosture(converter_ptr_, target_quat);

        if (!checker_ptr_->IsBodyInterferingWithGround(temp_node, divided_map_state))
        {
            // std::cout << "z = " << node.center_of_mass_global_coord.z + z << std::endl;
            return node.center_of_mass_global_coord.z + z;
        }
    }

    return node.center_of_mass_global_coord.z;
}

float GraphSearcherSpotTurn::GetAmountOfTurnEvaluationValue(
    const RobotStateNode& node,
    const Quaternion& target_quat) const
{
    // 目標姿勢を Qt，現在の姿勢を Qc とする．
    // Qt = Qc * Qd となるような Qd を求める．
    // Qd = Qc^-1 * Qt となる．

    // 最終姿勢を表すクォータニオンとの差分を計算する．
    const Quaternion target_to_current = node.posture.GetConjugate() * target_quat;

    return target_to_current.w;
}

float GraphSearcherSpotTurn::GetLegRotEvaluationValue(
    const RobotStateNode& node,
    const RobotStateNode& root_node) const
{
    float result = 0.0f;

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        if (leg_func::IsGrounded(node.leg_state, i))
        {
            result += (node.leg_pos[i].ProjectedXY() -
                       root_node.leg_pos[i].ProjectedXY()).GetLength();
        }
        else
        {
            result += (node.leg_pos[i] - root_node.leg_pos[i]).GetLength();
        }
    }

    return result;

    // const float margin = 50.0f;
}

float GraphSearcherSpotTurn::GetZDiffEvaluationValue(
    const RobotStateNode& node,
    const float target_z_value) const
{
    return abs(node.center_of_mass_global_coord.z - target_z_value);
}


}  // namespace designlab
