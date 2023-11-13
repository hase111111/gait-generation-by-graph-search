#include "graph_searcher_hato.h"

#include <iostream>
#include <cmath>

#include "designlab_math_util.h"
#include "graph_search_const.h"
#include "leg_state.h"


namespace dllf = designlab::leg_func;
namespace dlm = designlab::math_util;


GraphSearcherHato::GraphSearcherHato(const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr) :
	checker_ptr_(checker_ptr)
{
}

GraphSearcherHato::~GraphSearcherHato()
{
}

GraphSearchResult GraphSearcherHato::SearchGraphTree(
	const std::vector<RobotStateNode>& graph,
	const int graph_size,
	const TargetRobotState& target,
	RobotStateNode* output_result
)
{
	// targetの値によって，探索方法を変える必要がある．探索方法を抽象化するべき．

	// ターゲットモードが直進と仮定して処理を書いている

	int result_index = -1;
	float max_rot_angle = 0;
	float max_leg_rot_angle = 0;
	float max_margin = 0;
	float min_leg_dif = 0;

	const size_t parent_num = GetParentNodeIndex(graph, graph_size);

	if (parent_num < 0) { return GraphSearchResult::kFailureByNoNode; }

	InitEvaluationValue(graph.at(parent_num), target);

	for (size_t i = 0; i < graph_size; i++)
	{
		//最大深さのノードのみを評価する
		if (graph[i].depth == GraphSearchConst::kMaxDepth)
		{
			//結果が見つかっていない場合は，必ず結果を更新する
			if (result_index < 0)
			{
				result_index = static_cast<int>(i);
				max_rot_angle = CalcMoveFrowardEvaluationValue(graph[i], target);
				max_leg_rot_angle = CalcLegRotEvaluationValue(graph[i], target);
				max_margin = checker_ptr_->CalculateStabilityMargin(graph[i].leg_state, graph[i].leg_pos);
				min_leg_dif = abs(graph[i].global_center_of_mass.z - graph[parent_num].global_center_of_mass.z);
				continue;
			}

			float candiate_rot_angle = CalcMoveFrowardEvaluationValue(graph[i], target);
			float candiate_leg_rot_angle = CalcLegRotEvaluationValue(graph[i], target);
			float candiate_margin = checker_ptr_->CalculateStabilityMargin(graph[i].leg_state, graph[i].leg_pos);
			float candiate_leg_dif = abs(graph[i].global_center_of_mass.z - graph[parent_num].global_center_of_mass.z);

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
	if (result_index < 0 || result_index >= graph_size) { return GraphSearchResult::kFailureByNoNode; }

	//深さ1まで遡って値を返す
	if (GetDepth1NodeFromMaxDepthNode(graph, result_index, output_result) == false) { return GraphSearchResult::kFailureByNotReachedDepth; }

	return GraphSearchResult::kSuccess;
}

size_t GraphSearcherHato::GetParentNodeIndex(const std::vector<RobotStateNode>& graph, const int graph_size) const
{
	size_t parent_num = 0;

	for (size_t i = 0; i < graph_size; i++)
	{
		if (graph.at(i).depth == 0)
		{
			parent_num = i;
			break;
		}
	}

	return parent_num;
}

bool GraphSearcherHato::GetDepth1NodeFromMaxDepthNode(const std::vector<RobotStateNode>& graph, const size_t max_depth_node_index, RobotStateNode* output_node) const
{
	size_t result_index = max_depth_node_index;
	const size_t kGraphSize = graph.size();
	int count = 0;

	while (graph.at(result_index).depth != 1)
	{
		result_index = graph.at(result_index).parent_num;

		if (result_index < 0 || result_index >= kGraphSize) { return false; }

		count++;
		if (count > GraphSearchConst::kMaxDepth) { return false; }
	}

	(*output_node) = graph.at(result_index);
	return true;
}

void GraphSearcherHato::InitEvaluationValue(const RobotStateNode& parent_node, [[maybe_unused]]const TargetRobotState& target)
{
	parent_node_ = parent_node;
}

float GraphSearcherHato::CalcMoveFrowardEvaluationValue(const RobotStateNode& current_node, [[maybe_unused]] const TargetRobotState& target) const
{
	//designlab::Vector3 center_com_dif = current_node.global_center_of_mass - target.target_position;
	//designlab::Vector3 m_target_to_parent = parent_node_.global_center_of_mass - target.target_position;

	//return (int)(m_target_to_parent.ProjectedXY().GetLength() - center_com_dif.ProjectedXY().GetLength()) / 10 * 10.0f;

	designlab::Vector3 target_pos {10000, 0, 0};
	designlab::Vector3 target_to_parent = current_node.global_center_of_mass - target_pos;

	return target_pos.GetLength() - target_to_parent.GetLength();
}

float GraphSearcherHato::CalcLegRotEvaluationValue(const RobotStateNode& current_node, [[maybe_unused]] const TargetRobotState& target) const
{

	float result = 0.0f;

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (dllf::IsGrounded(current_node.leg_state, i))
		{
			result += (current_node.leg_pos[i] - parent_node_.leg_pos[i]).GetLength();
		}
	}

	return result / (float)HexapodConst::kLegNum;
}
