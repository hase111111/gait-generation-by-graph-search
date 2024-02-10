
//! @file      graph_searcher_straight_move.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_GRAPH_SEARCHER_STRAIGHT_MOVE_H_
#define DESIGNLAB_GRAPH_SEARCHER_STRAIGHT_MOVE_H_

#include <memory>
#include <tuple>
#include <string>
#include <vector>

#include "interface_graph_searcher.h"
#include "interface_hexapod_posture_validator.h"


namespace designlab
{

//! @class GraphSearcherStraightMove
//! @brief グラフ探索を行い，直進する動作を評価するクラス．
class GraphSearcherStraightMove final : public IGraphSearcher
{
    using Tag = GraphSearchEvaluationValue::Tag;

public:
    explicit GraphSearcherStraightMove(
        const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr);

    std::tuple<GraphSearchResult, GraphSearchEvaluationValue, RobotStateNode> SearchGraphTree(
        const GaitPatternGraphTree& graph,
        const RobotOperation& operation,
        const DividedMapState& divided_map_state,
        int max_depth) const override;

    std::tuple<GraphSearchResult, GraphSearchEvaluationValue, RobotStateNode> SearchGraphTreeVector(
        const std::vector<GaitPatternGraphTree>& graph_vector,
        const RobotOperation& operation,
        const DividedMapState& divided_map_state,
        int max_depth) const override;

private:
    static constexpr Tag kTagMoveForward = 0;
    static constexpr Tag kTagLegRot = 1;
    static constexpr Tag kTagStablyMargin = 2;
    static constexpr Tag kTagZDiff = 3;


    GraphSearchEvaluator InitializeEvaluator() const;

    float InitTargetZValue(const RobotStateNode& node,
                           const DividedMapState& devide_map_state,
                           const Vector3& move_direction) const;

    float GetMoveForwardEvaluationValue(
        const RobotStateNode& node,
        const RobotStateNode& root_node,
        const Vector3& normalized_move_direction) const;

    float GetLegRotEvaluationValue(
        const RobotStateNode& node,
        const RobotStateNode& root_node) const;

    float GetZDiffEvaluationValue(
        const std::vector<float>& com_trajectory,
        const float target_z_value) const;


    const std::shared_ptr<const IHexapodPostureValidator> checker_ptr_;

    GraphSearchEvaluator evaluator_;
};

}  // namespace designlab


#endif  // DESIGNLAB_GRAPH_SEARCHER_STRAIGHT_MOVE_H_
