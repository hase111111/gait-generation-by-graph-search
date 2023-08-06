#pragma once
#include "MapState.h"
#include "Node.h"
#include "InterfacePassFinder.h"
#include <memory>

class PassFinderFixed final :public IPassFinder
{
public:
	PassFinderFixed(std::unique_ptr<AbstractPassFinderFactory>&& _factory) : IPassFinder(std::move(_factory)) {};
	~PassFinderFixed() = default;

	EGraphSearchResult getNextNodebyGraphSearch(const SNode& current_node, const MapState* const p_map, const STarget& target, SNode& output_node) override;
};