#include "graph_searcher_hato.h"

#include <iostream>
#include <cmath>

#include "cassert_define.h"
#include "designlab_math_util.h"
#include "graph_search_const.h"
#include "leg_state.h"

namespace dl = ::designlab;
namespace dllf = ::designlab::leg_func;
namespace dlm = ::designlab::math_util;


GraphSearcherHato::GraphSearcherHato(const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr) :
	checker_ptr_(checker_ptr)
{
}

std::tuple<GraphSearchResult, RobotStateNode, int> GraphSearcherHato::SearchGraphTree(
	const GaitPatternGraphTree& graph,
	const TargetRobotState& target
) const
{
	assert(target.target_mode == TargetMode::kStraightMovePosition || target.target_mode == TargetMode::kStraightMoveVector);	//ターゲットモードは直進である．

	int result_index = -1;	//糞みたいな書き方なので，後で直す
	float max_rot_angle = 0.f;
	float max_leg_rot_angle = 0.f;
	float max_margin = 0.f;
	float min_leg_dif = 0.f;

	if (!graph.HasRoot())
	{
		RobotStateNode dummy_node;
		return { GraphSearchResult::kFailureByNoNode, dummy_node, -1 };
	}

	for (int i = 0; i < graph.GetGraphSize(); i++)
	{
		//最大深さのノードのみを評価する
		if (graph.GetNode(i).depth == GraphSearchConst::kMaxDepth)
		{
			//結果が見つかっていない場合は，必ず結果を更新する
			if (result_index < 0)
			{
				result_index = static_cast<int>(i);
				max_rot_angle = CalcMoveFrowardEvaluationValue(graph.GetNode(i), target);
				max_leg_rot_angle = CalcLegRotEvaluationValue(graph.GetNode(i), graph.GetRootNode());
				max_margin = checker_ptr_->CalculateStabilityMargin(graph.GetNode(i).leg_state, graph.GetNode(i).leg_pos);
				min_leg_dif = abs(graph.GetNode(i).global_center_of_mass.z - graph.GetRootNode().global_center_of_mass.z);
				continue;
			}

			const float candiate_rot_angle = CalcMoveFrowardEvaluationValue(graph.GetNode(i), target);
			const float candiate_leg_rot_angle = CalcLegRotEvaluationValue(graph.GetNode(i), graph.GetRootNode());
			const float candiate_margin = checker_ptr_->CalculateStabilityMargin(graph.GetNode(i).leg_state, graph.GetNode(i).leg_pos);
			const float candiate_leg_dif = abs(graph.GetNode(i).global_center_of_mass.z - graph.GetRootNode().global_center_of_mass.z);

			if (max_rot_angle < candiate_rot_angle)
			{
				max_rot_angle = candiate_rot_angle;
				max_leg_rot_angle = candiate_leg_rot_angle;
				max_margin = candiate_margin;
				min_leg_dif = candiate_leg_dif;
				result_index = static_cast<int>(i);
			}
			else if (dlm::IsEqual(max_rot_angle, candiate_rot_angle))
			{
				if (min_leg_dif > candiate_leg_dif)
				{
					max_rot_angle = candiate_rot_angle;
					max_leg_rot_angle = candiate_leg_rot_angle;
					max_margin = candiate_margin;
					min_leg_dif = candiate_leg_dif;
					result_index = static_cast<int>(i);
				}
				else if (dlm::IsEqual(min_leg_dif, candiate_leg_dif))
				{
					if (max_leg_rot_angle < candiate_leg_rot_angle)
					{
						max_rot_angle = candiate_rot_angle;
						max_leg_rot_angle = candiate_leg_rot_angle;
						max_margin = candiate_margin;
						min_leg_dif = candiate_leg_dif;
						result_index = static_cast<int>(i);
					}
					else if (dlm::IsEqual(max_leg_rot_angle, candiate_leg_rot_angle))
					{
						if (max_margin < candiate_margin)
						{
							max_rot_angle = candiate_rot_angle;
							max_leg_rot_angle = candiate_leg_rot_angle;
							max_margin = candiate_margin;
							min_leg_dif = candiate_leg_dif;
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
		RobotStateNode dummy_node;
		return { GraphSearchResult::kFailureByNoNode, dummy_node, -1 };
	}

	return { GraphSearchResult::kSuccess, graph.GetParentNode(result_index, 1), result_index };
}

float GraphSearcherHato::CalcMoveFrowardEvaluationValue(const RobotStateNode& current_node, const TargetRobotState& target) const
{
	dl::Vector3 target_pos = target.target_mode == TargetMode::kStraightMovePosition ? 
		target.target_position : target.target_direction * 100000.0f + current_node.global_center_of_mass;
	dl::Vector3 target_to_parent = current_node.global_center_of_mass - target_pos;

	return target_pos.GetLength() - target_to_parent.GetLength();
}

float GraphSearcherHato::CalcLegRotEvaluationValue(const RobotStateNode& current_node, const RobotStateNode& parent_node) const
{
	float result = 0.0f;

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (dllf::IsGrounded(current_node.leg_state, i))
		{
			result += (current_node.leg_pos[i] - parent_node.leg_pos[i]).GetLength();
		}
	}

	return result / static_cast<float>(HexapodConst::kLegNum);
}