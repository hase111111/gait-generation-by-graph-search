#include "GraphSearcherRandom.h"
#include <algorithm>
#include <random>

bool GraphSearcherRandom::searchGraphTree(const std::vector<SNode>& _graph, const STarget& _target, SNode& _output_result)
{
	//ここに，グラフを評価して _output_result に値を入れる処理を書く．このクラスはサンプルなのでテキトーに選びます．

	if (_graph.empty() == true) { return false; }	//グラフがないならfalse	


	//グラフの中を全て探索する．
	std::vector<SNode> _depth1_node;

	size_t _graph_size = _graph.size();
	for (size_t i = 0; i < _graph_size; i++)
	{
		// 深さ1，次の動作の中から，一番最初に見つかったものを記録する．
		if (_graph.at(i).depth == 1)
		{
			_depth1_node.push_back(_graph.at(i));
		}
	}

	if (_depth1_node.empty()) return false;

	// シャッフル
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(_depth1_node.begin(), _depth1_node.end(), engine);

	_output_result = _depth1_node.back();
	return true;
}
