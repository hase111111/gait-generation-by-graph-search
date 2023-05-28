#pragma once
#include "IGraphSearcher.h"


// 完全ランダムに次の動作を選んで返します．
class GraphSearcherRandom final : public IGraphSearcher
{
public:

	bool searchGraphTree(const std::vector<SNode>& _graph, const STarget& _target, SNode& _output_result) override;

private:

};
