#pragma once

#include <memory>

#include "map_state.h"
#include "Node.h"
#include "interface_pass_finder.h"


//! @param PassFinderFixed
//! @data 2023/08/17
//! @author ’·’Jì
//! @brief Ä•]‰¿è–@
class PassFinderFixed final :public IPassFinder
{
public:
	PassFinderFixed(std::unique_ptr<AbstractPassFinderFactory>&& factory) : IPassFinder(std::move(factory)) {};
	~PassFinderFixed() = default;

	EGraphSearchResult getNextNodebyGraphSearch(const SNode& current_node, const MapState* const p_map, const STarget& target, SNode& output_node) override;
};