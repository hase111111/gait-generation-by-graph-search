#pragma once

#include <memory>

#include "map_state.h"
#include "node.h"
#include "abstract_pass_finder.h"


//! @param PassFinderFixed
//! @data 2023/08/17
//! @author ’·’Jì
//! @brief Ä•]‰¿è–@
class PassFinderFixed final :public AbstractPassFinder
{
public:
	PassFinderFixed(std::unique_ptr<AbstractPassFinderFactory>&& factory) : AbstractPassFinder(std::move(factory)) {};
	~PassFinderFixed() = default;

	EGraphSearchResult getNextNodebyGraphSearch(const SNode& current_node, const MapState* const p_map, const STarget& target, SNode& output_node) override;
};



//! @file pass_finder_fixed.h
//! @data 2023/08/21
//! @author ’·’Jì
//! @brief Ä•]‰¿è–@‚ğs‚¤ƒNƒ‰ƒX
//! @n s” : @lineinfo
