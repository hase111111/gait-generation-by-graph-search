//! @file graph_searcher_hato.h
//! @brief 波東さんの手法でグラフ探索を行うクラスの実装．

#ifndef DESIGNLAB_GRAPH_SEARCHER_SPOT_TURN_H_
#define DESIGNLAB_GRAPH_SEARCHER_SPOT_TURN_H_

#include "interface_graph_searcher.h"

class GraphSearcherSpotTurn : public IGraphSearcher
{
public:
	GraphSearcherSpotTurn() = default;
	~GraphSearcherSpotTurn() = default;

	std::tuple<GraphSearchResult, RobotStateNode, int> SearchGraphTree(
		const std::vector<RobotStateNode>& graph,
		int graph_size,
		const TargetRobotState& target
	) const override;
};

#endif	// DESIGNLAB_GRAPH_SEARCHER_SPOT_TURN_H_