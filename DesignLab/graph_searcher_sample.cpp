//#include "graph_searcher_sample.h"
//
//
//
//EGraphSearchResult GraphSearcherSample::searchGraphTree(const std::vector<SNode>& graph, const STarget& target, SNode* output_result)
//{
//	//ここに，グラフを評価して _output_result に値を入れる処理を書く．このクラスはサンプルなのでテキトーに選ぶ．
//
//	if (graph.empty()) { return EGraphSearchResult::Failure; }	//グラフがないならfalse	
//
//
//	//グラフの中を全て探索する．
//	for (const auto& i : graph)
//	{
//		// 深さ1，次の動作の中から，一番最初に見つかったものを結果として出力する．
//		if (i.depth == 1)
//		{
//			//結果を入れて，終了．
//			(*output_result) = i;
//			return EGraphSearchResult::Success;
//		}
//	}
//
//	//見つからなければfalse
//	return EGraphSearchResult::FailureByNotReachedDepth;
//}
//
