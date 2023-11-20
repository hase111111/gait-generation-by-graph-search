#include "graph_searcher_random.h"

#include "designlab_math_util.h"

namespace dlm = designlab::math_util;


std::tuple<GraphSearchResult, RobotStateNode, std::unique_ptr<IEvaluationValue>> GraphSearcherRandom::SearchGraphTree(
	const std::vector<RobotStateNode>& graph, 
	const int graph_size,
	[[maybe_unused]]const TargetRobotState& target
) const
{
	//グラフを探索せずに，次の動作の中からランダムに一つ選択して移動する．

	if (graph.empty()) 
	{
		//グラフがないなら失敗	
		const RobotStateNode dummy_node;
		return { GraphSearchResult::kFailure, dummy_node, std::make_unique<RandomEvaluationValue>() };
	}	

	//グラフの中を全て探索する．
	std::vector<RobotStateNode> depth1_node;

	for (size_t i = 0; i < graph_size; i++)
	{
		// 深さ1の動作を全て記録する．
		if (graph[i].depth == 1)
		{
			depth1_node.emplace_back(graph[i]);
		}
	}

	if (depth1_node.empty()) 
	{
		//深さ1のノードが存在しないなら，終了．
		const RobotStateNode dummy_node;
		return { GraphSearchResult::kFailureByNotReachedDepth, dummy_node, std::make_unique<RandomEvaluationValue>() };
	}		

	const int select_index = depth1_node.size() == 1 ? 0 : dlm::GenerateRandomNumber(0, static_cast<int>(depth1_node.size()) - 1);
	const RobotStateNode result_node = depth1_node[static_cast<size_t>(select_index)];									// ランダムなやつを一つ選択する．

	return { GraphSearchResult::kSuccess, result_node, std::make_unique<RandomEvaluationValue>() };
}