#include "graph_searcher_spot_turn.h"

#include "cassert_define.h"
#include "graph_search_const.h"

namespace dl = ::designlab;
namespace dle = ::designlab::enums;
namespace dlm = ::designlab::math_util;
namespace dllf = ::designlab::leg_func;


std::tuple<GraphSearchResult, int, int> GraphSearcherSpotTurn::SearchGraphTree(
	const GaitPatternGraphTree& graph_tree,
	const RobotOperation& operation,
	const int max_depth
) const
{
	assert(operation.operation_type == dle::RobotOperationType::kSpotTurnLastPosture || operation.operation_type == dle::RobotOperationType::kSpotTurnRotAxis);

	if (!graph_tree.HasRoot())
	{
		const GraphSearchResult result = { dle::Result::kFailure ,"グラフが空のため評価できません．" };

		return { result, -1, -1 };
	}

	int result_index = -1;	//糞みたいな書き方なので，後で直す
	float max_turn_angle = 0.f;
	float max_leg_rot_angle = 0.f;
	float min_com_z_dif = 0.f;

	for (int i = 0; i < graph_tree.GetGraphSize(); i++)
	{
		//最大深さのノードのみを評価する
		if (graph_tree.GetNode(i).depth == max_depth)
		{
			//結果が見つかっていない場合は，必ず結果を更新する
			if (result_index < 0)
			{
				result_index = static_cast<int>(i);
				max_turn_angle = CalcTurnEvaluationValue(graph_tree.GetNode(i), operation);
				max_leg_rot_angle = CalcLegRotEvaluationValue(graph_tree.GetNode(i), graph_tree.GetRootNode());
				min_com_z_dif = abs(graph_tree.GetNode(i).global_center_of_mass.z - graph_tree.GetRootNode().global_center_of_mass.z);
				continue;
			}

			const float candiate_rot_angle = CalcTurnEvaluationValue(graph_tree.GetNode(i), operation);
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
		//深さ1のノードが存在しないなら，終了．
		const GraphSearchResult result = { dle::Result::kFailure ,"深さ1のノードが存在しません．" };

		return { result, -1, -1 };
	}

	const GraphSearchResult result = { dle::Result::kSuccess ,"" };
	return { result, graph_tree.GetParentNodeIndex(result_index, 1), result_index };
}

float GraphSearcherSpotTurn::CalcTurnEvaluationValue(const RobotStateNode& current_node, const RobotOperation& operation) const
{
	const float target_weight = 100000.f;	//方向指定の際のターゲットの重み．

	if (operation.operation_type == dle::RobotOperationType::kSpotTurnLastPosture)
	{
		//最終姿勢を表すクォータニオンとの差分を計算する
		const dl::Quaternion target_quat = operation.spot_turn_last_posture_;
		const dl::Quaternion current_quat = current_node.quat;

		const dl::Quaternion target_to_current = current_quat.GetInverse() * target_quat;

		//回転角を計算する．
		const float rot_angle = dlm::ConvertRadToDeg(2.f * asin(target_to_current.v.GetLength()));

		return target_weight - rot_angle;
	}

	return 0;
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