//! @file graph_searcher_straight_move.h
//! @brief グラフ探索を行うクラス.

#ifndef DESIGNLAB_GRAPH_SEARCHER_HATO_H_
#define DESIGNLAB_GRAPH_SEARCHER_HATO_H_

#include "interface_graph_searcher.h"
#include "interface_hexapod_vaild_checker.h"


//! @class GraphSearcherStraightMove
//! @brief グラフ探索を行い，直進する動作を評価するクラス．
class GraphSearcherStraightMove final : public IGraphSearcher
{
public:

	GraphSearcherStraightMove(const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr);

	std::tuple<GraphSearchResult, int, int> SearchGraphTree(const GaitPatternGraphTree& graph, const RobotOperation& operation, int max_depth) const override;

private:

	static constexpr float kMaxEvaluationValue = 1000000.0f;
	static constexpr float kMinEvaluationValue = -1000000.0f;


	enum class EvaluationResult : int
	{
		kUpdate,
		kEqual,
		kNotUpdate,
	};

	struct EvaluationValue final
	{
		int index{ -1 };
		float move_forward{ kMinEvaluationValue };
		float leg_rot{ kMinEvaluationValue };
		float stably_margin{ kMinEvaluationValue };
		float z_diff{ kMaxEvaluationValue };
	};

	struct InitialValue final
	{
		::designlab::Vector3 normalized_move_direction;
	};


	EvaluationResult UpdateEvaluationValueByAmoutOfMovement(int index, const GaitPatternGraphTree& tree, const EvaluationValue& max_evaluation_value, const InitialValue& init_value, EvaluationValue* candiate) const;

	EvaluationResult UpdateEvalutionValueByLegRot(int index, const GaitPatternGraphTree& tree, const EvaluationValue& max_evaluation_value, const InitialValue& init_value, EvaluationValue* candiate) const;

	EvaluationResult UpdateEvalutionValueByStablyMargin(int index, const GaitPatternGraphTree& tree, const EvaluationValue& max_evaluation_value, const InitialValue& init_value, EvaluationValue* candiate) const;

	EvaluationResult UpdateEvalutionValueByZDiff(int index, const GaitPatternGraphTree& tree, const EvaluationValue& max_evaluation_value, const InitialValue& init_value, EvaluationValue* candiate) const;


	const std::shared_ptr<const IHexapodVaildChecker> checker_ptr_;
};


#endif	// DESIGNLAB_GRAPH_SEARCHER_HATO_H_