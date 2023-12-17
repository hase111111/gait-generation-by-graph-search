//! @file graph_searcher_straight_move.h
//! @brief 波東さんの手法でグラフ探索を行うクラス．

#ifndef DESIGNLAB_GRAPH_SEARCHER_SPOT_TURN_H_
#define DESIGNLAB_GRAPH_SEARCHER_SPOT_TURN_H_

#include "interface_graph_searcher.h"


namespace designlab
{

class GraphSearcherSpotTurn final : public IGraphSearcher
{
public:
	GraphSearcherSpotTurn() = default;
	~GraphSearcherSpotTurn() = default;

	std::tuple<GraphSearchResult, int, int> SearchGraphTree(
		const GaitPatternGraphTree& graph_tree,
		const RobotOperation& operation,
		int max_depth
	) const override;

private:
	//! @brief 前進するための評価値を計算する
	float CalcTurnEvaluationValue(const RobotStateNode& current_node, const RobotOperation& operation) const;

	//! @brief 脚の平均回転量の評価値を計算する
	float CalcLegRotEvaluationValue(const RobotStateNode& current_node, const RobotStateNode& parent_node) const;
};

}	// namespace designlab


#endif	// DESIGNLAB_GRAPH_SEARCHER_SPOT_TURN_H_