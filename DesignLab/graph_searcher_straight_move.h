//! @file graph_searcher_straight_move.h
//! @brief グラフ探索を行うクラス.

#ifndef DESIGNLAB_GRAPH_SEARCHER_HATO_H_
#define DESIGNLAB_GRAPH_SEARCHER_HATO_H_

#include "interface_graph_searcher.h"
#include "interface_hexapod_vaild_checker.h"


//! @class GraphSearcherStraightMove
//! @brief グラフ探索を行うクラス．直進を評価する．
class GraphSearcherStraightMove final : public IGraphSearcher
{
public:

	GraphSearcherStraightMove(const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr);

	std::tuple<GraphSearchResult, int, int> SearchGraphTree(const GaitPatternGraphTree& graph, const RobotOperation& operation, int max_depth) const override;

private:

	static constexpr float kMarginOfMove = 10;

	struct EvaluationValue final
	{
		EvaluationValue() = delete;
		EvaluationValue(const RobotOperation& operation) : operation_(operation) {}

		static constexpr float kMaxEvaluationValue = 1000000.0f;
		static constexpr float kMinEvaluationValue = -1000000.0f;

		void Update(int new_index, const GaitPatternGraphTree& tree);

		const RobotOperation operation_;

		int index{ -1 };
		float move_forward{ kMinEvaluationValue };
		float leg_rot{ kMinEvaluationValue };
		float stably_margin{ kMinEvaluationValue };
		float z_diff{ kMaxEvaluationValue };

	private:
		float CalcMoveForwardEvaluationValue(int index, const GaitPatternGraphTree& tree) const;

	};

	//! @brief 前進するための評価値を計算する
	float CalcMoveFrowardEvaluationValue(const RobotStateNode& current_node, const RobotOperation& operation) const;

	//! @brief 脚の平均回転量の評価値を計算する
	float CalcLegRotEvaluationValue(const RobotStateNode& current_node, const RobotStateNode& parent_node) const;

	const std::shared_ptr<const IHexapodVaildChecker> checker_ptr_;
};


#endif	// DESIGNLAB_GRAPH_SEARCHER_HATO_H_