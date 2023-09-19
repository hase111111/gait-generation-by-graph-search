#pragma once

#include "abstract_graph_searcher.h"


// 完全ランダムに次の動作を選んで返します．
class GraphSearcherRandom final : public AbstractGraphSearcher
{
public:

	GraphSearcherRandom(const std::shared_ptr<const AbstractHexapodStateCalculator>& calc) : AbstractGraphSearcher(calc) {}

	EGraphSearchResult searchGraphTree(const std::vector<SNode>& graph, const STarget& target, SNode* output_result) override;
};
