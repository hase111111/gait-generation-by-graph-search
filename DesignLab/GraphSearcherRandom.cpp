#include "GraphSearcherRandom.h"
#include"MyMath.h"


EGraphSearchResult GraphSearcherRandom::searchGraphTree(const std::vector<SNode>& _graph, const STarget& _target, SNode& _output_result)
{
	//グラフを探索せずに，次の動作の中からランダムに一つ選択して移動する．

	if (_graph.empty() == true) { return EGraphSearchResult::Failure; }	//グラフがないなら失敗	


	//グラフの中を全て探索する．
	std::vector<SNode> _depth1_node;

	size_t _graph_size = _graph.size();

	for (size_t i = 0; i < _graph_size; i++)
	{
		// 深さ1の動作を全て記録する．
		if (_graph.at(i).depth == 1)
		{
			_depth1_node.push_back(_graph.at(i));
		}
	}

	if (_depth1_node.empty()) { return EGraphSearchResult::FailureByNotReachedDepth; }		//深さ1のノードが存在しないなら，終了．

	_output_result = _depth1_node.at(my_math::generateRandomNumber(0, (int)_depth1_node.size() - 1));		// ランダムなやつを一つ選択する．

	return EGraphSearchResult::Success;
}
