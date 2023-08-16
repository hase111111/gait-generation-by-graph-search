#pragma once

#include "abstract_pass_finder_factory.h"


//! @class PassFinderFactoryFixed
//! @date 2023/08/14
//! @auhtor ’·’Jì
//! @brief Ä•]‰¿è–@
class PassFinderFactoryFixed final : public AbstractPassFinderFactory
{
public:
	PassFinderFactoryFixed() = default;
	~PassFinderFactoryFixed() = default;

	void createGraphTreeCreator(const MapState* const map, std::unique_ptr<IGraphTreeCreator>& tree) override;

	void createGraphSearcher(std::unique_ptr<IGraphSearcher>& searcher) override;
};


//! @file pass_finder_factory_fixed.h
//! @date 2023/08/14
//! @auhtor ’·’Jì
//! @brief Ä•]‰¿è–@
//! @n s” : @lineinfo
