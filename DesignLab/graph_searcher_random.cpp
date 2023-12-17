#include "graph_searcher_random.h"

#include "designlab_math_util.h"


namespace designlab
{

std::tuple<GraphSearchResult, int, int> GraphSearcherRandom::SearchGraphTree(
	const GaitPatternGraphTree& graph,
	[[maybe_unused]] const RobotOperation& operation,
	[[maybe_unused]] const int max_depth
) const
{
	//グラフを探索せずに，次の動作の中からランダムに一つ選択して移動する．

	if (graph.IsEmpty())
	{
		//グラフがないなら失敗	
		const GraphSearchResult result = { enums::Result::kFailure ,"グラフが空のため評価できません．" };

		return { result, -1, -1 };
	}

	//グラフの中を全て探索する．
	std::vector<int> depth1_node;

	for (int i = 0; i < graph.GetGraphSize(); ++i)
	{
		// 深さ1の動作を全て記録する．
		if (graph.GetNode(i).depth == 1)
		{
			depth1_node.emplace_back(i);
		}
	}

	if (depth1_node.empty())
	{
		//深さ1のノードが存在しないなら，終了．
		const GraphSearchResult result = { enums::Result::kFailure ,"深さ1のノードが存在しません．" };

		return { result, -1, -1 };
	}

	// ランダムなやつを一つ選択する．
	const int select_index = depth1_node.size() == 1 ? 0 : math_util::GenerateRandomNumber(0, static_cast<int>(depth1_node.size()) - 1);
	const int result_node_index = depth1_node[static_cast<size_t>(select_index)];

	const GraphSearchResult result = { enums::Result::kSuccess ,"" };

	return { result, result_node_index, -1 };
}

}  // namespace designlab