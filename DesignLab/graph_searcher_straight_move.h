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

	static constexpr float kMaxEvaluationValue = 1000000.0f;
	static constexpr float kMinEvaluationValue = -1000000.0f;
	static constexpr float kMarginOfMove = 10;

	struct EvaluationValue final
	{
		int index{ -1 };
		float move_forward{ kMinEvaluationValue };
		float leg_rot{ kMinEvaluationValue };
		float stably_margin{ kMinEvaluationValue };
		float z_diff{ kMaxEvaluationValue };
	};

	bool UpdateEvaluationValueByAmoutOfMovement(int index, const GaitPatternGraphTree& tree, EvaluationValue* candiate) const;

	bool UpdateEvalutionValueByLegRot(int index, const GaitPatternGraphTree& tree, 
		const RobotOperation& operation, EvaluationValue* candiate);

	bool UpdateEvalutionValueByStablyMargin(int index, const GaitPatternGraphTree& tree,
		const RobotOperation& operation, EvaluationValue* candiate);

	bool UpdateEvalutionValueByZDiff(int index, const GaitPatternGraphTree& tree,
		const RobotOperation& operation, EvaluationValue* candiate);

	const std::shared_ptr<const IHexapodVaildChecker> checker_ptr_;

	EvaluationValue max_evaluation_value_;

	EvaluationValue candiate_evaluation_value_;

	::designlab::Vector3 no
};


#endif	// DESIGNLAB_GRAPH_SEARCHER_HATO_H_