#include "graph_searcher_spot_turn.h"

#include "cassert_define.h"
#include "graph_search_const.h"

namespace dl = ::designlab;
namespace dlm = ::designlab::math_util;
namespace dllf = ::designlab::leg_func;


std::tuple<GraphSearchResult, RobotStateNode, int> GraphSearcherSpotTurn::SearchGraphTree(
	const GaitPatternGraphTree& graph_tree,
	const TargetRobotState& target
) const
{
	assert(target.GetTargetMode() == TargetMode::kSpotTurnLastPosture || target.GetTargetMode() == TargetMode::kSpotTurnRotAxis);

	if (!graph_tree.HasRoot())
	{
		return { GraphSearchResult::kFailureByNoNode, RobotStateNode(), -1 };
	}

	int result_index = -1;	//糞みたいな書き方なので，後で直す
	float max_turn_angle = 0.f;
	float max_leg_rot_angle = 0.f;
	float min_com_z_dif = 0.f;

	if (!graph_tree.HasRoot())
	{
		return { GraphSearchResult::kFailureByNoNode, RobotStateNode(), -1 };
	}

	for (int i = 0; i < graph_tree.GetGraphSize(); i++)
	{
		//最大深さのノードのみを評価する
		if (graph_tree.GetNode(i).depth == GraphSearchConst::kMaxDepth)
		{
			//結果が見つかっていない場合は，必ず結果を更新する
			if (result_index < 0)
			{
				result_index = static_cast<int>(i);
				max_turn_angle = CalcTurnEvaluationValue(graph_tree.GetNode(i), target);
				max_leg_rot_angle = CalcLegRotEvaluationValue(graph_tree.GetNode(i), graph_tree.GetRootNode());
				min_com_z_dif = abs(graph_tree.GetNode(i).global_center_of_mass.z - graph_tree.GetRootNode().global_center_of_mass.z);
				continue;
			}

			const float candiate_rot_angle = CalcTurnEvaluationValue(graph_tree.GetNode(i), target);
			const float candiate_leg_rot_angle = CalcLegRotEvaluationValue(graph_tree.GetNode(i), graph_tree.GetRootNode());
			const float candiate_leg_dif = abs(graph_tree.GetNode(i).global_center_of_mass.z - graph_tree.GetRootNode().global_center_of_mass.z);

			if (max_turn_angle < candiate_rot_angle)
			{
				max_turn_angle = candiate_rot_angle;
				max_leg_rot_angle = candiate_leg_rot_angle;
				min_com_z_dif = candiate_leg_dif;
				result_index = static_cast<int>(i);
			}
			else if (dlm::IsEqual(max_turn_angle, candiate_rot_angle))
			{
				if (min_com_z_dif > candiate_leg_dif)
				{
					max_turn_angle = candiate_rot_angle;
					max_leg_rot_angle = candiate_leg_rot_angle;
					min_com_z_dif = candiate_leg_dif;
					result_index = static_cast<int>(i);
				}
				else if (dlm::IsEqual(min_com_z_dif, candiate_leg_dif))
				{
					if (max_leg_rot_angle < candiate_leg_rot_angle)
					{
						max_turn_angle = candiate_rot_angle;
						max_leg_rot_angle = candiate_leg_rot_angle;
						min_com_z_dif = candiate_leg_dif;
						result_index = static_cast<int>(i);
					}
				}

			}
		}
	}

	// index が範囲外ならば失敗
	if (result_index < 0 || result_index >= graph_tree.GetGraphSize())
	{
		return { GraphSearchResult::kFailureByNoNode, RobotStateNode(), -1 };
	}

	return { GraphSearchResult::kSuccess, graph_tree.GetParentNode(result_index, 1), result_index };
}

float GraphSearcherSpotTurn::CalcTurnEvaluationValue(const RobotStateNode& current_node, const TargetRobotState& target) const
{
	const float target_weight = 100000.f;	//方向指定の際のターゲットの重み．

	dl::Vector3 target_pos = target.GetTargetMode() == TargetMode::kStraightMovePosition ?
		target.GetStraightMovePosition() : target.GetStraightMoveVector() * target_weight + current_node.global_center_of_mass;

	dl::Vector3 target_to_parent = current_node.global_center_of_mass - target_pos;

	return target_pos.GetLength() - target_to_parent.GetLength();
}

float GraphSearcherSpotTurn::CalcLegRotEvaluationValue(const RobotStateNode& current_node, const RobotStateNode& parent_node) const
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