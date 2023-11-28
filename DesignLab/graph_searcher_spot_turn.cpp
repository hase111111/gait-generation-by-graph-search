#include "graph_searcher_spot_turn.h"

std::tuple<GraphSearchResult, RobotStateNode, int> GraphSearcherSpotTurn::SearchGraphTree(
    const GaitPatternGraphTree& graph_tree,
    const TargetRobotState& target
) const
{
    std::ignore = graph_tree;
    std::ignore = target;

    return std::tuple<GraphSearchResult, RobotStateNode, int>();
}
