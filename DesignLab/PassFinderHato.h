#pragma once
#include "MapState.h"
#include "Node.h"
#include "InterfacePassFinder.h"
#include <memory>

class PassFinderHato final : public IPassFinder
{
public:
	PassFinderHato(std::unique_ptr<AbstractPassFinderFactory>&& _factory) : IPassFinder(std::move(_factory)) {};
	~PassFinderHato() = default;

	EGraphSearchResult getNextNodebyGraphSearch(const SNode& _current_node, const MapState* const _p_map, const STarget& _target, SNode& _output_node) override;

private:

};


//! @file PassFinderHato.h 
//! @brief 波東さんの手法でグラフ探索を行うクラスの実装．
//! @author 長谷川
//! @date 2023/07/24

//! @class PassFinderHato
//! @brief 波東先輩の手法で，グラフ探索を行うクラス．
//! @author 長谷川
//! @date 2023/07/24