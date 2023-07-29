#pragma once
#include "InterfaceGraphSearcher.h"


// 完全ランダムに次の動作を選んで返します．
class GraphSearcherRandom final : public IGraphSearcher
{
public:

	EGraphSearchResult searchGraphTree(const std::vector<SNode>& graph, const STarget& target, SNode* output_result) override;

};
