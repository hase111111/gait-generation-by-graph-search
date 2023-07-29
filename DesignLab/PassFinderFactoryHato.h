#pragma once
#include "AbstractPassFinderFactory.h"

class PassFinderFactoryHato final : public AbstractPassFinderFactory
{
public:
	PassFinderFactoryHato() = default;
	~PassFinderFactoryHato() = default;

	void createGraphTreeCreator(const MapState* const map, std::unique_ptr<IGraphTreeCreator>& tree) override;

	void createGraphSearcher(std::unique_ptr<IGraphSearcher>& searcher) override;
};
