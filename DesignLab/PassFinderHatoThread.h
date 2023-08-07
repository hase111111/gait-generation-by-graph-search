#pragma once
#include "map_state.h"
#include "Node.h"
#include "InterfacePassFinder.h"
#include <memory>

class PassFinderHatoThread final : public IPassFinder
{
public:
	PassFinderHatoThread(std::unique_ptr<AbstractPassFinderFactory>&& _factory);
	~PassFinderHatoThread() = default;

	EGraphSearchResult getNextNodebyGraphSearch(const SNode& _current_node, const MapState* const _p_map, const STarget& _target, SNode& _output_node) override;
};
