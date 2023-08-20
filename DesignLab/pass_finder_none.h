#pragma once

#include <memory>

#include "map_state.h"
#include "node.h"
#include "interface_pass_finder.h"


//! @class PassFinderNone
//! @date 2023/08/14
//! @author 長谷川
//! @brief グラフ探索を行わないクラス．Graphicクラスのデバッグを行う際に用いる．
class PassFinderNone final :public IPassFinder
{
public:
	PassFinderNone(std::unique_ptr<AbstractPassFinderFactory>&& _factory) : IPassFinder(std::move(_factory)) {};
	~PassFinderNone() = default;

	EGraphSearchResult getNextNodebyGraphSearch(const SNode& current_node, const MapState* const p_map, const STarget& target, SNode& output_node) override;
};


//! @file pass_finder_none.h 
//! @date 2023/08/21
//! @author 長谷川
//! @brief グラフ探索を行わないクラス
//! @n 行数 : @lineinfo
