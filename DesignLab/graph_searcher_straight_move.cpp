
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#include "graph_searcher_straight_move.h"

#include <functional>
#include <vector>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "math_util.h"
#include "graph_search_const.h"
#include "leg_state.h"


namespace designlab
{

GraphSearcherStraightMove::GraphSearcherStraightMove(
    const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr) :
    checker_ptr_(checker_ptr),
    evaluator_(InitializeEvaluator())
{
}

std::tuple<GraphSearchResult, GraphSearchEvaluationValue, RobotStateNode> GraphSearcherStraightMove::SearchGraphTree(
    const GaitPatternGraphTree& graph,
    const RobotOperation& operation,
    const DividedMapState& divided_map_state,
    const int max_depth) const
{
    // ターゲットモードは直進である．
    assert(operation.operation_type == enums::RobotOperationType::kStraightMovePosition ||
           operation.operation_type == enums::RobotOperationType::kStraightMoveVector);

    if (!graph.HasRoot())
    {
        const GraphSearchResult result = { enums::Result::kFailure, "ルートノードがありません．" };
        return { result, GraphSearchEvaluationValue{}, RobotStateNode{} };
    }

    // 初期化．
    Vector3 normalized_move_direction;

    if (operation.operation_type == enums::RobotOperationType::kStraightMovePosition)
    {
        normalized_move_direction = (operation.straight_move_position_ -
                                                graph.GetRootNode().center_of_mass_global_coord);
        normalized_move_direction.z = 0.0f;
        normalized_move_direction = normalized_move_direction.GetNormalized();
    }
    else
    {
        normalized_move_direction = operation.straight_move_vector_;
        normalized_move_direction.z = 0.0f;
        normalized_move_direction = normalized_move_direction.GetNormalized();
        normalized_move_direction = normalized_move_direction.GetLength() == 0.0f ? Vector3::GetFrontVec() : normalized_move_direction;
    }

    const float target_z_value = InitTargetZValue(graph.GetRootNode(), divided_map_state, normalized_move_direction);

    CmdIOUtil::FormatOutput(OutputDetail::kDebug, "target_z_value = {}", target_z_value);

    GraphSearchEvaluationValue max_evaluation_value = evaluator_.InitializeEvaluationValue();
    int max_evaluation_value_index = -1;
    int log_depth = 0;
    enums::HexapodMove log_move = enums::HexapodMove::kNone;

    GraphSearchEvaluationValue candidate_evaluation_value = evaluator_.InitializeEvaluationValue();

    for (int i = 0; i < graph.GetGraphSize(); i++)
    {
        log_depth = log_depth < graph.GetNode(i).depth ? graph.GetNode(i).depth : log_depth;
        log_move = log_depth == graph.GetNode(i).depth ? graph.GetNode(i).next_move : log_move;

        // 最大深さのノードのみを評価する．
        if (graph.GetNode(i).depth != max_depth) { continue; }

        // 評価値を計算する．
        candidate_evaluation_value.value.at(kTagMoveForward) = GetMoveForwardEvaluationValue(graph.GetNode(i), graph.GetRootNode(), normalized_move_direction);
        // if (!evaluator_.LeftIsBetterWithTag(candidate_evaluation_value, max_evaluation_value, kTagMoveForward)) { continue; }

        candidate_evaluation_value.value.at(kTagLegRot) = GetLegRotEvaluationValue(graph.GetNode(i), graph.GetRootNode());
        // if (!evaluator_.LeftIsBetterWithTag(candidate_evaluation_value, max_evaluation_value, kTagLegRot)) { continue; }

        candidate_evaluation_value.value.at(kTagZDiff) = GetZDiffEvaluationValue(graph.GetCoMVerticalTrajectory(i), target_z_value);
        //candidate_evaluation_value.value.at(kTagZDiff) = GetZDiffEvaluationValue({ graph.GetNode(i).center_of_mass_global_coord.z }, target_z_value);

        // 評価値を比較する．
        if (evaluator_.LeftIsBetter(candidate_evaluation_value, max_evaluation_value))
        {
            // 上回っている場合は更新する．
            CmdIOUtil::FormatOutput(OutputDetail::kDebug, "max_evaluation_value = {}", max_evaluation_value.value[kTagZDiff]);
            max_evaluation_value = candidate_evaluation_value;
            max_evaluation_value_index = i;
        }
    }

    // インデックスが範囲外ならば失敗．
    if (graph.GetGraphSize() <= max_evaluation_value_index)
    {
        const GraphSearchResult result = { enums::Result::kFailure, "最大評価値のインデックスが範囲外です．" };
        return { result, GraphSearchEvaluationValue{}, RobotStateNode{} };
    }

    if (max_evaluation_value_index < 0)
    {
        const GraphSearchResult result =
        {
            enums::Result::kFailure,
            std::format("葉ノードが存在しません．最大深さ : {}，動作 : {}", log_depth, string_util::EnumToStringRemoveTopK(log_move))
        };

        return { result, GraphSearchEvaluationValue{}, RobotStateNode{} };
    }

    const GraphSearchResult result = { enums::Result::kSuccess, "" };

    return { result, max_evaluation_value, graph.GetParentNode(max_evaluation_value_index, 1) };
}

std::tuple<GraphSearchResult, GraphSearchEvaluationValue, RobotStateNode> GraphSearcherStraightMove::SearchGraphTreeVector(
    const std::vector<GaitPatternGraphTree>& graph_vector,
    const RobotOperation& operation,
    const DividedMapState& divided_map_state,
    int max_depth) const
{
    std::vector<std::tuple<GraphSearchResult, GraphSearchEvaluationValue, RobotStateNode>> result_vector;
    result_vector.resize(graph_vector.size());

    for (size_t i = 0; i < graph_vector.size(); i++)
    {
        // グラフ探索の結果を格納する．
        result_vector[i] = SearchGraphTree(graph_vector[i], operation, divided_map_state, max_depth);
    }

    // 最大評価値を持つものを探す．
    GraphSearchEvaluationValue max_evaluation_value = evaluator_.InitializeEvaluationValue();
    int max_evaluation_value_index = -1;

    for (int i = 0; i < result_vector.size(); i++)
    {
        const auto& [result, evaluation_value, _] = result_vector[i];

        // 失敗しているものは無視する．
        if (result.result != enums::Result::kSuccess)
        {
            continue;
        }

        // 評価値を比較する．
        if (evaluator_.LeftIsBetter(evaluation_value, max_evaluation_value))
        {
            // 上回っている場合は更新する．
            max_evaluation_value = evaluation_value;
            max_evaluation_value_index = i;
        }
    }

    // インデックスが範囲外ならば失敗．
    if (max_evaluation_value_index < 0 || result_vector.size() <= max_evaluation_value_index)
    {
        const GraphSearchResult result = { enums::Result::kFailure, "最大評価値のインデックスが範囲外です．" };
        return { result, GraphSearchEvaluationValue{}, RobotStateNode{} };
    }

    return result_vector[max_evaluation_value_index];
}

GraphSearchEvaluator GraphSearcherStraightMove::InitializeEvaluator() const
{
    GraphSearchEvaluator::EvaluationMethod move_forward_method =
    {
        .is_lower_better = false,
        .margin = 0.0f,
    };

    GraphSearchEvaluator::EvaluationMethod leg_rot_method =
    {
        .is_lower_better = false,
        .margin = 0.0f,
    };

    GraphSearchEvaluator::EvaluationMethod z_diff_method =
    {
        .is_lower_better = true,
        .margin = 0.f,
    };

    GraphSearchEvaluator ret({ {kTagMoveForward, move_forward_method}, {kTagLegRot, leg_rot_method}, {kTagZDiff, z_diff_method} },
                             { kTagZDiff,  kTagMoveForward, kTagLegRot });

    return ret;
}

float GraphSearcherStraightMove::InitTargetZValue(
    const RobotStateNode& node,
    const DividedMapState& divided_map_state,
    const Vector3& move_direction) const
{
    const float move_length = 100.0f;

    const Vector3 target_position = move_direction * move_length;

    const int div = 300;
    const float min_z = -150.0f;
    const float max_z = 150.0f;

    for (int i = 0; i < div; i++)
    {
        const float z = min_z + (max_z - min_z) / static_cast<float>(div) * static_cast<float>(i);

        Vector3 pos = node.center_of_mass_global_coord;
        pos += target_position;
        pos.z += z;

        RobotStateNode temp_node = node;
        temp_node.ChangeGlobalCenterOfMass(pos, false);

        if (!checker_ptr_->IsBodyInterferingWithGround(temp_node, divided_map_state))
        {
            return node.center_of_mass_global_coord.z + z;
        }
    }

    return node.center_of_mass_global_coord.z;
}

float GraphSearcherStraightMove::GetMoveForwardEvaluationValue(
    const RobotStateNode& node,
    const RobotStateNode& root_node,
    const Vector3& normalized_move_direction) const
{
    // 正規化されていることを確認する．
    assert(math_util::IsEqual(normalized_move_direction.GetSquaredLength(), 1.f));

    const Vector3 root_to_current = node.center_of_mass_global_coord - root_node.center_of_mass_global_coord;

    // root_to_current の normalized_move_direction 方向の成分を取り出す．
    const float result = root_to_current.Dot(normalized_move_direction);

    return result;

    // const float margin = 7.5f;
}

float GraphSearcherStraightMove::GetLegRotEvaluationValue(
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

    // const float margin = 10.0f;
}

float GraphSearcherStraightMove::GetZDiffEvaluationValue(
    const std::vector<float>& com_trajectory,
    const float target_z_value) const
{
    float result = abs(com_trajectory.back() - target_z_value);

    if (com_trajectory.size() == 3)
    {
        if ((com_trajectory[0] - com_trajectory[1]) * (com_trajectory[0] - com_trajectory[2]) <= 0)
        {
            result += abs(com_trajectory[0] - com_trajectory[1]);
        }
    }

    return result;
}

}  // namespace designlab
