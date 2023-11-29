//! @file graph_searcher_straight_move.h
//! @brief 波東さんの手法でグラフ探索を行うクラスの実装．

#ifndef DESIGNLAB_GRAPH_SEARCHER_SPOT_TURN_H_
#define DESIGNLAB_GRAPH_SEARCHER_SPOT_TURN_H_

#include "interface_graph_searcher.h"

class GraphSearcherSpotTurn final : public IGraphSearcher
{
public:
	GraphSearcherSpotTurn() = default;
	~GraphSearcherSpotTurn() = default;

	std::tuple<GraphSearchResult, RobotStateNode, int> SearchGraphTree(
		const GaitPatternGraphTree& graph_tree,
		const TargetRobotState& target
	) const override;

private:
	//! @brief 前進するための評価値を計算する
	float CalcTurnEvaluationValue(const RobotStateNode& current_node, const TargetRobotState& target) const;

	//! @brief 脚の平均回転量の評価値を計算する
	float CalcLegRotEvaluationValue(const RobotStateNode& current_node, const RobotStateNode& parent_node) const;
};

#endif	// DESIGNLAB_GRAPH_SEARCHER_SPOT_TURN_H_