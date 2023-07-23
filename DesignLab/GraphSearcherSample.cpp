#include "GraphSearcherSample.h"

EGraphSearchResult GraphSearcherSample::searchGraphTree(const std::vector<SNode>& _graph, const STarget& _target, SNode& _output_result)
{
	//ここに，グラフを評価して _output_result に値を入れる処理を書く．このクラスはサンプルなのでテキトーに選びます．

	if (_graph.empty() == true) { return EGraphSearchResult::Failure; }	//グラフがないならfalse	


	//グラフの中を全て探索する．
	for (const auto& i : _graph)
	{
		// 深さ1，次の動作の中から，一番最初に見つかったものを結果として出力する．
		if (i.depth == 1)
		{
			//結果を入れて，終了．
			_output_result = i;
			return EGraphSearchResult::Success;
		}
	}

	//見つからなければfalse
	return EGraphSearchResult::FailureByNotReachedDepth;
}