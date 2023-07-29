#pragma once
#include "InterfaceGraphSearcher.h"


class GraphSearcherHato final : public IGraphSearcher
{
public:

	GraphSearcherHato();
	~GraphSearcherHato();

	EGraphSearchResult searchGraphTree(const std::vector<SNode>& graph, const STarget& target, SNode* output_result) override;

private:

	const float MARGIN_OF_MOVE = 10;
};

//! @file GraphSearcherHato
//! @brief 波東さんの手法でグラフ探索を行うクラスの実装．
//! @date 2023/07/23
//! @auther 長谷川

//! @class GraphSearcherHato
//! @brief 波東先輩の手法で，グラフ探索を行うクラス．
//! @date 2023/07/23
//! @auther 長谷川