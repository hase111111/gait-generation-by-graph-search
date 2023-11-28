#include "graph_searcher_random.h"

#include "designlab_math_util.h"

namespace dlm = designlab::math_util;


std::tuple<GraphSearchResult, RobotStateNode, int> GraphSearcherRandom::SearchGraphTree(
	const GaitPatternGraphTree& graph,
	[[maybe_unused]]const TargetRobotState& target
) const
{
	//グラフを探索せずに，次の動作の中からランダムに一つ選択して移動する．

	if (graph.Empty()) 
	{
		//グラフがないなら失敗	
		return { GraphSearchResult::kFailure, RobotStateNode(), -1 };
	}	

	//グラフの中を全て探索する．
	std::vector<RobotStateNode> depth1_node;

	for (int i = 0; i < graph.GetGraphSize(); i++)
	{
		// 深さ1の動作を全て記録する．
		if (graph.GetNode(i).depth == 1)
		{
			depth1_node.emplace_back(graph.GetNode(i));
		}
	}

	if (depth1_node.empty()) 
	{
		//深さ1のノードが存在しないなら，終了．
		return { GraphSearchResult::kFailureByNotReachedDepth, RobotStateNode(), -1};
	}		

	// ランダムなやつを一つ選択する．
	const int select_index = depth1_node.size() == 1 ? 0 : dlm::GenerateRandomNumber(0, static_cast<int>(depth1_node.size()) - 1);
	const RobotStateNode result_node = depth1_node[static_cast<size_t>(select_index)];									

	return { GraphSearchResult::kSuccess, result_node, -1 };
}