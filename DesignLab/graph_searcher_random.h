//! @file graph_searcher_random.h
//! @brief ランダムにグラフ探索を行うクラス．

#ifndef DESIGNLAB_GRAPH_SEARCHER_RANDOM_H_
#define DESIGNLAB_GRAPH_SEARCHER_RANDOM_H_

#include "interface_graph_searcher.h"


namespace designlab
{

//! @class GraphSearcherRandom
//! @brief ランダムにグラフ探索を行うクラス．
//! @details 継承の方法の説明用．
class GraphSearcherRandom final : public IGraphSearcher
{
public:

	// 継承時には，戻り値，関数名，引数の型(引数名は違ってもよい)，を全て同じにする必要がある．
	// また，最後に必ずoverrideをつける．

	std::tuple<GraphSearchResult, int, int> SearchGraphTree(const GaitPatternGraphTree& graph, const RobotOperation& operation, int max_depth) const override;
};

} // namespace designlab


#endif // DESIGNLAB_GRAPH_SEARCHER_RANDOM_H_