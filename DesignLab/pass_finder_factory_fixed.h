#pragma once

#include "interface_pass_finder_factory.h"


//! @class PassFinderFactoryFixed
//! @date 2023/08/14
//! @author ’·’Jì
//! @brief Ä•]‰¿è–@
class PassFinderFactoryFixed final : public IPassFinderFactory
{
public:
	PassFinderFactoryFixed() = default;
	~PassFinderFactoryFixed() = default;

	void createGraphTreeCreator(const MapState* const map, std::shared_ptr<AbstractHexapodStateCalculator> calc, std::unique_ptr<IGraphTreeCreator>& tree) override;

	void createGraphSearcher(std::unique_ptr<AbstractGraphSearcher>& searcher, std::shared_ptr<AbstractHexapodStateCalculator> calc) override;
};


//! @file pass_finder_factory_fixed.h
//! @date 2023/08/14
//! @author ’·’Jì
//! @brief Ä•]‰¿è–@
//! @n s” : @lineinfo
