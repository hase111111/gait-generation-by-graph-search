#pragma once
#include "AbstractPassFinderFactory.h"

class PassFinderFactoryHato final : public AbstractPassFinderFactory
{
public:
	PassFinderFactoryHato() = default;
	~PassFinderFactoryHato() = default;

	void createPassFinder(std::unique_ptr<IGraphTreeCreator>& _tree, std::unique_ptr<IGraphSearcher>& _searcher, const MapState* const _map) override;
};
