#include "graph_searcher_hato.h"

#include <iostream>
#include <cmath>

#include "graph_search_const.h"
#include "leg_state.h"


GraphSearcherHato::GraphSearcherHato(std::shared_ptr<AbstractHexapodStateCalculator> calc) : AbstractGraphSearcher(calc)
{
	if (GraphSearchConst::DO_DEBUG_PRINT)
	{
		std::cout << "[GraphSearcher] GraphSearcherHato : コンストラクタが呼ばれた" << std::endl;
	}
}

GraphSearcherHato::~GraphSearcherHato()
{
	if (GraphSearchConst::DO_DEBUG_PRINT)
	{
		std::cout << "[GraphSearcher] GraphSearcherHato : デストラクタが呼ばれた" << std::endl;
	}
}

EGraphSearchResult GraphSearcherHato::searchGraphTree(const std::vector<SNode>& graph, const STarget& target, SNode* output_result)
{
	if (GraphSearchConst::DO_DEBUG_PRINT)
	{
		std::cout << "[GraphSearcher] GraphSearcherHato : searchGraphTree() 探索開始\n";
	}

	// _targetの値によって，探索方法を変える必要がある．探索方法を抽象化するべき．

	// @todo initializerで初期化する処理を書く

	// ターゲットモードが直進と仮定して処理を書いている

	int result_index = -1;
	float max_rot_angle = 0;
	float max_leg_rot_angle = 0;
	float max_margin = 0;
	float min_leg_dif = 0;

	const size_t kGraphSize = graph.size();
	size_t parent_num = getParentNodeIndex(graph);

	if (parent_num < 0) { return EGraphSearchResult::FailureByNoNode; }

	initEvaluationValue(graph.at(parent_num), target);

	for (size_t i = 0; i < kGraphSize; i++)
	{
		//最大深さのノードのみを評価する
		if (graph[i].depth == GraphSearchConst::MAX_DEPTH)
		{
			//結果が見つかっていない場合は，必ず結果を更新する
			if (result_index < 0)
			{
				result_index = static_cast<int>(i);
				max_rot_angle = calcMoveFrowardEvaluationValue(graph[i], target);
				max_leg_rot_angle = calcLegRotEvaluationValue(graph[i], target);
				max_margin = mp_calculator->calcStabilityMargin(graph[i].leg_state, graph[i].leg_pos);
				min_leg_dif = abs(graph[i].global_center_of_mass.z - graph[parent_num].global_center_of_mass.z);
				continue;
			}

			float candiate_rot_angle = calcMoveFrowardEvaluationValue(graph[i], target);
			float candiate_leg_rot_angle = calcLegRotEvaluationValue(graph[i], target);
			float candiate_margin = mp_calculator->calcStabilityMargin(graph[i].leg_state, graph[i].leg_pos);
			float candiate_leg_dif = abs(graph[i].global_center_of_mass.z - graph[parent_num].global_center_of_mass.z);

			if (max_rot_angle < candiate_rot_angle)
			{
				max_rot_angle = candiate_rot_angle;
				max_leg_rot_angle = candiate_leg_rot_angle;
				max_margin = candiate_margin;
				min_leg_dif = candiate_leg_dif;
				result_index = static_cast<int>(i);
			}
			else if (dl_math::isEqual(max_rot_angle, candiate_rot_angle))
			{
				if (min_leg_dif > candiate_leg_dif)
				{
					max_rot_angle = candiate_rot_angle;
					max_leg_rot_angle = candiate_leg_rot_angle;
					max_margin = candiate_margin;
					min_leg_dif = candiate_leg_dif;
					result_index = static_cast<int>(i);
				}
				else if (dl_math::isEqual(min_leg_dif, candiate_leg_dif))
				{
					if (max_leg_rot_angle < candiate_leg_rot_angle)
					{
						max_rot_angle = candiate_rot_angle;
						max_leg_rot_angle = candiate_leg_rot_angle;
						max_margin = candiate_margin;
						min_leg_dif = candiate_leg_dif;
						result_index = static_cast<int>(i);
					}
					else if (dl_math::isEqual(max_leg_rot_angle, candiate_leg_rot_angle))
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
	if (result_index < 0 || result_index >= kGraphSize) { return EGraphSearchResult::FailureByNoNode; }

	//深さ1まで遡って値を返す
	if (getDepth1NodeFromMaxDepthNode(graph, result_index, output_result) == false) { return EGraphSearchResult::FailureByNotReachedDepth; }

	if (GraphSearchConst::DO_DEBUG_PRINT)
	{
		std::cout << "[GraphSearcher] GraphSearcherHato : searchGraphTree() 探索終了" << std::endl;
	}

	return EGraphSearchResult::Success;
}

size_t GraphSearcherHato::getParentNodeIndex(const std::vector<SNode>& graph) const
{
	const size_t kGraphSize = graph.size();
	size_t parent_num = -1;

	for (size_t i = 0; i < kGraphSize; i++)
	{
		if (graph.at(i).depth == 0)
		{
			parent_num = i;
			break;
		}
	}

	return parent_num;
}

bool GraphSearcherHato::getDepth1NodeFromMaxDepthNode(const std::vector<SNode>& graph, const size_t max_depth_node_index, SNode* output_node) const
{
	size_t result_index = max_depth_node_index;
	const size_t kGraphSize = graph.size();
	int count = 0;

	while (graph.at(result_index).depth != 1)
	{
		result_index = graph.at(result_index).parent_num;

		if (result_index < 0 || result_index >= kGraphSize) { return false; }

		count++;
		if (count > GraphSearchConst::MAX_DEPTH) { return false; }
	}

	(*output_node) = graph.at(result_index);
	return true;
}

void GraphSearcherHato::initEvaluationValue(const SNode& parent_node, const STarget& target)
{
	m_parent_node = parent_node;
}

float GraphSearcherHato::calcMoveFrowardEvaluationValue(const SNode& current_node, const STarget& target) const
{
	dl_vec::SVector center_com_dif = current_node.global_center_of_mass - target.TargetPosition;
	dl_vec::SVector m_target_to_parent = m_parent_node.global_center_of_mass - target.TargetPosition;

	return (int)(m_target_to_parent.projectedXY().length() - center_com_dif.projectedXY().length()) / 10 * 10.0f;
}

float GraphSearcherHato::calcLegRotEvaluationValue(const SNode& current_node, const STarget& target) const
{
	float result = 0.0f;

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		if (dl_leg::isGrounded(current_node.leg_state, i))
		{
			result += (current_node.leg_pos[i] - m_parent_node.leg_pos[i]).length();
		}
	}

	return result / (float)HexapodConst::LEG_NUM;
}
