#pragma once
#include "map_state.h"
#include "Node.h"
#include "InterfacePassFinder.h"
#include <memory>

class PassFinderNone final :public IPassFinder
{
public:
	PassFinderNone(std::unique_ptr<AbstractPassFinderFactory>&& _factory) : IPassFinder(std::move(_factory)) {};
	~PassFinderNone() = default;

	EGraphSearchResult getNextNodebyGraphSearch(const SNode& current_node, const MapState* const p_map, const STarget& target, SNode& output_node) override;
};


//! @file PassFinderNone.h 
//! @brief グラフ探索を行わないクラスの実装．
//! @author 長谷川
//! @date 2023/07/24

//! @class PassFinderNone
//! @brief グラフ探索を行わないクラス．Graphicクラスのデバッグを行う際に用いる．
//! @author 長谷川
//! @date 2023/07/24
