//! @file graph_searcher_hato.h
//! @brief 波東さんの手法でグラフ探索を行うクラスの実装．

#ifndef DESIGNLAB_GRAPH_SEARCHER_HATO_H_
#define DESIGNLAB_GRAPH_SEARCHER_HATO_H_

#include "interface_graph_searcher.h"
#include "interface_hexapod_vaild_checker.h"


//! @class GraphSearcherHato
//! @brief 波東先輩の手法で，グラフ探索を行うクラス．
class GraphSearcherHato final : public IGraphSearcher
{
public:

	GraphSearcherHato(const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr);

	std::tuple<GraphSearchResult, RobotStateNode, int> SearchGraphTree(
		const GaitPatternGraphTree& graph,
		const TargetRobotState& target
	) const override;

private:

	//! @brief 前進するための評価値を計算する
	float CalcMoveFrowardEvaluationValue(const RobotStateNode& current_node, const TargetRobotState& target) const;

	//! @brief 脚の平均回転量の評価値を計算する
	float CalcLegRotEvaluationValue(const RobotStateNode& current_node, const RobotStateNode& parent_node) const;

	static constexpr float kMarginOfMove = 10;

	const std::shared_ptr<const IHexapodVaildChecker> checker_ptr_;
};


#endif	// DESIGNLAB_GRAPH_SEARCHER_HATO_H_