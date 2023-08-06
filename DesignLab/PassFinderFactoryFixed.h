#pragma once
#include "AbstractPassFinderFactory.h"

class PassFinderFactoryFixed final : public AbstractPassFinderFactory
{
public:
	PassFinderFactoryFixed() = default;
	~PassFinderFactoryFixed() = default;

	void createGraphTreeCreator(const MapState* const map, std::unique_ptr<IGraphTreeCreator>& tree) override;

	void createGraphSearcher(std::unique_ptr<IGraphSearcher>& searcher) override;
};
