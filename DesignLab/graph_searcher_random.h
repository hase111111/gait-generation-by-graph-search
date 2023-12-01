//! @file graph_searcher_random.h
//! @brief ランダムにグラフ探索を行うクラス．


#ifndef DESIGNLAB_GRAPH_SEARCHER_RANDOM_H_
#define DESIGNLAB_GRAPH_SEARCHER_RANDOM_H_


#include "interface_graph_searcher.h"


//! @class GraphSearcherRandom
//! @brief 完全ランダムに次の動作を選んで返します．
//! @n 継承の方法の説明用．
class GraphSearcherRandom final : public IGraphSearcher
{
public:

	// 継承時には，戻り値，関数名，引数の型(引数名は違ってもよい)，を全て同じにする必要がある．
	// また，最後に必ずoverrideをつける．

	std::tuple<GraphSearchResult, int, int> SearchGraphTree(const GaitPatternGraphTree& graph, const TargetRobotState& target, int max_depth) const override;
};


#endif // DESIGNLAB_GRAPH_SEARCHER_RANDOM_H_