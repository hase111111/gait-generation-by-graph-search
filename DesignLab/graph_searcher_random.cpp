#include "graph_searcher_random.h"

#include "designlab_math.h"


EGraphSearchResult GraphSearcherRandom::SearchGraphTree(const std::vector<SNode>& graph, const STarget& target, SNode* output_result)
{
	//グラフを探索せずに，次の動作の中からランダムに一つ選択して移動する．

	if (graph.empty()) { return EGraphSearchResult::Failure; }	//グラフがないなら失敗	

	//警告回避用
	STarget target_copy = target;

	//グラフの中を全て探索する．
	std::vector<SNode> depth1_node;

	size_t kGraphSize = graph.size();

	for (size_t i = 0; i < kGraphSize; i++)
	{
		// 深さ1の動作を全て記録する．
		if (graph[i].depth == 1)
		{
			depth1_node.emplace_back(graph[i]);
		}
	}

	if (depth1_node.empty()) { return EGraphSearchResult::FailureByNotReachedDepth; }		//深さ1のノードが存在しないなら，終了．


	(*output_result) = depth1_node.at(dl_math::generateRandomNumber(0, (int)depth1_node.size() - 1));		// ランダムなやつを一つ選択する．


	return EGraphSearchResult::Success;
}
