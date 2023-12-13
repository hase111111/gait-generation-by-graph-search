#include "graph_searcher_straight_move.h"

#include <iostream>
#include <cmath>

#include "cassert_define.h"
#include "designlab_math_util.h"
#include "graph_search_const.h"
#include "leg_state.h"

namespace dl = ::designlab;
namespace dle = ::designlab::enums;
namespace dllf = ::designlab::leg_func;
namespace dlm = ::designlab::math_util;


GraphSearcherStraightMove::GraphSearcherStraightMove(const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr) :
	checker_ptr_(checker_ptr)
{
}

std::tuple<GraphSearchResult, int, int> GraphSearcherStraightMove::SearchGraphTree(
	const GaitPatternGraphTree& graph,
	const RobotOperation& operation,
	const int max_depth
) const
{
	assert(operation.operation_type == dle::RobotOperationType::kStraightMovePosition || operation.operation_type == dle::RobotOperationType::kStraightMoveVector);	//ターゲットモードは直進である．

	int result_index = -1;	//糞みたいな書き方なので，後で直す
	float max_move_length = 0.f;
	float max_leg_rot_angle = 0.f;
	float max_margin = 0.f;
	float min_com_z_dif = 0.f;

	if (!graph.HasRoot())
	{
		return { GraphSearchResult::kFailureByNoNode, -1, -1 };
	}

	for (int i = 0; i < graph.GetGraphSize(); i++)
	{
		//最大深さのノードのみを評価する
		if (graph.GetNode(i).depth == max_depth)
		{
			//結果が見つかっていない場合は，必ず結果を更新する
			if (result_index < 0)
			{
				result_index = static_cast<int>(i);
				max_move_length = CalcMoveFrowardEvaluationValue(graph.GetNode(i), operation);
				max_leg_rot_angle = CalcLegRotEvaluationValue(graph.GetNode(i), graph.GetRootNode());
				max_margin = checker_ptr_->CalculateStabilityMargin(graph.GetNode(i).leg_state, graph.GetNode(i).leg_pos);
				min_com_z_dif = abs(graph.GetNode(i).global_center_of_mass.z - graph.GetRootNode().global_center_of_mass.z);
				continue;
			}

			const float candiate_rot_angle = CalcMoveFrowardEvaluationValue(graph.GetNode(i), operation);
			const float candiate_leg_rot_angle = CalcLegRotEvaluationValue(graph.GetNode(i), graph.GetRootNode());
			const float candiate_margin = checker_ptr_->CalculateStabilityMargin(graph.GetNode(i).leg_state, graph.GetNode(i).leg_pos);
			const float candiate_leg_dif = abs(graph.GetNode(i).global_center_of_mass.z - graph.GetRootNode().global_center_of_mass.z);

			if (max_move_length < candiate_rot_angle)
			{
				max_move_length = candiate_rot_angle;
				max_leg_rot_angle = candiate_leg_rot_angle;
				max_margin = candiate_margin;
				min_com_z_dif = candiate_leg_dif;
				result_index = static_cast<int>(i);
			}
			else if (dlm::IsEqual(max_move_length, candiate_rot_angle))
			{
				if (min_com_z_dif > candiate_leg_dif)
				{
					max_move_length = candiate_rot_angle;
					max_leg_rot_angle = candiate_leg_rot_angle;
					max_margin = candiate_margin;
					min_com_z_dif = candiate_leg_dif;
					result_index = static_cast<int>(i);
				}
				else if (dlm::IsEqual(min_com_z_dif, candiate_leg_dif))
				{
					if (max_leg_rot_angle < candiate_leg_rot_angle)
					{
						max_move_length = candiate_rot_angle;
						max_leg_rot_angle = candiate_leg_rot_angle;
						max_margin = candiate_margin;
						min_com_z_dif = candiate_leg_dif;
						result_index = static_cast<int>(i);
					}
					else if (dlm::IsEqual(max_leg_rot_angle, candiate_leg_rot_angle))
					{
						if (max_margin < candiate_margin)
						{
							max_move_length = candiate_rot_angle;
							max_leg_rot_angle = candiate_leg_rot_angle;
							max_margin = candiate_margin;
							min_com_z_dif = candiate_leg_dif;
							result_index = static_cast<int>(i);
						}
					}
				}

			}
		}
	}

	// index が範囲外ならば失敗
	if (result_index < 0 || result_index >= graph.GetGraphSize())
	{
		return { GraphSearchResult::kFailureByNoNode, -1, -1 };
	}

	return { GraphSearchResult::kSuccess, graph.GetParentNodeIndex(result_index, 1), result_index };
}

bool GraphSearcherStraightMove::UpdateEvaluationValueByAmoutOfMovement(
	const int index, 
	const GaitPatternGraphTree& tree, 
	const RobotOperation& operation, 
	EvaluationValue* candiate
)
{
	return false;
}

float GraphSearcherStraightMove::CalcMoveFrowardEvaluationValue(const RobotStateNode& current_node, const RobotOperation& operation) const
{
	const float target_weight = 100000.f;	//方向指定の際のターゲットの重み．

	dl::Vector3 target_pos = operation.operation_type == dle::RobotOperationType::kStraightMovePosition ?
		operation.straight_move_position_ : operation.straight_move_vector_ * target_weight + current_node.global_center_of_mass;

	dl::Vector3 target_to_parent = current_node.global_center_of_mass - target_pos;

	return target_pos.GetLength() - target_to_parent.GetLength();
}

float GraphSearcherStraightMove::CalcLegRotEvaluationValue(const RobotStateNode& current_node, const RobotStateNode& parent_node) const
{
	float result = 0.0f;

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (dllf::IsGrounded(current_node.leg_state, i))
		{
			result += (current_node.leg_pos[i].ProjectedXY() - parent_node.leg_pos[i].ProjectedXY()).GetLength();
		}
		else
		{
			result += (current_node.leg_pos[i] - parent_node.leg_pos[i]).GetLength();
		}
	}

	return result / static_cast<float>(HexapodConst::kLegNum);
}


float GraphSearcherStraightMove::EvaluationValue::CalcMoveForwardEvaluationValue([[maybe_unused]] int selected_index, [[maybe_unused]] const GaitPatternGraphTree& tree) const
{
	return 0.0f;
}