
//! @file      graph_searcher_straight_move.h
//! @author    hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_GRAPH_SEARCHER_STRAIGHT_MOVE_H_
#define DESIGNLAB_GRAPH_SEARCHER_STRAIGHT_MOVE_H_

#include <memory>
#include <tuple>
#include <string>

#include "interface_graph_searcher.h"
#include "interface_hexapod_vaild_checker.h"


namespace designlab
{

//! @class GraphSearcherStraightMove
//! @brief グラフ探索を行い，直進する動作を評価するクラス．
class GraphSearcherStraightMove final : public IGraphSearcher
{
public:
    explicit GraphSearcherStraightMove(
        const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr);

    std::tuple<GraphSearchResult, int, int> SearchGraphTree(
      const GaitPatternGraphTree& graph,
      const RobotOperation& operation,
      const DividedMapState& devide_map_state,
      int max_depth) const override;

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

        std::string ToString() const;
    };

    struct InitialValue final
    {
        Vector3 normalized_move_direction;

        float target_z_value;
    };

    float InitTargetZValue(const RobotStateNode& node,
                           const DividedMapState& devide_map_state,
                           const Vector3& move_direction) const;

    EvaluationResult UpdateEvaluationValueByAmountOfMovement(
        int index,
        const GaitPatternGraphTree& tree,
        const EvaluationValue& max_evaluation_value,
        const InitialValue& init_value,
        EvaluationValue* candiate) const;

    EvaluationResult UpdateEvaluationValueByLegRot(
        int index,
        const GaitPatternGraphTree& tree,
        const EvaluationValue& max_evaluation_value,
        const InitialValue& init_value,
        EvaluationValue* candiate) const;

    EvaluationResult UpdateEvaluationValueByStablyMargin(
        int index,
        const GaitPatternGraphTree& tree,
        const EvaluationValue& max_evaluation_value,
        const InitialValue& init_value,
        EvaluationValue* candiate) const;

    EvaluationResult UpdateEvaluationValueByZDiff(
        int index,
        const GaitPatternGraphTree& tree,
        const EvaluationValue& max_evaluation_value,
        const InitialValue& init_value,
        EvaluationValue* candiate) const;


    const std::shared_ptr<const IHexapodVaildChecker> checker_ptr_;
};

}  // namespace designlab


#endif  // DESIGNLAB_GRAPH_SEARCHER_STRAIGHT_MOVE_H_
