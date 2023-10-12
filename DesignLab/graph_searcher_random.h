#pragma once

#include "interface_graph_searcher.h"


// 完全ランダムに次の動作を選んで返します．
class GraphSearcherRandom final : public IGraphSearcher
{
public:

	GraphSearchResult SearchGraphTree(const std::vector<RobotStateNode>& graph, const TargetRobotState& target, RobotStateNode* output_result) override;
};
