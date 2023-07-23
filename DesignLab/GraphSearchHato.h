#pragma once
#include "MapState.h"
#include "Node.h"
#include "InterfaceGraphTreeCreator.h"
#include "InterfaceGraphSearcher.h"
#include "InterfaceGraphSearch.h"
#include <memory>

class GraphSearchHato final : public IGraphSearch
{
public:
	GraphSearchHato() = default;
	~GraphSearchHato() = default;

	EGraphSearchResult getNextNodebyGraphSearch(const SNode& _current_node, const MapState* const _p_map, const STarget& _target, SNode& _output_node) override;

private:
	std::unique_ptr<IGraphTreeCreator> mp_GraphTreeCreator;
	std::unique_ptr<IGraphSearcher> mp_GraphSearcher;
};


//! @file GraphSearchHato.h 
//! @brief 波東さんの手法でグラフ探索を行うクラスの実装．
//! @author 長谷川
//! @date 2023/07/23

//! @class GraphSearchHato
//! @brief 波東先輩の手法で，グラフ探索を行うクラス．
//! @author 長谷川
//! @date 2023/07/23